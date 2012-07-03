#include "ImageGridLayout.h"

ImageGridLayout::ImageGridLayout() : QLayout() {
	listWidgets = new QList<QLayoutItem*>();
	//listGeoms = new QList<QRect*>();
}

ImageGridLayout::~ImageGridLayout() {
	QLayoutItem* item;
	
		while(item=takeAt(0)) {
			delete item;
		}
		/*QRect* r;
		while(r=listGeoms->takeAt(0)) {
			delete r;
		}*/
	
}

void ImageGridLayout::addItem(QLayoutItem *item) {
	listWidgets->append(item);
	//listGeoms->append(NULL);
}

QLayoutItem* ImageGridLayout::itemAt(int pos) const {
	return listWidgets->value(pos);
}

QLayoutItem* ImageGridLayout::takeAt(int pos) {
	if((pos >= 0) && (pos < listWidgets->size())) {
		//listGeoms->takeAt(pos);
		return listWidgets->takeAt(pos);
	} else {
		return NULL;
	}
}

int ImageGridLayout::count() const {
	return listWidgets->size();
}

void ImageGridLayout::setGeometry(QRect &rect) {
	QLayout::setGeometry(rect);
	if(!listWidgets->size()) {
		return;
	}

	/*for(int i = 0; i < listWidgets->size(); i++) {
		QLayoutItem* o = listWidgets->at(i);		

	}*/

	/*for (int i = 0; i < listWidgets->size(); i++) {
		QLayoutItem* o = listWidgets->at(i);		
		QSize s = o->sizeHint();
		o->setGeometry(QRect(rect.x(),rect.y(),s.width(),s.height()));
	}*/
	int x = 0;
	int y = 0;
	//int line = 0;
	//int col = 0;
	int ymax = -1;
	for (int i = 0; i < listWidgets->size(); i++) {
		QLayoutItem* o = listWidgets->at(i);		
		//if(!listGeoms->at(i)) {                                              
			QSize s = o->sizeHint();		
			int width = s.width();
			int height = s.height();
			QRect* qr;
			if((x == 0) || (x + width <= rect.width())) {
				qr = new QRect(rect.x()+x, rect.y()+y, width, height);
				o->setGeometry(*qr);
				x += width+spacing();
				//col++;
				if(height > ymax) {
					ymax = height;
				}
			} else {
				//col = 0;
				//line++;
				y += ymax + spacing();								
				qr = new QRect(rect.x(), rect.y()+y, width, height);
				x=width+spacing();
				ymax=height;
				o->setGeometry(*qr);
			}
			//listGeoms->replace(i, qr);
		/*} else {
			QRect* qr = listGeoms->at(i);
			o->setGeometry(*qr);
		}*/
	}
}

QSize ImageGridLayout::sizeHint() const {
	QWidget* scroll = parentWidget()->parentWidget();	
	QSize s = scroll->size();
	int n = listWidgets->size();
	/*if (n > 0) {
		s = QSize(100,100); 	
	}*/
	int wwidth = s.width();
	int wheight = s.height();
	int x = 0;
	int y = 0;
	int ymax = -1;
	int width;
	int height;
			
	for (int i = 0; i < listWidgets->size(); i++) {
		QLayoutItem* o = listWidgets->at(i);		
		
			QSize s = o->sizeHint();		
			width = s.width();
			height = s.height();	
			if((x == 0) || (x + width <= wwidth)) {				
				x += width+spacing();
		
				if(height > ymax) {
					ymax = height;
				}
			} else {
				y += ymax + spacing();												
				x=width+spacing();
				ymax=height;				
			}
		
	}
	if(y+height > wheight) {
		wheight = y+height;
	}
	return QSize(wwidth, wheight);
}

QSize ImageGridLayout::minimumSize() const {	
	QWidget* scroll = parentWidget()->parentWidget();	
	QSize s = scroll->size();
	int n = listWidgets->size();
	/*if (n > 0) {
		s = QSize(100,100); 	
	}*/
	int wwidth = s.width();
	int wheight = s.height();
	int x = 0;
	int y = 0;
	int ymax = -1;
	int width;
	int height;
			
	for (int i = 0; i < listWidgets->size(); i++) {
		QLayoutItem* o = listWidgets->at(i);		
		
			QSize s = o->minimumSize();		
			width = s.width();
			height = s.height();	
			if((x == 0) || (x + width <= wwidth)) {				
				x += width+spacing();
		
				if(height > ymax) {
					ymax = height;
				}
			} else {
				y += ymax + spacing();												
				x=width+spacing();
				ymax=height;				
			}
		
	}
	if(ymax+height > wheight) {
		wheight = ymax+height;
	}
	return QSize(wwidth, wheight);
}


int ImageGridLayout::indexOf(QWidget* w) {
	QListIterator<QLayoutItem*> iter(*listWidgets);
	int i = 0;
	while(iter.hasNext()) {
		if(iter.next()->widget() == w) {
			return i;
		}
		i++;
	}
	return i;
}