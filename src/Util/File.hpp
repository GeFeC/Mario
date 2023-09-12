#pragma once

#include <fstream>
#include <sstream>

namespace mario::util{

static auto get_file_content(const std::string& path){
  auto file = std::ifstream(path);
  auto ss = std::stringstream();
  ss << file.rdbuf();

  return ss.str();
}

} //namespace mario::util
