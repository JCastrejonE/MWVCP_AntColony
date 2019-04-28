#ifndef GRAPH_H
#define GRAPH_H

class Graph
{
private:
  int order;
  double **G;

public:
  Graph();
  Graph(int);
  void updateEdge(int, int, double);
  double getWeight(int, int);
};

#endif
