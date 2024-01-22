#include <iostream>
#include "trim.hpp"


Trimmer::Trimmer(int readLength) :
  trim3(0),
  trim5(0),
  minLen(0),
  minProp(0.0),
  discardByLength(false),
  minQEnd(0),
  windowSize(20),
  minQWindow(20),
  minQMean(20)
{
  this->readLength = readLength;
}

Trimmer::Trimmer(int readLength, int trim3, int trim5) :
  minLen(0),
  minProp(0.0),
  discardByLength(false),
  minQEnd(0),
  windowSize(20),
  minQWindow(20),
  minQMean(20)
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
    bool discardByLength,
    int minQEnd,
    int windowSize,
    int minQWindow,
    int minQMean
  )
{
  this->readLength      = readLength;
  this->trim3           = trim3;
  this->trim5           = trim5;
  this->minLen          = minLen;
  this->minProp         = minProp;
  this->discardByLength = discardByLength;
  this->minQEnd         = minQEnd;
  this->windowSize      = windowSize;
  this->minQWindow      = minQWindow;
  this->minQMean        = minQMean;
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

void Trimmer::qualityTrim(SeqRead & read) {
  int originalLength = read.seq.length();
  trimQEnds(read);
  if (windowSize < read.qual.length()) {
    trimQSlidingWindow(read);
  }

  // Check if final read passes length and Q thresholds
  if (discardByLength) {
    float prop = (float)read.seq.length() / (float)originalLength;
    if (read.seq.length() < minLen || prop < minProp ) {
      read.keep = false;
    }
  }
  float avgQ = getMeanQuality(read.qual);
  if(avgQ < minQMean) {
    read.keep = false;
  }
}

void Trimmer::trimQEnds(SeqRead & read) {
  int i = read.seq.length() - 1;
  char q = read.qual[i] - 33;
  int cut = 0;
  while (q < minQEnd && i > 0) {
    q = read.qual[i] - 33;
    if (q < minQEnd) {
      cut++;
    }
    i--;
  }
  read.seq  = read.seq.substr(0,  read.seq.length()  - cut);
  read.qual = read.qual.substr(0, read.qual.length() - cut);

  i = 0;
  q = read.qual[i] - 33;
  cut = 0;
  while (q < minQEnd && i < read.seq.length()) {
    q = read.qual[i] - 33;
    if (q < minQEnd) {
      cut++;
    }
    i++;
  }
  read.seq  = read.seq.substr(cut,  read.seq.length());
  read.qual = read.qual.substr(cut, read.qual.length());
}


void Trimmer::trimQSlidingWindow(SeqRead & read) {
  int i           = 0;
  int windowEnd   = 0;
  float totalReqQ = minQWindow * windowSize;
  // Trim from the left
  while (i < read.qual.length() - windowSize + 1) {
    std::string window = read.qual.substr(i, windowSize);
    int wTotal = getTotalQuality(window);
    if (wTotal > totalReqQ) {
      break;
    }
    i++;
  }
  read.seq  = read.seq.substr(i,  read.seq.length()  - i);
  read.qual = read.qual.substr(i, read.qual.length() - i);

  // Trim from the right
  i = read.qual.length() - windowSize + 1;
  while (i > 0) {
    std::string window = read.qual.substr(i, windowSize);
    int wTotal = getTotalQuality(window);
    if (wTotal > totalReqQ) {
      break;
    }
    i--;
  }
  read.seq  = read.seq.substr(0,  i + windowSize);
  read.qual = read.qual.substr(0, i + windowSize);
}

float Trimmer::getMeanQuality(std::string qual) {
  int qTotal = getTotalQuality(qual);
  return (float) qTotal / (float) qual.length();
}

int Trimmer::getTotalQuality(std::string qual) {
  int qTotal = 0;
  for (int j : qual) {
    qTotal += (j - 33);
  }
  return qTotal;
}
