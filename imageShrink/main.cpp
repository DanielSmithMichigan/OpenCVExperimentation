#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

IplImage* shrink(IplImage* in) {
	IplImage* out = cvCreateImage(cvSize(in->width/2, in->height/2), in->depth, in->nChannels);
	cvPyrDown(in, out);
	return out;
}

IplImage* getRed(IplImage *src) {
	CvSize s = cvSize(src->width, src->height);
	int d = src->depth;
	IplImage* R = cvCreateImage(s, d, 1);
	cvSplit(src, R, NULL, NULL, NULL);
	return R;
}

IplImage* canny(IplImage* in,
	double lowThresh,
	double highThresh,
	double aperture
	) {
	if(in->nChannels != 1) {
		return 0;
	}
	IplImage* out = cvCreateImage(
		cvSize(in->width, in->height),
		IPL_DEPTH_8U,
		1
	);
	cvCanny( in, out, lowThresh, highThresh, aperture );
	return out;
}

int main(int argc, char** argv )
{
	IplImage *imageIn = cvLoadImage( argv[1] );
	cvNamedWindow("ImageShrink");
	imageIn = getRed(imageIn);
	imageIn = canny(imageIn, 1, 100, 3);
	cvShowImage("ImageShrink", imageIn);
	cvReleaseImage(&imageIn);
	cvWaitKey(0);
	cvDestroyWindow("ImageShrink");
	return 0;
}