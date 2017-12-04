#ifndef hashConst
#define hashConst
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

//��ϣ��
#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
#include "myException.h"
#include "myMath.h"
#include "defaultCompare.h"
#include <cmath>
#include <cstdlib>
namespace lmtc{
	//��ȫɢ��ģ������
	template<typename T> class CompleteHash;

	//ʹ�����ӷ�ʵ�ֹ�ϣ��
	template<typename T>
	class Hash{
	public:
		typedef unsigned long SIZE_T;

	public:
		//����ԱӦ��֤key_to_number��������ֵС��adequate_prime_number����adequate_prime_numberΪ������ slot_number������0,slot_number����adequate_prime_numberʱ���Զ���slotNumber=adequate_prime_number
		Hash(SIZE_T slot_number,SIZE_T adequate_prime_number,SIZE_T (*key_to_number)(const T &t),bool (*equal)(const T &t1,const T &t2)=NULL);
	public:
		T* search(const T &t);//���Ҽ����ɹ����ؼ���ָ�룬ʧ�ܷ���NULL
		bool insert(const T &t);//��������ɹ����뷵��true�����Ѵ����򷵻�false
		bool remove(const T &t);//ɾ�������ɹ�ɾ������true�����������򷵻�false
		void resetSlotNumber(const SIZE_T slotNum);//���ò��������slotNum>adequatePrimeNumber�����Զ���slotNumber=adequatePrimeNumber
		//void print();//��ӡ��ϣ��

	public:
		SIZE_T getSlotNumber()const{return slotNumber;}//��ȡ����	
		SIZE_T getMaxListSizeUpBound()const{return maxListSizeUpBound;}//��ȡ��ϣ���и�������󳤶�����	
		SIZE_T getKeyNumber()const{return keyNumber;}//��ȡ�Ѵ���ļ���
		SIZE_T geta()const{return a;}//��ȡȫ��ɢ�в���a
		SIZE_T getb()const{return b;}//��ȡȫ��ɢ�в���b
		
	private:
		bool verifyPrimeNumber(const SIZE_T &number)const;//�˺���������
		SIZE_T keyToNumber(const T &t)const;//��ȫ�ļ�ֵת����������֤����ֵС��adequatePrimeNumber��
		SIZE_T globleHash(const T &t)const;//ȫ��ɢ�к���
		void completeHashOptimize(const SIZE_T maxListLen,const SIZE_T maxTryTimes);//������ȫɢ���Ż�������CompleteHash<T>ʹ�ã���maxListLen��ʾ����ɢ��������󳤶ȣ�maxTryTimes��ʾ����ɢ��������

	private:
		SIZE_T slotNumber;//��ϣ��Ĳ���
		SIZE_T adequatePrimeNumber;//������������ؼ����޳�ͻӳ�䵽0��adequatePrimeNumber-1�������ʵ��������ã������ʶȵĳ�ͻ��
		std::vector<std::list<T>> hashVecList; //��ϣ��洢��
        SIZE_T (*keyToNumberPointer)(const T &t);//��ֵת������ָ��
		bool (*equal)(const T &t1,const T &t2);//��ȱȽϺ���
		SIZE_T a,b;//ȫ��ɢ�в�����
		SIZE_T keyNumber;//���д��ڵļ���
		SIZE_T maxListSizeUpBound;//��ϣ���и�������󳤶�����
	friend class CompleteHash<T>;
	};


	//completeHash
	template<typename T>
	class CompleteHash{
	public:
		typedef unsigned long SIZE_T;

	public:
		//����ԱӦ��֤key_to_number��������ֵС��adequate_prime_number����adequate_prime_numberΪ������ slot_number������0,slot_number����adequate_prime_numberʱ���Զ���slotNumber=adequate_prime_number
		CompleteHash(SIZE_T slot_number,SIZE_T adequate_prime_number,SIZE_T (*key_to_number)(const T &t),bool (*equal)(const T &t1,const T &t2)=NULL);
	public:
		T* search(const T &t);//���Ҽ����ɹ����ؼ���ָ�룬ʧ�ܷ���NULL
		bool insert(const T &t);//��������ɹ����뷵��true�����Ѵ����򷵻�false
		bool remove(const T &t);//ɾ�������ɹ�ɾ������true�����������򷵻�false
		void resetSlotNumber(const SIZE_T slotNum);//���ò��������slotNum>adequatePrimeNumber�����Զ���slotNumber=adequatePrimeNumber
		//��ȫɢ���Ż���һ��ɢ����ೢ��maxFirstTryTimes����ʹ�ö���ɢ�д洢��Ԫ����ǹؼ��ֵ�maxMemoryPerKeyNumber��������ɢ����ೢ��maxSecondTryTimes����ʹ�ö�����ϣ�����ӳ��Ȳ�����maxSecondHashListLen
		void completeHashOptimize(const SIZE_T maxMemoryPerKeyNumber=2,const SIZE_T maxFirstTryTimes=3,const SIZE_T maxSecondHashListLen=2,const SIZE_T maxSecondTryTimes=3);
		void print();//��ӡ��ϣ��
	public:
		SIZE_T getSlotNumber()const{return slotNumber;}//��ȡ����	
		SIZE_T getslotNumberI2()const{return slotNumberI2;}//��ȡ������ϣ�����ƽ����
		SIZE_T getKeyNumber()const{return keyNumber;}//��ȡ�Ѵ���ļ���
		SIZE_T geta()const{return a;}//��ȡȫ��ɢ�в���a
		SIZE_T getb()const{return b;}//��ȡȫ��ɢ�в���b

		
	private:
		bool verifyPrimeNumber(const SIZE_T &number)const;//�˺���������
		SIZE_T keyToNumber(const T &t)const;//��ȫ�ļ�ֵת����������֤����ֵС��adequatePrimeNumber��
	    SIZE_T globleHash(const T &t)const;//ȫ��ɢ�к���

	private:
		SIZE_T slotNumber;//��ϣ��Ĳ���
		SIZE_T slotNumberI2;//������ϣ�����ƽ����
		SIZE_T adequatePrimeNumber;//������������ؼ����޳�ͻӳ�䵽0��adequatePrimeNumber-1�������ʵ��������ã������ʶȵĳ�ͻ��
		std::vector<Hash<T>> hashVec; //��ϣ��洢��
        SIZE_T (*keyToNumberPointer)(const T &t);//��ֵת������ָ��
		bool (*equal)(const T &t1,const T &t2);//��ȱȽϺ���
		SIZE_T a,b;//ȫ��ɢ�в�����
		SIZE_T keyNumber;//���д��ڵļ���
	};


	template<typename T>
	Hash<T>::Hash(SIZE_T slot_number,SIZE_T adequate_prime_number,SIZE_T (*key_to_number)(const T &t),bool (*equal)(const T &t1,const T &t2)=NULL):
	slotNumber(slot_number),adequatePrimeNumber(adequate_prime_number),keyToNumberPointer(key_to_number),keyNumber(0),maxListSizeUpBound(0){
		if(!slotNumber){
			//std::cout<<"��ϣ�����쳣����������Ϊ0"<<std::endl;
			throw HashException("��ϣ�����쳣����������Ϊ0");
		}
		if(!key_to_number){
			//std::cout<<"��ϣ�����쳣����ֵת������ָ�벻��Ϊ��"<<std::endl;
			throw HashException("��ϣ�����쳣����ֵת������ָ�벻��Ϊ��");
		}
		if(!verifyPrimeNumber(adequate_prime_number)){
			//std::cout<<"��ϣ�����쳣����������������������"<<std::endl;
			throw HashException("��ϣ�����쳣����������������������");
		} 

		if(slotNumber>adequatePrimeNumber)
			slotNumber=adequatePrimeNumber;
		hashVecList.resize(slotNumber);

		if(equal==NULL)
		    this->equal=equal_default;
		else
			this->equal=equal;
		a=(SIZE_T)(1+averageRandom()*(adequate_prime_number-1));
		b=(SIZE_T)(averageRandom()*adequate_prime_number);
	}
	
	template<typename T>//�˺���������
	bool Hash<T>::verifyPrimeNumber(const SIZE_T &number)const{
		return true;
	}
		
	template<typename T>
	typename Hash<T>::SIZE_T Hash<T>::keyToNumber(const T &t)const{
		SIZE_T  number=keyToNumberPointer(t);
		if(number>=adequatePrimeNumber){
			//std::cout<<"��ϣ���쳣����ת����ֵʱ���ܴ��ڻ���ڴ�����"<<std::endl;
			throw HashException("��ϣ���쳣����ת����ֵʱ���ܴ��ڻ���ڴ�����");
		}
		return number;
	}

	template<typename T>
	typename Hash<T>::SIZE_T Hash<T>::globleHash(const T &t)const{
		//һ�ֲ�������ļ���((a*keyToNumber(t)+b)%adequatePrimeNumber)%slotNumber�ķ���
		unsigned long key=keyToNumber(t);
		double a1=a/100000,a2=a%100000,k1=key/100000,k2=key%100000;
		double mol=a1*k1*10000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol*=1000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol*=1000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		
		double mol2=(a1*k2+a2*k1)*1000;
		mol2=mol2-(SIZE_T)(mol2/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol2*=100;
		mol2=mol2-(SIZE_T)(mol2/adequatePrimeNumber)*(double)adequatePrimeNumber;

		mol+=a2*k2+mol2+b;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		/*if((SIZE_T)mol!=((a*keyToNumber(t)+b)%adequatePrimeNumber))
		    std::cout<<(SIZE_T)mol<<" !=  "<<((a*keyToNumber(t)+b)%adequatePrimeNumber)<<std::endl;*/
		return (SIZE_T)mol%slotNumber;
	}

	template<typename T>
	T* Hash<T>::search(const T &t){
		std::list<T> &ls=hashVecList[globleHash(t)];
		for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
			if(equal(t,*iter))
				return &(*iter);
		return NULL;
	}
	template<typename T>
	bool Hash<T>::insert(const T &t){
		if(search(t))
			return false;
		SIZE_T gblhs=globleHash(t);
		hashVecList[gblhs].push_front(t);
		keyNumber++;
		if(hashVecList[gblhs].size()>maxListSizeUpBound)
			maxListSizeUpBound=hashVecList[gblhs].size();
		return true;
	}
	template<typename T>
	bool Hash<T>::remove(const T &t){
		std::list<T> &ls=hashVecList[globleHash(t)];
		for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
			if(equal(t,*iter)){
				ls.erase(iter);
				keyNumber--;
				return true;
			}
		return false;
	}
	/*template<typename T>
	void Hash<T>::print(){
		for(SIZE_T i=0; i<slotNumber;i++){
			std::list<T> &ls=hashVecList[i];
			for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
				std::cout<<*iter<<"  ";
			if(!ls.empty())
				std::cout<<std::endl;
		}
	}*/
	
	template<typename T>
	void Hash<T>::resetSlotNumber(const SIZE_T slotNum){
		if(!slotNum){
			//std::cout<<"��ϣ���쳣��������������Ϊ0"<<std::endl;
			throw HashException("��ϣ���쳣��������������Ϊ0");
		}
		slotNumber=slotNum;
		if(slotNumber>adequatePrimeNumber)
			slotNumber=adequatePrimeNumber;

		a=(SIZE_T)(1+averageRandom()*(adequatePrimeNumber-1));
		b=(SIZE_T)(averageRandom()*adequatePrimeNumber);

		std::vector<std::list<T>> hashVcLs(slotNumber);
		for(SIZE_T i=0; i<hashVecList.size();i++){
			std::list<T> &ls=hashVecList[i];
			for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
				hashVcLs[globleHash(*iter)].push_front(*iter);
		}
		hashVecList.swap(hashVcLs);
		maxListSizeUpBound=0;
		for(SIZE_T i=0;i<hashVecList.size();i++)
			if(hashVecList[i].size()>maxListSizeUpBound)
				maxListSizeUpBound=hashVecList[i].size();
	}

	template<typename T>
	void Hash<T>::completeHashOptimize(const SIZE_T maxListLen,const SIZE_T maxTryTimes){//������ȫɢ���Ż�
		if(keyNumber==0)
			return;
		SIZE_T maxLen=std::max((SIZE_T)1,maxListLen);
		SIZE_T maxTime=std::max((SIZE_T)1,maxTryTimes);
		SIZE_T slotNow=std::max(slotNumber,keyNumber*keyNumber);
		SIZE_T A=a,B=b,S=slotNumber,M=maxListSizeUpBound;
		while(maxTime--)
		{
			if(maxListSizeUpBound<=maxLen)
				return;
			resetSlotNumber(slotNow);
			if(maxListSizeUpBound<M)
			{
				A=a;B=b;S=slotNumber;M=maxListSizeUpBound;
			}
		}
		a=A;b=B;slotNumber=S;maxListSizeUpBound=M;
		std::vector<std::list<T>> hashVcLs(slotNumber);
		for(SIZE_T i=0; i<hashVecList.size();i++){
			std::list<T> &ls=hashVecList[i];
			for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
				hashVcLs[globleHash(*iter)].push_front(*iter);
		}
		hashVecList.swap(hashVcLs);
	}
	


	template<typename T>
	CompleteHash<T>::CompleteHash(SIZE_T slot_number,SIZE_T adequate_prime_number,SIZE_T (*key_to_number)(const T &t),bool (*equal)(const T &t1,const T &t2)=NULL):
	slotNumber(slot_number),adequatePrimeNumber(adequate_prime_number),keyToNumberPointer(key_to_number),keyNumber(0),slotNumberI2(0){
		if(!slotNumber){
			//std::cout<<"��ϣ�����쳣����������Ϊ0"<<std::endl;
			throw HashException("��ϣ�����쳣����������Ϊ0");
		}
		if(!key_to_number){
			//std::cout<<"��ϣ�����쳣����ֵת������ָ�벻��Ϊ��"<<std::endl;
			throw HashException("��ϣ�����쳣����ֵת������ָ�벻��Ϊ��");
		}
		if(!verifyPrimeNumber(adequate_prime_number)){
			//std::cout<<"��ϣ�����쳣����������������������"<<std::endl;
			throw HashException("��ϣ�����쳣����������������������");
		} 

		if(slotNumber>adequatePrimeNumber)
			slotNumber=adequatePrimeNumber;
		for(unsigned long i=0;i<slotNumber;i++)
			hashVec.push_back(Hash<T>(1,adequatePrimeNumber,keyToNumberPointer,equal));

		if(equal==NULL)
		    this->equal=equal_default;
		else
			this->equal=equal;
		a=(SIZE_T)(1+averageRandom()*(adequate_prime_number-1));
		b=(SIZE_T)(averageRandom()*adequate_prime_number);
	}
	
	template<typename T>//�˺���������
	bool CompleteHash<T>::verifyPrimeNumber(const SIZE_T &number)const{
		return true;
	}
		
	template<typename T>
	typename CompleteHash<T>::SIZE_T CompleteHash<T>::keyToNumber(const T &t)const{
		SIZE_T  number=keyToNumberPointer(t);
		if(number>=adequatePrimeNumber){
			//std::cout<<"��ϣ���쳣����ת����ֵʱ���ܴ��ڻ���ڴ�����"<<std::endl;
			throw HashException("��ϣ���쳣����ת����ֵʱ���ܴ��ڻ���ڴ�����");
		}
		return number;
	}

	template<typename T>
	typename CompleteHash<T>::SIZE_T CompleteHash<T>::globleHash(const T &t)const{
		//һ�ֲ�������ļ���((a*keyToNumber(t)+b)%adequatePrimeNumber)%slotNumber�ķ���
		unsigned long key=keyToNumber(t);
		double a1=a/100000,a2=a%100000,k1=key/100000,k2=key%100000;
		double mol=a1*k1*10000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol*=1000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol*=1000;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		
		double mol2=(a1*k2+a2*k1)*1000;
		mol2=mol2-(SIZE_T)(mol2/adequatePrimeNumber)*(double)adequatePrimeNumber;
		mol2*=100;
		mol2=mol2-(SIZE_T)(mol2/adequatePrimeNumber)*(double)adequatePrimeNumber;

		mol+=a2*k2+mol2+b;
		mol=mol-(SIZE_T)(mol/adequatePrimeNumber)*(double)adequatePrimeNumber;
		/*if((SIZE_T)mol!=((a*keyToNumber(t)+b)%adequatePrimeNumber))
		    std::cout<<(SIZE_T)mol<<" !=  "<<((a*keyToNumber(t)+b)%adequatePrimeNumber)<<std::endl;*/
		return (SIZE_T)mol%slotNumber;
	}

	template<typename T>
	T* CompleteHash<T>::search(const T &t){
		return hashVec[globleHash(t)].search(t); 
	}
	template<typename T>
	bool CompleteHash<T>::insert(const T &t){
		SIZE_T gbHs=globleHash(t);
		bool b =hashVec[gbHs].insert(t);
		if(b==true){
			keyNumber++;
			slotNumberI2=slotNumberI2+2*hashVec[gbHs].keyNumber-1;
		}
		return b;
	}
	template<typename T>
	bool CompleteHash<T>::remove(const T &t){
		SIZE_T gbHs=globleHash(t);
		bool b=hashVec[gbHs].remove(t);
		if(b==true){
			keyNumber--;
			slotNumberI2=slotNumberI2-2*hashVec[gbHs].keyNumber-1;
		}
		return b;
	}
	template<typename T>
	void CompleteHash<T>::print(){
		for(SIZE_T i=0; i<slotNumber;i++)
			hashVec[i].print();
	}
	
	template<typename T>
	void CompleteHash<T>::resetSlotNumber(const SIZE_T slotNum){
		if(!slotNum){
			//std::cout<<"��ϣ���쳣��������������Ϊ0"<<std::endl;
			throw HashException("��ϣ���쳣��������������Ϊ0");
		}
		slotNumber=slotNum;
		if(slotNumber>adequatePrimeNumber)
			slotNumber=adequatePrimeNumber;

		a=(SIZE_T)(1+averageRandom()*(adequatePrimeNumber-1));
		b=(SIZE_T)(averageRandom()*adequatePrimeNumber);

		std::vector<Hash<T>> hashVc;
		for(unsigned long i=0;i<slotNumber;i++)
			hashVc.push_back(Hash<T>(1,adequatePrimeNumber,keyToNumberPointer,equal));

		for(SIZE_T i=0; i<hashVec.size();i++){
			Hash<T> &hs=hashVec[i];
			for(SIZE_T j=0;j<hs.hashVecList.size();j++){
				std::list<T> &ls=hs.hashVecList[j];
				if(ls.empty())
					continue;
				for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
					hashVc[globleHash(*iter)].insert(*iter);
			}
		}
		hashVec.swap(hashVc);
		slotNumberI2=0;
		for(SIZE_T i=0; i<hashVec.size();i++)
			slotNumberI2+=hashVec[i].keyNumber*hashVec[i].keyNumber;
	}
	template<typename T>
	void CompleteHash<T>::completeHashOptimize(const SIZE_T maxMemoryPerKeyNumber,const SIZE_T maxFirstTryTimes,const SIZE_T maxSecondHashListLen,const SIZE_T maxSecondTryTimes){
		if(keyNumber==0)
			return;
		SIZE_T maxTime=std::max((SIZE_T)1,maxFirstTryTimes);
		SIZE_T slotNow=std::max(slotNumber,keyNumber);
		SIZE_T A=a,B=b,S=slotNumber,I=slotNumberI2;
		while(maxTime--)
		{
			if(slotNumberI2<=maxMemoryPerKeyNumber*keyNumber)
			{
				for(SIZE_T i=0;i<hashVec.size();i++)
					hashVec[i].completeHashOptimize(maxSecondHashListLen,maxSecondTryTimes);
				return;
			}
			resetSlotNumber(slotNow);
			if(slotNumberI2<I)
			{
				A=a;B=b;S=slotNumber;I=slotNumberI2;
			}
		}
		a=A;b=B;slotNumber=S;slotNumberI2=I;

		std::vector<Hash<T>> hashVc;
		for(unsigned long i=0;i<slotNumber;i++)
			hashVc.push_back(Hash<T>(1,adequatePrimeNumber,keyToNumberPointer,equal));
		for(SIZE_T i=0; i<hashVec.size();i++){
			Hash<T> &hs=hashVec[i];
			for(SIZE_T j=0;j<hs.hashVecList.size();j++){
				std::list<T> &ls=hs.hashVecList[j];
				if(ls.empty())
					continue;
				for(std::list<T>::iterator iter=ls.begin();iter!=ls.end();iter++)
					hashVc[globleHash(*iter)].insert(*iter);
			}
		}
		hashVec.swap(hashVc);
		for(SIZE_T i=0;i<hashVec.size();i++)
			hashVec[i].completeHashOptimize(maxSecondHashListLen,maxSecondTryTimes);
	}

}

#endif