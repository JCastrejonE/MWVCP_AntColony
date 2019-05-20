project(
  'MWVCP_AntColony',
  'cpp', default_options : ['cpp_std=c++11', 'buildtype=release'])

incdir = include_directories('include')

src = ['src/AntColony.cpp', 'src/Graph.cpp', 'src/main.cpp']

executable(
  'MWVCP_AntColony',
  src,
  include_directories : incdir)

#e = executable('prog', 'test/testprog.cpp')
#test('all', e)