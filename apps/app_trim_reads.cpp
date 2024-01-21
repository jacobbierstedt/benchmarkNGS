#include <iostream>
#include <getopt.h>
#include <string>
#include <zlib.h>
#include "Reader.hpp"
#include "trim.hpp"




static int readLength         = 0;
static int trim3              = 0;
static int trim5              = 0;
static int minLen             = 0;
static float minProp          = 0.0;
static bool discardShorties   = false;
static int compressLevel      = 1;
static std::string inputFile  = "";
static std::string outputFile = "";

void usage(int xc) {
  fprintf(stderr, "Usage: trim_reads [options] -i <input_file> -o <output_file>\n\n");
  fprintf(stderr, "Required arguments:\n");
  fprintf(stderr, "-i str      input fastq/a (gzipped or not gzipped)\n");
  fprintf(stderr, "-o str      output gzipped fastq\n\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "-h          Display usage message\n");
  fprintf(stderr, "-l int      trim reads to overall length\n");
  fprintf(stderr, "-t int      bp to trim from right\n");
  fprintf(stderr, "-f int      bp to trim from left\n");
  fprintf(stderr, "-m int      minimum length read length\n");
  fprintf(stderr, "-p float    minimum proprotion of read to retain (Min: 0.0; Max: 1.0)\n");
  fprintf(stderr, "-d          discard reads that fail to meet length minimums\n");
  fprintf(stderr, "-z int      zlib compression level for output fastq (Default: 1; Min: 0; Max: 9)\n");
  exit(xc);
}

void parse_command_line(int argc, char ** argv) {
  int c;
  while ((c = getopt(argc, argv, "hl:t:f:m:p:dz:i:o:")) != -1) {
    switch(c) {
      case 'h': usage(0);                              break;
      case 'l': readLength      = atoi(optarg);        break;
      case 't': trim3           = atoi(optarg);        break;
      case 'f': trim5           = atoi(optarg);        break;
      case 'm': minLen          = atoi(optarg);        break;
      case 'p': minProp         = atof(optarg);        break;
      case 'd': discardShorties = true;                break;
      case 'z': compressLevel   = atoi(optarg);        break;
      case 'i': inputFile       = std::string(optarg); break;
      case 'o': outputFile      = std::string(optarg); break;
      default: usage(1);                               break;
    }
  }
  if (inputFile.length() <= 2) {
    std::cerr << "Option -i requires a valid file path: " << inputFile << std::endl; exit(1);
  }
  if (outputFile.length() <= 2) {
    std::cerr << "Option -o requires a valid file path: " << outputFile << std::endl; exit(1);
  }
  if (compressLevel > 9 || compressLevel < 0) {
    std::cerr << "Option -z compression level must be between 0 and 9" << std::endl; exit(1);
  }
}

void trimReads() {
  Trimmer trimmer  = Trimmer(readLength, trim3, trim5, minLen, minProp, discardShorties);
  Reader  seqs     = Reader(inputFile);
  gzFile  output   = gzopen(outputFile.c_str(), "w");

  gzsetparams(output, compressLevel, Z_DEFAULT_STRATEGY);

  SeqRead read;
  while (seqs.iterSequences(read) != -1) {
    trimmer.trimRead(read);
    if (read.keep) {
      int len = 0;
      std::string outread = read.toFastq(len);
      gzwrite(output, outread.c_str(), len);
    }
  }
  gzclose(output);
}

int main(int argc, char ** argv) {
  parse_command_line(argc, argv);
  trimReads();
  return 0;
}
