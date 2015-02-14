#include <bits/stdc++.h>
#ifndef _TRANSFORMATIONS_H_
#define _TRANSFORMATIONS_H_
#include "Objects.h"
#include "Setup.h"
using namespace std;

//initialize a row matrix of homogeneous coordinates of point p
void init_matrix(Point p, float homo_coord[1][4]);
//initialize a 4x4 transformation matrix with each cell assigned to 0
void null_matrix(float matrix[4][4]);
//multiplies the row matrix of point and the transformation matrix and stores result in another matrix c
void mult_matrix(float a[1][4], float b[4][4], float c[1][4]);

//defines a transformation matrix corresponding to translation by l,m,n viz x,y,z axes
void translate_matrix(float l, float m, float n, float matrix[4][4]);
//returns a new point translated with l, m, n magnitude w.r.to point pt.
Point translate(Point pt, float l, float m, float n);

//defines a transformation matrix correspoding to scaling factor
void scale_matrix(float sx, float sy, float sz, float matrix[4][4]);
//returns a new point scaled with respect to old point
Point scale(Point p, float sx, float sy, float sz);

//defines a transformation matrix correspoding to rotation of point about x axis
void rotatex_matrix(float theta, float matrix[4][4]);
//returns the point rotated w.r.to x axis
Point rotatex(Point p, float theta);
//defines a transformation matrix correspoding to rotation of point about y axis
void rotatey_matrix(float phi, float matrix[4][4]);
//returns the point rotated w.r.to y axis
Point rotatey(Point p, float phi);
//defines a transformation matrix correspoding to rotation of point about z axis
void rotatez_matrix(float psi, float matrix[4][4]);
//returns the point rotated w.r.to x axis
Point rotatez(Point p, float psi);

//defines a transformation matrix correspoding to shear
//for shear about yz plane fill b, c; xz plane fill d, f; xy plane fill g, h
void shear_matrix(float b, float c, float d, float e, float g, float h, float matrix[4][4]);
//returns the point p corresponding to the appropriate shear forces.
Point shear(Point p, float b, float c, float d, float e, float g, float h);

#endif //_TRANSFORMATIONS_H_
