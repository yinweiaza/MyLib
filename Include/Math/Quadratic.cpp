#include "StdAfx.h"
#include "Quadratic.h"

using namespace cv;

CQuadratic::CQuadratic( const cv::Point2f &p1, const cv::Point2f &p2, const cv::Point2f &p3 )
{
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	float x3 = p3.x;
	float y3 = p3.y;

	m_a	=	y1 / ((x1 - x2) * (x1 - x3)) +
			y2 / ((x2 - x3) * (x2 - x1)) +
			y3 / ((x3 - x2) * (x3 - x1));

	ASSERT(m_a < 0);

	m_b =	(y1 * (-1 * x2 - x3)) / ((x1 - x2) * (x1 - x3)) +
			(y3 * (-1 * x2 - x1)) / ((x3 - x1) * (x3 - x2)) +
			(y2 * (-1 * x3 - x1)) / ((x2 - x1) * (x2 - x3));

	m_c =	(y1 * x2 * x3) / ((x1 - x2) * (x1 - x3)) +
			(y2 * x1 * x3) / ((x2 - x1) * (x2 - x3)) +
			(y3 * x1 * x2) / ((x3 - x1) * (x3 - x2));

}

CQuadratic::~CQuadratic( void )
{

}

cv::Point2f CQuadratic::GetVertex()
{
	float x = -1 * (m_b /(2 * m_a));
	float y = (4 * m_a * m_c - m_b * m_b) / (4 * m_a);
	return cvPoint2D32f(x, y);
}