#ifndef TRIM_HPP
#define TRIM_HPP

#include <string>
#include "Reader.hpp"




class Trimmer {
public:
  int   readLength;
  int   trim3;
  int   trim5;
  int   minLen;
  float minProp;
  bool  discardByLength;

  Trimmer() {};
  Trimmer(int readLength);
  Trimmer(int readLength, int trim3, int trim5);
  Trimmer(
    int   readLength,
    int   trim3,
    int   trim5,
    int   minLen,
    float minProp,
    bool  discardByLength
  );
  virtual ~Trimmer () {};

  void trimReadToLength(SeqRead & read);
  void hard3Trim(SeqRead & read);
  void hard5Trim(SeqRead & read);
  void trimRead(SeqRead & read);
};

#endif // trim.hpp
