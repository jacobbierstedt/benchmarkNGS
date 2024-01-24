#include "RmHost.hpp"


RmHost::RmHost(
  const int kmerLength,
  const int minimizerLength,
  const int matchCutoff,
  const int ratioCutoff,
  const std::string dbFile)
{
  this->kmerLength      = kmerLength;
  this->minimizerLength = minimizerLength;
  this->matchCutoff     = matchCutoff;
  this->ratioCutoff     = ratioCutoff;
  this->db = RmHostDb(kmerLength, minimizerLength);
  this->kmseq = KmerizeSequence(kmerLength, minimizerLength);
  db.readDb(dbFile);
}


void RmHost::queryRead(SeqRead & seq) {
  MSet minimizers;
  kmseq.getMinimizers(seq, minimizers);
  // Discard any reads that are shorter than k
  if (minimizers.size() == 0) {
    seq.keep = false;
    return;
  }
  int hits = 0;
  for (auto q : minimizers) {
    if (auto hit = db.mmap.find(q); hit != db.mmap.end()) {
      hits += 1;
    }
  }
  float hostRatio = (float) hits / (float) minimizers.size();
  if (hostRatio > ratioCutoff || hits > matchCutoff) {
    seq.keep = false;
  }
}
