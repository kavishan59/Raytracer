#include "color.hpp"
#include <fstream>



void write_color(std::ofstream& file, const color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  //Translate [0,1] to [0,255]
  int rbyte = int(255.999 * r);
  int gbyte = int(255.999 * g);
  int bbyte = int(255.999 * b);
  
  //write pixel_color
  file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
