#include "../include/utils.h"
#include <math.h>

Vector2 normalize(Vector2 v){
  Vector2 v_n = v;
  
  double m = sqrt(v.x*v.x + v.y*v.y);
  if (m > 0){
    v_n.x = v_n.x/m;
    v_n.y = v_n.y/m;
  }

  return v_n;
}

Vector2 add(Vector2 v1, Vector2 v2){
  Vector2 sum;

  sum.x = v1.x + v2.x;
  sum.y = v1.y + v2.y;

  return sum;
}

Vector2 scale(Vector2 v1, double s){
  Vector2 scaled_v;

  scaled_v.x = v1.x * s;
  scaled_v.y = v1.y * s;

  return scaled_v;
}

Vector2 rotate(Vector2 v, double theta){
  Vector2 rotated;
  double cos_theta = cos(theta);
  double sin_theta = sin(theta);

  rotated.x = v.x * cos_theta - v.y * sin_theta;
  rotated.y = v.x * sin_theta + v.y * cos_theta;

  return rotated;
}
