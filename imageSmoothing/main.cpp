#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
	IplImage *imageIn = cvLoadImage( argv[1] );
	IplImage *imageOut;
	cvNamedWindow("Source");
	cvNamedWindow("Smoothing");
	cvShowImage("Source", imageIn);
	CvCapture *capture = cvCreateFileCapture(argv[1]);
	imageOut = cvCreateImage(cvGetSize(imageIn), IPL_DEPTH_8U, 3);
	cvSmooth(imageIn, imageOut, CV_GAUSSIAN, 3, 3);
	cvShowImage("Smoothing", imageOut);
	cvReleaseImage(&imageOut);
	cvWaitKey(0);
	cvDestroyWindow("Source");
	cvDestroyWindow("Smoothing");
	IplImage* frame;
	return 0;
}
