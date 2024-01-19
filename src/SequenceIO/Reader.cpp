#include <iostream>
#include "Reader.hpp"


Reader::Reader(std::string inputFile) {
  fp = getfp(inputFile);
  seq = kseq_init(fp);
}


Reader::~Reader() {
  kseq_destroy(seq);
  gzclose(fp);
}


gzFile Reader::getfp(std::string input) {
  if( std::string("-").compare(input) == 0) {
    fp = gzdopen(STDIN_FILENO, "r");
  } else {
    fp = gzopen(input.c_str(), "r");
  }
  if (fp == NULL) {
    std::cerr << "FATAL: Input " << input << " cannot be read\n";
    exit(1);
  }
  return fp;
}


void Reader::readSequences() {
  while (kseq_read(seq) != -1) {
    std::string qual;
    if (seq->qual.l != seq->seq.l) {
      qual = std::string(seq->seq.l, 73);
    } else {
      qual = std::string(seq->qual.s);
    }
    seqNames.push_back(std::string(seq->name.s));
    reads.push_back(std::string(seq->seq.s));
    quals.push_back(qual);
    seqls.push_back(seq->seq.l);
  }
  nReads++;
}

int Reader::iterSequences(SeqRead & read) {
  int status = kseq_read(seq);
  std::string qual;
  if (seq->qual.l != seq->seq.l) {
    qual = std::string(seq->seq.l, 73);
  } else {
    qual = std::string(seq->qual.s);
  }
  read.seq  = std::string(seq->seq.s);
  read.name = std::string(seq->name.s);
  read.qual = qual;
  return status;
}


std::string Reader::toFastq() {
  std::string fq;
  for (int i = 0; i < reads.size(); i++) {
    fq += '@' + seqNames[i] + '\n';
    fq += reads[i] + '\n';
    fq += "+\n";
    fq += quals[i] + '\n';
  }
  return fq;
}


std::string Reader::toFasta() {
  std::string fa;
  for (int i = 0; i < reads.size(); i++) {
    fa += '>' + seqNames[i] + '\n';
    fa += reads[i] + '\n';
  }
  return fa;
}
