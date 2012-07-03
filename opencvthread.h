#ifndef OPENCVTHREAD_H
#define OPENCVTHREAD_H

#include <QtGui>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <vector>
#include "Helper.h"

class OpenCVThread : public QThread {
	
public:
	OpenCVThread(QStringList&);

	void run();
	QImage* getResult();
	Mat* getMatResult();

private:
	QImage* res;
	Mat* matRes;
	QStringList files;
};

#endif