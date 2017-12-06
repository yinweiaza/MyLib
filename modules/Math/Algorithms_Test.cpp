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

// Algorithm_Lmtc.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"

//C++���
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <bitset>
#include <exception>
#include <memory>  //����auto_ptr�࣬���ڰ�ȫ�Ĺ���̬����Ķ��󣨲��ܹ���̬���飬��������������
#include <algorithm> //��׼���㷨
#include <numeric> //�������㷨
#include <functional> //��������
#include <fstream>

//C������
#include <cctype>
#include <cstddef>
#include <cstring>
#include <ctime>


//�Զ���ͷ�ļ�
#include "smartPtr.h"
#include "myException.h"
#include "array.h"
#include "symmetryArray.h"
#include "coordinateMapping.h"
#include "algebra.h"
#include "heap.h"
#include "redBlackTree.h"
#include "hash.h"
#include "numberTheory.h"
#include "myMath.h"
#include "sequence.h"
#include "matroid.h"
#include "binomialHeap.h"
#include "fibonacciHeap.h"
#include "nonIntersectSet.h"
#include "graph.h"
#include "number.h"
#include "fastFourierTransform.h"
#include "geometry.h"
#include "linearProgramming.h"
#include "operationsResearch.h"

//���ں�������Եĺ���ָ��
void f(double x){std::cout<<x<<std::endl;}
//����ɢ�в��Եĺ���ָ��
unsigned long keyToNumber(const unsigned long &t){return t;}
//������С���˹��Ʋ��Եĺ���ָ�롣
double polynomial(unsigned int i,double x){
	return std::pow(x,(int)i);
}
//���ڶ�Ԫ��С���˹��Ʋ��Եĺ���ָ�롣
double multiPolynomial(const std::vector<unsigned int> &iVec,const std::vector<double> &xVec){
	double temp1=1;
	for(unsigned int k=0;k<xVec.size();k++)
		temp1*=std::pow(xVec[k],(int)iVec[k]);
	return temp1;
}
//����������Եĺ���ָ��
bool canExpandTo(const int &item,const std::vector<int> &st){return true;}
void expandTo(const int &item,std::vector<int> &st){st.push_back(item);}

int _tmain(int argc, _TCHAR* argv[])
{
	srand((long)time(NULL));

///*����ʽ����
std::cout<<"����ʽ�������ԣ�"<<std::endl;
double pdAarr[]={9,3,4,8,49,7,22,7,8,47};
double pdBarr[]={4,3,21,15};
std::vector<lmtc::ComplexNumber> pdAvec(pdAarr,pdAarr+10);
std::vector<lmtc::ComplexNumber> pdBvec(pdBarr,pdBarr+4);
std::vector<lmtc::ComplexNumber>pdCvec=lmtc::Algebra::polynomialDivisionByFFT(pdAvec,pdBvec);
std::cout<<"����ʽ��Ϊ��by FFT,ֻ�ʺ���������"<<std::endl;
for(unsigned int i=0;i<pdCvec.size();i++)
	std::cout<<(std::string)pdCvec[i]<<"   ";
std::cout<<std::endl;//*/
std::vector<lmtc::ComplexNumber>pdA1vec=lmtc::Algebra::convolution(pdBvec,pdCvec);
std::cout<<"pdA1vec=pdBvec*pdCvecΪ��"<<std::endl;
for(unsigned int i=0;i<pdA1vec.size();i++)
	std::cout<<(std::string)pdA1vec[i]<<"   ";
std::cout<<std::endl;

std::vector<lmtc::ComplexNumber>pdCvecDv,pdCvecLf;
pdCvecDv=lmtc::Algebra::polynomialDivisionGeneral(pdAvec,pdBvec,pdCvecLf);
std::cout<<"����ʽ��Ϊ�����ط�������"<<std::endl;
for(unsigned int i=0;i<pdCvecDv.size();i++)
	std::cout<<(std::string)pdCvecDv[i]<<"   ";
std::cout<<std::endl;//*/
std::cout<<"����ʽ����Ϊ�����ط�������"<<std::endl;
for(unsigned int i=0;i<pdCvecLf.size();i++)
	std::cout<<(std::string)pdCvecLf[i]<<"   ";
std::cout<<std::endl;
std::vector<lmtc::ComplexNumber>pdA2vec=lmtc::Algebra::convolution(pdBvec,pdCvecDv);
std::cout<<"pdA2vec=pdBvec*pdCvecDvΪ��"<<std::endl;
for(unsigned int i=0;i<pdA2vec.size();i++)
	std::cout<<(std::string)pdA2vec[i]<<"   ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

///*��С���˹��������̬���������
std::cout<<"����̬��������ԣ�"<<std::endl;
double arr13[][3]={{1,-1,1},{1,1,1},{1,2,4},{1,3,9},{1,5,25}};
lmtc::Array<double> A4(2,5,3);
for(int i=0;i<5;i++)
	for(int j=0;j<3;j++)
		A4(i,j)=arr13[i][j];
double yArr[]={2,1,1,0,3};
double xArr[]={-1,1,2,3,5};
std::vector<double> X(xArr,xArr+5);
std::vector<double> Y(yArr,yArr+5);
std::vector<double> C;
std::vector<double> C1;
if(lmtc::Algebra::solveNormalityEquationsByLUP(A4,Y,C)){
//if(lmtc::Algebra::solveNormalityEquationsFast(A4,Y,C)){
	std::cout<<"��̬������Ľ�Ϊ��";
	for(unsigned int i=0;i<C.size();i++)
		std::cout<<C[i]<<"   ";
	std::cout<<std::endl;
}
else
	std::cout<<"��̬�����鲻�ɽ�"<<std::endl;
std::cout<<"��С���˹��Ʋ��ԣ�"<<std::endl;
//if(lmtc::Algebra::leastSquaresEstimationByLUP(polynomial,3,X,Y,C1)){
if(lmtc::Algebra::leastSquaresEstimationFast(polynomial,3,X,Y,C1)){
	std::cout<<"��С���˹��ƵĽ�Ϊ��";
	for(unsigned int i=0;i<C1.size();i++)
		std::cout<<C1[i]<<"   ";
	std::cout<<std::endl;
}
else
	std::cout<<"��С���˲��ɽ�"<<std::endl;
std::cout<<std::endl;//*/

///*��Ԫ��С���˹���,�˴������ƺ���Ϊ��Ԫ̩�ռ�������Ԫ���ϵ��κ����Ͷ�Ԫ��������С���˹���������ơ�
std::cout<<"��Ԫ��С���˹��Ʋ��ԣ�"<<std::endl;
unsigned int pmVarNum=2;//�Ա�����Ŀ
unsigned int pmDataRowNum=50;//����������Ŀ
unsigned int nVecPMArr[]={2,2};//����������ߴ���
std::vector<unsigned int> nVecPM(nVecPMArr,nVecPMArr+2);
lmtc::Array<double> thalerCoaf(2,3,3);//����������̩��ϵ��
thalerCoaf(0,0)=9;
thalerCoaf(0,1)=8;
thalerCoaf(0,2)=7;
thalerCoaf(1,0)=6;
thalerCoaf(1,1)=5;
thalerCoaf(1,2)=4;
thalerCoaf(2,0)=3;
thalerCoaf(2,1)=2;
thalerCoaf(2,2)=1;
lmtc::Array<double> XPM1(2,pmDataRowNum,pmVarNum);
std::vector<double> YPM1(pmDataRowNum);
for(unsigned int i=0;i<pmDataRowNum;i++){//������Ԫ̩�ռ��������������㡣
	std::vector<double> xVec;
	for(unsigned int j=0;j<pmVarNum;j++){
		XPM1(i,j)=lmtc::averageRandom(0,100);
		xVec.push_back(XPM1(i,j));
	}
	std::vector<unsigned int> iVec(2);
	for(unsigned int p=0;p<thalerCoaf.getDimLen(0);p++)
		for(unsigned int q=0;q<thalerCoaf.getDimLen(1);q++){
			iVec[0]=p;
			iVec[1]=q;
			YPM1[i]+=multiPolynomial(iVec,xVec)*thalerCoaf(p,q);
		}
	//��������������
	YPM1[i]+=lmtc::averageRandom(-2,2);
}
lmtc::Array<double> CPM1;//��С���˹��Ƶõ���ϵ����������顣
if(true==lmtc::Algebra::leastSquaresEstimationByLUP(multiPolynomial,nVecPM,XPM1,YPM1,CPM1))
	for(unsigned int i=0;i<CPM1.getDimLen(0);i++){
		for(unsigned int j=0;j<CPM1.getDimLen(1);j++)//�����Ԫ��С���˹��ƵĽ��
			std::cout<<"CPM1("<<i<<","<<j<<") = "<<CPM1(i,j)<<"   ";
		std::cout<<std::endl;
	}
std::cout<<std::endl;
std::cout<<std::endl;//*/


///*KMP����ƥ���㷨����
std::cout<<"KMP����ƥ�����"<<std::endl;
int seqKMP[]={1,3,5,4,6,2,0,4,3,2,9,3,5,4,6,2,3,5,4,6,2};
int patKMP[]={5,4,6,2};
int *ptrKMP=seqKMP;
int seqSize=21;
int patSize=4;
while(true){
	ptrKMP=lmtc::KMP<int>(ptrKMP,seqKMP+seqSize,patKMP,patKMP+patSize);
	if(ptrKMP==(seqKMP+seqSize))
		break;
	else{
		std::cout<<"st="<<(ptrKMP-seqKMP)<<",";
		ptrKMP++;
	}
}
std::cout<<std::endl;
std::cout<<std::endl;//*/


///*��������
std::cout<<"�������ԣ�"<<std::endl;
unsigned short unsh=0;
lmtc::BigDecimal bgDc("114159.556454325435");
lmtc::BigDecimal bgDc1(std::string("122598.456521"));
bgDc.resetPrecision(20);
bgDc1.resetPrecision(8);
bgDc+=bgDc1;
bgDc-=bgDc1;
bgDc*=-bgDc1;
bgDc/=-bgDc1;
bgDc*=5.23;
bgDc/=5.23;
bgDc=bgDc%bgDc1+(bgDc/bgDc1).getInteger()*bgDc1;
bgDc<<2;
bgDc>>2;
std::cout<<(std::string)bgDc<<"  "<<(std::string)(bgDc.getInteger())<<"  "<<(std::string)(bgDc.getFloat())<<"  "<<std::endl;
std::cout<<(bgDc>=bgDc)<<std::endl;
std::cout<<(bgDc<=122355555.555557877988789878)<<std::endl;
bool bstArr[]={0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1};
std::vector<bool> bstVec(bstArr,bstArr+110);
lmtc::BigDecimal bgDc2(bstVec);
std::cout<<"�ɶ�����ת�ɵĴ���Ϊ��"<<(std::string)bgDc2<<std::endl;
std::vector<bool> bstVec0;
unsigned int biDt0=bgDc2.getBinary(bstVec0);
std::cout<<"�д�������ת���ɵĶ�������Ϊ��"<<std::endl;
for(int i=(int)bstVec0.size()-1;i>=0;i--){
	std::cout<<bstVec0[i];
	if(i==biDt0&&i!=0)
		std::cout<<".";
}
std::cout<<std::endl;
std::vector<bool> bstVec1;
lmtc::BigDecimal bgDc3(std::string("122598.75"));
unsigned int biDt=bgDc3.getBinary(bstVec1);
std::cout<<"�ɸ�����ת���ɵĶ�����С��Ϊ��"<<std::endl;
for(int i=(int)bstVec1.size()-1;i>=0;i--){
	std::cout<<bstVec1[i];
	if(i==biDt&&i!=0)
		std::cout<<".";
}
std::cout<<std::endl;
std::cout<<bgDc3[7]<<std::endl<<std::endl;//*/


///*���Լ������С����������
std::cout<<"���Լ�����ԣ�"<<std::endl;
unsigned long aGcd=5*7*23*37*53,bGcd=3*11*23*37*179,dGcd;
long xGcd,yGcd;
lmtc::NumberTheory::gcd(aGcd,bGcd,dGcd,xGcd,yGcd);
if(dGcd==(aGcd*xGcd+bGcd*yGcd))
	std::cout<<"gcd("<<aGcd<<","<<bGcd<<")="<<dGcd<<"="<<aGcd<<"*"<<xGcd<<"+"<<bGcd<<"*"<<yGcd<<std::endl;
else
	std::cout<<"�㷨����!"<<std::endl;
std::cout<<"��С������Ϊ��"<<lmtc::NumberTheory::lcm(23,37)<<std::endl;
std::cout<<"�����汾�����Լ�����ԣ�"<<std::endl;
lmtc::BigDecimal aGcd1=5*7*23*37*53,bGcd1=3*11*23*37*179,dGcd1,xGcd1,yGcd1;
lmtc::NumberTheory::gcd(aGcd1,bGcd1,dGcd1,xGcd1,yGcd1);
if(dGcd1==(aGcd1*xGcd1+bGcd1*yGcd1))
std::cout<<"gcd("<<(std::string)aGcd1<<","<<(std::string)bGcd1<<")="<<(std::string)dGcd1<<"="<<(std::string)aGcd1<<"*"<<(std::string)xGcd1<<"+"<<(std::string)bGcd1<<"*"<<(std::string)yGcd1<<std::endl;
else
	std::cout<<"�㷨����!"<<std::endl;
std::cout<<std::endl;//*/

///*��ͬ�෽�̲���
std::cout<<"��ͬ�෽�̲��ԣ�"<<std::endl;
unsigned long mdleqa=14,mdleqn=100;
long mdleqb=30;
std::vector<unsigned long> mdleqX=lmtc::NumberTheory::modularLinearEquation(mdleqa,mdleqb,mdleqn);
std::cout<<"ͬ�෽�̵����н�Ϊ��";
for(unsigned int i=0;i<mdleqX.size();i++)
	std::cout<<"x"<<i<<"="<<mdleqX[i]<<"    ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

///*���Ӷ����ͬ�෽�������
std::cout<<"���Ӷ����ͬ�෽������ԣ�"<<std::endl;
std::vector<unsigned long> szN;
std::vector<long> szA;
szN.push_back(7);
szN.push_back(19);
szN.push_back(23);
szA.push_back(-16);
szA.push_back(35);
szA.push_back(12);
double szX=lmtc::NumberTheory::modularLinearEquationsBySunTzu(szA,szN);
std::cout<<"ͬ�෽����Ľ�Ϊ��"<<szX<<std::endl;
std::cout<<std::endl;//*/


///*ģȡ������
std::cout<<"ģȡ��������ԣ�"<<std::endl;
lmtc::BigDecimal moExpA=8,moExpB=10,moExpN=1000000000;
lmtc::BigDecimal moExpRs=lmtc::NumberTheory::modularExponentiation(moExpA,moExpB,moExpN);
std::cout<<(std::string)moExpA<<"^"<<(std::string)moExpB<<" % "<<(std::string)moExpN<<" = "<<(std::string)moExpRs<<std::endl;
std::cout<<std::endl;//*/

///*�����������
std::cout<<"�����������"<<std::endl;
lmtc::BigDecimal randBigInt=lmtc::averageRandomBigInteger(32);
std::cout<<"�������Ϊ��"<<(std::string)randBigInt<<std::endl;
lmtc::BigDecimal randBigFloat=lmtc::averageRandomBigFloat(32);
std::cout<<"���С��Ϊ��"<<(std::string)randBigFloat<<std::endl;
std::cout<<std::endl;//*/

///*MillerRabin��������
std::cout<<"MillerRabin��������"<<std::endl;
if(lmtc::NumberTheory::MillerRabin(lmtc::MAX_PRIME))
	std::cout<<"Ϊ������"<<std::endl;
else 
	std::cout<<"��Ϊ������"<<std::endl;
std::cout<<std::endl;//*/

/*//��������������(������50λ�Ĵ�����Ϊ��57833101454301616138860708250340300807978842653707)
std::cout<<"��������������"<<std::endl;
std::cout<<"�����Ĵ�����Ϊ��"<<(std::string)lmtc::NumberTheory::generatePrimeNumberByMillerRabin(50)<<std::endl;
std::cout<<std::endl;//*/

///*RSA�ۺϲ���
std::cout<<"RSA�ۺϲ���"<<std::endl;
///*����Ϊ�ܼ���50λʮ���ƴ����Ĺ�Կ(RSAE,RSAN)����Կ(RSAD,RSAN)
lmtc::BigDecimal RSAN("2521856375150261634008882739223972423156297845443147"),
                 RSAE("86244010814085248452881469"),
                 RSAD("2440156170950764584581132081694305962063303775164837");
//*/
/*//����Ϊ�ܼ���200λʮ���ƴ����Ĺ�Կ(RSAE,RSAN)����Կ(RSAD,RSAN)
lmtc::BigDecimal RSAN("6380616938818171200074355477705576014995641232691298837296561137740893181836377916290884463497517597095768366165545899640091860658179992054086438007286317457020617942159137676906625739461654367522339777"),
                 RSAE("20982538709357952226499259416280815673153042323347356977376673248400375494362252349061307887319768317"),
                 RSAD("5907082193902464506985888020290474680163560377060753951361684708389611677387682422762695048148474090312313036781635853755482565201060158527257044488185981709234453211197945547224621783304143918340505813");
//*/
/*//������Կ����Կ�Ĵ��롣��������200λ�Ĺ�Կ����Կ����2��Сʱ����˲���һ�ξ�Ӧ�ñ���������
unsigned int RSADgtNum=200;//������Ϣ��Ӧʮ���ƴ��������λ���������Ĺ�Կ��˽Կֻ�ʺ���������Ƶ���Ϣ�����򱨴�
if(true==lmtc::NumberTheory::generateKeyOfRSA(RSADgtNum,RSAN,RSAE,RSAD)){
	std::cout<<"��ԿΪ��"<<(std::string)RSAE<<"  &  "<<(std::string)RSAN<<std::endl;
	std::cout<<"˽ԿΪ��"<<(std::string)RSAD<<"  &  "<<(std::string)RSAN<<std::endl;
}else
	std::cout<<"������Կ˽Կ���̳���"<<std::endl;
//*/
///*
lmtc::BigDecimal RSAMSGDec="123456789987654321123456789987654321";//�����ܴ���
std::cout<<"�����ܴ���Ϊ��"<<(std::string)RSAMSGDec<<std::endl;
lmtc::BigDecimal enCryptDec=lmtc::NumberTheory::cryptRSA(RSAMSGDec,RSAE,RSAN);//����
std::cout<<"���ܽ������Ϊ��"<<(std::string)enCryptDec<<std::endl;
lmtc::BigDecimal deCryptDec=lmtc::NumberTheory::cryptRSA(enCryptDec,RSAD,RSAN);//����
std::cout<<"����Ϊԭ����Ϊ��"<<(std::string)deCryptDec<<std::endl;

//std::string RSAMSG="�㷨����ѧ��һ����֧���Ǹ��ڴ����Ե�һ�ſ�ѧ���ǻ㼯��������Ե�˼ά������";//�������ַ���
std::string RSAMSG="�㷨��һ��������";//�������ַ���
std::cout<<"�������ַ���Ϊ��"<<RSAMSG<<std::endl;
std::string enCryptStr=lmtc::NumberTheory::cryptRSA(RSAMSG,RSAE,RSAN);//����
std::cout<<"���ܽ���ַ���Ϊ��"<<enCryptStr<<std::endl;
std::string deCryptStr=lmtc::NumberTheory::cryptRSA(enCryptStr,RSAD,RSAN);//����
std::cout<<"����Ϊԭ�ַ���Ϊ��"<<deCryptStr<<std::endl;
std::cout<<std::endl;//*/

///*�����߶β���
std::cout<<"�����߶β���"<<std::endl;
lmtc::Segment seg1(2,1,5,3);
lmtc::Segment seg2(3,5,5,1);
if(seg1.intersect(seg2))
	std::cout<<"seg1��seg2�ཻ"<<std::endl;
else
	std::cout<<"���ཻ"<<std::endl;

int clkDirect=seg1.clockDirect(seg2);
if(clkDirect>0)
	std::cout<<"seg1��seg2��˳ʱ�뷽��"<<std::endl;
else if(clkDirect<0)
	std::cout<<"seg1��seg2����ʱ�뷽��"<<std::endl;
else
	std::cout<<"seg1��seg2����"<<std::endl;

int turnDirect=seg1.turnDirect(seg2);
if(turnDirect>0)
	std::cout<<"seg1��seg2Ϊ�ҹ�"<<std::endl;
else if(turnDirect<0)
	std::cout<<"seg1��seg2Ϊ���"<<std::endl;
else
	std::cout<<"seg1��seg2����"<<std::endl;
std::cout<<std::endl;//*/

///*ȷ������һ���߶��Ƿ��ཻ
std::cout<<"�߶��Ƿ��ཻ����"<<std::endl;
std::vector<lmtc::Segment> segments;
segments.push_back(lmtc::Segment(0.5,2,3.3,3));
segments.push_back(lmtc::Segment(2.6,3.6,5.2,3.1));
segments.push_back(lmtc::Segment(3,5,9,2.5));
segments.push_back(lmtc::Segment(4,2.2,10,4.5));
segments.push_back(lmtc::Segment(10,3.5,10.5,3.999));
segments.push_back(lmtc::Segment(7.7,3.4,10.5,4));
if(lmtc::Geometry::anySegmentsIntersect(segments))
	std::cout<<"���������߶��ཻ"<<std::endl;
else
	std::cout<<"�����߶ζ����ཻ"<<std::endl;
std::cout<<std::endl;
//*/

///*��㼯��͹��
std::cout<<"͹������"<<std::endl;
std::vector<lmtc::Point> psVec;
psVec.push_back(lmtc::Point(1,2));
psVec.push_back(lmtc::Point(2,1));
psVec.push_back(lmtc::Point(3,5));
psVec.push_back(lmtc::Point(5,5));
psVec.push_back(lmtc::Point(6,3));
psVec.push_back(lmtc::Point(3,4));
psVec.push_back(lmtc::Point(3,4.8));
psVec.push_back(lmtc::Point(3,4.2));
psVec.push_back(lmtc::Point(3,3.9));
psVec.push_back(lmtc::Point(3,4.89));
std::vector<unsigned int> ordVec=lmtc::Geometry::convexHallGraham(psVec);
std::cout<<"͹���ĸ��������Ϊ��";
for(unsigned int i=0;i<ordVec.size();i++)
	std::cout<<ordVec[i]<<",";
std::cout<<std::endl;
std::cout<<std::endl;
//*/

///*��������
std::cout<<"�����Բ���"<<std::endl;
std::vector<lmtc::Point> psVecClsPair;
psVecClsPair.push_back(lmtc::Point(4,2));
psVecClsPair.push_back(lmtc::Point(3,0));
psVecClsPair.push_back(lmtc::Point(5,0));
psVecClsPair.push_back(lmtc::Point(2,3));
psVecClsPair.push_back(lmtc::Point(6,2));
psVecClsPair.push_back(lmtc::Point(1,2));
psVecClsPair.push_back(lmtc::Point(7,3));
psVecClsPair.push_back(lmtc::Point(7,1));
psVecClsPair.push_back(lmtc::Point(0,1));
psVecClsPair.push_back(lmtc::Point(8,2));
psVecClsPair.push_back(lmtc::Point(4.4,1));
//psVecClsPair.push_back(lmtc::Point(4,2));
unsigned int clsP1,clsP2;
double clsLen=lmtc::Geometry::closestPointPair(psVecClsPair,clsP1,clsP2);
std::cout<<"�����Ե����Ϊ��"<<clsP1<<","<<clsP2<<" . ����Ϊ��"<<clsLen<<std::endl;
std::cout<<std::endl;
std::cout<<std::endl;
//*/

///*���Թ滮����
std::cout<<"��׼�����Թ滮����"<<std::endl;
lmtc::Array<long double> AsimplexStd(2,3,3);
std::vector<long double> bsimplexStd(3);
std::vector<long double> csimplexStd(3);
std::vector<long double> XsimplexStd;
long double VsimplexStd;
AsimplexStd(0,0)=1;AsimplexStd(0,1)=1;AsimplexStd(0,2)=3;
AsimplexStd(1,0)=2;AsimplexStd(1,1)=2;AsimplexStd(1,2)=5;
AsimplexStd(2,0)=4;AsimplexStd(2,1)=1;AsimplexStd(2,2)=2;
bsimplexStd[0]=30;bsimplexStd[1]=24,bsimplexStd[2]=36;
csimplexStd[0]=3;csimplexStd[1]=1,csimplexStd[2]=2;
/*AsimplexStd(0,0)=2;AsimplexStd(0,1)=-1;
AsimplexStd(1,0)=1;AsimplexStd(1,1)=-5;
bsimplexStd[0]=2;bsimplexStd[1]=-4;
csimplexStd[0]=2;csimplexStd[1]=-1;*/
int signStd=lmtc::LinearProgramming::simplexStandardForm(AsimplexStd,bsimplexStd,csimplexStd,XsimplexStd,VsimplexStd);
if(signStd==-1)
	std::cout<<"�����Թ滮�����޽�"<<std::endl;
else if(signStd==0)
	std::cout<<"�����Թ滮�����޿��н�"<<std::endl;
else if(signStd==1){
	std::cout<<"�����Թ滮��������ֵΪ��"<<VsimplexStd<<std::endl;
	std::cout<<"�����Թ滮�������Ž�Ϊ��"<<std::endl;
	for(unsigned int i=0;i<XsimplexStd.size();i++)
		std::cout<<"x"<<i<<" = "<<XsimplexStd[i]<<" ;";
	std::cout<<std::endl;
}
std::cout<<std::endl;

std::cout<<"һ�������Թ滮����"<<std::endl;
lmtc::Array<long double> AsimplexGeneral(2,5,2);
std::vector<long double> bsimplexGeneral(5);
std::vector<long double> csimplexGeneral(2);
bool ismaximize=false;
std::vector<int> cmpVec(5);
std::vector<bool> nonNegVec(2);
std::vector<long double> XsimplexGeneral;
long double VsimplexGeneral;
AsimplexGeneral(0,0)=1;AsimplexGeneral(0,1)=-1;
AsimplexGeneral(1,0)=1;AsimplexGeneral(1,1)=1;
AsimplexGeneral(2,0)=1;AsimplexGeneral(2,1)=0;
AsimplexGeneral(3,0)=1;AsimplexGeneral(3,1)=1;
AsimplexGeneral(4,0)=1;AsimplexGeneral(4,1)=-1;
bsimplexGeneral[0]=0;bsimplexGeneral[1]=-1;bsimplexGeneral[2]=5;bsimplexGeneral[3]=7;bsimplexGeneral[4]=1;
csimplexGeneral[0]=1.5;csimplexGeneral[1]=1;
cmpVec[0]=1;cmpVec[1]=1;cmpVec[2]=-1;cmpVec[3]=-1;cmpVec[4]=0;
nonNegVec[0]=false;nonNegVec[1]=false;
int signGeneral=lmtc::LinearProgramming::simplexGeneralForm(AsimplexGeneral,bsimplexGeneral,csimplexGeneral,ismaximize,cmpVec,nonNegVec,XsimplexGeneral,VsimplexGeneral);
if(signGeneral==-1)
	std::cout<<"�����Թ滮�����޽�"<<std::endl;
else if(signGeneral==0)
	std::cout<<"�����Թ滮�����޿��н�"<<std::endl;
else if(signGeneral==1){
	std::cout<<"�����Թ滮��������ֵΪ��"<<VsimplexGeneral<<std::endl;
	std::cout<<"�����Թ滮�������Ž�Ϊ��"<<std::endl;
	for(unsigned int i=0;i<XsimplexGeneral.size();i++)
		std::cout<<"x"<<i<<" = "<<XsimplexGeneral[i]<<" ;";
	std::cout<<std::endl;
}
std::cout<<std::endl;
//*/


///*�����������
std::cout<<"����������ԣ�"<<std::endl;
unsigned NAssign=12;
lmtc::Array<long double> costAssign(2,NAssign,NAssign);
for(unsigned int i=0;i<costAssign.getDimLen(0);i++)
	for(unsigned int j=0;j<costAssign.getDimLen(1);j++)
		costAssign(i,j)=lmtc::averageRandom(0,50);
std::vector<unsigned int> assignVec;
long double minCostAssign;
if(true==lmtc::OperationsResearch::bestAssign(costAssign,assignVec,minCostAssign))
{
	std::cout<<"�÷���������ͳɱ�Ϊ��"<<minCostAssign<<std::endl;
	std::cout<<"�÷����������Ž�Ϊ��"<<std::endl;
	for(unsigned int i=0;i<assignVec.size();i++)
		std::cout<<"x"<<i<<" = "<<assignVec[i]<<" ;";
	std::cout<<std::endl;

}else{
	std::cout<<"�������Թ滮���������ʧ�ܣ�"<<std::endl;
}
std::cout<<std::endl;//*/

///*���Ŷ�����ͼ����
std::cout<<"���Ŷ�����ͼ���ԣ�"<<std::endl;
unsigned NTDSG=12;
lmtc::Array<long double> ajacencyMatrixTDSG(2,NTDSG,NTDSG);
for(unsigned int i=0;i<ajacencyMatrixTDSG.getDimLen(0);i++)
	for(unsigned int j=0;j<ajacencyMatrixTDSG.getDimLen(1);j++)
		ajacencyMatrixTDSG(i,j)=lmtc::averageRandom(20,50);
std::vector<unsigned int> connectVecTDSG;
long double minLenTDSG;
if(true==lmtc::OperationsResearch::bestTwoDegreeSubGraph(ajacencyMatrixTDSG,connectVecTDSG,minLenTDSG))
{
	std::cout<<"�����Ŷ�����ͼ��̳���Ϊ��"<<minLenTDSG<<std::endl;
	std::cout<<"�����Ŷ�����ͼ���Ž�Ϊ��"<<std::endl;
	for(unsigned int i=0;i<connectVecTDSG.size();i++)
		std::cout<<"x"<<i<<" = "<<connectVecTDSG[i]<<" ;";
	std::cout<<std::endl;

}else{
	std::cout<<"�������Թ滮�����Ŷ�����ͼʧ�ܣ�"<<std::endl;
}
std::cout<<std::endl;//*/

///*������ԣ��ҳ������һ���������Ȩֵ�Ķ����Ӽ�����Ϊ�������⣬���߿��Թ���
std::cout<<"������ԣ�"<<std::endl;
double weight[]={5,3,8};
int item[]={3,2,9};
lmtc::Matroid<double,int,std::vector<int>> matroid(weight,weight+3,item,item+3,canExpandTo,expandTo);
std::vector<int> result;
double w=matroid.getGreatestWeightIndependentSet(result);
for(std::vector<int>::iterator p=result.begin();p!=result.end();p++)
	std::cout<<*p<<std::endl;
std::cout<<"Ȩ��:"<<w<<std::endl;
std::cout<<std::endl;//*/


///*�Գ��������
std::cout<<"�Գ�������ԣ�"<<std::endl;
std::vector<unsigned int>symArrDimV(4);
symArrDimV[0]=7;symArrDimV[1]=8;symArrDimV[2]=6;symArrDimV[3]=9;
lmtc::SymmetryArray<int> symArr(4,symArrDimV);
symArrDimV[0]=1;symArrDimV[1]=2;symArrDimV[2]=3;symArrDimV[3]=4;
symArr(symArrDimV)=12;
symArrDimV[0]=4;symArrDimV[1]=2;symArrDimV[2]=3;symArrDimV[3]=1;
const lmtc::SymmetryArray<int> symArrConst=symArr;
std::cout<<symArrConst(1,2,4,3)<<"  "<<symArr(1,4,2,3)<<"  "<<symArrConst(1,3,2,4)<<"  "<<symArr(1,4,3,2)<<"  "<<symArrConst(symArrDimV)<<"  "<<symArr(symArrDimV)<<std::endl;
std::cout<<"7*8*6*9="<<7*8*6*9<<std::endl;
std::cout<<"actual memory needed ="<<symArr.size()<<std::endl;
std::cout<<std::endl;//*/

///*һ��Ѳ���
std::cout<<"һ��Ѳ��ԣ�"<<std::endl;
int arrHeap[]={5,4,3,2,1,6,8,10,-1};
lmtc::Heap<int> hp(arrHeap,arrHeap+9);
//hp.printHeap();
hp.increaseKey(3,5);
hp.insert(9);
//hp.printHeap();
std::vector<int> st=hp.sort();
for(size_t i=0;i<st.size();i++)
	std::cout<<st[i]<<"  ";
std::cout<<std::endl; 
//hp.printHeap();
std::cout<<std::endl;//*/

///*ȫ����ȫɢ�в���
std::cout<<"ȫ����ȫɢ�в��ԣ�"<<std::endl;
lmtc::CompleteHash<unsigned long> hash(50,lmtc::MAX_PRIME,keyToNumber);
hash.insert(20);
for(unsigned int i=0;i<1000;i++)
	hash.insert((unsigned long)(lmtc::averageRandom()*50000));
hash.completeHashOptimize();
unsigned long *hashItem=hash.search(20);
hash.remove(*hashItem);
std::cout<<std::endl;//*/
	

///*���������
std::cout<<"��������ԣ�"<<std::endl;
lmtc::RedBlackTree<int> rbtree;
std::cout<<"�������"<<std::endl;
for(int i=1;i<20;i++)
{
	int temp=(int)(lmtc::averageRandom()*50000);//(int)(lmtc::averageRandom()*100000);
	if(temp!=rbtree.getItemValue(rbtree.insert(temp)))
		std::cout<<"error"<<std::endl;
	if(temp!=rbtree.getItemValue(rbtree.search(temp)))
		std::cout<<"error"<<std::endl;
	std::cout<<temp<<std::endl;
}

std::cout<<"size= "<<rbtree.size()<<std::endl;
	
rbtree.insert(12);
lmtc::RedBlackTree<int>::ItemType &po=rbtree.deleteItem(rbtree.search(12));
for(unsigned int i=0;i<rbtree.size();i++){
	lmtc::RedBlackTree<int>::ItemType t=rbtree.searchItmOfOrder(i);
	if(rbtree.getOrderOfItm(t)!=i)
		std::cout<<"˳��ͳ�ƴ���"<<std::endl;
}

std::cout<<"size= "<<rbtree.size()<<std::endl;

std::cout<<"���ɾ��"<<std::endl;
for(int i=1;i<20;i++)
{
	int temp=(int)(lmtc::averageRandom()*50000);//(int)(lmtc::averageRandom()*100000);
	lmtc::RedBlackTree<int>::ItemType del=rbtree.deleteItem(rbtree.search(temp));
	if(!del.isEmpty())
		std::cout<<rbtree.getItemValue(del)<<std::endl;
}

std::cout<<"size= "<<rbtree.size()<<std::endl;

std::cout<<"���������"<<std::endl;
lmtc::RedBlackTree<int>::ItemType x=rbtree.maximum();
while(true){
	if(x.isEmpty())
		break;
	std::cout<<rbtree.getItemValue(x)<<std::endl;
	x=rbtree.predecessor(x);
}

std::cout<<"�������"<<std::endl;
rbtree.traver_inOrder(f);
std::cout<<"ǰ�����"<<std::endl;
rbtree.traver_preOrder(f);
std::cout<<"��������"<<std::endl;

std::cout<<"��ʼ��֤"<<std::endl;
rbtree.asertTree(rbtree.getRoot());

for(unsigned int i=0;i<rbtree.size();i++){
	lmtc::RedBlackTree<int>::ItemType t=rbtree.searchItmOfOrder(i);
	if(rbtree.getOrderOfItm(t)!=i)
		std::cout<<"˳��ͳ�ƴ���"<<std::endl;
}
std::cout<<rbtree.size()<<std::endl;
rbtree.setEmpty();
std::cout<<"over"<<rbtree.size()<<std::endl;
std::cout<<std::endl;//*/

///*���ñ�׼��˳��ͳ�Ʋ���
std::cout<<"��׼��˳��ͳ�Ʋ��ԣ�"<<std::endl;
int ntha1[]={2,5,8,10,12,7,99,3,54,46};
int ntha2[]={3,5,6,13,9,19,8,7,54,28,99,46,3,5,2,5,8,10,12,7,99,3,54,46,2,5,8,10,12,7,99,3,54,46,2,5,8,10,12,7,99,3,54,46};
std::vector<int> nthv1(ntha1,ntha1+10);
std::vector<int> nthv2(ntha2,ntha2+44);
std::vector<int> nthvec=lmtc::longestCommonSubsequence<int>(nthv1.begin(),nthv1.end(),nthv2.begin(),nthv2.end());
for(unsigned int i=0;i<nthvec.size();i++)
	std::cout<<nthvec[i]<<std::endl;
std::random_shuffle(ntha2,ntha2+44);
std::nth_element(ntha2,ntha2+20,ntha2+44);
std::cout<<"˳��ͳ��"<<std::endl;
for(int i=0;i<44;i++)
	std::cout<<ntha2[i]<<std::endl;
std::cout<<std::endl;//*/

///*����Ѳ���
std::cout<<"����Ѳ��ԣ�"<<std::endl;
lmtc::BinomialHeap<int> bihp;
lmtc::BinomialHeap<int> bihp1;

for(int i=0;i<100;i++){
	int temp=(int)(lmtc::averageRandom()*5000000);
	int temp1=(int)(lmtc::averageRandom()*5000000);
	bihp.increaseKey(bihp.insert(temp),(int)(lmtc::averageRandom()*5000000));
	bihp1.increaseKey(bihp1.insert(temp1),(int)(lmtc::averageRandom()*5000000));
	if(bihp.get_prio().isEmpty()||bihp1.get_prio().isEmpty())
		std::cout<<"error"<<std::endl;
	if(lmtc::averageRandom()<0.5){
		int pio=bihp.getItemValue(bihp.get_prio());
		int pio1=bihp.getItemValue(bihp.extract_prio());
		if(pio!=pio1)
			std::cout<<"error1"<<std::endl;
	}
	if(lmtc::averageRandom()<0.5){
		int pio=bihp1.getItemValue(bihp1.get_prio());
		int pio1=bihp1.getItemValue(bihp1.extract_prio());
		if(pio!=pio1)
			std::cout<<"error1"<<std::endl;
	}

	if(lmtc::averageRandom()<0.1)
		bihp.asertBinomialHeap();

	if(lmtc::averageRandom()<0.1)
		bihp1.asertBinomialHeap();
}

bihp.heapUnion(bihp1);
std::cout<<"��֤��ʼ"<<std::endl;
bihp.asertBinomialHeap();
std::cout<<"��֤����"<<std::endl;

std::cout<<bihp.size()<<"end"<<bihp1.size()<<std::endl;

std::cout<<"�ͷ���Դ1"<<std::endl;
bihp.setEmpty();
std::cout<<"�ͷ���Դ2"<<std::endl;
bihp.traver_inOrder(f);
std::cout<<std::endl;//*/

///*쳲������Ѳ���
std::cout<<"쳲������Ѳ��ԣ�"<<std::endl;
lmtc::FibonacciHeap<int> fbnqhp;
lmtc::FibonacciHeap<int> fbnqhp1;
for(int i=0;i<100;i++){
	int temp=(int)(lmtc::averageRandom()*5000000);
	int temp1=(int)(lmtc::averageRandom()*5000000);
	fbnqhp.increaseKey(fbnqhp.insert(temp),(int)(lmtc::averageRandom()*5000000));
	fbnqhp1.increaseKey(fbnqhp1.insert(temp1),(int)(lmtc::averageRandom()*5000000));
	if(fbnqhp.get_prio().isEmpty()||fbnqhp1.get_prio().isEmpty())
		std::cout<<"error"<<std::endl;
	if(lmtc::averageRandom()<0.5){
		int pio=fbnqhp.getItemValue(fbnqhp.get_prio());
		int pio1=fbnqhp.getItemValue(fbnqhp.extract_prio());
		if(pio!=pio1)
			std::cout<<"error1"<<std::endl;
	}
	if(lmtc::averageRandom()<0.5){
		int pio=fbnqhp1.getItemValue(fbnqhp1.get_prio());
		int pio1=fbnqhp1.getItemValue(fbnqhp1.extract_prio());
		if(pio!=pio1)
			std::cout<<"error1"<<std::endl;
	}

	if(lmtc::averageRandom()<0.1)
		fbnqhp.asertFibonacciHeap();

	if(lmtc::averageRandom()<0.1)
		fbnqhp1.asertFibonacciHeap();
}
fbnqhp.heapUnion(fbnqhp1);

std::cout<<"��֤��ʼ"<<std::endl;
fbnqhp.asertFibonacciHeap();
std::cout<<"��֤����"<<std::endl;

std::cout<<fbnqhp.size()<<"end"<<fbnqhp1.size()<<std::endl;

std::cout<<"�ͷ���Դ1"<<std::endl;
fbnqhp.setEmpty();
std::cout<<"�ͷ���Դ2"<<std::endl;
std::cout<<std::endl;//*/

///*���ཻ���ϲ���
std::cout<<"���ཻ���ϲ��ԣ�"<<std::endl;
std::vector<lmtc::NonIntersectSet<int>> setVec;
for(int i=0;i<10;i++){
	setVec.push_back(lmtc::NonIntersectSet<int>());
	setVec[i].setValue(i);
	if(setVec[i].getValue()!=i)
		std::cout<<"error1"<<std::endl;
}
for(int i=0;i<10;i++){
	unsigned int a=(unsigned int)(lmtc::averageRandom()*10);
	unsigned int b=(unsigned int)(lmtc::averageRandom()*10);
	if(setVec[a].find()==setVec[b].find())
		std::cout<<a<<"===="<<b<<std::endl;
	setVec[a].unionSet(setVec[b]);
}
for(int i=0;i<10;i++){
	std::cout<<i<<"->"<<setVec[i].find()->getValue()<<std::endl;
	setVec[i].find()->setValue(setVec[i].find()->getValue());
}
std::cout<<std::endl;//*/

int arr[][5]={{0,1,0,0,0},{0,0,0,0,0},{1,1,0,1,1},{1,0,0,0,0},{1,0,0,1,0}};
lmtc::Array<int> ajacencyMatrix(2,5,5);
for(int i=0;i<5;i++)
	for(int j=0;j<5;j++)
		ajacencyMatrix(i,j)=arr[i][j];

//�ڽӾ���ת�ڽӱ�
std::vector<std::list<lmtc::Edge<int>>> ajacencyList;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix,ajacencyList);

//�ڽӱ��ת��
std::vector<std::list<lmtc::Edge<int>>> transposedAjacencyList;
lmtc::Graph::transposeAjacencyList(ajacencyList,transposedAjacencyList);


///*�����������
std::cout<<"����������ԣ�"<<std::endl;
std::vector<unsigned int> order=lmtc::Graph::topologicalSort(transposedAjacencyList);
std::cout<<"��������Ϊ��"<<std::endl;
for(std::vector<unsigned int>::iterator iter=order.begin();iter!=order.end();iter++)
	std::cout<<*iter<<"  ";
std::cout<<std::endl;//*/

///*ǿ��ͨ��֧����
std::cout<<"ǿ��ͨ��֧���ԣ�"<<std::endl;
std::vector<std::vector<unsigned int>> strongConnectComponents;
lmtc::Graph::computeStrngConctComps(ajacencyList,strongConnectComponents);
for(unsigned int i=0;i<strongConnectComponents.size();i++){
	std::cout<<"��ͨ��֧"<<i<<std::endl;
	for(std::vector<unsigned int>::iterator iter=strongConnectComponents[i].begin();iter!=strongConnectComponents[i].end();iter++)
		std::cout<<*iter<<" ; ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*ŷ����·����
std::cout<<"ŷ����·���ԣ�"<<std::endl;
int arr1[][5]={{1,1,1,0,0},{0,1,1,1,0},{0,0,0,1,1},{1,0,0,0,1},{1,1,0,0,0}};
//int arr1[][5]={{2,0,1,1,0},{0,2,0,1,1},{1,0,0,0,1},{1,1,0,0,0},{0,1,1,0,0}};
lmtc::Array<int> ajacencyMatrix1(2,5,5);
for(int i=0;i<5;i++)
	for(int j=0;j<5;j++)
		ajacencyMatrix1(i,j)=arr1[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList1;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix1,ajacencyList1);

std::list<lmtc::Edge<int>> EulerCircuit;
bool existEuler=lmtc::Graph::computeEulerCircuit(ajacencyList1,EulerCircuit,true);
if(existEuler==false&&EulerCircuit.empty())
	std::cout<<"Euler circuit or path not existed"<<std::endl;
else{
	std::cout<<"ŷ����·Ϊ��"<<std::endl;
	for(std::list<lmtc::Edge<int>>::iterator iter=EulerCircuit.begin();iter!=EulerCircuit.end();iter++)
		std::cout<<iter->vSt<<"-"<<iter->vEd<<"   -->   ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*��С����������
std::cout<<"��С���������ԣ�"<<std::endl;
int arr2[][8]={{1,1,2,7,3,0,0,0},{1,1,6,2,0,0,0,0},{2,6,1,4,0,0,0,0},{7,2,4,1,5,0,0,0},{3,0,0,5,1,0,0,0},{0,0,0,0,0,1,2,3},{0,0,0,0,0,2,1,1},{0,0,0,0,0,3,1,1}};
lmtc::Array<int> ajacencyMatrix2(2,8,8);
for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
		ajacencyMatrix2(i,j)=arr2[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList2;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix2,ajacencyList2);
std::vector<std::list<lmtc::Edge<int>>> mstAjacencyList;
int weightKruskal=lmtc::Graph::mstKruskal(ajacencyList2,mstAjacencyList);
std::cout<<"Kruskal��С������Ϊ��Ȩ�أ�"<<weightKruskal<<std::endl;
for(unsigned int i=0;i<mstAjacencyList.size();i++){
	for(std::list<lmtc::Edge<int>>::const_iterator iter=mstAjacencyList[i].begin();iter!=mstAjacencyList[i].end();iter++){
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
	}
	std::cout<<std::endl;
}
std::vector<std::list<lmtc::Edge<int>>> mstAjacencyListPrim;
int weightPrim=lmtc::Graph::mstPrim(ajacencyList2,mstAjacencyListPrim);
std::cout<<"Prim��С������Ϊ��Ȩ�أ�"<<weightPrim<<std::endl;
for(unsigned int i=0;i<mstAjacencyListPrim.size();i++){
	for(std::list<lmtc::Edge<int>>::const_iterator iter=mstAjacencyListPrim[i].begin();iter!=mstAjacencyListPrim[i].end();iter++){
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
	}
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*�ж�����ͼ��������ͼ�Ƿ���ڻ�·
std::cout<<"�ж�����ͼ��������ͼ�Ƿ���ڻ�·���ԣ�"<<std::endl;
int arr3[][4]={{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
lmtc::Array<int> ajacencyMatrix3(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		ajacencyMatrix3(i,j)=arr3[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList3;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix3,ajacencyList3);
if(lmtc::Graph::hasLoop(ajacencyList3,true)==true)
	std::cout<<"���ڻ�·"<<std::endl;
else
	std::cout<<"�����ڻ�·"<<std::endl;
std::cout<<std::endl;//*/

///*�������·��
std::cout<<"���·�����ԣ�"<<std::endl;
int arr4[][4]={{0,1,3,4},{0,3,1,2},{2,1,6,0},{6,3,0,1}};
lmtc::Array<int> ajacencyMatrix4(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		ajacencyMatrix4(i,j)=arr4[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList4;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix4,ajacencyList4);
unsigned s=0;
std::vector<unsigned int> p;
std::vector<int>d;
//if(true==lmtc::Graph::shortestPathBellmanFord(ajacencyList4,s,p,d))shortestPathDijkstra
//if(true==lmtc::Graph::shortestPathOfDag(ajacencyList4,s,p,d))
if(true==lmtc::Graph::shortestPathDijkstra(ajacencyList4,s,p,d))
	for(unsigned int i=0;i<p.size();i++)
		std::cout<<"parent of "<<i<<" is "<<p[i]<<" , and d of the path is "<<d[i]<<std::endl;
else
	std::cout<<"���ڸ���"<<std::endl;
std::cout<<std::endl;//*/

///*����ÿ�Զ��������·��
std::cout<<"ÿ�Զ��������·�����ԣ�"<<std::endl;
int arr5[][4]={{1,1,3,4},{0,1,1,2},{-2,-1,1,0},{6,3,0,1}};
lmtc::Array<int> ajacencyMatrix5(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		ajacencyMatrix5(i,j)=arr5[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList5;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix5,ajacencyList5);
lmtc::Array<unsigned int> pMatrix;
lmtc::Array<int>dMatrix;
//lmtc::Graph::shortestPathAllFloydWarshall(ajacencyList5,pMatrix,dMatrix);
if(lmtc::Graph::shortestPathAllJohnson(ajacencyList5,pMatrix,dMatrix))
for(unsigned int k=0;k<pMatrix.getDimLen(0);k++){
	std::cout<<"��k="<<k<<"ΪԴ������·����"<<std::endl;
	for(unsigned int i=0;i<pMatrix.getDimLen(1);i++)
		std::cout<<"parent of "<<i<<" is "<<pMatrix(k,i)<<" , and d of the path is "<<dMatrix(k,i)<<std::endl;
}
std::cout<<std::endl;//*/

///*���������
std::cout<<"��������ԣ�"<<std::endl;
int arr6[][6]={{-6,16,13,0,0,0},{0,0,10,12,0,0},{-6,4,0,0,14,0},{0,0,9,0,0,20},{0,0,0,7,0,4},{-4,0,0,-4,0,-4}};
lmtc::Array<int> ajacencyMatrix6(2,6,6);
for(int i=0;i<6;i++)
	for(int j=0;j<6;j++)
		ajacencyMatrix6(i,j)=arr6[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList6;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix6,ajacencyList6);
std::vector<std::list<lmtc::Edge<int>>> flow;
//int maxF=lmtc::Graph::maximumFlowFordFulkerson_EdmondsKarp(ajacencyList6,0,5,flow);
int maxF=lmtc::Graph::maximumFlowPushRelabelToFront(ajacencyList6,0,5,flow);
std::cout<<"�����="<<maxF<<std::endl;
for(unsigned int i=0;i<flow.size();i++)
	for(std::list<lmtc::Edge<int>>::const_iterator iter=flow[i].begin();iter!=flow[i].end();iter++)
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
std::cout<<std::endl;
std::cout<<std::endl;//*/


/*��������*/

///*����˷�����
std::cout<<"����˷����ԣ�"<<std::endl;
int arr7[][3]={{0,1,2},{3,1,5},{4,2,6},{1,8,3}};
lmtc::Array<int> Matrix1(2,4,3);
for(int i=0;i<4;i++)
	for(int j=0;j<3;j++)
		Matrix1(i,j)=arr7[i][j];

int arr8[][5]={{4,8,2,1,3},{3,2,6,4,7},{2,5,6,3,5}};
lmtc::Array<int> Matrix2(2,3,5);
for(int i=0;i<3;i++)
	for(int j=0;j<5;j++)
		Matrix2(i,j)=arr8[i][j];

//lmtc::Array<int> matrixRs=lmtc::Algebra::matrixMultiplySimple(Matrix1,Matrix2);
lmtc::Array<int> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(Matrix1,Matrix2);
std::cout<<"����˷����"<<std::endl;
for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
	for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
		std::cout<<matrixRs(i,j)<<"   ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*LUP �ֽ����
std::cout<<"LUP �ֽ���ԣ�"<<std::endl;
double arr9[][4]={{2,0,2,0.6},{3,3,4,-2},{5,5,4,2},{-1,-2,3.4,-1}};
lmtc::Array<double> A(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		A(i,j)=arr9[i][j];
lmtc::Array<double> L,U;
std::vector<unsigned int> P;
bool b=lmtc::Algebra::lupDecompose(A,L,U,P);
std::cout<<"LUP�ֽ���"<<std::endl;
if(b==true){
	std::cout<<"L:"<<std::endl;
	for(unsigned int i=0;i<L.getDimLen(0);i++){
		for(unsigned int j=0;j<L.getDimLen(1);j++)
			std::cout<<L(i,j)<<"   ";
		std::cout<<std::endl;
	}
	std::cout<<"U:"<<std::endl;
	for(unsigned int i=0;i<U.getDimLen(0);i++){
		for(unsigned int j=0;j<U.getDimLen(1);j++)
			std::cout<<U(i,j)<<"   ";
		std::cout<<std::endl;
	}
	std::cout<<"P:"<<std::endl;
	for(unsigned int i=0;i<P.size();i++)
		std::cout<<P[i]<<"   ";
	std::cout<<std::endl;
}
else
	std::cout<<"LUP�ֽ�ʧ�ܣ����췽��"<<std::endl;
std::cout<<std::endl;//*/

///*�����Է�����
std::cout<<"�����Է�������ԣ�"<<std::endl;
double arr10[][3]={{1,2,0},{3,4,4},{5,6,3}};
std::vector<double> b1;
b1.push_back(3);
b1.push_back(7);
b1.push_back(8);
lmtc::Array<double> A1(2,3,3);
for(int i=0;i<3;i++)
	for(int j=0;j<3;j++)
		A1(i,j)=arr10[i][j];
std::vector<double> X1;
if(lmtc::Algebra::solveLinearEquationsFast(A1,b1,X1))
//if(lmtc::Algebra::solveLinearEquationsByLUP(A1,b1,X1))
	for(unsigned int i=0;i<X1.size();i++)
		std::cout<<X1[i]<<"   ";
else
	std::cout<<"�޽�";
std::cout<<std::endl;//*/

///*����������
std::cout<<"����������ԣ�"<<std::endl;
double arr11[][3]={{1,2,0},{2,4,5},{5,6,3}};
lmtc::Array<double> A2(2,3,3);
for(int i=0;i<3;i++)
	for(int j=0;j<3;j++)
		A2(i,j)=arr11[i][j];
lmtc::Array<double> _A;
std::cout<<"��������"<<std::endl;
if(lmtc::Algebra::inverseMatrixFast(A2,_A)){
//if(lmtc::Algebra::inverseMatrixByLUP(A2,_A)){
	for(unsigned int i=0;i<_A.getDimLen(0);i++){
		for(unsigned int j=0;j<_A.getDimLen(1);j++)
			std::cout<<_A(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(A2,_A);
	std::cout<<"���������˵ĵ�λ����"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"������"<<std::endl;
std::cout<<std::endl;//*/

///*α��������
std::cout<<"α�������ԣ�"<<std::endl;
double arr12[][3]={{1,-1,1},{1,1,1},{1,2,4},{1,3,9},{1,5,25}};
lmtc::Array<double> A3(2,5,3);
for(int i=0;i<5;i++)
	for(int j=0;j<3;j++)
		A3(i,j)=arr12[i][j];
lmtc::Array<double> A3T=lmtc::Algebra::transposeMatrix(A3);
lmtc::Array<double> _Af;
lmtc::Array<double> _Ab;
std::cout<<"��ǰ��α�����"<<std::endl;
if(lmtc::Algebra::pseudoInverseMatrixForward(A3,_Af)){
	for(unsigned int i=0;i<_Af.getDimLen(0);i++){
		for(unsigned int j=0;j<_Af.getDimLen(1);j++)
			std::cout<<_Af(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(_Af,A3);
	std::cout<<"��ǰ��α�������˵ĵ�λ����"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"����ǰ��α��"<<std::endl;

std::cout<<"�����α�����"<<std::endl;
if(lmtc::Algebra::pseudoInverseMatrixBackward(A3T,_Ab)){
	for(unsigned int i=0;i<_Ab.getDimLen(0);i++){
		for(unsigned int j=0;j<_Ab.getDimLen(1);j++)
			std::cout<<_Ab(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(A3T,_Ab);
	std::cout<<"�����α�������˵ĵ�λ����"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"���ɺ���α��"<<std::endl;
std::cout<<std::endl;//*/

///*���ݺͲ���
std::cout<<"���ݺͲ��ԣ�"<<std::endl;
const unsigned int K=6;
const unsigned int N=100;
lmtc::Array<double> S=lmtc::Algebra::powerSumFormula(K);
std::cout<<"���ݺ͹�ʽ����"<<std::endl;
for(unsigned int i=0;i<=K;i++){
	std::cout<<"��Ϊ"<<i<<"ʱ�Ĺ�ʽ��";
	for(unsigned int j=0;j<i+2;j++)
		std::cout<<"c"<<j<<" = "<<S(i,j)<<", ";
	std::cout<<std::endl;
}
std::cout<<"when K="<<K<<" ,N="<<N<<"  ʱ��0^K+1^K+2^K+...+N^K = "<<lmtc::Algebra::powerSum(N,K)<<std::endl;
std::cout<<std::endl;//*/

///*�����������
std::cout<<"����������ԣ�"<<std::endl;
lmtc::ComplexNumber c1(0.34,0.69);
lmtc::ComplexNumber c2(0.53,0.49);
c1+=c2;
std::cout<<(std::string)c1<<std::endl;
c1-=c2;
std::cout<<(std::string)c1<<std::endl;
c1*=c2;
std::cout<<(std::string)c1<<std::endl;
c1/=c2;
std::cout<<(std::string)c1<<std::endl;
std::cout<<std::endl;//*/

///*���ٸ���Ҷ�任,����任����ֵ�ȶ��Էǳ��ã���cN=1000ʱΪ0.00000000001����cN=10000ʱΪ0.0000000001����cN=100000ʱΪ0.000000001����cN=1000000ʱΪ0.00000001��
std::cout<<"���ٸ���Ҷ�任���ԣ�"<<std::endl;
std::vector<lmtc::ComplexNumber> a;
unsigned int cN=100;
for(unsigned i=0;i<cN;i++)
	a.push_back(lmtc::ComplexNumber(lmtc::averageRandom(0,10),lmtc::averageRandom(0,10)));
std::vector<lmtc::ComplexNumber> y=lmtc::FastFourierTransform::fft(a);
std::vector<lmtc::ComplexNumber> a1=lmtc::FastFourierTransform::_fft(y);
std::cout<<"ϵ������a��";
for(unsigned i=0;i<a.size();i++)
	std::cout<<(std::string)a[i]<<"   ";
std::cout<<std::endl;
std::cout<<"��ֵ����y��";
for(unsigned i=0;i<y.size();i++)
	std::cout<<(std::string)y[i]<<"   ";
std::cout<<std::endl;
std::cout<<"ϵ������a1��";
for(unsigned i=0;i<a1.size();i++)
	std::cout<<(std::string)a1[i]<<"   ";
std::cout<<std::endl;

for(unsigned int i=0;i<a1.size();i++){
	if(i<a.size()&&!a[i].equal(a1[i],0.00001))
		std::cout<<"error:"<<(std::string)a[i]<<"==?"<<(std::string)a1[i]<<"   "<<std::endl;
	if(i>=a.size()&&!a1[i].equal(0,0.01))
		std::cout<<"error1"<<std::endl;
}
std::cout<<std::endl;//*/

///*����
std::cout<<"������ԣ�"<<std::endl;
double Aarr[]={1,2,3,23};
double Barr[]={5,4,34};
std::vector<lmtc::ComplexNumber> Avec(Aarr,Aarr+4);
std::vector<lmtc::ComplexNumber> Bvec(Barr,Barr+3);
std::vector<lmtc::ComplexNumber>Cvec=lmtc::Algebra::convolution(Avec,Bvec);
std::cout<<"���Ϊ��"<<std::endl;
for(unsigned int i=0;i<Cvec.size();i++)
	std::cout<<(std::string)Cvec[i]<<"   ";
std::cout<<std::endl;//*/

///*��ά����Ҷ�任����
std::cout<<"��ά����Ҷ�任���ԣ�"<<std::endl;
lmtc::Array<lmtc::ComplexNumber> aMultiFFT(2,3,4);
for(unsigned int i=0;i<aMultiFFT.getDimLen(0);i++)
	for(unsigned int j=0;j<aMultiFFT.getDimLen(1);j++)
		aMultiFFT(i,j)=lmtc::ComplexNumber(lmtc::averageRandom(0,10),lmtc::averageRandom(0,10));
lmtc::Array<lmtc::ComplexNumber> yMultiFFT=lmtc::FastFourierTransform::fft(aMultiFFT);
lmtc::Array<lmtc::ComplexNumber> a1MultiFFT=lmtc::FastFourierTransform::_fft(yMultiFFT);
std::cout<<"��ά����Ҷ�任"<<std::endl;
std::cout<<"aMultiFFT:"<<std::endl;
for(unsigned int i=0;i<aMultiFFT.getDimLen(0);i++){
	for(unsigned int j=0;j<aMultiFFT.getDimLen(1);j++)
		std::cout<<(std::string)aMultiFFT(i,j)<<", ";
	std::cout<<std::endl;
}
std::cout<<"yMultiFFT:"<<std::endl;
for(unsigned int i=0;i<yMultiFFT.getDimLen(0);i++){
	for(unsigned int j=0;j<yMultiFFT.getDimLen(1);j++)
		std::cout<<(std::string)yMultiFFT(i,j)<<", ";
	std::cout<<std::endl;
}
std::cout<<"a1MultiFFT:"<<std::endl;
for(unsigned int i=0;i<a1MultiFFT.getDimLen(0);i++){
	for(unsigned int j=0;j<a1MultiFFT.getDimLen(1);j++)
		std::cout<<(std::string)a1MultiFFT(i,j)<<", ";
	std::cout<<std::endl;
}
for(unsigned int i=0;i<a1MultiFFT.getDimLen(0);i++)
	for(unsigned int j=0;j<a1MultiFFT.getDimLen(1);j++){
		if(i<aMultiFFT.getDimLen(0)&&j<aMultiFFT.getDimLen(1)){
			if(!aMultiFFT(i,j).equal(a1MultiFFT(i,j),0.00001))
				std::cout<<"error:"<<(std::string)a1MultiFFT(i,j)<<"==?"<<(std::string)aMultiFFT(i,j)<<"   "<<std::endl;
		}else{
			if(!a1MultiFFT(i,j).equal(0,0.00001))
				std::cout<<"error1:"<<(std::string)a1MultiFFT(i,j)<<"==?"<<0<<"   "<<std::endl;
		}
	}
std::cout<<std::endl;//*/

///*��ά�������
std::cout<<"��ά������ԣ�"<<std::endl;
lmtc::Array<lmtc::ComplexNumber> mutiAvec(2,3,2);
lmtc::Array<lmtc::ComplexNumber> mutiBvec(3,2,3,3);
mutiAvec(1,1)=1;
mutiAvec(2,1)=2;
mutiAvec(0,1)=4;
mutiBvec(1,1,1)=3;
mutiBvec(1,2,0)=6;
mutiBvec(0,1,2)=5;
lmtc::Array<lmtc::ComplexNumber>mutiCvec=lmtc::Algebra::convolution(mutiAvec,mutiBvec);
std::cout<<"��ά���Ϊ��"<<std::endl;
std::vector<unsigned int> dimC(mutiCvec.getDimNum(),0);
while(dimC[mutiCvec.getDimNum()-1]<mutiCvec.getDimLen(mutiCvec.getDimNum()-1)){//��a��Ԫ�ظ��Ƶ�����������a1
	if(!mutiCvec(dimC).equal(0,0.001)){
		std::cout<<"mutiCvec(";
		for(unsigned int i=0;i<dimC.size();i++)
			std::cout<<dimC[i]<<"  ";
		std::cout<<")   =   "<<(std::string)mutiCvec(dimC)<<std::endl;
	}
	for(unsigned int i=0;i<mutiCvec.getDimNum();i++){//�����ά�����������һ������
		if(dimC[i]!=(mutiCvec.getDimLen(i)-1)){
			dimC[i]++;
			break;
		}else if(i!=(mutiCvec.getDimNum()-1))
			dimC[i]=0;
		else
			dimC[i]++;
	}
}
std::cout<<std::endl;//*/

///*��01�����������
std::cout<<"��01����������ԣ�"<<std::endl;
unsigned int pac1[]={8,8,8,8,10,10,10,10,10,10,10,10,10,10,12,12,12,12,12,12};
unsigned int pac[]={10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
unsigned int weight1[]={1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,8,8,9};   
double value[]={2,3,5,2,6,2,5,5,6,10,7,5,3,2,4,4,20,2,8,5,6,6,10,19,6,5,4,7,5,3,5,8,6,7,7,5,5,12,15,8,5,9,23,12,10,14,11,8,10,18};
double value1[]={1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,8,8,9};	   
std::vector<unsigned int>packVec(pac1,pac1+7);
std::vector<unsigned int>weightVec(weight1,weight1+30);
std::vector<double> valueVec(value,value+30);
std::vector<int> packAssigned;
double maxValue=lmtc::OperationsResearch::multi_01_package(packVec,weightVec,valueVec,packAssigned);
std::cout<<"�ö�01�����������װ��Ʒ������ֵΪ:"<<maxValue<<std::endl;
std::cout<<"��Ʒ�����·�ʽװ��(i->j��ʾi��Ʒ����j������j=0ʱ��ʾi��װ��)��"<<std::endl;
for(unsigned int i=0;i<weightVec.size();i++)
	std::cout<<i<<"->"<<packAssigned[i]<<" ; ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

std::cout<<"all test over!!!"<<std::endl;
	return 0;//��ʾ�ɹ�ִ��
}
