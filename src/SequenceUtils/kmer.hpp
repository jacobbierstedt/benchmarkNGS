#ifndef KMER_HPP
#define KMER_HPP

#include <string>
#include <bitset>

#define DEFAULT_KMASK 0x0F
#define DEFAULT_MIN_MAX 0x7FFFFFFFFFFFFFFF

class Minimizer {
public:
  uint64_t          mask;
  int               position;
  int               length;
  std::string       parentName;

  Minimizer ();
  Minimizer (const int length);
  virtual ~Minimizer (){};

  void getMinimizer(const std::string & kmer);
};


class Kmer {
public:
  uint64_t mask;
  int      position;
  int      length;

  Kmer ();
  Kmer (const int position, const int length);
  virtual ~Kmer () {};

  void getKmer(const std::string & seq);
};


#endif
