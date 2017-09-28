#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

bool comparatorCircleX(Vec3f a, Vec3f b)
{
	return a[0] < b[0];
}

bool comparatorCircleY(Vec3f a, Vec3f b)
{
	return a[1] < b[1];
}


int main()
{
	Mat image = imread("sheet.jpg", IMREAD_GRAYSCALE);
	if (!image.empty())
	{
		GaussianBlur(image, image, Size(3, 3), 0);
		//threshold(image, image, 180, 255, THRESH_BINARY_INV);
		adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
		bitwise_not(image, image);
		//So bao danh
		//Mat smallOne(image, Rect(1703, 424, 315, 773));
		//Ma de thi
		//Mat smallOne(image, Rect(2073, 424, 167, 773));
		//1-5
		Mat smallOne(image, Rect(352, 1432, 673, 365));
		imshow("Small", smallOne);
		Mat imgColor;
		cvtColor(smallOne, imgColor, CV_GRAY2BGR);
		vector<Vec3f> circles;
		//So bao danh
		//HoughCircles(smallOne, circles, CV_HOUGH_GRADIENT, 1, smallOne.rows / 16, 100, 10, 10, 40);
		//Ma de thi
		HoughCircles(smallOne, circles, CV_HOUGH_GRADIENT, 1, smallOne.rows / 8, 100, 10, 20, 40);
		//sort(circles.begin(), circles.end());
		for (size_t i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			circle(imgColor, center, circles[i][2], Scalar(0, 255, 0), -1, 8, 0);
		}
		imshow("Circle", imgColor);
		sort(circles.begin(), circles.end(), comparatorCircleY);
		vector<vector<Vec3f>> vCircles(5);
		int nY = circles[0][1];
		int nRow = 0;
		for (int i = 0; i < circles.size(); i++)
		{
			if (circles[i][1] + circles[i][2]> nY && circles[i][1] - circles[i][2]< nY)
			{
				vCircles[nRow].push_back(circles[i]);
			}
			else
			{
				nRow++;
				vCircles[nRow].push_back(circles[i]);
				nY = circles[i][1];
			}
		}
		for (int i = 0; i < vCircles.size(); i++)
		{
			sort(vCircles[i].begin(), vCircles[i].end(), comparatorCircleX);
			cout << "Question " << i;
			int nCurMax = 0;
			for (int j = 0; j < vCircles[i].size(); j++)
			{
				Vec3f circle = vCircles[i][j];
				int x = circle[0];
				int y = circle[1];
				int r = circle[2];
				Mat subMat = smallOne(Rect(x - r, y - r, 2 * r, 2 * r));
				int nNonZero = countNonZero(subMat);
				int nSize = subMat.size().width * subMat.size().height;
				double d = (double)countNonZero(subMat) / (subMat.size().width * subMat.size().height);
				if (d > 0.6)
				{
					cout << " answer : " << char('A' + j);
				}
			}
			cout << endl;
		}
		waitKey(0);
	}
	return 0;
}