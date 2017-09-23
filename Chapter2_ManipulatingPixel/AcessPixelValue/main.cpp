#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

// address of pixel at (j,i) that is &image.at(j,i)
//data = image.data + j*image.step + i*image.elemSize();

using namespace cv;
using namespace std;

void Salt(Mat image, int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		int r = rand() % image.rows - 1;
		int c = rand() % image.cols - 1;
		if (image.type() == CV_8UC1)
		{
			image.at<uchar>(r, c) = 255;
		}
		else if (image.type() == CV_8UC3)
		{
			image.at<Vec3b>(r, c)[0] = 255;
			image.at<Vec3b>(r, c)[1] = 255;
			image.at<Vec3b>(r, c)[2] = 255;
		}
	}
}

void ColorReduce(Mat image, int factor = 64)
{
	int nL = image.rows;
	int nC = image.cols * image.channels();
	for (int i = 0; i < nL; i++)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = 0; j < nC; j++)
		{
			data[j] = (data[j] / factor)*factor + factor / 2;
		}
	}
}

void Sharpen(Mat input, )

int main()
{
	Mat image;
	image = imread("1.jpg", IMREAD_COLOR);
	if (!image.empty())
	{
		//Salt(image, 1000);
		ColorReduce(image, 16);
		imshow("salt image", image);
	}
	waitKey(0);
}