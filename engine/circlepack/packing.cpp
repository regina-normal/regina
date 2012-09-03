#include "regina/dim2/dim2triangulation.h"
#include "regina/dim2/dim2triangle.h"
#include "regina/dim2/dim2vertex.h"
#include "regina/dim2/dim2edge.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <map>
#include "packing.h"
#include "flower.h"
#include "colour.h"
#include <cstdlib>


#define PI_F 3.14159265358979

namespace regina {

circlePacking::circlePacking(){};

circlePacking::~circlePacking()
{
 // run through the meadow and delete all the flowers
 for (std::map<Dim2Vertex*, CPflower*>::iterator i=meadow.begin();
      i!=meadow.end(); i++)
  delete (i->second);
}

void circlePacking::populate(const Dim2Triangulation* triRef)
{
	meadow.clear();
	label.clear();
	tri = triRef;
	haveLabel=false;
	size = tri->getNumberOfVertices();
	CPflower* tempFlower;
		
	for(Dim2Triangulation::VertexIterator it=tri->getVertices().begin();
		it!=tri->getVertices().end();it++)
	{
		tempFlower = new CPflower();
		tempFlower->giveContext(this);
		tempFlower->setHub(*it);
		meadow.insert(std::pair<Dim2Vertex*,CPflower*>(*it,tempFlower));
	}
	dehydration = tri->isoSig();
}

std::map<Dim2Vertex*,long double> circlePacking::getLabel()
{
	if(!haveLabel)
	{
		label.clear();
		for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); it!=tri->getVertices().end(); it++){
			label.insert(std::pair<Dim2Vertex*,long double>(*it,meadow[*it]->getRadius()));
			}
		haveLabel=true;
	}
	return label;
}


long double circlePacking::fullRelaxation(long double epsilon, long double delta)//, bool code)
{
	if(!haveLabel) getLabel();
	if(size<4)
		return 0;
//	setGeometry(code);
	long double c = epsilon+1;
	long double lambda = -1;
	bool flag = false;
	do{
		
		long double c_0 = c;
		long double lambda_0 = lambda;
		bool flag_0 = flag;
		std::map<Dim2Vertex*,long double> label_0 = label;

		c = relaxPacking(c);

		c = sqrt(c); lambda = c/c_0; flag=true;
		
//		if(lambda<1 and flag_0==true)
//			label = superAcceleration(&c,&lambda,lambda_0,label_0,epsilon,delta,&flag);

	}while ((c < epsilon+1 and c > 1) or (c > 1-epsilon and c < 1));

	return c;
}
/*
std::vector<long double> packing::superAcceleration(long double* c,long double* lambda,long double lambda_0, std::vector<long double> label_0, long double epsilon, long double delta, bool* flag)
{
	if(!c or !lambda or !flag)
	{	
		std::cout << "blah\n";
		exit(1);
	}
	std::vector<long double> newLabel;
	std::vector<long double> starMaker;
	//(i)
	*c = (*lambda) * (*c);
	//(ii)
	if(abs(*lambda - lambda_0))
		*lambda = *lambda/(1- *lambda);
	//(iii)
	for(long i=0;i<size;++i)
	{
		long double temp = (label[i]/(label_0[i]-label[i]))-epsilon;
//		if(temp<1 or geometry)
		starMaker.push_back(temp);
	}
	long double lambda_star = *max_element(starMaker.begin(),starMaker.end());

	//(iv)
	*lambda = std::min(*lambda,lambda_star);

	//(v)
	for(long i=0;i<size;++i)
		newLabel.push_back(label[i] + *lambda * (label[i]-label_0[i]));

	//(vi)
	*flag = false;
	return newLabel;
}
*/
long double circlePacking::relaxPacking(long double c)
{
	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); it!=tri->getVertices().end(); it++)
		if(not (*it)->isBoundary())	
			c+=pow(meadow[*it]->relax() - (2*PI_F),2);
	haveLabel=false;
	return c;
}

} // namespace regina
