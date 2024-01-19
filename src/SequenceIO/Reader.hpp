#ifndef READER_H
#define READER_H

#include "kseq.h"
#include <zlib.h>
#include <string>
#include <vector>

#define DEFAULT_QUAL 73
KSEQ_INIT(gzFile, gzread);

class SeqRead {
public:
  std::string seq;
  std::string name;
  std::string qual;
  std::string comment;

  SeqRead () {};
  SeqRead (kseq_t * ks);
  virtual ~SeqRead(){};

  std::string toFastq();
  std::string toFasta();
};


class Reader{
private:
  kseq_t * seq;
  gzFile   fp;
  gzFile getfp(const std::string);

public:
  std::vector<SeqRead>     reads;
  std::vector<std::string> names;
  int nReads;

  Reader() {};
  Reader(const std::string inputFile);
  virtual ~Reader();

  void readSequences();
  int iterSequences(SeqRead & read);
};


#endif // Reader.hpp
