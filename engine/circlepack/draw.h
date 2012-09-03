
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

/*! \file circlepack/draw.h
 *  \brief Postscript layout of the circle packing. 
 */

#ifndef __DRAW_H
#ifndef __DOXYGEN
#define __DRAW_H
#endif

#include "colour.h"
#include "facetData.h"

static regina::colour colourValues[] = {
regina::colour(1,0,0),              regina::colour(0,1,0),
regina::colour(0,0,1),              regina::colour(0,1,1),      
regina::colour(1,0,1),              regina::colour(1,1,0),
regina::colour(0.5,0,0),            regina::colour(0,0.5,0),
regina::colour(0,0,0.5),            regina::colour(0,0.5,0.5),  
regina::colour(0.5,0,0.5),          regina::colour(0,0.5,0.5),
regina::colour(0.5,0.5,0.5),        regina::colour(0.25,0,0),
regina::colour(0,0.25,0),           regina::colour(0,0,0.25),
regina::colour(0,0.25,0.25),        regina::colour(0.25,0,0.25),
regina::colour(0,0.25,0.25),        regina::colour(0.25,0.25,0.25),
regina::colour(0.125,0,0),          regina::colour(0,0.125,0),
regina::colour(0,0,0.125),          regina::colour(0,0.125,0.125),
regina::colour(0.125,0,0.125),      regina::colour(0,0.125,0.125),
regina::colour(0.125,0.125,0.125),  regina::colour(0.0625,0,0),     
regina::colour(0,0.0625,0),         regina::colour(0,0,0.0625),
regina::colour(0,0.0625,0.0625),    regina::colour(0.0625,0,0.0625),
regina::colour(0,0.0625,0.0625),    regina::colour(0.0625,0.0625,0.0625),
regina::colour(0.03125,0,0),        regina::colour(0,0.03125,0),
regina::colour(0,0,0.03125),        regina::colour(0,0.03125,0.03125),
regina::colour(0.03125,0,0.03125),  regina::colour(0,0.03125,0.03125),
regina::colour(0.03125,0.03125,0.03125)
};

//begin with a data structure triData. The structure is indexed by Dim2Triangle* tri. for each tri it contains:
//.. int tCol, corresponding to the tet or pent that tri originates from in buildLink()
//.. doubles x_i,y_i, i in[0,2] co-ords of vertices of the triangle

//have another data structure edgeData indexed by d2edge* edg.  for each edge it contains
//.. doubles x_i,y_i, i in[0,1] co-ords of vertices of the edge
//.. int eCol that somehow tells us where this edge is from (face or tet)
//.. bool bdry that tells us whether or not edge is a boundary edge
//.. int oCol that tells us which (tet or pent) the triangle edge was formerly attached to is from 

//one last data struct indexed by d2vert:
//.. doubles x,y cords of vert
//.. int vCol, corresponding to the edge or face that this vert originates from in buildLink()

//first, there are some functions missing from dim2component class.  They are simple enough
/*std::vector<Dim2Triangle*> getTriangles(Dim2Component* comp){
	std::vector<Dim2Triangle*> tri;
	for(unsigned long i=0;i!=comp->getNumberOfTriangles();i++)
		tri.push_back(comp->getTriangle(i));
	return tri;
}

std::vector<Dim2Edge*> getEdges(Dim2Component* comp){
	std::vector<Dim2Edge*> edg;
	for(unsigned long i=0;i!=comp->getNumberOfEdges();i++)
		edg.push_back(comp->getEdge(i));
	return edg;
}

std::vector<Dim2Vertex*> getVertices(Dim2Component* comp){
	std::vector<Dim2Vertex*> vrt;
	for(unsigned long i=0;i!=comp->getNumberOfVertices();i++)
		vrt.push_back(comp->getVertex(i));
	return vrt;
}*/

namespace regina{

//we want to draw the trianglulation of one component of our sphere:
inline void triangles(std::ostream& out, Dim2Component* tri, std::map<Dim2Edge*, edgeData> edges_){
	for(Dim2Triangulation::EdgeIterator it = tri->getEdges().begin(); it!=tri->getEdges().end(); it++){
		out<< "newpath\n"<<
		edges_[*it].x_0 << " " << edges_[*it].y_0 << " moveto\n" <<
		edges_[*it].x_1 << " " << edges_[*it].y_1 << " lineto\n" <<
		"0.5 setlinewidth\n0 0 0 setrgbcolor\nstroke\n";
	}
}

//we want to colour the trianglulation of one component of our sphere:
inline void triColour(std::ostream& out, Dim2Component* tri, std::map<Dim2Triangle*,triData> tri_, std::map<Dim2Vertex*,vertData> verts_, std::map<Dim2Edge*,edgeData> edges_){
	out << "%!\n";
	for(Dim2Triangulation::TriangleIterator it = tri->getTriangles().begin(); it!=tri->getTriangles().end(); it++){
		out<< "newpath\n"<<
		tri_[*it].x_0 << " " << tri_[*it].y_0 << " moveto\n" <<
		tri_[*it].x_1 << " " << tri_[*it].y_1 << " lineto\n" <<
		tri_[*it].x_2 << " " << tri_[*it].y_2 << " lineto\nclosepath\n";
		colourValues[(tri_[*it].tCol)].printForPS(out);
		out << "setrgbcolor\nfill\n";
	}
	//recall that we split a sphere into two discs.  we want to know what our disc used to be attached to.
	double O_x, O_y; O_x=0; O_y=0;
	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); it!=tri->getVertices().end(); it++){
		O_x+=verts_[*it].x; O_y+=verts_[*it].y;}
	O_x=O_x/tri->getNumberOfVertices(); O_y=O_y/tri->getNumberOfVertices();
	
	for(Dim2Triangulation::TriangleIterator it = tri->getTriangles().begin(); it!=tri->getTriangles().end(); it++)
	for(int i=0;i<3; i++) if((*it)->getEdge(i)->isBoundary()){
		out << O_x + 1000*(edges_[(*it)->getEdge(i)].x_0 - O_x) << " "
		    << O_y + 1000*(edges_[(*it)->getEdge(i)].y_0 - O_y) << " moveto\n"
		    << edges_[(*it)->getEdge(i)].x_0 << " " << edges_[(*it)->getEdge(i)].y_0 << " lineto\n"
		    << edges_[(*it)->getEdge(i)].x_1 << " " << edges_[(*it)->getEdge(i)].y_1 << " lineto\n"
		    << O_x + 1000*(edges_[(*it)->getEdge(i)].x_1 - O_x) << " "
		    << O_y + 1000*(edges_[(*it)->getEdge(i)].y_1 - O_y) << " lineto\nclosepath\n";
		
		colourValues[tri_[*it].adj[i]].printForPS(out);
			
		out << " setrgbcolor\nfill\n";
	}
	for(Dim2Triangulation::VertexIterator it = tri->getVertices().begin(); it!=tri->getVertices().end(); it++)
	if((*it)->isBoundary()){
		out << verts_[*it].x << " " << verts_[*it].y << " moveto\n"
		    << O_x + 1000*(verts_[*it].x - O_x) << " "
		    << O_y + 1000*(verts_[*it].y - O_y) << " lineto\n"
		    << "1 1 1 setrgbcolor\n stroke\n";
	}
	triangles(out,tri,edges_);
	out << "\n\n %/end \n\n";
}
/*
//we want to draw the dual decomposition
inline void dual(std::ostream& out, Dim2Component* tri, std::map<Dim2Triangle*,triData> tri_) 
{
	for(Dim2Triangulation::VertexIterator it=tri->getVertices().begin();
		it!=tri->getVertices().end();it++)
	{
		bool flag=true;
		out<<"newpath\n";
		for(std::deque<Dim2VertexEmbedding>::const_iterator that=(*it)->getEmbeddings().begin();
			that!=(*it)->getEmbeddings().end();that++)
		{
			out<<
		((tri_[(*that).getTriangle()].x_0) + (tri_[(*that).getTriangle()].x_1) + (tri_[(*that).getTriangle()].x_2))/3 <<" "<<
		((tri_[(*that).getTriangle()].y_0) + (tri_[(*that).getTriangle()].y_1) + (tri_[(*that).getTriangle()].y_2))/3 <<" ";

			if(flag)
				out<<"moveto\n";
			else
				out<<"lineto\n";
			flag=false;
		}
		out<<"2 setlinewidth\n0 0 0 setrgbcolor\nstroke\n";
	}
}

//we want to draw the dual decomposition this time with colour!
inline void dualColour(std::ostream& out, Dim2Component* tri, std::map<Dim2Triangle*,triData> tri_, std::map<Dim2Vertex*,vertData> verts_) 
{
    for(Dim2Triangulation::VertexIterator it=tri->getVertices().begin();
        it!=tri->getVertices().end();it++)
        {
         bool flag=true;
         out<<"newpath\n";
         for(std::deque<Dim2VertexEmbedding>::const_iterator that=(*it)->getEmbeddings().begin();
             that!=(*it)->getEmbeddings().end();that++)
             {
              out<<
               (tri_[(*that).getTriangle()].x_0+tri_[(*that).getTriangle()].x_1+tri_[(*that).getTriangle()].x_2)/3 <<" "<<
               (tri_[(*that).getTriangle()].y_0+tri_[(*that).getTriangle()].y_1+tri_[(*that).getTriangle()].y_2)/3 <<" ";
              if(flag)
                 out<<"moveto\n";
              else
                 out<<"lineto\n";
              flag=false;
             }
         out<<"closepath\n";
         colourValues[verts_[*it].vCol].printForPS(out);
         out<<" setrgbcolor\nfill\n";
        }
}*/
}//namespace regina
#endif
