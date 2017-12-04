#ifndef numberTheoryConst 
#define numberTheoryConst
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
#include <vector>
#include "number.h"
//���۳����㷨
namespace lmtc{
	//unsigned long���ܱ�ʾ���������
	const unsigned long MAX_PRIME=4294966297; 
	
	//Բ����
	const long double PI=3.1415926535897932384626433832795028841971;

	const long double E=2.71828182845904523536028747135266249775724;

	class NumberTheory{
	public:
		//��������minNumber����С����������smallPrimeNumber����С�������в��ԡ�
		static unsigned long generatePrimeNumberBySimpleTest(const unsigned long minNumber,const unsigned long smallPrimeNumber);
	
		//ŷ������㷨�����Լ��d=gcd(a,b),ʹ��d=ax+by
		//O(lgb),�μ����㷨����p526-528��
		static void gcd(const unsigned long a,const unsigned long b,unsigned long &d,long &x,long &y);
		//ŷ������㷨�Ĵ����汾������������ֵ�����㷨���ɷ����ת��Ϊ�����汾��һ���ͨ������ֵ���͸�ΪBigDecimal�Ϳ�������
		static void gcd(const BigDecimal &a,const BigDecimal &b,BigDecimal &d,BigDecimal &x,BigDecimal &y);

		//��С������,����lcm(a,b)=a*b/gcd(a,b).
		//O(lgb).
		static unsigned long lcm(const unsigned long a,const unsigned long b);

		//��ͬ�෽��ax=b(mod n),Ҫ��a,nΪ�������������쳣�������ر������н��������Ҫô�޽⣬Ҫôgcd(a,n)����ͬ�⣩��
		//O(lgn+gcd(a,n)),�μ����㷨����p533-535��
		static std::vector<unsigned long> modularLinearEquation(const unsigned long a,const long b,const unsigned long n);

		//���Ӷ����ͬ�෽����x=ai(mod ni)���й�ʣ�ඨ��,��ģN��Ψһ�⣬����N=n1*n2*...*nk��,Ҫ��n1��n2��...nk�������ʣ����򷵻ز���ȷ�����
		//O(lgN),�μ����㷨����p535-537��
		static unsigned long modularLinearEquationsBySunTzu(const std::vector<long> &a,const std::vector<unsigned long> &n);

		//ģȡ������
		//O(N^3),NΪ��������������λ�����μ����㷨����p539-540��
		static BigDecimal modularExponentiation(const BigDecimal &a,const BigDecimal &b,const BigDecimal &n);

		//MillerRabin���������㷨
		//O(time*N^3),NΪn�Ķ�����λ�����μ����㷨����p545-550��
		static bool MillerRabin(const BigDecimal &n,const unsigned int time=20);

		//����ʮ����λ��ΪdigitNum�Ĵ�����
		//����ʱ��Ϊ(time*digitNum^4)���μ����㷨����p545-550��
		static BigDecimal generatePrimeNumberByMillerRabin(const unsigned int digitNum,const unsigned int time=20);

		//������Կ��˽Կ
		//digitNumΪ��������Ϣ��ʮ���������λ������ԿΪ(e,n),˽ԿΪ(d,n)
		//����ʱ��Ϊ(time*digitNum^4)���μ����㷨����p540-544��
		static bool generateKeyOfRSA(const unsigned int digitNum,BigDecimal &n,BigDecimal &e,BigDecimal &d,const unsigned int time=20);
		
		//RSA���ܻ����(��Դ��������ܡ�����Ϊ��ͬ�㷨),ʵ�ʾ���ģȡ�ݵĹ���.
		//O(N^3),NΪ������Ӧʮ�������ֵ����λ�����μ����㷨����p540-544��
		static BigDecimal cryptRSA(const BigDecimal &msg,const BigDecimal &e_d,const BigDecimal &n);

		//RSA���ܻ����(����ַ��������ܡ�����Ϊ��ͬ�㷨),ʵ�ʾ���ģȡ�ݵĹ���.
		//O(N^3),NΪ������Ӧʮ�������ֵ����λ�����μ����㷨����p540-544��
		static std::string cryptRSA(const std::string &msg,const BigDecimal &e_d,const BigDecimal &n);

	private:
		//����MillerRabin�㷨�ĺ�����֤��
		static bool MillerRabin_witness(const BigDecimal &a,const BigDecimal &n);
	};
}
#endif