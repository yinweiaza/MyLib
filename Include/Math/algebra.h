#ifndef algebraConst
#define algebraConst
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

//����������㷨
#include <vector>
#include "array.h"
#include "number.h"
#include <cmath>
#include "myException.h"
namespace lmtc{
	class Algebra{
	public:
		//���㷽�ݺ͹�ʽ,���ض�ά����S,���У���������k<=K, 0^k+1^k+2^k+...+n^k=S(k,0)+S(k,1)*n+S(k,2)*n^2+...+S(k,k+1)*n^(k+1) .
		//O(K^3)�����㷨Ϊ���߷�����
		static Array<double> powerSumFormula(const unsigned int K);

		//���㷽�ݺͣ�0^k+1^k+2^k+...+n^k .
		//O(k^3),��n�޹أ����㷨Ϊ���߷�����
		static double powerSum(const unsigned int n,const unsigned int k);

		//���û��ɹ���������ض���ʽcoef[0]+coef[1]*x+......+coef[coef.size()-1]*x^(coef.size()-1)
		//O(n)���μ����㷨����p24��ϰ��2-3��
		template<typename T>
		static double computePolynomialByHonerRule(const double x,const std::vector<T> coef);

		//��ͨ�ľ���˷�������Ϊmatrix1��matrix2��������Ϊ��ά���飬�����������˵�Ҫ�󡣷��ض�ά���顣
		//O(n^3)
		template<typename T>
		static Array<T> matrixMultiplySimple(const Array<T> &matrix1,const Array<T> &matrix2);

		//Strassen����˷�������Ϊmatrix1��matrix2��������Ϊ��ά���飬�����������˵�Ҫ�󡣷��ض�ά���顣
		//O(n^2.81),�μ����㷨����p451-455��
		template<typename T>
		static Array<T> matrixMultiplyStrassen(const Array<T> &matrix1,const Array<T> &matrix2);

		//LUP�ֽ⡣AΪ���ֽ���󣬽������L��U��P��ʹ��PA=LU���ɷֽⷵ��true�����ɷֽ⣨���췽�󣩷���false��
		//O(n^3),�μ����㷨����p456-463��
		static bool lupDecompose(const Array<double> &A,Array<double> &L,Array<double> &U,std::vector<unsigned int> &P);

		//�����Է�����LUx=Pb��L��U��PΪĳ�����췽���LUP�ֽ⣬�����x���û����뱣֤L��U��P��ȷ�����򽫻����δ֪���쳣�����
		//O(n^2),�μ����㷨����p457��
		static void solveLinearEquationsGivenLUP(const Array<double> &L,const Array<double> &U,const std::vector<unsigned int> &P,const std::vector<double> &b,std::vector<double> &x);

		//�����Է�����Ax=b������LUP�ֽ⡣�����x���н��򷵻�true���޽ⷵ��false��
		//O(n^3),�μ����㷨����p457��
		static bool solveLinearEquationsByLUP(const Array<double> &A,const std::vector<double> &b,std::vector<double> &x);

		//���ٽ����Է�����Ax=b�����ÿ��پ������档�����x���н��򷵻�true���޽ⷵ��false��
		//O(n^2.81)�����μ��㷨����p456,465��
		static bool solveLinearEquationsFast(const Array<double> &A,const std::vector<double> &b,std::vector<double> &x);

		//����LUP�ֽ����������A������󣬽������_A��A���淵��true�������淵��false��
		//O(n^3),�μ����㷨����p464����
		static bool inverseMatrixByLUP(const Array<double> &A,Array<double> &_A);

		//���پ������档��A������󣬽������_A��A���淵��true�������淵��false��
		//ʱ�临�Ӷ������˷���ͬ������Strassen�˷�ʱΪ��O(n^2.81),�μ����㷨����p465��
		static bool inverseMatrixFast(const Array<double> &A,Array<double> &_A);

		//����ת�á�
		//O(n^2)
		template<typename T>
		static Array<T> transposeMatrix(const Array<T> &A);

		//��A��ǰ��α�����A_���Ƿ�����������������Ȼ�ƹ㣩����A_*A=E��
		//���ҽ���A������ʱ��A_���ڣ���ʱ����true�����򷵻�false��
		//O(n^2.81),����Strassen���ٳ˷��Ϳ��������㷨,�μ����㷨����p469����
		static bool pseudoInverseMatrixForward(const Array<double> &A,Array<double> &A_);

		//��A�ĺ���α�����_A���Ƿ�����������������Ȼ�ƹ㣩����A*_A=E��
		//���ҽ���A������ʱ��_A���ڣ���ʱ����true�����򷵻�false��
		//O(n^2.81),����Strassen���ٳ˷��Ϳ��������㷨,�μ����㷨����p469����
		static bool pseudoInverseMatrixBackward(const Array<double> &A,Array<double> &_A);

		//����LUP����̬������Ax=b��Ҫ��A��������С��������b����������A�������������쳣����
		//AΪ��������ɽⷵ��true�����򷵻�false�������x��
		//O(n^3),�μ����㷨����p469����
		static bool solveNormalityEquationsByLUP(const Array<double> &A,const std::vector<double> &b,std::vector<double> &x);

		//���ٽ���̬������Ax=b��Ҫ��A��������С��������b����������A�������������쳣����
		//AΪ��������ɽⷵ��true�����򷵻�false�������x��
		//O(n^2.81),�μ����㷨����p469����
		static bool solveNormalityEquationsFast(const Array<double> &A,const std::vector<double> &b,std::vector<double> &x);

		//��С���˹��ƣ�LUP����minimize{|F(X).C-Y|},����F(x)=( f(0,x),f(1,x),...,f(n-1,x) )��
		//f��n���������ƺ�������ʽ��X��Y�����������������ƽ������C��
		//Ҫ��X��Y�ߴ���ͬ������С��n�������쳣�������Ƴɹ�����true������˵�����ݲ�������ʧ�ܷ���false��
		//O(n^3),�μ����㷨����p469����
		static bool leastSquaresEstimationByLUP(double (*f)(unsigned int i,double x),const unsigned int n,const std::vector<double> &X,const std::vector<double> &Y,std::vector<double> &C);

		//������С���˹��ƣ�minimize{|F(X).C-Y|},����F(x)=( f(0,x),f(1,x),...,f(n-1,x) )��
		//f��n���������ƺ�������ʽ��X��Y�����������������ƽ������C��
		//Ҫ��X��Y�ߴ���ͬ������С��n�������쳣�������Ƴɹ�����true������˵�����ݲ�������ʧ�ܷ���false��
		//O(n^2.81),�μ����㷨����p469����
		static bool leastSquaresEstimationFast(double (*f)(unsigned int i,double x),const unsigned int n,const std::vector<double> &X,const std::vector<double> &Y,std::vector<double> &C);

		//�������С���˹��ƣ�LUP����Ϊ��������С���˹��Ƶ�������չ��ʽ��
		//����Ҫ�󼰺���Ҳ�ǵ�������С���˹��Ƶ�������չ��
		//O((n[0]*n[1]*...*n[varNum])^3) .
		static bool leastSquaresEstimationByLUP(double (*f)(const std::vector<unsigned int> &i,const std::vector<double> &x),const std::vector<unsigned int> &n,const lmtc::Array<double> &X,const std::vector<double> &Y,lmtc::Array<double> &C);

		//���ÿ��ٸ���Ҷ�任����a(*)b,���ؾ�������������Ϊ����ʽ�˻���
		//O(nlgn),����n=a.size()+b.size()���μ����㷨����p506-515��
		static std::vector<ComplexNumber> convolution(const std::vector<ComplexNumber> &a,const std::vector<ComplexNumber> &b);

		//���ö�ά���ٸ���Ҷ�任���ά���a(*)b,���ؾ����������ά�����Ϊ��ά����ʽ�˻���
		//��a��b��ά��Ϊ����ʱ��O(nlgn),����n=c.size(),cΪ���صľ���������μ����㷨����p506-515��520��
		static Array<ComplexNumber> convolution(const Array<ComplexNumber> &a,const Array<ComplexNumber> &b);

		//����ʽ����a/b�����ÿ��ٸ���Ҷ�任,���ض���ʽ��ϵ����������b����aʱ���Ա�֤�����ȷ����������ʱ�������ƫ����������������
		//O(nlgn),����n=max(a.size(),b.size())��
		static std::vector<ComplexNumber> polynomialDivisionByFFT(const std::vector<ComplexNumber> &a,const std::vector<ComplexNumber> &b);

		//����ʽ����a/b�����ط���,���ض���ʽ��ϵ������������ʽ������������c��
		//O((a.size-b.size)*b.size)��
		static std::vector<ComplexNumber> polynomialDivisionGeneral(const std::vector<ComplexNumber> &a,const std::vector<ComplexNumber> &b,std::vector<ComplexNumber> &c);

	private:
		//Strassen����˷�����������
		template<typename T>
		static Array<T> matrixMultiplyStrassen_visit(const Array<T> &matrix1,const Array<T> &matrix2);

		//���پ������渨��������AΪ�����ݶԳ��������󣩡�
		static bool inverseMatrixFast_visit(const Array<double> &A,Array<double> &_A);
	};

	//���û��ɹ���������ض���ʽcoef[0]+coef[1]*x+......+coef[coef.size()-1]*x^(coef.size()-1)
	template<typename T>
	double Algebra::computePolynomialByHonerRule(const double x,const std::vector<T> coef){
		double rs=0;
		for(int i=coef.size()-1;i>=0;i--)
		    rs=rs*x+coef[i];
		return rs;
	}

	//��ͨ�ľ���˷�
	//O(n^3)
	template<typename T>
	Array<T> Algebra::matrixMultiplySimple(const Array<T> &matrix1,const Array<T> &matrix2){
		if(matrix1.getDimNum()!=2||matrix2.getDimNum()!=2||matrix1.getDimLen(1)!=matrix2.getDimLen(0))
			throw MatrixException("����˷�matrixMultiplySimple�Ĳ���������Ҫ��");
		Array<T> matrixRs(2,matrix1.getDimLen(0),matrix2.getDimLen(1));
		for(unsigned int i=0;i<matrixRs.getDimLen(0);i++)
			for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
				for(unsigned int k=0;k<matrix1.getDimLen(1);k++)
					matrixRs(i,j)+=matrix1(i,k)*matrix2(k,j);
		return matrixRs;
	}
	
	//Strassen����˷�
	template<typename T>
	Array<T> Algebra::matrixMultiplyStrassen(const Array<T> &matrix1,const Array<T> &matrix2){
		if(matrix1.getDimNum()!=2||matrix2.getDimNum()!=2||matrix1.getDimLen(1)!=matrix2.getDimLen(0))
			throw MatrixException("����˷�matrixMultiplySimple�Ĳ���������Ҫ��");
		unsigned int u=0,v=0,w=0;
		unsigned int cout=1;
		while(u==0||v==0||w==0)
		{	
			if(u==0&&matrix1.getDimLen(0)<=cout)
				u=cout;
			if(v==0&&matrix1.getDimLen(1)<=cout)
				v=cout;
			if(w==0&&matrix2.getDimLen(1)<=cout)
				w=cout;
			cout*=2;
		}
		Array<T> matrixStd1(2,u,v);
		Array<T> matrixStd2(2,v,w);
		for(unsigned int i=0;i<matrix1.getDimLen(0);i++)
			for(unsigned int j=0;j<matrix1.getDimLen(1);j++){
				matrixStd1(i,j)=matrix1(i,j);
			}
		for(unsigned int i=0;i<matrix2.getDimLen(0);i++)
			for(unsigned int j=0;j<matrix2.getDimLen(1);j++){
				matrixStd2(i,j)=matrix2(i,j);
			}
		Array<T> matrixStdRs = matrixMultiplyStrassen_visit(matrixStd1,matrixStd2);
		Array<T> matrixRs(2,matrix1.getDimLen(0),matrix2.getDimLen(1));
		for(unsigned int i=0;i<matrixRs.getDimLen(0);i++)
			for(unsigned int j=0;j<matrixRs.getDimLen(1);j++)
				matrixRs(i,j)=matrixStdRs(i,j);
		return matrixRs;
	}
	//Strassen����˷�����������
	template<typename T>
	Array<T> Algebra::matrixMultiplyStrassen_visit(const Array<T> &matrix1,const Array<T> &matrix2){
		if(matrix1.getDimLen(0)==1||matrix1.getDimLen(1)==1||matrix2.getDimLen(1)==1)
			return matrixMultiplySimple(matrix1,matrix2);
		Array<T> temp1(2,matrix1.getDimLen(0)/2,matrix1.getDimLen(1)/2);
		Array<T> temp2(2,matrix2.getDimLen(0)/2,matrix2.getDimLen(1)/2);
		Array<T> matrixRs(2,matrix1.getDimLen(0),matrix2.getDimLen(1));

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i,j);
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i,j+temp2.getDimLen(1))-matrix2(i+temp2.getDimLen(0),j+temp2.getDimLen(1));
		Array<T> p1=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i,j)+matrix1(i,j+temp1.getDimLen(1));
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i+temp2.getDimLen(0),j+temp2.getDimLen(1));
		Array<T> p2=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i+temp1.getDimLen(0),j)+matrix1(i+temp1.getDimLen(0),j+temp1.getDimLen(1));
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i,j);
		Array<T> p3=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i+temp1.getDimLen(0),j+temp1.getDimLen(1));
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i+temp2.getDimLen(0),j)-matrix2(i,j);
		Array<T> p4=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i,j)+matrix1(i+temp1.getDimLen(0),j+temp1.getDimLen(1));
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i,j)+matrix2(i+temp2.getDimLen(0),j+temp2.getDimLen(1));
		Array<T> p5=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i,j+temp1.getDimLen(1))-matrix1(i+temp1.getDimLen(0),j+temp1.getDimLen(1));
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i+temp2.getDimLen(0),j)+matrix2(i+temp2.getDimLen(0),j+temp2.getDimLen(1));
		Array<T> p6=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp1.getDimLen(1);j++)
				temp1(i,j)=matrix1(i,j)-matrix1(i+temp1.getDimLen(0),j);
		for(unsigned int i=0;i<temp2.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++)
				temp2(i,j)=matrix2(i,j)+matrix2(i,j+temp2.getDimLen(1));
		Array<T> p7=matrixMultiplyStrassen_visit(temp1,temp2);

		for(unsigned int i=0;i<temp1.getDimLen(0);i++)
			for(unsigned int j=0;j<temp2.getDimLen(1);j++){
				matrixRs(i,j)=p5(i,j)+p4(i,j)-p2(i,j)+p6(i,j);
				matrixRs(i+temp1.getDimLen(0),j)=p3(i,j)+p4(i,j);
				matrixRs(i,j+temp2.getDimLen(1))=p1(i,j)+p2(i,j);
				matrixRs(i+temp1.getDimLen(0),j+temp2.getDimLen(1))=p5(i,j)+p1(i,j)-p3(i,j)-p7(i,j);
			}
		return matrixRs;
	}

	//����ת�á�
	template<typename T>
	Array<T> Algebra::transposeMatrix(const Array<T> &A){
		if(A.getDimNum()!=2)
			throw MatrixException("transposeMatrix�Ĳ���A��Ϊ��ά����");
		Array<T> AT(2,A.getDimLen(1),A.getDimLen(0));
		for(unsigned int i=0;i<AT.getDimLen(0);i++)
			for(unsigned int j=0;j<AT.getDimLen(1);j++)
				AT(i,j)=A(j,i);
		return AT;
	}

}

#endif