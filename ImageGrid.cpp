#include "ImageGrid.h"
#include "interface.h"

ImageGrid::ImageGrid() : QWidget() {
	deleteMenu = NULL;
	filesList = new QStringList();
	dragStatus = NULL;
	//count = 0;
	imgGridLayout = new QHBoxLayout();//ImageGridLayout();
	imgGridLayout->setAlignment(Qt::AlignLeft);
	//imgGridLayout->setSizeConstraint(QLayout::SetFixedSize);
	imgGridLayout->setSpacing(20);
	setLayout(imgGridLayout);	
	setAcceptDrops(true);		

	imgList = new QList<QImage*>();

	containerSize = 200;

	container = NULL;

	tmpLabel = new QLabel("Insert here");
	tmpLabel->setVisible(false);
	tmpLabel->setAlignment(Qt::AlignCenter);
	
	//hasPano = false;
	prevSelectedLabel = NULL;

	
}

void ImageGrid::mouseMoveEvent(QMouseEvent *e) {
	if(dragStatus) {				
		if(!dragImg) {
			int ind = imgGridLayout->indexOf(dragStatus);
			dragStatus->lower();
			tmpIndex = ind;
			dragImg = imgList->takeAt(ind);			
			tmpLabel->setFixedSize(containerSize, (int)(containerSize*18/20.0));
			tmpLabel->setVisible(true);
			imgGridLayout->insertWidget(tmpIndex, tmpLabel);
			imgGridLayout->removeWidget(dragStatus);
			tmpFile = filesList->takeAt(ind);
			QRect r = geometry();
			if(r.x() < 0) {
				int w = r.width();
				r.setX(0);
				r.setWidth(w);
			}
			imgGridLayout->setGeometry(r);					
		}	
		dragStatus->move(e->x() - dragWidgetXcorr, dragStatus->y());//me->y() - dragWidgetYcorr);
		/*if(e->x() > container->width()-3) {
			QScrollBar* sb = container->horizontalScrollBar();
			int val = sb->value();
			if(val != sb->maximum()) {
				sb->setValue(val+sb->pageStep());
			}
		}*/
		QWidget* child;
		if(((child = childAt(e->x() - dragWidgetXcorr, dragStatus->y())) && (child != tmpLabel))) {// || ((child = childAt(e->x() - dragWidgetXcorr + containerSize + 25, dragStatus->y()))/* && (child != tmpLabel)*/)) {
			if(child != dragStatus) {
				int ind = imgGridLayout->indexOf(child);
				if(tmpIndex != ind) {				
					if(tmpIndex != -1) {
						imgGridLayout->removeWidget(tmpLabel);
					}	
					tmpIndex = ind;					
					//tmpLabel->setFixedSize(containerSize, (int)(containerSize*19/20.0));
					imgGridLayout->insertWidget(tmpIndex, tmpLabel);
					QRect r = geometry();
					if(r.x() < 0) {
						int w = r.width();
						r.setX(0);
						r.setWidth(w);
					}
					imgGridLayout->setGeometry(r);					
				}
			}/* else {
				if(!imgGridLayout->count() || (imgGridLayout->itemAt(tmpIndex)->widget() != tmpLabel)) {					
					imgGridLayout->insertWidget(tmpIndex, tmpLabel);
					imgGridLayout->setGeometry((QRect)geometry());
				}
			}*/
		}
	}
}

void ImageGrid::mousePressEvent(QMouseEvent *e) {
	QWidget* child = NULL;
	if(child = childAt(e->pos())) {
		if(prevSelectedLabel) {
			prevSelectedLabel->setFrameStyle(QFrame::NoFrame);
		}
		((QLabel*)child)->setFrameStyle(QFrame::Box);
		((QLabel*)child)->setLineWidth(3);
		prevSelectedLabel = (QLabel*)child;
		QImage* i = imgList->at(imgGridLayout->indexOf(child));
		emit imgSelected(i);
		//Interface::instance()->showImg(i);
	}
	if(e->button() == Qt::LeftButton) {
		dragStatus = child;
	}
	if(dragStatus) {
		dragImg = NULL;
		tmpIndex = -1;
		dragWidgetXcorr = e->x() - dragStatus->x();
		//dragWidgetYcorr = me->y() - dragStatus->y();
	}
}

/*void ImageGrid::mouseReleaseEvent(QMouseEvent *me) {
	dragStatus = NULL;
}*/

/*ImageGrid::ImageGrid(QStringList *filesList) : QWidget() {
	deleteMenu = NULL;
	//dragStatus = NULL;
	//count = 0;
	imgGridLayout = new ImageGridLayout();
	setLayout(imgGridLayout);
	addFiles(filesList);
	setAcceptDrops(true);
}*/

void ImageGrid::addFiles(QStringList& filesList) {
	//QWidget* df = parentWidget();
	
	this->filesList;//->append(filesList);
	QStringListIterator iterator(filesList);	
	while(iterator.hasNext()) {
		QString tmp = iterator.next();
		if(!this->filesList->contains(tmp)) {
			this->filesList->append(tmp);
			QImage* im = new QImage();			
			im->load(tmp);
			imgList->append(im);
			QImage ti = im->scaled(containerSize, (int)(containerSize*18/20.0), Qt::KeepAspectRatio);
			//delete im;
			QPixmap img = QPixmap::fromImage(ti);			
			QLabel* label = new QLabel();
			label->setFixedSize(containerSize, (int)(containerSize*18/20.0));
			label->setAlignment(Qt::AlignCenter);
			label->setPixmap(img);			
			imgGridLayout->addWidget(label);
		}
		//count++;
	}	
	QRect r = geometry();
	if(r.x() < 0) {
		int w = r.width();
		r.setX(0);
		r.setWidth(w);
	}
	imgGridLayout->setGeometry(r);		
	emit fileAdded();
	//setFixedSize(imgGridLayout->sizeHint());
}

ImageGrid::~ImageGrid() {
	delete imgGridLayout;
	if(deleteMenu) {
		//delete deleteAction;
		delete deleteMenu;
	}
	delete filesList;
}

bool ImageGrid::event(QEvent* e) {	
	if((e->type() == QEvent::LayoutRequest) || (e->type() == QEvent::Resize)) {
		
		QRect geom = geometry();
		containerSize = geom.height();
		QListIterator<QImage*> iter(*imgList);
		int i = 0;		
		while(iter.hasNext()) {
		
			QLabel* label = (QLabel*)(imgGridLayout->itemAt(i)->widget());
			if(label == tmpLabel) {
				i++;
				continue;
			}
			QImage* im = iter.next();
			QImage ti = im->scaled(containerSize, (int)(containerSize*18/20.0), Qt::KeepAspectRatio);			
			QPixmap img = QPixmap::fromImage(ti);						
			label->setFixedSize(containerSize, (int)(containerSize*18/20.0));
			label->setAlignment(Qt::AlignCenter);
			label->setPixmap(img);									
			i++;
		}
		QRect r = geometry();
		if(r.x() < 0) {
			int w = r.width();
			r.setX(0);
			r.setWidth(w);
		}
		imgGridLayout->setGeometry(r);						
		QSize s = imgGridLayout->sizeHint();
		if(!container) {
			container = (QScrollArea*)parentWidget();
		}
		setFixedSize(container->width()>s.width()?container->width():s.width(), containerSize);		
	}
	return QWidget::event(e);
}

QStringList* ImageGrid::getFilesList() {
	return filesList;
}

void ImageGrid::contextMenuEvent(QContextMenuEvent* e) {
	if(!deleteMenu) {
		deleteMenu = new QMenu();
		deleteAction = deleteMenu->addAction("Delete");
		moveLeftAction = deleteMenu->addAction("Move left");
		moveRightAction = deleteMenu->addAction("Move right");		
	}
	QWidget* child;
	if(child = childAt(e->pos())) {		
		int ind = imgGridLayout->indexOf(child);			
		moveLeftAction->setEnabled(true);
		moveRightAction->setEnabled(true);
		if(ind == 0) {
			moveLeftAction->setEnabled(false);
		} 
		if(ind == imgGridLayout->count()-1) {
			moveRightAction->setEnabled(false);
		}
		QAction* res = deleteMenu->exec(e->globalPos());
		if(res == deleteAction) { 
			deleteCurrent();			
			//Interface::instance()->showImg(NULL);
		}
		if(res == moveLeftAction) {
			moveLeftCurrent();
		}
		if(res == moveRightAction) {
			moveRightCurrent();
		}
		QRect r = geometry();
		if(r.x() < 0) {
			int w = r.width();
			r.setX(0);
			r.setWidth(w);
		}
		imgGridLayout->setGeometry(r);	
			
	}
}

void ImageGrid::dropEvent(QDropEvent* e) {
	QList<QUrl> l = e->mimeData()->urls();
	QListIterator<QUrl> iter(l);
	while(iter.hasNext()) {
		QString filepath = iter.next().path();
		addFile(filepath.right(filepath.size()-1));
	}			
	QRect r = geometry();
	if(r.x() < 0) {
		int w = r.width();
		r.setX(0);
		r.setWidth(w);
	}
	imgGridLayout->setGeometry(r);						
}

void ImageGrid::dragEnterEvent(QDragEnterEvent* e) {
	if(e->mimeData()->hasUrls()) {
		e->acceptProposedAction();		
	}
}

void ImageGrid::addFile(QString& file) {
	filesList->append(file);
	QImage* im = new QImage();
	im->load(file);
	imgList->append(im);
	QImage ti = im->scaled(containerSize, (int)(containerSize*18/20.0), Qt::KeepAspectRatio);	
	QPixmap img = QPixmap::fromImage(ti);	
	QLabel* label = new QLabel();	
	label->setFixedSize(containerSize, (int)(containerSize*18/20.0));	
	label->setAlignment(Qt::AlignCenter);
	label->setPixmap(img);
	imgGridLayout->addWidget(label);
	emit fileAdded();
	//Interface::instance()->filesAdded();
	//imgGridLayout->setGeometry((QRect)geometry());			
	//setFixedSize(imgGridLayout->sizeHint());
}

/*void ImageGrid::setPano(QImage* img) {	
	if(img) {
		hasPano = true;
		QImage ti = img->scaled(200, 250, Qt::KeepAspectRatio);
		QPixmap* img = new QPixmap();
		img->convertFromImage(ti);
		QLabel* label = new QLabel();
		label->setPixmap(*img);
		imgGridLayout->addWidget(label);	
	}	
}*/

void ImageGrid::mouseReleaseEvent(QMouseEvent* e) {	
	if(dragStatus) {
		tmpLabel->setVisible(false);
		if(tmpIndex != -1) {
			imgGridLayout->takeAt(tmpIndex);
			imgGridLayout->insertWidget(tmpIndex, dragStatus);
			imgList->insert(tmpIndex, dragImg);
			filesList->insert(tmpIndex, tmpFile);
			QRect r = geometry();
			if(r.x() < 0) {
				int w = r.width();
				r.setX(0);
				r.setWidth(w);
			}
			imgGridLayout->setGeometry(r);	
			emit fileAdded();
		}
		//dragStatus->move(dragWidgetXcorr, me->y() - dragWidgetYcorr);
		dragStatus = NULL;
		dragImg = NULL;
	}
	
}

void ImageGrid::removeSelection() {
	if(prevSelectedLabel) {
		prevSelectedLabel->setFrameStyle(QFrame::NoFrame);
	}
}

QList<QImage*>* ImageGrid::getImgList() {
	return imgList;
}

QString ImageGrid::getCurrentFileName() {
	if(prevSelectedLabel) {
		return filesList->at(imgGridLayout->indexOf(prevSelectedLabel));
	} else {
		return QString();
	}
}

int ImageGrid::getCurrentId() {
	if(prevSelectedLabel) {
		return imgGridLayout->indexOf(prevSelectedLabel);
	} else {
		return -1;
	}
}

void ImageGrid::deleteCurrent() {
	int ind = getCurrentId();
	QWidget* child = imgGridLayout->itemAt(ind)->widget();
	filesList->removeAt(ind);		
	imgList->removeAt(ind);
	imgGridLayout->removeWidget(child);
	delete child;							
	prevSelectedLabel = NULL;
	emit fileDeleted();
}

void ImageGrid::moveLeftCurrent() {
	int ind = getCurrentId();
	QImage* tmpi = imgList->takeAt(ind-1);
	QString tmps = filesList->takeAt(ind-1);
	QLabel* tmpl = (QLabel*)imgGridLayout->takeAt(ind-1)->widget();
	imgList->insert(ind, tmpi);
	filesList->insert(ind, tmps);
	imgGridLayout->insertWidget(ind, tmpl);	
	emit fileAdded();
}

void ImageGrid::moveRightCurrent() {
	int ind = getCurrentId();
	QImage* tmpi = imgList->takeAt(ind);
	QString tmps = filesList->takeAt(ind);
	QLabel* tmpl = (QLabel*)imgGridLayout->takeAt(ind)->widget();
	imgList->insert(ind+1, tmpi);
	filesList->insert(ind+1, tmps);
	imgGridLayout->insertWidget(ind+1, tmpl);					
	emit fileAdded();
}

void ImageGrid::select(int n) {
	QWidget* child = imgGridLayout->itemAt(n)->widget();
	if(prevSelectedLabel) {
		prevSelectedLabel->setFrameStyle(QFrame::NoFrame);
	}
	((QLabel*)child)->setFrameStyle(QFrame::Box);
	((QLabel*)child)->setLineWidth(3);
	prevSelectedLabel = (QLabel*)child;
	QImage* i = imgList->at(n);
	emit imgSelected(i);
}