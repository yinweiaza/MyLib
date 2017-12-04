#pragma once
#ifndef __LINE_H__
#define __LINE_H__

#include "Point.h"
class CLine
{
public:
	CLine(float a = INVALID_VALUE,float b = INVALID_VALUE,float c = INVALID_VALUE){m_a = a; m_b = b; m_c = c;}
	CLine(CPointF &p1, CPointF &p2);
	~CLine(void);
public:	
	CPointF operator&(const CLine &other);

	void	Relocate(CPointF &p1, CPointF &p2);
	float GetXbyY(float y);
	float GetYbyX(float x);
	float GetSlop();
	float GetSlop(CPointF &p1, CPointF &p2);
	void  CreateLine( float slop, CPointF &ptPoint );
	float Distance2(CPointF &p);
	bool  IsSameSide(CPointF &p1, CPointF &p2);

public:
	static	bool IsCoincident(CPointF &p1, CPointF &p2,
		CPointF &q1, CPointF &q2)
	{return ((p1 == q1 && p2 == q2) || (p1 == q2 && p2 == q1));	};
	static	bool IsCross(CPointF &p1, CPointF &p2,
		CPointF &q1, CPointF &q2)
	{return !IsCoincident(p1, p2, q1, q2) && IsRectCross(p1, p2, q1, q2) && IsLineSegmentCross(p1, p2, q1, q2);};
	static	bool IsRectCross(CPointF &p1, CPointF &p2,
		CPointF &q1, CPointF &q2);
	static	bool IsLineSegmentCross(CPointF &p1, CPointF &p2,
		CPointF &q1, CPointF &q2);
	static	bool GetSegmentCross(CPointF &p1, CPointF &p2,
		CPointF &q1, CPointF &q2,
		float &x, float &y);

	float m_a;
	float m_b;
	float m_c;
};






inline  HRESULT  LinesCrossPoint(CLine cLine1,CLine cLine2,CPointF &CrossPoint)
{
	if ( cLine2.m_a / cLine1.m_a == cLine2.m_b / cLine1.m_b )
	{
		return T9_E_FAIL;
	}
	float a1 = cLine1.m_a;
	float b1 = cLine1.m_b;
	float c1 = cLine1.m_c;
	float a2 = cLine2.m_a;
	float b2 = cLine2.m_b;
	float c2 = cLine2.m_c;
	CrossPoint.x = ( b1 * c2 - b2 * c1 )/( a1 * b2 - a2 * b1 );
	CrossPoint.y = ( a2 * c1 - a1 * c2 )/( a1 * b2 - a2 * b1 );
	return T9_NOERROR;
}


/*
	判断三个点是否近似在一条直线上
	P1: ( X1,Y1);
	P2: ( X2,Y2);
	P3: ( X3,Y3);
	叉积 = (X1-X2)*(Y3-Y2)-(X3-X2)*(Y1-Y2) = 0    这是平行的判定
	但是理论上，灯点不一定完全平行，因而会有小偏差，这里我们采用阈值
*/
#define  POINTS_PARALLEL_THRESHOLD   5.0f
inline bool  IsPointsParallel( CPointF &pt1, CPointF &pt2,CPointF &pt3 )
{

	float crossProduct = ( pt1.x - pt2.x) * ( pt3.y - pt2.y ) - ( pt3.x - pt2.x ) * ( pt1.y - pt2.y );
	
	if ( crossProduct > POINTS_PARALLEL_THRESHOLD || crossProduct < -1 * POINTS_PARALLEL_THRESHOLD )
	{
		return false;
	}
	return TRUE;
}

class CSegment: public CLine
{
public:
	CSegment(CPointF &p1,CPointF &p2):
	CLine(p1, p2)
	{
		m_p1.x = p1.x;
		m_p1.y = p1.y;
		m_p2.x = p2.x;
		m_p2.y = p2.y;
	}
	~CSegment()
	{

	}

	CPointF m_p1, m_p2;
};


#endif