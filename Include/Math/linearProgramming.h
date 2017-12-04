#ifndef linearProgrammingConst
#define linearProgrammingConst
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
//线性规划
#include <vector>
#include "array.h"
namespace lmtc{
	class LinearProgramming{
	public:
		//一般型的单纯形法
		//当ismaximize=true时，目标为：最大化V=cX，否则为最小化V=cX
		//约束为AX cmpVec b,cmpVec中元素为负数表示<=,为正数表示>=,为0表示==。
		//非负约束向量nonNegVec中元素为true表示X中对应变量非负，否则可负。
		//返回-1表示无界，0表示无可行解，1表示成功得到最优解(最优解为X，最大化目标值为V) 。
		//参见《算法导论p477-481》
		static int simplexGeneralForm(const Array<long double> &A,const std::vector<long double> &b,const std::vector<long double> &c,const bool ismaximize,const std::vector<int> &cmpVec,const std::vector<bool> &nonNegVec,std::vector<long double> &X,long double &V);

		//标准型的单纯形法。
		//目标为:最大化V=cX，约束为:AX<=b 。
		//返回-1表示无界，0表示无可行解，1表示成功得到最优解(最优解为X，最大化目标值为V) 。
		//参见《算法导论p489-498》
		static int simplexStandardForm(const Array<long double> &A,const std::vector<long double> &b,const std::vector<long double> &c,std::vector<long double> &X,long double &V);
		
		//已知松弛型(N,B,A,b,c,v)的基本解可行，计算使得v最大化的最终松弛型。
		//返回false表示无界，true表示成功得到最终松弛型。
		//参见《算法导论p489-498》
		static bool simplexFeasibleSlackForm(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v);
		
		//由标准型(A0,b0,c0)获得基本解可行的松弛型(N,B,A,b,c,v)。
		//返回false表示无可行解，true表示成功得到基本解可行的松弛型。
		//参见《算法导论p498-502》
		static bool initializeSimplex(const Array<long double> &A0,const std::vector<long double> &b0,const std::vector<long double> &c0,std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v);	
	private:
		//主元操作，换出第l个基本变量，换入第e个非基本变量
		//O(m*n)，参见《算法导论p485-489》
		static void pivot(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v,const unsigned int l,const unsigned int e);

	};

}
#endif