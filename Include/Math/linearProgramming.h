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
//���Թ滮
#include <vector>
#include "array.h"
namespace lmtc{
	class LinearProgramming{
	public:
		//һ���͵ĵ����η�
		//��ismaximize=trueʱ��Ŀ��Ϊ�����V=cX������Ϊ��С��V=cX
		//Լ��ΪAX cmpVec b,cmpVec��Ԫ��Ϊ������ʾ<=,Ϊ������ʾ>=,Ϊ0��ʾ==��
		//�Ǹ�Լ������nonNegVec��Ԫ��Ϊtrue��ʾX�ж�Ӧ�����Ǹ�������ɸ���
		//����-1��ʾ�޽磬0��ʾ�޿��н⣬1��ʾ�ɹ��õ����Ž�(���Ž�ΪX�����Ŀ��ֵΪV) ��
		//�μ����㷨����p477-481��
		static int simplexGeneralForm(const Array<long double> &A,const std::vector<long double> &b,const std::vector<long double> &c,const bool ismaximize,const std::vector<int> &cmpVec,const std::vector<bool> &nonNegVec,std::vector<long double> &X,long double &V);

		//��׼�͵ĵ����η���
		//Ŀ��Ϊ:���V=cX��Լ��Ϊ:AX<=b ��
		//����-1��ʾ�޽磬0��ʾ�޿��н⣬1��ʾ�ɹ��õ����Ž�(���Ž�ΪX�����Ŀ��ֵΪV) ��
		//�μ����㷨����p489-498��
		static int simplexStandardForm(const Array<long double> &A,const std::vector<long double> &b,const std::vector<long double> &c,std::vector<long double> &X,long double &V);
		
		//��֪�ɳ���(N,B,A,b,c,v)�Ļ�������У�����ʹ��v��󻯵������ɳ��͡�
		//����false��ʾ�޽磬true��ʾ�ɹ��õ������ɳ��͡�
		//�μ����㷨����p489-498��
		static bool simplexFeasibleSlackForm(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v);
		
		//�ɱ�׼��(A0,b0,c0)��û�������е��ɳ���(N,B,A,b,c,v)��
		//����false��ʾ�޿��н⣬true��ʾ�ɹ��õ���������е��ɳ��͡�
		//�μ����㷨����p498-502��
		static bool initializeSimplex(const Array<long double> &A0,const std::vector<long double> &b0,const std::vector<long double> &c0,std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v);	
	private:
		//��Ԫ������������l�����������������e���ǻ�������
		//O(m*n)���μ����㷨����p485-489��
		static void pivot(std::vector<unsigned int> &N,std::vector<unsigned int> &B,Array<long double> &A,std::vector<long double> &b,std::vector<long double> &c,long double &v,const unsigned int l,const unsigned int e);

	};

}
#endif