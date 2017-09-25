#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>
/*
	Erode  : choose a structure and one anchor point in the structure. Move the structure over the image, replace the pixel in anchor
			by MINIMUM pixel in this structure -> remove noise
	Dilate : choose a structure and one anchor point in the structure. Move the structure over the image, replace the pixel in anchor
			by MAXIMUM pixel in this structure -> fill hole
*/
using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("1.jpg", IMREAD_COLOR);
	if (!image.empty())
	{
		/*Mat eroded;
		Mat dilated;
		erode(image, eroded, Mat());
		dilate(image, dilated, Mat());
		imshow("Erode", eroded);
		imshow("Dilate", dilated);*/
		Mat closed;
		Mat opened;
		Mat gradient;
		//remove small hole (dark region)
		morphologyEx(image, closed, MORPH_CLOSE, Mat(5, 5, CV_8U, Scalar(0)));

		//remove small object (object is bright on dark foreground)
		morphologyEx(image, opened, MORPH_CLOSE, Mat(5, 5, CV_8U, Scalar(0)));

		morphologyEx(image, gradient, MORPH_GRADIENT, Mat());
		imshow("Original", image);
		imshow("Gradient", gradient);
		waitKey(0);
	}
	return 0;
}