/*//Copyright Statements:
	This source code file is completed as part of the Open SAL(Open Standardized Algorithm 
Library) by Ming Li during the National Day vocation of 2013 at College of Information Science 
and Engineering, Central South University. All rights reserved. No part of these files may
be reproduced or transmitted in any form or by any means without permission from the author.
While, you are permitted to download this Library and its instruction manual freely at 
http://www.oschina.net/code/snippet_1259068_24335# and use it by yourself. 
	If you find some bugs or problems of this Library,you are welcomed to email me(limingcsu
@foxmail.com). If you want to know any details or updates about this Library or would like
to discuss issues about algorithms with enthusiasts,you can apply to join the QQ group(222576709).
*///
#include "stdafx.h"
#include "linearProgramming.h"
#include "myException.h"
#include <cmath>
namespace lmtc{
	void LinearProgramming::pivot(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v,const unsigned int l,const unsigned int e){
		//��l����ʽԼ����������e���ǻ����������룬���µ�l����ʽ��
		b[l]/=A(l,e);//A(l,e)���Ǵ�����ġ�
		for(unsigned int j=0;j<A.getDimLen(1);j++){
			if(j==e)
				continue;
			A(l,j)/=A(l,e);
		}
		A(l,e)=1/A(l,e);
		//��������Լ��
		for(unsigned int i=0;i<A.getDimLen(0);i++){
			if(i==l)
				continue;
			b[i]-=A(i,e)*b[l];
			for(unsigned int j=0;j<A.getDimLen(1);j++){
				if(j==e)
					continue;
				A(i,j)-=A(i,e)*A(l,j);
			}
			A(i,e)=-A(i,e)*A(l,e);
		}
		//����Ŀ�꺯��
		v+=c[e]*b[l];
		for(unsigned int j=0;j<A.getDimLen(1);j++){
			if(j==e)
				continue;
			c[j]-=c[e]*A(l,j);
		}
		c[e]=-c[e]*A(l,e);
		//������l�����������͵�e���ǻ�������
		std::swap(B[l],N[e]);
	}
	bool LinearProgramming::simplexFeasibleSlackForm(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v){
		if(A.getDimNum()!=2||A.getDimLen(0)!=b.size()||A.getDimLen(1)!=c.size()||b.size()==0||c.size()==0||N.size()!=c.size()||B.size()!=b.size())
			throw lmtc::LinearProgrammingException("�쳣��initializeSimplex���������������Ҫ��");
		while(true){
			//ѡ��c[e]����0����С�ķǻ����������e���˲��Կɱ���ѭ����
			unsigned int e=0;
			while(e<N.size()&&c[e]<=0.0000000000001)//c[e]<=0 ��ֵ�ȶ��ԣ�����
				e++;
			if(e>=N.size())
				break;
			//ѡ���һ�����Լ��l
			unsigned int l=0;
			bool infinite=true;
			long double increment=0;
			for(unsigned int i=0;i<B.size();i++){
				if(A(i,e)>0.0000000000001){//A(i,e)>0��ֵ�ȶ��ԣ�����
					double temp=b[i]/A(i,e);
					if(infinite){
						l=i;
						infinite=false;
						increment=temp;
					}else if(temp<increment){
						l=i;
						increment=temp;
					}
				}
			}
			if(infinite)//�޽�
				return false;
			else//���������Ԫ����
				pivot(N,B,A,b,c,v,l,e);	
		}
		return true;//�ɹ���ֹ
	}

	int LinearProgramming::simplexStandardForm(const Array<long double> &A0,const std::vector<long double> &b0,const std::vector<long double> &c0,std::vector<long double> &X,long double &V){
		if(A0.getDimNum()!=2||A0.getDimLen(0)!=b0.size()||A0.getDimLen(1)!=c0.size()||b0.size()==0||c0.size()==0)
			throw lmtc::LinearProgrammingException("�쳣��initializeSimplex���������������Ҫ��");
		std::vector<unsigned int> N;
		std::vector<unsigned int> B;
		Array<long double> A;
		std::vector<long double> b;
		std::vector<long double> c;
		long double v;
		if(!initializeSimplex(A0,b0,c0,N,B,A,b,c,v))
			return 0;//������
		if(!simplexFeasibleSlackForm(N,B,A,b,c,v))
			return -1;//�޽�
		V=v;
		X.clear();
		X.resize(N.size());
		for(unsigned int i=0;i<B.size();i++)
			if(B[i]<N.size())
				X[B[i]]=b[i];
		return 1;//�н����
	}

	bool LinearProgramming::initializeSimplex(const Array<long double> &A0,const std::vector<long double> &b0,const std::vector<long double> &c0,std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v){
		if(A0.getDimNum()!=2||A0.getDimLen(0)!=b0.size()||A0.getDimLen(1)!=c0.size()||b0.size()==0||c0.size()==0)
			throw lmtc::LinearProgrammingException("�쳣��initializeSimplex���������������Ҫ��");
		unsigned int k=0;//Ѱ��ʹb[k]��С��k
		for(unsigned int i=1;i<b0.size();i++)
			if(b0[i]<b0[k])
				k=i;
		if(b0[k]>=0){//b0��������0ʱ�����������ɳ��ͣ���Ϊ���еġ�
			N.resize(c0.size());
			for(unsigned int j=0;j<c0.size();j++)
				N[j]=j;
			B.resize(b0.size());
			for(unsigned int i=0;i<b0.size();i++)
				B[i]=i+c0.size();
			A=A0;
			b=b0;
			c=c0;
			v=0;
			return true;
		}
		//���츨���ɳ���
		std::vector<unsigned int> N1(c0.size()+1);
		std::vector<unsigned int> B1(b0.size());
		Array<long double> A1(2,B1.size(),N1.size());
		std::vector<long double> b1=b0;
		std::vector<long double> c1(N1.size());
		c1.back()=-1;//Ŀ�꺯����Ϊ-x0;
		long double v1=0;
		for(unsigned int j=0;j<N1.size();j++)
			N1[j]=j;
		for(unsigned int i=0;i<B1.size();i++)
			B1[i]=N1.size()+i;
		for(unsigned int i=0;i<A1.getDimLen(0);i++){
			A1(i,A1.getDimLen(1)-1)=-1;
			for(unsigned int j=0;j<A0.getDimLen(1);j++)
				A1(i,j)=A0(i,j);
		}
		pivot(N1,B1,A1,b1,c1,v1,k,c1.size()-1);//һ����Ԫ�������ɳ��ͱ�Ϊ���С�
		simplexFeasibleSlackForm(N1,B1,A1,b1,c1,v1);//���ɳ��ͽ��й滮
		if(std::abs(v1)>=0.0000000000001)//v1!=0,��ֵ�ȶ��ԣ������������ɳ��͵�����ֵ��Ϊ0���򲻿��С�
			return false;
		else{//�ɸ����ɳ��ͻ�ȡԭ��������ɳ��͡�
			unsigned int affix=B1.size();
			for(unsigned int i=0;i<B1.size();i++){
				if(B1[i]==(c1.size()-1)){
					affix=i;
					break;
				}
			}
			if(affix<B1.size()){//����������Ϊ��������������ĳ���ɻ���ķǻ���������
				unsigned int uN=N1.size();
				for(unsigned int k=0;k<N1.size();k++){
					if(std::abs(A1(affix,k))>=0.0000000000001){
						uN=k;
						break;
					}
				}
				if(uN<N1.size())//????����������á�
					pivot(N1,B1,A1,b1,c1,v1,affix,uN);
				else
					throw LinearProgrammingException("�쳣��initializeSimplex�У����յĸ����ɳ����е�ĳ��Լ���ұߵ�����ϵ����Ϊ�㣬�Ҹ�Լ�������Ϊ����������");
			}
			N.clear();
			for(unsigned int j=0;j<N1.size();j++)
				if(N1[j]!=c1.size()-1)
					N.push_back(N1[j]);
			B=B1;
			for(unsigned int i=0;i<B.size();i++)//����B
				if(B[i]>c0.size())
					B[i]--;
			for(unsigned int j=0;j<N.size();j++)//����N
				if(N[j]>c0.size())
					N[j]--;
			b=b1;
			c.clear();
			c.resize(c0.size());
			v=0;
			//����ԭԼ������ͨ��ȥ������������
			A=Array<long double>(2,A1.getDimLen(0),A1.getDimLen(1)-1);
			unsigned int j0=N1.size()+1;//Ѱ�Ҹ������������,һ�������ҵ����Ҳ�������ڽ�������ѭ�������쳣��
			for(unsigned int j=0;j<N1.size();j++){
				if(N1[j]==c1.size()-1){
					j0=j;
					break;
				}
			}
			for(unsigned int i=0;i<A1.getDimLen(0);i++){
				for(unsigned int j=0;j<j0;j++)
					A(i,j)=A1(i,j);
				for(unsigned int j=j0+1;j<A1.getDimLen(1);j++)
					A(i,j-1)=A1(i,j);
			}
			//�޸�ԭĿ�꺯����
			for(unsigned int j=0;j<N.size();j++){
				if(N[j]>=c0.size())
					continue;
				c[j]+=c0[N[j]];
			}
			for(unsigned int i=0;i<B.size();i++){
				if(B[i]>=c0.size())
					continue;
				v+=c0[B[i]]*b[i];
				for(unsigned int j=0;j<c.size();j++)
					c[j]-=c0[B[i]]*A(i,j);
			}
			return true;
		}
	}

	int LinearProgramming::simplexGeneralForm(const Array<long double> &A,const std::vector<long double> &b,const std::vector<long double> &c,const bool ismaximize,const std::vector<int> &cmpVec,const std::vector<bool> &nonNegVec, std::vector<long double> &X,long double &V){
		if(A.getDimNum()!=2||A.getDimLen(0)!=b.size()||A.getDimLen(1)!=c.size()||b.size()==0||c.size()==0||cmpVec.size()!=b.size()||nonNegVec.size()!=c.size())
			throw lmtc::LinearProgrammingException("�쳣��initializeSimplex���������������Ҫ��");
		unsigned int negNum=0;//�ɸ�������Ŀ
		std::vector<unsigned int>negOther(c.size());//����nonNegVec[j]=falseʱ����Ӧ����һ��������ΪnegOther[j]������Ϊ0;
		for(unsigned int j=0;j<nonNegVec.size();j++){
			if(!nonNegVec[j]){
				negOther[j]=c.size()+negNum;
				negNum++;
			}
		}
		unsigned int equalConsNum=0;//���Լ������Ŀ
		std::vector<unsigned int>equalConsOther(cmpVec.size());//����cmpVec[i]==0ʱ����Ӧ����һԼ��ΪequalConsOther[i]������Ϊ0
		for(unsigned int i=0;i<cmpVec.size();i++)
			if(cmpVec[i]==0){
				equalConsOther[i]=cmpVec.size()+equalConsNum;
				equalConsNum++;
			}
		Array<long double> A1(2,cmpVec.size()+equalConsNum,c.size()+negNum);
		std::vector<long double> b1(cmpVec.size()+equalConsNum);
		std::vector<long double> c1(c.size()+negNum);
		//����С������ת��Ϊ��������,����Ŀ���пɸ������滻Ϊ˫���������ʽ
		for(unsigned int j=0;j<negOther.size();j++){
			if(ismaximize)
				c1[j]=c[j];
			else
				c1[j]=-c[j];
			if(negOther[j]!=0)
				c1[negOther[j]]=-c1[j];
		}
		//�����Լ���ʹ��ڵ���Լ��ת��ΪС�ڵ���Լ��������˫���������ʽ�滻���пɸ�����
		for(unsigned int i=0;i<cmpVec.size();i++){
			b1[i]=b[i];
			//��˫���������ʽ�滻���пɸ�����
			for(unsigned int j=0;j<negOther.size();j++){
				A1(i,j)=A(i,j);
				if(negOther[j]!=0)
					A1(i,negOther[j])=-A(i,j);
			}
			if(cmpVec[i]>=1){//�����ڵ���Լ��ת��ΪС�ڵ���Լ��
				b1[i]=-b1[i];
				for(unsigned int j=0;j<A1.getDimLen(1);j++)
					A1(i,j)=-A1(i,j);
			}else if(cmpVec[i]==0){//������Լ��ת��Ϊ��Լ������ת��ΪС�ڵ���Լ����
				b1[equalConsOther[i]]=-b1[i];
				for(unsigned int j=0;j<A1.getDimLen(1);j++)
					A1(equalConsOther[i],j)=-A1(i,j);
			}

		}
		std::vector<long double>X1;
		long double V1;
		int sign=simplexStandardForm(A1,b1,c1,X1,V1);
		if(sign==1){//ת��Ϊԭ����Ľ�
			if(ismaximize)
				V=V1;
			else
				V=-V1;
			X.clear();
			X.resize(negOther.size());
			for(unsigned int j=0;j<negOther.size();j++){
				X[j]=X1[j];
				if(negOther[j]!=0)
					X[j]-=X1[negOther[j]];
			}
		}
		return sign;
	}
}