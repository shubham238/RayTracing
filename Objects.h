#include <bits/stdc++.h>
using namespace std;
#ifndef __OBJECTS__H__
#define __OBJECTS__H__

#define f(i,n) for(int i=0;i<n;i++)
#define fab(i,a,b) for(int i=a;i<=b;i++)
const float MAX_COORD = 1000000.0;
class Point
{
public:
	float x,y,z;
	void operator=(Point p)
	{
		x = p.x, y = p.y, z = p.z;
	}
	void print()
	{
		printf("%f, %f, %f\n",x,y,z);
	}
};

Point make_point(float x, float y, float z);

class vectors
{
public:
	float x, y, z;
	vectors(){x=0;y=0;z=0;};
	vectors(float u, float v, float w)
	{	x = u;
		y = v;
		z = w;};
	float mod();
	float mod(vectors a);
	void operator=(vectors a)
	{
		x = a.x,y = a.y, z = a.z;
	}
	void print()
	{
		printf("%f, %f, %f\n",x,y,z);
	}
};

float dotproduct(vectors a, vectors b);

vectors crossproduct(vectors a, vectors b);

vectors addvectors(vectors a, vectors b);

vectors subvectors(vectors a, vectors b);

vectors multscalar(float f, vectors a);

vectors multvectors(vectors a, vectors b);

vectors normalize(vectors a);

float area_of_triangle(Point a, Point b, Point c);

// p1-p2
vectors difference_vector(Point p1, Point p2); //returns normalized difference vector

float discriminant(float a, float b, float c);
float get_min_root(float A, float B, float C);

vectors point_to_vector(Point a);

Point vector_to_point(vectors v);

struct Line
{
	Point p;
	vectors direction;
};

Line make_line(Point p, vectors d);

class Object
{
public:
	vectors ka, kd, ks; //mu- refractive index, kr - reflectivity
	float n_spec; //specular power coefficient
	float ktg, ktr, mu; //ktg-> transmission coefficient, ktr-> reflection factor
	void print_properties()
	{
		printf("K_ambient: "); ka.print();
		printf("K_diffuse: "); kd.print();
		printf("K_specular: "); ks.print();
		printf("Refractive index: %f", mu); 
		printf("Reflectivity: %f", ktr); 
		printf("Specular coefficient: %f\n",n_spec);
		printf("Transmission coefficient: %f\n",ktg);
	}
};

class Sphere : public Object
{
public:
	Point center; //center of the sphere
	float radius;
	Sphere(Point p, float r, vectors a, vectors b, vectors c, float d, float e, float n, float f)
	{
		center = p;
		radius = r;
		ka = a, kd = b, ks = c, mu = d, ktr = e, n_spec = n, ktg = f;
	}

	Sphere(Point p, float r)
	{
		center = p, radius = r;
	}
	Sphere()
	{
		center = make_point(0,0,0);
		radius = 0.0;
		ka = vectors(0.0,0.0,0.0);
		kd = vectors(0.0,0.0,0.0);
		ks = vectors(0.0,0.0,0.0);
		ktr = ktg = mu = 0.0;
		n_spec = 0.0;
	}
	Point find_intersection(Line l); //finds the nearest point of intersection of line and sphere
	float param_intersection(Line l); //
	vectors get_normal(Point p); //returns the direction of normal at point p on the sphere

	void print()
	{
		printf("\nSphere data:\n");
		printf("Center: "); center.print();
		printf("Radius: %f\n",radius);
		print_properties();
	}
};


class Plane : public Object
{
public:
	Point* ar; //in CCW direction
	int num_points;
	Plane()
	{
		ar = NULL;
		num_points = 0;
	}
	Plane(Point* p, int len, vectors a, vectors b, vectors c, float d, float e, float n, float f)
	{
		ar = p;
		num_points = len;
		ka = a, kd = b, ks = c, mu = d, ktr = e, n_spec = n, ktg = f;
	}

	Plane(Point* a, int l)
	{
		ar = a, num_points = l;
	}

	Point find_intersection(Line l); //find the intersection of line with plane
	float param_intersection(Line l);
	vectors get_normal(); //get normal to the plane

	bool isContained(Point p); //used to find whether intersection point lies in the plane

	void print()
	{
		printf("Plane data:\n");
		printf("Number of points = %d\n",num_points);
		f(i,num_points) ar[i].print();
		print_properties();
	}
};

class Surface : public Plane
{
public:
	Surface(){}
	void print()
	{
		printf("Surface data:\n");
		printf("Number of points = %d\n",num_points);
		f(i,num_points) ar[i].print();
		print_properties();
	}
};

struct Light
{
	Point location; //Location of the light source
	vectors intensity; //intensity values for RGB respectively
	void print()
	{
		printf("Light data:\n");
		location.print();
		intensity.print();
	}
};


Point line_intersection(Line a, Line b); //finds the point of intersection of line a and b, if parallel, returns max_point

bool segment_line_intersection(Point a, Point b, Point p, vectors dir);//checks if line segment made by 'a' and 'b' intersects the line made by 'p' and 'dir'

// here incident is original direction, not the one in the slides!
vectors get_reflected_ray(vectors incident, vectors normal); //returns the reflected ray, given incident ray and normal at point of incidence

vectors get_transmitted_ray(vectors incident, vectors normal, float n1, float n2); //n1 is mu of current medium, n2 is mu of next medium.



#endif
