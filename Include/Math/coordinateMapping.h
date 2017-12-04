#ifndef coordinateMappingConst
#define coordinateMappingConst
//���ļ�����ʵ�ֶԳ�����SymmetryArray,���߿��Թ���
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

#include "array.h"
namespace lmtc{
class CrossFace {
public:
    //����Sp��m��limit����w�Խ���Գ�Ԫ������array[X,Y,Z]Ϊ�������飬X>=m,Y>=limit,Z>=w;
	long crossFace(long m,long limit,long w,Array<long> &arr)const;

	//����Sp��m��limit����W�Խ��漰֮ǰԪ����
	long sumNumofT(long m,long limit,long W)const;

	//��ʼ��numInCroF����
	long initNumInCroF(long m,long limit,long W,Array<long> &numInCroF)const;

	//��ʼ��numBfCroF����
	long initNumBfCroF(long m,long limit,long W,Array<long> &numInCroF,Array<long> &numBfCroF)const;

	//���������C��m��n�� arryΪ�����������Ϊnull��
	long combination(long m,long n,Array<long> &arry)const;

private:
	//���������C��m��n�� arryΪ�������鲻����Ϊnull�������鲻С��[m,n]��
	long combination1(long m,long n,Array<long> &arry1)const;	
};


class StandMapping {
	
public: //���ýӿ�

	//����dmNumά������Ϊlimit�ı�׼�Գƿռ�
	StandMapping(long dmNum,long limit);

	//Ĭ�Ϲ��캯��
	StandMapping():dm(0),limit(0),W(0){};
	
  //����ӳ�䣬���뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w�����Խ���ֵ�������У�Ȼ����ͬ�Խ���������������ȼ��ĸ�ά�����������С�
    long forwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬         ����ά��    ���Խ��棬������������λ�ã�
	(long dmNum,long limit,Array<long>&symPoint,long pointDmNum,long w,long st)const;
    
  //����ӳ�䣬���뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w�����Խ���ֵ�������У�Ȼ����ͬ�Խ���������������ȼ��ĸ�ά�����������С�
    long forwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬         ����ά��    ���Խ��棬������������λ�ã���������С��������
	(long dmNum,long limit,Array<long>&symPoint,long pointDmNum,long w,long st,long base)const;
   
    //����ӳ����뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w��
    long bkwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬  �Խ��� �� ����ά��  ����ӳ����ţ�  ������ĩ����λ�ã�dmNumÿά��ֵ��
	(long dmNum,long limit,Array<long> &symPoint,long f, long dm,    long num,    long end,  long offset)const;

	//����Խ����ض�ά����������ĳ�ԳƵ�֮��Ԫ���� 
   long forwdMappingIJ
   //��  ��ӳ�����꣬           ����ά�� ��  ���� ��   �Խ��� ��  ������������λ�ã���������С��������
	(Array<long> &symPoint,long pointDmNum,long limit,long w,long st,long base)const;

public://�ǳ��ýӿ�

    //�۰����Ѱ��sp��dmNum��limit����num�ԳƵ����ڶԽ���
    long findFace(long dmNum,long limit,long num)const;

	//�۰����sp��dmNum��limit����f�Խ������Ϊnum�ĶԳƵ��ά����
     long findDm(long dmNum,long f,long limit,long num)const;
     
    //��sp��dm��limit����f�Խ���dmά�����۰���Ҵ��ڵ���minItem����Ԫ����֮��Ԫ����Ϊback��
    long findFirstItem(long dm,long f,long limit,long minItem,long back)const;

    //���������C��m��n��array[X,Y]Ϊ������������X>=m+n,Y>=n
    long combination(long m,long n,Array<long> &arr)const;

    long numInCroF(long dm,long limit,long w)const;
    long numBfCroF(long dm,long limit,long w)const;

private:

	//���öԳƿռ䣨����ά���ͽ��޸ı�����numInCroFArr��numBfCroFArr
    void setArray(long dmNum,long limit);

private:
	Array<long> numInCroFArr;
	Array<long> numBfCroFArr;
	long dm;
	long limit;
	long W;
	CrossFace cross;
};


class UnsymMapping {

public:
	//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
	UnsymMapping(Array<long> &dmAr);

	//����ӳ�䣬��dmά����point�������
    long forwdMapping(Array<long> &point,long st)const;

    //����ӳ�䣬�����pn�õ�dmά���ꡣ
    void bkwdMapping(long pn,Array<long> &point,long st )const;

private:
	
	//����ά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
	long setDm(Array<long> &dmAr);

private:
	long dm;//�ǹ����Գƿռ�ά��
	Array<long> bd; //bd[0]�洢��ά����bd[i>0]��ʾiά����+1
    Array<long> bkDmMu;//bkDmMu[i]��ʾiά���Ժ�ά����ӿռ��С����bd[i]*bd[i+1]*......*bd[dm]
};


class UnTrimMapping {	//���׼�Ĺ���ӳ�䲻����
public:

	//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
	UnTrimMapping(Array<long> &dmAr) ;

	//Ĭ�Ϲ��캯��
	UnTrimMapping(){}
	
	//�ǹ�������ӳ�䣬������ά���������У�Ȼ����ͬ����ά�����Խ���ֵ�������У�Ȼ��������������ȼ��ĸ�ά�����������С�
	//st��ʾ��ͷ���ά��ȫ������ֵΪdm��symPoint(0--dm-1)��ʾ��iά����ֵ��Ҫ��ǽ������С�
	//�����������
	long forwdMapping(long i,long j, Array<long> &symPoint,long st)const;
	
	//�ǹ�������ӳ��
	//�ڣ�i��j�����·��ӿռ併���n��ӳ�䣬����symPoint����jλ����ǰ��
	//���ط������ά
	long bkwdMapping(long i,long j,Array<long> &symPoint,long n)const;

	//������������Ŀ
	long countN()const;

	//����ˮƽ�ָ�����
	long getbm()const;
	long getdm()const{return dm;}

private:
	//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
	long initArray(Array<long> &dmAr);

	//���أ�i��j�����·�������Գ�Ԫ����
	long countSunPro(long i,long j)const;

	long findU(long i,long j,long n)const;
	long findV(long i,long j,long u,long n)const;

private:
	Array<long> boundVal;
    Array<long> boundBegin;
    Array<long> sumNumOfSunSp;
    Array<long> sumNumOfSunPro;
    Array<long> sumNumBfSunPro;
    Array<long> cubeTr;
    long dm;//�ǹ����Գƿռ�ά��
	Array<long> bd;//bd[0]�洢��ά����bd[i>0]��ʾiά����
	long bm;//ˮƽ�ָ�����
	long maxBound;//�������ˮƽ���޲�
	Array<long> limitBetwBd;
	StandMapping stMap;
};


}

#endif
      