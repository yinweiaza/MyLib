#include "StdAfx.h"
#include "StepAccelaration.h"
using namespace std;

CTestStepAccelaration::CTestStepAccelaration( void )
{
	m_X		= 1.0f;	
	m_delta = 1.0f;
	m_K	    = 0.5f;
}

CTestStepAccelaration::~CTestStepAccelaration( void )
{

}

BOOL CTestStepAccelaration::GetNextX( double& x )
{
	ASSERT(m_mapData.PLookup(m_X));
	if(!m_mapData.PLookup(m_X - m_delta))
		x = m_X - m_delta;
	else if(!m_mapData.PLookup(m_X + m_delta))
		x = m_X + m_delta;
	else
	{
		m_delta *= m_K;
		GetNextX(x);
	}
	return true;
}

BOOL CTestStepAccelaration::IsBetter( double y )
{
	return y > m_curBest.second;
}