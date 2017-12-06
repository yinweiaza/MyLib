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

#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include "myMath.h"
#include <string>
namespace lmtc{

double averageRandom(const double min,const double max)//����һ��ƽ���ֲ��������������Ϊ0.000001�� ������ò�����10
{ 
	if(max-min<0.00001)
	{
		if(rand()%2==0)
			return min;
		else
			return max;
	}	  
    long minInteger=(long)(min*1000000); 
    long maxInteger=(long)(max*1000000); 
    long randInteger=(long)(rand()*10000+rand()%9999);
    long diffInteger=maxInteger-minInteger; 
    long resultInteger=randInteger%diffInteger+minInteger; 
    return (double)(resultInteger/1000000.0); 
} 

double averageRandom()//����һ��ƽ���ֲ��������,��ΧΪ[0-1)������Ϊ0.000001
{ 
    long randInteger=(long)(rand()*10000+rand()%9999);
    long resultInteger=randInteger%1000000; 
    return (double)(resultInteger/1000000.0); 
} 

BigDecimal averageRandomBigFloat(const unsigned int numDt){//�������ڵ���0С��1��ƽ���ֲ��������С����λ��ΪnumDt
	std::string dec;
	dec.push_back('.');
	for(unsigned int i=0;i<numDt;i++)
		dec.push_back('0'+(int)(10*averageRandom()));
	return BigDecimal(dec);
	
}
BigDecimal averageRandomBigInteger(const unsigned int digitNum){//����ʮ����λ��ΪdigitNum�Ĵ�������
	if(digitNum==0)
		return 0;
	std::string dec;
	dec.push_back('1'+(int)(9*averageRandom()));
	for(unsigned int i=1;i<digitNum;i++)
		dec.push_back('0'+(int)(10*averageRandom()));
	return BigDecimal(dec);
}

}//end namespace lmtc