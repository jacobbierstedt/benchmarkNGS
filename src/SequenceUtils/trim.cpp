#include <iostream>
#include "trim.hpp"


Trimmer::Trimmer(int readLength) :
  trim3(0),
  trim5(0),
  minLen(0),
  minProp(0.0),
  discardByLength(false)
{
  this->readLength = readLength;
}

Trimmer::Trimmer(int readLength, int trim3, int trim5) :
  minLen(0),
  minProp(0.0),
  discardByLength(false)
{
  this->readLength = readLength;
  this->trim3      = trim3;
  this->trim5      = trim5;
}

Trimmer::Trimmer(
    int readLength,
    int trim3,
    int trim5,
    int minLen,
    float minProp,
    bool discardByLength
  )
{
  this->readLength      = readLength;
  this->trim3           = trim3;
  this->trim5           = trim5;
  this->minLen          = minLen;
  this->minProp         = minProp;
  this->discardByLength = discardByLength;
}

void Trimmer::trimReadToLength(SeqRead & read) {
  read.seq  = read.seq.substr(0,  readLength);
  read.qual = read.qual.substr(0, readLength);
}

void Trimmer::hard3Trim(SeqRead & read) {
  int len   = read.seq.length() - trim3;
  read.seq  = read.seq.substr(0,  len);
  read.qual = read.qual.substr(0, len);
}

void Trimmer::hard5Trim(SeqRead & read) {
  read.seq  = read.seq.substr(trim5,  read.seq.length());
  read.qual = read.qual.substr(trim5, read.qual.length());
}

void Trimmer::trimRead(SeqRead & read) {
  int originalLength = read.seq.length();
  if (trim5 > 0 && read.seq.length() > trim5) {
    int   tlen = read.seq.length() - trim5;
    float prop = (float)tlen / (float)originalLength;
    if (tlen > minLen &&  prop > minProp) {
      hard5Trim(read);
    }
  }
  if (trim3 > 0 && read.seq.length() > trim3) {
    int   tlen = read.seq.length() - trim3;
    float prop = (float)tlen / (float)originalLength;
    if (tlen > minLen &&  prop > minProp) {
      hard3Trim(read);
    }
  }
  if (readLength > 0 && read.seq.length() > readLength) {
    trimReadToLength(read);
  }
  if (discardByLength) {
    float prop = (float)read.seq.length() / (float)originalLength;
    if (read.seq.length() < minLen || prop < minProp ) {
      read.keep = false;
    }
  }
}
