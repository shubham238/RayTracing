#include <bits/stdc++.h>
using namespace std;
#include "Objects.h"
#ifndef __SETUP__H__
#define __SETUP__H__
const int MAXSIZE = 10;
// this class is used to specify everything about the viewing coordinate system
// 
class SetupVCS
{
public:
	Point vrp; //view reference point
	vectors n; //normal vector
	vectors u,v; 
	Point eye; //eye in VCS
	Point ul; //upper left point of window in VCS
	Point lr; //lowe right point of window in VCS
	int MAXROW; //total rows of pixels in window
	int MAXCOL; //total cols

	// sets up the VCS given view reference point and normal & up vector
	void initiateVcs(Point p, vectors up, vectors nt);

	// given a point in VCS defined by this class, returns the corresponding WCS point
	Point vcs_to_wcs(Point vcspt);

	// constructor
	SetupVCS()
	{
		MAXROW = MAXCOL = 0;
	}

	// set eye, ul, ur in window, all are in VCS
	void set_eye_and_window(Point e, Point ul, Point lr, int m, int c);

	// given a cell position in matrix, returns the corresponding coordinate in WCS
	Point map_matrix_to_WCS(int i, int j);

	vector<Point> aliasing_map_matrix_to_WCS(int i, int j);

	void print()
	{
		printf("VRP: "); vrp.print();
		printf("Normal: "); n.print();
		printf("Up: "); u.print();
		printf("V: "); v.print();
		printf("Eye: ");eye.print();
		printf("Upper left: "); ul.print();
		printf("Lower Right: "); lr.print();
		printf("MAXROW = %d\n",MAXROW);
		printf("MAXCOL = %d\n",MAXCOL);
	}

};

void multiply_matrices(float A[][MAXSIZE], float B[][MAXSIZE], float C[][MAXSIZE], int n, int c, int r);

#endif
