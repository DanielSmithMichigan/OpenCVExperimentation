#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>

using namespace std;
using namespace cv;

IplImage *XImage2IplImageAdapter(XImage *ximage)
{
        IplImage *iplImage;
        assert(ximage->format == ZPixmap);
        assert(ximage->depth == 24); 

        iplImage = cvCreateImageHeader(
                cvSize(ximage->width, ximage->height), 
                IPL_DEPTH_8U,
                ximage->bits_per_pixel/8);

        iplImage->widthStep = ximage->bytes_per_line;
        if(ximage->data != NULL)
                iplImage->imageData = ximage->data;

        return iplImage;
}

IplImage* ImageFromDisplay(int x, int y, int width, int height)
{
    Display* display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);

    XImage* img = XGetImage(display, root, 0, 0 , width, height, AllPlanes, ZPixmap);
    return XImage2IplImageAdapter(img);
}

int main(int argc, char** argv )
{
    int width = 500;
    int height = 500;

    IplImage *display = ImageFromDisplay(0, 0, width, height);

    if (width && height)
    {

        namedWindow("WindowTitle", WINDOW_AUTOSIZE);
        cvShowImage("WindowTitle", display);

        waitKey(0);
    }
    return 0;
}