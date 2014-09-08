
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  responsible for the stuff below: Jonathan                             *
 *                                                                        *
 **************************************************************************
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

/*! \file morse/nmorsefunction.h
 *  \brief Basic support for Forman's discrete Morse theory.
 */




#ifndef __NMORSEFUNCTION_H
#ifndef __DOXYGEN
#define __NMORSEFUNCTION_H
#endif

//#include "regina-config.h" // For EXCLUDE_SNAPPEA
#include "regina-core.h"
#include "shareableobject.h"

namespace regina {


/**
 * \weakgroup triangulation
 * @{
 */

#ifndef EXCLUDE_MORSE


/**
 * Provides basic functionality to use the discrete Morse theory framework
 * as defined by Forman.
 *
 * An object of this class represents a Morse function which is basically
 * and ordering on the faces of a triangulation following certain conditions
 * as explained in more detail in 
 *
 * Forman, Robin. Morse Theory for Cell Complexes. Adv. in Math. 134, 90-145 (1995)
 *
 * \testpart
 */
class REGINA_API NMorseFunction : public ShareableObject {
    public:

        /**
         * Describes which algorithm was used to compute
         * this Morse function.
         *
         * This is important since some algorithms ensure certain
         * properties of a Morse function (e.g. a single critical face
         * of maximal index), while others may to do so.
         */
        enum MorseAlgFlags {
            /**< Collapsing approach choosing the lexicographically 
                 minimal face in each step. This is the default method. */
            DMT_BENEDETTILUTZLEX = 0x0000,
            /**< Collapsing approach choosing the lexicographically 
                 maximal face in each step. */
            DMT_BENEDETTILUTZREVLEX = 0x0001,
            /**< Collapsing approach from the Exp. Math. article. */
            DMT_BENEDETTILUTZ = 0x0002,
            /**< Collapsing approach choosing a uniformly random 
                 spanning tree in the top-dimensional level. */
            DMT_UNIFORMSPANNINGTREE = 0x0004,
            /**< Engstroem approach using minimal link size arguments
                 and standard Morse functions for cones. */
            DMT_ENGSTROEM = 0x0008
            /**< Collapsing approach: combine any of the DMT_BenedettiLutz*
                 with a random permutation of face labels. All other
                 methods will ignore this flag. */
            DMT_RANDOM = 0x0010,
        };

        /**
         * A combination of flags for algorithms to compute discrete Morse functions.
         *
         */
        typedef regina::Flags<MorseAlgFlags> MorseAlg;
    
        /**
         * Returns the bitwise OR of the two given flags.
         *
         * @param lhs the first flag to combine.
         * @param rhs the second flag to combine.
         * @return the combination of both flags.
         */
        inline MorseAlg operator | (MorseAlgFlags lhs, MorseAlgFlags rhs) {
            return MorseAlg(lhs) | rhs;
        }


//    private:
//        regina::snappea::Triangulation* snappeaData;
//            /**< The triangulation stored in SnapPea's native format. */
//        static bool kernelMessages;
//            /**< Should the SnapPea kernel write diagnostic messages to
//                 standard output? */

    public:
        /**
         * Creates a discrete Morse function for Regina triangulation
         * \a tri using method \a algortihm.

         * See above for a choice of values \a algorithm can have.
         *
         * @param tri the Regina triangulation to compute the Morse 
         * function from.
         * @param algorithm the algorithm type to be used to compute the
         * the Morse function.
         */
        NMorseFunction(const NTriangulation& tri, int algorithm);

        /**
         * Creates a discrete Morse function of the given Regina triangulation
         * \a tri using default method BenedettiLutzLex (see above for details)
         *
         * @param tri the Regina triangulation to compute the Morse 
         * function from.
         */
        NMorseFunction(const NTriangulation& tri);

        /**
         * Destroys this Morse function.
         */
        virtual ~NMorseFunction();


// explanation for JS:
/*

[const] returndatatype[{*|&}] functionname([const] arg1datatype[{*|&}] arg1, [const] arg2datatype[{*|&}] arg1, ...) [const] 

returndatatype: if pointer (bla* or bla&) then add 'const' in front if you don't want this to value to change
argndatatype: same
add 'const' in the end if this function should not change the data of the object

*/


        /**
         * Determines whether this object / discrete Morse function is valid
         *
         * @return \c false if this is a not a Morse function, or \c true
         * if this object is a valid discrete Morse function.
         */
        bool isValid() const;

        /**
         * Returns the type of algorithm used to construct this discrete Morse\
         * Morse function.
         *
         * @return the algorithm type.
         */
        Algorithm algorithm() const;


// TODO: 
// 1. replace NTriangle* by data types (template stuff...)

        NTriangle* criticalFace(int d, int i) const;
        /** 
         *
         *
         * @param d dimension of critical point
         * @param i index of critical point in list of critical \a d -faces
         * @return the \a i -th critical \a d -face
         */

        unsigned long countCriticalFaces(int d) const;
        /** 
         *
         *
         * @param d dimension of critical faces to count
         * @return the  number of critical \a d -faces
         */

        bool isPerfect() const;
        /** 
         *
         *
         * @return \c true if Morse function is perfect, \c false if not
         */

        unsigned long faceIndex(const NTriangle* t) const;
        /** 
         *
         *
         * @param t face of triangulation
         * @return the index of \a f in Morse function
         */

        NTriangle& Face(const int idx) const;
        /** 
         *
         *
         * @param idx position in Morse function
         * @return face at position \a idx in Morse function
         */

        NTriangle& matchedFace(const NEdge* t) const;
        /** 
         *
         *
         * @param t face of dimension i
         * @return (i+1)-face T where \a t is pointing to (e.g., 
         * \a t is an edge of triangle T, and an arrow is pointing 
         * from \a t inside T)
         */

        NEdge& isMatchedBy(const NTriangle* T) const;
        /** 
         * This is meant to be the inverse of matchedFace()
         *
         *
         * @param T face of dimension (i+1)
         * @return i-face t, such that t is pointing to \a T
         * (e.g., t is an edge of triangle \a T, and an arrow 
         * is pointing from t inside \a T)
         */

//        *NChain discreteGradient(const *NChain c) const;
        /** 
         *
         *
         * @param c i-chain to compute the discrete gradient from  
         * @return discret gradiant from \a c
         */

//        *NChain gradientFlow(const *NChain c) const;
        /** 
         *
         *
         * @param c i-chain
         * @return i-chain which is the image of \a c under the 
         * discrete gradient flow
         */



//    private:

};

/*@}*/


#endif // EXCLUDE_MORSE

} // namespace morse

#endif

