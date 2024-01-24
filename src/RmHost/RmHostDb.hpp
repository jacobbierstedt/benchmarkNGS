#ifndef RM_HOST_DB_HPP
#define RM_HOST_DB_HPP

#include "kmer.hpp"
#include "Reader.hpp"


class RmHostDb {
private:
  /* data */

public:
  int kmerLength;
  int minimizerLength;
  KmerizeSequence kmseq;
  MMap mmap;

  RmHostDb () {};
  RmHostDb (const int kmerLength, const int minimizerLength);
  virtual ~RmHostDb () {};

  void addDbSequence(SeqRead & seq);
  void writeDb(const std::string & outputFile);
  void readDb(const std::string & inputFile);
};



#endif
