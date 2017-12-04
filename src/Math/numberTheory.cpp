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
#include "numberTheory.h"
#include "myException.h"
#include "myMath.h"
#include <vector>
#include <iostream>
namespace lmtc{

	unsigned long NumberTheory::generatePrimeNumberBySimpleTest(const unsigned long minNumber,const unsigned long smallPrimeNumber){
		std::vector<unsigned long> smallPrimeVec;
		smallPrimeVec.push_back(2);
		unsigned long curNumber=3;
		while(smallPrimeVec.size()<smallPrimeNumber){//��������smallPrimeNumber��С������
			for(std::vector<unsigned long>::iterator iter=smallPrimeVec.begin();iter!=smallPrimeVec.end();iter++){
				if((curNumber%(*iter))==0){
					curNumber+=2;
					break;
				}
				if((iter+1)==smallPrimeVec.end()){
					smallPrimeVec.push_back(curNumber);
					curNumber+=2;
					break;
				}
			}
		}
		unsigned long prime=minNumber;
		if(prime<=smallPrimeVec[smallPrimeVec.size()-1])
			return smallPrimeVec[smallPrimeVec.size()-1];
		while(true){//����С�����б�����������ȫ��ͨ����Ϊ������
			for(std::vector<unsigned long>::iterator iter=smallPrimeVec.begin();iter!=smallPrimeVec.end();iter++){
				if((prime%(*iter))==0){
					prime++;
					break;
				}
				if((iter+1)==smallPrimeVec.end()){
					return prime;
				}
			}
		
		}
	}

	//ŷ������㷨�����Լ��d=gcd(a,b),ʹ��d=ax+by
	void NumberTheory::gcd(const unsigned long a,const unsigned long b,unsigned long &d,long &x,long &y){
		if(0==b){
			d=a;
			x=1;
			y=0;
			return;
		}
		gcd(b,a%b,d,x,y);//�ݹ����
		x-=a/b*y;
		std::swap(x,y);
	}
	//ŷ������㷨�Ĵ����汾
	void NumberTheory::gcd(const BigDecimal &a,const BigDecimal &b,BigDecimal &d,BigDecimal &x,BigDecimal &y){
		if(!a.is_psv()||!b.is_psv()||a.num_dt()!=0||b.num_dt()!=0||d.num_dt()!=0||x.num_dt()!=0||y.num_dt()!=0)
			throw NumberTheoryException("�쳣�������汾gcd�Ĳ���a��b����С����,�����в�������ΪС����");
		if(b==0){
			d=a;
			x=1;
			y=0;
			return;
		}
		gcd(b,a%b,d,x,y);//�ݹ����
		x-=a/b*y;
		std::swap(x,y);
	}

	//��С������,����lcm(a,b).
	unsigned long NumberTheory::lcm(const unsigned long a,const unsigned long b){
		if(0==a||0==b)
			throw NumberTheoryException("lcm�Ĳ���a��b����Ϊ0");
		long x,y;
		unsigned long d;
		gcd(a,b,d,x,y);//�������Լ��
		return a*b/d;
	}

	//��ͬ�෽��ax=b(mod n),Ҫ��a,nΪ�������������쳣�������ر������н��������Ҫô�޽⣬Ҫôgcd(a,n)����ͬ�⣩��
	//O(lgn+gcd(a,n)),�μ����㷨����p533-535��
	std::vector<unsigned long> NumberTheory::modularLinearEquation(const unsigned long a,const long b,const unsigned long n){
		if(0==a||0==n)
			throw NumberTheoryException("modularLinearEquation�Ĳ���a��n����Ϊ0");
		unsigned long d;
		long x,y;
		gcd(a,n,d,x,y);
		if(0==(b%(long)d)){
			std::vector<unsigned long> X;
			long x0=x*b/((long)d);
			x0-=(x0/(long)n)*(long)n;
			if(x0<0)
				x0+=(long)n;
			for(unsigned int i=0;i<d;i++)
				X.push_back(((unsigned long)x0+i*n/d)%n);
			return X;
		}else
			return std::vector<unsigned long>();
	}

	unsigned long NumberTheory::modularLinearEquationsBySunTzu(const std::vector<long> &a,const std::vector<unsigned long> &n){
		if(a.size()!=n.size())
			throw NumberTheoryException("modularLinearEquationsBySunTzu�в�������a��n�Ĺ�ģ��һ�£�");
		unsigned long N=1;//N=n1*n2*...*nk
		for(unsigned int i=0;i<n.size();i++){
			if(0==n[i])
				throw NumberTheoryException("modularLinearEquationsBySunTzu�в�������n�е�ֵ����Ϊ�㣡");
			N*=n[i];
		}
		long A=0;//�ۼӽ��
		for(unsigned int i=0;i<n.size();i++){
			unsigned long mi=N/n[i];
			long x,y;
			unsigned long d;
			gcd(mi,n[i],d,x,y);//����ģniʱmi����x
			x%=(long)n[i];
			if(x<0)
				x+=n[i];
			A+=a[i]*(long)mi*x;
			A%=(long)N;
		}
		if(A<0)//�����Nȡģ
			A+=N;
		return (unsigned long)A;
	}

	//ģȡ������
	BigDecimal NumberTheory::modularExponentiation(const BigDecimal &a,const BigDecimal &b,const BigDecimal &n){
		if(a.num_dt()!=0||b.num_dt()!=0||n.num_dt()!=0||!b.is_psv()||!n.is_psv())
			throw NumberTheoryException("�쳣��modularExponentiation�Ĳ�������ΪС������b��n��Ǹ�");
		BigDecimal d=1;
		std::vector<bool> bst;
		b.getBinary(bst);
		for(int i=(int)bst.size()-1;i>=0;i--){
			d=d*d%n;
			if(bst[i])
				d=d*a%n;
		}
		return d;
	}

	//MillerRabin���������㷨
	bool NumberTheory::MillerRabin(const BigDecimal &n,const unsigned int s){
		if(!n.is_psv()||n.num_dt()!=0||n<3||n[0]%2==0)
			return false;//nΪ����С����С��3��Ϊż���򷵻�false(���Ǵ�����);
		for(unsigned int i=0;i<s;i++){
			BigDecimal a;//������a
			a=(n-1)*averageRandomBigFloat(std::max(n.dgt_size(),(unsigned int)6))+1;//1��n-1�����������
			if(!MillerRabin_witness(a,n))
				return false;//ȷ��Ϊ����
		}
		return true;
	}
	
	bool NumberTheory::MillerRabin_witness(const BigDecimal &a,const BigDecimal &n){
		BigDecimal n_=n-1;
		std::vector<bool> u;
		n_.getBinary(u);
		unsigned int t=0;
		while(!u[t])//ͳ�Ƶ�λ����Ŀ
			t++;
		BigDecimal x=1;//��ǰ�沿��ģȡ��
		for(int i=(int)u.size()-1;i>=(int)t;i--){
			x=x*x%n;
			if(u[i])
				x=x*a%n;
		}
		//�Ժ��沿��ģȡ��
		for(unsigned int i=0;i<t;i++){
			BigDecimal x1=x*x%n;
			if(x1==1&&x!=1&&x!=n_)
				return false;//����
			x=x1;
		}
		if(x!=1)
			return false;//�������ɷ������ж�
		return true; //Ǳ�ڵ�������
	}

	BigDecimal NumberTheory::generatePrimeNumberByMillerRabin(const unsigned int digitNum,const unsigned int time){
		if(digitNum==0)
			return 2;
		while(true){
			BigDecimal n=averageRandomBigInteger(digitNum);
			if(n[0]%2==0)
				n+=1;//ż����1��Ϊ����
			if(MillerRabin(n,time))
				return n;
		}
	}

	bool NumberTheory::generateKeyOfRSA(const unsigned int digitNum,BigDecimal &n,BigDecimal &e,BigDecimal &d,const unsigned int time){
		if(digitNum<2||n.num_dt()!=0||e.num_dt()!=0||d.num_dt()!=0)
			return false;
		unsigned int num=digitNum/2+1;
		BigDecimal p=generatePrimeNumberByMillerRabin(num,time);
		BigDecimal q;
		do{
			q=generatePrimeNumberByMillerRabin(num,time);
		}while(p==q);
		n=p*q;
		BigDecimal phi_n=(p-1)*(q-1);
		while(true){
			e=averageRandomBigInteger(std::min(phi_n.dgt_size()-1,std::max(phi_n.dgt_size()/2,(unsigned int)5)));
			if(e[0]%2==0)
				e+=1;//ż����1��Ϊ����
			BigDecimal g,x,y;
			gcd(e,phi_n,g,x,y);
			if(g==1){
				if(x<0){
					x%=phi_n;
					x+=phi_n;
				}
				d=x;
				break;
			}
		}
		return true;
	}

	//RSA���ܻ����(��Դ���),ʵ�ʾ���ģȡ�ݵĹ���.
	BigDecimal NumberTheory::cryptRSA(const BigDecimal &msg,const BigDecimal &e_d,const BigDecimal &n){
		if(msg>=n)
			throw NumberTheoryException("�쳣��cryptRSA�д�������Ϣmsg���ܴ���n");
		return modularExponentiation(msg,e_d,n);
	}
	//RSA���ܻ����(����ַ���)
	std::string NumberTheory::cryptRSA(const std::string &msg,const BigDecimal &e_d,const BigDecimal &n){
		BigDecimal msgDec=0;
		BigDecimal pow256=1;
		for(unsigned int i=0;i<msg.size();i++){//�ַ���תʮ���ƴ���
			msgDec+=pow256*((int)msg[i]+128);
			pow256*=256;
		}
		if(msgDec>=n)
			throw NumberTheoryException("�쳣��cryptRSA�д�������Ϣmsg��Ӧʮ���������ܴ���n");
		BigDecimal crypt=modularExponentiation(msgDec,e_d,n);//�����ӽ���
		std::string cryptStr;
		while(crypt!=0){//����ת��Ϊ�ַ���
			BigDecimal temp=crypt%256;
			crypt/=256;
			int tempInt=0;
			if(temp.dgt_size()>0)
				tempInt+=temp[0];
			if(temp.dgt_size()>1)
				tempInt+=temp[1]*10;
			if(temp.dgt_size()>2)
				tempInt+=temp[2]*100;
			cryptStr.push_back(tempInt-128);
		}
		return cryptStr;
	}
}