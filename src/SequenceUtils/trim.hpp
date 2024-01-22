#ifndef TRIM_HPP
#define TRIM_HPP

#include <string>
#include "Reader.hpp"




class Trimmer {
public:
  // Hard trimming
  size_t readLength;
  size_t trim3;
  size_t trim5;
  size_t minLen;
  float  minProp;
  bool   discardByLength;

  // Quality trimming
  int    minQEnd;
  size_t windowSize;
  int    minQWindow;
  int    minQMean;

  Trimmer() {};
  Trimmer(int readLength);
  Trimmer(int readLength, int trim3, int trim5);
  Trimmer(
    int   readLength,
    int   trim3,
    int   trim5,
    int   minLen,
    float minProp,
    bool  discardByLength,
    int   minQEnd,
    int   windowSize,
    int   minQWindow,
    int   minQMean
  );
  virtual ~Trimmer () {};

  void trimReadToLength(SeqRead & read);
  void hard3Trim(SeqRead & read);
  void hard5Trim(SeqRead & read);
  void trimRead(SeqRead & read);

  void qualityTrim(SeqRead & read);
  void trimQEnds(SeqRead & read);
  void trimQSlidingWindow(SeqRead & read);
  float getMeanQuality(std::string qual);
  int getTotalQuality(std::string qual);
};

#endif // trim.hpp
