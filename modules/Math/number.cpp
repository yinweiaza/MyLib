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
#include "number.h"
#include "algebra.h"
#include <sstream>
namespace lmtc{

	BigDecimal::BigDecimal(short val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}

	BigDecimal::BigDecimal(unsigned short val):isPsv(true),numDt(0){
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}

	BigDecimal::BigDecimal(int val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}
	BigDecimal::BigDecimal(unsigned int val):isPsv(true),numDt(0){
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}
	BigDecimal::BigDecimal(long val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}
	BigDecimal::BigDecimal(unsigned long val):isPsv(true),numDt(0){
		while(val>0){
			dgt.push_back(val%10);
			val/=10;
		}
	}

	BigDecimal::BigDecimal(float val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		numDt=16;
		int cout=numDt;
		float iPart;
		float fPart;
		fPart=std::modf(val,&iPart);
		dgt.resize(numDt);
		while(iPart>=1){
			float iPt;
			float fPt;
			std::modf(iPart/10,&iPt);
			fPt=iPart-iPt*10;
			dgt.push_back((unsigned short)(fPt));
			iPart=iPt;
		}
		while(cout--){
			float iPt;
			fPart=std::modf(fPart*10,&iPt);
			dgt[cout]=((unsigned short)(iPt));
		}
	}

	BigDecimal::BigDecimal(double val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		numDt=16;
		int cout=numDt;
		double iPart;
		double fPart;
		fPart=std::modf(val,&iPart);
		dgt.resize(numDt);
		while(iPart>=1){
			double iPt;
			double fPt;
			std::modf(iPart/10,&iPt);
			fPt=iPart-iPt*10;
			dgt.push_back((unsigned short)(fPt));
			iPart=iPt;
		}
		while(cout--){
			double iPt;
			fPart=std::modf(fPart*10,&iPt);
			dgt[cout]=((unsigned short)(iPt));
		}
	}

	BigDecimal::BigDecimal(long double val):isPsv(true),numDt(0){
		if(val<0){
			isPsv=false;
			val=-val;
		}
		numDt=16;
		int cout=numDt;
		long double iPart;
		long double fPart;
		fPart=std::modf(val,&iPart);
		dgt.resize(numDt);
		while(iPart>=1){
			long double iPt;
			long double fPt;
			std::modf(iPart/10,&iPt);
			fPt=iPart-iPt*10;
			dgt.push_back((unsigned short)(fPt));
			iPart=iPt;
		}
		while(cout--){
			long double iPt;
			fPart=std::modf(fPart*10,&iPt);
			dgt[cout]=((unsigned short)(iPt));
		}
	}

	BigDecimal::BigDecimal(const std::string &dec):isPsv(true),numDt(0){
		if(dec.empty())
			return;
		bool dotFound=false;
		for(int i=(int)dec.size()-1;i>=0;i--){
			switch(dec[i]){
				case '-':
					if(i==0)
						isPsv=false;
					else
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				case '+':
					if(i!=0)
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				case '.':
					if(dotFound==false){
						numDt=dec.size()-i-1;
						dotFound=true;
					}else
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				default:
					if(dec[i]-'0'>9||dec[i]<'0')
						throw BigDecimalException("异常：字符串不符合数字串格式");
					else
						dgt.push_back((unsigned short)(dec[i]-'0'));
					break;
			}
		}
		int c=(int)dgt.size()-1;
		for(;c>=0;c--)
			if(dgt[c]!=0)
				break;
		dgt.resize(c+1);
	}

	BigDecimal::BigDecimal(const char *decStr):isPsv(true),numDt(0){
		std::string dec(decStr);
		if(dec.empty())
			return;
		bool dotFound=false;
		for(int i=(int)dec.size()-1;i>=0;i--){
			switch(dec[i]){
				case '-':
					if(i==0)
						isPsv=false;
					else
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				case '+':
					if(i!=0)
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				case '.':
					if(dotFound==false){
						numDt=dec.size()-i-1;
						dotFound=true;
					}else
						throw BigDecimalException("异常：字符串不符合数字串格式");
					break;
				default:
					if(dec[i]-'0'>9||dec[i]<'0')
						throw BigDecimalException("异常：字符串不符合数字串格式");
					else
						dgt.push_back((unsigned short)(dec[i]-'0'));
					break;
			}
		}
		int c=(int)dgt.size()-1;
		for(;c>=0;c--)
			if(dgt[c]!=0)
				break;
		dgt.resize(c+1);
	}

	BigDecimal::BigDecimal(const std::vector<bool> &bst):isPsv(true),numDt(0){
		if(bst.empty())
			return;
		unsigned int N=1;//计算正合幂
		while(N<bst.size())
			N*=2;
		std::vector<bool> bstPow=bst;
		bstPow.resize(N);//二进制串调整为正合幂
		std::vector<BigDecimal> powPower;
		powPower.push_back(1);
		powPower.push_back(2);
		while((unsigned int)(std::pow(2.0,powPower.size()-1.0)+0.5)<N)
			powPower.push_back(powPower.back()*powPower.back());
		BigDecimal dec=getBigDecimalFromBinary(bstPow,powPower,0,N-1);
		dgt=dec.dgt;
	}

	BigDecimal BigDecimal::getBigDecimalFromBinary(const std::vector<bool> &bstPow,const std::vector<BigDecimal> &powPower,unsigned int low,unsigned int up)const{
		if(low==up){
			if(bstPow[low])
				return 1;
			else
				return 0;
		}
		int len=(up-low+1)/2;
		int low1=low+len;
		int up1=low1-1;
		return getBigDecimalFromBinary(bstPow,powPower,low1,up)*powPower[(int)(std::log(len*1.0)/std::log(2.0)+1.5)]+getBigDecimalFromBinary(bstPow,powPower,low,up1);
	}

	BigDecimal BigDecimal::getInteger()const{
		if(dgt.size()<=numDt)
			return 0;
		BigDecimal minusDec=*this;
		minusDec.dgt.resize(dgt.size()-numDt);
		minusDec.numDt=0;
		for(unsigned int i=0;i<dgt.size()-numDt;i++)
			minusDec.dgt[i]=dgt[i+numDt];
		return minusDec;
	}

	BigDecimal BigDecimal::getFloat()const{
		if(dgt.size()<=numDt)
			return *this;
		BigDecimal minusDec=*this;
		minusDec.dgt.resize(numDt);
		return minusDec;
	}

	BigDecimal& BigDecimal::operator=(const BigDecimal &dec){
		unsigned int numDtOrg=numDt;
		dgt=dec.dgt;
		isPsv=dec.isPsv;
		numDt=dec.numDt;
		resetPrecision(numDtOrg);
		return *this;
	}

	BigDecimal& BigDecimal::operator+=(const BigDecimal &dec){
		if(isPsv!=dec.isPsv){
			this->operator-=(-dec);
			return *this;
		}
		unsigned int sumNumDt=std::max(numDt,dec.numDt);
		std::vector<unsigned short> sumDgt;
		unsigned int i=0,j=0;
		unsigned short carry=0;//进位
		while(i<dgt.size()||j<dec.dgt.size()){
			unsigned short s=0;
			if(numDt+j>dec.numDt+i){//numDt-i>dec.numDt-j
				if(i<dgt.size())
					s=dgt[i];
				i++;
			}
			else if(numDt+j<dec.numDt+i){//numDt-i<dec.numDt-j
				if(j<dec.dgt.size())
					s=dec.dgt[j];
				j++;
			}else{
				if(i>=dgt.size())
					s=dec.dgt[j];
				else if(j>=dec.dgt.size())
					s=dgt[i];
				else
					s=dgt[i]+dec.dgt[j];
				i++;
				j++;

			}
			s+=carry;
			carry=s/10;
			sumDgt.push_back(s%10);
		}
		if(1==carry)
			sumDgt.push_back(1);
		//将去除前导零后的有效数字存入dgt，精度不变
		int maxLoc=(int)sumDgt.size()-1;
		for(;maxLoc>=0;maxLoc--)
			if(sumDgt[maxLoc]!=0)
				break;
		dgt.clear();
		for(int i=sumNumDt-numDt;i<=maxLoc;i++)
			dgt.push_back(sumDgt[i]);
		return *this;
	}

	BigDecimal& BigDecimal::operator-=(const BigDecimal &dec){
		if(this->operator ==(dec)){//相等返回0
			dgt.clear();
			return *this;
		}
		if(isPsv!=dec.isPsv){//异号减法转加法
			this->operator+=(-dec);
			return *this;
		}
		//同号相减，利用dgt1、dgt2、numDt1、numDt2、toNegtive一般化处理减法。
		const std::vector<unsigned short> *dgt1,*dgt2;//被减数与减数的数字串，保证被减数大于减数
		int numDt1,numDt2;//被减数与减数的小数位数。
		bool toNegtive;//结果是否需取负。
		if(isPsv){
			if(this->operator>(dec)){
				dgt1=&dgt;dgt2=&dec.dgt;
				numDt1=numDt;numDt2=dec.numDt;
				toNegtive=false;
			}else{
				dgt1=&dec.dgt;dgt2=&dgt;
				numDt1=dec.numDt;numDt2=numDt;
				toNegtive=true;
			}
		}else{
			if(this->operator<(dec)){
				dgt1=&dgt;dgt2=&dec.dgt;
				numDt1=numDt;numDt2=dec.numDt;
				toNegtive=true;
			}else{
				dgt1=&dec.dgt;dgt2=&dgt;
				numDt1=dec.numDt;numDt2=numDt;
				toNegtive=false;
			}
		}
		//进行数字串的减法
		unsigned int difNumDt=std::max(numDt1,numDt2);
		std::vector<unsigned short> difDgt;
		unsigned int i=0,j=0;
		short borrow=0;//借位
		while(i<dgt1->size()||j<dgt2->size()){
			short d=0;
			if(numDt1+j>numDt2+i){//numDt1-i>numDt2-j
				if(i<dgt1->size())
					d=(short)(*dgt1)[i];
				i++;
			}
			else if(numDt1+j<numDt2+i){//numDt1-i<numDt2-j
				if(j<dgt2->size())
					d=-(short)(*dgt2)[j];
				j++;
			}else{
				if(i>=dgt1->size())
					d=-(short)(*dgt2)[j];
				else if(j>=dgt2->size())
					d=(short)(*dgt1)[i];
				else
					d=(short)(*dgt1)[i]-(short)(*dgt2)[j];
				i++;
				j++;
			}
			if(d>=borrow){
				d-=borrow;
				borrow=0;
			}else{
				d=(d+10-borrow);
				borrow=1;
			}
			difDgt.push_back(d);
		}
		if(0!=borrow)
			throw BigDecimalException("异常：大数相减异常，operator-=(const BigDecimal &dec) 的最终借位不为0");
		
		//将相减数字串去除前导0后的有效数字存入dgt
		int maxLoc=(int)difDgt.size()-1;
		for(;maxLoc>=0;maxLoc--)
			if(difDgt[maxLoc]!=0)
				break;
		dgt.clear();
		for(int j=difNumDt-numDt;j<=maxLoc;j++)
			dgt.push_back(difDgt[j]);
		isPsv=!toNegtive;
		return *this;
	}

	BigDecimal BigDecimal::operator+(const BigDecimal &dec)const{
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew.operator+=(dec);
		return decNew;
	}

	BigDecimal& BigDecimal::operator*=(const BigDecimal &dec){
		if(isPsv&&dec.isPsv||!isPsv&&!dec.isPsv)//确定正负号
			isPsv=true;
		else
			isPsv=false;
		if(dgt.empty()||dec.dgt.empty()){//有一个为零时，结果为零返回。
			dgt.clear();
			return *this;
		}
		std::vector<unsigned long> mul(dgt.size()+dec.dgt.size());
		//当乘数长度都很小，或其中一个乘数长度比另一个乘数长度小很多时，采用朴素乘法。
		if(dgt.size()<64&&dec.dgt.size()<64||dgt.size()<5*std::log((double)dec.dgt.size())/std::log(2.0)||dec.dgt.size()<5*std::log((double)dgt.size())/std::log(2.0)){
			for(unsigned int i=0;i<dgt.size();i++)//相乘运算
				for(unsigned int j=0;j<dec.dgt.size();j++)
					mul[i+j]+=dgt[i]*dec.dgt[j];
		}else{//乘数长度都较大，且其中一个并不比另一个小很多时,使用fft卷积计算乘法
			std::vector<ComplexNumber> mulCmp,a(dgt.begin(),dgt.end()),b(dec.dgt.begin(),dec.dgt.end());
			mulCmp=Algebra::convolution(a,b);
			for(unsigned int i=0;i<mulCmp.size();i++)
				mul[i]=(unsigned long)(mulCmp[i].getRel()+0.5);
		}
		for(unsigned int i=0;i<mul.size()-1;i++){//进位操作
			if(mul[i]>9){
				mul[i+1]+=mul[i]/10;
				mul[i]%=10;
			}
		}
		if(mul.back()>9)
			throw lmtc::BigDecimalException("异常：大数相乘异常，进位后最高位超过9");
		int i=(int)mul.size()-1;
		while(i>=0){//找到最高非零位置
			if(mul[i]==0)
				i--;
			else
				break;
		}
		dgt.clear();
		for(int j=dec.numDt;j<=i;j++)//将有效数字存入dgt
			dgt.push_back((unsigned short)mul[j]);
		return *this;	
	}

	BigDecimal& BigDecimal::operator/=(const BigDecimal &dec){
		std::vector<unsigned short> divident,divisor;//被除数、除数，移位转换为大整数的相除，仍保持被除数的精度。
		divident.insert(divident.end(),dec.numDt,0);
		divident.insert(divident.end(),dgt.begin(),dgt.end());
		divisor.insert(divisor.end(),dec.dgt.begin(),dec.dgt.end());
		
		int c=(int)divident.size()-1;
		for(;c>=0;c--)//消除被除数的前导零
			if(divident[c]!=0)
				break;
		divident.resize(c+1);
		c=(int)divisor.size()-1;
		for(;c>=0;c--)//消除除数的前导零
			if(divisor[c]!=0)
				break;
		divisor.resize(c+1);
		if(divisor.empty())
			throw BigDecimalException("异常：大数异常，除数为0");
		if(isPsv&&dec.isPsv||!isPsv&&!dec.isPsv)//确定正负号
			isPsv=true;
		else
			isPsv=false;
		if(divident.size()<divisor.size())//被除数有效位数小于除数，则置零
		{
			dgt.clear();
			return *this;
		}
		
		std::vector<unsigned short>quotient(divident.size()-divisor.size()+1);//商向量
		int u=(int)divident.size()-1;
		bool hasBorrow=false;//前面一位是否有借位
		while(u>=(int)divisor.size()-1){
			if(!hasBorrow){//没借位
				bool greaterEqual=true;
				for(int i=0;i<(int)divisor.size();i++){//比较是否可除
					if(divisor[(int)divisor.size()-1-i]<divident[u-i])
						break;
					else if(divisor[(int)divisor.size()-1-i]>divident[u-i]){
						greaterEqual=false;
						break;
					}
				}
				if(!greaterEqual){//不可除		
					if(divident[u])
						hasBorrow=true;
					u--;
				}else{//可除
					quotient[u+1-(int)divisor.size()]++;
					short borrow=0;
					for(int i=0;i<(int)divisor.size();i++){
						int temp=u+1+i-(int)divisor.size();
						short d=(short)divident[temp]-(short)divisor[i];
						if(d>=borrow){
							d-=borrow;
							borrow=0;
						}else{
							d=(d+10-borrow);
							borrow=1;
						}
						divident[temp]=d;
					}
					if(!divident[u])
						u--;
				}

			}else{//有借位，则肯定可除
				quotient[u+1-(int)divisor.size()]++;
				short borrow=0;
				for(int i=0;i<(int)divisor.size();i++){
					int temp=u+1+i-(int)divisor.size();
					short d=(short)divident[temp]-(short)divisor[i];
					if(d>=borrow){
						d-=borrow;
						borrow=0;
					}else{
						d=(d+10-borrow);
						borrow=1;
					}
					divident[temp]=d;
				}
				divident[u+1]-=borrow;
				if(!divident[u+1])
					hasBorrow=false;	
			}
		}
		c=(int)quotient.size()-1;
		for(;c>=0;c--)//消除被除数的前导零
			if(quotient[c]!=0)
				break;
		quotient.resize(c+1);
		dgt=quotient;
		return *this;
	}

	BigDecimal& BigDecimal::operator%=(const BigDecimal &dec){
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew/=dec;
		BigDecimal decTemp=decNew.getInteger();
		decTemp.resetPrecision(decNew.numDt);
		decTemp*=dec;
		this->operator-=(decTemp);
		return *this;
	}

	BigDecimal BigDecimal::operator*(const BigDecimal &dec)const{
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew.operator*=(dec);
		return decNew;
	}
	BigDecimal BigDecimal::operator/(const BigDecimal &dec)const{
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew.operator/=(dec);
		return decNew;
	}
	BigDecimal BigDecimal::operator%(const BigDecimal &dec)const{
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew.operator%=(dec);
		return decNew;
	}

	BigDecimal BigDecimal::operator-(const BigDecimal &dec)const{
		BigDecimal decNew=*this;
		if(decNew.numDt<dec.numDt)
			decNew.resetPrecision(dec.numDt);
		decNew.operator-=(dec);
		return decNew;
	}

	bool BigDecimal::operator==(const BigDecimal &dec)const{
		if(isPsv!=dec.isPsv){//异号时，除非-0==+0，否则返回false。
			for(unsigned int i=0;i<dgt.size();i++)
				if(dgt[i]!=0)
					return false;
			for(unsigned int i=0;i<dec.dgt.size();i++)
				if(dec.dgt[i]!=0)
					return false;
			return true;
		}
		//同号时比较
		int i=(int)dgt.size()-1,j=(int)dec.dgt.size()-1;
		while(i>=0||j>=0){
			if(i-(int)numDt>j-(int)dec.numDt){
				if(i>=0&&dgt[i]!=0)
					return false;
				i--;
			}
			else if(i-(int)numDt<j-(int)dec.numDt){
				if(j>=0&&dec.dgt[j]!=0)
					return false;
				j--;
			}else{
				unsigned short di=0;
				if(i>=0)
					di=dgt[i];
				unsigned short dj=0;
				if(j>=0)
					dj=dec.dgt[j];
				if(di!=dj)
					return false;
				i--;j--;
			}
		}
		return true;
	}

	bool BigDecimal::operator!=(const BigDecimal &dec)const{
		return !this->operator ==(dec);
	}
	bool BigDecimal::operator>(const BigDecimal &dec)const{
		if(isPsv!=dec.isPsv){//异号时，除非-0==+0返回false，否则返回isPsv。
			for(unsigned int i=0;i<dgt.size();i++)
				if(dgt[i]!=0)
					return isPsv;
			for(unsigned int i=0;i<dec.dgt.size();i++)
				if(dec.dgt[i]!=0)
					return isPsv;
			return false;//-0==+0
		}
		//同号时比较
		int i=(int)dgt.size()-1,j=(int)dec.dgt.size()-1;
		while(i>=0||j>=0){
			if(i-(int)numDt>j-(int)dec.numDt){
				if(i>=0&&dgt[i]!=0)
					return isPsv;
				i--;
			}
			else if(i-(int)numDt<j-(int)dec.numDt){
				if(j>=0&&dec.dgt[j]!=0)
					return !isPsv;
				j--;
			}else{
				unsigned short di=0;
				if(i>=0)
					di=dgt[i];
				unsigned short dj=0;
				if(j>=0)
					dj=dec.dgt[j];
				if(di>dj)
					return isPsv;
				else if(di<dj)
					return !isPsv;
				i--;j--;
			}
		}
		return false;//二者相等
	}
	bool BigDecimal::operator>=(const BigDecimal &dec)const{
		return this->operator >(dec)||this->operator ==(dec);
	}
	bool BigDecimal::operator<(const BigDecimal &dec)const{
		return !this->operator >=(dec);
	}
	bool BigDecimal::operator<=(const BigDecimal &dec)const{
		return !this->operator >(dec);
	}
	BigDecimal& BigDecimal::operator>>(const unsigned int digit){
		if(digit>=dgt.size())
			dgt.clear();
		else{
			for(unsigned int i=0;i<dgt.size()-digit;i++)
				dgt[i]=dgt[i+digit];
			dgt.resize(dgt.size()-digit);
		}
		return *this;
	}
	BigDecimal& BigDecimal::operator<<(const unsigned int digit){
		if(!dgt.empty())
				dgt.insert(dgt.begin(),digit,0);
		return *this;
	}

	void BigDecimal::resetPrecision(unsigned int num_dt){
		if(num_dt==numDt)
			return;
		if(num_dt>numDt){
			std::vector<unsigned short> dgtTemp;
			for(unsigned int i=0;i<num_dt-numDt;i++)
				dgtTemp.push_back(0);
			for(unsigned int i=0;i<dgt.size();i++)
				dgtTemp.push_back(dgt[i]);
			dgt=dgtTemp;
			numDt=num_dt;
		}else{
			std::vector<unsigned short> dgtTemp;
			for(unsigned int i=numDt-num_dt;i<dgt.size();i++)
				dgtTemp.push_back(dgt[i]);
			dgt=dgtTemp;
			numDt=num_dt;
		}
	}

	BigDecimal::operator std::string()const{
		std::ostringstream os;
		if(false==isPsv)
			os<<"-";
		if(dgt.size()<=numDt){
			os<<"0";
			if(numDt!=0)
				os<<".";
			int numZero=numDt-(int)dgt.size();
			while(numZero-->0)
				os<<"0";
			for(int i=(int)dgt.size()-1;i>=0;i--)
				os<<dgt[i];
		}else{
			for(int i=(int)dgt.size()-1;i>=(int)numDt;i--)
				os<<dgt[i];
			if(numDt!=0)
				os<<".";
			for(int i=(int)numDt-1;i>=0;i--)
				os<<dgt[i];
		}
		return os.str();
	}

	unsigned int BigDecimal::getBinary(std::vector<bool> &bstResult)const{
		BigDecimal Int=getInteger();
		Int.isPsv=true;
		BigDecimal Flt=getFloat();
		Flt.isPsv=true;
		bstResult.clear();
		int maxBiDt=(int)(Flt.numDt*std::log(10.0)/std::log(2.0)+0.5);//小数部分二进制串最大长度
		while(maxBiDt-->0){//乘2法计算小数部分的二进制串
			if(Flt==0)
				break;
			Flt*=2;
			if(Flt>=1){
				bstResult.push_back(1);
				Flt-=1;
			}else
				bstResult.push_back(0);
		}
		for(unsigned int i=0;i<bstResult.size()/2;i++)
			std::swap(bstResult[i],bstResult[bstResult.size()-1-i]);
		unsigned int biDt=bstResult.size();
		while(Int>0){//除2法计算整数部分的二进制串
			if(Int.dgt[0]%2)
				bstResult.push_back(1);
			else
				bstResult.push_back(0);
			Int/=2;
		}
		return biDt;
	}


}