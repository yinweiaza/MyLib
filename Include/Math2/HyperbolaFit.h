#pragma once

#include "SurfaceFit.h"

#define  PARAM_NUM	4
/*
双曲线模型: z = a*x^2+b*x.*y+a*y^2+b*x+c*y+d 
*/
class ChyperbolaFit:public CLEDModel
{
public:
	ChyperbolaFit(CvPoint3D32f *pPoints,int nPoints);
	~ChyperbolaFit();
protected:
	virtual BOOL  LeastSquare();
	virtual void  CalcArea();
public:
	virtual void  Fit();
	
	CvPoint3D32f *		m_pPoints;		//数据
	int					m_nPoints;     //总点数

private:
	float  m_fa;
	float  m_fb;
	float  m_fc;
	float  m_fd;

};