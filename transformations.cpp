
#include <cmath>
#include "transformations.h"
//using namespace std;
#define PI 3.14159265

void init_matrix(Point p, float homo_coord[1][4])
{
	homo_coord[0][0] = p.x;
	homo_coord[0][1] = p.y;
	homo_coord[0][2] = p.z;
	homo_coord[0][3] = 1.0;
}

void null_matrix(float matrix[4][4])
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			matrix[i][j] = 0;
		}
	}
}

void mult_matrix(float a[1][4], float b[4][4], float c[1][4])
{
	for(int i=0; i<1; i++)
	{
		for(int j=0; j<4; j++)
		{
			c[i][j] = 0;
			for(int k=0; k<4; k++)
			{
				c[i][j] = c[i][j]+(a[i][k]*b[k][j]);
			}
		}
	}
}

void translate_matrix(float l, float m, float n, float matrix[4][4])
{
	null_matrix(matrix);
	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;
	matrix[3][0] = l;
	matrix[3][1] = m;
	matrix[3][2] = n;
}

Point translate(Point pt, float l, float m, float n)
{
	float a[1][4];
	float b[4][4];
	float c[1][4];
	init_matrix(pt, a);
	null_matrix(b);
	translate_matrix(l, m, n, b);
	mult_matrix(a, b, c);
	Point q = make_point(c[0][0]/c[0][3],c[0][1]/c[0][3],c[0][2]/c[0][3]);
	return q;
}

void scale_matrix(float sx, float sy, float sz, float matrix[4][4])
{
	null_matrix(matrix);
	matrix[0][0] = sx;
	matrix[1][1] = sy;
	matrix[2][2] = sz;
	matrix[3][3] = 1;
}

Point scale(Point p, float sx, float sy, float sz)
{
	float a[1][4];
	float b[4][4];
	float c[1][4];
	init_matrix(p, a);
	null_matrix(b);
	scale_matrix(sx, sy, sz, b);
	mult_matrix(a, b, c);
	Point q = make_point(c[0][0]/c[0][3],c[0][1]/c[0][3],c[0][2]/c[0][3]);
	return q;
}

void rotatex_matrix(float theta, float matrix[4][4])
{
	null_matrix(matrix);
	float rad = (PI*theta)/180;
	matrix[0][0] = 1.0;
	matrix[1][1] = cos(rad);
	matrix[1][2] = sin(rad);
	matrix[2][1] = -sin(rad);
	matrix[2][2] = cos(rad);
	matrix[3][3] = 1.0;
}

Point rotatex(Point p, float theta)
{
	float a[1][4];
	float b[4][4];
	float c[1][4];
	init_matrix(p, a);
	null_matrix(b);
	rotatex_matrix(theta, b);
	mult_matrix(a, b, c);
	Point q = make_point(c[0][0]/c[0][3],c[0][1]/c[0][3],c[0][2]/c[0][3]);
	return q;
}

void rotatey_matrix(float phi, float matrix[4][4])
{
	null_matrix(matrix);
	float rad = (PI*phi)/180;
	matrix[0][0] = cos(rad);
	matrix[0][2] = -sin(rad);
	matrix[1][1] = 1;
	matrix[2][1] = sin(rad);
	matrix[2][3] = cos(rad);
	matrix[3][3] = 1.0;
}

Point rotatey(Point p, float phi)
{
	float a[1][4];
	float b[4][4];
	float c[1][4];
	init_matrix(p, a);
	null_matrix(b);
	rotatey_matrix(phi, b);
	mult_matrix(a, b, c);
	Point q = make_point(c[0][0]/c[0][3],c[0][1]/c[0][3],c[0][2]/c[0][3]);
	return q;
}

void rotatez_matrix(float psi, float matrix[4][4])
{
	null_matrix(matrix);
	float rad = (PI*psi)/180;
	matrix[0][0] = cos(rad);
	matrix[0][1] = sin(rad);
	matrix[1][0] = -sin(rad);
	matrix[1][1] = cos(rad);
	matrix[2][2] = 1.0;
	matrix[3][3] = 1.0;
}

Point rotatez(Point p, float psi)
{
	float a[1][4];
	float b[4][4];
	float c[1][4];
	init_matrix(p, a);
	null_matrix(b);
	rotatez_matrix(psi, b);
	mult_matrix(a, b, c);
	Point q = make_point(c[0][0]/c[0][3],c[0][1]/c[0][3],c[0][2]/c[0][3]);
	return q;
}

//for shear about yz plane fill b, c; xz plane fill d,f; xy plane fill g, h
void shear_matrix(float b, float c, float d, float f, float g, float h, float matrix[4][4])
{
	null_matrix(matrix);
	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;

	matrix[0][1] = b;
	matrix[0][2] = c;
	matrix[1][0] = d;
	matrix[1][2] = f;
	matrix[2][0] = g;
	matrix[2][2] = h;
}

Point shear(Point p, float b, float c, float d, float e, float g, float h)
{
	float a[1][4];
	float bm[4][4];
	float cm[1][4];
	init_matrix(p, a);
	null_matrix(bm);
	shear_matrix(b, c, d, e, g, h, bm);
	mult_matrix(a, bm, cm);
	Point q = make_point(cm[0][0]/cm[0][3],cm[0][1]/cm[0][3],cm[0][2]/cm[0][3]);
	return q;
}

