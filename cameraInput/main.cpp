#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
	CvCapture* capture = 0;
	capture = cvCreateCameraCapture(0);
	if (!capture) {
		return -1;
	}
	IplImage *bgr_frame = cvQueryFrame(capture);
	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	CvSize size = cvSize(
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
	);
	CvVideoWriter *writer = cvCreateVideoWriter(
		argv[1],
		CV_FOURCC('M', 'J', 'P', 'G'),
		fps,
		size
	);
	cout << fps;
	int x = 0;

	while ((bgr_frame = cvQueryFrame(capture)) != NULL && x++ < 100) {
		cvWriteFrame(writer, bgr_frame);
	}
	cvReleaseVideoWriter( &writer );
	cvReleaseCapture( &capture );
	return 0;
}