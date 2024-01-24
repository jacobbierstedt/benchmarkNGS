#include "RmHostDb.hpp"


RmHostDb::RmHostDb(const int kmerLength, const int minimizerLength) {
  this->kmerLength      = kmseq.kmerLength;
  this->minimizerLength = kmseq.minimizerLength;
  this->kmseq = KmerizeSequence(kmerLength, minimizerLength);
}

void RmHostDb::addDbSequence(SeqRead & seq) {
  // Get kmer map
  kmseq.getMinimizers(seq, mmap);
  std::cout << mmap.size() << std::endl;

}

void RmHostDb::writeDb(const std::string & outputFile) {
  gzFile output  = gzopen(outputFile.c_str(), "wb");
  gzsetparams(output, 1, Z_DEFAULT_STRATEGY);

  for (const auto & [mm, pos] : mmap) {
    std::string outstr = std::to_string(mm) + '\n';
    gzwrite(output, outstr.c_str(), outstr.length());
  }
  gzclose(output);
}

void RmHostDb::readDb(const std::string & inputFile) {
  gzFile input = gzopen(inputFile.c_str(), "rb");
  int i = 0;
  std::string buf;
  while (true) {
    char c = gzgetc(input);
    if (c == '\n') {
      uint64_t mini = std::stoull(buf.c_str());
      mmap.emplace(mini, 0);
      buf.clear();
    } else {
      buf += c;
    }
    if (c == -1) {
      break;
    }
    i++;
  }
}
