// LoadDisplaySaveImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat image;
	image = imread("1.jpg", IMREAD_GRAYSCALE);
	if (!image.empty())
	{
		//circle(image, cvPoint(100, 100), 30);
		imshow("image", image);
	}
	waitKey(0);
    return 0;
}

