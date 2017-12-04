#pragma once

#if(1500 == _MSC_VER)
#include "..\OpenSource\OpenCV\cxtypes.h"
#include "..\OpenSource\OpenCV\highgui.h"
#include "..\OpenSource\OpenCV\cxcore.h"
#include "..\OpenSource\OpenCV\cv.h"
#else
#include "..\OpenSource\OpenCV\OpenCV2.4.11\opencv.hpp"
#endif

class CLEDModel
{
public:
	CLEDModel(CvPoint3D32f* pPoints, int nPoints);
	virtual ~CLEDModel();
	float	GetLuminate(){return m_luminate;};
protected:
	float m_luminate;
};