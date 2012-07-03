#include "interface.h"
#include "Helper.h"


Interface::Interface(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setWindowTitle("BioTool interface alpha");
	
	settings = new QSettings("config.ini", QSettings::IniFormat);
	if(!settings->contains("Path")) {
		settings->setValue("Path", "C:/");
	}
	dirpath = settings->value("Path").toString();

	recentFiles = new QStringList();
		int size = settings->beginReadArray("Recent");
		for(int i = 0; i<size; i++) {
			settings->setArrayIndex(i);
			recentFiles->append(settings->value("filepath").toString());
		}
		settings->endArray();
	
	QWidget* userarea = centralWidget();
	if(!userarea) {
		userarea = new QWidget();
		setCentralWidget(userarea);
	}

	panoramaCreatorLaunched = false;

	QHBoxLayout* startLayout = new QHBoxLayout();
			
	QVBoxLayout* recentLayout = new QVBoxLayout();

	QStandardItemModel* modelFunName = new QStandardItemModel(30, 1, this);
	QItemSelectionModel* selModelName = new QItemSelectionModel(modelFunName);
	
	QLabel* recentLabel = new QLabel("Recent files:");
	recentLayout->addWidget(recentLabel);
	recentView = new QListWidget();
	recentView->setSelectionModel(selModelName);
	recentView->addItems(*recentFiles);
	recentView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	recentLayout->addWidget(recentView);	
	QPushButton* loadRecentButton = new QPushButton("Load selected files");
	recentLayout->addWidget(loadRecentButton);
	startLayout->insertLayout(0, recentLayout);

	connect(loadRecentButton, SIGNAL(clicked()), this, SLOT(loadRecent()));

	QVBoxLayout* toolsLayout = new QVBoxLayout();
	toolsLayout->setAlignment(Qt::AlignTop);
	QLabel* toolsLabel = new QLabel("Tools:");
	toolsLayout->addWidget(toolsLabel);
	QPushButton* panoramaCreatorButton = new QPushButton("Launch Panorama creator");
	toolsLayout->addWidget(panoramaCreatorButton);
	QPushButton* counterButton = new QPushButton("Launch Organic elements counter (NYI)");
	toolsLayout->addWidget(counterButton);
	counterButton->setEnabled(false);
	startLayout->insertLayout(0, toolsLayout);

	connect(panoramaCreatorButton, SIGNAL(clicked()), this, SLOT(launchPanoramaCreator()));

	userarea->setLayout(startLayout);
	
	QMenuBar* mb = menuBar();

	QMenu* fileMenu = mb->addMenu("File");
	openAction = fileMenu->addAction("Load image(s)...");	
	saveAction = fileMenu->addAction("Save current image");
	saveAction->setEnabled(false);
	savePanoAction = fileMenu->addAction("Save panorama");
	savePanoAction->setEnabled(false);
	QAction* exitAction = fileMenu->addAction("Exit");

	connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openFilesDialog()));
	connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(openSaveDialog()));
	connect(savePanoAction, SIGNAL(triggered(bool)), this, SLOT(openPanoSaveDialog()));
	connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(quit()));

	QMenu* editMenu = mb->addMenu("Edit");
	deleteAction = editMenu->addAction("Delete current image");
	deleteAction->setEnabled(false);
	moveLeftAction = editMenu->addAction("Move current image left");
	moveLeftAction->setEnabled(false);
	moveRightAction = editMenu->addAction("Move current image right");
	moveRightAction->setEnabled(false);
	brightnessUpAction = editMenu->addAction("+Brightness");
	brightnessUpAction->setEnabled(false);
	brightnessDownAction = editMenu->addAction("-Brightness");
	brightnessDownAction->setEnabled(false);
	contrastUpAction = editMenu->addAction("+Contrast");
	contrastUpAction->setEnabled(false);
	contrastDownAction = editMenu->addAction("-Contrast");
	contrastDownAction->setEnabled(false);
	scaleUpAction = editMenu->addAction("+Scale");
	scaleUpAction->setEnabled(false);
	scaleDownAction = editMenu->addAction("-Scale");
	scaleDownAction->setEnabled(false);
	setGrayAction = editMenu->addAction("Make grayscale");
	setGrayAction->setCheckable(true);
	setGrayAction->setEnabled(false);
	resetAction = editMenu->addAction("Reset changes");
	resetAction->setEnabled(false);
	applyAction = editMenu->addAction("Apply changes to current image");
	applyAction->setEnabled(false);
	applyToAllAction = editMenu->addAction("Apply changes to all images");
	applyToAllAction->setEnabled(false);
	
	connect(brightnessUpAction, SIGNAL(triggered(bool)), this, SLOT(brightnessUp()));	
	connect(brightnessDownAction, SIGNAL(triggered(bool)), this, SLOT(brightnessDown()));	
	connect(contrastUpAction, SIGNAL(triggered(bool)), this, SLOT(contrastUp()));	
	connect(contrastDownAction, SIGNAL(triggered(bool)), this, SLOT(contrastDown()));	
	connect(scaleUpAction, SIGNAL(triggered(bool)), this, SLOT(scaleUp()));	
	connect(scaleDownAction, SIGNAL(triggered(bool)), this, SLOT(scaleDown()));	
	connect(setGrayAction, SIGNAL(triggered(bool)), this, SLOT(setGray(bool)));	
	connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(resetImage()));	
	connect(applyAction, SIGNAL(triggered(bool)), this, SLOT(applyImage()));	
	connect(applyToAllAction, SIGNAL(triggered(bool)), this, SLOT(applyAllImages()));	

	toolsMenu = mb->addMenu("Tools");
	panoramaCreatorAction = toolsMenu->addAction("Create panorama");
	counterAction = toolsMenu->addAction("Count organic elements (NYI)");
	counterAction->setEnabled(false);

	connect(panoramaCreatorAction, SIGNAL(triggered(bool)), this, SLOT(launchPanoramaCreator()));



	filesListMenu = mb->addMenu("Files list");

	ag = new QActionGroup(filesListMenu);
	ag->setExclusive(true);


	connect(filesListMenu, SIGNAL(triggered(QAction*)), this, SLOT(fileSelected(QAction*)));	

	imgGridPanoSize = 250;
	pano = NULL;
	
	layout = new QVBoxLayout();	
	QHBoxLayout* imgViewConfig = new QHBoxLayout();
	QFrame* imgConfigFrame = new QFrame();
	QVBoxLayout* imgConfigFrameLayout = new QVBoxLayout();
	imgConfigFrameLayout->setAlignment(Qt::AlignTop);
	brightnessLabel = new QLabel("Brightness: 0%");
	brightness = 0;
	imgConfigFrameLayout->addWidget(brightnessLabel);
	brightnessSlider = new QSlider(Qt::Horizontal);
	brightnessSlider->setTracking(true);
	brightnessSlider->setRange(-100, 100);
	brightnessSlider->setValue(0);
	brightnessSlider->setSingleStep(10);
	brightnessSlider->setEnabled(false);
	imgConfigFrameLayout->addWidget(brightnessSlider);
	contrastLabel = new QLabel("Contrast: 1.0");
	contrast = 1;
	imgConfigFrameLayout->addWidget(contrastLabel);
	contrastSlider = new QSlider(Qt::Horizontal);
	contrastSlider->setTracking(true);
	contrastSlider->setRange(1, 190);
	contrastSlider->setValue(100);
	contrastSlider->setSingleStep(10);
	contrastSlider->setEnabled(false);
	imgConfigFrameLayout->addWidget(contrastSlider);
	isGray = new QCheckBox("Make grayscale");
	gray = false;
	isGray->setEnabled(false);
	imgConfigFrameLayout->addWidget(isGray);
	scaleLabel = new QLabel("Scale: 100%");
	scale = 100;
	imgConfigFrameLayout->addWidget(scaleLabel);
	scaleSlider = new QSlider(Qt::Horizontal);	
	scaleSlider->setTracking(true);
	scaleSlider->setRange(25, 500);
	scaleSlider->setValue(100);
	scaleSlider->setSingleStep(25);
	scaleSlider->setEnabled(false);
	imgConfigFrameLayout->addWidget(scaleSlider);		
	resetButton = new QPushButton("Reset parameters");
	imgConfigFrameLayout->addWidget(resetButton);	
	applyButton = new QPushButton("Apply changes to origin");
	imgConfigFrameLayout->addWidget(applyButton);	
	applyToAllButton = new QPushButton("Apply changes to all images");
	imgConfigFrameLayout->addWidget(applyToAllButton);		
	resetButton->setEnabled(false);
	applyButton->setEnabled(false);
	applyToAllButton->setEnabled(false);
	saveShowButton = new QPushButton("Save this image");
	saveShowButton->setEnabled(false);
	imgConfigFrameLayout->addWidget(saveShowButton);		
	imgConfigFrame->setLayout(imgConfigFrameLayout);
	imgConfigFrame->setFixedWidth(200);

	connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessChanged(int)));
	connect(contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(contrastChanged(int)));
	connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(scaleChanged(int)));
	connect(isGray, SIGNAL(stateChanged(int)), this, SLOT(grayButtonClicked(int)));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetImage()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyImage()));
	connect(applyToAllButton, SIGNAL(clicked()), this, SLOT(applyAllImages()));
	connect(saveShowButton, SIGNAL(clicked()), this, SLOT(openSaveDialog()));

	imgViewConfig->addWidget(imgConfigFrame);
	scrollImgViewArea = new QScrollArea();
	imgViewConfig->addWidget(scrollImgViewArea);
	layout->insertLayout(0, imgViewConfig);
	imgLabel = new QLabel();
	scrollImgViewArea->setAlignment(Qt::AlignCenter);
	scrollImgViewArea->setWidget(imgLabel);
	panoLabel = new QLabel();
	panoLabel->setFixedWidth(imgGridPanoSize);
	panoLabel->setVisible(false);
	panoLabel->setFrameStyle(QFrame::Box);
	panoLabel->setAlignment(Qt::AlignCenter);
    loadButton = new QPushButton(QIcon("Add folder.png"), "Load files");
	composeButton = new QPushButton(QIcon("kuba_icon_ok.png"), "Stitch");
	composeButton->setEnabled(false);
	saveButton = new QPushButton(QIcon("Save.png"), "Save pano");
	saveButton->setEnabled(false);

	connect(loadButton, SIGNAL(clicked()), this, SLOT(openFilesDialog()));	
	connect(composeButton, SIGNAL(clicked()), this, SLOT(compose()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(openPanoSaveDialog()));

	imgGrid = new ImageGrid();
	imgGridPano = new QFrame();
	QHBoxLayout* imgGridPanoLayout = new QHBoxLayout();		
	QScrollArea* scrollImgGrid = new QScrollArea();		
	scrollImgGrid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollImgGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);	
	scrollImgGrid->setWidget(imgGrid);
	imgGridPano->setFixedHeight(imgGridPanoSize);
	imgGridPanoLayout->addWidget(scrollImgGrid);
	imgGridPanoLayout->addWidget(panoLabel);	
	imgGridPano->setLayout(imgGridPanoLayout);
	layout->addWidget(imgGridPano);
	QFrame* buttonsFrame = new QFrame();	
	QHBoxLayout* buttonsLayout = new QHBoxLayout();	
	buttonsLayout->addWidget(loadButton);
	buttonsLayout->addWidget(composeButton);	
	buttonsLayout->addWidget(saveButton);
	buttonsFrame->setFixedSize(375, 50);
	buttonsFrame->setLayout(buttonsLayout);	
	layout->addWidget(buttonsFrame);

	connect(this, SIGNAL(panoSelected()), imgGrid, SLOT(removeSelection()));
	connect(imgGrid, SIGNAL(fileAdded()), this, SLOT(filesAdded()));
	connect(imgGrid, SIGNAL(imgSelected(QImage*)), this, SLOT(showImg(QImage*)));
	connect(imgGrid, SIGNAL(fileDeleted()), this, SLOT(fileDeleted()));

	connect(deleteAction, SIGNAL(triggered(bool)), imgGrid, SLOT(deleteCurrent()));
	connect(moveLeftAction, SIGNAL(triggered(bool)), this, SLOT(moveLeft()));
	connect(moveRightAction, SIGNAL(triggered(bool)), this, SLOT(moveRight()));

	saveDialog = new QDialog(this);
	saveDialog->setWindowTitle("Save file...");
	QVBoxLayout* dialogLayout = new QVBoxLayout();
	QFrame* fileNameSelectFrame = new QFrame();
	QHBoxLayout* fileNameSelectLayout = new QHBoxLayout();
	fileNameLabel = new QLabel();
	fileNameSelectLayout->addWidget(fileNameLabel);
	QPushButton* selectFile = new QPushButton("...");	
	selectFile->setFixedWidth(30);
	fileNameSelectLayout->addWidget(selectFile);
	fileNameSelectFrame->setLayout(fileNameSelectLayout);
	dialogLayout->addWidget(fileNameSelectFrame);
	QLabel* fileTypeLabel = new QLabel("Select file type:");
	dialogLayout->addWidget(fileTypeLabel);
	fileTypeBox = new QComboBox();
	fileTypeBox->addItem(".jpg");
	fileTypeBox->addItem(".png");
	fileTypeBox->addItem(".bmp");
	fileTypeBox->addItem(".xpm");
	fileTypeBox->addItem(".gif");
	fileTypeBox->setFixedWidth(60);
	fileTypeBox->setCurrentIndex(0);	
	dialogLayout->addWidget(fileTypeBox);	
	compressionLabel = new QLabel("Set JPG quality to 100%");
	dialogLayout->addWidget(compressionLabel);
	compressionSlider = new QSlider(Qt::Horizontal);
	compressionSlider->setRange(0, 100);
	compressionSlider->setValue(100);
	compressionSlider->setTracking(true);
	dialogLayout->addWidget(compressionSlider);		
	sizeLabel = new QLabel("Set size to 100% of original size");
	dialogLayout->addWidget(sizeLabel);
	sizeSlider = new QSlider(Qt::Horizontal);
	sizeSlider->setRange(25, 100);
	sizeSlider->setValue(100);
	sizeSlider->setTracking(true);
	dialogLayout->addWidget(sizeSlider);
	QCheckBox* isEditable = new QCheckBox("Edit size manually");	
	isEditable->setChecked(false);	
	dialogLayout->addWidget(isEditable);
	keepAR = new QCheckBox("Keep aspect ratio");
	keepAR->setChecked(true);
	keepAR->setEnabled(false);
	QFrame* widthAndHeight = new QFrame();
	QHBoxLayout* widthAndHeightLayout = new QHBoxLayout();
	QLabel* widthLabel = new QLabel("Width:");
	widthAndHeightLayout->addWidget(widthLabel);
	widthEdit = new QLineEdit();
	widthEdit->setEnabled(false);
	widthAndHeightLayout->addWidget(widthEdit);
	QLabel* heightLabel = new QLabel("Height:");
	widthAndHeightLayout->addWidget(heightLabel);
	heightEdit = new QLineEdit();
	heightEdit->setEnabled(false);
	widthAndHeightLayout->addWidget(heightEdit);
	widthAndHeight->setLayout(widthAndHeightLayout);
	dialogLayout->addWidget(widthAndHeight);
	QPushButton* dialogSaveButton = new QPushButton("Save");	
	dialogLayout->addWidget(dialogSaveButton);
	saveDialog->setLayout(dialogLayout);
	connect(sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(sizeChange(int)));
	connect(isEditable, SIGNAL(stateChanged(int)), this, SLOT(editableChanged(int)));
	connect(selectFile, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect(dialogSaveButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(fileTypeBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(typeChanged(const QString&)));
	connect(compressionSlider, SIGNAL(valueChanged(int)), this, SLOT(compressionChanged(int)));

	isStitching = false;
}

Interface::~Interface()
{
	
	delete loadButton;
	if(composeButton) {
		delete composeButton;
	}
	delete imgGrid;
	delete layout;
	delete settings;
}

void Interface::quit() {	
	qApp->quit();
}

void Interface::openFilesDialog() {	
	QStringList strList = QFileDialog::getOpenFileNames(NULL, "Load files...", dirpath, "Images (*.png *.xpm *.jpg *.bmp *.gif)");	
	if(strList.size()) {
		if(!panoramaCreatorLaunched) {
			launchPanoramaCreator();
		}
		QFileInfo fi(strList.at(0));
		dirpath = fi.absolutePath();
		settings->setValue("Path", dirpath);
		imgGrid->addFiles(strList);
	}
}

void Interface::compose() {
	QList<QImage*>* il = imgGrid->getImgList();
	QStringList sl;
	QImageWriter* iw = new QImageWriter();
	QDir d;
	d.mkpath("C:/Temp/BioToolPanoramaCreatortmpimgs/");
	for(int i = 0; i<il->size(); i++) {
		QString name = "C:/Temp/BioToolPanoramaCreatortmpimgs/"+QString::number(i)+".png";
		sl.append(name);
		iw->setFileName(name);
		iw->write(*il->at(i));
	}
	delete iw;
	th = new OpenCVThread(sl);
	connect(th, SIGNAL(finished()), this, SLOT(setPanorama()));
	composeButton->setEnabled(false);
	stitchAction->setEnabled(false);
	QRect geom = geometry();
	panoLabel->setPixmap(NULL);
	panoLabel->setText("Stitching...");
	panoLabel->update();
	panoLabel->setVisible(true);
	isStitching = true;
	th->start(QThread::TimeCriticalPriority);
}

void Interface::setPanorama() {
	isStitching = false;
	composeButton->setEnabled(true);
	stitchAction->setEnabled(true);
	setPano(new QImage(*th->getResult()));	
	panoMat = th->getMatResult();
	delete th;
}


void Interface::filesAdded() {
	QStringListIterator iter2(*imgGrid->getFilesList());
	while(iter2.hasNext()) {
		QString s = iter2.next();
		if(!recentFiles->contains(s)) {
			recentFiles->append(s);
			if(recentFiles->size() == 31) {
				recentFiles->takeAt(0);
			}
		}
	}
	settings->beginWriteArray("Recent");
	for(int i = 0; i<recentFiles->size(); i++) {
		settings->setArrayIndex(i);
		settings->setValue("filepath", recentFiles->at(i));
	}
	settings->endArray();
	composeButton->setEnabled(true);
	stitchAction->setEnabled(true);
	QListIterator<QAction*> iter1(filesListMenu->actions());
	while(iter1.hasNext()) {
		QAction* a = iter1.next();
		filesListMenu->removeAction(a);
		ag->removeAction(a);
	}
	QStringListIterator iter(*imgGrid->getFilesList());	
	while(iter.hasNext()) {
		QAction* a = filesListMenu->addAction(iter.next());
		ag->addAction(a);
		a->setCheckable(true);
	}
	if(imgGrid->getCurrentId() != -1) {
		filesListMenu->actions().at(imgGrid->getCurrentId())->setChecked(true);
	}
	if(pano) {
		filesListMenu->addSeparator();
		filesListMenu->addAction(panoAction);
		ag->addAction(panoAction);
	}
}

void Interface::save() {
	
		QString fname = fileNameLabel->text();
		QImage ti = saveImage->scaled(widthEdit->text().toInt(), heightEdit->text().toInt());
		QImageWriter* imgWriter = new QImageWriter(fname);
		if(fileTypeBox->currentIndex() == 0) {
			imgWriter->setQuality(compressionSlider->value());
		}
		imgWriter->write(ti);
		delete imgWriter;
		saveDialog->done(0);
	
	
}

void Interface::showImg(QImage* img) {	
	if(img) {
		if(img != pano) {
			panoLabel->setLineWidth(1);
			filesListMenu->actions().at(imgGrid->getCurrentId())->setChecked(true);
		} else {
			panoAction->setChecked(true);
		}
		imgLabelImg = img;
		imgLabelShowImg = new QImage(*img);
		if(img == pano) {
			imgLabelMatImg = panoMat;
		} else {
			imgLabelMatImg = new Mat(Helper::qImage2IplImage(*img));			
		}
		brightnessSlider->setEnabled(true);
		brightnessSlider->setValue(0);
		contrastSlider->setEnabled(true);
		contrastSlider->setValue(100);
		scaleSlider->setEnabled(true);		
		isGray->setEnabled(true);	
		isGray->setChecked(false);
		resetButton->setEnabled(true);
		applyButton->setEnabled(true);
		applyToAllButton->setEnabled(true);
		saveShowButton->setEnabled(true);
		saveAction->setEnabled(true);
		deleteAction->setEnabled(false);
		moveLeftAction->setEnabled(false);
		moveRightAction->setEnabled(false);
		if(img != pano) {
			deleteAction->setEnabled(true);
			if(imgGrid->getCurrentId() > 0) {
				moveLeftAction->setEnabled(true);
			}
			if(imgGrid->getCurrentId() < imgGrid->getImgList()->size()-1) {
				moveRightAction->setEnabled(true);
			}
		}
		brightnessUpAction->setEnabled(true);
		brightnessDownAction->setEnabled(true);
		contrastUpAction->setEnabled(true);
		contrastDownAction->setEnabled(true);
		scaleUpAction->setEnabled(true);
		scaleDownAction->setEnabled(true);
		setGrayAction->setEnabled(true);
		resetAction->setEnabled(true);
		applyAction->setEnabled(true);
		applyToAllAction->setEnabled(true);
		QImage ti = img->scaled(scrollImgViewArea->size(), Qt::KeepAspectRatio);
		scaleSlider->setValue((int)floor(((double)ti.width()/img->width()*100)));
		QPixmap p = QPixmap::fromImage(ti);
		imgLabel->setPixmap(p);
		imgLabel->setFixedSize(ti.size());
	} else {
		imgLabel->setPixmap(NULL);
	}
}

void Interface::setPano(QImage *img) {
	if(img) {
		if(pano) {
			delete pano;
		}
		pano = img;		
		QImage ti = img->scaled((int)(imgGridPanoSize*0.8), imgGridPanoSize, Qt::KeepAspectRatio);
		QPixmap p = QPixmap::fromImage(ti);
		panoLabel->setPixmap(p);		
		saveButton->setEnabled(true);
		savePanoAction->setEnabled(true);
		filesListMenu->addSeparator();
		if(!panoAction) {
			panoAction = filesListMenu->addAction("Panorama");
			ag->addAction(panoAction);
			panoAction->setCheckable(true);
		}
	} else {
		panoLabel->setText("Failed");
	}
}

void Interface::selectPano() {
	emit panoSelected();
	panoLabel->setLineWidth(3);
	showImg(pano);
}

void Interface::mousePressEvent(QMouseEvent* e) {
	if(pano) {
		if(childAt(e->pos()) == panoLabel) {
			selectPano();
		}
	}
}

void Interface::fileDeleted() {
	imgLabelImg = NULL;
	imgLabel->setPixmap(NULL);
	QListIterator<QAction*> iter1(filesListMenu->actions());
	while(iter1.hasNext()) {
		QAction* a = iter1.next();
		filesListMenu->removeAction(a);
		ag->removeAction(a);
	}
	QStringListIterator iter(*imgGrid->getFilesList());
	while(iter.hasNext()) {
		QAction* a = filesListMenu->addAction(iter.next());
		ag->addAction(a);
		a->setCheckable(true);
	}
	if(pano) {
		filesListMenu->addSeparator();		
		filesListMenu->addAction(panoAction);
		ag->addAction(panoAction);
	}
	brightnessSlider->setEnabled(false);
	contrastSlider->setEnabled(false);
	scaleSlider->setEnabled(false);		
	isGray->setEnabled(false);
	resetButton->setEnabled(false);
	applyButton->setEnabled(false);
	applyToAllButton->setEnabled(false);
	saveShowButton->setEnabled(false);
	saveAction->setEnabled(false);
	deleteAction->setEnabled(false);
	moveLeftAction->setEnabled(false);
	moveRightAction->setEnabled(false);
	brightnessUpAction->setEnabled(false);
	brightnessDownAction->setEnabled(false);
	contrastUpAction->setEnabled(false);
	contrastDownAction->setEnabled(false);
	scaleUpAction->setEnabled(false);
	scaleDownAction->setEnabled(false);
	setGrayAction->setEnabled(false);
	resetAction->setEnabled(false);
	applyAction->setEnabled(false);
	applyToAllAction->setEnabled(false);
	if(imgGrid->getFilesList()->isEmpty()) {
		composeButton->setEnabled(false);
		stitchAction->setEnabled(false);
	}
}

void Interface::sizeChange(int pc) {
	sizeLabel->setText("Set size to "+QString::number(pc)+"% of original size");
	widthEdit->setText(QString::number((int)(pc/100.0*saveImage->width())));
	heightEdit->setText(QString::number((int)(pc/100.0*saveImage->height())));
}

void Interface::editableChanged(int s) {
	if(!s) {
		widthEdit->setEnabled(false);
		heightEdit->setEnabled(false);
		keepAR->setEnabled(false);
	} else {
		widthEdit->setEnabled(true);
		heightEdit->setEnabled(true);
		keepAR->setEnabled(true);
	}
}

void Interface::selectFile() {
	QString fname = QFileDialog::getSaveFileName(saveDialog, "Save panorama...", dirpath, "Images (*.png *.xpm *.jpg *.bmp *.gif)");	
	if(fname.endsWith(".jpg", Qt::CaseInsensitive) || fname.endsWith(".png", Qt::CaseInsensitive) || fname.endsWith(".gif", Qt::CaseInsensitive) || fname.endsWith(".bmp", Qt::CaseInsensitive) || fname.endsWith(".xpm", Qt::CaseInsensitive)) {
		fileNameLabel->setText(fname);
		fileTypeBox->setCurrentIndex(fileTypeBox->findText(fname.right(4), Qt::MatchExactly));
	} else {
		fileNameLabel->setText(fname+fileTypeBox->currentText());
	}
}

void Interface::openSaveDialog() {	
	if(imgLabelImg) {
		saveImage = imgLabelShowImg;
		widthEdit->setText(QString::number(imgLabelShowImg->width()));	
		heightEdit->setText(QString::number(imgLabelShowImg->height()));
		if(imgLabelImg != pano) {
			fileNameLabel->setText(imgGrid->getCurrentFileName());
		}
		if(scaleSlider->value() <= 100) {
			sizeSlider->setValue(scaleSlider->value());
		} else {
			sizeSlider->setValue(100);
		}
		saveDialog->exec();
	}
}

void Interface::widthChanged(QString& text) {
	int val = text.toInt();
	if(val > pano->width()) {
		val = pano->width();
		widthEdit->setText(QString::number(val));
	}
	if(val < 100) {
		val = 100;
		widthEdit->setText(QString::number(val));
	}
	
	if(keepAR->checkState()) {
		double pc = (double)val/pano->width();
		heightEdit->setText(QString::number((int)(pc*pano->height())));
		if(pc < 0.25) {
			pc = 0.25;
		}
		sizeSlider->setValue((int)(pc*100));
	}
}

void Interface::heightChanged(QString& text) {
	int val = text.toInt();
	if(val > pano->height()) {
		val = pano->height();
		heightEdit->setText(QString::number(val));
	}
	if(val < 100) {
		val = 100;
		heightEdit->setText(QString::number(val));
	}
	
	if(keepAR->checkState()) {
		double pc = (double)val/pano->height();
		widthEdit->setText(QString::number((int)(pc*pano->width())));
		if(pc < 0.25) {
			pc = 0.25;
		}
		sizeSlider->setValue((int)(pc*100));
	}
}

void Interface::keepARChanged(int s) {
	if(s) {
		widthChanged(widthEdit->text());
	}
}

bool Interface::event(QEvent* e) {
	if((e->type() == QEvent::LayoutRequest) || (e->type() == QEvent::Resize)) {
		QRect geom = geometry();
		imgGridPanoSize = (int)(geom.height()/4.0);
		panoLabel->setFixedWidth(imgGridPanoSize);
		imgGridPano->setFixedHeight(imgGridPanoSize);
		if(pano) {
			if(!isStitching) {
				QImage ti = pano->scaled((int)(imgGridPanoSize*0.8), imgGridPanoSize, Qt::KeepAspectRatio);
				QPixmap p = QPixmap::fromImage(ti);			
				panoLabel->setPixmap(p);				
			} else {
				panoLabel->setText("Stitching...");
			}
		}
		imgGrid->setFixedHeight(imgGridPanoSize-50);
	}
	return QWidget::event(e);
}

void Interface::typeChanged(const QString& type) {
	if(!fileNameLabel->text().isEmpty()) {
		fileNameLabel->setText(fileNameLabel->text().left(fileNameLabel->text().length()-4)+type);
	}
	if(type == ".jpg") {
		compressionLabel->setEnabled(true);
		compressionSlider->setEnabled(true);
	} else {
		compressionLabel->setEnabled(false);
		compressionSlider->setEnabled(false);
	}
}

void Interface::compressionChanged(int pc) {
	compressionLabel->setText("Set JPG quality to "+QString::number(pc)+"%");
}

void Interface::brightnessChanged(int pc) {
	brightnessLabel->setText("Brightness: "+QString::number(pc)+"%");
	brightness = pc;
	applyImgChanges();
}

void Interface::contrastChanged(int c) {	
	contrast = (c>=100)?((c-90)/10.0):(c/100.0);
	contrastLabel->setText("Contrast: "+QString::number(contrast));		
	applyImgChanges();
}

void Interface::scaleChanged(int pc) {
	scaleLabel->setText("Scale: "+QString::number(pc)+"%");
	scale = scaleSlider->value()/100.0;	
	applyImgChanges();
}

void Interface::grayButtonClicked(int s) {
	if(s) {		
		gray = true;
	} else {		
		gray = false;
	}
	setGrayAction->setChecked(s);
	applyImgChanges();
}

void Interface::applyImgChanges() {	
	Mat newm;

	
	imgLabelMatImg->convertTo(newm, -1, contrast, brightness);
	if(gray) {
		cvtColor(newm, newm, CV_RGB2GRAY);
	}
	IplImage i = (IplImage)newm;
	delete imgLabelShowImg;
	imgLabelShowImg = new QImage(Helper::IplImage2QImage(&i));	
	QImage ti = imgLabelShowImg->scaled((int)(imgLabelImg->width()*scale), (int)(imgLabelImg->height()*scale), Qt::KeepAspectRatio);
	QPixmap p = QPixmap::fromImage(ti);
	imgLabel->setPixmap(p);
	imgLabel->setFixedSize(ti.size());
}

void Interface::resetImage() {
	showImg(imgLabelImg);
}

void Interface::applyImage() {
	*imgLabelImg = *imgLabelShowImg;	
	if(imgLabelImg == pano) {
		QImage ti = pano->scaled((int)(imgGridPanoSize*0.8), imgGridPanoSize, Qt::KeepAspectRatio);
		QPixmap p = QPixmap::fromImage(ti);
		panoLabel->setPixmap(p);
	} else {
		imgGrid->setFixedHeight(imgGridPanoSize-51);
		imgGrid->setFixedHeight(imgGridPanoSize-50);	
	}
}

void Interface::applyAllImages() {
	if((contrast != 1) || (brightness != 0) || gray) { 
		if(imgLabelImg == pano) {
			applyImage();
		}
		QList<QImage*>* l = imgGrid->getImgList();
		QListIterator<QImage*> iter(*l);
		while(iter.hasNext()) {
			QImage* i = iter.next();
			Mat m(Helper::qImage2IplImage(*i));
			Mat	res;
			m.convertTo(res, -1, contrast, brightness);
			if(gray) {
				cvtColor(res, res, CV_RGB2GRAY);
			}
			IplImage ip = (IplImage)res;
			QImage newimg(Helper::IplImage2QImage(&ip));
			*i = newimg;
		}
		imgGrid->setFixedHeight(imgGridPanoSize-51);
		imgGrid->setFixedHeight(imgGridPanoSize-50);
	}
}

void Interface::openPanoSaveDialog() {
	if(pano) {
		saveImage = pano;
		widthEdit->setText(QString::number(pano->width()));	
		heightEdit->setText(QString::number(pano->height()));
		if((pano == imgLabelImg) && (scaleSlider->value() <= 100)) {
			sizeSlider->setValue(scaleSlider->value());
		} else {
			sizeSlider->setValue(100);
		}
		saveDialog->exec();
	}
}

void Interface::brightnessUp() {
	if(brightnessSlider->value()<90) {
		brightnessSlider->setValue(brightnessSlider->value()+10);
	} else {
		brightnessSlider->setValue(100);
	}
}

void Interface::brightnessDown() {
	if(brightnessSlider->value()>-90) {
		brightnessSlider->setValue(brightnessSlider->value()-10);
	} else {
		brightnessSlider->setValue(-100);
	}
}

void Interface::contrastUp() {
	if(contrastSlider->value()<180) {
		contrastSlider->setValue(contrastSlider->value()+10);
	} else {
		contrastSlider->setValue(190);
	}
}

void Interface::contrastDown() {
	if(contrastSlider->value()>11) {
		contrastSlider->setValue(contrastSlider->value()-10);
	} else {
		contrastSlider->setValue(1);
	}
}

void Interface::scaleUp() {
	if(scaleSlider->value()<475) {
		scaleSlider->setValue(scaleSlider->value()+25);
	} else {
		scaleSlider->setValue(500);
	}
}

void Interface::scaleDown() {
	if(scaleSlider->value()>50) {
		scaleSlider->setValue(scaleSlider->value()-25);
	} else {
		scaleSlider->setValue(25);
	}
}

void Interface::setGray(bool set) {
	isGray->setChecked(set);
}

void Interface::fileSelected(QAction* act) {	
	act->setChecked(true);
	if(act != panoAction) {
		int ind = imgGrid->getFilesList()->indexOf(act->text());
		imgGrid->select(ind);
	} else {
		selectPano();
	}
}

void Interface::moveLeft() {
	imgGrid->moveLeftCurrent();
	int ind = imgGrid->getCurrentId();
	moveLeftAction->setEnabled(true);
	moveRightAction->setEnabled(true);
	if(ind == 0) {
		moveLeftAction->setEnabled(false);
	}
	if(ind == imgGrid->getImgList()->size()-1) {
		moveRightAction->setEnabled(false);
	}
}

void Interface::moveRight() {
	imgGrid->moveRightCurrent();
	int ind = imgGrid->getCurrentId();
	moveLeftAction->setEnabled(true);
	moveRightAction->setEnabled(true);
	if(ind == 0) {
		moveLeftAction->setEnabled(false);
	}
	if(ind == imgGrid->getImgList()->size()-1) {
		moveRightAction->setEnabled(false);
	}
}

void Interface::launchPanoramaCreator() {
	panoramaCreatorLaunched = true;
	toolsMenu->removeAction(panoramaCreatorAction);
	toolsMenu->removeAction(counterAction);
	stitchAction = toolsMenu->addAction("Stitch images");
	stitchAction->setEnabled(false);
	connect(stitchAction, SIGNAL(triggered(bool)), this, SLOT(compose()));	
	QAction* processAction = toolsMenu->addAction("Process result (NYI)");
	processAction->setEnabled(false);	
	showMaximized();
	centralWidget()->setVisible(false);
	QWidget* userarea = new QWidget();
	setCentralWidget(userarea);
	userarea->setLayout(layout);
	setWindowTitle("BioTool interface alpha - Panorama creator");
}

void Interface::loadRecent() {
	launchPanoramaCreator();
	QModelIndexList l;
	l << recentView->selectionModel()->selectedIndexes();
	QStringList s;
	for(int i = 0; i<l.size(); i++) {
		int ind = l[i].row();
		s.append(recentFiles->at(ind));
	}
	imgGrid->addFiles(s);
	filesAdded();
}