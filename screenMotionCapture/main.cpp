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

Mat AddMatchesToImage(Mat img_object, Mat img_scene)
{
    if (!img_object.data || !img_scene.data) {
        cout << "Error reading images";
    }

    int minHessian = 40;
    Ptr<SURF> surf = SURF::create(minHessian);
    vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    surf->detect(img_object, keypoints_object);
    surf->compute(img_object, keypoints_object, descriptors_object);
    surf->detect(img_scene, keypoints_scene);
    surf->compute(img_scene, keypoints_scene, descriptors_scene);
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_object, descriptors_scene, matches);
    double max_dist=0; double min_dist = 100;
    for (int i = 0; i < descriptors_object.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    cout << "Matches: " << matches.size() << endl;

    vector<DMatch> good_matches;
    for(int i = 0; i < descriptors_object.rows; i++) {
        if (true || matches[i].distance < 3000 * min_dist) {
            good_matches.push_back(matches[i]);
        }
    }

    Mat img_matches;
    drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    vector<Point2f> obj,scene;
    for(int i = 0; i < good_matches.size(); i++) {
        obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
    }

    cout << "obj: " << obj.size() << endl;
    cout << "scene: " << scene.size() << endl;
    cout << "Rows: " << img_object.rows << endl;
    cout << "Cols: " << img_object.cols << endl;

    Mat H = findHomography( obj, scene, CV_RANSAC );
    cout << H << endl;
    cout << "Size: " << H.size() << endl;
    // -- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); 
    obj_corners[1] = cvPoint( img_object.cols, 0 );
    obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); 
    obj_corners[3] = cvPoint( 0, img_object.rows );
    std::vector<Point2f> scene_corners(4);
    // perspectiveTransform( obj_corners, scene_corners, H);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    // line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
    // line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    // line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    // line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

    return img_matches;
}

IplImage* XImage2IplImageAdapter(XImage *ximage)
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

    // Mat object = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    Mat scene = ImageFromDisplay(width, height);
    // Mat imageMatches = AddMatchesToImage(object, scene);
    namedWindow("WindowTitle", WINDOW_AUTOSIZE);
    imshow("WindowTitle",  scene);

    waitKey(0);
    return 0;
}