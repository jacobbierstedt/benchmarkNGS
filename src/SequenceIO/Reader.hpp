#ifndef READER_H
#define READER_H

#include "kseq.h"
#include <zlib.h>
#include <string>
#include <vector>

KSEQ_INIT(gzFile, gzread);


class Reader{
private:
  gzFile getfp(std::string);

public:
  std::vector<std::string> seqNames;
  std::vector<std::string> reads;
  std::vector<std::string> quals;
  std::vector<int>         seqls;
  int nReads = 0;

  Reader() {};
  Reader(std::string inputFile);
  virtual ~Reader(){};

  void readSequences(gzFile fp);
  std::string toFastq();
  std::string toFasta();
};


#endif // Reader.hpp
