#include "stdafx.h"
#include "ConeModel.h"

#define _USE_MATH_DEFINES
#include <math.h>

CConeModel::CConeModel( CvPoint3D32f* pPoints, int nPoints )
:CLEDModel(pPoints, nPoints)
{
	//ģ��ΪԲ׶���棺z = b * sqrt(x^2+y^2) + a
	//��� = sqrt(x^2+y^2)
	//z = b�� + a Ϊĸ��
	//��Ŀ��������ĸ�ߣ���С���ˣ�

	ASSERT(nPoints >= 2);				//������Ҫ2���㱣֤���

	//��ȡ��
	double* pU = new double[nPoints];
	for(int i = 0; i < nPoints; i++)
	{
		float Xi = pPoints[i].x;
		float Yi = pPoints[i].y;
		pU[i]	= sqrtf(Xi * Xi + Yi * Yi);
	}

	//���Իع�
	double sigmaU2	= 0.0f;
	double sigmaZ	= 0.0f;
	double sigmaU	= 0.0f;
	double sigmaUZ	= 0.0f;
	for(int i = 0; i < nPoints; i++)
	{
		float Zi = pPoints[i].z;
		sigmaU2 += pU[i] * pU[i];
		sigmaZ	+= Zi;
		sigmaU	+= pU[i];
		sigmaUZ	+= pU[i] * Zi;
	}

	double a =	(sigmaU2 * sigmaZ - sigmaU * sigmaUZ)
				/(nPoints * sigmaU2 - sigmaU * sigmaU);

	double b =	(nPoints * sigmaUZ - sigmaU * sigmaZ)
				/(nPoints * sigmaU2 - sigmaU * sigmaU);

#ifdef _DEBUG
	//�ع�������ϵ��
	double avgU	= sigmaU / nPoints;
	double avgZ	= sigmaZ / nPoints;

	double sigmaDeltaU2 = 0.0f;
	double sigmaDeltaZ2 = 0.0f;
	double sigmaDeltaUZ = 0.0f;

	for(int i = 0; i < nPoints; i++)
	{
		float Zi = pPoints[i].z;
		
		sigmaDeltaU2 += (pU[i] - avgU) * (pU[i] - avgU);
		sigmaDeltaZ2 += (Zi	   - avgZ) * (Zi    - avgZ);
		sigmaDeltaUZ += (pU[i] - avgU) * (Zi    - avgZ);
	}

	double correlationCoef = sigmaDeltaUZ / sqrtf(sigmaDeltaU2 * sigmaDeltaZ2);
#endif // _DEBUG
	SafeDelete(pU);

	ASSERT(a > 0);
	ASSERT(b < 0);
	//�������Ϊ����
	double r = -a / b;
	double S = M_PI * r * r;
	double h = a;

	//m_luminate = S * h / 3;
	m_luminate = h;
}

CConeModel::~CConeModel()
{

}