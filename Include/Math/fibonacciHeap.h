#ifndef fibonacciHeapConst
#define fibonacciHeapConst
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
#include <vector>
#include <iostream>
//쳲�������ģ��
namespace lmtc{

	template<typename T> class FibonacciHeap; //쳲�������ģ������

	template<typename T>  //�ѽڵ�ģ������
	class FibonacciHeapTreeItem{
	private://���캯����Ϊ˽�У�ֻ����ԪFibonacciHeap<T>�ܷ���
		FibonacciHeapTreeItem():left(NULL),right(NULL),parent(NULL),child(NULL),mark(false),degree(0){}
		FibonacciHeapTreeItem(const T &t):data(t),left(NULL),right(NULL),parent(NULL),child(NULL),mark(false),degree(0){}
		FibonacciHeapTreeItem(const T &t, SmartPtr<FibonacciHeapTreeItem<T>> * pt, SmartPtr<FibonacciHeapTreeItem<T>> * chd, SmartPtr<FibonacciHeapTreeItem<T>> *lt, SmartPtr<FibonacciHeapTreeItem<T>> *rt):data(t),left(NULL),right(NULL),parent(NULL),child(NULL),mark(false),degree(0){if(lt!=NULL) left=*lt;if(rt!=NULL) right=*rt;if(pt!=NULL) parent=*pt;if(chd!=NULL) child=*chd;}	
	private://ȷ���Ƿ���������ֵ��븸�ӽڵ㣬ֻ����ԪFibonacciHeap<T>�ܷ���
		bool hasParent()const{return !parent.isEmpty();}
		bool hasChild()const{return !child.isEmpty();}
		bool hasLeft()const{return !left.isEmpty();}
		bool hasRight()const{return !right.isEmpty();}
		
	public://������������Ϊpublic����������ָ���е�ɾ����
		~FibonacciHeapTreeItem(){/*std::cout<<"deconstruct"<<data<<std::endl;*/}
	private:
		T data;//�洢����
		unsigned int degree;//ӵ�к�����Ŀ
		bool mark;//����һ�γ�Ϊ��һ���ڵ�ĺ����������Ƿ�ʧȥ�����ӣ�ֻ����ʧȥһ����

		SmartPtr<FibonacciHeapTreeItem<T>> parent;//���ڵ�����ָ���ָ�롣
		SmartPtr<FibonacciHeapTreeItem<T>> child;//��������ָ��
		SmartPtr<FibonacciHeapTreeItem<T>> left;//���ֵ�����ָ��		
		SmartPtr<FibonacciHeapTreeItem<T>> right;//���ֵ�����ָ��		
		
	friend class FibonacciHeap<T>; //�Ѵ洢��ͬ�������ݵĶ�������Ϊ��Ԫ
	};

	//쳲�������ģ��
	template<typename T>
	class FibonacciHeap{
	public:
	    typedef SmartPtr<FibonacciHeapTreeItem<T>> ItemType; //�ڵ�����ָ�����ͱ���

	public:
		FibonacciHeap():prioItm(NULL),nullItm(NULL),heap_size(0){prioItm=nullItm;pri_compare=greater_default;} //���캯������ʼ��prioItmΪ�սڵ�
		FibonacciHeap(bool isMaxHeap):prioItm(NULL),nullItm(NULL),heap_size(0){prioItm=nullItm;if(isMaxHeap==true) pri_compare=greater_default; else pri_compare=less_default;} //���캯������ʼ��prioItmΪ�սڵ�
		FibonacciHeap(bool (*pri_comp)(const T &a,const T &b)):prioItm(NULL),nullItm(NULL),heap_size(0){prioItm=nullItm;pri_compare=pri_comp;} //���캯������ʼ��prioItmΪ�սڵ�
	public:
		~FibonacciHeap(){erase_parentAndLeft_from(prioItm);}

	public:
		/*���ȼ����нӿ�*/
		//����в���Ԫ�أ�ƽ̯����O(1)
		ItemType insert(const T &key);
		//���Ӷ���ԭ�ص����ȼ������key��ԭʼֵ���ȼ����򷵻�false��ƽ̯����O(1)
		bool increaseKey(ItemType &itmPtr,const T &key);
		//����������ȼ�Ԫ�أ�ƽ̯����O(1)
		ItemType get_prio()const;
		//��ȡ������ȼ�Ԫ�أ�ƽ̯����O(lgn)
		ItemType extract_prio();
		//��hp�ϲ�����������hpΪ�նѣ�ƽ̯����O(1)
		void heapUnion(FibonacciHeap &hp); 
		//���increaseKey��extract_prio����ɾ��Ԫ�� ƽ̯����O(lgn)

		//�ж��Ƿ�Ϊ�ն�
		bool isEmpty()const{return prioItm.isEmpty();} 
		//��Ϊ�նѣ����ͷ���Դ
		void setEmpty(){erase_parentAndLeft_from(prioItm); prioItm=nullItm;heap_size=0;} 
		//��ȡ�Ѵ�С
		unsigned int size()const{return heap_size;}
		//��ȡ�ڵ�ֵ
		T getItemValue(const ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->data;}	
	
	public:
		//���Ժ�������֤쳲������ѽṹ�Ƿ���ȷ
		void asertFibonacciHeap()const;

	private:
		mutable ItemType prioItm; //������ȼ��ڵ�
		mutable ItemType nullItm;  //�սڵ�
		unsigned int heap_size;    //�Ѵ�С
		bool (*pri_compare)(const T &a,const T &b);	//�����ȼ��ȽϺ�����

	private:
		void erase_parentAndLeft_from(ItemType &itmPtr)const;//�öѵ�ÿ��Ԫ�صĸ�ָ�����ָ��Ϊ�գ�����ȥ��·���������ܱ�֤�Զ��ͷ���Դ
		void asertFibonacciTree(ItemType &itmPtr)const;//��֤����ĳ���Ƿ����쳲�����������
		void concatenateList(ItemType &ls1,ItemType &ls2)const;//����˫����
		void cut(ItemType &x,ItemType &y)const;//����increaseKey���жϲ���
		void cascading_cut(ItemType &y)const;//����increaseKey�м����ж�

	private:
		//��ȡ�ڵ����ֵ�����
		ItemType &getItemLeft(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->left;}
		//��ȡ�ڵ����ֵ�����
		ItemType &getItemRight(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->right;}
		//��ȡ�ڵ㸸�ڵ�����
		ItemType &getItemParent(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->parent;}
		//��ȡ�ڵ�ĳ�����ӵ�����
		ItemType &getItemChild(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->child;}
		
	};

	template<typename T>
	void FibonacciHeap<T>::erase_parentAndLeft_from(ItemType &itmPtr)const{
		if(itmPtr.isEmpty())
			return;
		ItemType x=itmPtr;
		ItemType left_x=x->left;
		if(x->hasLeft())
			x->left->right=nullItm;
		if(x->hasRight())
			x->right->left=nullItm;
		while(!x.isEmpty())
		{
			left_x=x->left;
			if(x->hasChild())
				erase_parentAndLeft_from(x->child);
			x->parent=nullItm;
			x->left=nullItm;
			x=left_x;
		}
	}


	template<typename T>
	void FibonacciHeap<T>::asertFibonacciTree(ItemType &itmPtr)const{
		if(!(itmPtr->left->right==itmPtr)||!(itmPtr->right->left==itmPtr))
			throw std::exception("error1");//std::cout<<"error1"<<std::endl;
		if(itmPtr->hasChild()&&!(itmPtr->child->parent==itmPtr))
			throw std::exception("error2");//std::cout<<"error2"<<std::endl;
		if(itmPtr->hasParent()&&pri_compare(itmPtr->data,itmPtr->parent->data))
			throw std::exception("error3");//std::cout<<"error3"<<std::endl;

		int dgr=itmPtr->degree;
		if(itmPtr->hasChild()){
			ItemType px=itmPtr->child;
			dgr--;
			asertFibonacciTree(px);
			px=px->right;
			while(!(px==itmPtr->child)){
				dgr--;
				asertFibonacciTree(px);
				px=px->right;
			}
		}
		if(dgr!=0)
			throw std::exception("degree error:degree != sunNumber");//std::cout<<"degree error:degree != sunNumber"<<std::endl;
	}

	template<typename T>
	void FibonacciHeap<T>::asertFibonacciHeap()const{
		if(isEmpty()){
			throw std::exception("asert end: empty tree");//std::cout<<"asert end: empty tree"<<std::endl;
			return;
		}
		ItemType px=prioItm;
		asertFibonacciTree(px);
		px=px->right;
		while(!(px==prioItm)){
			asertFibonacciTree(px);
			px=px->right;
		}
	}

	template<typename T>
	typename FibonacciHeap<T>::ItemType FibonacciHeap<T>::get_prio()const{
		return prioItm;
	}
	template<typename T>
	bool FibonacciHeap<T>::increaseKey(ItemType &itmPtr,const T &key){
		if(itmPtr.isEmpty()) 
			throw EmptySmartPtrAccessException();
		if(!pri_compare(key,itmPtr->data))
			return false;
		itmPtr->data=key;
		ItemType x=itmPtr;
		ItemType y=x->parent;
		if(!y.isEmpty()&&!pri_compare(y->data,x->data)){
			cut(x,y);
			cascading_cut(y);
		}
		if(pri_compare(key,prioItm->data))
			prioItm=x;
		return true;
	}

	template<typename T>
	void FibonacciHeap<T>::cut(ItemType &x,ItemType &y)const{
		if(x->right==x)
			y->child=nullItm;
		else{
			x->left->right=x->right;
			x->right->left=x->left;
			y->child=x->right;
		}
		x->parent=nullItm;
		x->left=x;
		x->right=x;
		x->mark=false;
		concatenateList(prioItm,x);
	}
	template<typename T>
	void FibonacciHeap<T>::cascading_cut(ItemType &y)const{
		ItemType z=y->parent;
		if(!z.isEmpty()){
			if(y->mark==false)
				y->mark=true;
			else{
				cut(y,z);
				cascading_cut(z);
			}
		}
	}

	template<typename T>
	void FibonacciHeap<T>::heapUnion(FibonacciHeap &hp){
		concatenateList(prioItm,hp.prioItm);
		if(!hp.isEmpty()&&(prioItm.isEmpty()||pri_compare(hp.prioItm->data,prioItm->data)))
			prioItm=hp.prioItm;
		heap_size+=hp.heap_size;
		hp.prioItm=hp.nullItm;
		hp.setEmpty();
	}

	template<typename T>
	void FibonacciHeap<T>::concatenateList(ItemType &ls1,ItemType &ls2)const{
		if(ls1.isEmpty()||ls2.isEmpty())
			return;
		ItemType right_ls1=ls1->right;
		ItemType right_ls2=ls2->right;
		ls1->right=right_ls2;
		right_ls2->left=ls1;
		ls2->right=right_ls1;
		right_ls1->left=ls2;
	}

	template<typename T>
	typename FibonacciHeap<T>::ItemType FibonacciHeap<T>::insert(const T &key){
		ItemType newItem(new FibonacciHeapTreeItem<T>(key,&nullItm,&nullItm,NULL,NULL));
		newItem->left=newItem;
		newItem->right=newItem;
		concatenateList(prioItm,newItem);
		if(prioItm.isEmpty()||pri_compare(key,prioItm->data))
			prioItm=newItem;
		heap_size++;
		return newItem;
	}

	template<typename T>
	typename FibonacciHeap<T>::ItemType FibonacciHeap<T>::extract_prio(){
		//�նѷ��ؿսڵ�
		if(isEmpty())
			return nullItm;
		//�Ӹ�������ɾ��prioItm�ݴ���prioItmTemp��prioItmָ�����������һ�ڵ㡣
		ItemType prioItmTemp=prioItm;
		if(prioItm->left==prioItm)
			prioItm=nullItm;
		else{
			prioItm->left->right=prioItm->right;
			prioItm->right->left=prioItm->left;
			prioItm=prioItm->left;
		}
		//�ÿ�prioItmTemp�����Һ���ָ��
		prioItmTemp->left=nullItm;
		prioItmTemp->right=nullItm;

		if(prioItmTemp->hasChild())
		{
			ItemType y=prioItmTemp->child;
			//�ÿ�prioItmTemp���ӵĸ�ָ��
			do{
				y->parent=nullItm;
				y=y->right;
			}while(!(y==prioItmTemp->child));
			//��prioItmTemp�ĺ�������������������
			concatenateList(prioItm,prioItmTemp->child);
			prioItm=prioItmTemp->child;
		}

		//���ɾ����ǿգ���ѹ��������
		if(!prioItm.isEmpty()){
			//���������������ϲ�����������vec
			unsigned int Dn=(int)(log(heap_size*1.0)/log(1.618));
			std::vector<ItemType> vec(Dn+1,nullItm);
			ItemType x=nullItm,y=nullItm;
			//���������������е��������ϲ�������vec
			while(!prioItm.isEmpty()){
				//���������е�ǰ�ڵ�ɾ�������ݴ���x
				x=prioItm;
				if(x->right==x)
					prioItm=nullItm;
				else{
					prioItm->left->right=prioItm->right;
					prioItm->right->left=prioItm->left;
					prioItm=prioItm->right;
				}
				//��xΪ������
				x->right=x;
				x->left=x;
				unsigned int d=x->degree;
				//��x��vec�е�������������˳����кϲ�
				while(!vec[d].isEmpty()){
					y=vec[d];
					//���x���ȼ���С���򽻻�x��yָ��
					if(!pri_compare(x->data,y->data))
						std::swap(x,y);
					//��y��x�ĺ�������ϲ�
					if(x->hasChild())
						concatenateList(x->child,y);
					else
						x->child=y;
					//����y�ĸ�ָ�룬mark�򣬼�x��degree��
					y->parent=x;
					y->mark=false;
					x->degree++;
					vec[d]=nullItm;
					d++;
				}
				//���ϲ��õ���������vec
				vec[d]=x;
			}
			//��vec�еĸ������������������prioItm
			for(unsigned int i=0;i<=Dn;i++){
				if(!vec[i].isEmpty()){
					concatenateList(prioItm,vec[i]);
					if(prioItm.isEmpty()||pri_compare(vec[i]->data,prioItm->data))
						prioItm=vec[i];
				}
			}
		}
		heap_size--;
		return prioItmTemp;
	}
}

#endif