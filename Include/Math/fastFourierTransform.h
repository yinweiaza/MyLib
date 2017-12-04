#ifndef fastFourierTransformConst
#define fastFourierTransformConst
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

//���ٸ���Ҷ�任������任
#include "number.h"
#include <vector>
#include "array.h"
namespace lmtc{
	class FastFourierTransform{
	public:
		//���ٸ���Ҷ�任��ϵ������a�ĳߴ��Զ�����Ϊ�����ݣ����������䣬���ظ���Ҷ��ֵ������
		//O(nlgn),n=a.size(),�μ����㷨����p506-515��
		static std::vector<ComplexNumber> fft(const std::vector<ComplexNumber> &a);

		//���ٸ���Ҷ��任��y�ߴ����Ϊ�����ݣ������쳣��������ԭϵ��������
		//O(nlgn),n=a.size(),�μ����㷨����p506-515��
		static std::vector<ComplexNumber> _fft(const std::vector<ComplexNumber> &y);

		//��ά���ٸ���Ҷ�任��ϵ������a�ĸ�ά�ߴ��Զ�����Ϊ�����ݣ����������䣬���ظ���Ҷ��ֵ������
		//O(nlgn),n=a.size(),�μ����㷨����p520��ϰ��30-3��
		static lmtc::Array<ComplexNumber> fft(const lmtc::Array<ComplexNumber> &a);

		//��ά���ٸ���Ҷ��任������y��ά�ߴ����Ϊ�����ݣ������쳣��������ԭϵ��������
		//O(nlgn),n=a.size(),�μ����㷨����p520��ϰ��30-3��
		static lmtc::Array<ComplexNumber> _fft(const lmtc::Array<ComplexNumber> &y);

	private:
		//���ٸ���Ҷ�任����������
		static std::vector<ComplexNumber> fft_visit(const std::vector<ComplexNumber> &a);

		//���ٸ���Ҷ��任����������
		static std::vector<ComplexNumber> _fft_visit(const std::vector<ComplexNumber> &y);

		//��ά���ٸ���Ҷ�任����������
		static lmtc::Array<ComplexNumber> fft_visit(const lmtc::Array<ComplexNumber> &a);

		//��ά���ٸ���Ҷ��任����������
		static lmtc::Array<ComplexNumber> _fft_visit(const lmtc::Array<ComplexNumber> &y);
	};
}
#endif