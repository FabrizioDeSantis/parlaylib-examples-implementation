#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct EdgeSeq {
	int src, dest;
};

class GraphSeq
{
public:
	vector<vector<int>> adjList;
	//costruttore
	GraphSeq(vector<EdgeSeq> const &edges, int n)
	{
		adjList.resize(n);
		for (auto &edge: edges)
		{
			adjList[edge.src].push_back(edge.dest);
			adjList[edge.dest].push_back(edge.src);
		}
	}
};

void BFSSeq(GraphSeq const &graph, int v, vector<bool> &discovered)
{
	queue<int> q;

	discovered[v] = true;
	q.push(v);

	// loop till queue is empty
	while (!q.empty())
	{
		// dequeue front node and print it
		v = q.front();
		q.pop();

		// do for every edge (v, u)
		for (int u: graph.adjList[v])
		{
			if (!discovered[u])
			{
				// mark it as discovered and enqueue it
				discovered[u] = true;
				q.push(u);
			}
		}
	}
}