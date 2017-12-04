#pragma  once
#include "stdafx.h"
#include "ExpAndHyperbolaFit.h"
// #include "..\..\Calibration\Common\CaliFunction.h"
#include "MatrixOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  DIMS	 7

CExpAndHyperbola::CExpAndHyperbola( CvPoint3D32f *pPoints,int nPoints )
:ChyperbolaFit(pPoints,nPoints)
{
	m_fa	=	0;
	m_fb	=	0;
	m_fc	=	0;
	m_fd	=	0;
	m_fe	=	0;
	m_ff	=	0;
	m_fg	=	0;
}

CExpAndHyperbola::~CExpAndHyperbola()
{

}
/*复合函数: 指数 + 双曲线模型
模型 : z = a*x^2+b*x.*y+c*y^2+d*x+e*y+f*exp(sqrt(x^2+y^2))+g
*/
BOOL CExpAndHyperbola::LeastSquare()
{
	CMatrix<double> dA(DIMS,DIMS);
	double dB[DIMS] = {0.0};
	double dX[DIMS] = {0.0};

	for ( int j=0; j<DIMS; j++ )
	{
		for ( int k=0; k<DIMS; k++ )
		{
			for ( int i=0; i< m_nPoints; i++ )
			{
				float x,y,z;
				x = m_pPoints[i].x;
				y = m_pPoints[i].y;
				z = m_pPoints[i].z;
				double dTemp[DIMS] = { x * x , x * y, y * y, x * x, y, exp(sqrt(x * x + y * y)), 1};

				dA[j][k] = dA[j][k] + dTemp[j] * dTemp[k];
			}
		}
	}

	for ( int i=0; i< DIMS; i++ )
	{
		for (int j=0; j< m_nPoints; j++ )
		{
			float x,y,z;
			x = m_pPoints[j].x;
			y = m_pPoints[j].y;
			z = m_pPoints[j].z;
			double dTemp[DIMS] = { x * x , x * y, y * y, x * x, y, exp(sqrt(x * x + y * y)), 1};
			dB[i] = dB[i] + dTemp[i] * z;
		}
	}
	CMatrix<double> invMatrix(DIMS,DIMS);
	if (! CMatrixOperate<double>::PseudoInv(dA,invMatrix) )
	{
		return FALSE;
	}

	for ( int i = 0; i < DIMS; i++ )
	{
		for ( int j = 0 ; j < DIMS ; j++ )
		{
			dX[i] += invMatrix[i][j] * dB[j];			
		}
	}
	m_fa	=	dX[0];
	m_fb	=	dX[1];
	m_fc	=	dX[2];
	m_fd	=	dX[3];
	m_fe	=	dX[4];
	m_ff	=	dX[5];
	m_fg	=	dX[6];

	CalcArea();
	return TRUE;
}

void CExpAndHyperbola::CalcArea()
{		
	int xmin,xmax,ymin,ymax;
	xmin = ymin = INT_MAX;
	xmax = ymax = INT_MIN;
	for(int  i=0;i<m_nPoints;i++ )
	{
		CvPoint3D32f pTemp = m_pPoints[i];
		if (xmin > pTemp.x)
		{
			xmin = pTemp.x;
		}
		if (xmax< pTemp.x)
		{
			xmax = pTemp.x;
		}
		if (ymin > pTemp.y)
		{
			ymin = pTemp.y;
		}
		if (ymax < pTemp.y)
		{
			ymax = pTemp.y;
		}
	}

	float fSum = 0;
	for ( int i= xmin; i< xmax; i++ )
	{
		for ( int j=ymin; j< ymax; j++ )
		{
			CvPoint2D32f cvPt;
			cvPt.x = i + 0.5f;
			cvPt.y = j + 0.5f;
			fSum = fSum + m_ff * exp(sqrt(  cvPt.x * cvPt.x +  cvPt.y * cvPt.y ));
		} 
	}

	float fLumi = 1/3 * m_fa * ( xmax * xmax * xmax - xmin * xmin * xmin ) * ( ymax - ymin )
		+ 1/4 * m_fb * ( xmax * xmax - xmin * xmin ) * ( ymax * ymax - ymin * ymin )
		+ 1/3 * m_fc * ( xmax - xmin ) * ( ymax * ymax * ymax - ymin * ymin * ymin )
		+ 1/2 * m_fd * ( xmax * xmax - xmin * xmin ) * ( ymax -ymin ) 
		+ 1/2 * m_fe * ( xmax - xmin ) * ( ymax * ymax - ymin * ymin )
		+ fSum
		+ m_fg	* ( xmax - xmin ) * ( ymax - ymin );
	ASSERT(fLumi > 0);

	m_luminate = fLumi;
}

void CExpAndHyperbola::Fit()
{
	LeastSquare();
}