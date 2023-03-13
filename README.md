# ParlayLib Examples
This repository contains the C++ code that shows a possible use of the ParlayLib library.

## Brief intro
ParlayLib is a C++ library for developing efficient parallel algorithms and software on multicore shared-memory machines. This library provides tools and primitives that extend those already available in the C++ standard library and allows to simplify the programming of efficient and scalable parallel algorithms.

## quicksort

This folder contains the implementation of the quicksort algorithm with ParlayLib.
For this example, the algorithm was mofiefied to read randomly generated sequences of numbers from files, so that at each execution the quicksort algorithm is executed for different sequences of numbers. Also, the performance of the ParlayLib version was compared to a sequential version of quicksort, run on the same number sequences. For larger sequences of numbers, the ParlayLib random number generator has been used. This random number generator produces a sequence of values which is stored in the data structure parlay::sequence (the equivalent of std::vector), while for the sequential version the values have been placed inside a std::vector.

## bfs

This folder contains the implementation of BFS algorithm with ParlayLib
First a performance study was done comparing it to a sequential version. Second, the ParlayLib example was modified to solve two problems of practical interest, such as finding the least-cost path from source to destination in a weighted directed graph and the Flood Fill Algorithm.
