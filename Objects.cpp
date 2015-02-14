#include <bits/stdc++.h>
using namespace std;
#include "Objects.h"
#include <cmath>

const float EPSILON = 0.01; //delta, for comparison


//point functions
Point make_point(float x, float y, float z)
{
	Point p;
	p.x = x, p.y =y, p.z = z;
	return p;
}
//point functions end

// vectors functions
float vectors::mod()
{
	return sqrt(x*x+y*y+z*z);
}
float vectors::mod(vectors a)
{
	return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

float dotproduct(vectors a, vectors b)
{
	return ((a.x*b.x)+(a.y*b.y)+(a.z*b.z));
}

vectors crossproduct(vectors a, vectors b)
{
	float cx = a.y*b.z - b.y*a.z;
	float cy = b.x*a.z - b.z*a.x;
	float cz = a.x*b.y - a.y*b.x;
	vectors c = vectors(cx, cy, cz);
	return c;
}

vectors addvectors(vectors a, vectors b)
{
	float vx = a.x+b.x;
	float vy = a.y+b.y;
	float vz = a.z+b.z;
	vectors v = vectors(vx, vy, vz);
	return v;
}

vectors subvectors(vectors a, vectors b)
{
	float vx = a.x-b.x;
	float vy = a.y-b.y;
	float vz = a.z-b.z;
	vectors v = vectors(vx, vy, vz);
	return v;
}

vectors multscalar(float f, vectors a)
{
	float vx = f*a.x;
	float vy = f*a.y;
	float vz = f*a.z;
	vectors v = vectors(vx, vy, vz);
	return v;
}

vectors multvectors(vectors a, vectors b)
{
	float vx = a.x*b.x;
	float vy = a.y*b.y;
	float vz = a.z*b.z;
	vectors v = vectors(vx, vy, vz);
	return v;
}

vectors normalize(vectors a)
{
	if(a.mod()==0.0)
		return a;
	float m = a.mod();
	return multscalar(1.0/m,a);
}

vectors point_to_vector(Point a)
{
	return vectors(a.x,a.y,a.z);
}

Point vector_to_point(vectors v)
{
	return make_point(v.x,v.y,v.z);
}

float area_of_triangle(Point a, Point b, Point c)
{
	vectors v1 = vectors(a.x-b.x, a.y-b.y, a.z-b.z);
	vectors v2 = vectors(c.x-b.x, c.y-b.y, c.z-b.z);
	vectors crs = crossproduct(v1,v2);
	float r = crs.mod()/2.0;
	// printf("Area between following points:\n");
	// a.print(); b.print(), c.print();
	// printf("%f\n",r);
	// printf("Cross pdt. "); crs.print();
	return r;
}

// vectors functions over

//line functions begin

Line make_line(Point p, vectors d)
{
	Line l;
	l.p = p;
	l.direction = d;
	return l;
}

//line function end

//Sphere functions begin
float square(float x)
{
	return x*x;
}
float discriminant(float a, float b, float c)
{
	return (b*b-4*a*c);
}

float get_min_root(float A, float B, float C)
{
	float disc = sqrt(discriminant(A,B,C));
	float t0 = (-B-disc)/(2*A);
	float t1 = (-B+disc)/(2*A);
	return min(t0,t1);
}

Point Sphere::find_intersection(Line l)
{
	float A = 1.0;
	float xd = l.direction.x, xo = l.p.x, xc = center.x;
	float yd = l.direction.y, yo = l.p.y, yc = center.y;
	float zd = l.direction.z, zo = l.p.z, zc = center.z;
	float B = 2*(xd*(xo-xc) + yd*(yo-yc) + zd*(zo-zc));
	float C = square(xo-xc) + square(yo-yc) + square(zo-zc) - square(radius);
	if(discriminant(A,B,C) < 0)
		return make_point(MAX_COORD, MAX_COORD, MAX_COORD); //point at infinity
	float t = get_min_root(A,B,C);
	return make_point(xo+xd*t, yo+yd*t, zo+zd*t);
}


float Sphere::param_intersection(Line l)
{
	float A = 1.0;
	float xd = l.direction.x, xo = l.p.x, xc = center.x;
	float yd = l.direction.y, yo = l.p.y, yc = center.y;
	float zd = l.direction.z, zo = l.p.z, zc = center.z;
	float B = 2*(xd*(xo-xc) + yd*(yo-yc) + zd*(zo-zc));
	float C = square(xo-xc) + square(yo-yc) + square(zo-zc) - square(radius);
	if(discriminant(A,B,C) < 0)
		return -1; //no intersection
	
	float disc = sqrt(discriminant(A,B,C));
	float t0 = (-B-disc)/(2*A);
	float t1 = (-B+disc)/(2*A);
	if(t0<0 && t1<0)
		return -1;
	if(t1>0 && t0>0)
	{
		// printf("Yo Sphere intersection\n");
		return min(t0,t1);
	}
	else
	{
		// printf("Yo Sphere intersection\n");
		return max(t0,t1);
	}
		
}

vectors Sphere::get_normal(Point p)
{
	return normalize(vectors(p.x - center.x, p.y - center.y, p.z - center.z));
}

// sphere functions over

vectors difference_vector(Point p1, Point p2)
{
	return normalize(vectors(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z));
}
// Plane functions begin

vectors Plane::get_normal()
{
	vectors a = difference_vector(ar[0],ar[1]);
	vectors b = difference_vector(ar[2],ar[1]);
	return normalize(crossproduct(a,b));
}

Point Plane::find_intersection(Line l)
{
	vectors n = get_normal();
	float D = -(dotproduct(n,point_to_vector(ar[0])));
	vectors r0 = point_to_vector(l.p);
	vectors rD = l.direction;
	float t = -(dotproduct(n,r0) +  D)/(dotproduct(n,rD));
	Point ret_p = vector_to_point(addvectors(r0, multscalar(t,rD)));
	if(isContained(ret_p))
		return ret_p;
	return make_point(MAX_COORD,MAX_COORD,MAX_COORD);
}

float Plane::param_intersection(Line l)
{
	vectors n = get_normal();
	float D = -(dotproduct(n,point_to_vector(ar[0])));
	vectors r0 = point_to_vector(l.p);
	vectors rD = l.direction;
	float t = -(dotproduct(n,r0) +  D)/(dotproduct(n,rD));
	Point ret_p = vector_to_point(addvectors(r0, multscalar(t,rD)));
	if(isContained(ret_p))
		return t;
	 return -1;
}

// bool Plane::isContained(Point p)
// {
// 	vectors dir = difference_vector(ar[0],ar[1]);
// 	int count=0;
// 	f(i,num_points-1)
// 	{
// 		if(segment_line_intersection(ar[i],ar[i+1],p,dir))
// 			count++;
// 	}
// 	return (count&1);
// }

// bool segment_line_intersection(Point a, Point b, Point p, vectors dir)
// {
// 	//later....
// 	return false;
// }

bool Plane::isContained(Point p)
{
	float ar_p = 0;
	f(i,num_points-1)
	{
		ar_p += area_of_triangle(p,ar[i],ar[i+1]);
	}
	ar_p += area_of_triangle(p,ar[0],ar[num_points-1]);

	float ar_f = 0;
	for(int i=1; i<num_points-1;i++)
	{
		ar_f += area_of_triangle(ar[0],ar[i],ar[i+1]);
	}
	float diff = ar_f-ar_p;
	// printf("Area diff = %f\n",diff );
	return (diff<EPSILON && -diff<EPSILON);

}
// barycentric, "ass"uming triangle

float __abs__(float a)
{
	if(a<0)
		return -a;
	else 
		return a;
}

// bool Plane::isContained(Point p)
// {
// 	Point v1,v2,v3;
// 	v1 = ar[0], v2 = ar[1], v3 = ar[2];
// 	float A1 = area_of_triangle(p,v3,v2);
// 	float A2 = area_of_triangle(p,v1,v3);
// 	float A3 = area_of_triangle(p,v1,v3);
// 	float A = area_of_triangle(v1,v2,v3);
// 	float u,v,w;
// 	u = A1/A, v = A2/A, w = A3/A;
// 	Point p_new = make_point(u*v1.x+v*v2.x+w*v3.x, u*v1.y+v*v2.y+w*v3.y, u*v1.z+v*v2.z+w*v3.z);
// 	if(__abs__(p_new.x - p.x) <= EPSILON && __abs__(p_new.y - p.y)<=EPSILON && __abs__(p_new.z - p.z)<=EPSILON)
// 		return true;
// 	return false;
// }


vectors get_reflected_ray(vectors incident, vectors normal)
{
//	vectors minus_inc = multscalar(-1,incident); //L = -L
	//vectors next = multscalar(2,multscalar(dotproduct(normal,minus_inc),normal)); //2(L.N)N
	vectors next = multscalar(2, multscalar(dotproduct(normal, incident), normal));
	return subvectors(incident, next);
}

// n1 is incident wala, n2 is transmiited wala ka medium...
vectors get_transmitted_ray(vectors incident, vectors normal, float n1, float n2)
{
	return incident;
	float costheta_i = -1*dotproduct(normalize(incident),normalize(normal)); 
	float sintheta_i = sqrt(1 - square(costheta_i));
	float costheta_t = sqrt(square(n2) - square(n1*sintheta_i))/n2;
	vectors prev = addvectors(incident, multscalar(costheta_i,normal));
	prev = multscalar(n1/n2,prev);
	prev = subvectors(prev, multscalar(costheta_t,normal));
	return prev;
}
