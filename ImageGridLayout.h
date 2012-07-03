#ifndef PANORAMALAYOUT_H
#define PANORAMALAYOUT_H

#include <QtGui>

class ImageGridLayout : public QLayout {

	Q_OBJECT

public:
	ImageGridLayout();
	~ImageGridLayout();

	void addItem(QLayoutItem* item);
	void setGeometry(QRect& rect);
	QSize sizeHint() const;
	QLayoutItem* itemAt(int pos) const;
	QLayoutItem* takeAt(int pos);
	QSize minimumSize() const;
	int count() const;
	int indexOf(QWidget* w);

private:
	QList<QLayoutItem*>* listWidgets;
	//QList<QRect*>* listGeoms;

};

#endif