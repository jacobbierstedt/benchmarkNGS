#ifndef SIMULATE_HPP
#define SIMULATE_HPP
#include "Reader.hpp"
#include <vector>
#include <random>
#include <time.h>


class Simulate {
public:
  int   length;
  float depth;
  int   terminalDepth;
  int   randomSeed;

  Simulate() {};
  Simulate(
    int length,
    float depth,
    int terminalDepth,
    int randomSeed
  );
  virtual ~Simulate () {};

  void simulate(SeqRead & ref, std::vector<SeqRead> & reads);
};


#endif
