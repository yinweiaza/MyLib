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
#include "geometry.h"
#include "mySort.h"
#include "myException.h"
#include <algorithm>
#include <numeric>
#include <cmath>
namespace lmtc{

	//判断与线段是否相交
	bool Segment::intersect(const Segment &seg)const{
		int d1=seg.clockDirect(Segment(seg.st,st));
		int d2=seg.clockDirect(Segment(seg.st,ed));
		int d3=clockDirect(Segment(st,seg.st));
		int d4=clockDirect(Segment(st,seg.ed));
		if(d1*d2<0&&d3*d4<0)
			return true;
		else if(d1==0&&st.x<=std::max((double)seg.st.x,(double)seg.ed.x)&&st.x>=std::min(seg.st.x,seg.ed.x)&&st.y<=std::max(seg.st.y,seg.ed.y)&&st.y>=std::min(seg.st.y,seg.ed.y))
			return true;
		else if(d2==0&&ed.x<=std::max(seg.st.x,seg.ed.x)&&ed.x>=std::min(seg.st.x,seg.ed.x)&&ed.y<=std::max(seg.st.y,seg.ed.y)&&ed.y>=std::min(seg.st.y,seg.ed.y))
			return true;
		else if(d3==0&&seg.st.x<=std::max(st.x,ed.x)&&seg.st.x>=std::min(st.x,ed.x)&&seg.st.y<=std::max(st.y,ed.y)&&seg.st.y>=std::min(st.y,ed.y))
			return true;
		else if(d4==0&&seg.ed.x<=std::max(st.x,ed.x)&&seg.ed.x>=std::min(st.x,ed.x)&&seg.ed.y<=std::max(st.y,ed.y)&&seg.ed.y>=std::min(st.y,ed.y))
			return true;
		else
			return false;
	}

	bool Geometry::anySegmentsIntersect(const std::vector<Segment> &segVec){
		std::vector<unsigned int> leftIndex(segVec.size());
		std::vector<unsigned int> rightIndex(segVec.size());
		std::vector<Point> leftPts(segVec.size());
		std::vector<Point> rightPts(segVec.size());
		std::vector<Segment> segVecStd(segVec.size());
		for(unsigned int i=0;i<segVec.size();i++){//设置左、右端点序号，获取左、右端点集合，以及修正线段集合中错位的左右端点
			leftIndex[i]=2*i;
			rightIndex[i]=2*i+1;
			if(segVec[i].st.x<=segVec[i].ed.x){
				leftPts[i]=segVec[i].st;
				rightPts[i]=segVec[i].ed;
				segVecStd[i]=segVec[i];
			}else{
				leftPts[i]=segVec[i].ed;
				rightPts[i]=segVec[i].st;
				segVecStd[i]=Segment(leftPts[i],rightPts[i]);
			}
		}
		sort(leftIndex,leftPts,left_equal_point);//对左端点序号排序
		sort(rightIndex,rightPts,left_equal_point);//对右端点序号排序
		std::vector<unsigned int> index;//合并后的排序
		unsigned int i=0,j=0;
		while(i<leftIndex.size()||j<rightIndex.size()){
			if(i>=leftIndex.size())
				index.push_back(rightIndex[j++]);
			else if(j>=rightIndex.size())
				index.push_back(leftIndex[i++]);
			else if(leftPts[leftIndex[i]/2].x<=rightPts[rightIndex[j]/2].x){
				index.push_back(leftIndex[i]);
				i++;
			}
			else{ 
				index.push_back(rightIndex[j]);
				j++;
			}
		}
		std::vector<RedBlackTree<Segment>::ItemType> segPtrVec(segVecStd.size());
		RedBlackTree<Segment> T(down_line_compare);
		for(unsigned int i=0;i<index.size();i++){//利用红黑树进行扫描
			unsigned s=index[i]/2;//线段序号
			if(index[i]%2==0){//左端点
				brushLineX=segVecStd[s].st.x;//更新扫除线
				segPtrVec[s]=T.insert(segVecStd[s]);
				RedBlackTree<Segment>::ItemType down=T.predecessor(segPtrVec[s]);
				if(!down.isEmpty()&&segVecStd[s].intersect(T.getItemValue(down)))
					return true;
				RedBlackTree<Segment>::ItemType above=T.successor(segPtrVec[s]);
				if(!above.isEmpty()&&segVecStd[s].intersect(T.getItemValue(above)))
					return true;
			}else{//右端点
				brushLineX=segVecStd[s].ed.x;//更新扫除线
				RedBlackTree<Segment>::ItemType down=T.predecessor(segPtrVec[s]);
				RedBlackTree<Segment>::ItemType above=T.successor(segPtrVec[s]);
				if(!down.isEmpty()&&!above.isEmpty()&&T.getItemValue(down).intersect(T.getItemValue(above)))
					return true;
				T.deleteItem(segPtrVec[s]);
			}
		}
		return false;
	}

	bool Geometry::down_line_compare(const Segment &s1,const Segment &s2){
		double y1=s1.st.y+(s1.ed.y-s1.st.y)*(brushLineX-s1.st.x)/(s1.ed.x-s1.st.x);
		double y2=s2.st.y+(s2.ed.y-s2.st.y)*(brushLineX-s2.st.x)/(s2.ed.x-s2.st.x);
		if(y1<y2)
			return true;
		else 
			return false;
	}

	std::vector<unsigned int> Geometry::convexHallGraham(const std::vector<Point> &psVec){
		if(psVec.size()<=3){
			std::vector<unsigned int> ordVec;
			for(unsigned int i=0;i<psVec.size();i++)
				ordVec.push_back(i);
			return ordVec;
		}
		int c=0;
		for(unsigned int i=1;i<psVec.size();i++){//寻找最低点(多个同时最低时，选择最左的）
			if(psVec[i].y<psVec[c].y||psVec[i].y==psVec[c].y&&psVec[i].x<psVec[c].x)
				c=i;
		}
		std::vector<Segment> segVec;
		std::vector<unsigned int> ordVec;
		for(unsigned int i=0;i<psVec.size();i++)//对以最低点为起点的各条线段排序
			if(i!=c){
				segVec.push_back(Segment(psVec[c],psVec[i]));
				ordVec.push_back(i);
			}
		sort(ordVec,segVec,clockwise_compare);
		std::vector<unsigned int> S(psVec.size());//用数组模拟栈
		S[0]=c;
		S[1]=ordVec[0];
		S[2]=ordVec[1];
		int top=3;
		for(unsigned int i=2;i<ordVec.size();i++){
			while(top>1&&Segment(psVec[S[top-2]],psVec[S[top-1]]).turnDirect(Segment(psVec[S[top-1]],psVec[ordVec[i]]))>=0)
				top--;//出栈
			S[top++]=ordVec[i];//入栈
		}
		S.resize(top);//栈中有效元素即为凸包。
		return S;
	}

	double Geometry::closestPointPair(const std::vector<Point> &psVec,unsigned int &p1,unsigned int &p2){
		if(psVec.size()<2)
			throw GeometryException("异常：closestPointPair的输入点集规模不能小于2");
		std::vector<unsigned int> X;
		std::vector<unsigned int> Y;
		for(unsigned int i=0;i<psVec.size();i++){//获得X、Y的初始有序序列
			X.push_back(i);
			Y.push_back(i);
		}
		sort(X,psVec,left_equal_point);
		sort(Y,psVec,down_equal_point);
		return closestPointPair_visit(psVec,X,Y,p1,p2);//调用递归子程序
	}

	double Geometry::closestPointPair_visit(const std::vector<Point> &psVec,std::vector<unsigned int> X,std::vector<unsigned int> Y,unsigned int &p1,unsigned int &p2){
		if(X.size()==2){//两个点时，直接计算
			p1=X[0];
			p2=X[1];
			return std::pow((psVec[p1].x-psVec[p2].x)*(psVec[p1].x-psVec[p2].x)+(psVec[p1].y-psVec[p2].y)*(psVec[p1].y-psVec[p2].y),0.5);
		}
		if(X.size()==3){//三个点时，三种情况选择一种
			p1=X[0];
			p2=X[1];
			double min=std::pow((psVec[p1].x-psVec[p2].x)*(psVec[p1].x-psVec[p2].x)+(psVec[p1].y-psVec[p2].y)*(psVec[p1].y-psVec[p2].y),0.5);
			double tem=std::pow((psVec[X[0]].x-psVec[X[2]].x)*(psVec[X[0]].x-psVec[X[2]].x)+(psVec[X[0]].y-psVec[X[2]].y)*(psVec[X[0]].y-psVec[X[2]].y),0.5);
			if(tem<min){
				min=tem;
				p1=X[0];
				p2=X[2];
			}
			tem=std::pow((psVec[X[1]].x-psVec[X[2]].x)*(psVec[X[1]].x-psVec[X[2]].x)+(psVec[X[1]].y-psVec[X[2]].y)*(psVec[X[1]].y-psVec[X[2]].y),0.5);
			if(tem<min){
				min=tem;
				p1=X[1];
				p2=X[2];
			}
			return min;
		}
		std::vector<unsigned int> XL(X.begin(),X.begin()+X.size()/2);//左半边的X
		std::vector<unsigned int> XR(X.begin()+X.size()/2,X.end());//右半边的X
		std::vector<unsigned int> YL;
		std::vector<unsigned int> YR;
		for(unsigned int i=0;i<Y.size();i++){//将Y分离为YL、YR
			if(YL.size()<=XL.size()&&left_equal_point(psVec[Y[i]],psVec[XL.back()]))   
				YL.push_back(Y[i]);
			else
				YR.push_back(Y[i]);
		}
		unsigned int pL1,pL2,pR1,pR2;
		double min1,min2;
		min1=closestPointPair_visit(psVec,XL,YL,pL1,pL2);//递归计算左半部分
		min2=closestPointPair_visit(psVec,XR,YR,pR1,pR2);//递归计算右半部分

		double min;//选择左右两边的最近点对中的较近者
		if(min1<=min2){
			min=min1;
			p1=pL1;
			p2=pL2;
		}else{
			min=min2;
			p1=pR1;
			p2=pR2;
		}
		std::vector<unsigned int> Ym;
		for(unsigned int i=0;i<Y.size();i++){//抽取距中轴距离小于min的Y的子序列
			if(std::abs(psVec[Y[i]].x-psVec[XL.back()].x)<min)
				Ym.push_back(Y[i]);
		}
		for(unsigned int i=0;i<Ym.size();i++){//利用Ym搜索在中轴附近是否存在更近点对
			for(unsigned int j=i+1;j<i+8&&j<Ym.size();j++){
				double tem=std::pow((psVec[Ym[i]].x-psVec[Ym[j]].x)*(psVec[Ym[i]].x-psVec[Ym[j]].x)+(psVec[Ym[i]].y-psVec[Ym[j]].y)*(psVec[Ym[i]].y-psVec[Ym[j]].y),0.5);
				if(tem<min){
					min=tem;
					p1=Ym[i];
					p2=Ym[j];
				}
			}
		}
		return min;
	}

	double Geometry::brushLineX;

}
