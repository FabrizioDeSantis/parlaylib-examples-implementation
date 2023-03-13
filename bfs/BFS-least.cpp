#include <iostream>
#include <string>
#include <chrono>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/internal/get_time.h>

#include "BFS-least.h"
#include "helper/graph_utils.h"

using nested_seq = parlay::sequence<parlay::sequence<int>>;
//using seq = parlay::sequence<int>;
using graph = nested_seq;
//using wgraph = parlay::sequence<parlay::sequence<vertex, int>>;
using utils = graph_utils<int>;

int main(int argc, char* argv[]) {
  //if (argc != 2) std::cout << usage << std::endl;
  //else {
    // long n = 0;
    // graph G;
    // try { n = std::stol(argv[1]); }
    // catch (...) {}
    // if (n == 0) {
    //   G = utils::read_symmetric_graph_from_file(argv[1]);
    //   n = G.size();
    // } else {
    //   G = utils::rmat_symmetric_graph(n, 20*n);
    //   utils::write_graph_to_file(G, "graph.txt");
    // }
    // //utils::print_graph_stats(G);

    // auto GW = utils::add_weights<int>(G,1,15); //grafo con archi pesati
    parlay::sequence<parlay::sequence<Node>> GW = {{{6,-1},{1,5}},{{6,3},{5,5},{2,7}},{{3,8}},{{4,10}},{},{{2,-1},{3,9},{4,1}},{{5,2}},{{6,9},{1,6}}};
    int startV, destinationV;
    std::cout << "Nodo di partenza: ";
    std::cin >> startV;
    std::cout << "Nodo di arrivo: ";
    std::cin >> destinationV;
    auto start = std::chrono::steady_clock::now();
    auto result = BFSleast(startV,destinationV,GW);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms\n";
    if(result.first.size()>0){
      for(int i=0; i<result.first.size(); i++){
        if(i==result.first.size()-1){
          std::cout << result.first[i] << std::endl;
        }
        else{
          std::cout << result.first[i] << " -> ";
        }
      }
      std::cout << "Costo minore " << result.second << std::endl;
    }
    else{
      std::cout << "Nessun percorso trovato" << std::endl;
    }
  //}
}
