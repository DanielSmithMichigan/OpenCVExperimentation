#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

vector<Rect> getSlidingWindows(Mat& image,int winWidth,int winHeight)
{
  vector<Rect> rects;
  int step = 16;
  for(int i=0;i<image.rows;i+=step)
  {
      if((i+winHeight)>image.rows){break;}
      for(int j=0;j< image.cols;j+=step)    
      {
          if((j+winWidth)>image.cols){break;}
          Rect rect(j,i,winWidth,winHeight);
          rects.push_back(rect);
      }
  } 
  return rects;
}

double distance(double x1, double y1, double x2, double y2) {
  double a = x1 - x2;
  double b = y1 - y2;
  return sqrt(a * a + b * b);
}
/// Global variables
Mat src, src_gray;
int thresh = 49;
int max_thresh = 55;
int cornerThreshold = 1;
int maxCornerThreshold = 40;
int minDistance = 100;
int maxDistance = 1000;
vector<Point> corners;

char* source_window = "Source image";
char* corners_window = "Corners detected";

/// Function header
void cornerHarris_demo( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window and a trackbar
  namedWindow( source_window, WINDOW_AUTOSIZE );
  createTrackbar( "Corner Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
  createTrackbar( "Corners Required: ", source_window, &cornerThreshold, maxCornerThreshold, cornerHarris_demo );
  createTrackbar( "Distance: ", source_window, &minDistance, maxDistance, cornerHarris_demo );

  cornerHarris_demo( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function cornerHarris_demo */
void cornerHarris_demo( int, void* )
{
  cout << "Running again" << endl;
  Mat dst, dst_norm, dst_norm_scaled;
  Mat out = src.clone();
  dst = Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );
  corners.clear();
  vector<Point>(corners).swap(corners);
	for( int y = 0; y < dst_norm.rows ; y++ )
	{ 
		for( int x = 0; x < dst_norm.cols; x++ )
		{
			if( (int) dst_norm.at<float>(y,x) > thresh)
			{
        corners.push_back(Point(x, y));
			}
		}
	}

  cout << "Size: " << corners.size() << endl;
  if (corners.size() < 3000) {
    for(vector<Point>::iterator it = corners.begin(); it != corners.end(); ++it) {
      circle(out, Point((*it).x, (*it).y), 2, CV_RGB(0,0,255));
      int numCorners = 0;
      for(vector<Point>::iterator inner = corners.begin(); inner != corners.end(); ++inner) {
        if (distance((*it).x, (*it).y, (*inner).x, (*inner).y) < minDistance) {
          numCorners++;
        }
      }
      if (numCorners < cornerThreshold) {
        continue;
      }
      circle(out, Point((*it).x, (*it).y), numCorners, CV_RGB(255,0 ,0));
    }
  }
  /// Drawing a circle around corners
  /// Showing the result
  cout << "Finished running" << endl;
  imshow( source_window, out );
}