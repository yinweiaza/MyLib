#pragma  once
#include "stdafx.h"
#include "SurfaceFit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSurfaceFit::CSurfaceFit(CvPoint3D32f *pPoints, int nPoints )
:CLEDModel(pPoints,nPoints)
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

CSurfaceFit::~CSurfaceFit()
{

	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = NULL;
	}
}

void CSurfaceFit::SetAlgorithm( IN UINT Method )
{
	m_Alg = Method;
}

BOOL CSurfaceFit::Fit(IN UINT Method)
{
	ASSERT( m_pPoints && m_nPoints );
	SetAlgorithm(Method);
	switch( m_Alg )
	{
	case Alg_TurstRegion:
		TrustRegion();
		break;
	case Alg_Levenberg:
		Levenberg();
		break;
	case Alg_LeastSquare:
		LeastSquare();
		break;
	default:
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

BOOL CSurfaceFit::TrustRegion()
{	
	ASSERT(0);
	return T9E_NoImplement;
}

BOOL CSurfaceFit::Levenberg()
{
	ASSERT(0);
	return T9E_NoImplement;
}

//void CSurfaceFit::SetData( CvPoint3D32f *pPoint,int index )
//{
//	m_pPoint[index].x = pPoint->x;
//	m_pPoint[index].y = pPoint->y;
//	m_pPoint[index].z = pPoint->z;
//}

/*
总共有两个参量-----采用的是抛物线
模型为：z = a + b*(x*x + y*y)    将x*x + y*y 看成X  z看成Y  则模型简化为: Y=a+b*X    
参量为: a,b
*/
BOOL CSurfaceFit::LeastSquare()
{
	ASSERT( m_pPoints && m_nPoints );
	double * pTemp;    //用来保存(x^2+y^2) 
	pTemp  = new double[m_nPoints];

	for (int i = 0; i<m_nPoints; i++ )
	{
		pTemp[i]  = pow((double)m_pPoints[i].x,2) + pow((double)m_pPoints[i].y,2);

	}
	double   nXXSum =0;
	double	 nYSum	=0;
	double	 nXSum	=0;
	double	 nXYSum =0;
	for ( int  i = 0 ; i< m_nPoints; i++ )
	{
		double nX = pTemp[i];
		double nY = m_pPoints[i].z;
		nXYSum += nX * nY;
		nYSum  += nY;
		nXSum  += nX;
		nXXSum += nX * nX;
	}
	m_fa = 1.0f * (nXXSum * nYSum  - nXSum * nXYSum)
			   /(m_nPoints * nXXSum - nXSum * nXSum);
	
	m_fb = 1.0f * (m_nPoints * nXYSum - nXSum * nYSum)
			  /(m_nPoints * nXXSum - nXSum * nXSum);
	ASSERT(m_fa && m_fb);
	delete[] pTemp;
	return CalcArea();
}

BOOL CSurfaceFit::CalcArea(  )
{
	ASSERT(m_fa && m_fb);
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
	//积分   体积
	double fLumi =  ( 1/3 * m_fb * (( xmax * xmax * xmax -  xmin * xmin * xmin) * ( ymax - ymin) 
				+  ( ymax * ymax * ymax - ymin * ymin * ymin )*( xmax - xmin )) + m_fa * (xmax - xmin) * (ymax -ymin));

// #ifndef DEBUG
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


  	ASSERT(fLumi>0);
	m_luminate = fLumi;
	return TRUE;
}