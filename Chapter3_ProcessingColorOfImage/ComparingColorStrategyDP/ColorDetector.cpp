#include "ColorDetector.h"

uint CColorDetector::getDistanceToTargeColor(Scalar input)
{
	return abs(m_targetColor[0]-input[0])
		+ abs(m_targetColor[1] - input[1])
		+ abs(m_targetColor[2] - input[2]);
}

CColorDetector::CColorDetector() :	m_MaxDistance(100),
									m_targetColor(Scalar(0))
{

}


CColorDetector::~CColorDetector()
{

}

void CColorDetector::SetMaxDistance(uint maxDistance)
{
	if (maxDistance < 0)
		maxDistance = 0;
	m_MaxDistance = maxDistance;
}

void CColorDetector::setTargetColor(int red, int gray, int blue)
{
	m_targetColor = CV_RGB(red, gray, blue);
}

void CColorDetector::process(Mat & input, Mat &output)
{
	absdiff(input, m_targetColor, output);
	vector<Mat> images;
	split(output, images);
	output = images[0] + images[1] + images[2];
	threshold(output, output, m_MaxDistance, 255, THRESH_BINARY);
	/*output.create(input.size(), CV_8U);
	Mat_<Vec3b>::const_iterator it = input.begin<Vec3b>();
	Mat_<Vec3b>::const_iterator itend = input.end<Vec3b>();
	Mat_<uchar>::iterator out = output.begin<uchar>();
	for (; it != itend; it++, out++)
	{
		if (getDistanceToTargeColor(*it) <= m_MaxDistance)
		{
			*out = 255;
		}
		else
		{
			*out = 0;
		}
	}*/
}
