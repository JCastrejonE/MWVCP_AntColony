#include <random>
#include <algorithm>
#include "AntColony.hpp"
#include "Graph.hpp"

using namespace std;

AntColony::AntColony(int n, int m)
{
  this->n = n;
  this->m = m;
  this->G = Graph(n);
  this->GC = Graph(n);
  this->improveAttempts = 0;
  this->bestCost = 0;
  for (int k = 0; k < ANTS; k++)
  {
    Ant *a = new Ant(k, this->n);
    this->ants.push_back(a);
  }
}

AntColony::~AntColony()
{
  for (auto x : this->ants)
  {
    delete x;
  }
  printf("Ants destroyed.\n");
}

pair<unordered_set<int>, int> AntColony::computeSolution()
{
  this->computeGComplete();
  this->computeInitialSolution();
  int C = this->bestCost;
  int a = this->bestSolution.size();
  this->initialPheromone = (double)this->n * (this->n - a) / C;
  printf("T0: %.6f\n", this->initialPheromone);
  uniform_int_distribution<int> randomVertex(0, this->n - 1);
  for (int j = 0; j < this->n; j++)
  {
    this->globalPheromone.push_back(this->initialPheromone);
  }
  while (this->improveAttempts < MAX_IMPROVE_ATTEMPTS)
  {
    for (Ant *k : this->ants)
    {
      this->resetAnt(k);
      this->antTransition(k, randomVertex(this->dre));
    }
    this->computeCycle();
  }
  return {this->bestSolution, this->bestCost};
}

void AntColony::computeInitialSolution()
{
  Graph g(this->n);
  for (int i = 0; i < this->n; i++)
    for (int j = 0; j < this->n; j++)
      g.updateEdge(i, j, this->GC.getWeight(i, j));
  for (int i = 0; i < this->n; i++)
  {
    int bestV;
    double maxRatio = 0;
    for (int j = 0; j < this->n; j++)
    {
      double ratio = g.getWeight(j, j) / this->G.getWeight(j, j);
      if (ratio > maxRatio)
      {
        bestV = j;
        maxRatio = ratio;
      }
    }
    if (maxRatio == 0)
      break;
    if (g.getWeight(bestV, bestV) > 0)
    {
      this->bestSolution.insert(bestV);
      this->bestCost += this->G.getWeight(bestV, bestV);
      for (int j = 0; j < this->n; j++)
      {
        if (bestV == j)
          continue;
        if (g.getWeight(bestV, j) == 1)
        {
          g.updateEdge(bestV, j, 0);
          g.updateEdge(j, bestV, 0);
          g.updateEdge(bestV, bestV, g.getWeight(bestV, bestV) - 1);
          g.updateEdge(j, j, g.getWeight(j, j) - 1);
        }
      }
    }
  }
}

void AntColony::computeCycle()
{
  bool cycleComplete = false;
  while (!cycleComplete)
  {
    cycleComplete = true;
    for (Ant *k : this->ants)
    {
      if (k->edgesMissing > 0)
      {
        cycleComplete = false;
        this->computeStep(k);
      }
    }
  }
  this->improveAttempts++;
  for (int j = 0; j < this->n; j++)
  {
    if (this->bestSolution.find(j) != this->bestSolution.end())
    {
      this->globalPheromone[j] = (1 - EVAPORATION_RATE) * this->globalPheromone[j] + (EVAPORATION_RATE * (1.0 / this->bestCost));
      // printf("%d %2.6f in cover\n", j, globalPheromone[j]);
    }
    else
    {
      this->globalPheromone[j] = (1 - EVAPORATION_RATE) * this->globalPheromone[j];
      // printf("%d %2.6f\n", j, globalPheromone[j]);
    }
  }
  // printf("--End of cycle--\n");
}

void AntColony::computeStep(Ant *k)
{
  double argMax = 0;
  int preferedVertex;
  double argSum = 0.0;
  for (int j = 0; j < this->n; j++)
  {
    double arg = this->globalPheromone[j] * (k->CV.getWeight(j, j) / this->G.getWeight(j, j)) * RELATIVE_SIGNIFICANCE * 10;
    if (arg > argMax)
    {
      argMax = arg;
      preferedVertex = j;
    }
    argSum += arg;
  }
  double q = this->urd(this->dre);
  if (q < THRESHOLD)
  {
    this->antTransition(k, preferedVertex);
  }
  else
  {
    vector<double> vertexProb(ceil(argSum));
    int i = 0;
    for (int j = 0; j < this->n; j++)
    {
      double vertexChances = this->globalPheromone[j] * (k->CV.getWeight(j, j) / this->G.getWeight(j, j)) * RELATIVE_SIGNIFICANCE * 10;
      for (; i < vertexChances; i++)
      {
        vertexProb[i] = j;
      }
    }
    uniform_int_distribution<double> dice(0, vertexProb.size() - 1);
    int result = vertexProb[dice(this->dre)];
    this->antTransition(k, result);
  }
}

void AntColony::antTransition(Ant *k, int j)
{
  this->updateConnectivityValue(k, j);
  k->actualVertex = j;
  // double prev = this->globalPheromone[j];
  this->globalPheromone[j] = (1 - PHI) * this->globalPheromone[j] + PHI * this->initialPheromone;
  // printf("Ant %d T%d=%2.6f => T%d=%2.6f\n", k->id, j, prev, j, this->globalPheromone[j]);
  if (k->edgesMissing == 0)
  {
    this->compareSolution(k->solution, k->solutionCost);
  }
}

void AntColony::updateConnectivityValue(Ant *k, int v)
{
  if (k->solution.find(v) == k->solution.end())
  {
    k->solution.insert(v);
    k->solutionCost += this->G.getWeight(v, v);
  }
  for (int j = 0; j < this->n; j++)
  {
    if (j == v)
      continue;
    if (k->CV.getWeight(v, j) == 1)
    {
      k->CV.updateEdge(v, j, 0);
      k->CV.updateEdge(j, v, 0);
      k->CV.updateEdge(v, v, k->CV.getWeight(v, v) - 1);
      k->CV.updateEdge(j, j, k->CV.getWeight(j, j) - 1);
      k->edgesMissing -= 1;
    }
  }
}

void AntColony::compareSolution(unordered_set<int> &solution, int solutionCost)
{
  if (solutionCost < this->bestCost)
  {
    this->bestSolution = solution;
    this->bestCost = solutionCost;
    this->improveAttempts = 0;
  }
}

void AntColony::resetAnt(Ant *k)
{
  for (int i = 0; i < this->n; i++)
    for (int j = 0; j < this->n; j++)
      k->CV.updateEdge(i, j, this->GC.getWeight(i, j));
  k->edgesMissing = this->m;
  k->solution.clear();
  k->solutionCost = 0;
}

void AntColony::computeGComplete()
{
  for (int i = 0; i < this->n; i++)
  {
    for (int j = i; j < this->n; j++)
    {
      if (i == j)
      {
        continue;
      }
      if (this->G.getWeight(i, j) == -1)
      {
        this->GC.updateEdge(i, j, 0);
      }
      else
      {
        this->GC.updateEdge(i, j, 1);
        this->GC.updateEdge(j, i, 1);
        this->GC.updateEdge(i, i, this->GC.getWeight(i, i) + 1);
        this->GC.updateEdge(j, j, this->GC.getWeight(j, j) + 1);
      }
    }
  }
}

void AntColony::setRandomEngine(int seed)
{
  default_random_engine dre(seed);
  this->dre = dre;
  uniform_real_distribution<double> urd(0.0, 1.0);
  this->urd = urd;
}

void AntColony::setWeight(int u, int w)
{
  this->G.updateEdge(u, u, w);
}

void AntColony::addEdge(int u, int v)
{
  this->G.updateEdge(u, v, 1);
  this->G.updateEdge(v, u, 1);
}