#pragma once

template<class Tx, class Ty>
class CSelfAdaption
{
public:
	CSelfAdaption(void);
	virtual ~CSelfAdaption(void);

	void	AddNewData(Tx x, Ty y);
	void	Clear();
	void	SetTarget(Ty y);
	BOOL	GetNextX(Tx& x);				//ÐèÒªÖØÐ´

	CMap<Tx,Tx,Ty,Ty>	m_mapData;
protected:
	Ty					m_Target;
	BOOL				m_bTargetSetted;
};

template<class Tx, class Ty>
CSelfAdaption<Tx, Ty>::CSelfAdaption(void)
{
	m_bTargetSetted = FALSE;
}

template<class Tx, class Ty>
CSelfAdaption<Tx, Ty>::~CSelfAdaption(void)
{
}

template<class Tx, class Ty>
void CSelfAdaption<Tx, Ty>::AddNewData(Tx x, Ty y)
{
	m_mapData.SetAt(x,y);
};

template<class Tx, class Ty>
void CSelfAdaption<Tx, Ty>::Clear()
{
	m_mapData.RemoveAll();
}

template<class Tx, class Ty>
void CSelfAdaption<Tx, Ty>::SetTarget(Ty y)
{
	m_Target		= y;
	m_bTargetSetted = TRUE;
}

template<class Tx, class Ty>
BOOL CSelfAdaption<Tx, Ty>::GetNextX(Tx& x)
{
	ASSERT(m_bTargetSetted);
	return m_bTargetSetted;
}

class CExposureQuantitySelfAdaption : public CSelfAdaption<float, int>
{
public:
	CExposureQuantitySelfAdaption(void);
	~CExposureQuantitySelfAdaption(void);

	BOOL GetNextX(float& x);
};

class COverexposureSelfAdaption : public CSelfAdaption<float, float>
{
public:
	COverexposureSelfAdaption(void);
	~COverexposureSelfAdaption(void);

	BOOL GetNextX(float& x);
};

class CAlogorithmTestAdaption : public CSelfAdaption<double, double>
{
public:
	CAlogorithmTestAdaption(void);
	~CAlogorithmTestAdaption(void);

	BOOL GetNextX(double& x);
};

