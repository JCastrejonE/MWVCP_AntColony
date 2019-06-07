## About
This project forms part of Heuristic combinatorial optimization course that forms part of Computer Science degree at **Facultad de Ciencias, Universidad Nacional Autónoma de México**. It tries to find a solution to Minimum Weight Vertex Coverage Problem using Ant Colony Optimization algorithm with some few variants.

For further understanding you can refer to the [PDF document in spanish](tex/Colonia_de_Hormigas.pdf).

## Building and running

[Meson build system](https://mesonbuild.com/index.html) is used to compile this project. Running:
```
$ meson build && cd build
$ ninja
```
in the root directory should setup *build* directory with the executable inside.

Use `$ build/MWVCP_AntColony  -h` to know more about the available options.
```
$ ./MWVCP_AntColony -h
MWVCP solution using Ant Colony heuristic.
Usage:
  ./MWVCP_AntColony [OPTION...]

  -h, --help               Show help
  -f, --file input_file    Input file to read instance (ignore to read from
                           stdin)
  -s, --seed N             Seed to use. Default is 0
  -g, --graph output_file  Create gv file of the solution.
                           If dot is installed you can create svg using:
                           $ dot in.gv -Tsvg > out.svg
```

## Graph option
This option generates a [dot](https://www.graphviz.org) file that represents the original graph with the vertex in best solution found coloured.

## Available tune options
The ant colony optimization heuristic can be tuned to experiment different results.
The header file [include/AntColony.hpp](include/AntColony.hpp) contains the constraints that can be modified.

Credits to [jarro2783](https://github.com/jarro2783/) for his [cxxopts](https://github.com/jarro2783/cxxopts) library to parse command line options.