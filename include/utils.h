#ifndef UTILS_H
#define UTILS_H

#include <math.h>

typedef struct{
  double x;
  double y;
}Vector2;

typedef struct{
  double x;
  double y;
  double z;
}Vector3;

Vector2 normalize(Vector2 v);
Vector2 add(Vector2 v1, Vector2 v2);
Vector2 scale(Vector2 v1, double s);
Vector2 rotate(Vector2 v, double theta);

#endif 
