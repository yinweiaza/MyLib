#ifndef myMathConst
#define myMathConst
/*//Copyright Statements:
	This source code file is completed as part of the Open SAL(Open Standardized Algorithm 
Library) by Ming Li during the summer vocation of 2013 at College of Information Science 
and Engineering, Central South University. All rights reserved. No part of these files may
be reproduced or transmitted in any form or by any means without permission from the author.
While, you are permitted to download this Library and its instruction manual freely at 
http://www.oschina.net/code/snippet_1259068_24335# and use it by yourself. 
	If you find some bugs or problems of this Library,you are welcomed to email me(limingcsu
@foxmail.com). If you want to know any details or updates about this Library or would like
to discuss issues about algorithms with enthusiasts,you can apply to join the QQ group(222576709).
*///
#include "number.h"
namespace lmtc{
double averageRandom(const double min,const double max);//����һ��ƽ���ֲ��������������Ϊ0.000001�� ������ò�����10
double averageRandom();//����һ������0��1��ƽ���ֲ��������������Ϊ0.000001
BigDecimal averageRandomBigFloat(const unsigned int numDt);//�������ڵ���0С��1��ƽ���ֲ��������С����λ��ΪnumDt
BigDecimal averageRandomBigInteger(const unsigned int digitNum);//����ʮ����λ��ΪdigitNum�������������

}
#endif