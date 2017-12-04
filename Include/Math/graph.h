#ifndef graphConst
#define graphConst
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
#include <list>
#include <queue>
#include "mySort.h"
#include "nonIntersectSet.h"
#include "fibonacciHeap.h"
#include "hash.h"
#include "numberTheory.h"
#include "myException.h"
#include "array.h"


/*ͼ����ͨ�����ݽṹ��ͼ�����ͨ���㷨*/
namespace lmtc{

	//�ߵ���ģ�壬һ��ָ����ߡ�
	template<typename T>
	class Edge{
	public:
		unsigned int vSt;
		unsigned int vEd;
		T data;
	public:
		Edge(unsigned int v_st,unsigned int v_ed,const T &t):vSt(v_st),vEd(v_ed),data(t){}
		Edge(unsigned int v_st,unsigned int v_ed):vSt(v_st),vEd(v_ed),data(){}
		bool operator==(const Edge<T> &e)const{return vSt==e.vSt&&vEd==e.vEd;}
	};
	
	/*ͼ���࣬����ͼ���г����㷨�����Ծ�̬�ӿ���ʽ����
	  �ڽӾ�������ͼ������ͼ������ͼ�ı߳������Σ���lmtc::Array<T>(2,vN,vN)��ʾ,T��ʾȨ���ͣ�vN��ʾ��������ֵΪ���ʾ�����ӣ�����Ԫ��ֵ��ʾȨ��
	  �ڽӱ����ͣ�����ͼ������ͼ������ͼ�ı߳������Σ�std::vector<std::list<Edge<T>>>*/
	class Graph{
	public:
		//���ڽӾ���ajacencyMatrixת�����ڽӱ��������ajacencyList�����ajacencyMatrix�������ڽӾ���Ҫ�󣬽��ÿ�ajacencyList��
		//O(V*V)
		template<typename T>
		static void ajacencyMatrixToList(const Array<T> &ajacencyMatrix,std::vector<std::list<Edge<T>>> &ajacencyList);

		//ת���ڽӱ�ajacencyList���������transposedAjacencyList�����뱣֤ajacencyListΪ��ȷ���ڽӱ�
		//O(V+E)
		template<typename T>
		static void transposeAjacencyList(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &transposedAjacencyList);

		/*��Ⱥ�������ȱ��������ڴ˵Ļ���ͼ�㷨*/
		//ͨ�ù�����ȱ����������������ɭ�֣����豣֤�ڽӱ���ȷ���ýӿ�û���κ�ʵ�ʹ��ܣ�����Ϊ�����ô��롣
		template<typename T>
		static void bfs(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s=0);

		//ͨ��������ȱ����������������ɭ�֣����豣֤�ڽӱ���ȷ���ýӿ�û���κ�ʵ�ʹ��ܣ�����Ϊ�����ô��롣
		template<typename T>
		static void dfs(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s=0);

		//����������ȱ���ȷ������ͼ��������ͼ�Ƿ���ڻ�·���豣֤�ڽӱ���ȷ����Ҫ���ò���isDirectedGraph��������ͼ��������ͼ��
		//O(E)
		template<typename T>
		static bool hasLoop(const std::vector<std::list<Edge<T>>> &ajacencyList,bool isDirectedGraph=true);

		//ͨ��������ȱ�����������ͼ���������豣֤�ڽӱ���ȷ������������������������������л�ͼ���򷵻ؿ�������
		//O(E+V)
		template<typename T>
		static std::vector<unsigned int> topologicalSort(const std::vector<std::list<Edge<T>>> &ajacencyList);

		//����������ȱ����ҳ�����ͼ������ͼ��ɣ�����ǿ��ͨ��֧���豣֤�ڽӱ�ajacencyList��ȷ���������strongConnectComponents
		//O(E+VlgV)��VlgVʱ����Ҫ���ڵڶ���������ȱ���ʱ��ʱ�������
		template<typename T>
		static void computeStrngConctComps(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::vector<unsigned int>> &strongConnectComponents);

		//���ñ��ε�������ȱ�����������ͼ������ͼ��ŷ�������豣֤�ڽӱ�ajacencyList��ȷ������ŷ��������true���������EulerCircuit�������򷵻�false��EulerCircuit�ÿգ���
		//O(V+E)����Ҫ���ò���isDirectedGraph��������ͼ��������ͼ����Ϊ����ͼʱ����ɢ�н��бߵ��߼�ɾ�������ʱ�临�Ӷ�������ͼ������ͬ�ġ�
		template<typename T>
		static bool computeEulerCircuit(const std::vector<std::list<Edge<T>>> &ajacencyList,std::list<Edge<T>> &EulerCircuit,bool isDirectedGraph=true);
	
		/*��С����������*/
		//Kruskal��������ͼ��С������������ͨ�������С����ɭ�֣�����ͼ�����Ի������ò��ཻ����ʵ�֡�
		//O(ElgE)
		template<typename T>
		static T mstKruskal(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &mstAjacencyList);

		//Prim��������ͼ��С������������ͨ�������С����ɭ�֣�����ͼ�����Ի�������쳲�������ʵ�֡�
		//O(E+VlgV)
		template<typename T>
		static T mstPrim(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &mstAjacencyList);

		/*���·������*/
		//Bellman-Ford�㷨��s��ʾԭ������ţ����ж��Ƿ��и���·������ͼ��������ͼ��������ͼ���и�Ȩ�ߡ�
		//���·����������ڸ��ڵ�����p�;�������d������и���·��p��d������ʾ�ҵ������������·�����ҷ���false�����򷵻�true��
		//O(V*E)
		template<typename T>
		static bool shortestPathBellmanFord(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d);
		
		//�����޻�·ͼ�ĵ�Դ���·����s��ʾԭ������ţ�����л�·�򷵻�false���ÿ�p��d���������򷵻�true��
		//���·����������ڸ��ڵ�����p�;�������d������л�·�򷵻�false�����򷵻�true��
		//O(V+E)
		template<typename T>
		static bool shortestPathOfDag(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d);

		//dijkstra�㷨��Ҫ���޸��ߣ���s��ʾԭ������ţ�����и����򷵻�false���ÿ�p��d���������򷵻�true��
		//���·����������ڸ��ڵ�����p�;�������d�������Դ����ĸ��ڵ�Ϊ�����򲻴����䵽Դ�����·����
		//O(E+VlgV)
		template<typename T>
		static bool shortestPathDijkstra(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d);

		//���ÿ��پ���˷�ʵ��ÿ�Զ��������·����O(V^e*lgV),e<3���μ����㷨����p385�����ԡ�

		//Floyd-Warshall�㷨����������ͼ�����ڸ���·��
		//���·��ɭ�ֽ�����ڸ��ڵ����p�;������d�������Դ����ĸ��ڵ�Ϊ�����򲻴����䵽Դ�����·����
		//O(V^3)���μ����㷨����p387��
		template<typename T>
		static void shortestPathAllFloydWarshall(const std::vector<std::list<Edge<T>>> &ajacencyList,Array<unsigned int> &p,Array<T> &d);
		
		//Johnson�㷨���ܹ��ж��Ƿ���ڸ���·��
		//���·��ɭ�ֽ�����ڸ��ڵ����p�;������d�������Դ����ĸ��ڵ�Ϊ�����򲻴����䵽Դ�����·����
		//������ڸ���·�����ÿ�p��d������false�����򷵻�true
		//O(V^2*lgV+VE)���μ����㷨����p391��
		template<typename T>
		static bool shortestPathAllJohnson(const std::vector<std::list<Edge<T>>> &ajacencyList,Array<unsigned int> &p,Array<T> &d);

		/*���������*/
		//FordFulkerson_EdmondsKarp�㷨��˼·Ϊ�������ù����������Ѱ�Ҳ��������е�����·������������
		//Ҫ��ajacencyList�޸���(���߿�����ת�ɷ������ߣ������㷨�������ת���������s�ͻ��t���㷨���Զ������Ի���s������Լ�t�ĳ��ߣ�������������flow֮�У������������ֵ��
		//O(V*E^2)���μ����㷨����p397-408��
		template<typename T>
		static T maximumFlowFordFulkerson_EdmondsKarp(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,const unsigned int t,std::vector<std::list<Edge<T>>> &flow);

		//Relabel_To_Front�㷨��˼·Ϊ���϶������������������е�һ������ڵ����ѹ����ر�ǲ�����ʹ֮���������
		//Ҫ��ajacencyList�޸���(���߿�����ת�ɷ������ߣ������㷨�������ת���������s�ͻ��t���㷨���Զ������Ի���s������Լ�t�ĳ��ߣ�������������flow֮�У������������ֵ��
		//O(V^3)���μ����㷨����p411-425��
		template<typename T>
		static T maximumFlowPushRelabelToFront(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,const unsigned int t,std::vector<std::list<Edge<T>>> &flow);
	
		//��Դ��������������⡶�㷨����p399����������ƥ�����⡶�㷨����p409������ת��Ϊ��Դ�㵥�������������⡣
	private:
		//�����ʾ���������ж�����ɫ��ö������
		typedef enum{WHITE,GRAY,BLACK} Color; 

		//ͨ��������ȱ�����ĳ����������������豣֤�ڽӱ���ȷ
		template<typename T>
		static void dfs_visit(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time);

		//����������ȱ���ȷ����ĳ�ڵ㿪ʼ������������Ƿ����һ��·���豣֤�ڽӱ���ȷ
		template<typename T>
		static bool has_loop_visit(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time,bool isDirectedGraph);

		//ͨ��������ȱ��������������򣨽���ĳ����������������豣֤�ڽӱ���ȷ
		template<typename T>
		static void topologicalSort_visit(std::vector<unsigned int> &order,const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time);

		//���ñ��ε�������ȱ�����������ͼ��ŷ����������ĳ����������������豣֤�ڽӱ�ajacencyList��ȷ������ŷ��������true���������EulerCircuit�������򷵻�false��EulerCircuit�ÿգ���
		template<typename T>
		static bool computeEulerCircuit_visit(std::vector<std::list<Edge<T>>> &ajacencyList,std::list<Edge<T>> &EulerCircuit,const unsigned int s,bool isDirectedGraph,CompleteHash<unsigned long> &hash);

		//����쳲������ѵı� �ȽϺ���
		template<typename T>
		static bool pri_comp_edge(const Edge<T> &a,const Edge<T> &b){return a.data<b.data;}

		//����ɢ�еı���ȱȽϺ���
		template<typename T>
		static bool edge_equal(const Edge<T> &a,const Edge<T> &b){return a.vSt==b.vSt&&a.vEd==b.vEd;}

		//����ɢ�е�Ĭ�ϼ�ֵת������
		template<typename T>
		static unsigned long key_to_number(const T &t){return t;}

		//����ɢ�еı�תֵ����,Ҫ�󶥵���������100000��
		template<typename T>
		static unsigned long edge_to_number(const Edge<T> &e){return e.vSt*100000+e.vEd;}
	};
	
	//���ڽӾ���ת�����ڽӱ�����ajacencyList�����ajacencyMatrix�������ڽӾ���Ҫ�󣬽��ÿ�ajacencyList��
	template<typename T>
	void Graph::ajacencyMatrixToList(const Array<T> &ajacencyMatrix,std::vector<std::list<Edge<T>>> &ajacencyList){
		ajacencyList.clear();
		if(ajacencyMatrix.getDimNum()!=2||ajacencyMatrix.getDimLen(0)!=ajacencyMatrix.getDimLen(1))
			return;
		ajacencyList.resize(ajacencyMatrix.getDimLen(0));
		for(unsigned int i=0;i<ajacencyList.size();i++){
			for(unsigned int j=0;j<ajacencyMatrix.getDimLen(1);j++){
				if(ajacencyMatrix(i,j)==0)
					continue;
				ajacencyList[i].push_back(Edge<T>(i,j,ajacencyMatrix(i,j)));
			}
		}
	}

	//ת���ڽӱ�����transposedAjacencyList�����뱣֤ajacencyListΪ��ȷ���ڽӱ�
	template<typename T>
	void Graph::transposeAjacencyList(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &transposedAjacencyList){
		transposedAjacencyList.clear();
		transposedAjacencyList.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				transposedAjacencyList[iter->vEd].push_back(Edge<T>(iter->vEd,iter->vSt,iter->data));
	}

	//ͨ�ù�����ȱ������豣֤�ڽӱ���ȷ
	template<typename T>
	void Graph::bfs(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s)
	{
		if(s>=ajacencyList.size())//Դ���㳬���ڽӱ�Χ
			return;
		/*����һЩ������Ϣ������d[u]��ʾu����ȣ�
		  p[u]��ʾu�ĸ����㣨p[u]==-1��ʾuΪ������
		  color[u]��ʾu����ɫ�� 
		  �����Զ��������ı�����Ϣ......
		*/
		std::vector<unsigned int> d(ajacencyList.size());
		std::vector<int> p(ajacencyList.size());
		std::vector<Color> color(ajacencyList.size());
		for(unsigned int u=0;u<ajacencyList.size();u++){//��ʼ��������Ϣ
			d[u]=0;
			p[u]=-1;
			color[u]=WHITE;
		}

		/*f(?)�������֮ǰ�û�����*/

		//�����Ƚ��ȳ����н��б���
		std::queue<unsigned int> Q;
		unsigned int x=s;
		do{
			if(color[x]==WHITE){

				/*f(?)�����֧�����㱻����֮ʱ�û�����*/

				color[x]=GRAY;
				Q.push(x);
			}
			//��ĳ��ͨ��֧���б���
			while(!Q.empty()){
				unsigned int u=Q.front();
				Q.pop();

				/*f(?)���붥�㱻��չ֮ǰ�û�����*/

				for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u].begin();iter!=ajacencyList[u].end();iter++){
					unsigned int v=iter->vEd;
					if(color[v]==WHITE){

						/*f(?)���붥�㱻����֮ʱ�û�����*/

						color[v]=GRAY;
						d[v]=d[u]+1;
						p[v]=u;
						Q.push(v);
					}
				}
				color[u]=BLACK;

				/*f(?)���붥��u����չ֮���û�����*/

			}
			x=(x+1)%ajacencyList.size();
		}while(x!=s);//��������ͨ��֧

		/*f(?)������Ϻ���û�����*/

	}

	//ͨ��������ȱ�����ĳ����������������豣֤�ڽӱ���ȷ
	template<typename T>
	void Graph::dfs_visit(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time){
		color[u]=GRAY;
		d[u]=++time;
		
		/*f(?)���붥��u������֮ʱ�û�����*/

		for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u].begin();iter!=ajacencyList[u].end();iter++){
			unsigned int v=iter->vEd;
			if(color[v]==WHITE){
				p[v]=u;
				dfs_visit(ajacencyList,v,d,f,p,color,time);
			}
		}
		color[u]=BLACK;
		f[u]=++time;

		/*f(?)���붥��u���ʽ����û�����*/
		//std::cout<<u<<std::endl;
	}

	//ͨ��������ȱ����������������ɭ�֣����豣֤�ڽӱ���ȷ
	template<typename T>
	void Graph::dfs(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s){
		if(s>=ajacencyList.size())//Դ���㳬���ڽӱ�Χ
			return;
		/*����һЩ������Ϣ������
		  d[u]��ʾu�����ֵ�ʱ�����f[u]��ʾuΪ����������������ϵ�ʱ���
		  p[u]��ʾu�ĸ����㣨p[u]==-1��ʾuΪ������
		  color[u]��ʾu����ɫ�� 
		  time��ʾʱ���������
		  �����Զ��������ı�����Ϣ......
		*/
		std::vector<unsigned int> d(ajacencyList.size());
		std::vector<unsigned int> f(ajacencyList.size());
		std::vector<int> p(ajacencyList.size());
		std::vector<Color> color(ajacencyList.size());
		unsigned int time=0;
		for(unsigned int u=0;u<ajacencyList.size();u++){//��ʼ��������Ϣ
			p[u]=-1;
			color[u]=WHITE;
		}

		/*f(?)�������֮ǰ�û�����*/

		//�����������ɭ��
		unsigned int u=s;
		do{
			if(color[u]==WHITE)
				dfs_visit(ajacencyList,u,d,f,p,color,time);//����ĳ���������
			u=(u+1)%ajacencyList.size();
		}while(u!=s);
		
		/*f(?)���������Ϻ���û�����*/

	}

	//����������ȱ���ȷ����ĳ�ڵ㿪ʼ������������Ƿ����һ��·���豣֤�ڽӱ���ȷ
	template<typename T>
	bool Graph::has_loop_visit(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time,bool isDirectedGraph){
		color[u]=GRAY;
		d[u]=++time;
		for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u].begin();iter!=ajacencyList[u].end();iter++){
			unsigned int v=iter->vEd;
			if(color[v]==WHITE){
				p[v]=u;
				if(true==has_loop_visit(ajacencyList,v,d,f,p,color,time,isDirectedGraph))
					return true;
			}else if(color[v]==GRAY){
				if(isDirectedGraph==true)//����·��ֵĽڵ�Ϊ��ɫ����Ϊ����ͼ��϶����ڻ�·
					return true;
				else if(p[u]!=v)//����·��ֵĽڵ�Ϊ��ɫ����Ϊ����ͼ����u�ĸ��ײ�Ϊv���л���
					return true;
			}
		}
		color[u]=BLACK;
		f[u]=++time;
		return false;
	}

	//����������ȱ���ȷ���Ƿ���ڻ�·���豣֤�ڽӱ���ȷ
	template<typename T>
	bool Graph::hasLoop(const std::vector<std::list<Edge<T>>> &ajacencyList,bool isDirectedGraph){
		if(ajacencyList.empty())//Դ���㳬���ڽӱ�Χ
			return false;
		std::vector<unsigned int> d(ajacencyList.size());
		std::vector<unsigned int> f(ajacencyList.size());
		std::vector<int> p(ajacencyList.size());
		std::vector<Color> color(ajacencyList.size());
		unsigned int time=0;
		for(unsigned int u=0;u<ajacencyList.size();u++){//��ʼ��������Ϣ
			p[u]=-1;
			color[u]=WHITE;
		}
		unsigned int u=0;
		do{
			if(color[u]==WHITE)
				if(true==has_loop_visit(ajacencyList,u,d,f,p,color,time,isDirectedGraph))//����ĳ���������
					return true;
			u=(u+1)%ajacencyList.size();
		}while(u!=0);
		return false;
	}

	//ͨ��������ȱ����������������豣֤�ڽӱ���ȷ
	template<typename T>
	void Graph::topologicalSort_visit(std::vector<unsigned int> &order,const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int u,std::vector<unsigned int> &d,std::vector<unsigned int> &f,std::vector<int> &p,std::vector<Color> &color,unsigned int &time){
		color[u]=GRAY;
		d[u]=++time;
		for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u].begin();iter!=ajacencyList[u].end();iter++){
			unsigned int v=iter->vEd;
			if(color[v]==WHITE){
				p[v]=u;
				topologicalSort_visit(order,ajacencyList,v,d,f,p,color,time);
			}
		}
		color[u]=BLACK;
		f[u]=++time;
		order.push_back(u);
	}


	//ͨ��������ȱ����������������豣֤�ڽӱ���ȷ
	template<typename T>
	std::vector<unsigned int> Graph::topologicalSort(const std::vector<std::list<Edge<T>>> &ajacencyList){
		if(ajacencyList.empty()||hasLoop(ajacencyList))//�ڽӱ�Ϊ�ջ����л�·���򷵻ؿ�����
			return std::vector<unsigned int>();
		std::vector<unsigned int> order;//�����������д洢����
		//����������ȱ������������̼����order���е����ɡ�
		std::vector<unsigned int> d(ajacencyList.size());
		std::vector<unsigned int> f(ajacencyList.size());
		std::vector<int> p(ajacencyList.size());
		std::vector<Color> color(ajacencyList.size());
		unsigned int time=0;
		for(unsigned int u=0;u<ajacencyList.size();u++){
			p[u]=-1;
			color[u]=WHITE;
		}
		unsigned int u=0;
		do{
			if(color[u]==WHITE)
				topologicalSort_visit(order,ajacencyList,u,d,f,p,color,time);
			u=(u+1)%ajacencyList.size();
		}while(u!=0);

		std::reverse(order.begin(),order.end());
		return order;
	}


	//����������ȱ����ҳ����е�ǿ��ͨ��֧���豣֤�ڽӱ���ȷ
	template<typename T>
	void Graph::computeStrngConctComps(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::vector<unsigned int>> &strongConnectComponents){
		strongConnectComponents.clear();
		//������ȱ����������ʱ���
		if(ajacencyList.empty())//�ڽӱ�Ϊ��
			return ;
		std::vector<unsigned int> d(ajacencyList.size());
		std::vector<unsigned int> f(ajacencyList.size());
		std::vector<int> p(ajacencyList.size());
		std::vector<Color> color(ajacencyList.size());
		unsigned int time=0;
		for(unsigned int u=0;u<ajacencyList.size();u++){//��ʼ��������Ϣ
			p[u]=-1;
			color[u]=WHITE;
		}
		unsigned int u=0;
		do{
			if(color[u]==WHITE)
				dfs_visit(ajacencyList,u,d,f,p,color,time);//����ĳ���������
			u=(u+1)%ajacencyList.size();
		}while(u!=0);

		//���㰴����ʱ�����������
		std::vector<unsigned int> order(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++)
			order[i]=i;
		mergeSort(order,f,0,ajacencyList.size()-1);
		std::reverse(order.begin(),order.end());
		//���ת���ڽӱ�
		std::vector<std::list<Edge<T>>> transposedAjacencyList;
		transposeAjacencyList(ajacencyList,transposedAjacencyList);
		//������ɫΪ��ɫ
		for(unsigned int u=0;u<ajacencyList.size();u++)
			color[u]=WHITE;
		//������ʱ����������ж�ת���ڽӱ������ȱ�����������ͨ��֧
		for(unsigned int i=0;i<order.size();i++)
		{
			if(color[order[i]]==WHITE){
				strongConnectComponents.push_back(std::vector<unsigned int>());
				strongConnectComponents.back().push_back(order[i]);
				dfs_visit(transposedAjacencyList,order[i],d,f,p,color,time);
			}else
				strongConnectComponents.back().push_back(order[i]);
		}
	}

	//����������ȱ�������ŷ����������ĳ����������������豣֤�ڽӱ�ajacencyList��ȷ������ŷ��������true���������EulerCircuit�������򷵻�false�������ܷ���false
	template<typename T>
	static bool Graph::computeEulerCircuit_visit(std::vector<std::list<Edge<T>>> &ajacencyList,std::list<Edge<T>> &EulerCircuit,const unsigned int s,bool isDirectedGraph,CompleteHash<unsigned long> &hash){
		while(!ajacencyList[s].empty()){
			Edge<T> e=ajacencyList[s].front();
			ajacencyList[s].pop_front();
			if(isDirectedGraph==false&&hash.search(std::max(e.vSt,e.vEd)*ajacencyList.size()+std::min(e.vSt,e.vEd))!=NULL)//���Ϊ����ͼ�Ҹñ�����չ����������
				continue;
			//if(!EulerCircuit.empty()&&EulerCircuit.back().vEd!=s)//û��Ҫ���м�顣
				//return false;
			EulerCircuit.push_back(e);
			if(isDirectedGraph==false)
				hash.insert(std::max(e.vSt,e.vEd)*ajacencyList.size()+std::min(e.vSt,e.vEd));
			if(false==computeEulerCircuit_visit(ajacencyList,EulerCircuit,e.vEd,isDirectedGraph,hash))
				return false;
		}
		if(!EulerCircuit.empty()){
			if(EulerCircuit.back().vEd!=EulerCircuit.front().vSt)//�����ǰ�ڵ���չ��ϣ���EulerCircuit����Ϊŷ����·
				return false;
			EulerCircuit.push_front(EulerCircuit.back());
			EulerCircuit.pop_back();
		}
		return true;
	}

	//����������ȱ�������ŷ��������������������������ֻ����ŷ����������һ������������У����豣֤�ڽӱ�ajacencyList��ȷ������ŷ��������true���������EulerCircuit�������򷵻�false��EulerCircuit�ÿգ���
	template<typename T>
	static bool Graph::computeEulerCircuit(const std::vector<std::list<Edge<T>>> &ajacencyList,std::list<Edge<T>> &EulerCircuit,bool isDirectedGraph){
		EulerCircuit.clear();
		if(ajacencyList.empty())//�ڽӱ�Ϊ�գ�����ڿյ�ŷ����·
			return true;
		std::vector<std::list<Edge<T>>> ajacencyListCopy=ajacencyList;
		//���Ϊ����ͼ���������������ù�ϣ�����������ȫ��ɢ��
		CompleteHash<unsigned long> hash(1,lmtc::MAX_PRIME,key_to_number);
		if(isDirectedGraph==false){
			unsigned int count=1;
			for(unsigned int i=0;i<ajacencyList.size();i++)
				count+=ajacencyList[i].size();
			hash.resetSlotNumber(count);
		}
		unsigned int u=0;
		do{
			if(EulerCircuit.empty()&&!ajacencyListCopy[u].empty()){
				if(false==computeEulerCircuit_visit(ajacencyListCopy,EulerCircuit,u,isDirectedGraph,hash)){
					EulerCircuit.clear();
					return false;
				}
			}else if(!EulerCircuit.empty()&&!ajacencyListCopy[u].empty()){
				EulerCircuit.clear();
				return false;
			}
			u=(u+1)%ajacencyListCopy.size();
		}while(u!=0);
		return true;
	}

	//Kruskal������С������
	template<typename T>
	static T Graph::mstKruskal(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &mstAjacencyList){
		mstAjacencyList.clear();
		mstAjacencyList.resize(ajacencyList.size());
		if(ajacencyList.empty())
			return 0;
		//ÿ�����㹹��һ����
		std::vector<lmtc::NonIntersectSet<unsigned int>> setVec;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			setVec.push_back(NonIntersectSet<unsigned int>(i));
		//���ڽӱ��ñ߼������Ͷ�Ӧ��Ȩֵ�����������߼�������Ȩֵ����
		std::vector<Edge<T>> edgVec;
		std::vector<T> valueVec;
		for(unsigned int i=0;i<ajacencyList.size();i++){
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt<iter->vEd){
					edgVec.push_back(*iter);
					valueVec.push_back(iter->data);
				}
			}
		}
		sort(edgVec,valueVec);
		T treeWeight=0;
		//��Ȩֵ��С�����Ǹ��ߣ�������˵㲻����ͬһ�����򽫱߼�����С���������ڽӱ����ϲ�������
		for(std::vector<Edge<T>>::iterator iter=edgVec.begin();iter!=edgVec.end();iter++){
			if(!(setVec[iter->vSt].find()==setVec[iter->vEd].find())){
				setVec[iter->vSt].unionSet(setVec[iter->vEd]);
				mstAjacencyList[iter->vSt].push_back(*iter);
				mstAjacencyList[iter->vEd].push_back(Edge<T>(iter->vEd,iter->vSt,iter->data));
				treeWeight+=iter->data;
			}
		}
		return treeWeight;
	}

	//Prim������С������
	template<typename T>
	static T Graph::mstPrim(const std::vector<std::list<Edge<T>>> &ajacencyList,std::vector<std::list<Edge<T>>> &mstAjacencyList){	
		mstAjacencyList.clear();
		mstAjacencyList.resize(ajacencyList.size());
		if(ajacencyList.empty())
			return 0;
		//������Ȩ������
		T max=0;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				if(max<iter->data)
					max=iter->data;
		max+=1;
		//�洢ÿ��������Ϣ��쳲������ѺͶѽڵ�������������Ϣ�洢��Edge<T>�У�vSt��ʾ����ţ�vEd��ʾvSt���ӵ���ǰ��������������㣬data��ʾ���ӵ���ǰ����������СȨ��
		FibonacciHeap<Edge<T>> Q(pri_comp_edge);
		std::vector<FibonacciHeap<Edge<T>>::ItemType> itemVec;
		//�����Ƿ��������ȶ�������
		std::vector<bool> blgVec(ajacencyList.size(),true);
		//�Ӷ���0��ʼ
		itemVec.push_back(Q.insert(Edge<T>(0,0,0)));
		//��ʼ����������Ϣ��������쳲������ѺͶѽڵ�����
		for(unsigned int i=1;i<ajacencyList.size();i++)
			itemVec.push_back(Q.insert(Edge<T>(i,i,max)));
		while(!Q.isEmpty()){
			//��ȡ��С�ڵ���Ϊ����չ���㣬��������֮�����������Ϣ
			Edge<T> u=Q.getItemValue(Q.extract_prio());
			blgVec[u.vSt]=false;
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u.vSt].begin();iter!=ajacencyList[u.vSt].end();iter++){
				Edge<T> v=Q.getItemValue(itemVec[iter->vEd]);
				if(blgVec[v.vSt]&&iter->data<v.data)
					Q.increaseKey(itemVec[v.vSt],Edge<T>(v.vSt,u.vSt,iter->data));
			}
		}
		T treeWeight=0;
		//����С�������������mstAjacencyList
		for(unsigned int i=1;i<ajacencyList.size();i++){
			Edge<T> u=Q.getItemValue(itemVec[i]);
			if(u.vSt==u.vEd){
				continue;
			}
			mstAjacencyList[u.vSt].push_back(u);
			mstAjacencyList[u.vEd].push_back(Edge<T>(u.vEd,u.vSt,u.data));
			treeWeight+=u.data;
		}
		return treeWeight;
	}

	template<typename T>
	static bool Graph::shortestPathBellmanFord(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d){
		p.clear();
		d.clear();
		if(ajacencyList.empty())
			return true;
		if(s>=ajacencyList.size())
			throw GraphException("�쳣��shortestPathBellmanFord�㷨��ԭ������ų�����Χ��");
		//������Ȩ������
		T max=0;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				if(max<iter->data)
					max=iter->data;
		max+=1;
		//��ʼ�����ڵ������;�������,ԭ���������
		p.resize(ajacencyList.size());
		d.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++){
			p[i]=i;
			d[i]=max*ajacencyList.size();
		}
		d[s]=0;
		//����ajacencyList.size()-1������бߵ��ɳڣ��Դ˹������·����
		for(unsigned int c=1;c<ajacencyList.size();c++){
			for(unsigned int i=0;i<ajacencyList.size();i++)
				for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				{
					if(d[iter->vEd]>d[iter->vSt]+iter->data)
					{
						d[iter->vEd]=d[iter->vSt]+iter->data;
						p[iter->vEd]=iter->vSt;
					}
				}
		}
		//�����ʱ���б߿��ɳڣ���ش��ڸ���·
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				if(d[iter->vEd]>d[iter->vSt]+iter->data)
					return false;
		return true;
	}

	template<typename T>
	static bool Graph::shortestPathOfDag(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d){
		p.clear();
		d.clear();
		if(s>=ajacencyList.size())
			throw GraphException("�쳣��shortestPathOfDag�㷨��ԭ������ų�����Χ��");
		std::vector<unsigned int> order=topologicalSort(ajacencyList);
		if(order.empty())//��ʾ�л����ͼ
			return false;
		//������Ȩ������
		T max=0;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++)
				if(max<iter->data)
					max=iter->data;
		max+=1;
		//��ʼ�����ڵ������;�������,ԭ���������
		p.resize(ajacencyList.size());
		d.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++){
			p[i]=i;
			d[i]=max*ajacencyList.size();
		}
		d[s]=0;
		//����������˳��Ը�����ı߽����ɳ�
		for(unsigned int i=0;i<order.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[order[i]].begin();iter!=ajacencyList[order[i]].end();iter++)
			{
				if(d[iter->vEd]>d[iter->vSt]+iter->data)
				{
					d[iter->vEd]=d[iter->vSt]+iter->data;
					p[iter->vEd]=iter->vSt;
				}
			}
		return true;
	}

	template<typename T>
	static bool  Graph::shortestPathDijkstra(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,std::vector<unsigned int> &p,std::vector<T> &d){
		p.clear();
		d.clear();
		if(ajacencyList.empty())
			return true;
		if(s>=ajacencyList.size())
			throw GraphException("�쳣��shortestPathBellmanFord�㷨��ԭ������ų�����Χ��");

		//������·��Ȩ������,��ȷ�������ڸ���,������ڸ��ߣ�����false��
		T max=0;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->data<0)
					return false;
				if(max<iter->data)
					max=iter->data;
			}
		max*=ajacencyList.size();
		
		//�洢ÿ��������Ϣ��쳲������ѺͶѽڵ�������������Ϣ�洢��Edge<T>�У�vSt��ʾ����ţ�vEd��ʾvSt�ĵ�ǰ���·�����ڵ㣬data��ʾ�ö��㵱ǰ�����·����������
		FibonacciHeap<Edge<T>> Q(pri_comp_edge);
		std::vector<FibonacciHeap<Edge<T>>::ItemType> itemVec;
		//��ʼ����������Ϣ��������쳲������ѺͶѽڵ�����
		for(unsigned int i=0;i<ajacencyList.size();i++)
			if(i==s)
				itemVec.push_back(Q.insert(Edge<T>(i,i,0)));
			else
				itemVec.push_back(Q.insert(Edge<T>(i,i,max)));
		while(!Q.isEmpty()){
			//��ȡ��С�ڵ���Ϊ����չ���㣬���ɳ���֮�����ĸ���
			Edge<T> u=Q.getItemValue(Q.extract_prio());
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[u.vSt].begin();iter!=ajacencyList[u.vSt].end();iter++){
				Edge<T> v=Q.getItemValue(itemVec[iter->vEd]);
				if(v.data>u.data+iter->data)
					Q.increaseKey(itemVec[v.vSt],Edge<T>(v.vSt,u.vSt,u.data+iter->data));
			}
		}
		//�����·�����������p,d,����true��
		p.resize(ajacencyList.size());
		d.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++){
			Edge<T> u=Q.getItemValue(itemVec[i]);
			p[i]=u.vEd;
			d[i]=u.data;
		}
		return true;
	}

	template<typename T>
	static void Graph::shortestPathAllFloydWarshall(const std::vector<std::list<Edge<T>>> &ajacencyList,Array<unsigned int> &p,Array<T> &d){
		d.clear();
		p.clear();
		if(ajacencyList.empty())
			return ;
		//������ʱ�������͸��ڵ����
		Array<T> d0(2,ajacencyList.size(),ajacencyList.size());
		Array<T> d1(2,ajacencyList.size(),ajacencyList.size());
		Array<unsigned int> p0(2,ajacencyList.size(),ajacencyList.size());
		Array<unsigned int> p1(2,ajacencyList.size(),ajacencyList.size());
		//������·��Ȩ�����ޡ�
		T max=0;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(max<iter->data)
					max=iter->data;
			}
		max*=ajacencyList.size();
		//��ʼ����ʱ�������͸��ڵ����
		d0.initialize(max);
		d1.initialize(max);
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(unsigned int j=0;j<ajacencyList.size();j++){
				p0(i,j)=j;
				p1(i,j)=j;
			}
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				d0(iter->vSt,iter->vEd)=iter->data;
				d1(iter->vSt,iter->vEd)=iter->data;
				p0(iter->vSt,iter->vEd)=iter->vSt;
				p1(iter->vSt,iter->vEd)=iter->vSt;
			}
		for(unsigned int i=0;i<ajacencyList.size();i++){
			d0(i,i)=0;
			d1(i,i)=0;
		}
		//��̬�滮�����������������͸��ڵ����
		for(unsigned int k=0;k<ajacencyList.size();k++)
			for(unsigned int i=0;i<ajacencyList.size();i++)
				for(unsigned int j=0;j<ajacencyList.size();j++){
					if(k%2==0){
						d1(i,j)=std::min(d0(i,j),d0(i,k)+d0(k,j));
						if(d0(i,j)>d0(i,k)+d0(k,j))
							p1(i,j)=p0(k,j);
						else
							p1(i,j)=p0(i,j);
					}else{
						d0(i,j)=std::min(d1(i,j),d1(i,k)+d1(k,j));
						if(d1(i,j)>d1(i,k)+d1(k,j))
							p0(i,j)=p1(k,j);
						else
							p0(i,j)=p1(i,j);
					}
				}
		//�������ս����p��d
		if(ajacencyList.size()%2==0){
			p=p0;
			d=d0;
		}else{
			p=p1;
			d=d1;
		}
	}

	template<typename T>
	static bool Graph::shortestPathAllJohnson(const std::vector<std::list<Edge<T>>> &ajacencyList,Array<unsigned int> &p,Array<T> &d){
		unsigned int V=ajacencyList.size();
		//��������ͼ��������hv��dV��
		std::vector<std::list<Edge<T>>> ajacencyList1=ajacencyList;
		ajacencyList1.push_back(std::list<Edge<T>>());
		for(unsigned int i=0;i<V;i++)
			ajacencyList1[V].push_back(Edge<T>(V,i,0));
		std::vector<unsigned int> pV;
		std::vector<T> dV;//���൱��hv
		bool notHasNegLoop=shortestPathBellmanFord(ajacencyList1,V,pV,dV);
		if(notHasNegLoop==false){
			p.clear();
			d.clear();
			return false;
		}
		//�ظ�Ȩ
		ajacencyList1.pop_back();
		for(unsigned int i=0;i<ajacencyList1.size();i++)
			for(std::list<Edge<T>>::iterator iter=ajacencyList1[i].begin();iter!=ajacencyList1[i].end();iter++)
				iter->data+=(dV[iter->vSt]-dV[iter->vEd]);
		
		//��ÿ����������Dijkstra�㷨
		std::vector<unsigned int> pTemp;
		std::vector<T> dTemp;
		Array<unsigned int> pM(2,V,V);
		Array<T> dM(2,V,V);
		for(unsigned int s=0;s<V;s++){
			shortestPathDijkstra(ajacencyList1,s,pTemp,dTemp);
			for(unsigned int i=0;i<V;i++){
				pM(s,i)=pTemp[i];
				dM(s,i)=dTemp[i]+dV[i]-dV[s];
			}
		}
		//�洢���ս��
		p=pM;
		d=dM;
		return true;
	}

	template<typename T>
	static T Graph::maximumFlowFordFulkerson_EdmondsKarp(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,const unsigned int t,std::vector<std::list<Edge<T>>> &flow){
		if(ajacencyList.empty())
		{	
			flow.clear();
			return 0;
		}
		if(s>=ajacencyList.size()||t>=ajacencyList.size()||s==t)
			throw GraphException("�쳣��maximumFlowFordFulkerson_EdmondsKarp�㷨��ʼĩ������ų�����Χ�������ͬ��");
		//�����ڽӱ�����������ͼ�ṹ������ɢ�й����Ӧ����
		unsigned int count=1;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			count+=ajacencyList[i].size();
		std::vector<std::list<Edge<T>>> flowGraph;
		flowGraph.resize(ajacencyList.size());
		CompleteHash<Edge<T>> flowHash(count*2,lmtc::MAX_PRIME,edge_to_number,edge_equal);
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				if(iter->data<0)
					throw GraphException("�쳣��maximumFlowFordFulkerson_EdmondsKarp�㷨�д��ڸ��ߣ�");
				flowGraph[iter->vSt].push_back(*iter);
				flowHash.insert(Edge<T>(iter->vSt,iter->vEd,0));
			}
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				if(flowHash.search(Edge<T>(iter->vEd,iter->vSt))==NULL){
					flowGraph[iter->vEd].push_back(Edge<T>(iter->vEd,iter->vSt,0));
					flowHash.insert(Edge<T>(iter->vEd,iter->vSt,0));
				}
			}
		flowHash.completeHashOptimize();//��ȫɢ��

		//�������ù����������Ѱ��s��t������·��
		while(true){
			//��ʼ��������Ϣ
			std::vector<unsigned int> d(ajacencyList.size());
			std::vector<int> p(ajacencyList.size());
			std::vector<Color> color(ajacencyList.size());
			std::vector<T> cfp(ajacencyList.size());//������������·���ܹ����ӵ��������ֵ

			for(unsigned int u=0;u<ajacencyList.size();u++){
				d[u]=0;
				p[u]=-1;
				color[u]=WHITE;
				cfp[u]=-1;
			}
			//�����Ƚ��ȳ����н��б���
			std::queue<unsigned int> Q;
			color[s]=GRAY;
			Q.push(s);
			//����sΪ������ͨ��֧���б���
			while(!Q.empty()){
				unsigned int u=Q.front();
				Q.pop();
				for(std::list<Edge<T>>::const_iterator iter=flowGraph[u].begin();iter!=flowGraph[u].end();iter++){
					T temp=iter->data-flowHash.search(Edge<T>(iter->vSt,iter->vEd))->data;
					if(temp<=0.0000000001)//���⸡������������Ϊ��
						continue;
					unsigned int v=iter->vEd;
					if(color[v]==WHITE){
						color[v]=GRAY;
						d[v]=d[u]+1;
						p[v]=u;
						Q.push(v);
						if(u==s||temp<cfp[u])
							cfp[v]=temp;
						else
							cfp[v]=cfp[u];
					}
				}
				color[u]=BLACK;
			}
			
			if(p[t]==-1)//�Ҳ�������·��
				break;
			unsigned int cur=t;
			while(p[cur]!=-1){
				flowHash.search(Edge<T>(p[cur],cur))->data+=cfp[t];
				flowHash.search(Edge<T>(cur,p[cur]))->data-=cfp[t];
				cur=p[cur];
			}
		}
		//��������������flow���������������
		flow.clear();
		flow.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				T temp=flowHash.search(*iter)->data;
				if(temp>0)
					flow[iter->vSt].push_back(Edge<T>(iter->vSt,iter->vEd,temp));
			}
		T maxF=0;
		for(std::list<Edge<T>>::const_iterator iter=flow[s].begin();iter!=flow[s].end();iter++)
			maxF+=iter->data;
		return maxF;
	}

	//ѹ���ر��ǰ���㷨
	template<typename T>
	static T Graph::maximumFlowPushRelabelToFront(const std::vector<std::list<Edge<T>>> &ajacencyList,const unsigned int s,const unsigned int t,std::vector<std::list<Edge<T>>> &flow){
		if(ajacencyList.empty())
		{	
			flow.clear();
			return 0;
		}
		if(s>=ajacencyList.size()||t>=ajacencyList.size()||s==t)
			throw GraphException("�쳣��maximumFlowPushRelabelToFront�㷨��ʼĩ������ų�����Χ�������ͬ��");
		//�����ڽӱ�����������ͼ�ṹ������ɢ�й����Ӧ��ǰ����
		unsigned int count=1;
		for(unsigned int i=0;i<ajacencyList.size();i++)
			count+=ajacencyList[i].size();
		std::vector<std::list<Edge<T>>> flowGraph;
		flowGraph.resize(ajacencyList.size());
		CompleteHash<Edge<T>> flowHash(count*2,lmtc::MAX_PRIME,edge_to_number,edge_equal);
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				if(iter->data<0)
					throw GraphException("�쳣��maximumFlowPushRelabelToFront�㷨�д��ڸ��ߣ�");
				flowGraph[iter->vSt].push_back(*iter);
				flowHash.insert(Edge<T>(iter->vSt,iter->vEd,0));
			}
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				if(flowHash.search(Edge<T>(iter->vEd,iter->vSt))==NULL){
					flowGraph[iter->vEd].push_back(Edge<T>(iter->vEd,iter->vSt,0));
					flowHash.insert(Edge<T>(iter->vEd,iter->vSt,0));
				}
			}
		flowHash.completeHashOptimize();//��ȫɢ��

		//��ʼ��ǰ����
		std::vector<unsigned int> h(flowGraph.size(),0);
		std::vector<T> e(flowGraph.size(),0);
		h[s]=flowGraph.size();
		for(std::list<Edge<T>>::const_iterator iter=flowGraph[s].begin();iter!=flowGraph[s].end();iter++){
			flowHash.search(*iter)->data=iter->data;
			flowHash.search(Edge<T>(iter->vEd,iter->vSt))->data=-iter->data;
			e[iter->vEd]=iter->data;
			e[s]-=iter->data;
		}

		//Lά�����������ж������������
		std::list<unsigned int> L;
		for(unsigned int i=0;i<flowGraph.size();i++)
			if(i!=s&&i!=t)
				L.push_back(i);
		//currentά���ڽӱ�flowGraph������ĵ�ǰָ�롣
		std::vector<std::list<Edge<T>>::const_iterator> current;
		for(unsigned int i=0;i<flowGraph.size();i++)
			current.push_back(flowGraph[i].begin());
		std::list<unsigned int>::const_iterator u=L.begin();
		while(u!=L.end()){
			unsigned int old_height=h[*u];
			//DISCHARGE(u)
			while(e[*u]>0.0000000001){//��e[*u]>0
				std::list<Edge<T>>::const_iterator v=current[*u];
				if(v==flowGraph[*u].end()){
					//RELABEL(u)
					unsigned int minH=flowGraph.size()*2;
					for(std::list<Edge<T>>::const_iterator iter=flowGraph[*u].begin();iter!=flowGraph[*u].end();iter++)
						if(iter->data-flowHash.search(*iter)->data>0.0000000001)
							minH=std::min(minH,h[iter->vEd]);
					h[*u]=minH+1;
					current[*u]=flowGraph[*u].begin();
				}
				else if((v->data-flowHash.search(*v)->data>0.0000000001)&&((h[v->vSt]-h[v->vEd])==1)){
					//PUSH(u,v)
					T df=std::min(e[*u],v->data-flowHash.search(*v)->data);
					flowHash.search(*v)->data+=df;
					flowHash.search(Edge<T>(v->vEd,v->vSt))->data-=df;
					e[*u]-=df;
					e[v->vEd]+=df;
				}
				else
					current[*u]++;
			}
			if(h[*u]>old_height){
				unsigned int tempU=*u;
				L.erase(u);
				L.push_front(tempU);
				u=++L.begin();
			}
			else
				u++;
		}
	
		//��������������flow���������������
		flow.clear();
		flow.resize(ajacencyList.size());
		for(unsigned int i=0;i<ajacencyList.size();i++)
			for(std::list<Edge<T>>::const_iterator iter=ajacencyList[i].begin();iter!=ajacencyList[i].end();iter++){
				if(iter->vSt==iter->vEd||iter->vSt==t||iter->vEd==s)//����������Ի���s������Լ�t�ĳ���
					continue;
				T temp=flowHash.search(*iter)->data;
				if(temp>0)
					flow[iter->vSt].push_back(Edge<T>(iter->vSt,iter->vEd,temp));
			}
		T maxF=0;
		for(std::list<Edge<T>>::const_iterator iter=flow[s].begin();iter!=flow[s].end();iter++)
			maxF+=iter->data;
		return maxF;
	}
	

}

#endif