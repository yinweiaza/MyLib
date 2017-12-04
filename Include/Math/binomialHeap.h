#ifndef binomialHeapConst
#define binomialHeapConst
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
#include "defaultCompare.h"
#include <iostream>
//�����ģ��
namespace lmtc{

	template<typename T> class BinomialHeap; //�����ģ������

	template<typename T>  //�ѽڵ�ģ������
	class BinomialHeapTreeItem{
	private://���캯����Ϊ˽�У�ֻ����ԪBinomialHeap<T>�ܷ���
		BinomialHeapTreeItem():left(NULL),right(NULL),parent(NULL),degree(0){}
		BinomialHeapTreeItem(const T &t):data(t),left(NULL),right(NULL),parent(NULL),degree(0){}
		BinomialHeapTreeItem(const T &t,SmartPtr<BinomialHeapTreeItem<T>> *lt, SmartPtr<BinomialHeapTreeItem<T>> *rt, SmartPtr<BinomialHeapTreeItem<T>> * pt):data(t),left(NULL),right(NULL),parent(NULL),degree(0){if(lt!=NULL) left=*lt;if(rt!=NULL) right=*rt;if(pt!=NULL) parent=*pt;}	
	private://ȷ���Ƿ�������Һ����븸�ڵ㣬ֻ����ԪBinomialHeap<T>�ܷ���
		bool hasLeft()const{return !left.isEmpty();}
		bool hasRight()const{return !right.isEmpty();}
		bool hasParent()const{return !parent.isEmpty();}
	public://������������Ϊpublic����������ָ���е�ɾ����
		~BinomialHeapTreeItem(){/*std::cout<<"deconstruct"<<data<<std::endl;*/}
	private:
		T data;//�洢����
		unsigned int degree;
		SmartPtr<BinomialHeapTreeItem<T>> left;//��������ָ��		
		SmartPtr<BinomialHeapTreeItem<T>> right;//�Һ�������ָ��		
		SmartPtr<BinomialHeapTreeItem<T>> parent;//���ڵ�����ָ��

	friend class BinomialHeap<T>; //�Ѵ洢��ͬ�������ݵĶ�������Ϊ��Ԫ
	};

	//�����ģ��
	template<typename T>
	class BinomialHeap{
	public:
	    typedef SmartPtr<BinomialHeapTreeItem<T>> ItemType; //�ڵ�����ָ�����ͱ���

	public:
		BinomialHeap():root(NULL),nullItm(NULL),heap_size(0){root=nullItm;pri_compare=greater_default;} //���캯������ʼ��rootΪ�սڵ�
		BinomialHeap(bool isMaxHeap):root(NULL),nullItm(NULL),heap_size(0){root=nullItm;if(isMaxHeap==true) pri_compare=greater_default; else pri_compare=less_default;} //���캯������ʼ��rootΪ�սڵ�
		BinomialHeap(bool (*pri_comp)(const T &a,const T &b)):root(NULL),nullItm(NULL),heap_size(0){root=nullItm;pri_compare=pri_comp;} //���캯������ʼ��rootΪ�սڵ�
	public:
		~BinomialHeap(){erase_parent_from(root);}

	public:
		/*����ѽӿ�*/
		//����в���Ԫ�أ�O(lgn)
		ItemType insert(const T &key);
		//���Ӷ���ԭ�ص����ȼ������key��ԭʼֵ���ȼ����򷵻�false��O(lgn)
		bool increaseKey(ItemType &itmPtr,const T &key);
		//����������ȼ�Ԫ�أ�O(lgn)
		ItemType get_prio()const;
		//��ȡ������ȼ�Ԫ�أ�O(lgn)
		ItemType extract_prio();
		//��hp�ϲ�����������hpΪ�նѣ�O(lgn)
		void heapUnion(BinomialHeap &hp); 
		//���increaseKey��extract_prio����ɾ��Ԫ�أ�O(lgn)

		//�ж��Ƿ�Ϊ�ն����
		bool isEmpty()const{return root.isEmpty();} 
		//��Ϊ�ն����
		void setEmpty(){erase_parent_from(root); root=nullItm;heap_size=0;} 
		//��ȡ�Ѵ�С
		unsigned int size()const{return heap_size;}
		//��ȡ�ڵ�ֵ
		T getItemValue(const ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->data;}	
	
	public:
		//���Ժ���
		void asertBinomialHeap()const;

	public://�Ѷ���ѵ��ɶ���������ǰ�����򡢺���������Ը����ڵ����������f����
		template<typename X>
		void traver_preOrder(void (*f)(X x))const;
		template<typename X>
		void traver_inOrder(void (*f)(X x))const;
		template<typename X>
		void traver_postOrder(void (*f)(X x))const;

	private:
	    mutable ItemType root; //���ڵ�   
		mutable ItemType nullItm;  //�սڵ�
		unsigned int heap_size;    //�Ѵ�С
		bool (*pri_compare)(const T &a,const T &b);	//�����ȼ��ȽϺ�����

	private://�Զ����ĳ�ڵ�Ϊ�����Ӷ���������ǰ�����򡢺���������Ը����ڵ����������f����
		template<typename X>
		void traver_preOrder_From(const ItemType &itmPtr,void (*f)(X x))const;
		template<typename X>
		void traver_inOrder_From(const ItemType &itmPtr,void (*f)(X x))const;
		template<typename X>
		void traver_postOrder_From(const ItemType &itmPtr,void (*f)(X x))const;

		void erase_parent_from(ItemType &itmPtr)const;//ɾ���Ӷ�����������ڵ�ĸ�ָ�룬�Ա��Զ��ͷ���Դ
		ItemType unionHeapList(ItemType &h1,ItemType &h2)const;//����������
		void asertBinomialTree(ItemType &itmPtr)const;//��֤ĳ�Ӷ������Ƿ���϶��������

	private:
		//��ȡ�ڵ���������
		ItemType &getItemLeft(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->left;}
		//��ȡ�ڵ��Һ�������
		ItemType &getItemRight(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->right;}
		//��ȡ�ڵ㸸�ڵ�����
		ItemType &getItemParent(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->parent;}
		
	};

	template<typename T>
	void BinomialHeap<T>::erase_parent_from(ItemType &itmPtr)const{
		if(itmPtr.isEmpty())
			return;
		itmPtr->parent=nullItm;
		erase_parent_from(itmPtr->left);
		erase_parent_from(itmPtr->right);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_preOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		f(itmPtr->data);
		traver_preOrder_From(itmPtr->left,f);
		traver_preOrder_From(itmPtr->right,f);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_inOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		traver_inOrder_From(itmPtr->left,f);
		f(itmPtr->data);
		traver_inOrder_From(itmPtr->right,f);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_postOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		traver_postOrder_From(itmPtr->left,f);
		traver_postOrder_From(itmPtr->right,f);
		f(itmPtr->data);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_preOrder(void (*f)(X x))const{
		traver_preOrder_From(root,f);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_inOrder(void (*f)(X x))const{
		traver_inOrder_From(root,f);
	}

	template<typename T> template<typename X>
	void BinomialHeap<T>::traver_postOrder(void (*f)(X x))const{
		traver_postOrder_From(root,f);
	}

	template<typename T>
	void BinomialHeap<T>::asertBinomialTree(ItemType &itmPtr)const{
		int dgr=itmPtr->degree;
		ItemType px=itmPtr->left;
		while(!px.isEmpty()){
			dgr--;
			if(px->degree!=dgr)
				throw BinomialHeapException("degree error:������������������");//std::cout<<"degree error:������������������"<<std::endl;
			if(pri_compare(px->data,itmPtr->data))
				throw BinomialHeapException("error:�����϶���ѵ�����");//std::cout<<"error:�����϶ѵ�����"<<std::endl;
			asertBinomialTree(px);
			px=px->right;
		}
		if(dgr!=0)
			throw BinomialHeapException("degree error:degree != sunNumber");//std::cout<<"degree error:degree != sunNumber"<<std::endl;
	}

	template<typename T>
	void BinomialHeap<T>::asertBinomialHeap()const{
		ItemType px=root;
		while(!px.isEmpty()){
			if(px->hasRight()&&px->right->degree<=px->degree)
				throw BinomialHeapException("degree error:main chain");//std::cout<<"degree error:main chain"<<std::endl;
			asertBinomialTree(px);
			px=px->right;
		}
	}

	template<typename T>
	typename BinomialHeap<T>::ItemType BinomialHeap<T>::get_prio()const{
		if(isEmpty())
			return nullItm;
		ItemType y=root;
		ItemType x=root->right;
		while(!x.isEmpty()){
			if(pri_compare(x->data,y->data))
				y=x;
			x=x->right;
		}
		return y;
	}
	template<typename T>
	bool BinomialHeap<T>::increaseKey(ItemType &itmPtr,const T &key){
		if(!pri_compare(key,itmPtr->data))
			return false;
		itmPtr->data=key;
		ItemType y=itmPtr;
		ItemType z=getItemParent(y);
		while(!z.isEmpty()&&pri_compare(y->data,z->data)){
			std::swap(y->data,z->data);
			y=z;
			z=getItemParent(y);
		}
		return true;
	}

	template<typename T>
	void BinomialHeap<T>::heapUnion(BinomialHeap &hp){
		root=unionHeapList(root,hp.root);
		heap_size+=hp.heap_size;
		hp.setEmpty();
	}

	template<typename T>
	typename BinomialHeap<T>::ItemType BinomialHeap<T>::unionHeapList(ItemType &h1,ItemType &h2)const{
		if(h1.isEmpty())
			return h2;
		if(h2.isEmpty())
			return h1;

		ItemType px=nullItm;
		ItemType newRoot=nullItm;
		ItemType p1=h1;
		ItemType p2=h2;
		if(p1->degree<p2->degree){
			px=p1;
			newRoot=p1;
			p1=p1->right;
		}
		else{
			px=p2;
			newRoot=p2;
			p2=p2->right;	
		}
		while(!p1.isEmpty()||!p2.isEmpty()){
			if(p1.isEmpty()){
				px->right=p2;
				break;
			}
			if(p2.isEmpty()){
				px->right=p1;
				break;
			}
			if(p1->degree<p2->degree){
				px->right=p1;
				px=p1;
				p1=p1->right;
			}else{
				px->right=p2;
				px=p2;
				p2=p2->right;
			}
		}
		ItemType prev_x=nullItm;
		ItemType x=newRoot;
		ItemType next_x=x->right;
		while(!next_x.isEmpty()){
			if(x->degree!=next_x->degree||(next_x->hasRight()&&next_x->right->degree==x->degree)){
				prev_x=x;
				x=next_x;
				next_x=x->right;
			}else if(pri_compare(x->data,next_x->data)){
				x->right=next_x->right;
				next_x->parent=x;
				next_x->right=x->left;
				x->left=next_x;
				x->degree++;
				next_x=x->right;
			}else{
				if(prev_x.isEmpty())
					newRoot=next_x;
				else
					prev_x->right=next_x;
				x->parent=next_x;
				x->right=next_x->left;
				next_x->left=x;
				next_x->degree++;
				x=next_x;
				next_x=x->right;
			}
		}
		return newRoot;
	}

	template<typename T>
	typename BinomialHeap<T>::ItemType BinomialHeap<T>::insert(const T &key){
		ItemType newItem(new BinomialHeapTreeItem<T>(key,&nullItm,&nullItm,&nullItm));
		root=unionHeapList(root,newItem);
		heap_size++;
		return newItem;
	}

	template<typename T>
	typename BinomialHeap<T>::ItemType BinomialHeap<T>::extract_prio(){
		if(isEmpty())
			return nullItm;
		ItemType y=root;
		ItemType pre_y=nullItm;
		ItemType x=root->right;
		ItemType pre_x=root;
		while(!x.isEmpty()){
			if(pri_compare(x->data,y->data)){
				y=x;
				pre_y=pre_x;
			}
			pre_x=x;
			x=x->right;
		}

		if(pre_y.isEmpty())
			root=y->right;
		else
			pre_y->right=y->right;

		ItemType sunListHead=nullItm;
		x=y->left;
		while(!x.isEmpty()){
			ItemType next_x=x->right;
			x->parent=nullItm;
			x->right=sunListHead;
			sunListHead=x;
			x=next_x;
		}
		root=unionHeapList(root,sunListHead);
		y->left=nullItm;
		y->right=nullItm;
		heap_size--;
		return y;
	}
}

#endif