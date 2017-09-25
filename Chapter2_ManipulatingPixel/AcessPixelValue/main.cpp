#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

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

void Sharpen(Mat &input, Mat &output)
{
	output.create(input.size(), input.type());
	int nChannel = input.channels();
	for (int i = 1; i < input.rows - 1; i++)
	{
		uchar *previous = input.ptr<uchar>(i - 1);
		uchar *current = input.ptr<uchar>(i);
		uchar *next = input.ptr<uchar>(i + 1);
		uchar *out = output.ptr<uchar>(i);
		for (int j = nChannel; j < (input.cols - 1)*nChannel; j++)
		{
			out[j] = saturate_cast<uchar>(5 * current[j] - previous[j] - next[j] - current[j - nChannel] - current[j + nChannel]);
		}
	}
	output.row(0).setTo(Scalar(0));
	output.row(input.rows-1).setTo(Scalar(0));
	output.col(0).setTo(Scalar(0));
	output.col(input.cols - 1).setTo(Scalar(0));
}

void Wave(Mat &input, Mat &output)
{
	Mat srcX(input.rows, input.cols, CV_32F);
	Mat srcY(input.rows, input.cols, CV_32F);
	for (int i = 0; i < input.rows; i++)
	{
		for (int j = 0; j < input.cols; j++)
		{
			srcX.at<float>(i, j) = j;
			srcY.at<float>(i, j) = i+5*sin(j/10);
		}
	}
	remap(input, output, srcX, srcY, CV_INTER_LINEAR);
}

int main()
{
	Mat image;
	Mat output;
	Mat imageWave;
	image = imread("1.jpg", IMREAD_COLOR);
	if (!image.empty())
	{
		//Salt(image, 1000);
		//ColorReduce(image, 16);
		imshow("Original image", image);
		Sharpen(image, output);
		imshow("Sharpen image", output);
		Wave(image, imageWave);
		imshow("Wave", imageWave);
	}
	waitKey(0);
}