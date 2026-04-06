#pragma once

#include "hittable.hpp"
#include "file.hpp"
#include "rt_const_util.hpp"
#include "vec3.hpp"
#include <fstream>

class camera {

public:
  double aspect_ratio = 1.0;   //ratio of image width over height
  int image_width = 100;
  int samples_per_pixel = 10;   //count of random samples for each pixel (anti aliasing)
  int max_depth = 10; //maximum number of ray bounces into scene


  void render(const hittable& world, std::ofstream& file){
    initialize();

    file << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    for(int j = 0; j < image_height; j++){
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
      for(int i = 0; i < image_width; i++){
        color pixel_color(0,0,0);
        for (int sample = 0;sample < samples_per_pixel; sample++){
          ray r = get_ray(i,j);
          pixel_color += ray_color(r,max_depth,world);
        }
        write_color(file,pixel_samples_scale* pixel_color);
      }
    }
    std::clog << "\rDone.             \n";
  }

private:
  int image_height;       // rendered image height , calculated using aspect ratio / width
  point3 center;          //camera center
  point3 pixel00_loc;     //location of the center of pixel 0,0
  vec3 pixel_delta_u;     //offset to pixel to the right
  vec3 pixel_delta_v;     //offset to pixel below
  double pixel_samples_scale; //color scale factor for a sum of pixel samples

  void initialize(){
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    
    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = point3(0,0,0);
  
    //determine viewport dimension
    auto focal_length = 1.0;      //distance between camera and the viewport
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    
    //calculate the vector across horizontal and vertical edges
    auto viewport_u = vec3(viewport_width, 0 , 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //calculate horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v= viewport_v / image_height;

    //calculate the location of the upper left pixel.
    auto viewport_upper_left = center - vec3(0,0,focal_length) - viewport_u / 2 - viewport_v /2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 

  }

  ray get_ray(int i, int j) const {
    //construct camera ray from the origin and directed randomly around pixel center location i,j
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j +offset.y()) * pixel_delta_v);
    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin,ray_direction);
  }

  vec3 sample_square() const {
    //return vector to a random point in the [-0.5,-0.5] - [+0.5,+0.5] unit square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }



  color ray_color(const ray& r, int depth, const hittable& world) const {
    hit_record rec;

    // if we exceeded the ray bouce limit , no more light
    if (depth <= 0)
      return color(0,0,0);

    if (world.hit(r,interval(0,infinity), rec)){
      //diffuse material , here a gray material
      vec3 direction = random_on_hemisphere(rec.normal);
      return 0.5 * ray_color(ray(rec.p,direction), depth - 1,world);
      //return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5,0.7,1.0);
  }
};
