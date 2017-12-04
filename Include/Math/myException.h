#ifndef myExceptionConst
#define myExceptionConst
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

/* �Լ����쳣�࣬ʹ�������ռ䣨lmtc��*/
#include <exception>
#include <string>
namespace lmtc{

	class arrayOutBound:public std::exception{
	public:
		arrayOutBound():exception("�쳣���������Խ��"){};	
		arrayOutBound(const char * str):exception(str){};
		arrayOutBound(const std::string &str):exception(str.c_str()){};	
	};

	class arrayAccessException:public std::exception{
	public:
		arrayAccessException():exception("�쳣��������ʴ���"){};	
		arrayAccessException(const char * str):exception(str){};
		arrayAccessException(const std::string &str):exception(str.c_str()){};	
	};

	class mappingException:public std::exception{
	public:
		mappingException():exception("�쳣��ӳ���쳣"){};	
		mappingException(const char * str):exception(str){};
		mappingException(const std::string &str):exception(str.c_str()){};	
	};

	class HeapAccessException:public std::exception{
	public:
		HeapAccessException():exception("�쳣���ѷ����쳣"){};	
		HeapAccessException(const char * str):exception(str){};
		HeapAccessException(const std::string &str):exception(str.c_str()){};	
	};

	class EmptySmartPtrAccessException:public std::exception{
	public:
		EmptySmartPtrAccessException():exception("�쳣�����ʿյ�����ָ��"){};	
		EmptySmartPtrAccessException(const char * str):exception(str){};
		EmptySmartPtrAccessException(const std::string &str):exception(str.c_str()){};	
	};

	class HashException:public std::exception{
	public:
		HashException():exception("�쳣����ϣ���쳣"){};	
		HashException(const char * str):exception(str){};
		HashException(const std::string &str):exception(str.c_str()){};	
	};

	class MatroidException:public std::exception{
	public:
		MatroidException():exception("�쳣�������쳣"){};	
		MatroidException(const char * str):exception(str){};
		MatroidException(const std::string &str):exception(str.c_str()){};	
	};

	class GraphException:public std::exception{
	public:
		GraphException():exception("�쳣��ͼ�쳣"){};	
		GraphException(const char * str):exception(str){};
		GraphException(const std::string &str):exception(str.c_str()){};	
	};

	class MatrixException:public std::exception{
	public:
		MatrixException():exception("�쳣�������쳣"){};	
		MatrixException(const char * str):exception(str){};
		MatrixException(const std::string &str):exception(str.c_str()){};	
	};

	class ComplexNumberException:public std::exception{
	public:
		ComplexNumberException():exception("�쳣�������쳣"){};	
		ComplexNumberException(const char * str):exception(str){};
		ComplexNumberException(const std::string &str):exception(str.c_str()){};	
	};

	class BigDecimalException:public std::exception{
	public:
		BigDecimalException():exception("�쳣�������쳣"){};	
		BigDecimalException(const char * str):exception(str){};
		BigDecimalException(const std::string &str):exception(str.c_str()){};	
	};

	class FastFourierTransformException:public std::exception{
	public:
		FastFourierTransformException():exception("�쳣�����ٸ���Ҷ�任�쳣"){};	
		FastFourierTransformException(const char * str):exception(str){};
		FastFourierTransformException(const std::string &str):exception(str.c_str()){};	
	};

	class NumberTheoryException:public std::exception{
	public:
		NumberTheoryException():exception("�쳣�����۷�����쳣"){};	
		NumberTheoryException(const char * str):exception(str){};
		NumberTheoryException(const std::string &str):exception(str.c_str()){};	
	};

	class GeometryException:public std::exception{
	public:
		GeometryException():exception("�쳣�����㼸�η�����쳣"){};	
		GeometryException(const char * str):exception(str){};
		GeometryException(const std::string &str):exception(str.c_str()){};	
	};
	
	class LinearProgrammingException:public std::exception{
	public:
		LinearProgrammingException():exception("�쳣�����Թ滮������쳣"){};	
		LinearProgrammingException(const char * str):exception(str){};
		LinearProgrammingException(const std::string &str):exception(str.c_str()){};	
	};

	class OperationsResearchException:public std::exception{
	public:
		OperationsResearchException():exception("�쳣���˳�ѧ������쳣"){};	
		OperationsResearchException(const char * str):exception(str){};
		OperationsResearchException(const std::string &str):exception(str.c_str()){};	
	};

	class BinomialHeapException:public std::exception{
	public:
		BinomialHeapException():exception("�쳣������ѷ�����쳣"){};	
		BinomialHeapException(const char * str):exception(str){};
		BinomialHeapException(const std::string &str):exception(str.c_str()){};	
	};

}



#endif 