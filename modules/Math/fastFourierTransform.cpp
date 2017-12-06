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

#include "stdafx.h"
#include "fastFourierTransform.h"
#include "myException.h"
#include "numberTheory.h"
#include <cmath>
namespace lmtc{
	std::vector<ComplexNumber> FastFourierTransform::fft(const std::vector<ComplexNumber> &a){
		if(a.empty())
			return std::vector<ComplexNumber>();
		unsigned int N=1;//����������
		while(N<a.size())
			N*=2;
		std::vector<ComplexNumber> a1(N);
		for(unsigned int i=0;i<a.size();i++)
			a1[i]=a[i];
		return fft_visit(a1);
	}
	std::vector<ComplexNumber> FastFourierTransform::_fft(const std::vector<ComplexNumber> &y){
		if(y.empty())
			return std::vector<ComplexNumber>();
		unsigned int N=1;//����������
		while(N<y.size())
			N*=2;
		if(N!=y.size())
			throw FastFourierTransformException("����Ҷ��任_fft�Ĳ���y�Ĺ�ģ��Ϊ������");
		std::vector<ComplexNumber> a = _fft_visit(y);
		for(unsigned int i=0;i<N;i++)
			a[i]=a[i]/N;
		return a;
	}

	std::vector<ComplexNumber> FastFourierTransform::fft_visit(const std::vector<ComplexNumber> &a){
		unsigned int n=a.size();
		if(n==1)
			return a;
		ComplexNumber wn(std::cos(2*PI/n),std::sin(2*PI/n));
		ComplexNumber w(1,0);
		std::vector<ComplexNumber> a0(n/2);
		std::vector<ComplexNumber> a1(n/2);
		for(unsigned int i=0;i<n/2;i++){
			a0[i]=a[i*2];
			a1[i]=a[i*2+1];
		}
		std::vector<ComplexNumber> y0=fft_visit(a0);
		std::vector<ComplexNumber> y1=fft_visit(a1);
		std::vector<ComplexNumber> y(n);
		for(unsigned int k=0;k<n/2;k++){
			ComplexNumber temp=w*y1[k];
			y[k]=y0[k]+temp;
			y[k+n/2]=y0[k]-temp;
			w=w*wn;
		}
		return y;
	}
	std::vector<ComplexNumber> FastFourierTransform::_fft_visit(const std::vector<ComplexNumber> &y){
		unsigned int n=y.size();
		if(n==1)
			return y;
		ComplexNumber wn(std::cos(2*(n-1)*PI/n),std::sin(2*(n-1)*PI/n));
		ComplexNumber w(1,0);
		std::vector<ComplexNumber> y0(n/2);
		std::vector<ComplexNumber> y1(n/2);
		for(unsigned int i=0;i<n/2;i++){
			y0[i]=y[i*2];
			y1[i]=y[i*2+1];
		}
		std::vector<ComplexNumber> a0=_fft_visit(y0);
		std::vector<ComplexNumber> a1=_fft_visit(y1);
		std::vector<ComplexNumber> a(n);
		for(unsigned int k=0;k<n/2;k++){
			ComplexNumber temp=w*a1[k];
			a[k]=a0[k]+temp;
			a[k+n/2]=a0[k]-temp;
			w=w*wn;
		}
		return a;
	}

	//��ά���ٸ���Ҷ�任
	lmtc::Array<ComplexNumber> FastFourierTransform::fft(const lmtc::Array<ComplexNumber> &a){
		if(a.empty())//�ձ任
			return lmtc::Array<ComplexNumber>();
		std::vector<unsigned int> dim(a.getDimNum(),1);//�����ά��������
		for(unsigned int i=0;i<dim.size();i++)
			while(dim[i]<a.getDimLen(i))
				dim[i]*=2;
		lmtc::Array<ComplexNumber> a1(a.getDimNum(),dim);
		std::vector<unsigned int> dimC(a.getDimNum(),0);
		while(dimC[a.getDimNum()-1]<a.getDimLen(a.getDimNum()-1)){//��a��Ԫ�ظ��Ƶ�����������a1
			a1(dimC)=a(dimC);
			for(unsigned int i=0;i<a.getDimNum();i++){//�����ά�����������һ������
				if(dimC[i]!=(a.getDimLen(i)-1)){
					dimC[i]++;
					break;
				}else if(i!=(a.getDimNum()-1))
					dimC[i]=0;
				else
					dimC[i]++;
			}
		}
		return fft_visit(a1);//����������������������ĸ���Ҷ�任
	}

	//��ά���ٸ���Ҷ��任
	lmtc::Array<ComplexNumber> FastFourierTransform::_fft(const lmtc::Array<ComplexNumber> &y){
		if(y.empty())
			return lmtc::Array<ComplexNumber>();//�ձ任
		std::vector<unsigned int> dim(y.getDimNum(),1);//��֤y��ά�Ƿ�Ϊ������
		for(unsigned int i=0;i<dim.size();i++){
			while(dim[i]<y.getDimLen(i))
				dim[i]*=2;
			if(dim[i]!=y.getDimLen(i))
				throw FastFourierTransformException("��ά����Ҷ��任_fft�Ĳ���y�ĸ�ά��ģ��ȫΪ������");
		}
		return _fft_visit(y);//����������������������ĸ���Ҷ��任
	}

	//��ά���ٸ���Ҷ�任����������
	lmtc::Array<ComplexNumber> FastFourierTransform::fft_visit(const lmtc::Array<ComplexNumber> &a){
		lmtc::Array<ComplexNumber> A=a;
		for(unsigned int d=0;d<A.getDimNum();d++){//�����ص�dά�ĸ���Ҷ�任
			std::vector<unsigned int> dimC(A.getDimNum(),0);
			while(true){//��������dά�������ά����
				//���е�ǰA.getDimNum()-1ά���괦�ĵ�dά����Ҷ�任
				std::vector<lmtc::ComplexNumber> ad(A.getDimLen(d));
				for(unsigned int i=0;i<A.getDimLen(d);i++){
					dimC[d]=i;
					ad[i]=A(dimC);
				}
				std::vector<lmtc::ComplexNumber> yd=fft(ad);
				for(unsigned int i=0;i<A.getDimLen(d);i++){
					dimC[d]=i;
					A(dimC)=yd[i];
				}
				if(A.getDimNum()==1)//һά�任����ɷ���
					break;
				for(unsigned int i=0;i<A.getDimNum();i++){//������һ������dά���A.getDimNum()-1ά����
					if(i==d)
						continue;
					if(dimC[i]!=(A.getDimLen(i)-1)){
						dimC[i]++;
						break;
					}else if(i==(d-1)&&d==(A.getDimNum()-1)||i==(A.getDimNum()-1)&&d!=(A.getDimNum()-1))
						dimC[i]++;
					else
						dimC[i]=0;
				}
				if(d==(A.getDimNum()-1)&&dimC[d-1]==A.getDimLen(d-1)||d!=(A.getDimNum()-1)&&dimC[A.getDimNum()-1]==A.getDimLen(A.getDimNum()-1))
					break;//�ص�dά�ı�������
			}
		}
		return A;
	}

	//��ά���ٸ���Ҷ��任����������
	lmtc::Array<ComplexNumber> FastFourierTransform::_fft_visit(const lmtc::Array<ComplexNumber> &y){
		lmtc::Array<ComplexNumber> A=y;
		for(int d=A.getDimNum()-1;d>=0;d--){//�����ص�dά�ĸ���Ҷ��任��������任��˳���෴��
			std::vector<unsigned int> dimC(A.getDimNum(),0);
			while(true){//��������dά�������ά����
				//���е�ǰA.getDimNum()-1ά���괦�ĵ�dά����Ҷ��任
				std::vector<lmtc::ComplexNumber> yd(A.getDimLen(d));
				for(unsigned int i=0;i<A.getDimLen(d);i++){
					dimC[d]=i;
					yd[i]=A(dimC);
				}
				std::vector<lmtc::ComplexNumber> ad=_fft(yd);
				for(unsigned int i=0;i<A.getDimLen(d);i++){
					dimC[d]=i;
					A(dimC)=ad[i];
				}
				if(A.getDimNum()==1)//һά�任����ɷ���
					break;
				for(unsigned int i=0;i<A.getDimNum();i++){//������һ������dά���A.getDimNum()-1ά����
					if(i==d)
						continue;
					if(dimC[i]!=(A.getDimLen(i)-1)){
						dimC[i]++;
						break;
					}else if(i==(d-1)&&d==(A.getDimNum()-1)||i==(A.getDimNum()-1)&&d!=(A.getDimNum()-1))
						dimC[i]++;
					else
						dimC[i]=0;
				}
				if(d==(A.getDimNum()-1)&&dimC[d-1]==A.getDimLen(d-1)||d!=(A.getDimNum()-1)&&dimC[A.getDimNum()-1]==A.getDimLen(A.getDimNum()-1))
					break;//�ص�dά�ı�������
			}
		}
		return A;
	}
}
