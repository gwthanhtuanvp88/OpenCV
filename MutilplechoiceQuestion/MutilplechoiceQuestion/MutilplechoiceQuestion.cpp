#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
		pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

bool comparatorCircleY(Vec3f a, Vec3f b)
{
	return a[1] < b[1];
}

bool comparatorCircleX(Vec3f a, Vec3f b)
{
	return a[0] < b[0];
}

bool comparator2(double a, double b) {
	return a<b;
}
bool comparator3(Vec3f a, Vec3f b) {
	return a[0]<b[0];
}

bool comparator(Point2f a, Point2f b) {
	return a.x<b.x;
}
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;
	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}

	sort(top.begin(), top.end(), comparator);
	sort(bot.begin(), bot.end(), comparator);

	cv::Point2f tl = top[0];
	cv::Point2f tr = top[top.size() - 1];
	cv::Point2f bl = bot[0];
	cv::Point2f br = bot[bot.size() - 1];
	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}

int main() 
{
	Mat img;
	img = imread("example.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//namedWindow("Before");
	namedWindow("After");
	//imshow("Before", img);
	GaussianBlur(img, img, Size(3, 3), 0);
	adaptiveThreshold(img, img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
	bitwise_not(img, img);

	Mat img2;
	cvtColor(img, img2, CV_GRAY2BGR);
	Mat imgColor;
	cvtColor(img, imgColor, CV_GRAY2BGR);
	vector<Vec4i> lines;
	HoughLinesP(img, lines, 1, CV_PI/180, 80, 400, 10);
	for (int i = 0; i < lines.size(); i++)
	{
		Vec4i points = lines[i];
		line(imgColor, Point(points[0], points[1]), Point(points[2], points[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
	//imshow("After", imgColor);
	vector<Point2f> vCorners;
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = i+1; j < lines.size(); j++)
		{
			Point2f p = computeIntersect(lines[i], lines[j]);
			if (p.x >= 0 && p.y >= 0 && p.x < img.cols && p.y < img.rows)
			{
				vCorners.push_back(p);
			}
		}
	}
	Point2f pCenter;
	for (int i = 0; i < vCorners.size(); i++)
	{
		pCenter += vCorners[i];
	}
	pCenter *= (1.0 / vCorners.size());
	sortCorners(vCorners, pCenter);

	Rect r = boundingRect(vCorners);
	Mat quad = Mat::zeros(r.height, r.width, CV_8UC3);
	vector<Point2f> vQuad_Pts;

	vQuad_Pts.push_back(Point2f(0, 0));
	vQuad_Pts.push_back(Point2f(quad.cols, 0));
	vQuad_Pts.push_back(Point2f(quad.cols, quad.rows));
	vQuad_Pts.push_back(Point2f(0, quad.rows));
	Mat tranMtx = getPerspectiveTransform(vCorners, vQuad_Pts);
	warpPerspective(img2, quad, tranMtx, quad.size());
	imshow("After", quad);
	Mat cImg;
	Mat cImgBinary;
	cvtColor(quad, cImg, CV_BGR2GRAY);
	//imshow("After", quad);
	adaptiveThreshold(cImg, cImgBinary, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
	vector<Vec3f> circles;
	//imshow("After", cImg);
	HoughCircles(cImg, circles, CV_HOUGH_GRADIENT, 1, cImg.rows / 8, 75, 60);
	sort(circles.begin(), circles.end(), comparatorCircleY);
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		circle(quad, center, circles[i][2], Scalar(0, 255, 0), -1, 8, 0);
	}
	imshow("quad", quad);
	vector<vector<Vec3f>> vCircles(5);
	int nY = circles[0][1];
	int nRow = 0;
	for (int i = 0; i < circles.size(); i++)
	{
		if (circles[i][1] + circles[i][2] / 2 > nY && circles[i][1] - circles[i][2] / 2 < nY)
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
			Mat subMat = cImg(Rect(x - r, y - r, 2 * r, 2 * r));
			int nNonZero = countNonZero(subMat);
			int nSize = subMat.size().width * subMat.size().height;
			double d = (double)countNonZero(subMat) / (subMat.size().width * subMat.size().height);
			if (d > 0.3)
			{
				cout << " answer : " << char('A' + j);
			}
		}
		cout << endl;
	}
	//imshow("After", quad);
	waitKey(0);
	return 0;
}