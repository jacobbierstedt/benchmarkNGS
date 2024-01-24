#include <iostream>
#include <string>
#include <unordered_map>
#include "Reader.hpp"
#include "kmer.hpp"
#include "RmHostDb.hpp"
#include "RmHost.hpp"



static int kmerLength         = 31;
static int minimizerLength    = 19;
static std::string inputFile  = "";
static std::string outputFile = "";
static std::string inputDb    = "";
static int matchCutoff        = 2;
static float ratioCutoff      = 0.1;


void usage(int xc) {
  fprintf(stderr, "Usage: rmhost [options] -i <input_file> -o <output_file>\n\n");
  fprintf(stderr, "Required arguments:\n");
  fprintf(stderr, "-i str      input fastq/a containing sequences to de-host (gzip or not; use '-' for stdin)\n");
  fprintf(stderr, "-d str      input rmhost database index (See rmhost_build)\n");
  fprintf(stderr, "-o str      output fastq for dehosted reads (gzipped)\n");
  fprintf(stderr, "-k int      kmer length (Min: 20; Max: 40)\n");
  fprintf(stderr, "-m int      minimzer length (Min: 10; Max: 30)\n");
  fprintf(stderr, "\nOptions:\n");
  fprintf(stderr, "-x int      maximum matches per read to host db (Default: 2)\n");
  fprintf(stderr, "-r int      maximum host:non-host kmer ratio (Default: 0.1) \n");
  exit(xc);
}

void parse_command_line(int argc, char ** argv) {
  if (argc <= 2) {
    usage(1);
  }
  int c;
  while ((c = getopt(argc, argv, "hk:m:d:x:r:i:o:")) != -1) {
    switch(c) {
      case 'h': usage(0);                              break;
      case 'k': kmerLength      = atoi(optarg);        break;
      case 'm': minimizerLength = atoi(optarg);        break;
      case 'x': matchCutoff     = atoi(optarg);        break;
      case 'r': ratioCutoff     = atof(optarg);        break;
      case 'd': inputDb         = std::string(optarg); break;
      case 'i': inputFile       = std::string(optarg); break;
      case 'o': outputFile      = std::string(optarg); break;
      default: usage(1);                               break;
    }
  }
  if (inputFile.length() < 1) {
    std::cerr << "Option -i requires a valid file path: " << inputFile << std::endl; exit(1);
  }
  if (outputFile.length() <= 2) {
    std::cerr << "Option -o requires a valid file path: " << outputFile << std::endl; exit(1);
  }
  if (kmerLength > 40 || kmerLength < 20) {
    std::cerr << "Option -k kmer length must be between 20 and 40" << std::endl; exit(1);
  }
  if (minimizerLength > 30 || minimizerLength < 10) {
    std::cerr << "Option -k kmer length must be between 10 and 30" << std::endl; exit(1);
  }
  if (ratioCutoff < 0.0 || ratioCutoff > 1.0 ) {
    std::cerr << "Option -r ratio cutoff must be between 0 and 1" << std::endl; exit(1);
  }
}

void dehost_reads() {
  RmHost dehost = RmHost(
      kmerLength,
      minimizerLength,
      matchCutoff,
      ratioCutoff,
      inputDb);
  Reader reads   = Reader(inputFile);
  gzFile output  = gzopen(outputFile.c_str(), "w");
  gzsetparams(output, 1, Z_DEFAULT_STRATEGY);

  SeqRead read;
  while (reads.iterSequences(read) != -1) {
    dehost.queryRead(read);
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
  dehost_reads();
  return 0;
}
