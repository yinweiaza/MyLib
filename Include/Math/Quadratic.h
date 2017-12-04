#pragma once

#if(1500 == _MSC_VER)
#include "..\OpenSource\OpenCV\cxtypes.h"
#include "..\OpenSource\OpenCV\highgui.h"
#include "..\OpenSource\OpenCV\cxcore.h"
#include "..\OpenSource\OpenCV\cv.h"
#else
#include "..\OpenSource\OpenCV\OpenCV2.4.11\opencv.hpp"
#endif

class CQuadratic
{
public:
	CQuadratic(const cv::Point2f &p1, const cv::Point2f &p2,  const cv::Point2f &p3);
	~CQuadratic(void);
	
	cv::Point2f GetVertex();

	float m_a;
	float m_b;
	float m_c;
};
