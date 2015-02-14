#include <bits/stdc++.h>
using namespace std;
#include "Objects.h"
#ifndef __OBJECT__READER__
#define __OBJECT__READER__

// reads input from file stream given that n sphere data is to be read
// fstream object cannot be copied, therefore passed by reference
Sphere** readSphere(fstream& file, int n);

// reads input from file stream for n planes
Plane** readPlane(fstream& file, int n);

Surface** readSurface(fstream& file, int n);

Light** readLightSources(fstream& file, int n);

#endif
