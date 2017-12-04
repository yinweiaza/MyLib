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
//���������ַ������
namespace lmtc{
	//�����࣬���������������ͼ������㡣
	class ComplexNumber{
	public:
		//���캯��
		ComplexNumber():rel(0),vir(0){}
		ComplexNumber(long double realPart):rel(realPart),vir(0){}//ʹ�����������Ϳɼ�������
		ComplexNumber(long double realPart,long double virtualPart):rel(realPart),vir(virtualPart){}
		//����������
		ComplexNumber operator+(const ComplexNumber &cxNb)const{return ComplexNumber(rel+cxNb.rel,vir+cxNb.vir);}
		ComplexNumber operator-(const ComplexNumber &cxNb)const{return ComplexNumber(rel-cxNb.rel,vir-cxNb.vir);}
		ComplexNumber operator*(const ComplexNumber &cxNb)const{return ComplexNumber(rel*cxNb.rel-vir*cxNb.vir,rel*cxNb.vir+vir*cxNb.rel);}
		ComplexNumber operator/(const ComplexNumber &cxNb)const{long double temp=cxNb.rel*cxNb.rel+cxNb.vir*cxNb.vir; if(temp<0.0000000001) throw ComplexNumberException("�����쳣");return ComplexNumber((rel*cxNb.rel+vir*cxNb.vir)/temp,(cxNb.rel*vir-rel*cxNb.vir)/temp);}
		ComplexNumber& operator+=(const ComplexNumber &cxNb){rel+=cxNb.rel,vir+=cxNb.vir;return *this;}
		ComplexNumber& operator-=(const ComplexNumber &cxNb){rel-=cxNb.rel,vir-=cxNb.vir;return *this;}
		ComplexNumber& operator*=(const ComplexNumber &cxNb){long double rel1=rel*cxNb.rel-vir*cxNb.vir;long double vir1=rel*cxNb.vir+vir*cxNb.rel;rel=rel1;vir=vir1;return *this;}
		ComplexNumber& operator/=(const ComplexNumber &cxNb){long double temp=cxNb.rel*cxNb.rel+cxNb.vir*cxNb.vir;long double rel1=(rel*cxNb.rel+vir*cxNb.vir)/temp;long double vir1=(cxNb.rel*vir-rel*cxNb.vir)/temp;rel=rel1;vir=vir1;return *this;}
		//�ַ���ת��������
		operator std::string()const{char str[50];sprintf_s(str,"%.12lf + %.12lfi",rel,vir);return std::string(str);}
	
		//��ȱȽϣ�ʵ�����鲿����Ⱦ���Ϊaccuracy
		bool equal(const ComplexNumber &cxNb,long double accuracy=0.0000001)const{if(std::abs(rel-cxNb.rel)<accuracy&&std::abs(vir-cxNb.vir)<accuracy) return true;else return false;}
		//��ȡʵ�����鲿
		long double getRel()const{return rel;}
		long double getVir()const{return vir;}
	private:
		long double rel;//ʵ��
		long double vir;//�鲿
	};

	//�����࣬���ݴ�ʵ���ʹ����������������������ͼ������㡣
	class BigDecimal{
	public:
		//���캯��
		//Ĭ�ϳ�ʼ��Ϊ�����֣������㴦��������С��λ��O(1)
		BigDecimal():isPsv(true),numDt(0){}
		//�������͵Ĺ��캯����������С��λ��O(d),dΪ����val��ʮ����λ��
		BigDecimal(short val);
		BigDecimal(int val);
		BigDecimal(long val);
		BigDecimal(unsigned short val);
		BigDecimal(unsigned int val);
		BigDecimal(unsigned long val);
		//���ܸ������͵Ĺ��캯����С��λ����Ϊ16λ��O(d+16),dΪ����val����������ʮ����λ��
		BigDecimal(float val);
		BigDecimal(double val);
		BigDecimal(long double val);
		//���������ַ����Ĺ��캯�����ַ������磺"(+/-)(0-9)*((.)(0-9)*)",�����ַ����Զ����������š�С��λ����O(n),nΪ�ַ�������
		BigDecimal(const std::string &dec);
		BigDecimal(const char *dec);
		//���ܶ����ƴ��Ĺ��캯������������Ĵ�������bst[0]��ʾ���������ĵ�λ��O(NlgN),NΪbst�Ĵ�С
		BigDecimal(const std::vector<bool> &bst);

		//���������ء�
		//������+��-(2)��*��/��%�ķ���ֵ��С��λ����ʾΪ�������еĽϴ�С��λ���������������ǲ������κθı�ġ�
		//������=��+=��-=��*=��/=��%=��<<��>>���ı������С��λ����������̲������������м���Ȼ���ٽضϵĲ��ԡ�
		//������С��λ�����������������(������С��λ���Ա���֮)����ʱ�Զ����㡣��Զ����������������
		//������+��-(2)��=��+=��-=���ַ���ת�����Լ����еıȽϲ�������ʱ�临�Ӷ�ΪO(n),nΪ�������нϴ��ʮ����λ����
		//������>>��<<��ʱ�临�Ӷ�ΪO(n+digit),nΪ������ʮ����λ��,digitΪλ����Ŀ��
		//������*��*=��ʱ�临�Ӷ�ΪO(min(nlgn��n1*n2)),n1,n2Ϊ����������ʮ����λ��,n=max(n1,n2).
		//������/��/=��%��%=��ʱ�临�Ӷ�ΪO(n1*n2),n1,n2Ϊ����������ʮ����λ��
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
		BigDecimal& operator>>(unsigned int digit);//ʮ�������ƣ��൱��digit�γ�10����
		BigDecimal& operator<<(unsigned int digit);//ʮ�������ƣ��൱��digit�γ�10����
		operator std::string()const;//�ַ���ת��������

		//����С��λ��Ϊnum_dtλС�����ұ���ԭ��ֵ���䡣
		//����С��λ��ʱΪO(num_dt-numDt),����С��λ��ʱΪO(n),nΪʮ����λ����
		void resetPrecision(unsigned int num_dt);

		//������Ͳ���,O(n),nΪʮ����λ��
		BigDecimal getInteger()const;
		//���С������,O(n),nΪʮ����λ��
		BigDecimal getFloat()const;

		//������ִ��Ķ����ƴ���ʽ,����bstResult�������ض�����С��λ����
		//O(n*n),nΪʮ����λ��
		unsigned int getBinary(std::vector<bool> &bstResult)const;

	public://��ȡ����������Ϣ
		bool is_psv()const{return isPsv;}
		unsigned int num_dt()const{return numDt;}
		unsigned int dgt_size()const{return dgt.size();}
		//���ڷ���dgt����ķ����Ų���������
		unsigned short operator[](unsigned int i)const{if(i>=dgt.size())throw BigDecimalException("�쳣�����ʳ�����Χ");return dgt[i];}

	private:
		//���ڽ��ܶ����������Ĺ��캯�����á�
		BigDecimal getBigDecimalFromBinary(const std::vector<bool> &bstPow,const std::vector<BigDecimal> &powPower,unsigned int low,unsigned int up)const;
	private:
		bool isPsv;//�Ƿ�Ϊ��
		unsigned int numDt;//С����λ��
		std::vector<unsigned short> dgt;//ʮ�������ִ���dgt[0]Ϊ���λ��
	};
}

#endif