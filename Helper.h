#ifndef HELPER_H
#define HELPER_H

#include <QtGui>
#include <opencv2/opencv.hpp>

using namespace cv;

class Helper {
public:
	static IplImage* qImage2IplImage(const QImage&);
	static QImage IplImage2QImage(const IplImage*);
};

#endif