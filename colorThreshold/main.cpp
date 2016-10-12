#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/text.hpp>

using namespace std;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 50;
int dilation_type;
int erosion_type;

int main(int argc, char** argv )
{
	cv::Mat imageIn = cv::imread(argv[1]);
	cv::Mat imageOut;
	imshow("BEFORE", imageIn);
	cv::cvtColor(imageIn,imageIn,cv::COLOR_BGR2HSV);

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0; 
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;
	/// Global variables
    cv::namedWindow("CONTROL", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Create trackbars in "Control" window
	cv::createTrackbar("LowH", "CONTROL", &iLowH, 179); //Hue (0 - 179)
	cv::createTrackbar("HighH", "CONTROL", &iHighH, 179);

	cv::createTrackbar("LowS", "CONTROL", &iLowS, 255); //Saturation (0 - 255)
	cv::createTrackbar("HighS", "CONTROL", &iHighS, 255);

	cv::createTrackbar("LowV", "CONTROL", &iLowV, 255); //Value (0 - 255)
	cv::createTrackbar("HighV", "CONTROL", &iHighV, 255);

	cv::createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "CONTROL", &erosion_elem, max_elem);
	cv::createTrackbar( "Erosion Kernel size:\n 2n +1", "CONTROL", &erosion_size, max_kernel_size);

	cv::createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "CONTROL", &dilation_elem, max_elem);
	cv::createTrackbar( "Dilation Kernel size:\n 2n +1", "CONTROL", &dilation_size, max_kernel_size);

	while(true) {
		cv::inRange(imageIn, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imageOut);
		if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
		else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
		else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }
  		cv::Mat erosionElem = getStructuringElement( erosion_type, cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size ) );
  		cv::erode( imageOut, imageOut, erosionElem );
		if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
		else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
		else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
  		cv::Mat dilationElem = getStructuringElement( dilation_type, cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ), cv::Point( dilation_size, dilation_size ) );
  		cv::dilate( imageOut, imageOut, dilationElem );
		imshow("AFTER", imageOut);
		cv::waitKey(250);
	}
	return 0;
}