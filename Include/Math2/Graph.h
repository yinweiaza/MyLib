#include <list>

//无向图模型，可在该类基础上构造有向图

using namespace std;
class CGraph
{
public:
	CGraph(int nVeticals);
	~CGraph();
	void		AddEdge(int v, int w);
	
	int			GetVerticals()				{return 	m_nVerticals;};
	int			GetEdges()					{return		m_nEdges;};
	list<int>*	GetAdjacency(int vetical)	{return		&m_pAdjacency[vetical];};

protected:
	const int	m_nVerticals;
	int			m_nEdges;
	list<int>*	m_pAdjacency;
};