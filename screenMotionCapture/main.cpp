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
    XImage* img = XGetImage(display, root, 600, 600 , Width, Height, AllPlanes, ZPixmap);
    if (!img->data) {
        cout << "No image data";
        throw("No image data");
    }
    Mat scene = Mat(Height, Width, img->bits_per_pixel > 24 ? CV_8UC4 : CV_8UC3, img->data);
    if (!scene.data) {
        cout << "Error reading scene";
        throw("Error reading scene");
    }
    cvtColor(scene, scene, CV_BGRA2BGR);
    XFree(img);
    XCloseDisplay(display);
    return scene;
}

Mat MatchAgainst(Mat img_scene, Mat img_object, const Scalar& color, String Name = "")
{
    Mat result;
    matchTemplate( img_scene, img_object, result, CV_TM_SQDIFF_NORMED );
    imshow( "ResultWindow", result);
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    cout << Name << ": " << minVal << endl;
    if (minVal < .25) {
        matchLoc = minLoc;
        rectangle( img_scene, matchLoc, Point( matchLoc.x + img_object.cols , matchLoc.y + img_object.rows ), color, 3 );
    }
    return img_scene;
}

int main(int argc, char** argv )
{
    Mat tin = imread("../images/drop.png", CV_LOAD_IMAGE_COLOR);
    if (!tin.data) {
        throw("Error reading scene");
    }
    Mat img_scene;
    while(true) {
        waitKey(100);
        Mat img_scene = ImageFromDisplay(300, 300);
        img_scene = MatchAgainst(img_scene, tin, CV_RGB(255, 255, 255), "TIN");
        imshow( "Good Matches & Object detection", img_scene );
    }
    

    
    return 0;
}