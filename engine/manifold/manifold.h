
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
 * - must \e not override writeTextShort() or writeTextLong(), since these
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
         * @return the common name of this 3-manifold.
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
         * @return the common name of this 3-manifold in TeX format.
         */
        std::string texName() const;
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
         * \exception NotImplemented explicit construction has not yet been
         * implemented for this particular 3-manifold.
         *
         * \exception FileError the construction needs to be read from file (as
         * opposed to computed on the fly), but the file is inaccessible or its
         * contents cannot be read and parsed correctly.  Currently this can
         * only happen for the subclass SnapPeaCensusManifold, which reads its
         * triangulations from the SnapPea census databases that are installed
         * with Regina.
         *
         * @return a triangulation of this 3-manifold, if this
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
         * \exception NotImplemented homology calculation has not yet been
         * implemented for this particular 3-manifold.
         *
         * \exception FileError the homology needs to be read from file (as
         * opposed to computed), but the file is inaccessible or its contents
         * cannot be read and parsed correctly.  Currently this can only happen
         * for the subclass SnapPeaCensusManifold, which reads its results from
         * the SnapPea census databases that are installed with Regina.
         *
         * @return the first homology group of this 3-manifold, if this
         * functionality has been implemented.
         */
        virtual AbelianGroup homology() const;

        /**
         * Returns whether or not this is a finite-volume hyperbolic manifold.
         *
         * @return \c true if this is a finite-volume hyperbolic
         * manifold, or \c false if not.
         */
        virtual bool isHyperbolic() const = 0;

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
         * this routine simply compares the strings returned by name().
         *
         * @param compare the 3-manifold representation with which this
         * will be compared.
         * @return \c true if and only if this is "smaller" than the
         * given 3-manifold representation.
         */
        bool operator < (const Manifold& compare) const;

        /**
         * Writes the common name of this 3-manifold as a
         * human-readable string to the given output stream.
         *
         * \ifacespython Not present; instead use the variant name()
         * that takes no arguments and returns a string.
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
         * \ifacespython Not present; instead use the variant texName()
         * that takes no arguments and returns a string.
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
         * \ifacespython Not present; instead use the variant structure()
         * that takes no arguments and returns a string.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeStructure(std::ostream& out) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * Subclasses must not override this routine.  They should
         * override writeName() instead.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * Subclasses must not override this routine.  They should
         * override writeName() and writeStructure() instead.
         *
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
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

