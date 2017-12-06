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
#include "operationsResearch.h"
#include "myException.h"
#include <cmath>
namespace lmtc{
	bool OperationsResearch::bestAssign(const Array<long double> &cost,std::vector<unsigned int> &assignVec,long double &minCost){
		if(cost.getDimNum()!=2||cost.getDimLen(0)!=cost.getDimLen(1)||cost.getDimLen(0)==0)
			throw OperationsResearchException("异常：OperationsResearch::bestAssign输入参数cost不符合要求！");
		unsigned int n=cost.getDimLen(0);
		unsigned int N=n*n;
		unsigned int M=n*2;
		Array<long double> A(2,M,N);
		std::vector<long double> b(M,1);
		std::vector<long double> c(N);
		bool ismaximize=false;
		std::vector<int> cmpVec(M,0);
		std::vector<bool> nonNegVec(N,true);
		std::vector<long double> X;
		long double V;
		//初始化目标函数，和约束矩阵
		for(unsigned int r=0;r<n;r++){
			for(unsigned int t=0;t<n;t++){
				c[r*n+t]=cost(r,t);
				A(r,r*n+t)=1;
				A(t+n,r*n+t)=1;
			}
		}
		if(1!=LinearProgramming::simplexGeneralForm(A,b,c,ismaximize,cmpVec,nonNegVec,X,V))
		{
			//std::cout<<"该线性规划无解或无界"<<std::endl;
			return false;//该线性规划无解或无界
		}
		//由线性规划的解构造原分配问题的解。
		minCost=V;
		assignVec.clear();
		assignVec.resize(n);
		for(unsigned int i=0;i<X.size();i++){
			double tmp=std::abs(X[i]-1);
			if(std::abs(X[i])>0.0000000000001&&tmp>0.0000000000001)
				return false;//线性规划不能找到正确解
			if(tmp<=0.0000000000001)
				assignVec[i/n]=i%n;
		}
		return true;
	}

	//最优二度子图的线性规划解法
	bool OperationsResearch::bestTwoDegreeSubGraph(const Array<long double> &ajacencyMatrix,std::vector<unsigned int> &connectVec,long double &minLen){
		if(ajacencyMatrix.getDimNum()!=2||ajacencyMatrix.getDimLen(0)!=ajacencyMatrix.getDimLen(1)||ajacencyMatrix.getDimLen(0)==0)
			throw OperationsResearchException("异常：OperationsResearch::bestTwoDegreeSubGraph输入参数ajacencyMatrix不符合要求！");
		unsigned int n=ajacencyMatrix.getDimLen(0);
		unsigned int N=n*n;
		unsigned int M=n*2;
		Array<long double> A(2,M,N);
		std::vector<long double> b(M,1);
		std::vector<long double> c(N);
		bool ismaximize=false;
		std::vector<int> cmpVec(M,0);
		std::vector<bool> nonNegVec(N,true);
		std::vector<long double> X;
		long double V;
		//初始化目标函数，和约束矩阵
		for(unsigned int r=0;r<n;r++){
			for(unsigned int t=0;t<n;t++){
				if(r==t)
					continue;
				c[r*n+t]=ajacencyMatrix(r,t);
				A(r,r*n+t)=1;
				A(t+n,r*n+t)=1;
			}
		}
		if(1!=LinearProgramming::simplexGeneralForm(A,b,c,ismaximize,cmpVec,nonNegVec,X,V))
		{
			//std::cout<<"该线性规划无解或无界"<<std::endl;
			return false;//该线性规划无解或无界
		}
		//由线性规划的解构造原分配问题的解。
		minLen=V;
		connectVec.clear();
		connectVec.resize(n);
		for(unsigned int i=0;i<X.size();i++){
			if(i/n==i%n)
				continue;
			double tmp=std::abs(X[i]-1);
			if(std::abs(X[i])>0.0000000000001&&tmp>0.0000000000001)
				return false;//线性规划不能找到正确解
			if(tmp<=0.0000000000001)
				connectVec[i/n]=i%n;
		}
		return true;
	}

	double OperationsResearch::multi_01_package(const std::vector<unsigned int> &pack,const std::vector<unsigned int> &weight,const std::vector<double> &value,std::vector<int> &packAssigned){
		if(weight.size()!=value.size())
			throw OperationsResearchException("异常：OperationsResearch::multi_01_package输入参数weight和value尺寸不等！");
		if(pack.size()==0||weight.size()==0){
			packAssigned.assign(weight.size(),-1);
			return 0;
		}
		std::vector<unsigned int> packBd=pack;
		for(unsigned int i=0;i<packBd.size();i++)
			packBd[i]++;
		SymmetryArray<std::vector<double> > V(packBd.size(),packBd);
		V.initialize(std::vector<double>(weight.size()+1,-1));
		std::vector<unsigned int> pack1=pack;
		double val=multi_01_package_visit(V,pack1,weight.size(),weight,value);
		packAssigned.assign(weight.size(),-1);
		for(unsigned int k=weight.size();k>0;k--){
			double temp1=V(pack1)[k];
			if(std::abs(V(pack1)[k-1]-temp1)<0.0000000001)
				continue;
			unsigned int i=0;
			for(;i<pack1.size();i++){
				if(pack1[i]<weight[k-1])
					continue;
				pack1[i]-=weight[k-1];
				if(std::abs(V(pack1)[k-1]+value[k-1]-temp1)<0.0000000001){
					packAssigned[k-1]=i;
					break;
				}
				pack1[i]+=weight[k-1];
			}
			if(i>=pack1.size())
				throw OperationsResearchException("异常：OperationsResearch::multi_01_package反向求解时错误！");
		}
		return val;
	}

	double OperationsResearch::multi_01_package_visit(SymmetryArray<std::vector<double>> &V,std::vector<unsigned int> &pack,unsigned int k,const std::vector<unsigned int> &weight,const std::vector<double> &value){
		double &v=V(pack)[k];
		if(v>=0)
			return v;
		if(k==0){
			v=0;
			return 0;
		}
		double maxValue=multi_01_package_visit(V,pack,k-1,weight,value);
		for(unsigned int i=0;i<pack.size();i++){
			if(pack[i]<weight[k-1])
				continue;
			pack[i]-=weight[k-1];
			double temp=multi_01_package_visit(V,pack,k-1,weight,value)+value[k-1];
			if(temp>maxValue)
				maxValue=temp;
			pack[i]+=weight[k-1];
		}
		v=maxValue;
		return maxValue;
	}

}