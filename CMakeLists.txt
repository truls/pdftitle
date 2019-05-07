cmake_minimum_required (VERSION 2.8)
project (pdftitle CXX)
add_definitions (-Wall -Wextra -Werror -pedantic -std=c++17)

include(FindPkgConfig)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pkg_check_modules (POPPLERCPP REQUIRED poppler-cpp>=0.67)
include_directories(${POPPLERCPP_INCLUDE_DIRS})
link_directories(${POPPLERCPP_LIBRARY_DIRS})

pkg_check_modules (ICU REQUIRED icu-uc>=62)
include_directories(${ICU_INCLUDE_DIRS})
link_directories(${ICU_LIBRARY_DIRS})
string(REPLACE ";" " " ICU_LDFLAGS "${ICU_LDFLAGS}")

set (CMAKE_CXX_STANDARD_LIBRARIES "-lstdc++fs")

add_executable (pdftitle src/main.cc src/pdf.cc)
set_target_properties(pdftitle
  PROPERTIES LINK_FLAGS "${POPPLERCPP_LDFLAGS} ${ICU_LDFLAGS}")
