#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <vector>

#include "pdf.h"

using std::cout, std::endl, std::string, std::vector;
namespace fs = std::filesystem;

int main (int argc, char** argv) {

  if (argc != 2) {
    cout << "need a single argument" << endl;
    std::exit(EXIT_FAILURE);
  }

  vector<string> args;
  for (int i = 0; i < argc; i++) {
    string const a(argv[i]);
    args.push_back(a);
  }

  fs::path f = args[1];

  if (!fs::is_regular_file(f)) {
    cout << "File " << f << " does not exists" << endl;
    std::exit(EXIT_FAILURE);
  }

  string res;
  identifyHeadline(f).toUTF8String(res);
  cout << res << std::endl;
}
