#include "rt_const_util.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>


std::string create_ppm_filename(){
  int i = 0;
  std::string filename;
  
  do {
    filename = "output" + std::to_string(i) + ".ppm";
    i++;
  } while (std::filesystem::exists(filename));

  return filename;
}

