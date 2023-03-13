#include <atomic>

#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/delayed.h>

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

  parlay::sequence<parlay::sequence<Node>> array;
  parlay::sequence<int> path;

  parlay::sequence<Node> frontier(1, {start, 0, {start}});
  
  parlay::sequence<parlay::sequence<Node>> frontiers;

  while (frontier.size() > 0) {

    frontiers.push_back(frontier);

    // get out edges of the frontier and flatten

    frontier  = parlay::flatten(parlay::map(frontier, [&] (auto v) {
      return parlay::map(edges[v.vertexId], [&] (auto e) {
        v.predecessors.push_back(e.vertexId);
        return Node{e.vertexId, e.weight=v.weight+e.weight, v.predecessors};
      });
    }));

    // check for every vertex with parallel for each

    parlay::for_each(frontier, [&] (auto v) {
      if(v.vertexId == destination && v.weight < minSum){
        minSum = v.weight;
        path = v.predecessors;
      }
    });

  }

  return std::make_pair(path, minSum);

}