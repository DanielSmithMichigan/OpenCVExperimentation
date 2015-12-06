/*
 * webcam-demo.cpp
 *
 * A demo program of End-to-end Scene Text Detection and Recognition.
 *
 * Created on: Jul 31, 2014
 *     Author: Lluis Gomez i Bigorda <lgomez AT cvc.uab.es>
 */

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/text.hpp"


using namespace std;
using namespace cv;
using namespace cv::text;

//Perform text detection and recognition from webcam
int main(int argc, char* argv[])
{
    Ptr<OCRTesseract> ocr = OCRTesseract::create();
    cv::Mat img1 = imread("../images/resize.png");
	cv::cvtColor(img1, img1, cv::COLOR_RGB2GRAY);
	cv::threshold(img1, img1, 100, 255, THRESH_BINARY_INV);
	imshow("W", img1);
	cv::waitKey(100000);
    string output;
    ocr->run(img1, output);
    cout << output;
    return 0;
}
