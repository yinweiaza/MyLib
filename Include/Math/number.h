#ifndef numberConst
#define numberConst
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

#include "myException.h"
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
//处理复杂数字方面的类
namespace lmtc{
	//复数类，可与内置数字类型兼容运算。
	class ComplexNumber{
	public:
		//构造函数
		ComplexNumber():rel(0),vir(0){}
		ComplexNumber(long double realPart):rel(realPart),vir(0){}//使得与内置类型可兼容运算
		ComplexNumber(long double realPart,long double virtualPart):rel(realPart),vir(virtualPart){}
		//操作符重载
		ComplexNumber operator+(const ComplexNumber &cxNb)const{return ComplexNumber(rel+cxNb.rel,vir+cxNb.vir);}
		ComplexNumber operator-(const ComplexNumber &cxNb)const{return ComplexNumber(rel-cxNb.rel,vir-cxNb.vir);}
		ComplexNumber operator*(const ComplexNumber &cxNb)const{return ComplexNumber(rel*cxNb.rel-vir*cxNb.vir,rel*cxNb.vir+vir*cxNb.rel);}
		ComplexNumber operator/(const ComplexNumber &cxNb)const{long double temp=cxNb.rel*cxNb.rel+cxNb.vir*cxNb.vir; if(temp<0.0000000001) throw ComplexNumberException("除零异常");return ComplexNumber((rel*cxNb.rel+vir*cxNb.vir)/temp,(cxNb.rel*vir-rel*cxNb.vir)/temp);}
		ComplexNumber& operator+=(const ComplexNumber &cxNb){rel+=cxNb.rel,vir+=cxNb.vir;return *this;}
		ComplexNumber& operator-=(const ComplexNumber &cxNb){rel-=cxNb.rel,vir-=cxNb.vir;return *this;}
		ComplexNumber& operator*=(const ComplexNumber &cxNb){long double rel1=rel*cxNb.rel-vir*cxNb.vir;long double vir1=rel*cxNb.vir+vir*cxNb.rel;rel=rel1;vir=vir1;return *this;}
		ComplexNumber& operator/=(const ComplexNumber &cxNb){long double temp=cxNb.rel*cxNb.rel+cxNb.vir*cxNb.vir;long double rel1=(rel*cxNb.rel+vir*cxNb.vir)/temp;long double vir1=(cxNb.rel*vir-rel*cxNb.vir)/temp;rel=rel1;vir=vir1;return *this;}
		//字符串转换操作符
		operator std::string()const{char str[50];sprintf_s(str,"%.12lf + %.12lfi",rel,vir);return std::string(str);}
	
		//相等比较，实部和虚部的相等精度为accuracy
		bool equal(const ComplexNumber &cxNb,long double accuracy=0.0000001)const{if(std::abs(rel-cxNb.rel)<accuracy&&std::abs(vir-cxNb.vir)<accuracy) return true;else return false;}
		//获取实部或虚部
		long double getRel()const{return rel;}
		long double getVir()const{return vir;}
	private:
		long double rel;//实部
		long double vir;//虚部
	};

	//大数类，兼容大实数和大整数，可与内置数字类型兼容运算。
	class BigDecimal{
	public:
		//构造函数
		//默认初始化为空数字，当成零处理，整型无小数位。O(1)
		BigDecimal():isPsv(true),numDt(0){}
		//接受整型的构造函数，整型无小数位。O(d),d为数字val的十进制位数
		BigDecimal(short val);
		BigDecimal(int val);
		BigDecimal(long val);
		BigDecimal(unsigned short val);
		BigDecimal(unsigned int val);
		BigDecimal(unsigned long val);
		//接受浮点类型的构造函数，小数位数设为16位。O(d+16),d为数字val的整数部分十进制位数
		BigDecimal(float val);
		BigDecimal(double val);
		BigDecimal(long double val);
		//接受数字字符串的构造函数，字符串型如："(+/-)(0-9)*((.)(0-9)*)",根据字符串自动设置正负号、小数位数。O(n),n为字符串长度
		BigDecimal(const std::string &dec);
		BigDecimal(const char *dec);
		//接受二进制串的构造函数，构造成正的大整数。bst[0]表示二进制数的低位。O(NlgN),N为bst的大小
		BigDecimal(const std::vector<bool> &bst);

		//操作符重载。
		//操作符+、-(2)、*、/、%的返回值的小数位数提示为操作数中的较大小数位数，操作数本身是不会有任何改变的。
		//操作符=、+=、-=、*=、/=、%=、<<、>>不改变自身的小数位数，运算过程采用先提升进行计算然后再截断的策略。
		//受限于小数位数，会出现下溢的情况(可扩大小数位数以避免之)，此时自动归零。永远不会出现上溢的现象。
		//操作符+、-(2)、=、+=、-=、字符串转换、以及所有的比较操作符的时间复杂度为O(n),n为操作数中较大的十进制位数。
		//操作符>>、<<的时间复杂度为O(n+digit),n为大数的十进制位数,digit为位移数目。
		//操作符*、*=的时间复杂度为O(min(nlgn，n1*n2)),n1,n2为两操作数的十进制位数,n=max(n1,n2).
		//操作符/、/=、%、%=的时间复杂度为O(n1*n2),n1,n2为两操作数的十进制位数
		BigDecimal operator+(const BigDecimal &dec)const;
		BigDecimal operator-()const{BigDecimal minusDec=*this;minusDec.isPsv=!isPsv;return minusDec;}
		BigDecimal operator-(const BigDecimal &dec)const;
		BigDecimal operator*(const BigDecimal &dec)const;
		BigDecimal operator/(const BigDecimal &dec)const;
		BigDecimal operator%(const BigDecimal &dec)const;
		BigDecimal& operator=(const BigDecimal &dec);
		BigDecimal& operator+=(const BigDecimal &dec);
		BigDecimal& operator-=(const BigDecimal &dec);
		BigDecimal& operator*=(const BigDecimal &dec);
		BigDecimal& operator/=(const BigDecimal &dec);
		BigDecimal& operator%=(const BigDecimal &dec);
		bool operator==(const BigDecimal &dec)const;
		bool operator!=(const BigDecimal &dec)const;
		bool operator>(const BigDecimal &dec)const;
		bool operator>=(const BigDecimal &dec)const;
		bool operator<(const BigDecimal &dec)const;
		bool operator<=(const BigDecimal &dec)const;
		BigDecimal& operator>>(unsigned int digit);//十进制右移，相当于digit次除10运算
		BigDecimal& operator<<(unsigned int digit);//十进制左移，相当于digit次乘10运算
		operator std::string()const;//字符串转换操作符

		//重置小数位数为num_dt位小数，且保持原数值不变。
		//增加小数位数时为O(num_dt-numDt),减少小数位数时为O(n),n为十进制位数。
		void resetPrecision(unsigned int num_dt);

		//获得整型部分,O(n),n为十进制位数
		BigDecimal getInteger()const;
		//获得小数部分,O(n),n为十进制位数
		BigDecimal getFloat()const;

		//获得数字串的二进制串形式,存入bstResult，并返回二进制小数位数。
		//O(n*n),n为十进制位数
		unsigned int getBinary(std::vector<bool> &bstResult)const;

	public://获取大数基本信息
		bool is_psv()const{return isPsv;}
		unsigned int num_dt()const{return numDt;}
		unsigned int dgt_size()const{return dgt.size();}
		//用于访问dgt数组的方括号操作符重载
		unsigned short operator[](unsigned int i)const{if(i>=dgt.size())throw BigDecimalException("异常：访问超出范围");return dgt[i];}

	private:
		//用于接受二进制向量的构造函数调用。
		BigDecimal getBigDecimalFromBinary(const std::vector<bool> &bstPow,const std::vector<BigDecimal> &powPower,unsigned int low,unsigned int up)const;
	private:
		bool isPsv;//是否为正
		unsigned int numDt;//小数点位数
		std::vector<unsigned short> dgt;//十进制数字串，dgt[0]为最低位。
	};
}

#endif