#include "opencvthread.h"


OpenCVThread::OpenCVThread(QStringList& files) : QThread() {	
	this->files.append(files);
	res = NULL;
}

void OpenCVThread::run() {
	Stitcher st = Stitcher::createDefault(false);
	vector<Mat> list;
	QStringListIterator iter(files);
			
	while(iter.hasNext()) {	
		list.push_back(imread(iter.next().toStdString()));
	}
	matRes = new Mat();
	IplImage i;
	int cnt = 0;
	do {
		Stitcher::Status ss = st.stitch(list, *matRes);	
		if(ss == Stitcher::OK) {	
			i = (IplImage)(*matRes);	
			cnt++;
		} else {
			return;
		}
	} while((cnt < 3) && (i.width<5) && (i.height<5));		
	if((i.width<5) && (i.height<5)) {
		return;
	}	
	res = new QImage(Helper::IplImage2QImage(&i));							
}

QImage* OpenCVThread::getResult() {
	return res;
}

Mat* OpenCVThread::getMatResult() {
	return matRes;
}