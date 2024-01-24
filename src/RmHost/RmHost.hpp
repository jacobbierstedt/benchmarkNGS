#ifndef RM_HOST_HPP
#define RM_HOST_HPP

#include <iostream>
#include <string>
#include "kmer.hpp"
#include "Reader.hpp"
#include "RmHostDb.hpp"


class RmHost {
public:
  int   kmerLength;
  int   minimizerLength;
  int   matchCutoff;
  float ratioCutoff;

  RmHostDb db;
  KmerizeSequence kmseq;

  RmHost () {};
  RmHost(
    const int kmerLength,
    const int minimizerLength,
    const int matchCutoff,
    const int ratioCutoff,
    const std::string dbFile
  );
  virtual ~RmHost () {};

  void queryRead(SeqRead & seq);
};

#endif
