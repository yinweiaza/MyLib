#ifndef redBlackTreeConst
#define redBlackTreeConst
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

//ʹ������ָ��ʵ�ְ�ȫ�����ṹ
namespace lmtc{

	template<typename T> class RedBlackTree; //�����ģ������

	template<typename T>  //������ڵ�ģ������
	class RedBlackTreeItem{
	private:
		//���캯����Ϊ˽�У�ֻ����ԪRedBlackTree<T>�ܷ���
		RedBlackTreeItem():size(1),left(NULL),right(NULL),parent(NULL),isBlack(true){}
		RedBlackTreeItem(const T &t):size(1),data(t),left(NULL),right(NULL),parent(NULL),isBlack(true){}
		RedBlackTreeItem(const T &t,SmartPtr<RedBlackTreeItem<T>> *lt, SmartPtr<RedBlackTreeItem<T>> *rt, SmartPtr<RedBlackTreeItem<T>> * pt):data(t),size(1),left(NULL),right(NULL),parent(pt),isBlack(true){if(lt!=NULL) left=*lt;if(rt!=NULL) right=*rt;}	
		RedBlackTreeItem(bool is_black):size(1),left(NULL),right(NULL),parent(NULL),isBlack(is_black){}
		RedBlackTreeItem(const T &t,bool is_black):data(t),size(1),left(NULL),right(NULL),parent(NULL),isBlack(is_black){}
		RedBlackTreeItem(const T &t,SmartPtr<RedBlackTreeItem<T>> *lt, SmartPtr<RedBlackTreeItem<T>> *rt, SmartPtr<RedBlackTreeItem<T>> * pt,bool is_black):data(t),size(1),left(NULL),right(NULL),parent(pt),isBlack(is_black){if(lt!=NULL) left=*lt;if(rt!=NULL) right=*rt;}	
	private:
		//ȷ���Ƿ�������Һ����븸�ڵ㣬�Ƿ��Ǻڽڵ㣬ֻ����ԪRedBlackTree<T>�ܷ���
		bool hasLeft()const{return !left.isEmpty();}
		bool hasRight()const{return !right.isEmpty();}
		bool hasParent()const{return parent!=NULL&&!parent->isEmpty();}
		bool isBlackItem()const{return isBlack;}
	public://������������Ϊpublic����������ָ���е�ɾ����
		~RedBlackTreeItem(){/*std::cout<<"deconstruct"<<data<<std::endl;*/}
	private:
		T data;//�洢����
		unsigned long size;//�Ըýڵ�Ϊ���������Ľڵ���
		SmartPtr<RedBlackTreeItem<T>> left;//��������ָ��		
		SmartPtr<RedBlackTreeItem<T>> right;//�Һ�������ָ��		
		SmartPtr<RedBlackTreeItem<T>> *parent;//���ڵ�����ָ���ָ�룬�˴���������Ƿ�ֹ���ӽڵ��е�����ָ���໥���ôӶ����µ��ڴ�й¶���⣨����ָ�벻���໥���ã����򲻻��Զ��ͷ��ڴ���Դ��
		bool isBlack;  //�ڵ�������
	friend class RedBlackTree<T>; //�Ѵ洢��ͬ�������ݵĶ�������Ϊ��Ԫ
	};

	//�����ģ�塣������չ��̬˳��ͳ�ƹ��ܣ�
	template<typename T>
	class RedBlackTree{
	public:
		typedef SmartPtr<RedBlackTreeItem<T>> ItemType; //�ڵ�����ָ�����ͱ���
	public:
		//�����������캯����Ҫ��T����<�ȽϷ����бȽϡ������ṩ�ȽϺ���ָ�븲��ǰ�ߡ�
		//���캯������ʼ��rootΪ�սڵ㣬less_compare��ʾС�ڱȽϺ���ָ�룬Ϊ����ʹ��Ĭ�ϲ�����'<'.
		RedBlackTree(bool (*less_compare)(const T &t1,const T &t2)=NULL):root(NULL),nullItm(NULL),lessCompare(less_compare){root=nullItm;if(lessCompare==NULL) lessCompare=less_default;}
		//���캯������ʼ��rootΪ�½��ڵ㣬less_compare��ʾС�ڱȽϺ���ָ�룬Ϊ����ʹ��Ĭ�ϲ�����'<'.
		RedBlackTree(const T &e,bool (*less_compare)(const T &t1,const T &t2)=NULL):root(NULL),nullItm(NULL),lessCompare(less_compare){root= ItemType(new RedBlackTreeItem<T>(e,&nullItm,&nullItm,&nullItm));if(lessCompare==NULL) lessCompare=less_default;} 
	
	public:
		//��������������нӿ��е�ItemType &�����������Ǻ�����нڵ�����ã�ֻ��Ҫ��ָ�����нڵ�����ˡ�ItemType.isEmpty()���жϸ���������ֵ�Ƿ�Ϊ��Ч�ڵ㡣
		ItemType insert(const T &t);//���룬�����½ڵ㡣
		ItemType search(const T &t)const;//���ң����ش���ڵ㡣
		ItemType minimum()const;//������С�ڵ㡣
		ItemType maximum()const;//�������ڵ㡣
		ItemType minimumFrom(const ItemType &t)const;//����tΪ���������в�����С�ڵ㡣
		ItemType maximumFrom(const ItemType &t)const;//����tΪ���������в������ڵ㡣
		ItemType successor(const ItemType &t)const;//���Һ�̡�
		ItemType predecessor(const ItemType &t)const;//����ǰ����
		ItemType deleteItem(const ItemType &t);//ɾ���ڵ㣬������֮��

		//˳��ͳ��ѧ��˳���0��ʼ
		ItemType searchItmOfOrder(unsigned long i)const;//���ҵ�i˳��Ľڵ�
		unsigned long getOrderOfItm(const ItemType &t)const;//ȷ���ڵ��˳��

		//�ж��Ƿ�Ϊ����
		bool isEmpty()const{return root.isEmpty();} 
		//��Ϊ����
		void setEmpty(){ root=nullItm;} 
		//��ȡ�ڵ�����Ŀ
		unsigned long size()const{return getItemSize(root);}
		//����Ϊ���ڵ���
		void resetRoot(const T &e){root=ItemType(new RedBlackTreeItem<T>(e,&nullItm,&nullItm,&nullItm));}
		//��ȡ���ڵ�����
		const ItemType &getRoot()const{return root;}
		//��ȡ�ڵ�ֵ
		T getItemValue(const ItemType &itmPtr)const{if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->data;}
		//��ȡ�ڵ��sizeֵ
		unsigned long getItemSize(const ItemType &itmPtr)const{if(itmPtr.isEmpty()) return 0;else return itmPtr->size;}
		

		//��������ǰ�����򡢺���������Ը����ڵ����������f����
		template<typename X>
		void traver_preOrder(void (*f)(X x))const;
		template<typename X>
		void traver_inOrder(void (*f)(X x))const;
		template<typename X>
		void traver_postOrder(void (*f)(X x))const;
		//�Զ�����ĳ�ڵ�Ϊ����������ǰ�����򡢺���������Ը����ڵ����������f����
		template<typename X>
		void traver_preOrder_From(const ItemType &itmPtr,void (*f)(X x))const;
		template<typename X>
		void traver_inOrder_From(const ItemType &itmPtr,void (*f)(X x))const;
		template<typename X>
		void traver_postOrder_From(const ItemType &itmPtr,void (*f)(X x))const;

	public:
		//���Ժ���
		unsigned long asertTree(const ItemType &itmPtr);

	private:
		//���������ɾ���ӿڵĸ�������
		void leftRotate(ItemType &itmPtr);
		void rightRotate(ItemType &itmPtr);
		void insertFixUp(ItemType *z);
		void deleteItemFixUp(ItemType *x,ItemType *p);

	private://���µ�ΪһЩ�������ӿڣ�����Ӧ��������������Ϊ�û�ֱ��ʹ�ÿ����ƻ�������ṹ��
		//��ȡ�ڵ���������
		ItemType &getItemLeft(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->left;}
		//��ȡ�ڵ��Һ�������
		ItemType &getItemRight(ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else return itmPtr->right;}
		//��ȡ�ڵ㸸�ڵ�����
		ItemType &getItemParent(const ItemType &itmPtr)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else{if(itmPtr->parent!=NULL) return *itmPtr->parent; else return nullItm;} }
		//���ýڵ������
		void setItemValue(ItemType &itmPtr,const T &val)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else  itmPtr->data=val;}
		//��������޸����ṹ���ڲ��������β�itmPtr����Ϊ���ڲ�������
		//���ýڵ�����ӣ�ԭ����������ɾ���������µ���������
		ItemType &setItemLeft(ItemType &itmPtr,const T &e)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else  itmPtr->left=ItemType(new RedBlackTreeItem<T>(e,&nullItm,&nullItm,&itmPtr));return itmPtr->left;}
		//����ڵ�����ӣ�ԭ����������Ϊ�����ӵ��������������µ���������
		ItemType &insertItemLeft(ItemType &itmPtr,const T &e)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else{ ItemType itmTemp(new RedBlackTreeItem<T>(e,&itmPtr->left,&nullItm,&itmPtr));if(!itmPtr->left.isEmpty()) itmPtr->left->parent=&itmPtr->left;  itmPtr->left=itmTemp ; } return itmPtr->left; }
		//���ýڵ���Һ��ӣ�ԭ����������ɾ���������µ��Һ�������
		ItemType &setItemRight(ItemType &itmPtr,const T &e)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else  itmPtr->right=ItemType(new RedBlackTreeItem<T>(e,&nullItm,&nullItm,&itmPtr));return itmPtr->right;}
		//����ڵ���Һ��ӣ�ԭ����������Ϊ���Һ��ӵ��������������µ��Һ�������
		ItemType &insertItemRight(ItemType &itmPtr,const T &e)const{ if(itmPtr.isEmpty()) throw EmptySmartPtrAccessException();else{ ItemType itmTemp(new RedBlackTreeItem<T>(e,&nullItm,&itmPtr->right,&itmPtr));if(!itmPtr->right.isEmpty()) itmPtr->right->parent=&itmPtr->right; itmPtr->right=itmTemp;} return itmPtr->right; }
		//ɾ����ĳ�ڵ�Ϊ��������
		void deleteSunTree(ItemType &itmPtr){if(itmPtr.isEmpty()) return; itmPtr=nullItm;}
		//��ȡ�ڵ�������е���ʵ����
		ItemType &getRealReference(ItemType &itmPtr){ if(itmPtr.isEmpty()) return nullItm;if(!itmPtr->hasParent()) return root;ItemType &parent=getItemParent(itmPtr);if(parent->left==itmPtr) return parent->left;else return parent->right;}

	private:
	   ItemType root; //���ڵ�  
       mutable ItemType nullItm;  //�սڵ�
	   bool (*lessCompare)(const T &t1,const T &t2);//С�ڱȽϺ���
	};
	template<typename T>
	void RedBlackTree<T>::leftRotate(ItemType &itm){
		if(itm.isEmpty()||!itm->hasRight())
			return;
		ItemType itmPtrX=itm,itmPtrY=itm->right,&itmPtrP=*(itm->parent);
		itmPtrX->right=itmPtrY->left;
		itmPtrY->left=itmPtrX;
		itmPtrY->parent=itmPtrX->parent;

		//���������ж���ṹ��Ϣά��
		itmPtrY->size=itmPtrX->size;
		itmPtrX->size=getItemSize(itmPtrX->left)+getItemSize(itmPtrX->right)+1;
		//ά������
		
		if(itmPtrX->hasRight()){
			if(itmPtrX->right->hasLeft())
				itmPtrX->right->left->parent=&itmPtrX->right;
			if(itmPtrX->right->hasRight())
				itmPtrX->right->right->parent=&itmPtrX->right;
		}

		if(itmPtrP.isEmpty()){
			root=itmPtrY;
			if(root->hasRight())
				root->right->parent=&root;
			if(root->hasLeft()){
				root->left->parent=&root;
				if(root->left->hasLeft())
					root->left->left->parent=&root->left;
				if(root->left->hasRight())
					root->left->right->parent=&root->left;
			}
		}else{
			if(itmPtrX==itmPtrP->left){
				itmPtrP->left=itmPtrY;
				if(itmPtrY->hasRight())
					itmPtrY->right->parent=&itmPtrP->left;
				if(itmPtrY->hasLeft()){
					itmPtrY->left->parent=&itmPtrP->left;
					if(itmPtrY->left->hasLeft())
						itmPtrY->left->left->parent=&itmPtrP->left->left;
					if(itmPtrY->left->hasRight())
						itmPtrY->left->right->parent=&itmPtrP->left->left;
				}
			}
			else{
				itmPtrP->right=itmPtrY;
				if(itmPtrY->hasRight())
					itmPtrY->right->parent=&itmPtrP->right;
				if(itmPtrY->hasLeft()){
					itmPtrY->left->parent=&itmPtrP->right;
					if(itmPtrY->left->hasLeft())
						itmPtrY->left->left->parent=&itmPtrP->right->left;
					if(itmPtrY->left->hasRight())
						itmPtrY->left->right->parent=&itmPtrP->right->left;
				}
			}
		}
	}
	template<typename T>
	void RedBlackTree<T>::rightRotate(ItemType &itm){
		if(itm.isEmpty()||!itm->hasLeft())
			return;
		ItemType itmPtrX=itm,itmPtrY=itm->left,&itmPtrP=*(itm->parent);
		itmPtrX->left=itmPtrY->right;
		itmPtrY->right=itmPtrX;
		itmPtrY->parent=itmPtrX->parent;

		//���������ж���ṹ��Ϣά��
		itmPtrY->size=itmPtrX->size;
		itmPtrX->size=getItemSize(itmPtrX->left)+getItemSize(itmPtrX->right)+1;
		//ά������

		if(itmPtrX->hasLeft()){
			if(itmPtrX->left->hasLeft())
				itmPtrX->left->left->parent=&itmPtrX->left;
			if(itmPtrX->left->hasRight())
				itmPtrX->left->right->parent=&itmPtrX->left;
		}
		
		if(itmPtrP.isEmpty()){
			root=itmPtrY;
			if(root->hasLeft())
				root->left->parent=&root;
			if(root->hasRight()){
				root->right->parent=&root;
				if(root->right->hasRight())
					root->right->right->parent=&root->right;
				if(root->right->hasLeft())
					root->right->left->parent=&root->right;
			}
		}else if(itmPtrX==itmPtrP->right){
			itmPtrP->right=itmPtrY;
			if(itmPtrY->hasLeft())
				itmPtrY->left->parent=&itmPtrP->right;
			if(itmPtrY->hasRight()){
				itmPtrY->right->parent=&itmPtrP->right;
				if(itmPtrY->right->hasRight())
					itmPtrY->right->right->parent=&itmPtrP->right->right;
				if(itmPtrY->right->hasLeft())
					itmPtrY->right->left->parent=&itmPtrP->right->right;
			}
		}else{
			itmPtrP->left=itmPtrY;
			if(itmPtrY->hasLeft())
				itmPtrY->left->parent=&itmPtrP->left;
			if(itmPtrY->hasRight()){
				itmPtrY->right->parent=&itmPtrP->left;
				if(itmPtrY->right->hasRight())
					itmPtrY->right->right->parent=&itmPtrP->left->right;
				if(itmPtrY->right->hasLeft())
					itmPtrY->right->left->parent=&itmPtrP->left->right;
			}
		}
	}

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_preOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		f(itmPtr->data);
		traver_preOrder_From(itmPtr->left,f);
		traver_preOrder_From(itmPtr->right,f);
	}

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_inOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		traver_inOrder_From(itmPtr->left,f);
		f(itmPtr->data);
		traver_inOrder_From(itmPtr->right,f);
	}

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_postOrder_From(const ItemType &itmPtr,void (*f)(X x))const{
		if(itmPtr.isEmpty())
			return;
		traver_postOrder_From(itmPtr->left,f);
		traver_postOrder_From(itmPtr->right,f);
		f(itmPtr->data);
	}
	

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_preOrder(void (*f)(X x))const{
		traver_preOrder_From(root,f);
	}

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_inOrder(void (*f)(X x))const{
		traver_inOrder_From(root,f);
	}

	template<typename T> template<typename X>
	void RedBlackTree<T>::traver_postOrder(void (*f)(X x))const{
		traver_postOrder_From(root,f);
	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::insert(const T &t){
		ItemType *y=&nullItm;
		ItemType *x=&root;
		while(!x->isEmpty()){
			y=x;
			if(lessCompare(t,(*x)->data))
				x=&(*x)->left;
			else
				x=&(*x)->right;

			//������̵Ķ���ṹ��Ϣά��
			(*y)->size++;
			//ά������
		}
		if(y->isEmpty()){
			resetRoot(t);

			//������̵Ķ���ṹ��Ϣά��
			//ά������

			return root;
		}
		else if(lessCompare(t,(*y)->data)){
			insertItemLeft((*y),t);
			(*y)->left->isBlack=false;
			ItemType son=(*y)->left;
			insertFixUp(&(*y)->left);
			return son;
		}else{
			insertItemRight((*y),t);
			(*y)->right->isBlack=false;
			ItemType son=(*y)->right;
			insertFixUp(&(*y)->right);
			return son;
		}

	}

	template<typename T>
	void RedBlackTree<T>::insertFixUp(ItemType *z){
		while((*z)->hasParent()&&getItemParent(*z)->isBlack==false){//z�ĸ��״��ڣ��Ҹ��ڵ�Ϊ��ɫ
			ItemType &gp=getItemParent(getItemParent(*z));
			ItemType &p=getItemParent(*z);
			if(p==gp->left){//z�ĸ������游������
				ItemType &y=gp->right;
				if(!y.isEmpty()&&y->isBlack==false){
					y->isBlack=true;
					p->isBlack=true;
					gp->isBlack=false;
					z=&gp;
				}else{
					if(*z==p->right)
						leftRotate(p);
					gp->isBlack=false;
					gp->left->isBlack=true;
					rightRotate(gp);
					break;
				}
			}else{//z�ĸ������游���Һ���
				ItemType &y=gp->left;
				if(!y.isEmpty()&&y->isBlack==false){
					y->isBlack=true;
					p->isBlack=true;
					gp->isBlack=false;
					z=&gp;
				}else{
					if(*z==p->left)
						rightRotate(p);
					gp->isBlack=false;
					gp->right->isBlack=true;
					leftRotate(gp);
					break;
				}
			}
		}
		root->isBlack=true;
	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::search(const T &k)const{
		ItemType x=root;
		while(!x.isEmpty()&&(lessCompare(x->data,k)||lessCompare(k,x->data))){
			if(lessCompare(k,x->data))
				x=x->left;
			else
				x=x->right;
		}
		return x;
	}
	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::minimum()const{
		return minimumFrom(root);
	}
	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::maximum()const{
		return maximumFrom(root);
	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::minimumFrom(const ItemType &itmPtr)const{
		if(itmPtr.isEmpty()) 
			return itmPtr;
		ItemType x=itmPtr;
		while(x->hasLeft())
			x=x->left;
		return x;
	}
	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::maximumFrom(const ItemType &itmPtr)const{
		if(itmPtr.isEmpty()) 
			return itmPtr;	
		ItemType x=itmPtr;
		while(x->hasRight())
			x=x->right;
		return x;
	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::successor(const ItemType &itmPtr)const{
		if(itmPtr.isEmpty()) 
			throw EmptySmartPtrAccessException();
		if(itmPtr->hasRight())
			return minimumFrom(itmPtr->right);
		ItemType x=itmPtr;
		ItemType y=getItemParent(itmPtr);
		while(!y.isEmpty()&&x==y->right){
			x=y;
			y=getItemParent(y);
		}
		return y;
	}
	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::predecessor(const ItemType &itmPtr)const{
		if(itmPtr.isEmpty()) 
			throw EmptySmartPtrAccessException();
		if(itmPtr->hasLeft())
			return maximumFrom(itmPtr->left);
		ItemType x=itmPtr;
		ItemType y=getItemParent(itmPtr);
		while(!y.isEmpty()&&x==y->left){
			x=y;
			y=getItemParent(y);
		}
		return y;

	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::deleteItem(const ItemType &itmPtr){
		if(itmPtr.isEmpty()) 
			return itmPtr;
		ItemType z=itmPtr;
		ItemType y(NULL);
		if(!z->hasLeft()||!z->hasRight())
			y=z;
		else
			y=successor(z);
		ItemType x(NULL);
		if(y->hasLeft())
			x=y->left;
		else
			x=y->right;
		if(!x.isEmpty())
			x->parent=y->parent;
		if(!y->hasParent()){
			root=x;
			if(!x.isEmpty()&&x->hasLeft())
				x->left->parent=&root;
			if(!x.isEmpty()&&x->hasRight())
				x->right->parent=&root;
		}
		else {
			ItemType &p=getItemParent(y);
			if(y==p->left){
				p->left=x;
				if(!x.isEmpty()&&x->hasLeft())
					x->left->parent=&p->left;
				if(!x.isEmpty()&&x->hasRight())
					x->right->parent=&p->left;
			}else{
				p->right=x;
				if(!x.isEmpty()&&x->hasLeft())
					x->left->parent=&p->right;
				if(!x.isEmpty()&&x->hasRight())
					x->right->parent=&p->right;
			}	
		}

		if(!(y==z))
			std::swap(z->data,y->data);

		ItemType *xPtr=NULL,*xparentPtr=y->parent;
		if(xparentPtr->isEmpty())
			xPtr=&root;
		else{
			if(x==(*xparentPtr)->left)
				xPtr=&(*xparentPtr)->left;
			else 
				xPtr=&(*xparentPtr)->right;
		}

		//ɾ�����̵Ķ���ṹ��Ϣά�����޸�yԭ����ÿ������
		ItemType *p=xparentPtr;
		while(!p->isEmpty()){
			(*p)->size--;
			p=(*p)->parent;
		}
		//ά������
		
		if(y->isBlack==true)
			deleteItemFixUp(xPtr,xparentPtr);

		return y;
	}


	template<typename T>
	void RedBlackTree<T>::deleteItemFixUp(ItemType *x,ItemType *p){
		while(!((*x)==root)&&(x->isEmpty()||(*x)->isBlack==true)){
			if((*x)==(*p)->left){
				ItemType w=(*p)->right;
				if(w->isBlack==false){
					w->isBlack=true;
					(*p)->isBlack=false;
					leftRotate(*p);
					x=&w->left->left;
					p=&w->left;
					w=w->left->right;
				}
				if((!w->hasLeft()||w->left->isBlack==true)&&(!w->hasRight()||w->right->isBlack==true)){
					w->isBlack=false;
					x=p;
					p=(*p)->parent;
				}else{
					if(!w->hasRight()||w->right->isBlack==true){
						w->left->isBlack=true;
						w->isBlack=false;
						rightRotate((*p)->right);
						w=(*p)->right;
					}
					w->isBlack=(*p)->isBlack;
					(*p)->isBlack=true;
					w->right->isBlack=true;
					leftRotate(*p);
					x=&root;
				}
			}else
			{
				ItemType w=(*p)->left;
				if(w->isBlack==false){
					w->isBlack=true;
					(*p)->isBlack=false;
					rightRotate(*p);
					x=&w->right->right;
					p=&w->right;
					w=w->right->left;
				}
				if((!w->hasRight()||w->right->isBlack==true)&&(!w->hasLeft()||w->left->isBlack==true)){
					w->isBlack=false;
					x=p;
					p=(*p)->parent;
				}else{
					if(!w->hasLeft()||w->left->isBlack==true){
						w->right->isBlack=true;
						w->isBlack=false;
						leftRotate((*p)->left);
						w=(*p)->left;
					}
					w->isBlack=(*p)->isBlack;
					(*p)->isBlack=true;
					w->left->isBlack=true;
					rightRotate(*p);
					x=&root;
				}
			}
		}
		if(!x->isEmpty())
			    (*x)->isBlack=true;
	}

	template<typename T>
	typename RedBlackTree<T>::ItemType RedBlackTree<T>::searchItmOfOrder(unsigned long i)const{//���ҵ�i˳��Ľڵ�
		if(isEmpty())
			return nullItm;
		const ItemType *p=&root;
		while(!p->isEmpty()){
			unsigned long s=getItemSize((*p)->left);
			if(i==s)
				return *p;
			if(i<s)
				p=&(*p)->left;
			else{
				p=&(*p)->right;
				i-=s;
				i--;
			}	
		}
		return nullItm;

	}
	template<typename T>
	unsigned long RedBlackTree<T>::getOrderOfItm(const ItemType &t)const{//ȷ���ڵ��˳��
		if(t.isEmpty())
			throw EmptySmartPtrAccessException();
		unsigned long s=getItemSize(t->left);
		const ItemType *p=&t;
		while(!((*p)==root)){
			if((*p)==getItemParent(*p)->right)
				s=s+getItemSize(getItemParent(*p)->left)+1;
			p=(*p)->parent;
		}
		return s;
	}

	template<typename T>
	unsigned long RedBlackTree<T>::asertTree(const ItemType &itmPtr){
		//��֤�ڵ㲻Ϊ��
		if(itmPtr.isEmpty()){
			//std::cout<<"empty"<<std::endl;
			throw std::exception("empty");
			return 0;
		}

		//��֤�����Ϊ��ɫ
		if(itmPtr==root&&root->isBlack==false)
			throw std::exception("������ڵ�Ϊ��ɫ");//std::cout<<"������ڵ�Ϊ��ɫ"<<std::endl;

		unsigned long L0=0,L1=0,L2=0;
		if(itmPtr->isBlack==true)
			L0=1;

		if(itmPtr->hasLeft())
		{
			//std::cout<<"father is "<<itmPtr->data<<" and left son is "<<itmPtr->left->data<<std::endl;
			//std::cout<<" father is "<<getItemParent(itmPtr->left)->data<<std::endl;

			//��֤���ӹ�ϵ�Ƿ���ȷ
			if(!(getItemParent(itmPtr->left)==itmPtr))
				throw std::exception("father wrong");//std::cout<<"wrong when father is "<<itmPtr->data<<" and left son is "<<itmPtr->left->data<<"  wrong father is "<<getItemParent(itmPtr->left)->data<<std::endl;
			
			//��֤��ڵ㲻���к캢��
			if(itmPtr->isBlack==false&&itmPtr->left->isBlack==false)
				throw std::exception("color wrong");//std::cout<<"color wrong:   red father is "<<itmPtr->data<<" and red left son is "<<itmPtr->left->data<<std::endl;

			L1=asertTree(itmPtr->left);
		}
		if(itmPtr->hasRight())
		{
			//std::cout<<"father is "<<itmPtr->data<<" and right son is "<<itmPtr->right->data<<std::endl;
			//std::cout<<" father is "<<getItemParent(itmPtr->right)->data<<std::endl;
			
			//��֤���ӹ�ϵ�Ƿ���ȷ
			if(!(getItemParent(itmPtr->right)==itmPtr))
				throw std::exception("father wrong2");//std::cout<<"wrong when father is "<<itmPtr->data<<" and right son is "<<itmPtr->right->data<<"  wrong father is "<<getItemParent(itmPtr->right)->data<<std::endl;
			
			//��֤��ڵ㲻���к캢��
			if(itmPtr->isBlack==false&&itmPtr->right->isBlack==false)
				throw std::exception("color wrong2");//std::cout<<"color wrong:   red father is "<<itmPtr->data<<" and red right son is "<<itmPtr->right->data<<std::endl;

			L2=asertTree(itmPtr->right);
		}

		//��֤ÿ����֧�ĺڸ߶���ͬ
		if(L1!=L2)
			throw std::exception("black height wrong");//std::cout<<"black height wrong:   when father is "<<itmPtr->data<<std::endl;
		return L1+L0;
	}
}

#endif