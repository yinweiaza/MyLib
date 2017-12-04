#pragma once

#include "LEDModel.h"
class CConeModel: public CLEDModel
{
public:
	CConeModel(CvPoint3D32f* pPoints, int nPoints);
	~CConeModel();
};