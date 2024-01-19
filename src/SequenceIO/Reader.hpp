#ifndef READER_H
#define READER_H

#include "kseq.h"
#include <zlib.h>
#include <string>
#include <vector>

KSEQ_INIT(gzFile, gzread);

class SeqRead {
public:
  std::string seq;
  std::string name;
  std::string qual;
  SeqRead (){};
  virtual ~SeqRead(){};
};


class Reader{
private:
  kseq_t * seq;
  gzFile fp;
  gzFile getfp(std::string);

public:
  std::vector<std::string> seqNames;
  std::vector<std::string> reads;
  std::vector<std::string> quals;
  std::vector<int>         seqls;
  int nReads = 0;

  Reader() {};
  Reader(std::string inputFile);
  virtual ~Reader();

  void readSequences();
  int iterSequences(SeqRead & read);
  std::string toFastq();
  std::string toFasta();
};


#endif // Reader.hpp
