#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui>
#include "ImageGrid.h"
#include "opencvthread.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Interface : public QMainWindow
{
	Q_OBJECT

public:	
	Interface(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Interface();


	
	void mousePressEvent(QMouseEvent* e);
	bool event(QEvent* e);

private slots:
	void quit();
	void openFilesDialog();
	void compose();
	void setPanorama();
	void save();
	void selectPano();
	void sizeChange(int pc);
	void editableChanged(int s);
	void selectFile();
	void openSaveDialog();
	void widthChanged(QString& text);
	void heightChanged(QString& text);
	void keepARChanged(int s);
	void compressionChanged(int pc);
	void typeChanged(const QString& type);
	void brightnessChanged(int pc);
	void contrastChanged(int c);
	void scaleChanged(int pc);
	void grayButtonClicked(int s);
	void resetImage();
	void applyImage();
	void applyAllImages();
	void openPanoSaveDialog();
	void brightnessUp();
	void brightnessDown();
	void contrastUp();
	void contrastDown();
	void scaleUp();
	void scaleDown();
	void setGray(bool set);
	void fileSelected(QAction* act);
	void moveLeft();
	void moveRight();
	void launchPanoramaCreator();
	void loadRecent();

signals:
	void panoSelected();

public slots:
	void filesAdded();
	void showImg(QImage* img);	
	void fileDeleted();

private:
	

	void setPano(QImage* img);
	void applyImgChanges();

	QVBoxLayout* layout;
	QPushButton* loadButton;
	ImageGrid* imgGrid;
	QPushButton* composeButton;
	QPushButton* saveButton;
	OpenCVThread* th;
	QLabel* imgLabel;
	QImage* pano;
	QLabel* panoLabel;
	QString dirpath;
	QSettings* settings;
	QDialog* saveDialog;
	QLabel* fileNameLabel;
	QLineEdit* widthEdit;
	QLineEdit* heightEdit;
	QLabel* sizeLabel;
	QCheckBox* keepAR;
	QSlider* sizeSlider;
	int imgGridPanoSize;
	QFrame* imgGridPano;
	QScrollArea* scrollImgViewArea;
	QImage* imgLabelImg;
	Mat* imgLabelMatImg;
	QImage* imgLabelShowImg;
	QComboBox* fileTypeBox;
	QLabel* compressionLabel;
	QSlider* compressionSlider;
	QLabel* brightnessLabel;
	QSlider* brightnessSlider;
	QLabel* contrastLabel;
	QSlider* contrastSlider;
	QLabel* scaleLabel;
	QSlider* scaleSlider;
	QCheckBox* isGray;
	QPushButton* resetButton;
	QPushButton* applyButton;
	QPushButton* applyToAllButton;
	QPushButton* saveShowButton;
	QImage* saveImage;
	Mat* panoMat;
	int brightness;
	double contrast;
	double scale;
	bool gray;

	QAction* saveAction;
	QAction* savePanoAction;
	QAction* deleteAction;
	QAction* moveLeftAction;
	QAction* moveRightAction;
	QAction* brightnessUpAction;
	QAction* brightnessDownAction;
	QAction* contrastUpAction;
	QAction* contrastDownAction;
	QAction* scaleUpAction;
	QAction* scaleDownAction;
	QAction* setGrayAction;
	QAction* resetAction;
	QAction* applyAction;
	QAction* applyToAllAction;
	QAction* stitchAction;
	QAction* panoAction;
	QAction* openAction;
	QAction* panoramaCreatorAction;
	QAction* counterAction;

	QActionGroup* ag;

	QMenu* toolsMenu;
	QMenu* filesListMenu;

	QStringList* recentFiles;
	QListWidget* recentView;
	bool panoramaCreatorLaunched;

	bool isStitching;
};

#endif // INTERFACE_H
