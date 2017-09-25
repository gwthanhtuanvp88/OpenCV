#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("1.jpg", IMREAD_GRAYSCALE);
	if (!image.empty())
	{
		Mat contour;
		Canny(image, contour, 125, 350);
		imshow("Original", image);
		imshow("Canny", contour);
		waitKey(0);
	}
	return 0;
}