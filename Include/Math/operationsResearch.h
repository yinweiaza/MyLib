#ifndef operationsResearchConst
#define operationsResearchConst
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
//运筹学方面的算法
#include "array.h"
#include <vector>
#include "linearProgramming.h"
#include "symmetryArray.h"
namespace lmtc{
	class OperationsResearch{
	public:
		//分配问题的线性规划解法，该算法为本人原创，比匈牙利算法低效，但具有理论价值。匈牙利算法留给下一版本。
		//二维数组cost表示成本矩阵，cost(i,j)表示第i个人做第j个任务的成本。
		//成功返回true，否则false，成功时结果存于assignVec、minCost，minCost表示最小分配总成本，assignVec[i]表示分配给第i个人的任务序号。
		//时间复杂度相当于n^2个变量，2n个约束的线性规划的复杂度，n表示人数或任务数。
		static bool bestAssign(const Array<long double> &cost,std::vector<unsigned int> &assignVec,long double &minCost);

		//最优二度子图的线性规划解法（兼容有向图），该问题为本人提出，算法为本人原创。二度图即各顶点入度和出度皆为1（对于无向图即各顶点度为2），最优二度子图即包含原图所有顶点的一个子图，该子图为二度图，且该二度子图的各边权值之和最小化。
		//二维数组ajacencyMatrix表示图的边权值矩阵，ajacencyMatrix(i,j)表示有向边i->j的权值，无向图时ajacencyMatrix即为对称矩阵。
		//成功返回true，否则false，成功时结果存于connectVec、minLen，minLen表示二度子图的最小权值和，connectVec[i]表示有向边i->connectVec[i]（无向图时表示无向边i-connectVec[i]）属于该二度子图。
		//时间复杂度相当于n^2个变量，2n个约束的线性规划的复杂度，n表示图的顶点数。
		static bool bestTwoDegreeSubGraph(const Array<long double> &ajacencyMatrix,std::vector<unsigned int> &connectVec,long double &minLen);
		
		//多01背包问题的伪多项式变换算法，可用于解中小规模的并行调度问题。此算法为本人提出。
		//输入：pack给出了各背包的承重，weight给出了各物品的重量，value给出各物品的价值。
		//返回这些背包所能容纳的物品的最大总价值，packAssigned[i]表示第i个物品需装入的背包序号，packAssigned[i]值为-1时表示不装入任何背包。
		//时间与空间复杂度为：O(C(m+w,m)*n)，n表示物品数目，m表示背包数目，w表示背包平均承重，C为组合记号。
		static double multi_01_package(const std::vector<unsigned int> &pack,const std::vector<unsigned int> &weight,const std::vector<double> &value,std::vector<int> &packAssigned);
		
		//最小费用流问题，留给下一版本

		//多商品流问题，留给下一版本

	private:
		static double multi_01_package_visit(SymmetryArray<std::vector<double> > &V,std::vector<unsigned int> &pack,unsigned int k,const std::vector<unsigned int> &weight,const std::vector<double> &value);
	};
	
}
#endif