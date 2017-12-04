#ifndef geometryConst
#define geometryConst
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
#include "redBlackTree.h"
//计算几何方面的算法
namespace lmtc{

	//平面点类
	class Point{
	public://构造函数
		Point():x(0),y(0){}//默认构造为原点
		Point(double _x,double _y):x(_x),y(_y){}
	public:
		double x;
		double y;
	public:
		bool operator<=(const Point &pt)const{return x<=pt.x&&y<=pt.y;}
	};
	
	//有向线段类
	class Segment{
	public:
		Point st,ed;
	public://构造函数
		Segment():st(),ed(){}//默认为原点零线段
		Segment(const Point &e):st(),ed(e){}//起点为原点线段
		Segment(const Point &s,const Point &e):st(s),ed(e){}
		Segment(double _x,double _y):st(),ed(_x,_y){}//起点为原点线段
		Segment(double _x0,double _y0,double _x1,double _y1):st(_x0,_y0),ed(_x1,_y1){}
	public:
		//相对原点，判断与向量seg的顺逆时针方向。返回1表示在seg的顺时针方向，-1表示在seg的逆时针方向，0表示共线。
		//O(1),参考《算法导论p575-579》
		int clockDirect(const Segment &seg)const{double direct=(ed.x-st.x)*(seg.ed.y-seg.st.y)-(seg.ed.x-seg.st.x)*(ed.y-st.y);if(direct<0) return -1;else if(direct>0)return 1;else return 0;}
		
		//末端点指向seg起点时，判断是到seg是左拐还是右拐。返回1表示右拐，-1表示左拐，0表示共线。
		//O(1),参考《算法导论p575-579》
		int turnDirect(const Segment &seg)const{double direct=(ed.x-st.x+seg.ed.x-seg.st.x)*(ed.y-st.y)-(ed.x-st.x)*(ed.y-st.y+seg.ed.y-seg.st.y);if(direct<0) return -1;else if(direct>0)return 1;else return 0;}
	
		//判断与线段是否相交
		//O(1),参考《算法导论p575-579》
		bool intersect(const Segment &seg)const;

		//此操作符无意义，纯粹出于语法方便。
		bool operator<(const Segment &)const{return false;}
	};

	class Geometry{
	public:
		//判断segVec中是否存在任意线段相交，是则返回true，否则false。
		//O(nlgn),参考《算法导论p580-583》
		static bool anySegmentsIntersect(const std::vector<Segment> &segVec);
		
		//Graham扫描法求凸包，返回psVec点集的点序号向量，该向量中各个点依次相连即为凸包。
		//O(nlgn),参考《算法导论p584-590》
		static std::vector<unsigned int> convexHallGraham(const std::vector<Point> &psVec);
		
		//最近点对算法。寻找psVec中最近的两点，该两点的序号存于p1，p2，并返回该两点的距离。
		//O(nlgn),参考《算法导论p591-593》
		static double closestPointPair(const std::vector<Point> &psVec,unsigned int &p1,unsigned int &p2);

	private:
		static double closestPointPair_visit(const std::vector<Point> &psVec,std::vector<unsigned int> X,std::vector<unsigned int> Y,unsigned int &p1,unsigned int &p2);
		//anySegmentsIntersect和closestPointPair中用于排序端点的比较函数
		static bool left_equal_point(const Point &p1,const Point &p2){if(p1.x<p2.x)return true;else if(p1.x==p2.x&&p1.y<=p2.y)return true;else return false;}
		//closestPointPair中用于排序端点的比较函数
		static bool down_equal_point(const Point &p1,const Point &p2){if(p1.y<p2.y)return true;else if(p1.y==p2.y&&p1.x<=p2.x)return true;else return false;}
		//anySegmentsIntersect中用于红黑树的小于比较符，依赖于扫除线brushLineX的值。
		static bool down_line_compare(const Segment &s1,const Segment &s2);
		//anySegmentsIntersect中扫除线位置，动态变化。
		static double brushLineX;
		//用于convexHallGraham算法的极角比较符。
		static bool clockwise_compare(const Segment &s1,const Segment &s2){int temp=s1.clockDirect(s2);if(temp==1)return true;else if(temp==-1)return false;else if((s1.ed.x-s1.st.x)*(s1.ed.x-s1.st.x)+(s1.ed.y-s1.st.y)*(s1.ed.y-s1.st.y)>(s2.ed.x-s2.st.x)*(s2.ed.x-s2.st.x)+(s2.ed.y-s2.st.y)*(s2.ed.y-s2.st.y))return false;else return true;}
	};

}
#endif