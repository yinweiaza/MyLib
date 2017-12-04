#ifndef nonIntersectSetConst
#define nonIntersectSetConst
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

#include "smartPtr.h"
#include "myException.h"
#include <iostream>
//ʹ������ָ��ʵ�ְ�ȫ�Ĳ��ཻ����
namespace lmtc{
	template<typename T> class NonIntersectSet;//���ཻ����ģ������

	template<typename T>  //���ཻ���Ͻڵ�ģ������
	class NonIntersectSetItem{
	public:
		void setValue(const T &t){data=t;}
		T getValue()const{return data;}
	private:
		//���캯����Ϊ˽�У�ֻ����ԪNonIntersectSet<T>�ܷ���
		NonIntersectSetItem(const T &t):rank(0),data(t),parent(NULL){}
		NonIntersectSetItem():rank(0),parent(NULL){}
	private:
		//ȷ���Ƿ���ڸ��ڵ�
		bool hasParent()const{return !parent.isEmpty();}

	public://������������Ϊpublic����������ָ���е�ɾ����
		~NonIntersectSetItem(){/*std::cout<<"deconstruct"<<data<<std::endl;*/}

	private:
		T data;//�洢����
		unsigned long rank;//�Ըýڵ�Ϊ���������Ľڵ���
		SmartPtr<NonIntersectSetItem<T>> parent;//���ڵ�����ָ��

	friend class NonIntersectSet<T>;
	};

	template<typename T> //���ཻ����ģ��
	class NonIntersectSet{
	public:
		typedef SmartPtr<NonIntersectSetItem<T>> ItemType;
	public:
		//���캯��������������
		NonIntersectSet(const T &t):setItem(new NonIntersectSetItem<T>(t)){}
		//Ĭ�Ϲ��캯������������δ����
		NonIntersectSet():setItem(new NonIntersectSetItem<T>()){}
	
	public:
		//�ϲ���һ���ϣ��������ͬһ������ֻ��ѹ��·�����������Ӵ���
		void unionSet(NonIntersectSet &st);
		//�ҵ���һ����
		ItemType find(){return setFindFrom(setItem);}

		void setValue(const T &t){setItem->data=t;}
		T getValue()const{return setItem->data;}

	private:
		void link(ItemType &x,ItemType &y)const;
		ItemType setFindFrom(ItemType &st)const;

	private:
		 ItemType setItem;
	};

	template<typename T>
	void NonIntersectSet<T>::link(ItemType &x,ItemType &y)const{
		if(x==y)//����ͬһ���ϵĸ�����������
			return;

		if(x->rank>y->rank)
			y->parent=x;
		else{
			x->parent=y;
			if(x->rank==y->rank)
				y->rank++;
		}
	}

	template<typename T>
	typename NonIntersectSet<T>::ItemType NonIntersectSet<T>::setFindFrom(ItemType &st)const{
		if(st->hasParent()){
			st->parent=setFindFrom(st->parent);
			return st->parent;
		}else
			return st;
	}

	template<typename T>
	void NonIntersectSet<T>::unionSet(NonIntersectSet &st){
		link(find(),st.find());
	}
}

#endif