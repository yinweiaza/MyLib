#ifndef matroidConst
#define matroidConst
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
#include <iostream>
#include "mySort.h"
#include "myException.h"
/*拟阵模板类，主要是利用加权拟阵的最大权值独立子集贪婪算法来通用地解决一大类贪婪问题
*/
namespace lmtc{

template<typename WeightType,typename ItemType,typename SetType>
class Matroid{
public:
	template<typename WeightIterator,typename ItemIterator>
	Matroid(WeightIterator beg1,WeightIterator end1,ItemIterator beg2,ItemIterator end2,bool (*can_expand_to)(const ItemType &item,const SetType &st),void (*expand_to)(const ItemType &item,SetType &st)):
	weightVec(beg1,end1),itemVec(beg2,end2),canExpandTo(can_expand_to),expandTo(expand_to){
		if(weightVec.size()!=itemVec.size()){
			//std::cout<<"拟阵权重向量大小不等于拟阵元素向量大小"<<std::endl;
			throw MatroidException("拟阵权重向量大小不等于拟阵元素向量大小");
		}
	}

public:
	 WeightType getGreatestWeightIndependentSet(SetType &A)const;
private:
	std::vector<WeightType> weightVec;
	std::vector<ItemType> itemVec;
	bool (*canExpandTo)(const ItemType &item,const SetType &st);
	void (*expandTo)(const ItemType &item,SetType &st);
};

template<typename WeightType,typename ItemType,typename SetType>
WeightType Matroid<typename WeightType,typename ItemType,typename SetType>::getGreatestWeightIndependentSet(SetType &A)const{
	for(std::vector<WeightType>::const_iterator p=weightVec.begin();p!=weightVec.end();p++)
		if((p+1)!=weightVec.end()&&(*p)>(*(p+1))){
			std::vector<unsigned int> orderedSeq;
			for(unsigned int i=0;i<weightVec.size();i++)
				orderedSeq.push_back(i);
			mergeSort(orderedSeq,weightVec,0,weightVec.size()-1);
			std::vector<ItemType> itemOrderedVec;
			for(unsigned int i=0;i<itemVec.size();i++)
				itemOrderedVec.push_back(itemVec[orderedSeq[i]]);

			std::vector<WeightType> weightOrderedVec(weightVec);
			std::sort(weightOrderedVec.begin(),weightOrderedVec.end());

			WeightType W=0;
			for(unsigned int i=weightOrderedVec.size();i>0;i--){
				if(canExpandTo(itemOrderedVec[i-1],A)){
					expandTo(itemOrderedVec[i-1],A);
					W+=weightOrderedVec[i-1];
				}
			}
			return W;
		}
	
	WeightType W=0;
	for(unsigned int i=weightVec.size();i>0;i--){
		if(canExpandTo(itemVec[i-1],A)){
			expandTo(itemVec[i-1],A);
			W+=weightVec[i-1];
		}
	}
	return W;
}

}
#endif