#ifndef arrayConst
#define arrayConst
/*//Copyright Statements:
	This source code file is completed as part of the Open SAL(Open Standardized Algorithm 
Library) by Ming Li during the summer vocation of 2013 at College of Information Science 
and Engineering, Central South University. All rights reserved. No part of these files may
be reproduced or transmitted in any form or by any means without permission from the author.
While, you are permitted to download this Library and its instruction manual freely at 
http://www.oschina.net/code/snippet_1259068_24335# and use it by yourself. 
	If you find some bugs or problems of this Library,you are welcomed to email me(limingcsu
@foxmail.com). If you want to know any details or updates about this Library or would like
to discuss issues about algorithms with enthusiasts,you can apply to join the QQ group(222576709).
*///

/* ��ȫ��ֵ������ģ�壬ʹ�������ռ䣨lmtc��*/
#include <vector>
#include <cstddef>
#include <cstdarg>
#include <iostream>
#include <exception>
#include "myException.h"

namespace lmtc{

	template<typename T>
	class Array{
	public:	//���ýӿ�
		//Ĭ�Ϲ��캯��
		Array():dimNum(0),memory(0){}
		//���ÿɱ�����б�dimNum��ʾά�������治����d��������ʵ�Σ�����Ĳ����Զ�ʡ�ԡ�����������֤������Ŀ��ȷ�������������Ҫ����Ŀ��������п�����ĳ���쳣����ֹ�����ǲ��ܰٷְٱ�֤��
		explicit Array(const size_t dimNum,...);
		//����ά���������Ĺ��캯��
		Array(const size_t d,const std::vector<unsigned int> &dimV);
		Array(const size_t d,const std::vector<int> &dimV);
		Array(const size_t d,const std::vector<unsigned long> &dimV);
		Array(const size_t d,const std::vector<long> &dimV);
		Array(const size_t d,const std::vector<unsigned short> &dimV);
		Array(const size_t d,const std::vector<short> &dimV);
		//���ܵ������Ĺ��캯������ʼ��Ϊ1ά����
		template<typename Iterator>
		Array(Iterator  b, Iterator  e);
		//�����ػ��汾���������ڲ��ÿɱ�����б�Ĺ��캯�� ��ȫƥ��ʱ�����ػ��汾��������ÿɱ�����б�
		template<> Array(int  d, int  b){construct_initialize(d,b);}
		template<> Array(unsigned int  d, unsigned int  b){construct_initialize(d,b);}
		template<> Array(long  d, long  b){construct_initialize(d,b);}
		template<> Array(unsigned long  d, unsigned long  b){construct_initialize(d,b);}
		template<> Array(short  d, short  b){construct_initialize(d,b);}
		template<> Array(unsigned short  d, unsigned short  b){construct_initialize(d,b);}

		//���ú���������ʽ�������飬������dimNum��������ʵ�Σ�����Ĳ����Զ�ʡ�ԡ�����������֤������Ŀ��ȷ�������������Ҫ����Ŀ��������п�����ĳ���쳣����ֹ�����ǲ��ܰٷְٱ�֤��
		T& operator()(...);
		const T& operator()(...)const;
		//�������������ĵ�����ʽ�������飬Ҫ�����ƿɱ������ʽ��
		T& operator()(const std::vector<int> &psVec){return getElement(psVec);}
		T& operator()(const std::vector<unsigned int> &psVec){return getElement(psVec);}
		T& operator()(const std::vector<long> &psVec){return getElement(psVec);}
		T& operator()(const std::vector<unsigned long> &psVec){return getElement(psVec);}
		T& operator()(const std::vector<short> &psVec){return getElement(psVec);}
		T& operator()(const std::vector<unsigned short> &psVec){return getElement(psVec);}
		const T& operator()(const std::vector<int> &psVec)const{return getElement(psVec);}
		const T& operator()(const std::vector<unsigned int> &psVec)const{return getElement(psVec);}
		const T& operator()(const std::vector<long> &psVec)const{return getElement(psVec);}
		const T& operator()(const std::vector<unsigned long> &psVec)const{return getElement(psVec);}
		const T& operator()(const std::vector<short> &psVec)const{return getElement(psVec);}
		const T& operator()(const std::vector<unsigned short> &psVec)const{return getElement(psVec);}
		
		//��ʼ������
		template<typename X>
		void initialize(const X &t);

		//�������
		void clear(){ dimNum=0;memory=0;dimVec.clear();arr.clear();}

		//ת��һά����
		void transformToOneDimension();

		//Ϊ��ʹ�ò�ͬ���͵�����Ԫ��֮��Ҳ�ܻ�����ʣ��ڶ�����ݸ��ƹ��캯���͸�ֵ������ʱ��Ҫ��
		template<typename Y> friend class Array;

		//��ͬ����������ݸ��ƹ��캯��,X!=Tʱ��ʹ�ã���ͬʱʹ�úϳɵĸ��ƹ��캯��
		template<typename X>
		Array(const Array<X> & comArr);

		//��ͬ����������ݸ�ֵ������,X!=Tʱ��ʹ�ã���ͬʱʹ�úϳɵĸ�ֵ������
		template<typename X>
		Array<T> &operator=(const Array<X> & comArr);

		//����������������⴦��
		~Array(){}

	public://��ȡ������Ϣ
		//��ȡά��
		size_t getDimNum()const{ return dimNum;}
		//��ȡĳά����
		size_t getDimLen(const size_t d)const{ if(d>=dimNum) throw arrayOutBound("�쳣�����ܻ�ȡ��������ά����ά����");else return dimVec[d];}
		//��ȡ�洢�ڴ��С
		size_t size()const{ return memory;}
		//��֤�Ƿ�Ϊ��
		bool empty()const{ return memory==0;}

	public://������Ϣ
		typedef size_t size_type;
		typedef typename std::vector<T>::iterator iterator; 
		typedef typename std::vector<T>::const_iterator const_iterator; 
		typedef typename std::vector<T>::reverse_iterator reverse_iterator; 
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator; 
		typedef typename std::vector<T>::difference_type difference_type; 
		typedef typename std::vector<T>::value_type value_type; 
		typedef typename std::vector<T>::reference reference; 
		typedef typename std::vector<T>::const_reference const_reference; 

	public://������
		iterator begin(){ return arr.begin();}
		const_iterator begin()const{ return arr.begin();}
		iterator end(){ return arr.end();}
		const_iterator end()const{ return arr.end();}
		reverse_iterator rbegin(){ return arr.rbegin();}
		const_reverse_iterator rbegin()const{ return arr.rbegin();}
		reverse_iterator rend(){ return arr.rend();}
		const_reverse_iterator rend()const{ return arr.rend();}

	private:
		void construct_initialize(size_t d,size_t b);//�����ʼ��������Array(d,b);
		template<typename D>
		const T &getElement(const std::vector<D> &psVec)const;//����ӳ���ȡԪ������,���ڵ��ò�����
		template<typename D>
		T &getElement(const std::vector<D> &psVec);//����ӳ���ȡԪ������,���ڵ��ò�����
		template<typename D>
		void initArray(const size_t d,const std::vector<D> &dimV);//�����ʼ��������Array( d,const vector<X> dimV);
	
	private://������Ϣ
		size_t dimNum;//ά��
		std::vector<size_t> dimVec;	//ά��������		
		size_t memory;//���ڴ��С
		std::vector<T> arr;	//���ݴ洢����
	};


	//�ɱ�����б�ͨ������ģ�幹�캯������
	template<typename T>
	Array<T>::Array(const size_t d,...):dimNum(d),memory(0){
		memory=1;
		va_list var_arg;
		va_start(var_arg,d);
		for(size_t count=0;count<d;count++){	
			dimVec.push_back(va_arg(var_arg,size_t));
			memory*=dimVec[dimVec.size()-1];
		}
		va_end(var_arg);
		if(0==d)
			memory=0;
		arr=std::vector<T>(memory);
	}
	

	//����ά���������Ĺ��캯�����ڹ��캯���е�����һ���캯���������ʼ������ֻ�Ǵ�����һ���뱾���޹صĶ�����ͨ���������ڿɱ�����б�
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<unsigned int> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<int> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<unsigned long> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<long> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<unsigned short> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	Array<T>::Array(const size_t d,const std::vector<short> &dimV){
		initArray(d,dimV);
	}

	template<typename T>
	template<typename D>
	void Array<T>::initArray(const size_t d,const std::vector<D> &dimV){
		dimNum=d;
		if(d>dimV.size())
			throw arrayAccessException("����ʱ����ά��������ά������С��ָ������ά��");
		memory=1;
		for(size_t count=0;count<dimNum;count++)
		{
			dimVec.push_back(dimV[count]);
			memory*=dimV[count];
		}
		if(0==dimNum)
			memory=0;
		arr=std::vector<T>(memory);
	}
	
	//���ܵ������Ĺ��캯������ʼ��Ϊ1ά����
	template<typename T>
	template<typename Iterator>
	Array<T>::Array(Iterator  b, Iterator  e):dimNum(1),memory(0){
		for(Iterator p=b;p!=e;p++){
			memory++;
			arr.push_back(*p);
		}
		dimVec.push_back(memory);
	}


	template<typename T>
	T& Array<T>::operator()(...){
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		va_list var_arg;
		va_start(var_arg,const_cast<Array<T> *>(this));
		size_t preDimPdc=1;
		size_t storeSite=0;
		for(size_t count=0;count<dimNum;count++){
			size_t len=va_arg(var_arg,size_t);
			if(len>=dimVec[count])
				throw(arrayOutBound());
			storeSite+=len*preDimPdc;
			preDimPdc*=dimVec[count];
		}
		va_end(var_arg);
		return arr[storeSite];
	}
	

	template<typename T>
	const T& Array<T>::operator()(...)const{
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		va_list var_arg;
		va_start(var_arg,const_cast<const Array<T> *>(this));
		size_t preDimPdc=1;
		size_t storeSite=0;
		for(size_t count=0;count<dimNum;count++){
			size_t len=va_arg(var_arg,size_t);
			if(len>=dimVec[count])
				throw(arrayOutBound());
			storeSite+=len*preDimPdc;
			preDimPdc*=dimVec[count];
		}
		va_end(var_arg);
		return arr[storeSite];
	}
	

	template<typename T>
	template<typename D>
	const T &Array<T>::getElement(const std::vector<D> &psVec)const {
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		if(dimNum>psVec.size())
			throw arrayAccessException("������ʴ�����������ά������С������ά��");
		size_t preDimPdc=1;
		size_t storeSite=0;
		for(size_t count=0;count<dimNum;count++){
			size_t len=psVec[count];
			if(len>=dimVec[count])
				throw(arrayOutBound());
			storeSite+=len*preDimPdc;
			preDimPdc*=dimVec[count];
		}
		return arr[storeSite];
	}
	

	template<typename T>
	template<typename D>
	T &Array<T>::getElement(const std::vector<D> &psVec){
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		if(dimNum>psVec.size())
			throw arrayAccessException("������ʴ�����������ά������С������ά��");
		size_t preDimPdc=1;
		size_t storeSite=0;
		for(size_t count=0;count<dimNum;count++){
			size_t len=psVec[count];
			if(len>=dimVec[count])
				throw(arrayOutBound());
			storeSite+=len*preDimPdc;
			preDimPdc*=dimVec[count];
		}
		return arr[storeSite];
	}
	
	template<typename T>
	template<typename X>
	void Array<T>::initialize(const X &t){
		for(size_t i=0;i<memory;i++)
			arr[i]=t;
	}

	//ת��һά����
	template<typename T>
	void Array<T>::transformToOneDimension(){
		dimNum=1;
		dimVec.clear();
		dimVec.push_back(memory);
	}


	//��ͬ����������ݸ��ƹ��캯��
	template<typename T>
	template<typename X>
	Array<T>::Array(const Array<X> & comArr):dimNum(comArr.dimNum),dimVec(comArr.dimVec),memory(comArr.memory){
		arr.assign(comArr.arr.begin(),comArr.arr.end());
	}

	//��ͬ����������ݸ�ֵ������
	template<typename T>
	template<typename X>
	Array<T> & Array<T>::operator=(const Array<X> & comArr){
		dimNum=comArr.dimNum;
		dimVec=comArr.dimVec;
		memory=comArr.memory;
		arr.assign(comArr.arr.begin(),comArr.arr.end());
		return *this;
	}

	template<typename T>
	void Array<T>::construct_initialize(size_t d,size_t b){
			dimNum=1;
			memory=b;
			if(d!=1)
			{
				//std::cout<<"����ʱ��������ά��������1"<<std::endl;
				throw arrayAccessException("����ʱ��������ά��������1");
			}
			dimVec.push_back(b);
			arr=std::vector<T>(memory);	
		}
}


#endif 