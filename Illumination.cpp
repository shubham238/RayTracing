#include <bits/stdc++.h>
#include "Illumination.h"
using namespace std;

vectors getDiffuseIntensity(Point p, vectors norm, vectors kd, Light source)
{
	vectors l = normalize(difference_vector(source.location,p)); //l vector
	float costheta = dotproduct(l,normalize(norm)); //cos theta between normal and light direction
	vectors v = multscalar(costheta,source.intensity); //multiply light intensity by cos theta
	vectors ret = multvectors(kd,v); //take corresponding components
	return ret;
}

vectors getAmbientIntensity(vectors ka, vectors amb)
{
	// printf("ka: "); ka.print();
	// printf("amb: "); amb.print();

	vectors a = multvectors(ka,amb);
	// printf("result: "); a.print();
	return a;
}

vectors getSpecularIntensity(Point p, vectors norm, vectors ks, float n_spec, Light source, vectors view_dir)
{
	// printf("Inside specular: \n");
	vectors l = normalize(difference_vector(source.location,p)); //l vector
	// printf("l :");l.print();
	vectors H = normalize(addvectors(l,view_dir)); //(L+V)/|L+V|
	// printf("H: "); H.print();
	float cosalpha = dotproduct(H,normalize(norm)); //replace R.V by N.H
	if(cosalpha <= 0)
		return vectors(0,0,0);
	// printf("cos alpha = %f\n",cosalpha);
	cosalpha = pow(cosalpha,n_spec);
	// printf("cos alpha power n_spec = %f\n",cosalpha);
	vectors v = multscalar(cosalpha,source.intensity); //multiply light intensity by cos aplha
	// printf("v = "); v.print();
	vectors ret = multvectors(ks,v); //take corresponding components
	return ret;
}
