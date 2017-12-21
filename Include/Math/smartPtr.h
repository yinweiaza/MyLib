#ifndef smartPtrConst
#define smartPtrConst
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

/* ����ָ��ͨ��ģ�� ֧�ָ�ֵ�����ƣ������ã���ͷ�����������ͣ�*/
#include <cstddef>
namespace lmtc{

template<typename T> class SmartPtr;

template<typename T>
class basePtr{
	//��������ָ����Ϊ��Ԫ
	friend class SmartPtr<T> ;
	//��������ָ��
	T* sp;
	//���ü���
	size_t use;
	//˽�й��캯�������ܻ�������ָ�룬���ü�����ʼ��Ϊ1
	basePtr(T *p):sp(p),use(1) {}
	//��������
	~basePtr() { delete sp;}
};

template<typename T>
class SmartPtr{
public:
	//�ṩ������������
	typedef T dataType;
	//Ĭ�Ϲ��캯������ʼ��ΪNULL
	SmartPtr():ptr(new basePtr<T>(NULL)){}
	//���캯�������ܻ�������ָ��
	SmartPtr(T *p):ptr(new basePtr<T>(p)){}
	//���ƹ��캯�����������ü���
	SmartPtr(const SmartPtr &orig):ptr(orig.ptr){ ++ptr->use;}
	//���ظ�ֵ������
	SmartPtr & operator=(const SmartPtr &);
	//��������
	~SmartPtr(){ if(--ptr->use==0) delete ptr;}
	//���ؽ����ò�����
	T &operator*(){return *ptr->sp;}
	const T &operator*()const{ return *ptr->sp;}
	//���ؼ�ͷ��������ֻ�е�TΪ������ʱ���ò������źϷ���
	T *operator->(){return ptr->sp;}
	const T *operator->()const{return ptr->sp;} 
	//�ж�ָ���Ƿ�Ϊ��
	bool isEmpty()const{return ptr->sp==NULL;}
	//�ж���ָ���Ƿ����
	bool operator==(const SmartPtr &stPr)const{return ptr->sp==stPr.ptr->sp;}
	//�ж���ָ���Ƿ����
	bool operator!=(const SmartPtr &stPr)const{return !(ptr->sp==stPr.ptr->sp);}
private:
	basePtr<T> *ptr;
};

//����ָ��ĸ�ֵ����������
template<typename T>
SmartPtr<T> &SmartPtr<T>::operator=(const SmartPtr &spt){
	++spt.ptr->use;
	if(--ptr->use==0)
		delete ptr;
	ptr=spt.ptr;
	return *this;
}

}


#endif