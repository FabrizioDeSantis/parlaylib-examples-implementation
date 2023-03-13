#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include <parlay/io.h>
#include <parlay/primitives.h>
#include <parlay/random.h>
#include <parlay/sequence.h>
#include <parlay/internal/get_time.h>

#include "quicksort.h"
#include <fstream>

// **************************************************************
// Metodi per quicksort sequenziale

// last element is taken as pivot
int partition(std::vector<long int> &v, int start, int end){
	
	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
		if(v[i]<v[pivot]){
			std::swap(v[i],v[j]);
			++j;
		}
	}
	std::swap(v[j],v[pivot]);
	return j;
	
}

void sequentialQuicksort(std::vector<long int> &v, int start, int end ){

	if(start<end){
		int p = partition(v,start,end);
		sequentialQuicksort(v,start,p-1);
		sequentialQuicksort(v,p+1,end);
	}
	
}

void printVector(std::vector<long int> v){
	for(int i=0;i<10;++i)
		std::cout<<v[i]<<" ";
	std::cout<<"\n\n";
}

// **************************************************************

auto read_file_parlay(const std::string& filename){
  auto str = parlay::chars_from_file(filename);
  auto tokens = parlay::tokens(str, [] (char c) {return c == '\n' || c == ' ';});
  long n = parlay::chars_to_long(tokens[0]);
  long nr = parlay::chars_to_long(tokens[1]);
  long ni = n*nr;
  auto entries = parlay::tabulate(ni, [&] (long i){
    return parlay::chars_to_long(tokens[i+2]);
  });
  return parlay::tabulate(nr, [&] (long i){
    return to_sequence(entries.cut(n*i, n + n*i));
  });
}

int main(int argc, char* argv[]) {
    auto usage = "Usage: quicksort n || quicksort filename";
    
    if (argc != 2) std::cout << usage << std::endl;
    else {
      long n = 0;
      parlay::sequence<parlay::sequence<long int>> v;
      parlay::sequence<long> result;
      try{n = std::stol(argv[1]);}
      catch(...){}
      if(n==0){
        std::vector<std::vector<long int>> v1;
        v = read_file_parlay(argv[1]);
        for(int i=0; i<v.size(); i++){
          std::vector<long int> temp;
          for(int j=0; j<v[i].size(); j++){
            temp.push_back(v[i][j]);
          }
          v1.push_back(temp);
        }
        
        for(int i=0; i<v.size(); i++){
          std::cout << "----------------" << std::endl;
          std::cout << "Parallel" << std::endl;
          auto start = std::chrono::steady_clock::now();
          result = quicksort(v[i]);
          auto end = std::chrono::steady_clock::now();
          std::chrono::duration<double> elapsed_seconds = end-start;
          std::cout << "elapsed time: " << elapsed_seconds.count()*1000 << " ms\n";
          auto first_ten = result.head(10);
          std::cout << "first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
          std::cout << "Sequential" << std::endl;
          auto start1 = std::chrono::steady_clock::now();
          sequentialQuicksort(v1[i], 0, v1[i].size()-1);
          auto end1 = std::chrono::steady_clock::now();
          std::chrono::duration<double> elapsed_seconds1 = end1-start1;
          std::cout << "elapsed time: " << elapsed_seconds1.count()*1000 << " ms\n";
          printVector(v1[i]);
          std::cout << "----------------" << std::endl;
        }
      }
      else{
        //Generazione di numeri casuali
        parlay::random_generator gen(565);
        std::uniform_int_distribution<long> dis(0, (n-1));
        auto data = parlay::tabulate(n, [&] (long i) {
          auto r = gen[i];
          return dis(r);
        });
        //Trasferimento numeri generati in un vector per il quicksort sequenziale
        std::vector<long int> temp;
        for(auto x : data){
          temp.push_back(x);
        }
        std::cout << "----------------" << std::endl;
        std::cout << "Parallel" << std::endl;
        auto start = std::chrono::steady_clock::now();
        result = quicksort(data);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time: " << elapsed_seconds.count()*1000 << " ms\n";
        auto first_ten = result.head(10);
        std::cout << "first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
        std::cout << "Sequential" << std::endl;
        auto start1 = std::chrono::steady_clock::now();
        sequentialQuicksort(temp, 0, temp.size()-1);
        auto end1 = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds1 = end1-start1;
        std::cout << "elapsed time: " << elapsed_seconds1.count()*1000 << " ms\n";
        printVector(temp);
        std::cout << "----------------" << std::endl;
      }
    }
}
