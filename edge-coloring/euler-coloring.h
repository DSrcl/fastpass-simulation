#ifndef __euler_coloring_h__
#define __euler_coloring_h__

#include <cstdint>
#include <vector>
#include <queue>
#include <map>

using namespace std;

typedef uint8_t Vertex; // id of vertex
typedef uint64_t Links; // bitmap of vertices incident to a vertex
typedef uint64_t Edge; // bitmap with two bits set
typedef vector<Edge> Path;
typedef vector<Edge> ColorGroup;

// a graph is  a list of bitmaps for every vertices,
// set bits of which are vertices incident to vertex that the bitmap belongs to



class Graph {
private:
	vector<Links> graph;
	map<Edge, int> edgeCounts;
	vector<Path> eulerPartition();
	uint8_t getDegree(Vertex v);	
	Vertex getNeighbor(Vertex v);
	queue<Vertex> loadVertices();
	void removeEdge(Edge edge);
	void colorGraph(vector<ColorGroup>& coloring);
public:
	Graph(const vector<Edge>& edges);
	Graph(Graph* g);
	void eulerSplit(queue<Graph>& graphs);
	vector<ColorGroup> eulerColoring();
	uint8_t getMaxDegree();
	

};


#endif
