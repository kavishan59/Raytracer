#include "rt_const_util.hpp"
#include <cmath>


//vector utlity Functions
std::ostream& operator<<(std::ostream& out, const vec3& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3& u, const vec3& v) {
  return vec3(u.e[0] + v.e[0],u.e[1] + v.e[1],u.e[2] + v.e[2]);
}

vec3 operator-(const vec3& u, const vec3& v) {
  return vec3(u.e[0] - v.e[0],u.e[1] - v.e[1],u.e[2] - v.e[2]);
}

vec3 operator*(const vec3& u, const vec3& v) {
  return vec3(u.e[0] * v.e[0],u.e[1] * v.e[1],u.e[2] * v.e[2]);
}

vec3 operator*(double t, const vec3& v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

vec3 operator*(const vec3& v, double t) {
  return t * v;
}

vec3 operator/(const vec3& v, double t) {
  return (1/t) * v;
}

double dot(const vec3& u, const vec3& v) {
  return (u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2]);
}

vec3 cross(const vec3& u, const vec3& v) {
  return vec3(u.e[1]*v.e[2] - u.e[2]*v.e[1],
              u.e[2]*v.e[0] - u.e[0]*v.e[2],
              u.e[0]*v.e[1] - u.e[1]*v.e[0]);
}

vec3 unit_vector(const vec3& v) {
  return v / v.length();
}

vec3 vec3::random() {
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max) {
    return vec3(random_double(min,max), random_double(min,max), random_double(min, max));
}


//random unit vector but inside an unit sphere ( x^2 + y^2 + z^2 <= 1)
vec3 random_unit_vector() {
  while (true) {
    auto p = vec3::random(-1,1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1)
      return p/sqrt(lensq);
  }
}

vec3 random_on_hemisphere(const vec3& normal) {
  vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere,normal) > 0.0)  // in the same hemisphere as normal
    return on_unit_sphere;
  else
    return -on_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n){
  return v - 2*dot(v,n)*n;
}
