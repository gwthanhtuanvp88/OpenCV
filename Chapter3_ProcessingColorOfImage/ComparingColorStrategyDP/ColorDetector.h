#pragma once
#include <vector>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
using namespace cv;
using namespace std;
class CColorDetector
{
private:
	Scalar m_targetColor;
	uint m_MaxDistance;
	uint getDistanceToTargeColor(Scalar input);
public:
	CColorDetector();
	~CColorDetector();
	void SetMaxDistance(uint maxDistance);
	void setTargetColor(int red, int gray, int blue);
	void process(Mat & input, Mat & output);
};

