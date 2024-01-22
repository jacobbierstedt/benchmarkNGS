#include <iostream>
#include <getopt.h>
#include <string>
#include <zlib.h>
#include "Reader.hpp"
#include "Simulate.hpp"


static float depth            = 0.0;
static int length             = 0;
static int terminalDepth      = 0;
static std::string inputFile  = "";
static std::string outputFile = "";
static int compressLevel      = 1;
static int randomSeed         = 0;


void usage(int xc) {
  fprintf(stderr, "Usage: simulate [options] -i <input_file> -o <output_file>\n\n");
  fprintf(stderr, "Required arguments:\n");
  fprintf(stderr, "-i str      input fastq/a (gzipped or not gzipped)\n");
  fprintf(stderr, "-o str      output gzipped fastq\n");
  fprintf(stderr, "-l int      read length (Min: 8)\n");
  fprintf(stderr, "-d float    average read depth to simulate (Min: 0)\n");
  fprintf(stderr, "\nOptions:\n");
  fprintf(stderr, "-z int      zlib compression level for output fastq (Default: 1; Min: 0; Max: 9)\n");
  fprintf(stderr, "-e int      depth of reads at ends of sequence\n");
  fprintf(stderr, "-r int      random seed (Default: random)\n");
  exit(xc);
}

void parse_command_line(int argc, char ** argv) {
  int c;
  while ((c = getopt(argc, argv, "hd:l:e:r:i:o:")) != -1) {
    switch(c) {
      case 'h': usage(0);                              break;
      case 'd': depth         = atof(optarg);          break;
      case 'l': length        = atoi(optarg);          break;
      case 'e': terminalDepth = atoi(optarg);          break;
      case 'z': compressLevel = atoi(optarg);          break;
      case 'r': randomSeed    = atoi(optarg);          break;
      case 'i': inputFile     = std::string(optarg);   break;
      case 'o': outputFile    = std::string(optarg);   break;
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
  if (depth <= 0) {
    std::cerr << "Option -d must be greater than 0.0" << std::endl; exit(1);
  }
  if (length <= 8) {
    std::cerr << "Option -l must be greater than 8" << std::endl; exit(1);
  }
}


void simulate_from_references() {
  Simulate sim   = Simulate(length, depth, terminalDepth, randomSeed);
  Reader refs    = Reader(inputFile);
  gzFile output  = gzopen(outputFile.c_str(), "w");
  gzsetparams(output, compressLevel, Z_DEFAULT_STRATEGY);

  SeqRead ref;
  std::vector<SeqRead> simulatedReads;
  while (refs.iterSequences(ref) != -1) {
    sim.simulate(ref, simulatedReads);
  }

  for (auto read : simulatedReads) {
    int len = 0;
    std::string fastq = read.toFastq(len);
    gzwrite(output, fastq.c_str(), len);
  }
  gzclose(output);
}


int main(int argc, char ** argv) {
  parse_command_line(argc, argv);
  simulate_from_references();
  return 0;
}
