#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//utility function

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

//common headers

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "interval.hpp"
