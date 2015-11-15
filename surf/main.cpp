#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(int argc, char** argv )
{
	Mat img_object = imread("../images/object.png", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_scene = imread("../images/scene.png", CV_LOAD_IMAGE_GRAYSCALE );
	if (!img_object.data || !img_scene.data) {
		cout << "Error reading images";
		return -1;
	}

	int minHessian = 400;
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

	printf("-- Max dist: %f, \n", max_dist);
	printf("-- Min dist: %f, \n", min_dist);

	vector<DMatch> good_matches;
	for(int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance < 300 * min_dist) {
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
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); 
	obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); 
	obj_corners[3] = cvPoint( 0, img_object.rows );
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform( obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	//-- Show detected matches
	imshow( "Good Matches & Object detection", img_matches );

	// line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	// line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	// line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	// line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	
	// //-- Show detected matches
	// imshow( "Good Matches & Object detection", img_matches );

	waitKey(0);
	return 0;
}