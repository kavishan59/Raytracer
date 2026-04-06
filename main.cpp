#include "rt_const_util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "file.hpp"
#include <string>
#include <fstream>


int main (int argc, char *argv[]) {

  //creating output file
  std::string filename = create_ppm_filename();
  std::ofstream file(filename);
  if (!file) {
    std::cerr << "Error creating file" << std::endl;
    return 1;
  }

  //world
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0,0,-1),0.5));
  world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));
  
  camera cam;

  cam.aspect_ratio =16.0/9.0;
  cam.image_width = 1080;
  cam.samples_per_pixel = 100;
  cam.max_depth = 10;

  cam.render(world,file);
  file.close();
  return 0;
}
