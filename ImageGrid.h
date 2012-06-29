#ifndef IMAGEGRID_H
#define IMAGEGRID_H

#include <QtGui>

class ImageGrid : public QWidget {

	Q_OBJECT

public:
	ImageGrid();
	~ImageGrid();

	void mousePressEvent(QMouseEvent* me);
	void mouseMoveEvent(QMouseEvent* me);

	void mouseReleaseEvent(QMouseEvent* e);
	void contextMenuEvent(QContextMenuEvent* e);
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);	

	void addFiles(QStringList& filesList);
	void addFile(QString& file);
	
	bool event(QEvent* e);
	QStringList* getFilesList();
	QList<QImage*>* getImgList();
	QString getCurrentFileName();
	int getCurrentId();

signals:
	void fileDeleted();
	void imgSelected(QImage* i);
	void fileAdded();

public slots:

	void removeSelection();
	void deleteCurrent();
	void moveLeftCurrent();
	void moveRightCurrent();
	void select(int n);

private:
	QWidget* dragStatus;
	QHBoxLayout* imgGridLayout;
	int dragWidgetXcorr;
	int dragWidgetYcorr;	
	QStringList* filesList;
	QMenu* deleteMenu;
	QAction* deleteAction;
	QAction* moveLeftAction;
	QAction* moveRightAction;
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