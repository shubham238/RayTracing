// Here we are implementing the Phong's illumination model

#include <bits/stdc++.h>
#include "Objects.h"
using namespace std;
#ifndef __ILLUMINATION__
#define __ILLUMINATION__

//takes a point p, "norm"-normal at that point, kd- diffuse material property of the point, source: light source
//returns diffuse illumination intensity at that point due to single light source
// assuming that light from light source to p is not obstructed.
vectors getDiffuseIntensity(Point p, vectors norm, vectors kd, Light source);

// returns specular illumination intensity due to light source in given viewing direction
vectors getSpecularIntensity(Point p, vectors norm, vectors ks, float n_spec, Light source, vectors view_dir);

// given the material property and ambient light intensity, it returns the ambient intensity at that point
vectors getAmbientIntensity(vectors ka, vectors amb);

#endif
