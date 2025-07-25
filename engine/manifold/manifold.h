
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file manifold/manifold.h
 *  \brief Deals with the underlying 3-manifolds of triangulations.
 */

#ifndef __REGINA_MANIFOLD_H
#ifndef __DOXYGEN
#define __REGINA_MANIFOLD_H
#endif

#include "regina-core.h"
#include "algebra/abeliangroup.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"

namespace regina {

class AbelianGroup;

/**
 * \defgroup manifold Standard 3-Manifolds
 * Standard 3-manifolds whose structures are well-understood.
 */

/**
 * Represents a particular 3-manifold, independent of how it might be
 * triangulated.  This is an abstract base class: its subclasses correspond
 * to different families of 3-manifolds.
 *
 * Each subclass:
 *
 * - must override all pure virtual functions (of course);
 *
 * - may optionally override construct(), homology() and/or writeStructure(),
 *   if they are able to provide this functionality;
 *
 * - must _not_ override writeTextShort() or writeTextLong(), since these
 *   routines are not virtual, and are provided directly by the Manifold base
 *   class;
 *
 * - must provide value semantics (including at least a copy constructor and
 *   assignment operator);
 *
 * - must provide comparison operators (== and !=);
 *
 * - must provide member and global swap functions, for consistency across all
 *   Manifold subclasses.
 *
 * \ingroup manifold
 */
class Manifold : public Output<Manifold> {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~Manifold() = default;

        /**
         * Returns the common name of this 3-manifold as a
         * human-readable string.
         *
         * \return the common name of this 3-manifold.
         */
        std::string name() const;
        /**
         * Returns the common name of this 3-manifold in TeX format.
         * No leading or trailing dollar signs will be included.
         *
         * \warning The behaviour of this routine has changed as of
         * Regina 4.3; in earlier versions, leading and trailing dollar
         * signs were provided.
         *
         * \return the common name of this 3-manifold in TeX format.
         */
        std::string texName() const;
        /**
         * Returns details of the structure of this 3-manifold that
         * might not be evident from its common name.  For instance, for
         * an orbit space S³/G this routine might return the full
         * Seifert structure.
         *
         * This routine may return the empty string if no additional
         * details are deemed necessary.
         *
         * \return a string describing additional structural details.
         */
        std::string structure() const;
        /**
         * Returns a triangulation of this 3-manifold, if such a
         * construction has been implemented.  For details of which types of
         * 3-manifolds have implemented this routine, see the class notes
         * for each corresponding subclasses of Manifold.
         *
         * The default implemention of this routine just throws a
         * NotImplemented exception.
         *
         * \exception NotImplemented Explicit construction has not yet been
         * implemented for this particular 3-manifold.
         *
         * \exception FileError The construction needs to be read from file (as
         * opposed to computed on the fly), but the file is inaccessible or its
         * contents cannot be read and parsed correctly.  Currently this can
         * only happen for the subclass SnapPeaCensusManifold, which reads its
         * triangulations from the SnapPea census databases that are installed
         * with Regina.
         *
         * \return a triangulation of this 3-manifold, if this
         * construction has been implemented.
         */
        virtual Triangulation<3> construct() const;
        /**
         * Returns the first homology group of this 3-manifold, if such
         * a routine has been implemented.  For details of which types of
         * 3-manifolds have implemented this routine, see the class notes
         * for each corresponding subclasses of Manifold.
         *
         * The default implemention of this routine just throws a
         * NotImplemented exception.
         *
         * \exception NotImplemented Homology calculation has not yet been
         * implemented for this particular 3-manifold.
         *
         * \exception FileError The homology needs to be read from file (as
         * opposed to computed), but the file is inaccessible or its contents
         * cannot be read and parsed correctly.  Currently this can only happen
         * for the subclass SnapPeaCensusManifold, which reads its results from
         * the SnapPea census databases that are installed with Regina.
         *
         * \return the first homology group of this 3-manifold, if this
         * functionality has been implemented.
         */
        virtual AbelianGroup homology() const;

        /**
         * Returns whether or not this is a finite-volume hyperbolic manifold.
         *
         * \return \c true if this is a finite-volume hyperbolic
         * manifold, or \c false if not.
         */
        virtual bool isHyperbolic() const = 0;

        /**
         * Compares representations of two 3-manifolds according to an
         * aesthetic ordering.
         *
         * The only purpose of this routine is to implement a consistent
         * ordering of 3-manifold representations.  The specific ordering
         * used is purely aesthetic on the part of the author, and is subject
         * to change in future versions of Regina.
         *
         * It does not matter whether the two 3-manifolds are homeomorphic;
         * this routine compares the specific _representations_ of these
         * manifolds (and so in particular, different representations of the
         * same 3-manifold might well be ordered differently).
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \warning Currently this routine is only implemented in full for
         * closed 3-manifolds.  For most classes of bounded 3-manifolds,
         * this routine simply compares the strings returned by name().
         * For this reason, the return value is currently marked as a
         * weak ordering, since it is possible that different representations
         * of the same 3-manifold will produce the same string name.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the 3-manifold representation to compare this with.
         * \return A result that indicates how this and the given 3-manifold
         * representation should be ordered with respect to each other.
         */
        std::weak_ordering operator <=> (const Manifold& rhs) const;

        /**
         * Writes the common name of this 3-manifold as a
         * human-readable string to the given output stream.
         *
         * \nopython Instead use the variant name() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
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
         * \nopython Instead use the variant texName() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;
        /**
         * Writes details of the structure of this 3-manifold that
         * might not be evident from its common name to the given output
         * stream.  For instance, for an orbit space S³/G this routine
         * might write the full Seifert structure.
         *
         * This routine may write nothing if no additional
         * details are deemed necessary.  The default implementation of
         * this routine behaves in this way.
         *
         * \nopython Instead use the variant structure() that takes no
         * arguments and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeStructure(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * Subclasses must not override this routine.  They should
         * override writeName() instead.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * Subclasses must not override this routine.  They should
         * override writeName() and writeStructure() instead.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * A default constructor.
         *
         * This does nothing in the base Manifold class, and is not for
         * public use.  It is declared here so that subclasses can use it
         * implicitly in their own default constructors.
         */
        Manifold() = default;
        /**
         * A copy constructor.
         *
         * This does nothing in the base Manifold class, and is not for
         * public use.  It is declared here so that subclasses can use it
         * implicitly in their own copy constructors.
         */
        Manifold(const Manifold&) = default;
        /**
         * A copy assignment operator.
         *
         * This does nothing in the base Manifold class, and is not for
         * public use.  It is declared here so that subclasses can use it
         * implicitly in their own copy assignment operators.
         */
        Manifold& operator = (const Manifold&) = default;
};

// Inline functions for Manifold

inline std::ostream& Manifold::writeStructure(std::ostream& out) const {
    return out;
}

inline void Manifold::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void Manifold::writeTextLong(std::ostream& out) const {
    writeName(out);
    std::string details = structure();
    if (! details.empty())
        out << " ( " << details << " )";
    out << std::endl;
}

} // namespace regina

#endif

