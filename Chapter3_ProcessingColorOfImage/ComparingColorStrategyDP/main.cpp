#include <iostream>
#include <vector>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

#include "ColorDetector.h"

using namespace std;
using namespace cv;

int main()
{
	CColorDetector cdetect;
	Mat result;
	Mat image = imread("1.jpg", IMREAD_COLOR);
	if (!image.empty())
	{
		cdetect.setTargetColor(0, 0, 0);
		cdetect.SetMaxDistance(100);
		cdetect.process(image, result);
		imshow("inital image", image);
		Mat hsv;
		cvtColor(image, hsv, CV_BGR2HSV);
		//imshow("hsv", hsv);
		vector<Mat> vImages;
		split(image, vImages);
		imshow("hue", vImages[0]);
		imshow("saturation", vImages[1]);
		imshow("value", vImages[2]);
		//imshow("detect image", result);
		waitKey(0);
	}
	return 0;
}