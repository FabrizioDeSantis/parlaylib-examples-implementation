#include <atomic>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/delayed.h>
#include <iostream>
#include <stdlib.h>

struct Color{
  int b;
  int g;
  int r;
};

struct Pixel{
  int x;
  int y;
  Color color;
};

struct Movement{
  int x;
  int y;
};

//spostamenti nelle 8 direzioni possibili

parlay::sequence<Movement> mov = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

parlay::sequence<Pixel> neighbours(parlay::sequence<parlay::sequence<Pixel>> const &mat, Pixel pix, Color target){
  //recupero gli spostamenti permessi
  auto permitted = parlay::filter(mov, [&] (auto i) {
      return ((pix.x+i.x >= 0 && pix.x+i.x < mat.size()) && (pix.y+i.y >= 0 && pix.y+i.y < mat[0].size()) 
      && (mat[pix.x+i.x][pix.y+i.y].color.b==target.b 
      && mat[pix.x+i.x][pix.y+i.y].color.g==target.g && mat[pix.x+i.x][pix.y+i.y].color.r==target.r));
  });
  //recupero le coordinate dei pixel 
  auto out = parlay::map(permitted, [&] (auto i){
    return Pixel{pix.x+i.x, pix.y+i.y};
  });
  return out;
}

auto FloodFill(Pixel start, parlay::sequence<parlay::sequence<Pixel>> &mat, Color replacement) {

  auto visited = parlay::tabulate(mat.size(), [&] (long x){
    return parlay::tabulate<std::atomic<bool>>(mat[x].size(), [&] (long y){
      return (x==start.x && y==start.y) ? true : false;
    });
  });

  Color target = mat[start.x][start.y].color; //salvo il colore target da sostituire

  mat[start.x][start.y].color=replacement;

  //std::cout << "Target: " << target << std::endl;
  //std::cout << "Replacement: " << replacement << std::endl;

  if(target.b == replacement.b && target.g == replacement.g && target.r == replacement.r){
    return;
  }

  parlay::sequence<Pixel> frontier(1, {start.x, start.y});

  while (frontier.size() > 0) {

    auto out = parlay::flatten(parlay::map(frontier, [&] (auto p) {
      mat[p.x][p.y].color=replacement;
      return neighbours(mat, p, target);
    }));

    frontier = parlay::filter(out, [&] (auto&& p){
      bool expected=false;
      return (!visited[p.x][p.y]) && visited[p.x][p.y].compare_exchange_strong(expected, true); 
    });

  }

}