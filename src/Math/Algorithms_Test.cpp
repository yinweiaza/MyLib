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

// Algorithm_Lmtc.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"

//C++类库
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <bitset>
#include <exception>
#include <memory>  //含有auto_ptr类，用于安全的管理动态申请的对象（不能管理动态数组，不能用于容器）
#include <algorithm> //标准库算法
#include <numeric> //算术型算法
#include <functional> //函数对象
#include <fstream>

//C函数库
#include <cctype>
#include <cstddef>
#include <cstring>
#include <ctime>


//自定义头文件
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

//用于红黑树测试的函数指针
void f(double x){std::cout<<x<<std::endl;}
//用于散列测试的函数指针
unsigned long keyToNumber(const unsigned long &t){return t;}
//用于最小二乘估计测试的函数指针。
double polynomial(unsigned int i,double x){
	return std::pow(x,(int)i);
}
//用于多元最小二乘估计测试的函数指针。
double multiPolynomial(const std::vector<unsigned int> &iVec,const std::vector<double> &xVec){
	double temp1=1;
	for(unsigned int k=0;k<xVec.size();k++)
		temp1*=std::pow(xVec[k],(int)iVec[k]);
	return temp1;
}
//用于拟阵测试的函数指针
bool canExpandTo(const int &item,const std::vector<int> &st){return true;}
void expandTo(const int &item,std::vector<int> &st){st.push_back(item);}

int _tmain(int argc, _TCHAR* argv[])
{
	srand((long)time(NULL));

///*多项式除法
std::cout<<"多项式除法测试："<<std::endl;
double pdAarr[]={9,3,4,8,49,7,22,7,8,47};
double pdBarr[]={4,3,21,15};
std::vector<lmtc::ComplexNumber> pdAvec(pdAarr,pdAarr+10);
std::vector<lmtc::ComplexNumber> pdBvec(pdBarr,pdBarr+4);
std::vector<lmtc::ComplexNumber>pdCvec=lmtc::Algebra::polynomialDivisionByFFT(pdAvec,pdBvec);
std::cout<<"多项式商为（by FFT,只适合整除）："<<std::endl;
for(unsigned int i=0;i<pdCvec.size();i++)
	std::cout<<(std::string)pdCvec[i]<<"   ";
std::cout<<std::endl;//*/
std::vector<lmtc::ComplexNumber>pdA1vec=lmtc::Algebra::convolution(pdBvec,pdCvec);
std::cout<<"pdA1vec=pdBvec*pdCvec为："<<std::endl;
for(unsigned int i=0;i<pdA1vec.size();i++)
	std::cout<<(std::string)pdA1vec[i]<<"   ";
std::cout<<std::endl;

std::vector<lmtc::ComplexNumber>pdCvecDv,pdCvecLf;
pdCvecDv=lmtc::Algebra::polynomialDivisionGeneral(pdAvec,pdBvec,pdCvecLf);
std::cout<<"多项式商为（朴素方法）："<<std::endl;
for(unsigned int i=0;i<pdCvecDv.size();i++)
	std::cout<<(std::string)pdCvecDv[i]<<"   ";
std::cout<<std::endl;//*/
std::cout<<"多项式余数为（朴素方法）："<<std::endl;
for(unsigned int i=0;i<pdCvecLf.size();i++)
	std::cout<<(std::string)pdCvecLf[i]<<"   ";
std::cout<<std::endl;
std::vector<lmtc::ComplexNumber>pdA2vec=lmtc::Algebra::convolution(pdBvec,pdCvecDv);
std::cout<<"pdA2vec=pdBvec*pdCvecDv为："<<std::endl;
for(unsigned int i=0;i<pdA2vec.size();i++)
	std::cout<<(std::string)pdA2vec[i]<<"   ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

///*最小二乘估计与解正态方程组测试
std::cout<<"解正态方程组测试："<<std::endl;
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
	std::cout<<"正态方程组的解为：";
	for(unsigned int i=0;i<C.size();i++)
		std::cout<<C[i]<<"   ";
	std::cout<<std::endl;
}
else
	std::cout<<"正态方程组不可解"<<std::endl;
std::cout<<"最小二乘估计测试："<<std::endl;
//if(lmtc::Algebra::leastSquaresEstimationByLUP(polynomial,3,X,Y,C1)){
if(lmtc::Algebra::leastSquaresEstimationFast(polynomial,3,X,Y,C1)){
	std::cout<<"最小二乘估计的解为：";
	for(unsigned int i=0;i<C1.size();i++)
		std::cout<<C1[i]<<"   ";
	std::cout<<std::endl;
}
else
	std::cout<<"最小二乘不可解"<<std::endl;
std::cout<<std::endl;//*/

///*多元最小二乘估计,此处待估计函数为二元泰勒级数。三元以上的任何类型多元函数的最小二乘估计与此类似。
std::cout<<"多元最小二乘估计测试："<<std::endl;
unsigned int pmVarNum=2;//自变量数目
unsigned int pmDataRowNum=50;//产生样本数目
unsigned int nVecPMArr[]={2,2};//各变量的最高次数
std::vector<unsigned int> nVecPM(nVecPMArr,nVecPMArr+2);
lmtc::Array<double> thalerCoaf(2,3,3);//产生样本的泰勒系数
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
for(unsigned int i=0;i<pmDataRowNum;i++){//产生多元泰勒级数的若干样本点。
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
	//加入适量的噪声
	YPM1[i]+=lmtc::averageRandom(-2,2);
}
lmtc::Array<double> CPM1;//最小二乘估计得到的系数存入该数组。
if(true==lmtc::Algebra::leastSquaresEstimationByLUP(multiPolynomial,nVecPM,XPM1,YPM1,CPM1))
	for(unsigned int i=0;i<CPM1.getDimLen(0);i++){
		for(unsigned int j=0;j<CPM1.getDimLen(1);j++)//输出多元最小二乘估计的结果
			std::cout<<"CPM1("<<i<<","<<j<<") = "<<CPM1(i,j)<<"   ";
		std::cout<<std::endl;
	}
std::cout<<std::endl;
std::cout<<std::endl;//*/


///*KMP序列匹配算法测试
std::cout<<"KMP序列匹配测试"<<std::endl;
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


///*大数测试
std::cout<<"大数测试："<<std::endl;
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
std::cout<<"由二进制转成的大数为："<<(std::string)bgDc2<<std::endl;
std::vector<bool> bstVec0;
unsigned int biDt0=bgDc2.getBinary(bstVec0);
std::cout<<"有大数反向转换成的二进制数为："<<std::endl;
for(int i=(int)bstVec0.size()-1;i>=0;i--){
	std::cout<<bstVec0[i];
	if(i==biDt0&&i!=0)
		std::cout<<".";
}
std::cout<<std::endl;
std::vector<bool> bstVec1;
lmtc::BigDecimal bgDc3(std::string("122598.75"));
unsigned int biDt=bgDc3.getBinary(bstVec1);
std::cout<<"由浮点数转换成的二进制小数为："<<std::endl;
for(int i=(int)bstVec1.size()-1;i>=0;i--){
	std::cout<<bstVec1[i];
	if(i==biDt&&i!=0)
		std::cout<<".";
}
std::cout<<std::endl;
std::cout<<bgDc3[7]<<std::endl<<std::endl;//*/


///*最大公约数、最小公倍数测试
std::cout<<"最大公约数测试："<<std::endl;
unsigned long aGcd=5*7*23*37*53,bGcd=3*11*23*37*179,dGcd;
long xGcd,yGcd;
lmtc::NumberTheory::gcd(aGcd,bGcd,dGcd,xGcd,yGcd);
if(dGcd==(aGcd*xGcd+bGcd*yGcd))
	std::cout<<"gcd("<<aGcd<<","<<bGcd<<")="<<dGcd<<"="<<aGcd<<"*"<<xGcd<<"+"<<bGcd<<"*"<<yGcd<<std::endl;
else
	std::cout<<"算法错误!"<<std::endl;
std::cout<<"最小公倍数为："<<lmtc::NumberTheory::lcm(23,37)<<std::endl;
std::cout<<"大数版本的最大公约数测试："<<std::endl;
lmtc::BigDecimal aGcd1=5*7*23*37*53,bGcd1=3*11*23*37*179,dGcd1,xGcd1,yGcd1;
lmtc::NumberTheory::gcd(aGcd1,bGcd1,dGcd1,xGcd1,yGcd1);
if(dGcd1==(aGcd1*xGcd1+bGcd1*yGcd1))
std::cout<<"gcd("<<(std::string)aGcd1<<","<<(std::string)bGcd1<<")="<<(std::string)dGcd1<<"="<<(std::string)aGcd1<<"*"<<(std::string)xGcd1<<"+"<<(std::string)bGcd1<<"*"<<(std::string)yGcd1<<std::endl;
else
	std::cout<<"算法错误!"<<std::endl;
std::cout<<std::endl;//*/

///*解同余方程测试
std::cout<<"解同余方程测试："<<std::endl;
unsigned long mdleqa=14,mdleqn=100;
long mdleqb=30;
std::vector<unsigned long> mdleqX=lmtc::NumberTheory::modularLinearEquation(mdleqa,mdleqb,mdleqn);
std::cout<<"同余方程的所有解为：";
for(unsigned int i=0;i<mdleqX.size();i++)
	std::cout<<"x"<<i<<"="<<mdleqX[i]<<"    ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

///*孙子定理解同余方程组测试
std::cout<<"孙子定理解同余方程组测试："<<std::endl;
std::vector<unsigned long> szN;
std::vector<long> szA;
szN.push_back(7);
szN.push_back(19);
szN.push_back(23);
szA.push_back(-16);
szA.push_back(35);
szA.push_back(12);
double szX=lmtc::NumberTheory::modularLinearEquationsBySunTzu(szA,szN);
std::cout<<"同余方程组的解为："<<szX<<std::endl;
std::cout<<std::endl;//*/


///*模取幂运算
std::cout<<"模取幂运算测试："<<std::endl;
lmtc::BigDecimal moExpA=8,moExpB=10,moExpN=1000000000;
lmtc::BigDecimal moExpRs=lmtc::NumberTheory::modularExponentiation(moExpA,moExpB,moExpN);
std::cout<<(std::string)moExpA<<"^"<<(std::string)moExpB<<" % "<<(std::string)moExpN<<" = "<<(std::string)moExpRs<<std::endl;
std::cout<<std::endl;//*/

///*随机大数测试
std::cout<<"随机大数测试"<<std::endl;
lmtc::BigDecimal randBigInt=lmtc::averageRandomBigInteger(32);
std::cout<<"随机大数为："<<(std::string)randBigInt<<std::endl;
lmtc::BigDecimal randBigFloat=lmtc::averageRandomBigFloat(32);
std::cout<<"随机小数为："<<(std::string)randBigFloat<<std::endl;
std::cout<<std::endl;//*/

///*MillerRabin素数测试
std::cout<<"MillerRabin素数测试"<<std::endl;
if(lmtc::NumberTheory::MillerRabin(lmtc::MAX_PRIME))
	std::cout<<"为素数！"<<std::endl;
else 
	std::cout<<"不为素数！"<<std::endl;
std::cout<<std::endl;//*/

/*//产生大质数测试(产生的50位的大质数为：57833101454301616138860708250340300807978842653707)
std::cout<<"产生大质数测试"<<std::endl;
std::cout<<"产生的大质数为："<<(std::string)lmtc::NumberTheory::generatePrimeNumberByMillerRabin(50)<<std::endl;
std::cout<<std::endl;//*/

///*RSA综合测试
std::cout<<"RSA综合测试"<<std::endl;
///*如下为能加密50位十进制大数的公钥(RSAE,RSAN)和密钥(RSAD,RSAN)
lmtc::BigDecimal RSAN("2521856375150261634008882739223972423156297845443147"),
                 RSAE("86244010814085248452881469"),
                 RSAD("2440156170950764584581132081694305962063303775164837");
//*/
/*//如下为能加密200位十进制大数的公钥(RSAE,RSAN)和密钥(RSAD,RSAN)
lmtc::BigDecimal RSAN("6380616938818171200074355477705576014995641232691298837296561137740893181836377916290884463497517597095768366165545899640091860658179992054086438007286317457020617942159137676906625739461654367522339777"),
                 RSAE("20982538709357952226499259416280815673153042323347356977376673248400375494362252349061307887319768317"),
                 RSAD("5907082193902464506985888020290474680163560377060753951361684708389611677387682422762695048148474090312313036781635853755482565201060158527257044488185981709234453211197945547224621783304143918340505813");
//*/
/*//产生公钥、密钥的代码。产生上面200位的公钥、密钥花了2个小时，因此产生一次就应该保存起来。
unsigned int RSADgtNum=200;//加密信息对应十进制大数的最大位数。产生的公钥、私钥只适合满足该限制的信息，否则报错。
if(true==lmtc::NumberTheory::generateKeyOfRSA(RSADgtNum,RSAN,RSAE,RSAD)){
	std::cout<<"公钥为："<<(std::string)RSAE<<"  &  "<<(std::string)RSAN<<std::endl;
	std::cout<<"私钥为："<<(std::string)RSAD<<"  &  "<<(std::string)RSAN<<std::endl;
}else
	std::cout<<"产生公钥私钥过程出错"<<std::endl;
//*/
///*
lmtc::BigDecimal RSAMSGDec="123456789987654321123456789987654321";//待加密大数
std::cout<<"待加密大数为："<<(std::string)RSAMSGDec<<std::endl;
lmtc::BigDecimal enCryptDec=lmtc::NumberTheory::cryptRSA(RSAMSGDec,RSAE,RSAN);//加密
std::cout<<"加密结果大数为："<<(std::string)enCryptDec<<std::endl;
lmtc::BigDecimal deCryptDec=lmtc::NumberTheory::cryptRSA(enCryptDec,RSAD,RSAN);//解密
std::cout<<"解密为原大数为："<<(std::string)deCryptDec<<std::endl;

//std::string RSAMSG="算法是数学的一个分支、是富于创造性的一门科学、是汇集灵感与理性的思维艺术！";//待加密字符串
std::string RSAMSG="算法是一门艺术！";//待加密字符串
std::cout<<"待加密字符串为："<<RSAMSG<<std::endl;
std::string enCryptStr=lmtc::NumberTheory::cryptRSA(RSAMSG,RSAE,RSAN);//加密
std::cout<<"加密结果字符串为："<<enCryptStr<<std::endl;
std::string deCryptStr=lmtc::NumberTheory::cryptRSA(enCryptStr,RSAD,RSAN);//解密
std::cout<<"解密为原字符串为："<<deCryptStr<<std::endl;
std::cout<<std::endl;//*/

///*有向线段测试
std::cout<<"有向线段测试"<<std::endl;
lmtc::Segment seg1(2,1,5,3);
lmtc::Segment seg2(3,5,5,1);
if(seg1.intersect(seg2))
	std::cout<<"seg1与seg2相交"<<std::endl;
else
	std::cout<<"不相交"<<std::endl;

int clkDirect=seg1.clockDirect(seg2);
if(clkDirect>0)
	std::cout<<"seg1在seg2的顺时针方向"<<std::endl;
else if(clkDirect<0)
	std::cout<<"seg1在seg2的逆时针方向"<<std::endl;
else
	std::cout<<"seg1与seg2共线"<<std::endl;

int turnDirect=seg1.turnDirect(seg2);
if(turnDirect>0)
	std::cout<<"seg1到seg2为右拐"<<std::endl;
else if(turnDirect<0)
	std::cout<<"seg1到seg2为左拐"<<std::endl;
else
	std::cout<<"seg1与seg2共线"<<std::endl;
std::cout<<std::endl;//*/

///*确定任意一对线段是否相交
std::cout<<"线段是否相交测试"<<std::endl;
std::vector<lmtc::Segment> segments;
segments.push_back(lmtc::Segment(0.5,2,3.3,3));
segments.push_back(lmtc::Segment(2.6,3.6,5.2,3.1));
segments.push_back(lmtc::Segment(3,5,9,2.5));
segments.push_back(lmtc::Segment(4,2.2,10,4.5));
segments.push_back(lmtc::Segment(10,3.5,10.5,3.999));
segments.push_back(lmtc::Segment(7.7,3.4,10.5,4));
if(lmtc::Geometry::anySegmentsIntersect(segments))
	std::cout<<"存在两条线段相交"<<std::endl;
else
	std::cout<<"所有线段都不相交"<<std::endl;
std::cout<<std::endl;
//*/

///*求点集的凸包
std::cout<<"凸包测试"<<std::endl;
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
std::cout<<"凸包的各顶点序号为：";
for(unsigned int i=0;i<ordVec.size();i++)
	std::cout<<ordVec[i]<<",";
std::cout<<std::endl;
std::cout<<std::endl;
//*/

///*求最近点对
std::cout<<"最近点对测试"<<std::endl;
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
std::cout<<"最近点对的序号为："<<clsP1<<","<<clsP2<<" . 距离为："<<clsLen<<std::endl;
std::cout<<std::endl;
std::cout<<std::endl;
//*/

///*线性规划测试
std::cout<<"标准型线性规划测试"<<std::endl;
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
	std::cout<<"该线性规划问题无解"<<std::endl;
else if(signStd==0)
	std::cout<<"该线性规划问题无可行解"<<std::endl;
else if(signStd==1){
	std::cout<<"该线性规划问题最优值为："<<VsimplexStd<<std::endl;
	std::cout<<"该线性规划问题最优解为："<<std::endl;
	for(unsigned int i=0;i<XsimplexStd.size();i++)
		std::cout<<"x"<<i<<" = "<<XsimplexStd[i]<<" ;";
	std::cout<<std::endl;
}
std::cout<<std::endl;

std::cout<<"一般形线性规划测试"<<std::endl;
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
	std::cout<<"该线性规划问题无解"<<std::endl;
else if(signGeneral==0)
	std::cout<<"该线性规划问题无可行解"<<std::endl;
else if(signGeneral==1){
	std::cout<<"该线性规划问题最优值为："<<VsimplexGeneral<<std::endl;
	std::cout<<"该线性规划问题最优解为："<<std::endl;
	for(unsigned int i=0;i<XsimplexGeneral.size();i++)
		std::cout<<"x"<<i<<" = "<<XsimplexGeneral[i]<<" ;";
	std::cout<<std::endl;
}
std::cout<<std::endl;
//*/


///*分配问题测试
std::cout<<"分配问题测试："<<std::endl;
unsigned NAssign=12;
lmtc::Array<long double> costAssign(2,NAssign,NAssign);
for(unsigned int i=0;i<costAssign.getDimLen(0);i++)
	for(unsigned int j=0;j<costAssign.getDimLen(1);j++)
		costAssign(i,j)=lmtc::averageRandom(0,50);
std::vector<unsigned int> assignVec;
long double minCostAssign;
if(true==lmtc::OperationsResearch::bestAssign(costAssign,assignVec,minCostAssign))
{
	std::cout<<"该分配问题最低成本为："<<minCostAssign<<std::endl;
	std::cout<<"该分配问题最优解为："<<std::endl;
	for(unsigned int i=0;i<assignVec.size();i++)
		std::cout<<"x"<<i<<" = "<<assignVec[i]<<" ;";
	std::cout<<std::endl;

}else{
	std::cout<<"利用线性规划解分配问题失败！"<<std::endl;
}
std::cout<<std::endl;//*/

///*最优二度子图测试
std::cout<<"最优二度子图测试："<<std::endl;
unsigned NTDSG=12;
lmtc::Array<long double> ajacencyMatrixTDSG(2,NTDSG,NTDSG);
for(unsigned int i=0;i<ajacencyMatrixTDSG.getDimLen(0);i++)
	for(unsigned int j=0;j<ajacencyMatrixTDSG.getDimLen(1);j++)
		ajacencyMatrixTDSG(i,j)=lmtc::averageRandom(20,50);
std::vector<unsigned int> connectVecTDSG;
long double minLenTDSG;
if(true==lmtc::OperationsResearch::bestTwoDegreeSubGraph(ajacencyMatrixTDSG,connectVecTDSG,minLenTDSG))
{
	std::cout<<"该最优二度子图最短长度为："<<minLenTDSG<<std::endl;
	std::cout<<"该最优二度子图最优解为："<<std::endl;
	for(unsigned int i=0;i<connectVecTDSG.size();i++)
		std::cout<<"x"<<i<<" = "<<connectVecTDSG[i]<<" ;";
	std::cout<<std::endl;

}else{
	std::cout<<"利用线性规划解最优二度子图失败！"<<std::endl;
}
std::cout<<std::endl;//*/

///*拟阵测试，找出拟阵的一个具有最大权值的独立子集。此为抽象问题，读者可略过。
std::cout<<"拟阵测试："<<std::endl;
double weight[]={5,3,8};
int item[]={3,2,9};
lmtc::Matroid<double,int,std::vector<int>> matroid(weight,weight+3,item,item+3,canExpandTo,expandTo);
std::vector<int> result;
double w=matroid.getGreatestWeightIndependentSet(result);
for(std::vector<int>::iterator p=result.begin();p!=result.end();p++)
	std::cout<<*p<<std::endl;
std::cout<<"权重:"<<w<<std::endl;
std::cout<<std::endl;//*/


///*对称数组测试
std::cout<<"对称数组测试："<<std::endl;
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

///*一般堆测试
std::cout<<"一般堆测试："<<std::endl;
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

///*全域完全散列测试
std::cout<<"全域完全散列测试："<<std::endl;
lmtc::CompleteHash<unsigned long> hash(50,lmtc::MAX_PRIME,keyToNumber);
hash.insert(20);
for(unsigned int i=0;i<1000;i++)
	hash.insert((unsigned long)(lmtc::averageRandom()*50000));
hash.completeHashOptimize();
unsigned long *hashItem=hash.search(20);
hash.remove(*hashItem);
std::cout<<std::endl;//*/
	

///*红黑树测试
std::cout<<"红黑树测试："<<std::endl;
lmtc::RedBlackTree<int> rbtree;
std::cout<<"随机插入"<<std::endl;
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
		std::cout<<"顺序统计错误"<<std::endl;
}

std::cout<<"size= "<<rbtree.size()<<std::endl;

std::cout<<"随机删除"<<std::endl;
for(int i=1;i<20;i++)
{
	int temp=(int)(lmtc::averageRandom()*50000);//(int)(lmtc::averageRandom()*100000);
	lmtc::RedBlackTree<int>::ItemType del=rbtree.deleteItem(rbtree.search(temp));
	if(!del.isEmpty())
		std::cout<<rbtree.getItemValue(del)<<std::endl;
}

std::cout<<"size= "<<rbtree.size()<<std::endl;

std::cout<<"逆序输出："<<std::endl;
lmtc::RedBlackTree<int>::ItemType x=rbtree.maximum();
while(true){
	if(x.isEmpty())
		break;
	std::cout<<rbtree.getItemValue(x)<<std::endl;
	x=rbtree.predecessor(x);
}

std::cout<<"中序遍历"<<std::endl;
rbtree.traver_inOrder(f);
std::cout<<"前序遍历"<<std::endl;
rbtree.traver_preOrder(f);
std::cout<<"遍历结束"<<std::endl;

std::cout<<"开始验证"<<std::endl;
rbtree.asertTree(rbtree.getRoot());

for(unsigned int i=0;i<rbtree.size();i++){
	lmtc::RedBlackTree<int>::ItemType t=rbtree.searchItmOfOrder(i);
	if(rbtree.getOrderOfItm(t)!=i)
		std::cout<<"顺序统计错误"<<std::endl;
}
std::cout<<rbtree.size()<<std::endl;
rbtree.setEmpty();
std::cout<<"over"<<rbtree.size()<<std::endl;
std::cout<<std::endl;//*/

///*利用标准库顺序统计测试
std::cout<<"标准库顺序统计测试："<<std::endl;
int ntha1[]={2,5,8,10,12,7,99,3,54,46};
int ntha2[]={3,5,6,13,9,19,8,7,54,28,99,46,3,5,2,5,8,10,12,7,99,3,54,46,2,5,8,10,12,7,99,3,54,46,2,5,8,10,12,7,99,3,54,46};
std::vector<int> nthv1(ntha1,ntha1+10);
std::vector<int> nthv2(ntha2,ntha2+44);
std::vector<int> nthvec=lmtc::longestCommonSubsequence<int>(nthv1.begin(),nthv1.end(),nthv2.begin(),nthv2.end());
for(unsigned int i=0;i<nthvec.size();i++)
	std::cout<<nthvec[i]<<std::endl;
std::random_shuffle(ntha2,ntha2+44);
std::nth_element(ntha2,ntha2+20,ntha2+44);
std::cout<<"顺序统计"<<std::endl;
for(int i=0;i<44;i++)
	std::cout<<ntha2[i]<<std::endl;
std::cout<<std::endl;//*/

///*二项堆测试
std::cout<<"二项堆测试："<<std::endl;
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
std::cout<<"验证开始"<<std::endl;
bihp.asertBinomialHeap();
std::cout<<"验证结束"<<std::endl;

std::cout<<bihp.size()<<"end"<<bihp1.size()<<std::endl;

std::cout<<"释放资源1"<<std::endl;
bihp.setEmpty();
std::cout<<"释放资源2"<<std::endl;
bihp.traver_inOrder(f);
std::cout<<std::endl;//*/

///*斐波那契堆测试
std::cout<<"斐波那契堆测试："<<std::endl;
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

std::cout<<"验证开始"<<std::endl;
fbnqhp.asertFibonacciHeap();
std::cout<<"验证结束"<<std::endl;

std::cout<<fbnqhp.size()<<"end"<<fbnqhp1.size()<<std::endl;

std::cout<<"释放资源1"<<std::endl;
fbnqhp.setEmpty();
std::cout<<"释放资源2"<<std::endl;
std::cout<<std::endl;//*/

///*不相交集合测试
std::cout<<"不相交集合测试："<<std::endl;
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

//邻接矩阵转邻接表
std::vector<std::list<lmtc::Edge<int>>> ajacencyList;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix,ajacencyList);

//邻接表的转置
std::vector<std::list<lmtc::Edge<int>>> transposedAjacencyList;
lmtc::Graph::transposeAjacencyList(ajacencyList,transposedAjacencyList);


///*拓扑排序测试
std::cout<<"拓扑排序测试："<<std::endl;
std::vector<unsigned int> order=lmtc::Graph::topologicalSort(transposedAjacencyList);
std::cout<<"拓扑序列为："<<std::endl;
for(std::vector<unsigned int>::iterator iter=order.begin();iter!=order.end();iter++)
	std::cout<<*iter<<"  ";
std::cout<<std::endl;//*/

///*强连通分支测试
std::cout<<"强连通分支测试："<<std::endl;
std::vector<std::vector<unsigned int>> strongConnectComponents;
lmtc::Graph::computeStrngConctComps(ajacencyList,strongConnectComponents);
for(unsigned int i=0;i<strongConnectComponents.size();i++){
	std::cout<<"连通分支"<<i<<std::endl;
	for(std::vector<unsigned int>::iterator iter=strongConnectComponents[i].begin();iter!=strongConnectComponents[i].end();iter++)
		std::cout<<*iter<<" ; ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*欧拉回路测试
std::cout<<"欧拉回路测试："<<std::endl;
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
	std::cout<<"欧拉回路为："<<std::endl;
	for(std::list<lmtc::Edge<int>>::iterator iter=EulerCircuit.begin();iter!=EulerCircuit.end();iter++)
		std::cout<<iter->vSt<<"-"<<iter->vEd<<"   -->   ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*最小生成树测试
std::cout<<"最小生成树测试："<<std::endl;
int arr2[][8]={{1,1,2,7,3,0,0,0},{1,1,6,2,0,0,0,0},{2,6,1,4,0,0,0,0},{7,2,4,1,5,0,0,0},{3,0,0,5,1,0,0,0},{0,0,0,0,0,1,2,3},{0,0,0,0,0,2,1,1},{0,0,0,0,0,3,1,1}};
lmtc::Array<int> ajacencyMatrix2(2,8,8);
for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
		ajacencyMatrix2(i,j)=arr2[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList2;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix2,ajacencyList2);
std::vector<std::list<lmtc::Edge<int>>> mstAjacencyList;
int weightKruskal=lmtc::Graph::mstKruskal(ajacencyList2,mstAjacencyList);
std::cout<<"Kruskal最小生成树为：权重："<<weightKruskal<<std::endl;
for(unsigned int i=0;i<mstAjacencyList.size();i++){
	for(std::list<lmtc::Edge<int>>::const_iterator iter=mstAjacencyList[i].begin();iter!=mstAjacencyList[i].end();iter++){
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
	}
	std::cout<<std::endl;
}
std::vector<std::list<lmtc::Edge<int>>> mstAjacencyListPrim;
int weightPrim=lmtc::Graph::mstPrim(ajacencyList2,mstAjacencyListPrim);
std::cout<<"Prim最小生成树为：权重："<<weightPrim<<std::endl;
for(unsigned int i=0;i<mstAjacencyListPrim.size();i++){
	for(std::list<lmtc::Edge<int>>::const_iterator iter=mstAjacencyListPrim[i].begin();iter!=mstAjacencyListPrim[i].end();iter++){
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
	}
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*判断有向图或者无向图是否存在回路
std::cout<<"判断有向图或者无向图是否存在回路测试："<<std::endl;
int arr3[][4]={{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
lmtc::Array<int> ajacencyMatrix3(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		ajacencyMatrix3(i,j)=arr3[i][j];
std::vector<std::list<lmtc::Edge<int>>> ajacencyList3;
lmtc::Graph::ajacencyMatrixToList(ajacencyMatrix3,ajacencyList3);
if(lmtc::Graph::hasLoop(ajacencyList3,true)==true)
	std::cout<<"存在回路"<<std::endl;
else
	std::cout<<"不存在回路"<<std::endl;
std::cout<<std::endl;//*/

///*计算最短路径
std::cout<<"最短路径测试："<<std::endl;
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
	std::cout<<"存在负边"<<std::endl;
std::cout<<std::endl;//*/

///*计算每对顶点间的最短路径
std::cout<<"每对顶点间的最短路径测试："<<std::endl;
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
	std::cout<<"以k="<<k<<"为源点的最短路径树"<<std::endl;
	for(unsigned int i=0;i<pMatrix.getDimLen(1);i++)
		std::cout<<"parent of "<<i<<" is "<<pMatrix(k,i)<<" , and d of the path is "<<dMatrix(k,i)<<std::endl;
}
std::cout<<std::endl;//*/

///*计算最大流
std::cout<<"最大流测试："<<std::endl;
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
std::cout<<"最大流="<<maxF<<std::endl;
for(unsigned int i=0;i<flow.size();i++)
	for(std::list<lmtc::Edge<int>>::const_iterator iter=flow[i].begin();iter!=flow[i].end();iter++)
		std::cout<<iter->vSt<<"-"<<iter->vEd<<":"<<iter->data<<"   ";
std::cout<<std::endl;
std::cout<<std::endl;//*/


/*矩阵运算*/

///*矩阵乘法测试
std::cout<<"矩阵乘法测试："<<std::endl;
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
std::cout<<"矩阵乘法结果"<<std::endl;
for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
	for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
		std::cout<<matrixRs(i,j)<<"   ";
	std::cout<<std::endl;
}
std::cout<<std::endl;//*/

///*LUP 分解测试
std::cout<<"LUP 分解测试："<<std::endl;
double arr9[][4]={{2,0,2,0.6},{3,3,4,-2},{5,5,4,2},{-1,-2,3.4,-1}};
lmtc::Array<double> A(2,4,4);
for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
		A(i,j)=arr9[i][j];
lmtc::Array<double> L,U;
std::vector<unsigned int> P;
bool b=lmtc::Algebra::lupDecompose(A,L,U,P);
std::cout<<"LUP分解结果"<<std::endl;
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
	std::cout<<"LUP分解失败，奇异方阵！"<<std::endl;
std::cout<<std::endl;//*/

///*解线性方程组
std::cout<<"解线性方程组测试："<<std::endl;
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
	std::cout<<"无解";
std::cout<<std::endl;//*/

///*求逆矩阵测试
std::cout<<"矩阵求逆测试："<<std::endl;
double arr11[][3]={{1,2,0},{2,4,5},{5,6,3}};
lmtc::Array<double> A2(2,3,3);
for(int i=0;i<3;i++)
	for(int j=0;j<3;j++)
		A2(i,j)=arr11[i][j];
lmtc::Array<double> _A;
std::cout<<"矩阵求逆"<<std::endl;
if(lmtc::Algebra::inverseMatrixFast(A2,_A)){
//if(lmtc::Algebra::inverseMatrixByLUP(A2,_A)){
	for(unsigned int i=0;i<_A.getDimLen(0);i++){
		for(unsigned int j=0;j<_A.getDimLen(1);j++)
			std::cout<<_A(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(A2,_A);
	std::cout<<"与逆矩阵相乘的单位矩阵"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"不可逆"<<std::endl;
std::cout<<std::endl;//*/

///*伪逆矩阵测试
std::cout<<"伪逆矩阵测试："<<std::endl;
double arr12[][3]={{1,-1,1},{1,1,1},{1,2,4},{1,3,9},{1,5,25}};
lmtc::Array<double> A3(2,5,3);
for(int i=0;i<5;i++)
	for(int j=0;j<3;j++)
		A3(i,j)=arr12[i][j];
lmtc::Array<double> A3T=lmtc::Algebra::transposeMatrix(A3);
lmtc::Array<double> _Af;
lmtc::Array<double> _Ab;
std::cout<<"求前向伪逆矩阵"<<std::endl;
if(lmtc::Algebra::pseudoInverseMatrixForward(A3,_Af)){
	for(unsigned int i=0;i<_Af.getDimLen(0);i++){
		for(unsigned int j=0;j<_Af.getDimLen(1);j++)
			std::cout<<_Af(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(_Af,A3);
	std::cout<<"与前向伪逆矩阵相乘的单位矩阵"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"不可前向伪逆"<<std::endl;

std::cout<<"求后向伪逆矩阵"<<std::endl;
if(lmtc::Algebra::pseudoInverseMatrixBackward(A3T,_Ab)){
	for(unsigned int i=0;i<_Ab.getDimLen(0);i++){
		for(unsigned int j=0;j<_Ab.getDimLen(1);j++)
			std::cout<<_Ab(i,j)<<"   ";
		std::cout<<std::endl;
	}
	lmtc::Array<double> matrixRs=lmtc::Algebra::matrixMultiplyStrassen(A3T,_Ab);
	std::cout<<"与后向伪逆矩阵相乘的单位矩阵"<<std::endl;
	for(unsigned int i=0;i<matrixRs.getDimLen(0);i++){
		for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
			std::cout<<matrixRs(i,j)<<"   ";
		std::cout<<std::endl;
	}
}
else
	std::cout<<"不可后向伪逆"<<std::endl;
std::cout<<std::endl;//*/

///*方幂和测试
std::cout<<"方幂和测试："<<std::endl;
const unsigned int K=6;
const unsigned int N=100;
lmtc::Array<double> S=lmtc::Algebra::powerSumFormula(K);
std::cout<<"方幂和公式如下"<<std::endl;
for(unsigned int i=0;i<=K;i++){
	std::cout<<"幂为"<<i<<"时的公式：";
	for(unsigned int j=0;j<i+2;j++)
		std::cout<<"c"<<j<<" = "<<S(i,j)<<", ";
	std::cout<<std::endl;
}
std::cout<<"when K="<<K<<" ,N="<<N<<"  时，0^K+1^K+2^K+...+N^K = "<<lmtc::Algebra::powerSum(N,K)<<std::endl;
std::cout<<std::endl;//*/

///*复数运算测试
std::cout<<"复数运算测试："<<std::endl;
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

///*快速傅里叶变换,及逆变换，数值稳定性非常好，当cN=1000时为0.00000000001，当cN=10000时为0.0000000001，当cN=100000时为0.000000001，当cN=1000000时为0.00000001，
std::cout<<"快速傅里叶变换测试："<<std::endl;
std::vector<lmtc::ComplexNumber> a;
unsigned int cN=100;
for(unsigned i=0;i<cN;i++)
	a.push_back(lmtc::ComplexNumber(lmtc::averageRandom(0,10),lmtc::averageRandom(0,10)));
std::vector<lmtc::ComplexNumber> y=lmtc::FastFourierTransform::fft(a);
std::vector<lmtc::ComplexNumber> a1=lmtc::FastFourierTransform::_fft(y);
std::cout<<"系数向量a：";
for(unsigned i=0;i<a.size();i++)
	std::cout<<(std::string)a[i]<<"   ";
std::cout<<std::endl;
std::cout<<"点值向量y：";
for(unsigned i=0;i<y.size();i++)
	std::cout<<(std::string)y[i]<<"   ";
std::cout<<std::endl;
std::cout<<"系数向量a1：";
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

///*求卷积
std::cout<<"卷积测试："<<std::endl;
double Aarr[]={1,2,3,23};
double Barr[]={5,4,34};
std::vector<lmtc::ComplexNumber> Avec(Aarr,Aarr+4);
std::vector<lmtc::ComplexNumber> Bvec(Barr,Barr+3);
std::vector<lmtc::ComplexNumber>Cvec=lmtc::Algebra::convolution(Avec,Bvec);
std::cout<<"卷积为："<<std::endl;
for(unsigned int i=0;i<Cvec.size();i++)
	std::cout<<(std::string)Cvec[i]<<"   ";
std::cout<<std::endl;//*/

///*多维傅里叶变换测试
std::cout<<"高维傅里叶变换测试："<<std::endl;
lmtc::Array<lmtc::ComplexNumber> aMultiFFT(2,3,4);
for(unsigned int i=0;i<aMultiFFT.getDimLen(0);i++)
	for(unsigned int j=0;j<aMultiFFT.getDimLen(1);j++)
		aMultiFFT(i,j)=lmtc::ComplexNumber(lmtc::averageRandom(0,10),lmtc::averageRandom(0,10));
lmtc::Array<lmtc::ComplexNumber> yMultiFFT=lmtc::FastFourierTransform::fft(aMultiFFT);
lmtc::Array<lmtc::ComplexNumber> a1MultiFFT=lmtc::FastFourierTransform::_fft(yMultiFFT);
std::cout<<"高维傅里叶变换"<<std::endl;
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

///*多维卷积测试
std::cout<<"多维卷积测试："<<std::endl;
lmtc::Array<lmtc::ComplexNumber> mutiAvec(2,3,2);
lmtc::Array<lmtc::ComplexNumber> mutiBvec(3,2,3,3);
mutiAvec(1,1)=1;
mutiAvec(2,1)=2;
mutiAvec(0,1)=4;
mutiBvec(1,1,1)=3;
mutiBvec(1,2,0)=6;
mutiBvec(0,1,2)=5;
lmtc::Array<lmtc::ComplexNumber>mutiCvec=lmtc::Algebra::convolution(mutiAvec,mutiBvec);
std::cout<<"多维卷积为："<<std::endl;
std::vector<unsigned int> dimC(mutiCvec.getDimNum(),0);
while(dimC[mutiCvec.getDimNum()-1]<mutiCvec.getDimLen(mutiCvec.getDimNum()-1)){//将a中元素复制到正合幂数组a1
	if(!mutiCvec(dimC).equal(0,0.001)){
		std::cout<<"mutiCvec(";
		for(unsigned int i=0;i<dimC.size();i++)
			std::cout<<dimC[i]<<"  ";
		std::cout<<")   =   "<<(std::string)mutiCvec(dimC)<<std::endl;
	}
	for(unsigned int i=0;i<mutiCvec.getDimNum();i++){//计算多维数组遍历的下一个坐标
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

///*多01背包问题测试
std::cout<<"多01背包问题测试："<<std::endl;
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
std::cout<<"该多01背包问题的所装物品的最大价值为:"<<maxValue<<std::endl;
std::cout<<"物品按如下方式装填(i->j表示i物品放入j背包，j=0时表示i不装入)："<<std::endl;
for(unsigned int i=0;i<weightVec.size();i++)
	std::cout<<i<<"->"<<packAssigned[i]<<" ; ";
std::cout<<std::endl;
std::cout<<std::endl;//*/

std::cout<<"all test over!!!"<<std::endl;
	return 0;//表示成功执行
}
