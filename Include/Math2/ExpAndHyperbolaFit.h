#pragma  once
#include "HyperbolaFit.h"

class CExpAndHyperbola : public ChyperbolaFit
{
public:
	CExpAndHyperbola(CvPoint3D32f *pPoints,int nPoints);
	virtual ~CExpAndHyperbola();
protected:
	virtual BOOL  LeastSquare();
	virtual void  CalcArea();
public:
	virtual void  Fit();
private:
	float  m_fa;
	float  m_fb;
	float  m_fc;
	float  m_fd;
	float  m_fe;
	float  m_ff;
	float  m_fg;
};