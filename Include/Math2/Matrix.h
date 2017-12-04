#pragma once

//  Description:
//		 ͨ�ö�ά������
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
//  |001| New    | Newly created.                       |   �� ��     |2017/02/23|
//  +---+--------+--------------------------------------+-------------+----------+
//  |002|        |                                      |             |          |
//  +----------------------------------------------------------------------------+
//________________________________________________________________________________
//////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <assert.h>
#include <string>
#include <vector>
using namespace std;


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ������
 *
 * @tparam T
 */
/* ----------------------------------------------------------------------------*/
template<class T>
class CMatrix
{
public:
	CMatrix(){ m_nDot = 0;m_ppMatrix = NULL;};
	CMatrix(SIZE dim);
	explicit CMatrix(int nRowDim,int nColDim);
	virtual ~CMatrix();
protected:
	SIZE   m_matrixDim;  //ά��      cx:  ����ά��    cy: ����ά��;
	T**	   m_ppMatrix;	//��ά����
public:
	SIZE   Size()      { return m_matrixDim;		}
	
	CMatrix<T> &operator=( CMatrix<T> &other );
	/*
		������<< ,�����;
		����
		   CMatrix<double> matrix(3,3);
		   matrix<< 1,1,1,
		            2,1,3,
					4,-1,3;
	*/
	CMatrix<T> &operator<<(T data);
	CMatrix<T> &operator,(T data);
	T*			operator[](int index);

	CMatrix<T>&  operator+ (const CMatrix<T> &other);
	CMatrix<T>  operator* (CMatrix<T> &other);
	//��ʽ��Ϊ��λ����
	void  SetUnitMatrix();

	//�趨ά�ȣ�
	void  SetSize(int row,int col)
	{
		m_matrixDim.cx = col;
		m_matrixDim.cy = row;
		Make2DArray(m_ppMatrix,m_matrixDim.cy,m_matrixDim.cx);
	}

	//��ȡԪ��
	T	GetElementByIndex(int row,int col)
	{
		ASSERT( row < m_matrixDim.cy && col < m_matrixDim.cx);
		return m_ppMatrix[row][col];
	}

	//����Ԫ��
	void	SetElement(int row,int col, T element )
	{
		ASSERT( row < m_matrixDim.cy && col < m_matrixDim.cx);
		m_ppMatrix[row][col] = element;
	}


protected:
	virtual bool AllocMem();	
private:
	int	 m_nDot;			//���Ÿ���
	bool m_bAlloc;			//�Ƿ����������ڴ棻
};

template<class T>
CMatrix<T> CMatrix<T>::operator*(CMatrix<T> &other)
{
	SIZE matSize1 = m_matrixDim;
	SIZE matSize2 = other.Size();
	assert(matSize1.cx == matSize2.cy
		);
	CMatrix<T> matr(matSize1.cy,matSize2.cx);
 	for (int i = 0;i < matSize1.cy; i++ )		//�У�
 	{
 		for (int k = 0;k < matSize2.cx; k++ )	//�У�
 		{
 			matr[i][k] = (T)0.0f;
 			for (int j = 0;j < matSize1.cx; j++ )
 			{
 				matr[i][k] += m_ppMatrix[i][j] * other[j][k];
 			}
 		}
 	}
	return matr;
}
/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  �����ڴ�
 *
 * @tparam T
 *
 * @Returns �Ƿ�����ɹ�  
 */
/* ----------------------------------------------------------------------------*/
template<class T>
bool CMatrix<T>::AllocMem()
{
	if (!m_bAlloc) {
		Make2DArray(m_ppMatrix,m_matrixDim.cy,m_matrixDim.cx);
		m_bAlloc = true;
		return true;
	}else{
		return false;
	}
}

typedef CMatrix<double>	CMatrixD;
typedef CMatrix<float>	CMatrixF;
typedef CMatrix<int>	CMatrixI;



/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  �������
 *
 * @tparam T
 * @Param mat1
 * @Param mat2
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T>&  CMatrix<T>::operator+(const CMatrix<T> &other)
{
	SIZE matSize1 = m_matrixDim;
	SIZE matSize2 = other.Size();
	assert(matSize1.cx == matSize2.cx 
		&& matSize1.cy == matSize2.cy
		);
	CMatrix<T> matr(matSize1.cy,matSize2.cx);
	for (int i = 0; i < matSize1.cx; i++ )
	{
		for (int j=0; j < matSize1.cy; j++ )
		{
			m_ppMatrix[j][i] += other[j][i];
		}
	}
	return *this;
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��ȡԪ��
 *
 * @tparam T
 * @Param index
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
template<class T>
T* CMatrix<T>::operator[]( int index )
{
	ASSERT( index < m_matrixDim.cy );
	return reinterpret_cast<T*>(m_ppMatrix[index]);
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ����������
 *
 * @tparam T
 * @Param data  ֵ
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T> & CMatrix<T>::operator,( T data )
{
	m_nDot++;
	m_ppMatrix[m_nDot/m_matrixDim.cx][m_nDot%m_matrixDim.cx] = data;						//�У��У�
	return *this;
}
template<class T>
CMatrix<T> & CMatrix<T>::operator<<( T data )
{
	assert(NULL !=m_ppMatrix);
	m_ppMatrix[m_nDot][m_nDot] = data;
	return *this;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��ʽ����λ����
 *
 * @tparam T
 */
/* ----------------------------------------------------------------------------*/
template<class T>
void CMatrix<T>::SetUnitMatrix()
{
	assert(m_ppMatrix);
	int nMin = min(m_matrixDim.cx,m_matrixDim.cy);
	for ( int i =0; i< m_matrixDim.cx; i++ )
	{
		for ( int j =0; j< m_matrixDim.cy; j++ )
		{
			if ( i == j && i< nMin && j < nMin )
			{
				m_ppMatrix[j][i] = 1;
			}else
				m_ppMatrix[j][i] = 0;
		}
	}
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ���캯��
 *
 * @tparam T
 * @Param dim  ά��
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T>::CMatrix(SIZE dim)
{
	m_nDot	   = 0;
	m_matrixDim = dim;
	m_ppMatrix = NULL;
	Make2DArray(m_ppMatrix,m_matrixDim.cy,m_matrixDim.cx);
	m_bAlloc	= true;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��ֵ
 *
 * @tparam T
 * @Param other
 *
 * @Returns   
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T> & CMatrix<T>::operator=( CMatrix<T> &other )
{
	if ( m_ppMatrix )
	{
		Delete2DArray(m_ppMatrix,m_matrixDim.cx);
		m_bAlloc = false;
		m_matrixDim.cx = 0;
		m_matrixDim.cy = 0;
	}
	m_matrixDim = other.Size();
	Make2DArray(m_ppMatrix,m_matrixDim.cy,m_matrixDim.cx);
	m_bAlloc = true;
	for( int i =0; i< m_matrixDim.cy; i++ )
	{
		for(int j =0; j < m_matrixDim.cx; j++ )
		{
			m_ppMatrix[i][j] = other[i][j];
		}
	}
	return *this;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��������
 *
 * @tparam T
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T>::~CMatrix()
{
	Delete2DArray(m_ppMatrix,m_matrixDim.cy);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ���캯��
 *
 * @tparam T
 * @Param nRowDim   ��ά��
 * @Param nColDim   ��ά��
 */
/* ----------------------------------------------------------------------------*/
template<class T>
CMatrix<T>::CMatrix( int nRowDim,int nColDim )
{
	m_nDot	   = 0;
	m_matrixDim.cx = nColDim;
	m_matrixDim.cy = nRowDim;
	m_ppMatrix = NULL;
	Make2DArray(m_ppMatrix,nRowDim,nColDim);
	m_bAlloc	   = true;
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��������ռ�
 *
 * @tparam T
 * @Param ppMatrix
 * @Param nRow
 * @Param nCol
 */
/* ----------------------------------------------------------------------------*/
template <class T>
void Make2DArray(T** &ppMatrix,int nRow,int nCol)
{
	ppMatrix = new T*[nRow];
	for ( int i=0; i<nRow; i++ )
	{
		ppMatrix[i] = new T[nCol];
	}
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ɾ������
 *
 * @tparam T
 * @Param ppMatrix
 * @Param nCol
 */
/* ----------------------------------------------------------------------------*/
template <class T>
void Delete2DArray( T** &ppMatrix,int nRow)
{
	if ( !ppMatrix )
	{
		return;
	}
	ASSERT(ppMatrix && nRow > 0);
	for ( int i=0; i< nRow; i++ )
	{
		delete[] ppMatrix[i];
	}
	delete[] ppMatrix;
	ppMatrix = NULL;
}
