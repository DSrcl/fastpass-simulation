#include <cstdint>
#include <queue>
#include <vector>
#include <tuple>
#include <assert.h>
#include "euler-coloring.h"

#include <iostream>
#include <bitset>



using namespace std;


uint64_t getZeroMask(uint8_t x)
{
	return 1 << (x - 1);	
}


uint64_t getOneMask(uint8_t x)
{
	return ~(getZeroMask(x));
}



Edge makeEdge(Vertex v, Vertex w)
{
	Edge e = 0;
	e |= (getZeroMask(v + 1));
	e |= (getZeroMask(w + 1));
	return e;	
}



int getGraphSizeFromEdges(const vector<Edge>& edges) {
	int size = 0;
	for (int i = 0; i < edges.size(); i++) {
		int curSize = fls(edges[i]);
		if (curSize > size) {
			size = curSize;
		}
	}

	return size;
}




Graph::Graph(Graph* g)
{
	this->graph = vector<Links>(g->graph);	
	this->edgeCounts = g->edgeCounts;
}


Graph::Graph(const vector<Edge>& edges)
{
	this->graph = vector<Links>(getGraphSizeFromEdges(edges));
	for (int i = 0; i < edges.size(); i++) {
		Edge edge = edges[i];
		if (edgeCounts[edge]++ == 0) {
			Vertex v = fls(edge);
			Vertex w = ffs(edge);
			this->graph[v - 1] |= getZeroMask(w);
			this->graph[w - 1] |= getZeroMask(v);	
		}	
	}
}




uint8_t Graph::getDegree(Vertex v)
{
	uint8_t degree = 0;
	uint8_t removedBits = 0;
	uint8_t firstOne;
	Links links = graph[v]; 
	while ((firstOne = ffs(links)) != 0) {
		Edge edge = makeEdge(v, firstOne + removedBits - 1);
		degree += edgeCounts[edge];
		removedBits += firstOne;
		links >>= firstOne;
	}
	

	return degree;
}




uint8_t Graph::getMaxDegree()
{
	uint8_t maxDegree = 0;
	for (Vertex v = 0; v < graph.size(); v++) {
		uint8_t degree = getDegree(v);

		if (degree > maxDegree) {
			maxDegree = degree;
		}
	}
	return maxDegree;
}






Vertex Graph::getNeighbor(Vertex v)
{
	return ffs(graph[v]) - 1; // ids of vertices are 0-base
}



queue<Vertex> Graph::loadVertices()
{
	queue<Vertex> vertices;
	vector<uint8_t> degrees;

	for (Vertex v = 0; v < graph.size(); v++) {
		uint8_t degree = getDegree(v);
		degrees.push_back(degree);	
	}

	// push odd degree vertix
	for (int i = 0; i < degrees.size(); i++) {
		uint8_t degree = degrees[i];
		Vertex v = i;
		if (degree % 2 != 0) {
			vertices.push(v);
		}
	}

	// push even degree vertix
	for (int i = 0; i < degrees.size(); i++) {
		uint8_t degree = degrees[i];
		Vertex v = i;
		if (degree % 2 == 0) {
			vertices.push(v);
		}
	}

	return vertices;
}





void Graph::removeEdge(Edge edge)
{
	assert(edgeCounts[edge] > 0);
	if (--edgeCounts[edge] == 0) {
		Vertex v = fls(edge);
		Vertex w = ffs(edge);	
		graph[v - 1] &= getOneMask(w);
		graph[w - 1] &= getOneMask(v);		
	}
}



vector<Path> Graph::eulerPartition()
{
	vector<Path> paths;
	Graph temp_graph (this);
	queue<Vertex> vertices = temp_graph.loadVertices();	
	while (vertices.size() > 0) {
		Vertex v = vertices.front();
		vertices.pop();
		Vertex start = v;
		Path path; 

		while (temp_graph.getDegree(v) > 0) {
			Vertex w = temp_graph.getNeighbor(v);
			
			if (temp_graph.getDegree(w) > 0) {
				Edge edge = makeEdge(v, w);
				path.push_back(edge);
				temp_graph.removeEdge(edge);	
			} 
			
			v = w;	
		}

		if (temp_graph.getDegree(start) > 0) {
			vertices.push(start);
		}

		if (path.size() > 0) {
			paths.push_back(path);
		}
	}

	return paths;
}

// find last set bit
uint8_t fls(uint64_t x)
{
	uint8_t bits = 0;
	uint8_t firstBit;
	while ((firstBit = ffs(x)) > 0) {
		bits += firstBit;
		x >>= firstBit;
	}
	return bits;
}


void Graph::eulerSplit(queue<Graph>& graphs)
{
	vector<Path> paths = eulerPartition();
	vector<Edge> edgeGroup1;
	vector<Edge> edgeGroup2;
	for (int i = 0; i < paths.size(); i++) {
		Path path = paths[i];

		for (int j = 0; j < path.size(); j++) {
			Edge edge = path[j];
			
			if (j % 2 == 0) {
				edgeGroup1.push_back(edge);
			} else {
				edgeGroup2.push_back(edge);
			}
		}
	}
	
	Graph graph1 = Graph(edgeGroup1);
	Graph graph2 = Graph(edgeGroup2);
	

	graphs.push(graph1);
	graphs.push(graph2);
}



void Graph::colorGraph(vector<ColorGroup>& coloring)
{
	Graph tempGraph (this);
	ColorGroup cgroup;
	for (Vertex v = 0; v < tempGraph.graph.size(); v++) {
		while (tempGraph.getDegree(v) > 0) {
			Vertex w = tempGraph.getNeighbor(v);
			Edge edge = makeEdge(v, w);
			tempGraph.removeEdge(edge);
			cgroup.push_back(edge);	
		}
	}
	coloring.push_back(cgroup);	
}


vector<ColorGroup> Graph::eulerColoring()
{
	vector<ColorGroup> coloring;
	queue<Graph> subgraphs;
	subgraphs.push(*this);
	while (subgraphs.size() > 0) {
		Graph curGraph = subgraphs.front();
		subgraphs.pop();
		if (curGraph.getMaxDegree() == 1) {
			curGraph.colorGraph(coloring);
		} else {
			curGraph.eulerSplit(subgraphs);
		}
	}
	return coloring;
}







// --------------------stuff for debugging -----------------


int main()
{
	vector<Edge> edges;
	queue<Graph> graphs;
	edges.push_back(0b100001); 
	edges.push_back(0b100010);
	edges.push_back(0b100010);
	edges.push_back(0b100100);
	edges.push_back(0b001010);
	edges.push_back(0b001100);
	

	Graph graph (edges);
	vector<ColorGroup> colors = graph.eulerColoring();		

	for (int i = 0; i < colors.size(); i++) {
		cout << "color" << i << " :" << endl;
		for (int j = 0; j < colors[i].size(); j++) {
			Edge edge = colors[i][j];
			bitset<10> x (edge);
			cout << x << ", ";
		}
		cout << "" << endl;
	}	

	
	return 0;
}

















