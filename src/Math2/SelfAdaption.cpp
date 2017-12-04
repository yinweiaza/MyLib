#include "StdAfx.h"
#include "SelfAdaption.h"
#include "Line.h"
#include "SplineInterpolation.h"
#include "Quadratic.h"


using namespace std;

extern inline float GetXbyY(float y);
extern inline float GetYbyX(float x);


CExposureQuantitySelfAdaption::CExposureQuantitySelfAdaption( void ) : CSelfAdaption()
{

}

CExposureQuantitySelfAdaption::~CExposureQuantitySelfAdaption( void )
{

}

BOOL CExposureQuantitySelfAdaption::GetNextX(float& x)
{
	if (!__super::GetNextX(x))
	{
		ASSERT(0);
		return FALSE;
	}
	if(m_mapData.GetCount() < 2)
	{
		ASSERT(0);
		return FALSE;
	}

	float key;
	int value;

	pair<float, double> c1, c2;
	c1.second = INVALID_VALUE;
	c2.second = INVALID_VALUE;
	POSITION pos;
	pos = m_mapData.GetStartPosition();
	while(pos)
	{
		m_mapData.GetNextAssoc(pos, key, value);
		float ratio = (value > m_Target)?((0x100 - (float)m_Target)/(0x100 - value)):((float)m_Target / value);
		if(ratio < c1.second || ratio < c1.second)
		{
			if(c1.second > c2.second)
			{
				c1.first	= key;
				c1.second	= ratio;
			}
			else
			{
				c2.first	= key;
				c2.second	= ratio;
			}
		}
	}

	int y;

	m_mapData.Lookup(c1.first, y);
	CPointF p1(c1.first, y);

	m_mapData.Lookup(c2.first, y);
	CPointF p2(c2.first, y);

	CLine line(p1, p2);
	x = line.GetXbyY(m_Target);

	ASSERT(!_isnan(x) && _finite(x));
	return TRUE;
};


COverexposureSelfAdaption::COverexposureSelfAdaption( void )
{

}

COverexposureSelfAdaption::~COverexposureSelfAdaption( void )
{

}

BOOL COverexposureSelfAdaption::GetNextX( float& x )
{
	if (!__super::GetNextX(x))
	{
		ASSERT(0);
		return FALSE;
	}
	if(m_mapData.GetCount() < 2)
	{
		ASSERT(0);
		return FALSE;
	}

	float key;
	float value;

	pair<float, double> c1, c2;
	c1.second = INVALID_VALUE;
	c2.second = INVALID_VALUE;
	POSITION pos;
	pos = m_mapData.GetStartPosition();
	while(pos)
	{
		m_mapData.GetNextAssoc(pos, key, value);
		float ratio = (value > m_Target)?(value / m_Target):(m_Target / value);
		if(ratio < c1.second || ratio < c1.second)
		{
			if(c1.second > c2.second)
			{
				c1.first	= key;
				c1.second	= ratio;
			}
			else
			{
				c2.first	= key;
				c2.second	= ratio;
			}
		}
	}

	float y;

	m_mapData.Lookup(c1.first, y);
	CPointF p1(c1.first, y);

	m_mapData.Lookup(c2.first, y);
	CPointF p2(c2.first, y);

	CLine line(p1, p2);
	x = line.GetXbyY(m_Target);

	ASSERT(!_isnan(x) && _finite(x));
	return TRUE;
}

CAlogorithmTestAdaption::CAlogorithmTestAdaption( void )
{

}

CAlogorithmTestAdaption::~CAlogorithmTestAdaption( void )
{

}

BOOL CAlogorithmTestAdaption::GetNextX( double& x )
{
	if (!__super::GetNextX(x))
	{
		ASSERT(0);
		return FALSE;
	}
	if(m_mapData.GetCount() < 3)
	{
		ASSERT(0);
		return FALSE;
	}

	double key;
	double value;

	pair<double, double> c1, c2, c3;
	c1.second = INVALID_VALUE;
	c2.second = INVALID_VALUE;
	c3.second = INVALID_VALUE;
	POSITION pos;
	pos = m_mapData.GetStartPosition();
	while(pos)
	{
		m_mapData.GetNextAssoc(pos, key, value);
		float ratio = (value > m_Target)?(value / m_Target):(m_Target / value);
		if(ratio < c1.second || ratio < c2.second || ratio < c3.second)
		{
			if(c1.second > c2.second)
			{
				if(c1.second > c3.second)
				{
					c1.first	= key;
					c1.second	= ratio;
				}
				else
				{
					c3.first	= key;
					c3.second	= ratio;
				}
			}
			else
			{
				if(c2.second > c3.second)
				{
					c2.first	= key;
					c2.second	= ratio;
				}
				else
				{
					c3.first	= key;
					c3.second	= ratio;
				}
			}
		}
	}

	double y;

	m_mapData.Lookup(c1.first, y);
	cv::Point2f p1(c1.first, y);

	m_mapData.Lookup(c2.first, y);
	cv::Point2f p2(c2.first, y);

	m_mapData.Lookup(c3.first, y);
	cv::Point2f p3(c3.first, y);

	CQuadratic quadratic(p1, p2, p3);

	cv::Point2f Vertex = quadratic.GetVertex();

	ASSERT(Vertex.x > 0 && Vertex.x < 10
		&& Vertex.y > 0 && Vertex.y < 10);

	x = Vertex.x;

	ASSERT(!_isnan(x) && _finite(x));
	return TRUE;
}