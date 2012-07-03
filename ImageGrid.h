#ifndef IMAGEGRID_H
#define IMAGEGRID_H

#include <QtGui>
//#include "ImageGridLayout.h"

class ImageGrid : public QWidget {

	Q_OBJECT

public:
	ImageGrid();
	//ImageGrid(QStringList* filesList);
	~ImageGrid();

	void mousePressEvent(QMouseEvent* me);
	//void mouseReleaseEvent(QMouseEvent* me);
	void mouseMoveEvent(QMouseEvent* me);

	void mouseReleaseEvent(QMouseEvent* e);
	void contextMenuEvent(QContextMenuEvent* e);
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);	

	void addFiles(QStringList& filesList);
	void addFile(QString& file);
	
	//QSize sizeHint();	
	bool event(QEvent* e);
	QStringList* getFilesList();
	QList<QImage*>* getImgList();
	QString getCurrentFileName();
	int getCurrentId();
	//void setPano(QImage* i);

signals:
	void fileDeleted();
	void imgSelected(QImage* i);
	void fileAdded();
	/*void rightCornerReached();
	void leftCornerReached();*/

public slots:

	void removeSelection();
	void deleteCurrent();
	void moveLeftCurrent();
	void moveRightCurrent();
	void select(int n);

private:
	QWidget* dragStatus;
	QHBoxLayout* imgGridLayout;
	//int count;
	int dragWidgetXcorr;
	int dragWidgetYcorr;	
	QStringList* filesList;
	QMenu* deleteMenu;
	QAction* deleteAction;
	QAction* moveLeftAction;
	QAction* moveRightAction;
	//bool hasPano;
	QLabel* prevSelectedLabel;
	int containerSize;
	QList<QImage*>* imgList;
	QImage* dragImg;	
	QScrollArea* container;
	int tmpIndex;
	QLabel* tmpLabel;
	QString tmpFile;
};

#endif