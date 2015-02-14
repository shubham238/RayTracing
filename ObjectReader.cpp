#include <bits/stdc++.h>
using namespace std;
#include "ObjectReader.h"

Sphere** readSphere(fstream& file, int n)
{
	Sphere** sp = new Sphere*[n];
	f(i,n)
	{
		sp[i] = new Sphere;
		file >> sp[i]->center.x >> sp[i]->center.y >> sp[i]->center.z;
		printf("%f\n",(sp[i]->center).x);
		file >> sp[i]->radius;
		file >> sp[i]->ka.x >> sp[i]->ka.y >> sp[i]->ka.z;
		file >> sp[i]->kd.x >> sp[i]->kd.y >> sp[i]->kd.z;
		file >> sp[i]->ks.x >> sp[i]->ks.y >> sp[i]->ks.z;
		file >> sp[i]->mu;
		file >> sp[i]->ktr;
		file >> sp[i]->n_spec;
		file >> sp[i]->ktg;
	}
	return sp;
}

Plane** readPlane(fstream& file, int n)
{
	Plane** pl = new Plane*[n];
	int l;
	f(i,n)
	{
		pl[i] = new Plane;
		file >> l;
		pl[i]->num_points = l;
		pl[i]->ar = new Point[l];
		f(j,l)
			file >> pl[i]->ar[j].x >> pl[i]->ar[j].y >> pl[i]->ar[j].z;
		file >> pl[i]->ka.x >> pl[i]->ka.y >> pl[i]->ka.z;
		file >> pl[i]->kd.x >> pl[i]->kd.y >> pl[i]->kd.z;
		file >> pl[i]->ks.x >> pl[i]->ks.y >> pl[i]->ks.z;
		file >> pl[i]->mu;
		file >> pl[i]->ktr;
		file >> pl[i]->n_spec;
		file >> pl[i]->ktg;
	}
	return pl;
}

Surface** readSurface(fstream& file, int n)
{
	Surface** pl = new Surface*[n];
	int l;
	f(i,n)
	{
		pl[i] = new Surface;
		file >> l;
		pl[i]->num_points = l;
		pl[i]->ar = new Point[l];
		f(j,l)
			file >> pl[i]->ar[j].x >> pl[i]->ar[j].y >> pl[i]->ar[j].z;
		file >> pl[i]->ka.x >> pl[i]->ka.y >> pl[i]->ka.z;
		file >> pl[i]->kd.x >> pl[i]->kd.y >> pl[i]->kd.z;
		file >> pl[i]->ks.x >> pl[i]->ks.y >> pl[i]->ks.z;
		file >> pl[i]->mu;
		file >> pl[i]->ktr;
		file >> pl[i]->n_spec;
		file >> pl[i]->ktg;
	}
	return pl;
}

Light** readLightSources(fstream& file, int n)
{
	Light** lt = new Light*[n];
	f(i,n)
	{
		lt[i] = new Light;
		file >> lt[i]->location.x >> lt[i]->location.y >> lt[i]->location.z;
		file >> lt[i]->intensity.x >> lt[i]->intensity.y >> lt[i]->intensity.z;
	}
	return lt;
}
