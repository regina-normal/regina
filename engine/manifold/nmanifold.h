
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file manifold/nmanifold.h
 *  \brief Deals with the underlying 3-manifolds of triangulations.
 */

#ifndef __NMANIFOLD_H
#ifndef __DOXYGEN
#define __NMANIFOLD_H
#endif

#include "shareableobject.h"

namespace regina {

class NAbelianGroup;
class NTriangulation;

/**
 * \addtogroup manifold Standard 3-Manifolds
 * Standard 3-manifolds whose structures are well-understood.
 * @{
 */

/**
 * Represents a particular 3-manifold.  The triangulation of this
 * 3-manifold that may be in use is not of interest.
 *
 * Subclasses corresponding to the different types of 3-manifold must
 * override at least the virtual functions writeName() and writeTeXName().
 * They do not need to override writeTextShort() or writeTextLong()
 * since these routines are properly implemented in the base class
 * NManifold.
 *
 * \testpart
 */
class NManifold : public ShareableObject {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~NManifold();

        /**
         * Returns the common name of this 3-manifold as a
         * human-readable string.
         *
         * @return the common name of this 3-manifold.
         */
        std::string getName() const;
        /**
         * Returns the common name of this 3-manifold in TeX format.
         * No leading or trailing dollar signs will be included.
         *
         * \warning The behaviour of this routine has changed as of
         * Regina 4.3; in earlier versions, leading and trailing dollar
         * signs were provided.
         *
         * @return the common name of this 3-manifold in TeX format.
         */
        std::string getTeXName() const;
        /**
         * Returns details of the structure of this 3-manifold that
         * might not be evident from its common name.  For instance, for
         * an orbit space S^3/G this routine might return the full
         * Seifert structure.
         *
         * This routine may return the empty string if no additional
         * details are deemed necessary.
         *
         * @return a string describing additional structural details.
         */
        std::string getStructure() const;
        /**
         * Returns a triangulation of this 3-manifold, if such a
         * construction has been implemented.  If no construction routine
         * has yet been implemented for this 3-manifold (for instance,
         * if this 3-manifold is a Seifert fibred space with
         * sufficiently many exceptional fibres) then this routine will
         * return 0.
         *
         * The details of which 3-manifolds have construction routines
         * can be found in the notes for the corresponding subclasses of
         * NManifold.  The default implemention of this routine returns 0.
         *
         * @return a triangulation of this 3-manifold, or 0 if the
         * appropriate construction routine has not yet been implemented.
         */
        virtual NTriangulation* construct() const;
        /**
         * Returns the first homology group of this 3-manifold, if such
         * a routine has been implemented.  If the calculation of
         * homology has not yet been implemented for this 3-manifold
         * then this routine will return 0.
         *
         * The details of which 3-manifolds have homology calculation routines
         * can be found in the notes for the corresponding subclasses of
         * NManifold.  The default implemention of this routine returns 0.
         *
         * The homology group will be newly allocated and must be destroyed
         * by the caller of this routine.
         *
         * @return the first homology group of this 3-manifold, or 0 if
         * the appropriate calculation routine has not yet been implemented.
         */
        virtual NAbelianGroup* getHomologyH1() const;

        /**
         * Determines in a fairly ad-hoc fashion whether this representation
         * of this 3-manifold is "smaller" than the given representation
         * of the given 3-manifold.
         *
         * The ordering imposed on 3-manifolds is purely aesthetic on
         * the part of the author, and is subject to change in future
         * versions of Regina.
         *
         * The ordering also depends on the particular representation of
         * the 3-manifold that is used.  As an example, different
         * representations of the same Seifert fibred space might well
         * be ordered differently.
         *
         * All that this routine really offers is a well-defined way of
         * ordering 3-manifold representations.
         *
         * \warning Currently this routine is only implemented in full for
         * closed 3-manifolds.  For most classes of bounded 3-manifolds,
         * this routine simply compares the strings returned by getName().
         *
         * @param compare the 3-manifold representation with which this
         * will be compared.
         * @return \c true if and only if this is "smaller" than the
         * given 3-manifold representation.
         */
        bool operator < (const NManifold& compare) const;

        /**
         * Writes the common name of this 3-manifold as a
         * human-readable string to the given output stream.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;
        /**
         * Writes the common name of this 3-manifold in TeX format to
         * the given output stream.  No leading or trailing dollar signs
         * will be included.
         *
         * \warning The behaviour of this routine has changed as of
         * Regina 4.3; in earlier versions, leading and trailing dollar
         * signs were provided.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;
        /**
         * Writes details of the structure of this 3-manifold that
         * might not be evident from its common name to the given output
         * stream.  For instance, for an orbit space S^3/G this routine
         * might write the full Seifert structure.
         *
         * This routine may write nothing if no additional
         * details are deemed necessary.  The default implementation of
         * this routine behaves in this way.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeStructure(std::ostream& out) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for NManifold

inline NManifold::~NManifold() {
}

inline NTriangulation* NManifold::construct() const {
    return 0;
}

inline NAbelianGroup* NManifold::getHomologyH1() const {
    return 0;
}

inline std::ostream& NManifold::writeStructure(std::ostream& out) const {
    return out;
}

inline void NManifold::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void NManifold::writeTextLong(std::ostream& out) const {
    writeName(out);
    std::string details = getStructure();
    if (! details.empty())
        out << " ( " << details << " )";
}

} // namespace regina

#endif

