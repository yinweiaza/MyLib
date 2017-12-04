#include "stdafx.h"
#include "Graph.h"


CGraph::CGraph( int nVeticals )	: m_nVerticals(nVeticals)
{
	m_nEdges = 0;
	m_pAdjacency = new list<int>[nVeticals];
}

CGraph::~CGraph()
{
	SafeDeleteArray(m_pAdjacency);
}

void CGraph::AddEdge( int v, int w )
{
	m_pAdjacency[v].push_back(w);
	m_pAdjacency[w].push_back(v);
	m_nEdges++;
}