#include <atomic>

#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/delayed.h>

// **************************************************************
// Parallel Breadth First Search
// The graph is a sequence of sequences of vertex ids, representing
// the outedges for each vertex.
// Returns a sequence of sequences, with the ith element corresponding to
// **************************************************************

struct Node{
  int vertexId;
  int weight;
  parlay::sequence<int> predecessors;
};

int minSum = INT_MAX;

#include<iostream>
#include<stdlib.h>

template <typename vertex, typename weighted_graph>
auto BFSleast(vertex start, vertex destination, const weighted_graph& edges) {
  
  parlay::sequence<int> path;

  parlay::sequence<Node> frontier(1, {start, 0, {}});

  while (frontier.size() > 0) {

    // get out edges of the frontier and flatten

    frontier  = parlay::flatten(parlay::map(frontier, [&] (auto v) {
      return parlay::map(edges[v.vertexId], [&] (auto e) {
        e.predecessors = v.predecessors;
        e.predecessors.push_back(v.vertexId);
        return Node{e.vertexId, e.weight=v.weight+e.weight, e.predecessors};
      });
    }));

    // check for every vertex with parallel for each

    parlay::for_each(frontier, [&] (auto v) {
      if(v.vertexId == destination && v.weight < minSum){
        minSum = v.weight;
        path = v.predecessors;
        path.push_back(destination);
      }
    });

  }

  return std::make_pair(path, minSum);

}