#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

CvRect box;
bool drawingBox = false;

void drawBox(IplImage* image, CvRect rect) {
	cvRectangle(
			image,
			cvPoint(box.x, box.y),
			cvPoint(box.x+box.width, box.y+box.height),
			cvScalar(0xff,0x00,0x00)
		);
}

void cvMouseCallback(int event, int x, int y, int flags, void* param) 
{
	IplImage* image = (IplImage*) param;
	switch(event) {
		case CV_EVENT_MOUSEMOVE: {
			if (drawingBox) {
				box.width = x - box.x;
				box.height = y - box.y;
			}
		}
		break;
		case CV_EVENT_LBUTTONDOWN: {
			drawingBox = true;
			box = cvRect(x, y, 0, 0);
		}
		break;
		case CV_EVENT_LBUTTONUP: {
			drawingBox = false;
			if (box.width < 0) {
				box.x += box.width;
				box.width *= -1;
			}
			if (box.height < 0) {
				box.y += box.height;
				box.height *= -1;
			}
			drawBox(image, box);
		}
		break;
	}
}

int main(int argc, char** argv )
{
	box = cvRect(-1, -1, 0, 0);
	IplImage* image = cvCreateImage(
		cvSize(200, 200),
		IPL_DEPTH_8U,
		3
	);

	cvZero( image );
	IplImage* temp = cvCloneImage ( image );
	cvNamedWindow("mouseInput");
	cvSetMouseCallback("mouseInput",
		cvMouseCallback,
		(void*) image
	);

	while( 1 ) {
		cvCopy( image, temp );
		if (drawingBox) {
			drawBox(temp, box);
		}
		cvShowImage("mouseInput", temp);
		if ( cvWaitKey(15) == 27) {
			break;
		}
	}

	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyWindow( "mouseInput" );
	return 0;
}
