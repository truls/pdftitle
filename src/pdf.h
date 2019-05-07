#ifndef _PDF_H

#include <filesystem>
#include <unicode/unistr.h>
namespace fs = std::filesystem;

icu::UnicodeString identifyHeadline(fs::path f);

#endif //_PDF_H
