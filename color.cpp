#include "color.hpp"
#include <fstream>
#include "interval.hpp"



void write_color(std::ofstream& file, const color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  //Translate [0,1] to [0,255]
  static const interval intensity(0.000,0.999);
  int rbyte = int(256 * intensity.clamp(r));
  int gbyte = int(256 * intensity.clamp(g));
  int bbyte = int(256 * intensity.clamp(b));
  
  //write pixel_color
  file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
