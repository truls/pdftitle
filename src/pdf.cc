#include <iostream>
#include <list>
#include <string>
#include <list>
#include <vector>
#include <filesystem>

#include <unicode/unistr.h>
#include "poppler/cpp/poppler-document.h"
#include "poppler/cpp/poppler-page.h"

/**
   Classification criteria (current):
   - Simply an in-order concatenation of the text with the largest
   font-size on the first page

   Classification weight criteria (future):
   - Location on page. Below center is getting score deduction
 */


using std::cout, std::endl, std::string, std::vector;
namespace fs = std::filesystem;

struct Entry {
  double height;
  double xpos;
  bool has_space;
  icu::UnicodeString text;
};

struct Group {
  double height;
  vector<Entry> boxes;
};

static icu::UnicodeString concatGroup(Group g) {
  icu::UnicodeString res;
  double prevX = g.boxes[0].xpos;
  for(auto& e: g.boxes) {
    res.append(e.text);
    if (e.has_space || e.xpos > prevX) {
      res.append(icu::UnicodeString(" "));
    }
    prevX = e.xpos;
  }
  return res;
}

static vector<Group> groupEntries(std::list<Entry> es) {
  // TODO: Don't spend time grouping all words. The current
  // implementation won't need them.
  vector<Group> res;
  double cur = 0;
  vector<Entry> boxes;
  for (const auto& e : es) {
    if (cur != e.height && boxes.size() > 0) {
      res.push_back({cur, boxes});
      cur = e.height;
      boxes.clear();
    } else if (cur != e.height) {
      cur = e.height;
    }
    boxes.push_back(e);
  }

  return res;
}


icu::UnicodeString identifyHeadline(fs::path f) {

  auto pdf =
    std::unique_ptr<poppler::document>(poppler::document::load_from_file(f));

  auto pg = std::unique_ptr<poppler::page>(pdf->create_page(0));

  auto lst = pg->text_list();
  std::list<Entry> boxes;

  for (const poppler::text_box& t : lst) {
    string s = t.text().to_latin1();
    //cout << t.bbox().height() <<  " " << s << endl;
    if (! t.bbox().is_empty()) {
      boxes.push_back(
                      {t.bbox().height(), t.bbox().x(), t.has_space_after(),
           icu::UnicodeString::fromUTF8(icu::StringPiece(s.c_str()))});
    }
  }

  boxes.sort([](const Entry& a, const Entry& b) {
               return a.height > b.height;
             });

  // for (auto& e: boxes) {
  //   string ee;
  //   e.text.toUTF8String(ee);
  //   //cout << e.height << " " << e.xpos << " \"" << ee << "\" " << e.has_space << endl;
  // }

  auto grouped = groupEntries(boxes);

  icu::UnicodeString res;

  for (auto& g: grouped) {
      string s;
      res = concatGroup(g);
      if (g.boxes.size() > 1) {
        break;
      }  else {
        res.toUTF8String(s);
        std::cerr << "Skipping (too sort): " << g.height << " " << s << std::endl;
      }
  }

  return res;
}
