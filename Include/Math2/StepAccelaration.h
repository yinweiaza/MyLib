#pragma once
#include<utility>
template<class Tx, class Ty>
class CStepAccelaration
{
public:
	CStepAccelaration(void);
	virtual ~CStepAccelaration(void);

	void	AddNewData(Tx x, Ty y);
	BOOL	GetNextX(Tx& x);							//需要重写
	virtual BOOL	IsBetter(Ty y) PURE;

protected:
	Tx								m_X;				//初始解
	CMap<Tx,Tx,Ty,Ty>				m_mapData;			//训练集
	Tx								m_delta;			//偏移
	double							m_K;				//偏移变换系数
	std::pair<Tx,Ty>				m_curBest;			//当前最优点
	//Ty							m_e;				//允差
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
