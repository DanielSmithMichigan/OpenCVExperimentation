#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/text.hpp>

using namespace std;

int main(int argc, char** argv )
{
	cv::Mat imageIn = cv::imread("../images/font.tif");
	imshow("W", imageIn);
	int width = imageIn.cols;
	cvNamedWindow("ImageShrink");
	int threshold = 120;
	int maxValue = 255;
	int thresholdType = cv::THRESH_BINARY;
	double ratio = 2.5;
	cout << "Resizing with ratio: " << ratio << endl;
	int newWidth = ratio * width;
	int minHeight = imageIn.rows * ratio;
	cv::resize(imageIn, imageIn, cv::Size(newWidth, minHeight), 0, 0, cv::INTER_NEAREST);
	cv::cvtColor(imageIn, imageIn, cv::COLOR_RGB2GRAY);
	cv::threshold(imageIn, imageIn, threshold, maxValue, thresholdType);
	imwrite("../images/newFont.tif", imageIn);
	return 0;
}