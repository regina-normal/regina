#include "dim4/dim4triangulation.h"
#include "dim4/dim4edge.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "dim2/dim2component.h"
#include "dim2/dim2triangulation.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2vertex.h"

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "extra.h"
#include "packing.h"
#include "flower.h"
#include "vertexAssign.h"
#include "sphereSplit.h"
#include "draw.h"
#include "circlePack.h"
#include "facetData.h"

#define EPSILON 0.0000000001
#define DELTA 0.0000000001

namespace regina {

std::string circlePackPS(Dim4Triangulation* tri, Dim4Edge* edge)
{
	Dim4Isomorphism* iso( 
         new Dim4Isomorphism( edge->getNumberOfEmbeddings() ) );
	const std::auto_ptr<Dim2Triangulation> toLink(edge->buildLink(iso));
	Dim2Triangulation* link = toLink.get();

	std::map<Dim2Triangle*,triData> isoTri;
	std::map<Dim2Edge*,edgeData> isoEdge;
	std::map<Dim2Vertex*,vertData> isoVert;

	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	{
		triData temp;
		int A = iso->pentImage((*it)->markedIndex());
		temp.tCol=A;

		isoTri.insert(std::pair<Dim2Triangle*,triData>(*it,temp));
	}
	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	for(int i=0;i<3;i++)
	{
		int temp = isoTri[(*it)->adjacentTriangle(i)].tCol;
		isoTri[*it].adj[i] = temp;
	}

	std::set<Dim2Triangle*> disc = getDisc(link);
	if(link->getEulerChar()==2 && link->getNumberOfComponents()==1) 
        split(link,disc);

	circlePacking* pack( new circlePacking(link));

	long double error = pack->fullRelaxation(EPSILON,DELTA);

	std::map<Dim2Vertex*,std::pair<double,double> > coords;
	for(Dim2Triangulation::ComponentIterator it = link->getComponents().begin();
		it!=link->getComponents().end(); it++)
	{
		std::map<Dim2Vertex*,std::pair<double,double> > temp;
		std::map<Dim2Vertex*,long double> compLabel = pack->getLabel();
		for(Dim2Triangulation::VertexIterator that = 
            link->getVertices().begin();
			that!=link->getVertices().end(); that++)
			if(std::find((*it)->getVertices().begin(),
            (*it)->getVertices().end(),*that)==(*it)->getVertices().end())
				compLabel.erase(*that);
		temp = getCoords(*it,compLabel);
		for(Dim2Triangulation::VertexIterator that = 
        (*it)->getVertices().begin(); that != (*it)->getVertices().end(); 
            that++)
			coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> > 
            (*that,temp[*that]));			
	}

	for(Dim2Triangulation::EdgeIterator it = link->getEdges().begin();
		it!=link->getEdges().end(); it++)
	{
		edgeData temp;
		temp.x_0=coords[(*it)->getVertex(0)].first;
		temp.y_0=coords[(*it)->getVertex(0)].second;
		temp.x_1=coords[(*it)->getVertex(1)].first;
		temp.y_1=coords[(*it)->getVertex(1)].second;
		//edge colour code goes here
		isoEdge.insert(std::pair<Dim2Edge*,edgeData>(*it,temp));
	}

	for(Dim2Triangulation::VertexIterator it = link->getVertices().begin();
		it!=link->getVertices().end(); it++)
	{
		vertData temp;
		temp.x=coords[(*it)].first;
		temp.y=coords[(*it)].second;
		//vertex colour code goes here
		isoVert.insert(std::pair<Dim2Vertex*,vertData>(*it,temp));
	}

	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	{
		triData temp;
		isoTri[*it].x_0=coords[(*it)->getVertex(0)].first;
		isoTri[*it].y_0=coords[(*it)->getVertex(0)].second;
		isoTri[*it].x_1=coords[(*it)->getVertex(1)].first;
		isoTri[*it].y_1=coords[(*it)->getVertex(1)].second;
		isoTri[*it].x_2=coords[(*it)->getVertex(2)].first;
		isoTri[*it].y_2=coords[(*it)->getVertex(2)].second;
	}

	std::stringstream ss;
	for(Dim2Triangulation::ComponentIterator it = link->getComponents().begin();
		it!=link->getComponents().end(); it++)
	{
		triColour(ss,*it,isoTri,isoVert,isoEdge);
	}
    delete pack; 
    delete iso; 
	return ss.str();
}


std::string circlePackPS(NTriangulation* tri, NVertex* vertex)
{
	NIsomorphism* iso( new NIsomorphism( vertex->getNumberOfEmbeddings() ) );
	Dim2Triangulation* link( new Dim2Triangulation( *vertex->buildLink(iso) ) );

	std::map<Dim2Triangle*,triData> isoTri;
	std::map<Dim2Edge*,edgeData> isoEdge;
	std::map<Dim2Vertex*,vertData> isoVert;

	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	{
		triData temp;
		int A = iso->tetImage((*it)->markedIndex());
		temp.tCol=A;

		isoTri.insert(std::pair<Dim2Triangle*,triData>(*it,temp));
	}											
	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	for(int i=0;i<3;i++)
	{
		int temp = isoTri[(*it)->adjacentTriangle(i)].tCol;
		isoTri[*it].adj[i] = temp;
	}											
	std::set<Dim2Triangle*> disc = getDisc(link);
	if(link->getEulerChar()==2 && link->getNumberOfComponents()==1) 
        split(link,disc);

	circlePacking* pack( new circlePacking(link));
												
	long double error = pack->fullRelaxation(EPSILON,DELTA);
	std::map<Dim2Vertex*,std::pair<double,double> > coords;
	for(Dim2Triangulation::ComponentIterator it = link->getComponents().begin();
		it!=link->getComponents().end(); it++)
	{
		std::map<Dim2Vertex*,std::pair<double,double> > temp;
		std::map<Dim2Vertex*,long double> compLabel = pack->getLabel();
		for(Dim2Triangulation::VertexIterator 
             that = link->getVertices().begin();
			that!=link->getVertices().end(); that++)
			if(std::find((*it)->getVertices().begin(),
            (*it)->getVertices().end(),*that)==(*it)->getVertices().end())
				compLabel.erase(*that);					
		temp = getCoords(*it,compLabel);
		for(Dim2Triangulation::VertexIterator that = 
            (*it)->getVertices().begin(); that != (*it)->getVertices().end(); 
            that++)
			coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> > 
            (*that,temp[*that]));			
											
	}										
	for(Dim2Triangulation::EdgeIterator it = link->getEdges().begin();
		it!=link->getEdges().end(); it++)
	{
		edgeData temp;
		temp.x_0=coords[(*it)->getVertex(0)].first;
		temp.y_0=coords[(*it)->getVertex(0)].second;
		temp.x_1=coords[(*it)->getVertex(1)].first;
		temp.y_1=coords[(*it)->getVertex(1)].second;
		//edge colour code goes here
		isoEdge.insert(std::pair<Dim2Edge*,edgeData>(*it,temp));
	}
											
	for(Dim2Triangulation::VertexIterator it = link->getVertices().begin();
		it!=link->getVertices().end(); it++)
	{
		vertData temp;
		temp.x=coords[(*it)].first;
		temp.y=coords[(*it)].second;
		//vertex colour code goes here
		isoVert.insert(std::pair<Dim2Vertex*,vertData>(*it,temp));
	}
	for(Dim2Triangulation::TriangleIterator it = link->getTriangles().begin();
		it!=link->getTriangles().end(); it++)
	{
		triData temp;
		isoTri[*it].x_0=coords[(*it)->getVertex(0)].first;
		isoTri[*it].y_0=coords[(*it)->getVertex(0)].second;
		isoTri[*it].x_1=coords[(*it)->getVertex(1)].first;
		isoTri[*it].y_1=coords[(*it)->getVertex(1)].second;
		isoTri[*it].x_2=coords[(*it)->getVertex(2)].first;
		isoTri[*it].y_2=coords[(*it)->getVertex(2)].second;
	}
	std::stringstream ss;
	for(Dim2Triangulation::ComponentIterator it = link->getComponents().begin();
		it!=link->getComponents().end(); it++)
	{
		triColour(ss,*it,isoTri,isoVert,isoEdge);
	}
    delete pack; 
    delete link; 
    delete iso;
	return ss.str();

}

} // namespace regina
