#include <stdio.h>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv )
{
    IplImage* img = cvLoadImage(argv[1]);

    cvNamedWindow("Esxample1", CV_WINDOW_AUTOSIZE);
    cvShowImage("Example1",img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyWindow("Example1");

    return 0;
}