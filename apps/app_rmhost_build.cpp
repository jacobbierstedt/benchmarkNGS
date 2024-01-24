#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <bitset>

#include "Reader.hpp"
#include "kmer.hpp"
#include "RmHostDb.hpp"



static int kmerLength         = 31;
static int minimizerLength    = 19;
static std::string inputFile  = "";
static std::string outputFile = "database.rmhost.rmhi";


void usage(int xc) {
  fprintf(stderr, "Usage: rmhost_build [options] -i <input_file> -o <output_file>\n\n");
  fprintf(stderr, "Required arguments:\n");
  fprintf(stderr, "-i str      input fasta containing host sequences (gzip or not; use '-' for stdin)\n");
  fprintf(stderr, "-o str      output database file (Default: database.rmhost.dbi)\n");
  fprintf(stderr, "-k int      kmer length (Min: 20; Max: 40)\n");
  fprintf(stderr, "-m int      minimzer length (Min: 10; Max: 30)\n");
  exit(xc);
}

void parse_command_line(int argc, char ** argv) {
  if (argc <= 2) {
    usage(1);
  }
  int c;
  while ((c = getopt(argc, argv, "hk:m:i:o:")) != -1) {
    switch(c) {
      case 'h': usage(0);                              break;
      case 'k': kmerLength      = atoi(optarg);        break;
      case 'm': minimizerLength = atoi(optarg);        break;
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
}

void build_rmhost_db( ) {
  Reader refs(inputFile);
  RmHostDb db(kmerLength, minimizerLength);
  SeqRead ref;
  while (refs.iterSequences(ref) != -1) {
    db.addDbSequence(ref);
  }
  db.writeDb(outputFile);
  // db.readDb(outputFile);
}



int main(int argc, char ** argv) {
  parse_command_line(argc, argv);
  build_rmhost_db();
  return 0;
}
