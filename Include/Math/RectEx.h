#pragma once
#ifndef RECTEX_H
#define RECTEX_H
template<class T>
BOOL G_IsRectOverlapBase(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2)
{
	if(x1 < x2)
	{
		if(w1 <= x2 - x1)
			return FALSE;
	}
	else
	{
		if(w2 <= x1 - x2)
			return FALSE;
	}

	if(y1 < y2)
	{
		if(h1 <= y2 - y1)
			return FALSE;
	}
	else
	{
		if(h2 <= y1 - y2)
			return FALSE;
	}

	return TRUE;
};

inline BOOL G_IsRectOverlap(const CRect & rc1, const CRect & rc2)
{
	return G_IsRectOverlapBase<int>(rc1.left, rc1.top, rc1.Width(), rc1.Height(), rc2.left, rc2.top, rc2.Width(), rc2.Height());
};

inline BOOL G_IsRectOverlap(const RECT & rc1, const RECT & rc2)
{
	return G_IsRectOverlapBase<int>(rc1.left, rc1.top, rc1.right - rc1.left, rc1.bottom - rc1.top, rc2.left, rc2.top, rc2.right - rc2.left, rc2.bottom - rc2.top);
};

#endif