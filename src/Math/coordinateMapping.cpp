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

#include "stdafx.h"
#include "coordinateMapping.h"
#include "array.h"
#include <iostream>

namespace lmtc{
    //����Sp��m��limit����w�Խ���Գ�Ԫ������array[X][Y][Z]Ϊ�������飬X>=m,Y>=limit,Z>=w;
	long CrossFace::crossFace(long m,long limit,long w,Array<long> &arr)const{
		if(arr(m,limit,w)!=-1)//֮ǰ�����
			return arr(m,limit,w);
		if(w>m*limit)//�Խ��泬��Χ������
			return arr(m,limit,w)=0;
		if(limit==0)//��ά����Ϊ��
			return arr(m,limit,w)=1;
		if(m==1)//ֻ��һά
			return arr(m,limit,w)=1;
		if(w==0)//0�Խ���
			return arr(m,limit,w)=1;
		long i=w,j=0;
		long sum=0;
		//һ������ݹ����
		while(true){
			if(--i>=0&&++j<=m)sum+=crossFace(j,limit-1,i,arr);
			else break;
		}
		arr(m,limit,w)=sum;
		return sum;
	}

	//����Sp��m��limit����W�Խ��漰֮ǰԪ����
	long CrossFace::sumNumofT(long m,long limit,long W)const{
	    Array<long> arry(3,m+1,limit+1,W+1);
	    long i,j,u;
	    for(i=0;i<=m;i++)
	    	for(j=0;j<=limit;j++)
	    		for(u=0;u<=W;u++)
	    			arry(i,j,u)=-1;
		long sum=0;
		
		for(i=0;i<=W;i++)
		 sum+=crossFace(m,limit,i,arry);
		return sum;
	}

	//��ʼ��numInCroF����
	long CrossFace::initNumInCroF(long m,long limit,long W,Array<long> &numInCroF)const{
		 long i,j,l;
         for(i=0;i<=m;i++)
       	  for(j=0;j<=limit;j++)
       		  for(l=0;l<=W;l++)
       			numInCroF(i,j,l)=-1;
         for(i=0;i<=m;i++)
          	  for(j=0;j<=limit;j++)
          		  for(l=0;l<=W;l++)
          			crossFace(i,j,l,numInCroF);
         return 1;
         
	}

	//��ʼ��numBfCroF����
	long CrossFace::initNumBfCroF(long m,long limit,long W,Array<long> &numInCroF,Array<long> &numBfCroF)const{
		 long i,j,l;
		 for(i=0;i<=m;i++)
         	  for(j=0;j<=limit;j++)
         		  for(l=0;l<=W;l++)
         			  if(l>i*j) numBfCroF(i,j,l+1)=numBfCroF(i,j,l);
         			  else  numBfCroF(i,j,l+1)=numBfCroF(i,j,l)+numInCroF(i,j,l);
         	  
		return 1;
	}

	//���������C��m��n�� arryΪ�����������Ϊnull��
	long CrossFace::combination(long m,long n,Array<long> &arry)const{
		return combination1(m,n,arry);
	}

	//���������C��m��n�� arryΪ�������鲻����Ϊnull�������鲻С��[m][n]��
	long CrossFace::combination1(long m,long n,Array<long> &arry1)const{
		if(arry1(m,n)!=0)return arry1(m,n);
		if(n==0){arry1(m,n)=1;return 1;}
		if(m==n){arry1(m,n)=1;return 1;}
		else
		{
			arry1(m,n)=combination1(m-1,n,arry1)+combination1(m-1,n-1,arry1);
			return arry1(m,n);
		}
	}




	//��StandMapping �Ķ���

	//����dmNumά������Ϊlimit�ı�׼�Գƿռ�
	StandMapping::StandMapping(long dmNum,long limit){
		setArray(dmNum,limit);
	}
	
	//���öԳƿռ䣨����ά���ͽ��޸ı�����numInCroFArr��numBfCroFArr
    void StandMapping::setArray(long dmNum,long limit){
    	dm=dmNum;
    	this->limit=limit;
    	W=dm*limit;
		numInCroFArr=Array<long>(3,dmNum+1,limit+1,dmNum*limit+1);
		numBfCroFArr=Array<long>(3,dmNum+1,limit+1,dmNum*limit+2);
		long W=dmNum*limit;
		cross.initNumInCroF(dmNum, limit, W, numInCroFArr);
		cross.initNumBfCroF(dmNum, limit, W, numInCroFArr, numBfCroFArr);
	}
    
  //����ӳ�䣬���뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w��
    long StandMapping::forwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬         ����ά��    ���Խ��棬������������λ�ã�
	(long dmNum,long limit,Array<long>&symPoint,long pointDmNum,long w,long st)const{
    	if(w==0||pointDmNum==0)return 1;//��ά���꼰�����꣨0��ӳ��Ϊ1��
    	if(pointDmNum>dmNum||w<pointDmNum)return 0;//������Ϣì��
    	//w�Խ���pointDmNumά֮ǰԪ����
		long sumBf=numBfCroFArr(dmNum,limit,w)+numInCroFArr(pointDmNum-1,limit,w);
		//w�Խ���pointDmNumάԪ������
		long sumjw=numInCroFArr(pointDmNum,limit-1,w-pointDmNum);
		//��ȥw�Խ���pointDmNumά�и�Ԫ��֮��Ԫ�������õ���Ԫ����w�Խ���pointDmNumά�д���
		sumjw-=forwdMappingIJ(symPoint,pointDmNum,limit,w,st,0);
		//����֮�ͼ�Ϊ�ܴ���
	    return sumBf+sumjw;
	}
    
  //����ӳ�䣬���뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w��
    long StandMapping::forwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬         ����ά��    ���Խ��棬������������λ�ã�
	(long dmNum,long limit,Array<long>&symPoint,long pointDmNum,long w,long st,long base)const{
    	if(w==0||pointDmNum==0)return 1;//��ά���꼰�����꣨0��ӳ��Ϊ1��
    	if(pointDmNum>dmNum||w<pointDmNum)return 0;//������Ϣì��
    	//w�Խ���pointDmNumά֮ǰԪ����
		long sumBf=numBfCroFArr(dmNum,limit,w)+numInCroFArr(pointDmNum-1,limit,w);
		//w�Խ���pointDmNumάԪ������
		long sumjw=numInCroFArr(pointDmNum,limit-1,w-pointDmNum);
		//��ȥw�Խ���pointDmNumά�и�Ԫ��֮��Ԫ�������õ���Ԫ����w�Խ���pointDmNumά�д���
		sumjw-=forwdMappingIJ(symPoint,pointDmNum,limit,w,st,base);
		//����֮�ͼ�Ϊ�ܴ���
	    return sumBf+sumjw;
	}

  //����Խ����ض�ά����������ĳ�ԳƵ�֮��Ԫ���� 
   long StandMapping::forwdMappingIJ
   //��  ��ӳ�����꣬           ����ά�� ��               ���� ���Խ��� ��  ������������λ�ã�
	(Array<long> &symPoint,long pointDmNum,long limit,long w,long st,long base)const{
		long i,t,p,q,l,numBack=0;
		long end=st+pointDmNum;
		for(i=st;i<end;i++){
			t=symPoint(i)-base;
			p=end-i;
			q=w-(t+1)*p;
			l=limit-t-1;
			if(q>=0&&l>=0)
			numBack+=numInCroFArr(p,l,q);
			w-=t;
		}
		return numBack;
	}
   
    //����ӳ����뱣֤pointDmNum<=dmNum,w>=pointDmNum;����������limit��w��
    long StandMapping::bkwdMapping
	//(�Գƿռ�ά�������ޣ�        ��ӳ�����꣬  �Խ��� �� ����ά��  ����ӳ����ţ�  ������ĩ����λ�ã�dmNumÿά��ֵ��
	(long dmNum,long limit,Array<long> &symPoint,long f, long dm,    long num,    long end,  long offset)const{
		if(f==-1)//����Խ���
			f=findFace(dmNum,limit,num);
		num-=numBfCroFArr(dmNum,limit,f);//�õ���f�Խ������������
		if(dm==-1)//��������ά��
			dm=findDm(dmNum,f,limit,num);
		num-=numInCroFArr(dm-1,limit,f);//�õ���f�Խ���dmά���������
		if(f==0){//(0)ӳ��Ϊ1��
			long j;
			for(j=end-dmNum+1;j<=end;j++)
			   symPoint(j)=offset;
			return dm;
		}
		long minItem=1;
		long back=numInCroFArr(dm,limit-1,f-dm)-num;//�õ�p��f�Խ���dmά�����к���Ԫ����
		long i,sav;
		i=sav=end-dm+1;
		//��ε�����ø�ά����������ϻ�ֵ����symPoint[end-dm+1---end]
		while(i<=end){
			long d=end-i+1;
			minItem=findFirstItem(d,f,limit,minItem,back);
			symPoint(i)=minItem+offset;
			long temp=f-d*(minItem+1);
			if(temp>=0)
			back-=numInCroFArr(d,limit-minItem-1,temp);
			f-=minItem;
			i++;
			
		}
		i=end-dmNum+1;
		//��ά��������Ϊ��ֵ
		while(i<sav){
			symPoint(i)=offset;
			i++;
		}
		//�����������ά��
		return dm;
	}
    
	//�۰����Ѱ��sp��dmNum��limit����num�ԳƵ����ڶԽ���
    long StandMapping::findFace(long dmNum,long limit,long num)const{
		long maxF=dmNum*limit;
		if(num<=0||num>numBfCroFArr(dmNum,limit,maxF+1))return 0;
		long begin=0;long end=maxF+1;
		long mid;
		while(begin<end-1){
			mid=(begin+end)/2;
			if(numBfCroFArr(dmNum,limit,mid)<num)
				begin=mid;
			else end=mid;
		}
		return begin;
	}
	//�۰����sp��dmNum��limit����f�Խ������Ϊnum�ĶԳƵ��ά����
     long StandMapping::findDm(long dmNum,long f,long limit,long num)const{
    	if(f==0)return 1;
    	long begin=f/limit+((f%limit)==0?0:1);
    	long dmMin=(dmNum>f)?f:dmNum;long end=dmMin;
    	if(num<1||num>numInCroFArr(dmMin,limit,f))return 0;
    	if(numInCroFArr(dmMin-1,limit,f)<num)return dmMin;
    	long mid;
    	while(begin<end-1){
    		mid=(begin+end)/2;
    		if(numInCroFArr(mid-1,limit,f)<num) begin=mid;
    		else end=mid;
    	}
    	return begin;
    }
     
    //��sp��dm��limit����f�Խ���dmά�����۰���Ҵ��ڵ���minItem����Ԫ����֮��Ԫ����Ϊback��
    long StandMapping::findFirstItem(long dm,long f,long limit,long minItem,long back)const{
    	long temp=f-(dm-1)*limit;
    	long begin=(minItem>temp)?minItem:temp;
    	long end=f/dm;
    	if(back>=numInCroFArr(dm,limit-begin,f-dm*begin))return 0;
    	if(back<numInCroFArr(dm,limit-end,f-dm*end))return end;
    	long mid;
    	while(begin<end-1){
    		mid=(begin+end)/2;
    		if(numInCroF(dm,limit-mid,f-dm*mid)>back)begin=mid;
    		else end=mid;
    	}
    	return begin;
    }
    //���������C��m��n��array[X,Y]Ϊ������������X>=m+n,Y>=n
    long StandMapping::combination(long m,long n,Array<long> &arr)const{
    	return cross.combination(m, n, arr);
    }
    long StandMapping::numInCroF(long dm,long limit,long w)const{
    	if(dm>this->dm||limit>this->limit||w>W+1||dm<0||limit<0||w<0)
    		return 0;
    	else return numInCroFArr(dm,limit,w);
    }
    long StandMapping::numBfCroF(long dm,long limit,long w)const{
    	if(dm>this->dm||limit>this->limit||w>W+1||dm<0||limit<0||w<0)
    		return 0;
    	else return numBfCroFArr(dm,limit,w);
    }



	//��UnsymMapping�Ķ���
	UnsymMapping::UnsymMapping(Array<long> &dmAr) {//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
		dm=dmAr(0);
		bd=Array<long>(1,dm+1);
		bkDmMu=Array<long>(1,dm+1);
		if(0==setDm(dmAr))
			throw std::exception("����dm С��1");//std::cout<<"����dm С��1"<<std::endl;
		// TODO Auto-generated constructor stub
	}

	
	long UnsymMapping::setDm(Array<long> &dmAr){//����ά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
		dm=dmAr(0);
		if(dm<=0) return 0;
		long i,temp=1;
		bd(0)=dmAr(0);
		for(i=1;i<=dm;i++){//�����ά�ǽ����Ҹ�ά����+1��
			bd(i)=dmAr(i)+1;
			temp=bd(i);
			if(bd(i)<temp){
				return 0;
			}
			
		}
		temp=1;
		//����bkDmMu��
		for(i=dm;i>0;i--){
			temp*=bd(i);
			bkDmMu(i)=temp;
		}
		return 1;
	}
    long UnsymMapping::forwdMapping(Array<long> &point,long st)const{//����ӳ�䣬��dmά����point�������
    	long i,pn=0;
    	for(i=st;i<dm+st-1;i++){
    		pn+=point(i)*bkDmMu(i-st+2);
    	}
    	pn+=point(i);
    	return pn;
    }
    
    void UnsymMapping::bkwdMapping(long pn,Array<long> &point,long st )const{//����ӳ�䣬�����pn�õ�dmά���ꡣ
    	long bkSum=pn;
    	long i;
    	for(i=st;i<dm+st-1;i++){
    		point(i)=(long)bkSum/bkDmMu(i-st+2);
    		bkSum%=bkDmMu(i-st+2);
    	}
    	point(i)=(long)bkSum;
    }


	//��UnTrimMapping�Ķ���
	UnTrimMapping::UnTrimMapping(Array<long> &dmAr) {//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
		if(initArray(dmAr)==0)
			throw std::exception("dmAr ����Ҫ��");//std::cout<<"dmAr ����Ҫ��"<<std::endl;
		// TODO Auto-generated constructor stub
	}

	//��ʼά��Ϣ��dmAr[0]�洢��ά����dmAr[i>0]��ʾiά���ޡ�
	 long UnTrimMapping::initArray(Array<long> &dmAr){
		dm=dmAr(0);
		bd=dmAr;
		if(dm<=0) 
		  return 0;
		long i,j,m=1,temp=0;
		for(i=1;i<=dm;i++){
			if(dmAr(i)<temp)
				return 0;
			if(dmAr(i)>temp)m++;
			temp=dmAr(i);
		}
		bm=m;
		boundVal=Array<long>(1,m);
		boundBegin=Array<long>(1,m+1);
		limitBetwBd=Array<long>(1,m);
		boundVal(0)=0;
		limitBetwBd(0)=0;
		temp=0;m=0;
		long maxBd=0;long mi;
		for(i=1;i<=dm;i++){
		    mi=dmAr(i)-temp;
			if(mi>0){
				m++;
				boundVal(m)=dmAr(i);
				boundBegin(m)=i-1;
			    limitBetwBd(m)=mi-1;
				if(mi>maxBd)maxBd=mi;
				temp=dmAr(i);
			}
			
		}
		maxBound=maxBd;
		m=bm;
		stMap=StandMapping(dm,maxBound);
		boundBegin(m)=dm;
		sumNumOfSunSp=Array<long>(2,m,dm);
		sumNumOfSunPro=Array<long>(4,m,dm,m,dm);
		sumNumBfSunPro=Array<long>(4,m,dm,m,dm);
		cubeTr=Array<long>(2,dm+1,maxBd);
		//��ʼ��cubeTr
		Array<long> comb=Array<long>(2,dm+maxBd,maxBd);
		for(i=1;i<=dm;i++)
			for(j=0;j<=maxBd-1;j++){
				cubeTr(i,j)=stMap.combination(i+j, j, comb);
			}
		for(j=0;j<=maxBd-1;j++){
			cubeTr(0,j)=0;
		}
		//
		for(j=0;j<dm;j++){
			sumNumOfSunSp(0,j)=1;
		}
		long u,v;
		for(i=1;i<m;i++)
			for(j=boundBegin(i+1)-1;j<dm;j++){
				long sumNumBf=sumNumOfSunSp(i-1,j);
				for(u=0;u<i;u++){
					long bdBegin=boundBegin(u+1);
					for(v=j;v>=bdBegin;v--){
						if(u<i-1){
							sumNumOfSunPro(i,j,u,v)=sumNumOfSunPro(i-1,j,u,v);
							sumNumBfSunPro(i,j,u,v)=sumNumBfSunPro(i-1,j,u,v);
						}else{
							long W=boundVal(i)-boundVal(u)-1;
							sumNumBfSunPro(i,j,u,v)=sumNumBf;
							if(v==0)
								sumNumBf+=(sumNumOfSunPro(i,j,u,v)=cubeTr(j-v+1,W));
							else
							    sumNumBf+=(sumNumOfSunPro(i,j,u,v)=sumNumOfSunSp(u,v-1)*cubeTr(j-v+1,W));				
						}
					}
				}
				sumNumOfSunSp(i,j)=sumNumBf;
			}
		return 1;
		// TODO Auto-generated constructor stub
	}
	
	//�ǹ�������ӳ��
	//st��ʾ��ͷ���ά��ȫ������ֵΪdm��symPoint(0--dm-1)��ʾ��iά����ֵ��Ҫ��ǽ������С�
	//�����������
	long UnTrimMapping::forwdMapping(long i,long j,Array<long> &symPoint,long st)const{
		long temp;
		if(symPoint(j)>boundVal(i)) return 0;
		if(j<st||j<0)return 1;
		long u=i,v=j;
		
		while(--u>=0){
			if(symPoint(j)>boundVal(u)) break;
		}
		//if(u<0&&0==symPoint[j])return 1;
		long base=boundVal(u)+1;
		long start=j,w=(symPoint(j)-base);
		while(--v>=st&&v>=boundBegin(u+1)){
			   temp=symPoint(v)-base;
			if(temp<0)
				break;
			else{
				
				w+=temp;
				if(temp>0)start=v;
			}
		}
		long Ns=0;
		long dm=j-v,limit=limitBetwBd(u+1);
		long Nt=stMap.forwdMapping(dm, limit, symPoint, j-start+1, w,start,base)-1;
		
		long sumS=0,sumT,numIn,numBef=sumNumBfSunPro(i,j,u,v+1);
		sumT=cubeTr(dm,limit);
		if(v<0)
			numIn=Nt+1;
		else{
			Ns=forwdMapping(u,v,symPoint,st)-1;
			sumS=sumNumOfSunSp(u,v);
			if(sumS>sumT)
				numIn=Nt*sumS+Ns+1;
			else numIn=Ns*sumT+Nt+1;
		}    
		return numBef+numIn;
	}
	
	//�ǹ�������ӳ��
	//�ڣ�i��j�����·��ӿռ併���n��ӳ�䣬����symPoint����jλ����ǰ��
	//���ط������ά
	long UnTrimMapping::bkwdMapping(long i,long j, Array<long> &symPoint, long n)const{
		if(n==1){
			long x;
			for(x=0;x<=j;x++)
				symPoint(x)=0;
			return x;
		}
		
		long u=findU(i,j,n);
		long v=findV(i,j,u,n);
		long Ns,Nt,sumS,sumT,numBef;
		numBef=sumNumBfSunPro(i,j,u,v);
		n-=numBef;
		long offset=boundVal(u)+1;
		long limit=boundVal(u+1)-boundVal(u)-1;
		long d=j-v+1;
		if(u==0){
			stMap.bkwdMapping(d, limit, symPoint, -1, -1, n, j,1);
			if(v>0)
			return bkwdMapping(u,v-1,symPoint,1);
			else return 0;
		}
		else{
			n--;
			sumS=sumNumOfSunSp(u,v-1);
			sumT=cubeTr(d,limit);
			if(sumS>sumT){
				Ns=n%sumS+1;Nt=n/sumS+1;
				}
			else{
				Nt=n%sumT+1;Ns=n/sumT+1;
			}
			stMap.bkwdMapping(d, limit, symPoint, -1, -1, Nt, j,offset);
			return bkwdMapping(u,v-1,symPoint,Ns);
		}
			
	}

	//���أ�i��j�����·�������Գ�Ԫ����
	long UnTrimMapping::countSunPro(long i,long j)const{
		if(i>=bm||j>=dm||i<0||j<0) 
			return 0;
		return sumNumOfSunSp(i,j);
	}

	long UnTrimMapping::countN()const{
		return countSunPro(bm-1,dm-1);
	}

	long UnTrimMapping::getbm()const{
		return bm;
	}

	long UnTrimMapping::findU(long i,long j,long n)const{
		if(n==1)return 0;
		long begin=i,end=0,mid;
		while(begin>end+1){
			mid=(begin+end)/2;
			if(sumNumOfSunSp(mid,j)>=n)begin=mid;
			else end=mid;
		}
		return end;
		
		
	}
	long UnTrimMapping::findV(long i,long j,long u,long n)const{
		long begin=boundBegin(u+1),end=j,mid;
		if(sumNumBfSunPro(i,j,u,begin)<n)return begin;
		while(begin<end-1){
			mid=(begin+end)/2;
			if(sumNumBfSunPro(i,j,u,mid)>=n)
				begin=mid;
			else end=mid;
		}
		return end;
	}
	
	
}

      