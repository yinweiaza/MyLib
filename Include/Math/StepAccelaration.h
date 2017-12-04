#pragma once
#include<utility>
template<class Tx, class Ty>
class CStepAccelaration
{
public:
	CStepAccelaration(void);
	virtual ~CStepAccelaration(void);

	void	AddNewData(Tx x, Ty y);
	BOOL	GetNextX(Tx& x);							//��Ҫ��д
	virtual BOOL	IsBetter(Ty y) PURE;

protected:
	Tx								m_X;				//��ʼ��
	CMap<Tx,Tx,Ty,Ty>				m_mapData;			//ѵ����
	Tx								m_delta;			//ƫ��
	double							m_K;				//ƫ�Ʊ任ϵ��
	std::pair<Tx,Ty>				m_curBest;			//��ǰ���ŵ�
	//Ty							m_e;				//�ʲ�
};

template<class Tx, class Ty>
CStepAccelaration<Tx, Ty>::CStepAccelaration(void)
{
	
}

template<class Tx, class Ty>
CStepAccelaration<Tx, Ty>::~CStepAccelaration(void)
{
}

template<class Tx, class Ty>
void CStepAccelaration<Tx, Ty>::AddNewData(Tx x, Ty y)
{
	m_mapData.SetAt(x,y);
	if(IsBetter(y))
	{
		m_X					= x;
		m_curBest.first		= x;
		m_curBest.second	= y;
	}
};

class CTestStepAccelaration : public CStepAccelaration<double, double>
{
public:
	CTestStepAccelaration(void);
	~CTestStepAccelaration(void);

	BOOL			GetNextX(double& x);
	virtual BOOL	IsBetter(double y);
};
