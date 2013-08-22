
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

/*! \file circlepack/packing.h
 *  \brief Generates circle packings of discs at present. 
 */

#ifndef __PACKING_H
#ifndef __DOXYGEN
#define __PACKING_H
#endif

#include <map>

namespace regina{

class CPflower;
class Dim4Vertex;
class Dim4Edge;
class NVertex;
class colour;


class REGINA_API circlePacking {
private:
	std::map<Dim2Vertex*,CPflower*> meadow;
	const Dim2Triangulation* tri;
	long size;
//	bool geometry;//true = euclidean, false = hyperbolic

	std::string dehydration;
	bool haveLabel;

	std::map<Dim2Vertex*,long double> label;

//	inline void setGeometry(bool code){ geometry=code; }

	long double relaxPacking(long double errorEstimate);

	void populate(const Dim2Triangulation* tri);

	std::vector<long double> superAcceleration(long double *c,
           long double *lambda,long double lambda_0,    
      std::vector<long double> label_0, long double epsilon, 
           long double delta, bool * flag);

public:
	circlePacking();
    ~circlePacking();
	inline circlePacking(const Dim2Triangulation* tri){ populate(tri); };

	inline CPflower* getFlowerAt(Dim2Vertex* want){ return meadow[want]; }
	inline std::map<Dim2Vertex*,CPflower*> getMeadow(){ return meadow; }
	std::map<Dim2Vertex*,long double> getLabel();
//	inline bool getGeometry(){ return geometry; }

	long double fullRelaxation(long double epsilon, long double delta);
    //, bool code);
};

} // namespace regina
#endif
