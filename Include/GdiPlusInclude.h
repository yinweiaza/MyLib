#pragma once


#define iterator _iterator

#ifdef _DEBUG

namespace Gdiplus
{
	namespace DllExports
	{
#include <GdiplusMem.h>
	};

#ifndef _GDIPLUSBASE_H
#define _GDIPLUSBASE_H
	class GdiplusBase
	{
	public:
		void (operator delete)(void* in_pVoid)
		{
			DllExports::GdipFree(in_pVoid);
		}

		void* (operator new)(size_t in_size)
		{
			return DllExports::GdipAlloc(in_size);
		}

		void (operator delete[])(void* in_pVoid)
		{
			DllExports::GdipFree(in_pVoid);
		}

		void* (operator new[])(size_t in_size)
		{
			return DllExports::GdipAlloc(in_size);
		}

		void * (operator new)(size_t nSize, LPCSTR lpszFileName, int nLine)
		{
			return DllExports::GdipAlloc(nSize);
		}

		void operator delete(void* p, LPCSTR lpszFileName, int nLine)
		{
			DllExports::GdipFree(p);
		}

	};
#endif //_GDIPLUSBASE_H
}
#endif // _DEBUG

#include <gdiplus.h>
#undef iterator

using namespace Gdiplus;
#pragma comment( lib, "gdiplus.lib" )


class CGdiPlusLoadAndUnload
{
private:
	GdiplusStartupInput m_gdiplusStartupInput; 
	ULONG_PTR			m_pGdiToken;

public:
	CGdiPlusLoadAndUnload()
	{
		m_pGdiToken = 0;
	}

	~CGdiPlusLoadAndUnload()
	{
		UnLoad();
	}

	void Load()
	{
		if(m_pGdiToken == 0)
			GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	}

	void UnLoad()
	{
		//GDI+ Õ∑≈
		if(m_pGdiToken != 0)
		{
			GdiplusShutdown(m_pGdiToken);
			m_pGdiToken = 0;
		}
	}
};
