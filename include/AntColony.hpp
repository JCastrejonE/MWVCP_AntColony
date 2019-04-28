#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include <vector>
#include <unordered_set>
#include <random>
#include "Graph.hpp"
#include "Ant.hpp"

using namespace std;

#define ANTS 10
#define RELATIVE_SIGNIFICANCE 1
#define THRESHOLD 0.5
#define EVAPORATION_RATE 0.2
#define PHI 0.12
#define MAX_IMPROVE_ATTEMPTS 150

class AntColony
{
private:
  int n;
  int m;
  Graph G;
  Graph GC;
  vector<Ant *> ants;
  vector<double> globalPheromone;
  double initialPheromone;
  unordered_set<int> bestSolution;
  int bestCost;
  int improveAttempts;
  default_random_engine dre;
  uniform_real_distribution<double> urd;

  void computeGComplete();
  void resetAnt(Ant *);
  void updateConnectivityValue(Ant *, int);
  void computeStep(Ant *);
  void computeCycle();
  void antTransition(Ant *, int);
  void compareSolution(unordered_set<int> &, int);
  void computeInitialSolution();

public:
  AntColony(int, int);
  ~AntColony();
  void addEdge(int, int);
  void setWeight(int, int);
  void setRandomEngine(int);
  pair<unordered_set<int>, int> computeSolution();
};

#endif