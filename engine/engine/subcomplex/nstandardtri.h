
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nstandardtri.h
 *  \brief Deals with triangulations whose structures are well-understood.
 */

#ifndef __NSTANDARDTRI_H
#ifndef __DOXYGEN
#define __NSTANDARDTRI_H
#endif

#include "shareableobject.h"

namespace regina {

class NAbelianGroup;
class NComponent;
class NManifold;
class NTriangulation;

/**
 * \addtogroup subcomplex Standard Triangulations and Subcomplexes
 * Standard triangulations and subcomplexes of triangulations whose
 * structures are well-understood.
 * @{
 */

/**
 * Describes a triangulation or subcomplex of a triangulation whose structure
 * is well-understood.  An NStandardTriangulation is generally connected
 * with a real triangulation, i.e., an NTriangulation object, which it
 * describes some portion of.
 *
 * In general NStandardTriangulation objects cannot be constructed
 * directly, but are instead created through static identification
 * routines such as
 * NStandardTriangulation::isStandardTriangulation(NTriangulation*).
 *
 * Subclasses corresponding to different families of triangulations do
 * not need to override writeTextShort() since this routine is
 * properly implemented in the base class NStandardTriangulation.
 */
class NStandardTriangulation : public ShareableObject {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~NStandardTriangulation();

        /**
         * Returns the name of this specific triangulation as a
         * human-readable string.
         *
         * @return the name of this triangulation.
         */
        std::string getName() const;
        /**
         * Returns the name of this specific triangulation in TeX
         * format.  Leading and trailing dollar signs will be included.
         *
         * @return the name of this triangulation in TeX format.
         */
        std::string getTeXName() const;
        /**
         * Returns the 3-manifold represented by this triangulation.
         *
         * The 3-manifold will be newly allocated and must be destroyed
         * by the caller of this routine.
         *
         * @return the underlying 3-manifold.
         */
        virtual NManifold* getManifold() const = 0;
        /**
         * Returns the expected first homology group of this triangulation,
         * if such a routine has been implemented.  If the calculation of
         * homology has not yet been implemented for this triangulation
         * then this routine will return 0.
         *
         * This routine does not work by calling
         * NTriangulation::getHomologyH1() on the associated real
         * triangulation.  Instead the homology is calculated directly
         * from the known properties of this standard triangulation.
         *
         * The details of which standard triangulations have homology
         * calculation routines can be found in the notes for the
         * corresponding subclasses of NStandardTriangulation.  The
         * default implementation of this routine returns 0.
         *
         * The homology group will be newly allocated and must be
         * destroyed by the caller of this routine.
         *
         * If this NStandardTriangulation describes an entire NTriangulation
         * (and not just a part thereof) then the results of this routine
         * should be identical to the homology group obtained by calling
         * NTriangulation::getHomologyH1() upon the associated real
         * triangulation.
         *
         * @return the first homology group of this triangulation, or 0 if
         * the appropriate calculation routine has not yet been implemented.
         */
        virtual NAbelianGroup* getHomologyH1() const;

        /**
         * Writes the name of this triangulation as a human-readable
         * string to the given output stream.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;
        /**
         * Writes the name of this triangulation in TeX format
         * to the given output stream.  Leading and trailing dollar
         * signs will be included.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Determines whether the given component represents one of the
         * standard triangulations understood by Regina.  The list of
         * recognised triangulations is expected to grow between
         * releases.
         *
         * If the standard triangulation returned has boundary faces
         * then the given component must have the same corresponding
         * boundary faces, i.e., the component cannot have any further
         * identifications of these boundary faces with each other.
         *
         * @param component the triangulation component under examination.
         * @return the details of the standard triangualation if the
         * given component is recognised, or 0 otherwise.
         */
        static NStandardTriangulation* isStandardTriangulation(
            NComponent* component);
        /**
         * Determines whether the given triangulation represents one of the
         * standard triangulations understood by Regina.  The list of
         * recognised triangulations is expected to grow between
         * releases.
         *
         * If the standard triangulation returned has boundary faces
         * then the given triangulation must have the same corresponding
         * boundary faces, i.e., the triangulation cannot have any further
         * identifications of these boundary faces with each other.
         *
         * @param tri the triangulation under examination.
         * @return the details of the standard triangualation if the
         * given triangulation is recognised, or 0 otherwise.
         */
        static NStandardTriangulation* isStandardTriangulation(
            NTriangulation* tri);
};

/*@}*/

// Inline functions for NStandardTriangulation

inline NStandardTriangulation::~NStandardTriangulation() {
}

inline NAbelianGroup* NStandardTriangulation::getHomologyH1() const {
    return 0;
}

inline void NStandardTriangulation::writeTextShort(std::ostream& out) const {
    writeName(out);
}

} // namespace regina

#endif

