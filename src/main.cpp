#include <iostream>
#include <chrono>
#include <fstream>
#include <unordered_set>
#include "jarro2783/cxxopts.hpp"
#include "AntColony.hpp"

using namespace std;

int main(int argc, char **argv)
{
  auto gstart = chrono::steady_clock::now();
  int seed = 0;
  int n, m, u, v, w;
  istream *is = &cin;
  ifstream inFile;
  FILE *plotFile;
  bool plot = false;

  cxxopts::Options options(argv[0], "MWVCP solution using Ant Colony heuristic.");
  try
  {
    options
      .add_options()
      ("h, help", "Show help")
      ("f, file", "Input file to read instance (ignore to read from stdin)", cxxopts::value<string>(), "input_file")
      ("s, seed", "Seed to use. Default is 0", cxxopts::value<int>(), "N")
      ("g, graph", "Create gv file of the solution. If dot is installed you can create svg using: $ dot in.gv -Tsvg > out.svg", cxxopts::value<string>(), "output_file");

    auto result = options.parse(argc, argv);
    if (result.count("h"))
    {
      cout << options.help() << endl;
      return 0;
    }
    if (result.count("f"))
    {
      inFile.open(result["f"].as<string>());
      is = &inFile;
    }
    if (result.count("s"))
    {
      seed = result["s"].as<int>();
    }
    if (result.count("g"))
    {
      plotFile = fopen(result["g"].as<string>().c_str(), "w");

      // INITIALIZE GRAPH FILE
      fprintf(plotFile, "graph G {\n");
      plot = true;
    }

  } catch (const cxxopts::OptionException& e)
  {
    cout << "Error parsing options: " << e.what() << endl;
    return 1;
  }

  *is >> n >> m;
  AntColony AC(n, m);
  for(int i = 0; i < n; i++) {
    *is >> w;
    AC.setWeight(i, w);
  }
  for(; m > 0; m--) {
    *is >> u >> v;
    AC.addEdge(u, v);
    if(plot)
      fprintf(plotFile, "\tv%d -- v%d\n", u, v);
  }
  AC.setRandomEngine(seed);
  printf("\nSeed: %d\n", seed);
  pair<unordered_set<int>, int> result = AC.computeSolution();
  for (int x : result.first)
  {
    printf("%d, ", x);
    if(plot)
      fprintf(plotFile, "v%d [fontcolor=white, style=filled, fillcolor=\"#705f84\"];\n", x);
  }
  printf("\nEvaluation: %d\n", result.second);

  if(plot) {
    fprintf(plotFile, "}");
    fclose(plotFile);
  }


  auto gend = chrono::steady_clock::now();
  printf("Total elapsed time: %lld\n", chrono::duration_cast<chrono::seconds>(gend - gstart).count());
}