#ifndef sequenceConst
#define sequenceConst
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

#include <vector>
#include "array.h"
#include "defaultCompare.h"
/*������ص��㷨
*/
namespace lmtc{

//����������С�����̬�滮�㷨������v1��v2������������С�
//O(v1.size()*v2.size())
template<typename T>
std::vector<T> longestCommonSubsequence(const std::vector<T> &v1,const std::vector<T> &v2,bool (*equal)(const T &,const T &)=lmtc::equal_default){
	std::vector<T> vc;//�������д洢����
	if(v1.size()==0||v2.size()==0)
		return vc;
	lmtc::Array<int> arr(2,v1.size()+1,v2.size()+1);
	//��̬�滮����
	for(unsigned int i=1;i<=v1.size();i++)
		for(unsigned int j=1;j<=v2.size();j++)
			if(equal(v1[i-1],v2[j-1]))
				arr(i,j)=arr(i-1,j-1)+1;
			else
				arr(i,j)=std::max(arr(i,j-1),arr(i-1,j));
	//�������
	unsigned int i=v1.size(),j=v2.size();
	while(i!=0&&j!=0){
		if(equal(v1[i-1],v2[j-1])){
			vc.push_back(v1[i-1]);
			i--;
			j--;
		}
		else if(arr(i,j)==arr(i,j-1))
			j--;
		else i--;
	}
	std::reverse(vc.begin(),vc.end());
	return vc;
}
//���ܵ������Ĳ����İ汾
template<typename T,typename Iterator1,typename Iterator2>
std::vector<T> longestCommonSubsequence(Iterator1 beg1,Iterator1 end1,Iterator2 beg2,Iterator2 end2,bool (*equal)(const T &,const T &)=lmtc::equal_default){
	std::vector<T> v1(beg1,end1);
	std::vector<T> v2(beg2,end2);
	return longestCommonSubsequence(v1,v2,equal);
}

//����ƥ�䣬KMP�㷨
//T��ʾ��ƥ������Ԫ�صļ������͡�������beg1��end1��ʾ��ƥ�����У�������beg2��end2��ʾģʽ���У�equal��ʾ������Ԫ�ؽ�����ȱȽϵĺ���ָ�룬Ϊ����ʹ��'=='�ȽϷ���
//���ص�һ��ƥ��ʱָ����Ԫ�صĵ�������
//O(end1-beg1 + end2-beg2),�μ����㷨����p568-573��
template<typename T ,typename Iterator1,typename Iterator2>
Iterator1 KMP(const Iterator1 beg1,const Iterator1 end1,const Iterator2 beg2,const Iterator2 end2,bool(*equal)(const T&,const T&)=lmtc::equal_default){
	int patternSize=end2-beg2;//ģʽ�ĳ��ȡ�
	std::vector<int> next(patternSize);
	next[0]=-1;
	int i=0,j=-1;
	while(i<patternSize-1){//��ȡģʽ��next����
		if(j==-1||equal(*(beg2+i),*(beg2+j))){
			i++;
			j++;
			if(!equal(*(beg2+i),*(beg2+j)))
				next[i]=j;
			else
				next[i]=next[j];
		}
		else j=next[j];
	}

	//ģʽƥ��
	int sequenceSize=end1-beg1;
	i=0;j=0;
	while(i<sequenceSize&&j<patternSize){
		if(j==-1||equal(*(beg1+i),*(beg2+j))){
			i++;
			j++;
		}
		else
			j=next[j];
	}
	if(j>=patternSize)
		return beg1+(i-patternSize);
	else 
		return end1;
}


}//end namespace
#endif