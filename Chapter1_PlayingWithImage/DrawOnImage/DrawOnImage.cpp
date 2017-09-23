// LoadDisplaySaveImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"

using namespace std;
using namespace cv;

void OnMouse(int nEvent, int x, int y, int flag, void *param)
{
	cout << x << ", " << y << endl;
}

int main()
{
	Mat image;
	Mat logo;
	image = imread("1.jpg", IMREAD_COLOR);
	logo = imread("logo.jpg", IMREAD_COLOR);
	if (!image.empty() && !logo.empty())
	{
		Mat imageROI(image, Rect(image.cols - logo.cols, image.rows - logo.rows, logo.cols, logo.rows));
		logo.copyTo(imageROI);
		imshow("IOR", image);
	}
	waitKey(0);
	return 0;
}

