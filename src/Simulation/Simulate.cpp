#include "Simulate.hpp"
#include <iostream>


Simulate::Simulate(int length, float depth, int terminalDepth, int randomSeed) {
  this->length        = length;
  this->depth         = depth;
  this->terminalDepth = terminalDepth;
  this->randomSeed    = randomSeed;
}

void Simulate::simulate(SeqRead & ref, std::vector<SeqRead> & reads) {

  int refLength = ref.seq.length();
  int nTotalReads = ((refLength * depth) / length) + (terminalDepth * 2);
  nTotalReads = nTotalReads >= (int)depth ? nTotalReads : (int)depth;
  int maxStart = (refLength - length) >= 0 ? (refLength - length) : 0;

  int nsims = 0;
  for (int i=0; i < terminalDepth; i++) {
    SeqRead front(ref, 0, length, nsims);
    nsims++;
    SeqRead back(ref, ref.seq.length() - length, length, nsims);
    nsims++;
    reads.push_back(front);
    reads.push_back(back);
  }

  std::mt19937 r = randomSeed == 0 ? std::mt19937(time(NULL)) : std::mt19937(randomSeed);
  std::uniform_int_distribution<int> indRange(0, maxStart);
  while (nsims < nTotalReads) {
    int simStart = indRange(r);
    SeqRead sim(ref, simStart, length, nsims);
    reads.push_back(sim);
    nsims++;
  }
}
