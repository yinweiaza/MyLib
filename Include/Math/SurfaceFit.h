#pragma once

#include "LEDModel.h"

enum  SupportAlgf{
	Alg_TurstRegion = 0,
	Alg_Levenberg,
	Alg_LeastSquare,
};
class CSurfaceFit: public CLEDModel
{
public:	
	CSurfaceFit(CvPoint3D32f *pPoints,int nPoints);	
	virtual ~CSurfaceFit();
protected:
	CvPoint3D32f *		m_pPoints;		//数据
	int					m_nPoints;     //总点数
	UINT				m_Alg;	
public:
	void	SetAlgorithm(IN UINT Method);

	virtual	BOOL  Fit(IN UINT Method);
protected:
	BOOL  TrustRegion();
	BOOL  Levenberg();
	virtual BOOL  LeastSquare();
	BOOL  CalcArea();

private:
	float		m_fa;
	float		m_fb;

};