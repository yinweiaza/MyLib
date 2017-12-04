#include "RectEx.h"
#pragma once
#ifndef RECTEX_SET_H
#define RECTEX_SET_H
template<class TYPE, class ARG_TYPE>
class CRectExSet: public CArray<TYPE, ARG_TYPE&>
{
public:
	INT_PTR Add(ARG_TYPE newElement)
	{
		for(int i = 0; i < GetCount(); i++)
		{
			if(G_IsRectOverlap(m_pData[i], newElement))
				return -1;
		}
		return __super::Add(newElement);
	}
};
#endif

