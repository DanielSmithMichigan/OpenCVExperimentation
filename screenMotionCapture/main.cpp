#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

Mat ImageFromDisplay(int Width, int Height)
{
    Display* display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);
    XImage* img = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);
    XFree(img);
    XCloseDisplay(display);
    return Mat(Height, Width, img->bits_per_pixel > 24 ? CV_8UC4 : CV_8UC3, img->data);

}

int main(int argc, char** argv )
{
    int width = 1500;
    int height = 500;
    Mat img_object = imread("../images/rocks.png", CV_LOAD_IMAGE_COLOR );
    Mat img_scene = ImageFromDisplay(width, height);  
    cvtColor(img_scene, img_scene, CV_BGRA2BGR);
    // img_scene.convertTo(img_scene, CV_32F);
    // img_object.convertTo(img_object, CV_32F);
    if (!img_object.data || !img_scene.data) {
        cout << "Error reading images";
        return -1;
    }
    Mat result;
    matchTemplate( img_scene, img_object, result, CV_TM_SQDIFF_NORMED );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc = minLoc;
    rectangle( img_scene, matchLoc, Point( matchLoc.x + img_object.cols , matchLoc.y + img_object.rows ), CV_RGB(255, 255, 255), 3 );

    imshow( "Good Matches & Object detection", img_scene );
    
    waitKey(0);
    return 0;
}