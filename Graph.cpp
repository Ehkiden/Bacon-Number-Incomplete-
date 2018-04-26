#ifndef GRAPH_CPP
#define GRAPH_CPP

// It provides the implementation of the Graph class
#include <list>     // create a queue from a list
#include <cassert>
#include "Graph.h"
#include <set>
#include <queue>

// default constructor
Graph::Graph(int numVertices) :adj(Matrix<int>(numVertices, numVertices, -1))
{
}

bool Graph::hasEdge(int v, int w)
{
	assert(v >= 0 && v < adj.GetSizeX() && w >= 0 && w < adj.GetSizeX());
	if (adj(v, w) == -1)
		return false;
	return true;
}

void Graph::addEdge(int v, int w, int edge)
{
	adj(v, w) = edge;
	adj(w, v) = edge;

}


int Graph::getEdge(int v, int w)
{
	return adj(v, w);
}


void Graph::BFS(int s, vector<int>& distance, vector<int>& go_through)
{
	go_through[s] = -1;
	int current;
	set<int> S;
	queue<int> Q;
	S.insert(s);
	distance[s] = 0;
	Q.push(s);
	while (!Q.empty()) {
		current = Q.front();
		Q.pop();

		for (int i = 0; i < adj.GetSizeX(); i++) {
			if (hasEdge(current, i) && (S.count(i) != 1)) {
				Q.push(i);
				S.insert(i);

				go_through[i] = current;

				distance[i] = distance[current] + 1;
			}
		}
	}

}

#endif