#include <iostream>
#include <string>
#include <chrono>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/internal/get_time.h>

#include "BFS.h"
#include "BFS-sequential.h"
#include "helper/graph_utils.h"

// **************************************************************
// Driver
// **************************************************************
using vertex = int;
using nested_seq = parlay::sequence<parlay::sequence<vertex>>;
using graph = nested_seq;
using utils = graph_utils<vertex>;

int main(int argc, char* argv[]) {
  auto usage = "Usage: BFS <n> || BFS <filename>";
  if (argc != 2) std::cout << usage << std::endl;
  else {
    long n = 0;
    graph G;
    try { n = std::stol(argv[1]); }
    catch (...) {}
    if (n == 0) {
      G = utils::read_symmetric_graph_from_file(argv[1]);
      n = G.size();
    } else {
      G = utils::rmat_symmetric_graph(n, 20*n);
    }
    utils::print_graph_stats(G);
    std::vector<EdgeSeq> edges;
    for(int i=0; i<G.size(); i++){
      for(int j=0; j<G[i].size(); j++){
        EdgeSeq tmp = {i, G[i][j]};
        edges.push_back(tmp);
      }
    }
    GraphSeq graphSeq(edges, edges.size());
    std::vector<bool> discovered(n, false);
    nested_seq result;
    //Esecuzione versione parallela di BFS
    std::cout << "ParlayLib" << std::endl;
    auto start = std::chrono::steady_clock::now();
    result = BFS(1, G);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count()*1000 << " ms\n";
    //Esecuzione versione sequenziale
    std::cout << "Sequenziale" << std::endl;
    auto start2 = std::chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
    {
      if (discovered[i] == false)
      {
        // avvio BFS dal vertice "i"
        BFSSeq(graphSeq, i, discovered);
      }
    }
    auto end2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds2 = end2-start2;
    std::cout << "elapsed time: " << elapsed_seconds2.count()*1000 << " ms\n";
  }
}
