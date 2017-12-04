#pragma once
#include <assert.h>
#include <string.h>
#include "Matrix.h"
using namespace std;


//  Description:
//		��������ࣻ
//================================================================================

//////////////////////////////////////////////////////////////////////////////////
//  Edit history:
//      (After updating this file(New, Add, Remove, Modify, Bugfix),
//       list here please.)
//================================================================================
//  ______________________________________________________________________________
//  |   |                Remark                         |             |          |
//  |No.|_______________________________________________|   Author    |   Date   |
//  |   |  Mode  |          Description                 |             |          |
//  +---+--------+--------------------------------------+-------------+----------+
//  |001| New    | Newly created.                       |   �� ��     |2017/02/22|
//  +---+--------+--------------------------------------+-------------+----------+
//  |002|        |                                      |             |          |
//  +----------------------------------------------------------------------------+
//________________________________________________________________________________
//////////////////////////////////////////////////////////////////////////////////

template<class T>
class CMatrixOperate
{
public: 
	CMatrixOperate();
	~CMatrixOperate();
public:
	//�������;
	static bool  MatrixMul(IN CMatrix<T>& mulMat1, IN CMatrix<T>& mulMat2,OUT CMatrix<T> &resMat);
	//�������
	bool		 MatrixAdd(IN CMatrix<T>& mulMat1, IN CMatrix<T>& mulMat2,OUT CMatrix<T> &resMat);
	//����;
	static bool  PseudoInv(CMatrix<T>& inputMatrix,CMatrix<T> &outPutMatrix );
	//����η�����;
	static bool  SolveEqu(CMatrix<T>& inputMatrix,T* InputVec,T* pOutputVec);

protected:
	static void  Solve(CMatrix<T>& inputMatrix,T* pInputVec,T* pOutPutVec);

};


template<class T>
bool CMatrixOperate<T>::MatrixAdd(IN CMatrix<T>& mulMat1, IN CMatrix<T>& mulMat2,OUT CMatrix<T> &resMat)
{

}

typedef CMatrixOperate<double> CMatrixOperateD;
typedef CMatrixOperate<float> CMatrixOperateF;

template<class T>
bool CMatrixOperate<T>::MatrixMul(IN CMatrix<T>& mulMat1,IN CMatrix<T>& mulMat2,OUT CMatrix<T> &resMat )
{
	SIZE mulSize1 = mulMat1.Size();
	SIZE mulSize2 = mulMat2.Size();
	SIZE resSize = resMat.Size();
	assert( mulSize1.cx == mulSize2.cy 
		 && resSize.cy == mulSize1.cy 
		 && resSize.cx == mulSize2.cx );
	if ( mulSize1.cx != mulSize2.cy 
		&& resSize.cy != mulSize1.cy 
		&& resSize.cx != mulSize2.cx )
	{
		assert(0);
		return false;
	}
	for ( int i=0; i<mulSize1.cy; i++ )
	{
		for ( int j=0; j<mulSize2.cx; j++)
		{
			T temp = (T)0;
			for ( int k = 0; k< mulSize1.cx; k++ )
			{
				temp += mulMat1.GetElementByIndex(i,k) * mulMat2.GetElementByIndex(k,j);
			}
			resMat.SetElement(i,j,temp);
		}
	}
	return true;
}

template<class T>
bool CMatrixOperate<T>::PseudoInv(CMatrix<T>& inputMatrix,CMatrix<T> &outPutMatrix )
{
	//��ʱ����;
	CMatrix<T> tempMatrix(inputMatrix.Size());
	tempMatrix = inputMatrix;
	//��λ����
	outPutMatrix.SetUnitMatrix();

	SIZE matrixDim = inputMatrix.Size();
	assert( matrixDim.cx == matrixDim.cy );
	int dim  = matrixDim.cx;
	for (int i =0; i< dim; i++ )		// ��;
	{
		T temp = tempMatrix[i][i];
		for ( int j = 0; j< dim; j++ )
		{
			tempMatrix[i][j] /= temp;
			outPutMatrix[i][j] /= temp;
		}

		for ( int k =0; k< dim; k++ )
		{
			if ( i == k)
			{
				continue;
			}
			temp = tempMatrix[k][i];
			for ( int j = 0; j< dim; j++ )
			{
				tempMatrix[k][j] = tempMatrix[k][j] - tempMatrix[i][j] * temp;
				outPutMatrix[k][j] = outPutMatrix[k][j] - outPutMatrix[i][j] * temp;
			}
		}
	}

	return true;
}
template<class T>
bool CMatrixOperate<T>::SolveEqu(CMatrix<T>& inputMatrix,T* pInputVec,T* pOutputVec )
{
	SIZE matrixDim = inputMatrix.Size();
	CMatrix<T> outputMat(matrixDim.cy,matrixDim.cx);
	if (!PseudoInv(inputMatrix,outputMat))
	{
		return false;
	}
	Solve(outputMat,pInputVec,pOutputVec);
	return true;
}

template<class T>
CMatrixOperate<T>::CMatrixOperate()
{

}

template<class T>
CMatrixOperate<T>::~CMatrixOperate()
{

}

template<class T>
void CMatrixOperate<T>::Solve(CMatrix<T>& inputMatrix,T* pInputVec,T* pOutPutVec )
{
	SIZE nMatDim = inputMatrix.Size();

	for ( int i =0 ;i < nMatDim.cy; i++ )
	{
		T temp = (T)0;
		for ( int j=0;j< nMatDim.cx; j++ )
		{
			temp += inputMatrix[i][j] * pInputVec[j];
		}
		pOutPutVec[i] = temp;
	}
}