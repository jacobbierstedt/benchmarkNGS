#include <iostream>
#include "trim.hpp"


Trimmer::Trimmer(int readLength) :
  trim3(0),
  trim5(0)
{
  this->readLength = readLength;
}

Trimmer::Trimmer(int readLength, int trim3, int trim5) {
  this->readLength = readLength;
  this->trim3      = trim3;
  this->trim5      = trim5;
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
