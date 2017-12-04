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
//�˳�ѧ������㷨
#include "array.h"
#include <vector>
#include "linearProgramming.h"
#include "symmetryArray.h"
namespace lmtc{
	class OperationsResearch{
	public:
		//������������Թ滮�ⷨ�����㷨Ϊ����ԭ�������������㷨��Ч�����������ۼ�ֵ���������㷨������һ�汾��
		//��ά����cost��ʾ�ɱ�����cost(i,j)��ʾ��i��������j������ĳɱ���
		//�ɹ�����true������false���ɹ�ʱ�������assignVec��minCost��minCost��ʾ��С�����ܳɱ���assignVec[i]��ʾ�������i���˵�������š�
		//ʱ�临�Ӷ��൱��n^2��������2n��Լ�������Թ滮�ĸ��Ӷȣ�n��ʾ��������������
		static bool bestAssign(const Array<long double> &cost,std::vector<unsigned int> &assignVec,long double &minCost);

		//���Ŷ�����ͼ�����Թ滮�ⷨ����������ͼ����������Ϊ����������㷨Ϊ����ԭ��������ͼ����������Ⱥͳ��Ƚ�Ϊ1����������ͼ���������Ϊ2�������Ŷ�����ͼ������ԭͼ���ж����һ����ͼ������ͼΪ����ͼ���Ҹö�����ͼ�ĸ���Ȩֵ֮����С����
		//��ά����ajacencyMatrix��ʾͼ�ı�Ȩֵ����ajacencyMatrix(i,j)��ʾ�����i->j��Ȩֵ������ͼʱajacencyMatrix��Ϊ�Գƾ���
		//�ɹ�����true������false���ɹ�ʱ�������connectVec��minLen��minLen��ʾ������ͼ����СȨֵ�ͣ�connectVec[i]��ʾ�����i->connectVec[i]������ͼʱ��ʾ�����i-connectVec[i]�����ڸö�����ͼ��
		//ʱ�临�Ӷ��൱��n^2��������2n��Լ�������Թ滮�ĸ��Ӷȣ�n��ʾͼ�Ķ�������
		static bool bestTwoDegreeSubGraph(const Array<long double> &ajacencyMatrix,std::vector<unsigned int> &connectVec,long double &minLen);
		
		//��01���������α����ʽ�任�㷨�������ڽ���С��ģ�Ĳ��е������⡣���㷨Ϊ���������
		//���룺pack�����˸������ĳ��أ�weight�����˸���Ʒ��������value��������Ʒ�ļ�ֵ��
		//������Щ�����������ɵ���Ʒ������ܼ�ֵ��packAssigned[i]��ʾ��i����Ʒ��װ��ı�����ţ�packAssigned[i]ֵΪ-1ʱ��ʾ��װ���κα�����
		//ʱ����ռ临�Ӷ�Ϊ��O(C(m+w,m)*n)��n��ʾ��Ʒ��Ŀ��m��ʾ������Ŀ��w��ʾ����ƽ�����أ�CΪ��ϼǺš�
		static double multi_01_package(const std::vector<unsigned int> &pack,const std::vector<unsigned int> &weight,const std::vector<double> &value,std::vector<int> &packAssigned);
		
		//��С���������⣬������һ�汾

		//����Ʒ�����⣬������һ�汾

	private:
		static double multi_01_package_visit(SymmetryArray<std::vector<double> > &V,std::vector<unsigned int> &pack,unsigned int k,const std::vector<unsigned int> &weight,const std::vector<double> &value);
	};
	
}
#endif