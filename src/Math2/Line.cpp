#include "StdAfx.h"
#include "Line.h"
#include <float.h>
//#include "Point.h"

using namespace std;


CLine::CLine(CPointF& p1, CPointF &p2)
{
	Relocate(p1, p2);
}

CLine::~CLine( void )
{

}

CPointF CLine::operator&( const CLine &other )
{
	float a1 = m_a;
	float b1 = m_b;
	float c1 = m_c;
	float a2 = other.m_a;
	float b2 = other.m_b;
	float c2 = other.m_c;
	float denominator = a1 * b2 - a2 * b1;
	if(abs(denominator) < FLT_EPSILON)
	{
		return CPointF(INVALID_VALUE, INVALID_VALUE);
	}
	float x = (c2 * b1 - c1 * b2)/denominator;
	float y = (c1 * a2 - c2 * a1)/denominator;
	return CPointF(x, y);
}

float CLine::GetXbyY( float y )
{
	return -1*(m_b * y + m_c)/m_a;
}

float CLine::GetYbyX( float x )
{
	return -1*(m_a * x + m_c)/m_b;
}

bool CLine::IsRectCross( CPointF &p1, CPointF &p2, CPointF &q1, CPointF &q2 )
{
	return	min(p1.x,p2.x) <= max(q1.x,q2.x)
	     && min(q1.x,q2.x) <= max(p1.x,p2.x)
		 && min(p1.y,p2.y) <= max(q1.y,q2.y)
		 && min(q1.y,q2.y) <= max(p1.y,p2.y);
}

bool CLine::IsLineSegmentCross( CPointF &p1, CPointF &p2, CPointF &q1, CPointF &q2 )
{
	double line1,line2;
	line1 = p1.x * (q1.y - p2.y) +
		p2.x * (p1.y - q1.y) +
		q1.x * (p2.y - p1.y);
	line2 = p1.x * (q2.y - p2.y) +
		p2.x * (p1.y - q2.y) + 
		q2.x * (p2.y - p1.y);
	if ((line1 * line2 >= 0) && !(abs(line1) <= FLT_EPSILON && abs(line2) <= FLT_EPSILON))
		return false;

	line1 = q1.x * (p1.y - q2.y) +
		q2.x * (q1.y - p1.y) +
		p1.x * (q2.y - q1.y);
	line2 = q1.x * (p2.y - q2.y) + 
		q2.x * (q1.y - p2.y) +
		p2.x * (q2.y - q1.y);
	if ((line1 * line2 >= 0) && !(abs(line1) <= FLT_EPSILON && abs(line2) <= FLT_EPSILON))
		return false;
	return true;
}

bool CLine::GetSegmentCross( CPointF &p1, CPointF &p2,
							CPointF &q1, CPointF &q2,
							float &x, float &y )
{
	if(IsCross(p1, p2, q1, q2))
	{
		float tmpLeft,tmpRight;
		tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
		tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);

		if(tmpLeft < FLT_EPSILON && tmpLeft > -FLT_EPSILON)
		{
			return false;
		}
		x = tmpRight/tmpLeft;

		tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
		tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x- p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y); 

		if(tmpLeft < FLT_EPSILON && tmpLeft > -FLT_EPSILON)
		{
			return false;
		}
		y = tmpRight/tmpLeft;

		ASSERT(x > 0 && y > 0);
		return true;
	}
	return false;
}

float CLine::GetSlop()
{
	if ( m_b == 0 )
	{
		return FLT_MAX;     //´¹Ö±
	}
	return ( -1.0f * m_a / m_b );
}

float CLine::GetSlop( CPointF &p1, CPointF &p2 )
{
	if ( p1.x == p2.x )
	{
		return FLT_MAX;
	}
	return ( (p2.y - p1.y)/(p2.x - p1.x) );
}

void CLine::Relocate( CPointF &p1, CPointF &p2 )
{
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	m_a = y2 - y1;
	m_b = x1 - x2;
	m_c = x2 * y1 - x1 * y2;
}


void CLine::CreateLine( float slop, CPointF &ptPoint )
{
	if ( slop == FLT_MAX )
	{
		m_a  = 1;
		m_b  = 0;
		m_c  = - ptPoint.x;
	}else{
		m_a = - slop;
		m_b = 1;
		m_c = -( m_a * ptPoint.x + ptPoint.y );
	}
}

float CLine::Distance2( CPointF &p )
{
	return abs(m_a * p.x + m_b * p.y + m_c) / sqrtf(m_a * m_a + m_b * m_b);
}

bool CLine::IsSameSide( CPointF &p1, CPointF &p2 )
{
	return (m_a * p1.x + m_b * p1.y + m_c) * (m_a * p2.x + m_b * p2.y + m_c) > 0;
}