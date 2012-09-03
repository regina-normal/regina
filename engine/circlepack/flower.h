
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

/*! \file circlepack/flower.h
 *  \brief The key object in a circle packing. 
 */

#ifndef __FLOWER_H
#ifndef __DOXYGEN
#define __FLOWER_H
#endif


namespace regina {

class circlePacking;

/**
 *  This is the primary object used in a circle packing. 
 */
class REGINA_API CPflower {
private:
	circlePacking* packRef;
	Dim2Vertex* hub;
	long size;
	long double radius;
	long double angleSum;
	long double targetSum;
	bool haveAngleSum;

	long double relax();
	long double euclideanRelaxation();
//	long double hyperbolicRelaxation();

	void computeAngleSum();
//	long double interiorAngle(long double u,long double v,long double w);

	inline void giveContext(circlePacking* fromHere){ packRef = fromHere; }
   	inline void setHub(Dim2Vertex* whereItIs){ hub = whereItIs;size = hub->getNumberOfEmbeddings(); }
	
	friend class circlePacking;

public:
	CPflower();

	inline long double getRadius(){ return radius; }
	inline Dim2Vertex* getHub(){ return hub; }
	inline long double getAngleSum(){ if(!haveAngleSum) computeAngleSum(); return angleSum; }
	inline long double getTargetSum(){ return targetSum; }

};


} // namespace regina
#endif
