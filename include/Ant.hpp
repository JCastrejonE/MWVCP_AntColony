#ifndef ANT_H
#define ANT_H

#include <unordered_set>
#include "Graph.hpp"

class Ant
{
private:
public:
  int id;
  Graph CV;
  std::unordered_set<int> solution;
  int solutionCost;
  int edgesMissing;
  int actualVertex;

  Ant(int k, int n){ this->id = k; this->CV = Graph(n);};
};

#endif