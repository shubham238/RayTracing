#include <iostream>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include "Objects.h"
#include "ObjectReader.h"
#include "Illumination.h"
#include "Setup.h"
using namespace std;

Plane** pl; //array of pointer to planes
int num_planes; //number of planes
Sphere** sp; //array of pointer to spheres
int num_spheres; //number of spheres
Light** lt; // array of pointer to light sources
int num_lights; //number of light sources
Surface** sr; //array of pointer to surfaces
int num_surfaces; //number of surfaces
vectors ambient; //ambient light intensity. It's constant and hence global
SetupVCS* view_set; //Contains the viewing system
vectors** bitplane; //RGB values in a matrix //size is stored in view_set
const float INF = 10000.0; //Infinity
int window_width = 600, window_height = 600; //size of window
float* pixels;
 
// converts the data stored in bitplane array to float array
float* getRGBdata()
{
	float* pixels = new float[3*window_height*window_width];
	int p_ind = 0;
	f(i,window_width)
	// for(int j=window_width-1; j>=0; j--)
	{
		f(j,window_height)
		{
			pixels[p_ind++] = bitplane[i][j].x;
			// printf("%f ",pixels[p_ind-1]);
			pixels[p_ind++] = bitplane[i][j].y;
			// printf("%f ",pixels[p_ind-1]);
			pixels[p_ind++] = bitplane[i][j].z;
			// printf("%f ",pixels[p_ind-1]);
			// printf("\n");
		}
	}
	return pixels;
}

// this will set all the data structures
void readFile(char* filename)
{
	fstream file;
	file.open(filename,ios::in);
	// start reading

	// Sphere reading
	char ch[100]; int n;
	file >> ch;
	file >> n; //number of spheres
	num_spheres = n; //set number of spheres
	sp = readSphere(file,n);

	// Plane reading
	file >> ch;
	file >> n;
	num_planes = n;
	pl = readPlane(file,n);

	// Surface reading
	file >> ch;
	file >> n;
	num_surfaces = n;
	sr = readSurface(file,n);

	// Light source reading
	file >> ch;
	file >> n;
	num_lights = n;
	lt = readLightSources(file,n);
	// end reading

	file >> ch;
	file >> ambient.x >> ambient.y >> ambient.z;
	file.close();
}

// sets the view_set data structure, and makes the viewing co-ordinate system.
// ease of conversion tpo WCS
void set_viewing_system(char* filename)
{
	view_set = new SetupVCS;
	fstream file;
	file.open(filename,ios::in);
	vectors up, nt;
	Point p,eye,ul,lr;
	int r,c;
	file >> p.x >> p.y >> p.z;
	file >> nt.x >> nt.y >> nt.z;
	file >> up.x >> up.y >> up.z;
	file >> eye.x >> eye.y >> eye.z;
	file >> ul.x >> ul.y >> ul.z;
	file >> lr.x >> lr.y >> lr.z;
	file >> r >> c;
	window_width = r, window_height = c;
	view_set->initiateVcs(p,up,nt);
	view_set->set_eye_and_window(eye,ul,lr,r,c);
	// view_set->print();
	view_set->n = vectors(0,0,1);
	view_set->u = vectors(0,1,0);
	view_set->v = vectors(1,0,0);
}

// returns INF if line 'l' does not intersect any object
// else returns the parameter of the point of intersection
float ray_objects_intersection(Line* l, int& index_sphere, int& index_plane)
{
	float t = INF; //INF means infinity

	// check intersection with all spheres
	index_sphere = -1;
	f(i,num_spheres)
	{
		Sphere* s = sp[i];
		float f = s->param_intersection(*l);
		if(f>=0 && f<t)
		{
			index_sphere = i;
			t = f;
		}

	}
	// if index sphere is -1 then none of the spheres were intersected

	index_plane = -1;
	f(i,num_planes)
	{
		Plane* p = pl[i];
		float f = p->param_intersection(*l);
		if(f>=0 && f<t)
		{
			index_plane = i;
			t = f;
		}
	}
	// if(t==INF) printf("No intersection yo!\n");
	return t;
}

// given a line, checks it's intersection with all objects and returns the RGB value at that point
// depth is the recursion depth
// mu is the medium in which light is travelling.
vectors get_pixel_shade(Line* l, int depth, int mu)
{
	if(depth == 2) //base case for relfection/transmission
		return vectors(0,0,0);
	int index_plane = -1, index_sphere = -1;
	float t = ray_objects_intersection(l,index_sphere, index_plane);
	int __a,__b;

	// if t is INF, that means non intersection
	if(t==INF || t<=0)
	{
		// printf("YoAmbient\n");
		return ambient;
	}
	else if(index_plane == -1) //no plane before a sphere
	{//intersection with a sphere YO
		t -=0.1;
		Point p = make_point(l->p.x + t*(l->direction.x),l->p.y + t*(l->direction.y),l->p.z + t*(l->direction.z));
		//  p is the intersection point

		vectors  ret = getAmbientIntensity(sp[index_sphere]->ka,ambient);
		

		
		// this loop checks whether the point is illuminated by any light source! (Phong model!)
		// Phong illumination
		f(i,num_lights)
		{
			Light* src = lt[i];
			Line* new_line = new Line;
			new_line->p = p;
			new_line->direction = difference_vector(src->location,p);
			float isInt = ray_objects_intersection(new_line,__a,__b);
			if(isInt <= 0 || isInt==INF)
			{
				// printf("YoSphere\n");
				ret = addvectors(ret,getDiffuseIntensity(p,sp[index_sphere]->get_normal(p),sp[index_sphere]->kd, *src));
				ret = addvectors(ret,getSpecularIntensity(p,sp[index_sphere]->get_normal(p), sp[index_sphere]->ks, sp[index_sphere]->n_spec, *src, difference_vector(l->p,p)));
			}
//			 else
//			 {
//			 	printf("Sphre Light ray intercepted! ");
//			 	if(__b == -1)
//			 		printf("By Sphere\n");
//			 	else
//			 		printf("By Plane\n");
//			 }
		}

		// Reflection color!
		// Here all we have to do is compute the reflected ray. and pass it to this function, increasing the depth by 1
		float ref_coef = sp[index_sphere]->ktr; //reflectivity of the intersection point.
		
		vectors ref_ray_direction = get_reflected_ray(l->direction, sp[index_sphere]->get_normal(p)); 
		
		Line* reflected_line = new Line;
		reflected_line->p = p; //intersection point
		reflected_line->direction = ref_ray_direction;
		
		vectors ref_shade = get_pixel_shade(reflected_line, depth+1,mu);
		ref_shade = multscalar(ref_coef, ref_shade);
		// ret = multscalar(1-ref_coef, ret);
		ret = addvectors(ret, ref_shade);
		

		// Transmission color! 
		t+=0.2;
		p = make_point(l->p.x + t*(l->direction.x),l->p.y + t*(l->direction.y),l->p.z + t*(l->direction.z));
		
		float trans_coef = sp[index_sphere]->ktg;

		vectors trans_ray_direction = get_transmitted_ray(l->direction, sp[index_sphere]->get_normal(p), mu, sp[index_sphere]->mu);
		Line* trans_line = new Line;
		trans_line->p = p;
		trans_line->direction = trans_ray_direction;

		vectors trans_shade = get_pixel_shade(trans_line, depth+1, sp[index_sphere]->mu);
		trans_shade = multscalar(trans_coef, trans_shade);
		// ret = multscalar(1-trans_coef, ret);
		ret = addvectors(ret, trans_shade);
		return ret;
	}
	else //there is a plane
	{
		// t/=2;
		t-=0.1;
		// printf("Plane intersection!\n");
		Point p = make_point(l->p.x + t*(l->direction.x),l->p.y + t*(l->direction.y),l->p.z + t*(l->direction.z));
		vectors ret = getAmbientIntensity(pl[index_plane]->ka,ambient);
		f(i,num_lights)
		{
			Light* src = lt[i];
			Line* new_line = new Line;
			new_line->p = p;
			new_line->direction = difference_vector(src->location,p);
			float isInt = ray_objects_intersection(new_line,__a,__b);

			if(isInt <= 0 || isInt==INF)
			{
				// printf("YoPlane!\n");
				ret = addvectors(ret,getDiffuseIntensity(p,pl[index_plane]->get_normal(),pl[index_plane]->kd, *src));
				ret = addvectors(ret,getSpecularIntensity(p,pl[index_plane]->get_normal(), pl[index_plane]->ks, pl[index_plane]->n_spec, *src, difference_vector(l->p,p)));
				// printf("Ret..\n");
			}
//			else
//			{
//				printf("Plane Light ray intercepted! %f",isInt);
//				 if(__b == -1)
//					printf("By Sphere\n");
//				else
//					printf("By plane\n");
//			}
		}
		// printf("Plane yo!\n");
		// ret.print();
		// Reflection color!
		// Here all we have to do is compute the reflected ray. and pass it to this function, increasing the depth by 1
		
		float ref_coef = pl[index_plane]->ktr; //reflectivity of the intersection point.
		
		vectors ref_ray_direction = get_reflected_ray(l->direction, pl[index_plane]->get_normal()); 
		
		Line* reflected_line = new Line;
		reflected_line->p = p; //intersection point
		reflected_line->direction = ref_ray_direction;
		
		vectors ref_shade = get_pixel_shade(reflected_line, depth+1,mu);
		ref_shade = multscalar(ref_coef, ref_shade);
		ret = addvectors(ret, ref_shade);
		

		// Transmission color!
		t+=0.2;
		p = make_point(l->p.x + t*(l->direction.x),l->p.y + t*(l->direction.y),l->p.z + t*(l->direction.z));
		
		float trans_coef = pl[index_plane]->ktg;

		vectors trans_ray_direction = get_transmitted_ray(l->direction, pl[index_plane]->get_normal(), mu, pl[index_plane]->mu);
		Line* trans_line = new Line;
		trans_line->p = p;
		trans_line->direction = trans_ray_direction;

		vectors trans_shade = get_pixel_shade(trans_line, depth+1, pl[index_plane]->mu);
		trans_shade = multscalar(trans_coef, trans_shade);
		ret = addvectors(ret, trans_shade); 
		return ret;
	}
}

// here we have the actual ray tracing algorithm
// this must be called only after set_viewing_system
void ray_tracing_procedure()
{
	int r = view_set->MAXROW, c = view_set->MAXCOL;
	
	bitplane = new vectors*[r];
	f(i,r)
		bitplane[i] = new vectors[c];
	// eye is in WCS!!!
	Point eye = view_set->vcs_to_wcs(view_set->eye);

	vector<Point> pts;
	f(i,r)
	{
		f(j,c)
		{
//			p = (view_set->map_matrix_to_WCS(i,j)); //tested (correct)
			pts = (view_set->aliasing_map_matrix_to_WCS(i,j));
			// printf("p = %f, %f, %f\n",p.x,p.y,p.z);
			Line* l = new Line;
			l->p = eye;
			vectors cumm = vectors();
			for(int ct = 0; ct<4; ct++)
			{
				l->direction = difference_vector(pts[ct],eye);
				cumm = addvectors(cumm,get_pixel_shade(l,0,1));
				//bitplane[i][j] = get_pixel_shade(l,0,1);
				//printf("@(%d,%d) : %f %f %f\n",i,j,bitplane[i][j].x,bitplane[i][j].y,bitplane[i][j].z);
			}
			bitplane[i][j] = multscalar(1/4.0,cumm);
		}
	}
}

	
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawPixels(window_width,window_height,GL_RGB,GL_FLOAT,pixels);
  glutSwapBuffers();
}


int main(int argc, char** argv) 
{
	char obj_file[] = "object.txt";
	char vcs_file[] = "viewsystem.txt";
	readFile(obj_file);

//	 f(i,num_spheres) sp[i]->print(), printf("\n");;
//	 f(i,num_planes) pl[i]->print(), printf("\n");;
//	 f(i,num_surfaces) sr[i]->print(),printf("\n");;
//	 f(i,num_lights) lt[i]->print(),printf("\n");
//	 printf("Ambient: ");ambient.print();
	
	set_viewing_system(vcs_file);
	ray_tracing_procedure();
	pixels = getRGBdata();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Illumination Model");

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse_button);
	//glutMotionFunc(mouse_motion);
	//glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutMainLoop();
}
