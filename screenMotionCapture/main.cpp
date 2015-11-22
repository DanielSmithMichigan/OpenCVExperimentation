#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

using namespace std;
using namespace cv;

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

Mat MatchAgainstScreen(int width, int height, Mat img_object)
{
    Mat img_scene = ImageFromDisplay(width, height); 
    if (!img_scene.data) {
        cout << "Error reading scene";
        throw("Error reading scene");
    } 
    cvtColor(img_scene, img_scene, CV_BGRA2BGR);Mat result;
    matchTemplate( img_scene, img_object, result, CV_TM_SQDIFF_NORMED );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc = minLoc;
    rectangle( img_scene, matchLoc, Point( matchLoc.x + img_object.cols , matchLoc.y + img_object.rows ), CV_RGB(255, 255, 255), 3 );
    return img_scene;
}

int main(int argc, char** argv )
{
    Mat img_object = imread("../images/rocks.png", CV_LOAD_IMAGE_COLOR );
    if (!img_object.data) {
        throw("Error reading scene");
    }
    Mat img_scene;
    img_scene = MatchAgainstScreen(800, 800, img_object);
    imshow( "Good Matches & Object detection", img_scene );
    while(true) {
        waitKey(500);
        img_scene = MatchAgainstScreen(800, 800, img_object);
        imshow( "Good Matches & Object detection", img_scene );
    }
    

    
    return 0;
}