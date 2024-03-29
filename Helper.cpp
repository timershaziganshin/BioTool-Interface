#include "Helper.h"

IplImage* Helper::qImage2IplImage(const QImage& qImage)
  {
    int width = qImage.width();
    int height = qImage.height();
 
    // Creates a iplImage with 3 channels
    IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    char * imgBuffer = img->imageData;
 
    //Remove alpha channel
	bool f = qImage.hasAlphaChannel();
	int jump = qImage.bytesPerLine()/qImage.width();
	
    for (int y=0;y<img->height;y++){
      QByteArray a((const char*)qImage.scanLine(y), qImage.bytesPerLine());
	  for (int i=0; i<a.size(); i+=jump){
          //Swap from RGB to BGR
	      imgBuffer[2] = a[i+2];
          imgBuffer[1] = a[i+1];
          imgBuffer[0] = a[i];
          imgBuffer+=3;
      }
  }
 
  return img;
  }

QImage Helper::IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;
 
    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage img(qImageBuffer, width, height, iplImage->widthStep, QImage::Format_RGB888);
      return img.rgbSwapped();
    } else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage img(qImageBuffer, width, height, iplImage->widthStep, QImage::Format_Indexed8);
 
	QVector<QRgb> colorTable;
	for (int i = 0; i < 256; i++){
	    colorTable.push_back(qRgb(i, i, i));
	}
	img.setColorTable(colorTable);
	return img;
    }else{
      qWarning() << "Image cannot be converted.";
      return QImage();
    }
}