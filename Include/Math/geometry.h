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
//���㼸�η�����㷨
namespace lmtc{

	//ƽ�����
	class Point{
	public://���캯��
		Point():x(0),y(0){}//Ĭ�Ϲ���Ϊԭ��
		Point(double _x,double _y):x(_x),y(_y){}
	public:
		double x;
		double y;
	public:
		bool operator<=(const Point &pt)const{return x<=pt.x&&y<=pt.y;}
	};
	
	//�����߶���
	class Segment{
	public:
		Point st,ed;
	public://���캯��
		Segment():st(),ed(){}//Ĭ��Ϊԭ�����߶�
		Segment(const Point &e):st(),ed(e){}//���Ϊԭ���߶�
		Segment(const Point &s,const Point &e):st(s),ed(e){}
		Segment(double _x,double _y):st(),ed(_x,_y){}//���Ϊԭ���߶�
		Segment(double _x0,double _y0,double _x1,double _y1):st(_x0,_y0),ed(_x1,_y1){}
	public:
		//���ԭ�㣬�ж�������seg��˳��ʱ�뷽�򡣷���1��ʾ��seg��˳ʱ�뷽��-1��ʾ��seg����ʱ�뷽��0��ʾ���ߡ�
		//O(1),�ο����㷨����p575-579��
		int clockDirect(const Segment &seg)const{double direct=(ed.x-st.x)*(seg.ed.y-seg.st.y)-(seg.ed.x-seg.st.x)*(ed.y-st.y);if(direct<0) return -1;else if(direct>0)return 1;else return 0;}
		
		//ĩ�˵�ָ��seg���ʱ���ж��ǵ�seg����ջ����ҹա�����1��ʾ�ҹգ�-1��ʾ��գ�0��ʾ���ߡ�
		//O(1),�ο����㷨����p575-579��
		int turnDirect(const Segment &seg)const{double direct=(ed.x-st.x+seg.ed.x-seg.st.x)*(ed.y-st.y)-(ed.x-st.x)*(ed.y-st.y+seg.ed.y-seg.st.y);if(direct<0) return -1;else if(direct>0)return 1;else return 0;}
	
		//�ж����߶��Ƿ��ཻ
		//O(1),�ο����㷨����p575-579��
		bool intersect(const Segment &seg)const;

		//�˲����������壬��������﷨���㡣
		bool operator<(const Segment &)const{return false;}
	};

	class Geometry{
	public:
		//�ж�segVec���Ƿ���������߶��ཻ�����򷵻�true������false��
		//O(nlgn),�ο����㷨����p580-583��
		static bool anySegmentsIntersect(const std::vector<Segment> &segVec);
		
		//Grahamɨ�跨��͹��������psVec�㼯�ĵ�����������������и���������������Ϊ͹����
		//O(nlgn),�ο����㷨����p584-590��
		static std::vector<unsigned int> convexHallGraham(const std::vector<Point> &psVec);
		
		//�������㷨��Ѱ��psVec����������㣬���������Ŵ���p1��p2�������ظ�����ľ��롣
		//O(nlgn),�ο����㷨����p591-593��
		static double closestPointPair(const std::vector<Point> &psVec,unsigned int &p1,unsigned int &p2);

	private:
		static double closestPointPair_visit(const std::vector<Point> &psVec,std::vector<unsigned int> X,std::vector<unsigned int> Y,unsigned int &p1,unsigned int &p2);
		//anySegmentsIntersect��closestPointPair����������˵�ıȽϺ���
		static bool left_equal_point(const Point &p1,const Point &p2){if(p1.x<p2.x)return true;else if(p1.x==p2.x&&p1.y<=p2.y)return true;else return false;}
		//closestPointPair����������˵�ıȽϺ���
		static bool down_equal_point(const Point &p1,const Point &p2){if(p1.y<p2.y)return true;else if(p1.y==p2.y&&p1.x<=p2.x)return true;else return false;}
		//anySegmentsIntersect�����ں������С�ڱȽϷ���������ɨ����brushLineX��ֵ��
		static bool down_line_compare(const Segment &s1,const Segment &s2);
		//anySegmentsIntersect��ɨ����λ�ã���̬�仯��
		static double brushLineX;
		//����convexHallGraham�㷨�ļ��ǱȽϷ���
		static bool clockwise_compare(const Segment &s1,const Segment &s2){int temp=s1.clockDirect(s2);if(temp==1)return true;else if(temp==-1)return false;else if((s1.ed.x-s1.st.x)*(s1.ed.x-s1.st.x)+(s1.ed.y-s1.st.y)*(s1.ed.y-s1.st.y)>(s2.ed.x-s2.st.x)*(s2.ed.x-s2.st.x)+(s2.ed.y-s2.st.y)*(s2.ed.y-s2.st.y))return false;else return true;}
	};

}
#endif