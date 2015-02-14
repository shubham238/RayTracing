#include <bits/stdc++.h>
#include "Setup.h"
using namespace std;

void SetupVCS::initiateVcs(Point p, vectors up, vectors nt)
{
	// Point p = make_point(vrpx, vrpy, vrp/z);
	vrp = p;

	// vectors up = vectors(upx, upy, upz);
	// printf("up.x = %f, u.y = %f, u.z = %f\n", up.x,up.y,up.z);
	// vectors nt = vectors(nx, ny, nz);
	// printf("nt.x = %f, u.y = %f, u.z = %f\n", nt.x,nt.y,nt.z);

	vectors corr_up = subvectors(up, multscalar(dotproduct(up, nt),up));
	// printf("corr_up.x = %f, corr_up.y = %f, corr_up.z = %f\n", corr_up.x,corr_up.y,corr_up.z);
	vectors vt = multscalar((1/(corr_up.mod(corr_up))), corr_up);

	vectors ut = crossproduct (vt, nt);

	u = normalize(ut);
	v = normalize(vt);
	n = normalize(nt);
}

Point SetupVCS::vcs_to_wcs(Point vcspt)
{
	return vcspt;
	float wcsmatrix[1][MAXSIZE];
	float vcsmatrix[1][MAXSIZE];
	float axes[3][MAXSIZE];

	vcsmatrix[0][0] = vcspt.x;
	vcsmatrix[0][1] = vcspt.y;
	vcsmatrix[0][2] = vcspt.z;

	axes[0][0] = u.x;
	axes[0][1] = u.y;
	axes[0][2] = u.z;
	axes[1][0] = v.x;
	axes[1][1] = v.y;
	axes[1][2] = v.z;
	axes[2][0] = n.x;
	axes[2][1] = n.y;
	axes[2][2] = n.z;

	multiply_matrices(vcsmatrix, axes, wcsmatrix, 1, 3, 3);

	float r = pow(vrp.x,2)+pow(vrp.y,2)+pow(vrp.z,2);

	for(int i=0; i<3; i++)
	{
		wcsmatrix[0][i] = wcsmatrix[0][i] + r;
	}

	Point wcspt = make_point(wcsmatrix[0][0],wcsmatrix[0][1],wcsmatrix[0][2]);
	return wcspt;
}

void multiply_matrices(float A[][MAXSIZE], float B[][MAXSIZE], float C[][MAXSIZE], int n, int c, int r)
{
    f(i,n)
    {
        f(j,r)
        {
            float temp=0;
            f(k,c) temp+=A[i][k]*B[k][j];
            C[i][j] = temp;
        }
    }
}

void SetupVCS::set_eye_and_window(Point e, Point ul, Point lr, int m, int c)
{
	eye = e;
	this->ul = ul;
	this->lr = lr;
	MAXROW = m;
	MAXCOL = c;
}

Point SetupVCS::map_matrix_to_WCS(int i__, int j__)
{
	float i = i__, j = j__;
	float wr, wb, wt, wl, du, dv;
	wr = lr.x;
	wb = lr.y;
	wl = ul.x;
	wt = ul.y;
	du = (wr-wl)/MAXCOL;
	dv = (wt-wb)/MAXROW;
	Point vcs_map = make_point(wl+j*du, wb+i*dv,0);
//	Point vcs_map = make_point(lr.x+i*du, ul.y-j*dv,0);
	// printf("Returning this from map: ");
	// vcs_map.print();
	return vcs_to_wcs(vcs_map);
}

vector<Point> SetupVCS::aliasing_map_matrix_to_WCS(int i__, int j__)
{
	float i = i__, j = j__;
	float wr, wb, wt, wl, du, dv;
	wr = lr.x;
	wb = lr.y;
	wl = ul.x;
	wt = ul.y;
	du = (wr-wl)/MAXCOL;
	dv = (wt-wb)/MAXROW;
	vector<Point> ans;
	ans.push_back(vcs_to_wcs(make_point(wl+j*du+(du/4), wb+i*dv+(dv/4), 0)));
	ans.push_back(vcs_to_wcs(make_point(wl+j*du+(du/4), wb+i*dv+(3*dv/4), 0)));
	ans.push_back(vcs_to_wcs(make_point(wl+j*du+(3*du/4), wb+i*dv+(dv/4), 0)));
	ans.push_back(vcs_to_wcs(make_point(wl+j*du+(3*du/4), wb+i*dv+(3*dv/4),0)));
//	Point vcs_map = make_point(lr.x+i*du, ul.y-j*dv,0);
	// printf("Returning this from map: ");
	// vcs_map.print();
	return ans;
}
