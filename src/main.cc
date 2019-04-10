#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <filesystem>
#include <list>
#include <memory>

#include "poppler/cpp/poppler-document.h"
#include "poppler/cpp/poppler-page.h"

#include <unicode/unistr.h>

using std::cout, std::endl, std::string, std::vector;
namespace fs = std::filesystem;
namespace icu = icu_62;

struct Entry {
  double height;
  double xpos;
  icu::UnicodeString text;
};

struct Group {
  double height;
  vector<icu::UnicodeString> terms;
};

vector<Group> groupEntries(std::list<Entry> es) {
  vector<Group> res;
  double cur = 0;
  vector<icu::UnicodeString> words;
  for (auto e : es) {
    if (cur != e.height && words.size() > 0) {
      res.push_back({cur, words});
      cur = e.height;
      words.empty();
    } else if (cur != e.height) {
      cur = e.height;
    } else {
      words.push_back(e.text);
    }
  }

  return res;
}


void identifyHeadline(fs::path f) {

  auto pdf =
    std::unique_ptr<poppler::document>(poppler::document::load_from_file(f));

  std::cout << "Got pdf " << pdf->get_title().to_latin1() << endl;

  auto pg = std::unique_ptr<poppler::page>(pdf->create_page(0));

  auto lst = pg->text_list();
  std::list<Entry> boxes;

  for (poppler::text_box& t : lst) {
    string s = t.text().to_latin1();
    //cout << t.bbox().height() <<  " " << s << endl;
    boxes.push_back({ t.bbox().height(),
                      t.bbox().x(),
                      icu::UnicodeString::fromUTF8(icu::StringPiece(s.c_str())) });
  }

  boxes.sort([](const Entry& a, const Entry& b) {
               return a.height > b.height;
             });

  for (auto& e: boxes) {
    string ee;
    e.text.toUTF8String(ee);
    cout << e.height << " " << e.xpos << " " << ee << endl;
  }
}

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

  cout << "I got " << argc << " arguments which were" << endl;

  std::for_each(args.begin(), args.end(),
                [](string const s) {
                  cout << s << endl;
                });

  fs::path f = args[1];

  if (!fs::is_regular_file(f)) {
    cout << "File " << f << " does not exists" << endl;
    std::exit(EXIT_FAILURE);
  }

  identifyHeadline(f);
}
