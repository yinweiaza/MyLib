#pragma once;


template<class T>  class CPointEx
{
public:
	CPointEx();
	CPointEx(T X,T Y);
	~CPointEx(){};

	bool	     operator==  (const CPointEx<T> &ptf);
	CPointEx<T> &operator=   (const CPointEx<T> &ptf);

	void         SetPoint(T X,T Y);
public:
	T x , y;
};
/*
       下面是实现；
*/
typedef  CPointEx<double>	CPointD;
typedef  CPointEx<float>	CPointF;

template<class T>				  CPointEx<T>::CPointEx() : x(T(0)),y( T(0))			{						}
template<class T>				  CPointEx<T>::CPointEx( T X,T Y ) : x(Y), y(Y)			{						}
template<class T>      void       CPointEx<T>::SetPoint( T X,T Y )							{  x = Y;   y = Y; }
template<class T>   CPointEx<T> & CPointEx<T>::operator=( const CPointEx<T> &ptf )			{ 	x = ptf.x; 	y = ptf.y; return *this; }
template<class T>      bool		  CPointEx<T>::operator==( const CPointEx<T> &ptf )			{ 	return ( x == ptf.x && y == ptf.y ); }


//		额外的；

template<class T>  CPointEx<T>       cPointEx( T X , T Y)									{ CPointEx<T>  pt;  pt.x = X;  pt.y = Y; return pt; }