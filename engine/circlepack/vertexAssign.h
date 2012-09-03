
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file circlepack/vertexAssign.h
 *  \brief Given a packed disc, these routines find Cartesian coordinates
 *         for the packing. 
 */

#ifndef __VERTEXASSIGN_H
#ifndef __DOXYGEN
#define __VERTEXASSIGN_H
#endif

//want rescale to 450x700
double SCALE = 1;

namespace regina{

inline bool allCoordsAssigned(std::map<Dim2Vertex*,std::pair<double,double> >* 
    coords, Dim2Component* tri)
{
	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); 
        it!=tri->getVertices().end(); it++) if(coords->count(*it) == 0)
    		return false;
	return true;
}

inline std::map<Dim2Vertex*,std::pair<double,double> >
assSurrCoord(Dim2Vertex* vert, std::map<Dim2Vertex*,
        std::pair<double,double> > coords, 
        std::map<Dim2Vertex*,long double> label)
{
	double x = coords[vert].first; 							
	double y = coords[vert].second;							

	for(std::deque<Dim2VertexEmbedding>::const_iterator it = 
        vert->getEmbeddings().begin();
		it!=vert->getEmbeddings().end(); it++){
		Dim2Triangle* tri;
		int index;
		Dim2Vertex* vert_0;
		Dim2Vertex* vert_1;
		double x_0, y_0, x_1, y_1, v0r, v1r, theta_0, theta_1;

		tri = (*it).getTriangle();
		index = (*it).getVertex();
		vert_0 = tri->getVertex(mod(index+tri->orientation(),3));
		vert_1 = tri->getVertex(mod(index-tri->orientation(),3));

		if(coords.find(vert_0)!=coords.end() and 
           coords.find(vert_1)==coords.end())
		{
			x_0 = coords[vert_0].first;
			y_0 = coords[vert_0].second;
			theta_0=atan2(y_0-y,x_0-x);
			v0r = label[vert_0];
			v1r = label[vert_1];
			theta_1=interiorAngle(label[vert],v0r,v1r);
			x_1 = SCALE*(label[vert]+v1r)*cos(theta_0 + theta_1) + x;
			y_1 = SCALE*(label[vert]+v1r)*sin(theta_0 + theta_1) + y;
			coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> >
            (vert_1,std::pair<double,double>(x_1,y_1)));
		}
	}
	return coords;
}

inline std::map<Dim2Vertex*,std::pair<double,double> >
 reScaled(Dim2Component* tri, std::map<Dim2Vertex*,long double> label, 
          std::pair<double,double> origin)
{
	std::map<Dim2Vertex*,std::pair<double,double> > coords;
	Dim2Vertex* temp_0 = tri->getVertex(0);

	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin();
		it!=tri->getVertices().end(); it++) 
		if(temp_0->getNumberOfEmbeddings() < (*it)->getNumberOfEmbeddings() && 
           not (*it)->isBoundary() || temp_0->isBoundary() && not 
                (*it)->isBoundary())
			temp_0=(*it);

	coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> >
            (temp_0,origin));
	if(tri->getNumberOfVertices()==1) return coords;
	
	Dim2Vertex* temp_1 = temp_0;
	for(std::deque<Dim2VertexEmbedding>::const_iterator it = 
        temp_0->getEmbeddings().begin();
		it!=temp_0->getEmbeddings().end(); it++){
		Dim2Triangle* emb = (*it).getTriangle();
		int index = (*it).getVertex();
		if( emb->getVertex( mod(index+emb->orientation(),3) ) !=temp_0)
			{ temp_1=emb->getVertex(mod(index+emb->orientation(),3)); break; }
		if( emb->getVertex( mod(index-emb->orientation(),3) ) !=temp_0)
			{ temp_1=emb->getVertex(mod(index-emb->orientation(),3)); break; }
	}
	double temp_y = origin.second - SCALE*label[temp_0] - SCALE*label[temp_1];
	coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> >(temp_1,
        std::pair<double,double>(origin.first,temp_y)));

	while (!allCoordsAssigned(&coords,tri))
		for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); 
            it!=tri->getVertices().end(); it++)
			if(coords.count(*it) == 1)
	{
				Dim2Vertex* temp = *it;			
				coords = assSurrCoord(temp,coords,label);
	}
	return coords;
}

inline std::pair<double,double>
 init(Dim2Component* tri, std::map<Dim2Vertex*,long double> label)
{
	SCALE = 1;
	std::map<Dim2Vertex*,std::pair<double,double> > coords;
	Dim2Vertex* temp_0 = tri->getVertex(0);

	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin();
		it!=tri->getVertices().end(); it++) 
		if(temp_0->getNumberOfEmbeddings() < (*it)->getNumberOfEmbeddings() && 
           not (*it)->isBoundary() || temp_0->isBoundary() && not 
                (*it)->isBoundary())
			temp_0=(*it);

	coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> >
        (temp_0,std::pair<double,double>(270,410)));
	if(tri->getNumberOfVertices()==1) return std::pair<double,double>(0,0);
	
	Dim2Vertex* temp_1 = temp_0;
	for(std::deque<Dim2VertexEmbedding>::const_iterator it = 
            temp_0->getEmbeddings().begin();
		it!=temp_0->getEmbeddings().end(); it++){
		Dim2Triangle* emb = (*it).getTriangle();
		int index = (*it).getVertex();
		if( emb->getVertex( mod(index+emb->orientation(),3) ) !=temp_0)
			{ temp_1=emb->getVertex(mod(index+emb->orientation(),3)); break; }
		if( emb->getVertex( mod(index-emb->orientation(),3) ) !=temp_0)
			{ temp_1=emb->getVertex(mod(index-emb->orientation(),3)); break; }
	}
	double temp_y = 410 - SCALE*label[temp_0] - SCALE*label[temp_1];
	coords.insert(std::pair<Dim2Vertex*,std::pair<double,double> >
        (temp_1,std::pair<double,double>(270,temp_y)));

	while (!allCoordsAssigned(&coords,tri))
		for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); 
            it!=tri->getVertices().end(); it++)
			if(coords.count(*it) == 1)
	{
				Dim2Vertex* temp = *it;			
				coords = assSurrCoord(temp,coords,label);
	}
//want the picture to have buffers of 56 on each side, 60 top and bottom.
//this makes a rectangular frame of 500*600 in the postscript file (standard 
// 8.5*10 inch paper frame, 72 pts/inch)
	double min_x = 270;
	double max_x = 270;
	double min_y = 410;
	double max_y = 410;
	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); 
        it!=tri->getVertices().end();it++)
	{
		if(min_x>coords[*it].first) min_x = coords[*it].first;
		else if(max_x<coords[*it].first) max_x = coords[*it].first;
		if(min_y>coords[*it].second) min_y = coords[*it].second;
		else if( max_y<coords[*it].second) max_y = coords[*it].second;
	}
	double rsc_x = 450/(max_x - min_x);					
	double rsc_y = 700/(max_y - min_y);					
	double rsc = std::min(rsc_x,rsc_y);
	SCALE = rsc;
	double off_x = 270 + (max_x-270)*SCALE - 556;
	double off_y = 410 + (max_y-410)*SCALE - 660;
	//std::cout << off_x << '\n';
	//std::cout << off_y << '\n';
	return std::pair<double,double>(270 - off_x,410 - off_y);
}


inline std::map<Dim2Vertex*,std::pair<double,double> >
 getCoords(Dim2Component* tri, std::map<Dim2Vertex*,long double> label)
{
	std::pair<double,double> origin = init(tri,label);
	return reScaled(tri,label,origin);
}
}//namespace regina
#endif
