#pragma once

#include "hittable.hpp"
#include "file.hpp"
#include "rt_const_util.hpp"
#include "vec3.hpp"
#include <cmath>
#include <fstream>
#include "material.hpp"

class camera {

public:
  double aspect_ratio = 1.0;   //ratio of image width over height
  int image_width = 100;
  int samples_per_pixel = 10;   //count of random samples for each pixel (anti aliasing)
  int max_depth = 10; //maximum number of ray bounces into scene

  double vfov = 90; //vertical view angle (field of view)
  point3 lookfrom = point3(0,0,0);  //point camera is looking from
  point3 lookat = point3(0,0,-1);   //point camera is looking at 
  vec3 vup = vec3(0,1,0);           //camera-relative "up" direction


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
  vec3 u, v, w;           //camera frame basics vectors

  void initialize(){
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    
    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;
  
    //determine viewport dimension
    auto focal_length = (lookfrom - lookat).length();      //distance between camera and the viewport
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta/2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
   
    //calculate u,v,w unit basis vectors for the camera coordinate frame.
    w = unit_vector(lookfrom -lookat);
    u = unit_vector(cross(vup,w));
    v = cross(w,u);


    //calculate the vector across horizontal and vertical edges
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    //calculate horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v= viewport_v / image_height;

    //calculate the location of the upper left pixel.
    auto viewport_upper_left = center - (focal_length* w) - viewport_u / 2 - viewport_v /2;
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

    if (world.hit(r,interval(0.001,infinity), rec)){
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered))
        return attenuation * ray_color(scattered, depth - 1, world);
      return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5,0.7,1.0);
  }
};
