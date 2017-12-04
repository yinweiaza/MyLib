#include <vector>

const double EP = 0.00000001;

// 点结构
typedef struct PT{
	double x;
	double y;
}PT;

//线结构
typedef struct SEGMENT
{
	PT* ptStart;
	PT* ptEnd;
}SEGMENT;

//三角形类
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

	// 获取三角形外接圆中心点及半径
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

	// 通过样本点集，生成三角形集，由vecTriangleWork输出
	void BuildDelaunayEx(std::vector<PT>& vecPT, std::vector<TRIANGLE>& vecTriangleWork);

	BOOL IsPtsBuildTriangle(PT* pt1, PT* pt2, PT* pt3);
public:
	CMyDelaunay();
	virtual ~CMyDelaunay();

};