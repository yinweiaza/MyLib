#ifndef symmetryArrayConst
#define symmetryArrayConst
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
#include "coordinateMapping.h"
#include "array.h"
#include <algorithm>

namespace lmtc{

	template<typename T>
	class SymmetryArray{
	public:	//���ýӿ�
		//Ĭ�Ϲ��캯��
		SymmetryArray();
		//���ÿɱ�����б�dimNum��ʾά�������治����d��������ʵ�Σ�����Ĳ����Զ�ʡ�ԡ�����������֤������Ŀ��ȷ�������������Ҫ����Ŀ��������п�����ĳ���쳣����ֹ�����ǲ��ܰٷְٱ�֤��
		explicit SymmetryArray(const size_t dimNum,...);
		//����ά���������Ĺ��캯��
		SymmetryArray(const size_t d,const std::vector<unsigned int> &dimV);
		SymmetryArray(const size_t d,const std::vector<int> &dimV);
		SymmetryArray(const size_t d,const std::vector<unsigned long> &dimV);
		SymmetryArray(const size_t d,const std::vector<long> &dimV);
		SymmetryArray(const size_t d,const std::vector<unsigned short> &dimV);
		SymmetryArray(const size_t d,const std::vector<short> &dimV);
		//���ܵ������Ĺ��캯������ʼ��Ϊ1ά����
		template<typename Iterator>
		SymmetryArray( Iterator b, Iterator e);
		//�����ػ��汾���������ڲ��ÿɱ�����б�Ĺ��캯�� ��ȫƥ��ʱ�����ػ��汾��������ÿɱ�����б�
		template<> SymmetryArray(int  d, int  b){construct_initialize(d,b);}
		template<> SymmetryArray(unsigned int  d, unsigned int  b){construct_initialize(d,b);}
		template<> SymmetryArray(long  d, long  b){construct_initialize(d,b);}
		template<> SymmetryArray(unsigned long  d, unsigned long  b){construct_initialize(d,b);}
		template<> SymmetryArray(short  d, short  b){construct_initialize(d,b);}
		template<> SymmetryArray(unsigned short  d, unsigned short  b){construct_initialize(d,b);}
		
		//���ú���������ʽ�������飬������dimNum��������ʵ�Σ�����Ĳ����Զ�ʡ�ԡ�����������֤������Ŀ��ȷ�������������Ҫ����Ŀ��������п�����ĳ���쳣����ֹ�����ǲ��ܰٷְٱ�֤��
		T& operator()(...);
		const T& operator()(...)const;
		//�������������ĵ�����ʽ�������飬Ҫ�����ƿɱ������ʽ��
		T& operator()(const std::vector<int> &psVec){ return getElement(psVec);}
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

		//�ǹ�������ӳ�䣬������ά���������У�Ȼ����ͬ����ά�����Խ���ֵ�������У�Ȼ��������������ȼ��ĸ�ά�����������С�
		//st��ʾ��ͷ���ά��ȫ������ֵΪdm��symPoint(0--dm-1)��ʾ��iά����ֵ��Ҫ��ǽ������С�
		//�����������
		long forwdMapping(const Array<long> &symPoint,const size_t st)const;
	
		//�ǹ�������ӳ��
		//���ط������ά
		long bkwdMapping(Array<long> &symPoint,const size_t n)const;

		//��ʼ������
		template<typename X>
		void initialize(const X &t);

		//�������
		void clear(){ dimNum=0;memory=0;dimVec.clear();arr.clear();unTriMp=UnTrimMapping();}

		//ת��һά����
		void transformToOneDimension();

		//Ϊ��ʹ�ò�ͬ���͵�����Ԫ��֮��Ҳ�ܻ�����ʣ��ڶ�����ݸ��ƹ��캯���͸�ֵ������ʱ��Ҫ��
		template<typename Y> friend class SymmetryArray;

		//��ͬ����������ݸ��ƹ��캯��,X!=Tʱ��ʹ�ã���ͬʱʹ�úϳɵĸ��ƹ��캯��
		template<typename X>
		SymmetryArray(const SymmetryArray<X> & comArr);

		//��ͬ����������ݸ�ֵ������,X!=Tʱ��ʹ�ã���ͬʱʹ�úϳɵĸ�ֵ������
		template<typename X>
		SymmetryArray<T> &operator=(const SymmetryArray<X> & comArr);

		//����������������⴦��
		~SymmetryArray(){}

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
		void construct_initialize(size_t d,size_t b);//�����ʼ��������SymmetryArray(d,b);
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
		UnTrimMapping unTriMp;//�ǹ���ӳ�����
	};

	//Ĭ�Ϲ��캯��
	template<typename T>
	SymmetryArray<T>::SymmetryArray():dimNum(0),memory(0){
	}

	//�ɱ�����б�ͨ������ģ�幹�캯������
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,...): dimNum(d),memory(0){
		if(d==0)
			return;
		std::vector<size_t> dimV;
		dimV.push_back(d);
		va_list var_arg;
		va_start(var_arg,d);
		for(size_t count=0;count<d;count++){	
			size_t temp=va_arg(var_arg,size_t);
			dimVec.push_back(temp);
			dimV.push_back(temp);
		}
		va_end(var_arg);
		std::sort(dimV.begin()+1,dimV.end());
		if(dimV[1]!=0)
		{
			for(size_t i=1;i<=d;i++)
				dimV[i]--;
			unTriMp=UnTrimMapping(Array<long>(dimV.begin(),dimV.end()));
		    memory=unTriMp.countN();
		    arr=std::vector<T>(memory);
		}
	}
	

	//����ά���������Ĺ��캯�����ڹ��캯���е�����һ���캯���������ʼ������ֻ�Ǵ�����һ���뱾���޹صĶ�����ͨ���������ڿɱ�����б�
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<unsigned int> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<int> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<unsigned long> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<long> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<unsigned short> &dimV){
		initArray(d,dimV);
	}
	template<typename T>
	SymmetryArray<T>::SymmetryArray(const size_t d,const std::vector<short> &dimV){
		initArray(d,dimV);
	}
	
	template<typename T>
	template<typename D>
	void SymmetryArray<T>::initArray(const size_t d,const std::vector<D> &dv){
		dimNum=d;
		memory=0;
		if(d>dv.size())
			throw arrayAccessException("����ʱ����ά��������ά������С��ָ������ά��");
		if(d==0)
			return;
		std::vector<size_t> dimV;
		dimV.push_back(d);
		for(size_t count=0;count<dimNum;count++)
		{
			dimVec.push_back(dv[count]);
			dimV.push_back(dv[count]);
		}
		std::sort(dimV.begin()+1,dimV.end());
		if(dimV[1]!=0)
		{
			for(size_t i=1;i<=d;i++)
				dimV[i]--;
			unTriMp=UnTrimMapping(Array<long>(dimV.begin(),dimV.end()));
		    memory=unTriMp.countN();
		    arr=std::vector<T>(memory);
		}
		
	}


	//���ܵ������Ĺ��캯������ʼ��Ϊ1ά����
	template<typename T>
	template<typename Iterator>
	SymmetryArray<T>::SymmetryArray( Iterator b,  Iterator e):dimNum(1),memory(0){
		for(Iterator p=b;p!=e;p++){
			memory++;
			arr.push_back(*p);
		}
		dimVec.push_back(memory);
		if(memory!=0)
		{
		    Array<long> dimArr(1,2);
		    dimArr(0)=1;
		    dimArr(1)=memory-1;
		    unTriMp=UnTrimMapping(dimArr);	
		}
	}

	template<typename T>
	T& SymmetryArray<T>::operator()(...){
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		va_list var_arg;
		va_start(var_arg,const_cast<SymmetryArray<T> *>(this));
	    Array<long> symPoint(1,dimNum);
		for(size_t count=0;count<dimNum;count++){
			size_t len=va_arg(var_arg,size_t);
			if(len>=dimVec[count])
				throw(arrayOutBound());
			symPoint(count)=len;
		}
		va_end(var_arg);
		bool issorted=true;
		for(size_t i=0;i<dimNum-1;i++)
			if(symPoint(i)>symPoint(i+1))
				issorted=false;
		if(issorted==false)
		    std::sort(symPoint.begin(),symPoint.end());
		size_t st=0;
		for(size_t i=0;i<dimNum;i++)
		{
			if(symPoint(i)==0)
				st=i+1;
			else
				break;
		}
		return arr[forwdMapping(symPoint, st)];
	}
	

	template<typename T>
	const T& SymmetryArray<T>::operator()(...)const{
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		va_list var_arg;
		va_start(var_arg,const_cast<const SymmetryArray<T> *>(this));
	    Array<long> symPoint(1,dimNum);
		for(size_t count=0;count<dimNum;count++){
			size_t len=va_arg(var_arg,size_t);
			if(len>=dimVec[count])
				throw(arrayOutBound());
			symPoint(count)=len;
		}
		va_end(var_arg);
		bool issorted=true;
		for(size_t i=0;i<dimNum-1;i++)
			if(symPoint(i)>symPoint(i+1))
				issorted=false;
		if(issorted==false)
		    std::sort(symPoint.begin(),symPoint.end());
		size_t st=0;
		for(size_t i=0;i<dimNum;i++)
		{
			if(symPoint(i)==0)
				st=i+1;
			else
				break;
		}
		return arr[forwdMapping(symPoint, st)];
	}


	template<typename T>
	template<typename D>
	const T &SymmetryArray<T>::getElement(const std::vector<D> &psVec)const{
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		if(dimNum>psVec.size())
			throw arrayAccessException("������ʴ�����������ά������С������ά��");
	    lmtc::Array<long> symPoint(1,dimNum);
		for(size_t count=0;count<dimNum;count++){
			size_t len=psVec[count];
			if(len>=dimVec[count])
				throw(arrayOutBound());
			symPoint(count)=len;
		}
		bool issorted=true;
		for(size_t i=0;i<dimNum-1;i++)
			if(symPoint(i)>symPoint(i+1))
				issorted=false;
		if(issorted==false)
		    std::sort(symPoint.begin(),symPoint.end());
		size_t st=0;
		for(size_t i=0;i<dimNum;i++)
		{
			if(symPoint(i)==0)
				st=i+1;
			else
				break;
		}
		return arr[forwdMapping(symPoint, st)];
	}

	template<typename T>
	template<typename D>
	T &SymmetryArray<T>::getElement(const std::vector<D> &psVec){
		if(0==memory)
			throw(arrayOutBound("0�������飬�ܾ�����"));
		if(dimNum>psVec.size())
			throw arrayAccessException("������ʴ�����������ά������С������ά��");
	    lmtc::Array<long> symPoint(1,dimNum);
		for(size_t count=0;count<dimNum;count++){
			size_t len=psVec[count];
			if(len>=dimVec[count])
				throw(arrayOutBound());
			symPoint(count)=len;
		}
		bool issorted=true;
		for(size_t i=0;i<dimNum-1;i++)
			if(symPoint(i)>symPoint(i+1))
				issorted=false;
		if(issorted==false)
		    std::sort(symPoint.begin(),symPoint.end());
		size_t st=0;
		for(size_t i=0;i<dimNum;i++)
		{
			if(symPoint(i)==0)
				st=i+1;
			else
				break;
		}
		return arr[forwdMapping(symPoint, st)];
	}
	
	template<typename T>
	long SymmetryArray<T>::forwdMapping(const Array<long> &symPoint,const size_t st)const{
		return unTriMp.forwdMapping(unTriMp.getbm()-1, unTriMp.getdm()-1, const_cast<Array<long> &>(symPoint), st)-1;
	}
	
	template<typename T>
	long SymmetryArray<T>::bkwdMapping(Array<long> &symPoint,const size_t n)const{
		if(n>=memory)
		{
			//std::cout<<"����ӳ����ų�����Χ"<<std::endl;
			throw mappingException("����ӳ����ų�����Χ");
		}
		return unTriMp.bkwdMapping(unTriMp.getbm()-1,unTriMp.getdm()-1,symPoint,n+1);
	}

	template<typename T>
	template<typename X>
	void SymmetryArray<T>::initialize(const X &t){
		for(size_t i=0;i<memory;i++)
			arr[i]=t;
	}

	//ת��һά����
	template<typename T>
	void SymmetryArray<T>::transformToOneDimension(){
		dimNum=1;
		dimVec.clear();
		dimVec.push_back(memory);
		if(memory!=0)
		{
			Array<long> dimArr(1,2);
			dimArr(0)=1;
			dimArr(1)=memory-1;
			unTriMp=UnTrimMapping(dimArr);	
		}
		else
			unTriMp=UnTrimMapping();	
	}


	//��ͬ����������ݸ��ƹ��캯��
	template<typename T>
	template<typename X>
	SymmetryArray<T>::SymmetryArray(const SymmetryArray<X> & comArr):dimNum(comArr.dimNum),dimVec(comArr.dimVec),memory(comArr.memory),unTriMp(comArr.unTriMp){
		arr.assign(comArr.arr.begin(),comArr.arr.end());
	}

	//��ͬ����������ݸ�ֵ������
	template<typename T>
	template<typename X>
	SymmetryArray<T> & SymmetryArray<T>::operator=(const SymmetryArray<X> & comArr){
		dimNum=comArr.dimNum;
		dimVec=comArr.dimVec;
		memory=comArr.memory;
		arr.assign(comArr.arr.begin(),comArr.arr.end());
		unTriMp=comArr.unTriMp;
		return *this;
	}

	template<typename T>
	void SymmetryArray<T>::construct_initialize(size_t d,size_t b){
		dimNum=1;
		memory=b;
		if(d!=1)
		{
			//std::cout<<"����ʱ��������ά��������1"<<std::endl;
			throw arrayAccessException("����ʱ��������ά��������1");
		}
		dimVec.push_back(b);
		arr=std::vector<T>(memory);	
		if(memory!=0)
		{
			Array<long> dimArr(1,2);
			dimArr(0)=1;
			dimArr(1)=memory-1;
			unTriMp=UnTrimMapping(dimArr);	
		}
	}
}


#endif 