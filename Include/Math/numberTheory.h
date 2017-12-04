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
//数论常用算法
namespace lmtc{
	//unsigned long所能表示的最大质数
	const unsigned long MAX_PRIME=4294966297; 
	
	//圆周率
	const long double PI=3.1415926535897932384626433832795028841971;

	const long double E=2.71828182845904523536028747135266249775724;

	class NumberTheory{
	public:
		//产生大于minNumber的最小质数，利用smallPrimeNumber个较小质数进行测试。
		static unsigned long generatePrimeNumberBySimpleTest(const unsigned long minNumber,const unsigned long smallPrimeNumber);
	
		//欧几里得算法求最大公约数d=gcd(a,b),使得d=ax+by
		//O(lgb),参见《算法导论p526-528》
		static void gcd(const unsigned long a,const unsigned long b,unsigned long &d,long &x,long &y);
		//欧几里得算法的大数版本。所有内置数值类型算法都可方便地转换为大数版本，一般仅通过将数值类型改为BigDecimal就可做到。
		static void gcd(const BigDecimal &a,const BigDecimal &b,BigDecimal &d,BigDecimal &x,BigDecimal &y);

		//最小公倍数,返回lcm(a,b)=a*b/gcd(a,b).
		//O(lgb).
		static unsigned long lcm(const unsigned long a,const unsigned long b);

		//解同余方程ax=b(mod n),要求a,n为正整数（否则异常），返回保存所有解的向量（要么无解，要么gcd(a,n)个不同解）。
		//O(lgn+gcd(a,n)),参见《算法导论p533-535》
		static std::vector<unsigned long> modularLinearEquation(const unsigned long a,const long b,const unsigned long n);

		//孙子定理解同余方程组x=ai(mod ni)（中国剩余定理,对模N有唯一解，其中N=n1*n2*...*nk）,要求n1、n2、...nk两两互质，否则返回不正确结果。
		//O(lgN),参见《算法导论p535-537》
		static unsigned long modularLinearEquationsBySunTzu(const std::vector<long> &a,const std::vector<unsigned long> &n);

		//模取幂运算
		//O(N^3),N为参数的最大二进制位数。参见《算法导论p539-540》
		static BigDecimal modularExponentiation(const BigDecimal &a,const BigDecimal &b,const BigDecimal &n);

		//MillerRabin素数测试算法
		//O(time*N^3),N为n的二进制位数。参见《算法导论p545-550》
		static bool MillerRabin(const BigDecimal &n,const unsigned int time=20);

		//产生十进制位数为digitNum的大质数
		//期望时间为(time*digitNum^4)。参见《算法导论p545-550》
		static BigDecimal generatePrimeNumberByMillerRabin(const unsigned int digitNum,const unsigned int time=20);

		//产生公钥和私钥
		//digitNum为待加密信息的十进制数最大位数，公钥为(e,n),私钥为(d,n)
		//期望时间为(time*digitNum^4)。参见《算法导论p540-544》
		static bool generateKeyOfRSA(const unsigned int digitNum,BigDecimal &n,BigDecimal &e,BigDecimal &d,const unsigned int time=20);
		
		//RSA加密或解密(针对大数，加密、解密为相同算法),实际就是模取幂的过程.
		//O(N^3),N为参数对应十进制数字的最大位数。参见《算法导论p540-544》
		static BigDecimal cryptRSA(const BigDecimal &msg,const BigDecimal &e_d,const BigDecimal &n);

		//RSA加密或解密(针对字符串，加密、解密为相同算法),实际就是模取幂的过程.
		//O(N^3),N为参数对应十进制数字的最大位数。参见《算法导论p540-544》
		static std::string cryptRSA(const std::string &msg,const BigDecimal &e_d,const BigDecimal &n);

	private:
		//用于MillerRabin算法的合数见证。
		static bool MillerRabin_witness(const BigDecimal &a,const BigDecimal &n);
	};
}
#endif