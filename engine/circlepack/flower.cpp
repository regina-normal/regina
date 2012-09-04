#include "dim2/dim2triangulation.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2vertex.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include "packing.h"
#include "flower.h"
#include "extra.h"

#define INITIAL_RADIUS 0.5
#define PI_F 3.14159265358979
#define TARGET_SUM 6.283185307

namespace regina {

CPflower::CPflower() 
{
	radius = INITIAL_RADIUS;
	targetSum = TARGET_SUM;
	angleSum = 0;
	size = 0;
	hub = 0;
	haveAngleSum = false;
};

void CPflower::computeAngleSum()
{
	angleSum=0;
	long double u = radius;
	Dim2Triangle* tri;
	int index;
	for(std::deque<Dim2VertexEmbedding>::const_iterator it=hub->getEmbeddings().begin();
		it!=hub->getEmbeddings().end();it++)
	{
		tri = (*it).getTriangle();
		index = (*it).getVertex();
		long double v = packRef->getFlowerAt(tri->getVertex(mod(index+tri->orientation(),3)))->getRadius();
		long double w = packRef->getFlowerAt(tri->getVertex(mod(index-tri->orientation(),3)))->getRadius();
		angleSum += interiorAngle(u,v,w);
	}
	haveAngleSum=true;
}
/*
long double CPflower::interiorAngle(long double u, long double v, long double w)
{
	if(packRef->getGeometry())
		return acos((pow((u+w),2)+pow((u+v),2)-pow((v+w),2))/(2*(u+w)*(u+v)));
	else
		return 2*asin(sqrt(v*((1-u)/(1-(v*u)))*((1-w)/(1-(v*w)))));

}
*/

long double CPflower::relax()
{	
//	if(packRef->getGeometry())
		return euclideanRelaxation();
//	else
//		return hyperbolicRelaxation();
}

long double CPflower::euclideanRelaxation()
{
	computeAngleSum();

	long double beta=sin(angleSum/(2*size));				
	long double delta=sin(PI_F/size);					
	long double v_hat = (beta/(1-beta))*radius;				
	long double u = ((1-delta)/delta)*v_hat;				
	radius = u;

	computeAngleSum();
	return angleSum;
}
/*
long double CPflower::hyperbolicRelaxation()
{
 if(free)
 {
  long double beta=sin(angleSum/(2*size));
  long double delta=sin(PI_F/size);
  long double v_hat = (beta-sqrt(radius))/((beta*radius)-sqrt(radius));
  if(v_hat<0)
   v_hat=0;
  long double t = (2*delta)/(sqrt(pow((1-v_hat),2)+(4*v_hat*pow(delta,2)))+1-v_hat);
  long double u = pow(t,2);
  radius = u;
 }
 computeAngleSum();
 return angleSum;
}
*/
} // namespace regina
