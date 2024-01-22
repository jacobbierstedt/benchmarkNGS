#include <iostream>
#include "Reader.hpp"


SeqRead::SeqRead(kseq_t * ks) :
  keep(true)
{
  seq  = std::string(ks->seq.s);
  name = std::string(ks->name.s);
  std::string kqual;
  if (ks->qual.l != ks->seq.l) {
    kqual = std::string(ks->seq.l, DEFAULT_QUAL);
  } else {
    kqual = std::string(ks->qual.s);
  }
  qual = kqual;
}

SeqRead::SeqRead(SeqRead & ref, int index, int length, int id) :
  keep(true)
{
  if (index + length > ref.seq.length()) {
    keep = false;
  }
  name = ref.name + "." + std::to_string(index) + "." + std::to_string(id);
  seq  = ref.seq.substr(index, length);
  qual = ref.qual.substr(index, length);
}

std::string SeqRead::toFasta() {
  std::string o;
  o += '>' + name + '\n';
  o += seq + '\n';
  return o;
}

std::string SeqRead::toFastq(int & len) {
  std::string o;
  o += '@' + name + '\n';
  o += seq + '\n';
  o += "+\n";
  o += qual + '\n';
  len = o.length();
  return o;
}

std::string SeqRead::toFastq() {
  int x = 0;
  return toFastq(x);
}



Reader::Reader(const std::string inputFile) {
  nReads = 0;
  fp     = getfp(inputFile);
  seq    = kseq_init(fp);
}

Reader::~Reader() {
  kseq_destroy(seq);
  gzclose(fp);
}

gzFile Reader::getfp(const std::string input) {
  if( std::string("-").compare(input) == 0) {
    fp = gzdopen(STDIN_FILENO, "r");
  } else {
    fp = gzopen(input.c_str(), "r");
  }
  if (fp == NULL) {
    std::cerr << "FATAL: Unable to read input: " << input << "\n";
    exit(1);
  }
  return fp;
}

void Reader::readSequences() {
  while (kseq_read(seq) != -1) {
    SeqRead sr = SeqRead(seq);
    reads.push_back(sr);
    names.push_back(sr.name);
  }
  nReads++;
}
/* Return value:
   >=0  length of the sequence (normal)
   -1   end-of-file
   -2   truncated quality string
   -3   error reading stream
 */
int64_t Reader::iterSequences(SeqRead & read) {
  int64_t status = kseq_read(seq);
  if (status == -1) {
    return status;
  }
  SeqRead nread = SeqRead(seq);
  read = nread;
  return status;
}
