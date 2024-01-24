#include "kmer.hpp"

static const uint8_t dna[] = {
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

Minimizer::Minimizer() :
  mask(DEFAULT_MIN_MAX)
{
}
Minimizer::Minimizer(const int length) :
  mask(DEFAULT_MIN_MAX)
{
  this->length = length;
}

void Minimizer::getMinimizer(const std::string & kmer) {
  for (int i=0; i< kmer.length()- length + 1; i++) {
    std::string subkmer = kmer.substr(i, length);
    Kmer km(i, length);
    km.getKmer(subkmer);
    if (km.mask < mask) {
      mask = km.mask;
    }
  }
}

Kmer::Kmer() :
  mask(DEFAULT_KMASK)
{
}

Kmer::Kmer(const int position, const int length) :
  mask(DEFAULT_KMASK)
{
  this->position = position;
  this->length = length;
}

void Kmer::getKmer(const std::string & seq) {
  for(int i = 0; i< seq.length(); i++) {
    mask <<= 2;
    mask |= dna[seq[i]];
  }
}

KmerizeSequence::KmerizeSequence(const int kmerLength, const int minimizerLength)
{
  this->kmerLength      = kmerLength;
  this->minimizerLength = minimizerLength;
}

void KmerizeSequence::getMinimizers(SeqRead & seq, MMap & mmap) {
  if (seq.seq.length() <= kmerLength) {
    return;
  }
  int i = 0;
  while (i < seq.seq.length() - kmerLength + 1) {
    std::string kstr = seq.seq.substr(i, kmerLength);
    Minimizer mini(minimizerLength);
    mini.getMinimizer(kstr);
    mmap.emplace(mini.mask, i);
    i++;
  }
}

void KmerizeSequence::getMinimizers(SeqRead & seq, MSet & mset) {
  if (seq.seq.length() <= kmerLength) {
    return;
  }
  int i = 0;
  while (i < seq.seq.length() - kmerLength + 1) {
    std::string kstr = seq.seq.substr(i, kmerLength);
    Minimizer mini(minimizerLength);
    mini.getMinimizer(kstr);
    mset.emplace(mini.mask);
    i++;
  }
}
