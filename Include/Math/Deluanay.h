#include <vector>

const double EP = 0.00000001;

// ��ṹ
typedef struct PT{
	double x;
	double y;
}PT;

//�߽ṹ
typedef struct SEGMENT
{
	PT* ptStart;
	PT* ptEnd;
}SEGMENT;

//��������
class TRIANGLE
{
public:
	TRIANGLE(PT* pt1, PT* pt2, PT* pt3);
	PT* GetFirstPt(){return _ptFirst;}
	PT* GetSecondPt(){return _ptSecond;}
	PT* GetThirdPt(){return _ptThird;}

	SEGMENT GetSegment1(){SEGMENT seg = {_ptFirst,_ptSecond};return seg;}
	SEGMENT GetSegment2(){SEGMENT seg = {_ptFirst,_ptThird};return seg;}
	SEGMENT GetSegment3(){SEGMENT seg = {_ptSecond,_ptThird};return seg;}

	BOOL IsTriangleSame(TRIANGLE& tri);

	BOOL IsPtInCircle(PT* pt);
protected:

	// ��ȡ���������Բ���ĵ㼰�뾶
	void InitData();
private:
	PT* _ptFirst;
	PT* _ptSecond;
	PT* _ptThird;

	PT            _ptCenter;
	double        _Radius2;
	double        _Radius;
};

class CMyDelaunay  
{
public:

	// ͨ�������㼯�����������μ�����vecTriangleWork���
	void BuildDelaunayEx(std::vector<PT>& vecPT, std::vector<TRIANGLE>& vecTriangleWork);

	BOOL IsPtsBuildTriangle(PT* pt1, PT* pt2, PT* pt3);
public:
	CMyDelaunay();
	virtual ~CMyDelaunay();

};