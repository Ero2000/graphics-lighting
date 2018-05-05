#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;

  normalize(normal);
  normalize(light[LOCATION]);

  i.red = calculate_ambient(alight, areflect).red + calculate_diffuse(light, dreflect, normal).red + calculate_specular(light, sreflect, view, normal).red;
  i.green = calculate_ambient(alight, areflect).green + calculate_diffuse(light, dreflect, normal).green + calculate_specular(light, sreflect, view, normal).green;
  i.blue = calculate_ambient(alight, areflect).blue + calculate_diffuse(light, dreflect, normal).blue + calculate_specular(light, sreflect, view, normal).blue;
  
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];

  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double ct = dot_product(normal, light[LOCATION]);
  d.red = dreflect[RED] * ct * light[COLOR][RED];
  d.green = dreflect[GREEN] * ct * light[COLOR][GREEN];
  d.blue = dreflect[BLUE] * ct * light[COLOR][GREEN];

  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double big[3], tmp;

  big[0] = (2 * dot_product(normal, light[LOCATION]) * normal[RED]) - light[LOCATION][RED]; //RED
  big[1] = (2 * dot_product(normal, light[LOCATION]) * normal[GREEN]) - light[LOCATION][GREEN]; //GREEN
  big[2] = (2 * dot_product(normal, light[LOCATION]) * normal[BLUE]) - light[LOCATION][BLUE]; //BLUE
  
  tmp = dot_product(big, view); 
  if (tmp < 0)
    tmp = 0;
  tmp = pow(tmp, SPECULAR_EXP); 

  s.red = light[COLOR][RED] * sreflect[RED] * tmp;
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * tmp;
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * tmp;

  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255)
    c->red = 255;
  if (c->blue > 255)
    c->blue = 255;
  if (c->green > 255)
    c->green = 255;
}

//vector functions
//normalize vector, should modify the parameter
void normalize( double *vector ) {
  double temp = sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));
  vector[0] /= temp;
  vector[1] /= temp;
  vector[2] /= temp;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
