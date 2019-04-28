#include <iostream>
#include <chrono>
#include <unordered_set>
#include "AntColony.hpp"

using namespace std;

int main(int argc, char **argv)
{
  auto gstart = chrono::steady_clock::now();
  int seed = 100;
  int n, m, u, v, w;
  cin >> n >> m;
  AntColony AC(n, m);
  for(int i = 0; i < n; i++) {
    cin >> w;
    AC.setWeight(i, w);
  }
  for(; m > 0; m--) {
    cin >> u >> v;
    AC.addEdge(u, v);
  }
  AC.setRandomEngine(seed);
  printf("\nSeed: %d\n", seed);
  pair<unordered_set<int>, int> result = AC.computeSolution();
  for (int x : result.first)
  {
    printf("%d, ", x);
  }
  printf("\nEvaluation: %d\n", result.second);

  auto gend = chrono::steady_clock::now();
  printf("Total elapsed time: %lld\n", chrono::duration_cast<chrono::seconds>(gend - gstart).count());
}