#pragma once
#include "stdafx.h"
#include "HyperbolaFit.h"
/*#include "..\..\Calibration\Common\CaliFunction.h"*/
#include "MatrixOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ChyperbolaFit::ChyperbolaFit( CvPoint3D32f *pPoints,int nPoints ): CLEDModel(pPoints,nPoints)
{
	m_nPoints	= nPoints;
	m_pPoints	= new CvPoint3D32f[nPoints];
	for(int i = 0; i < nPoints; i++)
	{
		m_pPoints[i].x = pPoints[i].x;
		m_pPoints[i].y = pPoints[i].y;
		m_pPoints[i].z = pPoints[i].z;
	}
}

ChyperbolaFit::~ChyperbolaFit()
{
	if ( m_pPoints )
	{
		delete m_pPoints;
		m_pPoints = NULL;
	}
}

void ChyperbolaFit::Fit()
{	
	LeastSquare();
}


/*
双曲线 最小二乘法
通过求导，可以转化为下面的形式:  AX = B
其中,  X为(a,b,c,d)， 即四个参数
*/
BOOL ChyperbolaFit::LeastSquare()
{
	CMatrix<double>  dA(PARAM_NUM,PARAM_NUM);
	double dB[PARAM_NUM] = {0.0};
	double dX[PARAM_NUM] = {0.0};
	
	for ( int i = 0; i< m_nPoints; i++ )
	{
		float x,y,z;
		x = m_pPoints[i].x;
		y = m_pPoints[i].y;
		z = m_pPoints[i].z;
		dA[0][0] +=  x * x + y * y;
		dA[0][1] +=  x * y + x;
		dA[0][2] +=  y;
		dA[0][3]  =  m_nPoints;

		dA[1][0] +=  y * ( x * x + y * y );
		dA[1][1] +=  y * ( x * y + x );
		dA[1][2] +=  y * y;
		dA[1][3] +=  y;

		dA[2][0] += ( x * x + y * y ) * ( x * y + x );
		dA[2][1] += ( x * y + x * x ) * ( x * y + x * x );
		dA[2][2] +=  y * ( x * y + x );
		dA[2][3] +=  x * y + x;
		
		dA[3][0] += ( x * x + y * y ) * ( x * x + y * y );
		dA[3][1] += ( x * y + x ) * ( x * x  + y * y );
		dA[3][2] += y * ( x * x + y * y );
		dA[3][3] += x * x + y * y;

	}

	CMatrix<double> invMatrix(dA.Size());
	if ( !CMatrixOperate<double>::PseudoInv(dA,invMatrix) )
	{
		ASSERT(0);
		return FALSE;
	}
	for ( int i = 0;i < m_nPoints; i++ )
	{
		float x,y,z;
		x = m_pPoints[i].x;
		y = m_pPoints[i].y;
		z = m_pPoints[i].z;
		dB[0] += z;
		dB[1] += y * z;
		dB[2] += z * ( x * y + x * x );
		dB[3] += z * ( x * x + y * y );
	}
	
	for ( int i = 0; i < PARAM_NUM; i++ )
	{
		for ( int j = 0 ; j < PARAM_NUM ; j++ )
		{
			dX[i] += invMatrix[i][j] * dB[j];			
		}
	}

	m_fa = dX[0];
	m_fb = dX[1];
	m_fc = dX[2];
	m_fd = dX[3];

	CalcArea();

	return TRUE;
}

void ChyperbolaFit::CalcArea()
{
	ASSERT(m_fa && m_fb && m_fc && m_fd);
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
	float fLumi = 1/3 * m_fa * ( xmax * xmax * xmax - xmin * xmin * xmin ) * ( ymax - ymin )
				 + 1/4 * m_fb * ( xmax * xmax - xmin * xmin ) * ( ymax * ymax - ymin * ymin )
				 + 1/3 * m_fa * ( xmax - xmin ) * ( ymax * ymax * ymax - ymin * ymin * ymin )
				 + 1/2 * m_fb * ( xmax * xmax - xmin * xmin ) * ( ymax -ymin ) 
				 + 1/2 * m_fc * ( xmax - xmin ) * ( ymax * ymax - ymin * ymin )
				 + m_fd	* ( xmax - xmin ) * ( ymax - ymin );

	ASSERT( fLumi > 0 );
// #ifdef DEBUG
// 	TRACE(_T("Lumi:%lf\n"),fLumi);	
// 	CString strFilePath,strFile;
// 	strFilePath = _T("D:\\MyLumi.txt");
// 	strFile.Format(_T("%lf\r\n"),fLumi);
// 
// 	CFile file;
// 	file.Open(strFilePath, CFile::modeReadWrite );
// 
// 	file.SeekToEnd();
// 	file.Write(strFile.GetBuffer(), sizeof(TCHAR) * strFile.GetLength());
// 	file.Close();
// #endif
	m_luminate = fLumi;
}