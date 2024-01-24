#ifndef KMER_HPP
#define KMER_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Reader.hpp"

#define DEFAULT_KMASK 0x0F
#define DEFAULT_MIN_MAX 0x7FFFFFFFFFFFFFFF

typedef std::unordered_set<uint64_t> MSet;
typedef std::unordered_map<uint64_t, int> MMap;

class Minimizer {
public:
  uint64_t          mask;
  int               position;
  int               length;
  std::string       parentName;

  Minimizer ();
  Minimizer (const int length);
  virtual ~Minimizer (){};

  void getMinimizer(const std::string & kmer);
};


class Kmer {
public:
  uint64_t mask;
  int      position;
  int      length;

  Kmer ();
  Kmer (const int position, const int length);
  virtual ~Kmer () {};

  void getKmer(const std::string & seq);
};

class KmerizeSequence {
private:
  /* data */

public:
  int kmerLength;
  int minimizerLength;
  // std::unordered_map<uint64_t, int> mmap;

  KmerizeSequence() {};
  KmerizeSequence(int kmerLength, int minimizerLength);
  virtual ~KmerizeSequence () {};

  void getMinimizers(SeqRead & seq, MMap & mmap);
  void getMinimizers(SeqRead & seq, MSet & mset);
};


#endif
