
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

/*! \file circlepack/sphereSplit.h
 *  \brief Partitions a triangulated 2-sphere into two discs, and attempts
 *         to ensure the discs have a equal number of triangles. 
 */

#ifndef __SPHERESPLIT_H
#ifndef __DOXYGEN
#define __SPHERESPLIT_H
#endif

namespace regina{

/**
 *  When you try to decompose the 2-sphere triangulation into two discs, we
 * we start by picking one triangle.  This algorithm attempts to make an 
 * intelligent first pick via a local extremization. 
 */
Dim2Triangle* firstTriangle(Dim2Triangulation* tri){
	Dim2Triangle* temp=tri->getTriangle(0);
	for(Dim2Triangulation::TriangleIterator it=tri->getTriangles().begin();
		it!=tri->getTriangles().end();it++)
	{
		if(  (temp)->getVertex(0)==(temp)->getVertex(1) or
		     (temp)->getVertex(0)==(temp)->getVertex(2) or
		     (temp)->getVertex(1)==(temp)->getVertex(2))
			temp=(*it);
		else if(temp->getVertex(0)->getNumberOfEmbeddings()
		+temp->getVertex(1)->getNumberOfEmbeddings()
		+temp->getVertex(2)->getNumberOfEmbeddings() 
		<(*it)->getVertex(0)->getNumberOfEmbeddings()
		+(*it)->getVertex(1)->getNumberOfEmbeddings()
		+(*it)->getVertex(2)->getNumberOfEmbeddings() and
		     (*it)->getVertex(0)!=(*it)->getVertex(1) and
		     (*it)->getVertex(0)!=(*it)->getVertex(2) and
		     (*it)->getVertex(1)!=(*it)->getVertex(2))
			temp=(*it);
	}
	return temp;
}
//this tri has the highest vertex degree sums or is tied for the highest 
// degree sums.  Add it to our collection(it is the first): (f2)

//	std::set<Dim2Triangle*> disc;
//	disc.insert(temp);

//we build a list of boundary edges, ie the edges we will split the sphere on
//simultaneously we want to collect all triangles that share an edge with a 
// triangle in disc that are not already in disc: (f3)
//we also want boundary vertices
//building this collection is linear in the size of tri,
//  so we recalculate at will
//	std::set<Dim2Vertex*> bvrt;
//	std::set<Dim2Edge*> bdry;
//	std::set<Dim2Triangle*> adj;

void calcBoundary(std::set<Dim2Triangle*>* disc,std::set<Dim2Triangle*>* adj,
                  std::set<Dim2Vertex*>* bvrt,std::set<Dim2Edge*>* bdry){
	(*adj).clear();(*bvrt).clear();(*bdry).clear();
	for(std::set<Dim2Triangle*>::iterator it=(*disc).begin();it!=(*disc).end();
        it++)
		for(int i=0;i<3;i++) 
			if((*disc).find((*it)->adjacentTriangle(i))==(*disc).end()){
				(*bdry).insert((*it)->getEdge(i));
				(*adj).insert((*it)->adjacentTriangle(i));
				(*bvrt).insert((*it)->getEdge(i)->getVertex(0));
				(*bvrt).insert((*it)->getEdge(i)->getVertex(1));
				}
}

//Next, check each of these for the property: (f4)
//  >= 2 edges in bdry.  we want to add these triangles more than triangles 
//  with exactly one edge in bdry.  moreover, if we find any
// triangles satisfying this property, we restart process in the hopes of 
//  obtaining more triangles with this property.
/**
 *  Sub-routine for splitting sphere triangulation into two discs.  This
 *  attempts to find a triangle to add to the disc we're building, moreover
 *  it looks for triangles that share two edges with the already-existing
 *  disc. 
 */
bool addTwo(std::set<Dim2Triangle*>* disc, std::set<Dim2Triangle*>* adj, 
            std::set<Dim2Edge*>* bdry,int breaker){
	bool flag = false;
	for(std::set<Dim2Triangle*>::iterator it=(*adj).begin(); it!=(*adj).end();it++)
	{
		int e=0;
		for(int i=0;i!=3;i++) if((*bdry).find((*it)->getEdge(i))!=(*bdry).end())
			e++;
		if(e>1){
			(*disc).insert(*it);
			if((*disc).size()>=breaker) return true;
            //gotta check if we can quit whenever we add a tri to disc.
			flag=true;
		}
	}
	return flag;
}

//Next add all of the triangles with:
// exactly 1 edge in bdry
// exactly 2 vert in bdry
//Whenever we add a triangle in this way, we want to see if any more desirable 
// triangles have come available
int checkSum(Dim2Triangle* tri, std::set<Dim2Triangle*> disc){
	int sum=0;
	for(int i=0;i!=3;i++)
		for(std::deque<Dim2VertexEmbedding>::const_iterator it = 
            tri->getVertex(i)->getEmbeddings().begin();
			it!=tri->getVertex(i)->getEmbeddings().end(); it++)
			if(std::find(disc.begin(),disc.end(),
               (*it).getTriangle())!=disc.end())
				sum++;
	return sum;	
}


void addOne(std::set<Dim2Triangle*>* disc, std::set<Dim2Triangle*>* adj,
            std::set<Dim2Vertex*>* bvrt,std::set<Dim2Edge*>* bdry){
	std::set<Dim2Triangle*> valid;
	for(std::set<Dim2Triangle*>::iterator it=(*adj).begin(); 
        it!=(*adj).end(); it++)
	{
		int e=0;
		int v=0;
		for(int i=0;i!=3;i++){
			if((*bdry).find((*it)->getEdge(i))!=(*bdry).end())
				e++;
			if((*bvrt).find((*it)->getVertex(i))!=(*bvrt).end() and
				   (*it)->getVertex(0)!=(*it)->getVertex(1) and
				   (*it)->getVertex(0)!=(*it)->getVertex(2) and
				   (*it)->getVertex(1)!=(*it)->getVertex(2))
				v++;
		}
		if(v==2 && e==1){
			valid.insert(*it);
		}
	}
	Dim2Triangle* best = *(valid.begin());
	for(std::set<Dim2Triangle*>::iterator it=valid.begin(); 
        it!=valid.end(); it++)
	{
		if(best == NULL) best = (*it);
		else if (checkSum(best,*disc)<checkSum(*it,*disc)) best = (*it);
	}
	disc->insert(best);
}

/**
 *  I think this routine takes a triangulated 2-sphere and 
 * attempts to partition it into two triangulated discs with
 * equal numbers of triangles.  Comments Sam? 
 */
std::set<Dim2Triangle*> getDisc(Dim2Triangulation* tri) {
	int magicNumber = tri->getNumberOfTriangles()/2;
	std::set<Dim2Triangle*> disc;
	disc.insert(firstTriangle(tri));
	std::set<Dim2Triangle*> adjacencies;
	std::set<Dim2Vertex*> bndryVerts;
	std::set<Dim2Edge*> bndryEdges;
	
	for(;;){
		if(disc.size()>=magicNumber) break;
		else calcBoundary(&disc,&adjacencies,&bndryVerts,&bndryEdges);
		if(not addTwo(&disc,&adjacencies,&bndryEdges,magicNumber)) 
         addOne(&disc,&adjacencies,&bndryVerts,&bndryEdges);		
	}
	
	return disc;
}

bool split(Dim2Triangulation* tri, std::set<Dim2Triangle*> slice) {
	for(std::set<Dim2Triangle*>::iterator it = slice.begin();
        it!=slice.end();it++)
		for(int i=0;i<3;i++)
			if(slice.find((*it)->adjacentTriangle(i))==slice.end())
				(*it)->unjoin(i);
	return(tri->getEulerChar()==2 and tri->getNumberOfComponents()==2);
}

} //namespace regina
#endif
