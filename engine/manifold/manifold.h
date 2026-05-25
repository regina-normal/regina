
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  \brief Deals with the underlying manifolds that are represented by
 *  triangulations.
 */

#ifndef __REGINA_MANIFOLD_H
#ifndef __DOXYGEN
#define __REGINA_MANIFOLD_H
#endif

#include "regina-core.h"
#include "algebra/abeliangroup.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class AbelianGroup;

/**
 * \defgroup manifold Standard Manifolds
 * Standard 3-manifolds whose structures are well-understood.
 */

/**
 * An abstract base class for various well-understood 3-manifolds.
 *
 * Each subclass of Manifold describes a particular type of well-understood
 * 3-manifold.  A single subclass could describe one particular manifold
 * (e.g., the 3-sphere), or an entire parameterised family (e.g., lens spaces).
 * Manifolds do _not_ encode specific triangulations; instead they describe the
 * topology of a manifold, independent of how it might be triangulated.
 *
 * Importantly, these subclasses do not aim to classify all 3-manifolds:
 * there are many 3-manifolds that are not represented by any subclass of
 * Manifold at all.
 *
 * When defining a new subclass of Manifold:
 *
 * - you must override the pure virtual functions writeName(), writeTeXName(),
 *   and isHyperbolic();
 *
 * - optionally, you can override construct(), homology() and/or
 *   writeStructure() if your class is able to provide this functionality;
 *
 * - you must _not_ override writeTextShort() or writeTextLong(), since these
 *   routines are not virtual, and are provided directly by the Manifold base
 *   class;
 *
 * - you must provide value semantics (including at least a copy constructor
 *   and assignment operator);
 *
 * - you must provide comparison operators (== and !=);
 *
 * - optionally, you may incorporate your subclass into the global ordering on
 *   manifolds (operator <=>);
 *
 * - you must provide member and global swap functions, for consistency across
 *   all Manifold subclasses.
 *
 * \ingroup manifold
 */
class Manifold : public Output<Manifold> {
    public:
        /**
         * A virtual destructor.  This does nothing, since the base class
         * Manifold does not hold any data of its own.
         */
        virtual ~Manifold() = default;

        /**
         * Returns the common name of this manifold as a human-readable
         * string.
         *
         * \return the common name of this manifold.
         */
        std::string name() const;

        /**
         * Returns the common name of this manifold in TeX format.
         *
         * The TeX will assume that we are within math mode, and no leading
         * or trailing dollar signs will be included.
         *
         * \return the common name of this manifold in TeX format.
         */
        std::string texName() const;

        /**
         * Returns details of the structure of this manifold that might not
         * be evident from its common name.  For instance, for an orbit space
         * S³/G this routine might return the full Seifert structure.
         *
         * This routine may return the empty string if no additional
         * details are deemed necessary.
         *
         * \return a string describing additional structural details.
         */
        std::string structure() const;

        /**
         * Returns a triangulation of this manifold, if such a construction
         * has been implemented.
         *
         * Subclasses of Manifold may choose whether or not to implement this
         * routine.  Moreover, if a subclass describes a parameterised family
         * of manifolds, it may choose to implement this only for some members
         * of the family.  If construction has not been implemented for this
         * particular manifold, then this routine should throw a NotImplemented
         * exception.
         *
         * Individual subclasses of Manifold should explain in their class
         * notes whether they implement construct().  The default implementation
         * provided by this base class just throws a NotImplemented exception.
         *
         * It is expected that over time, more subclasses of Manifold
         * will implement construct() in future releases of Regina.
         *
         * \exception NotImplemented Explicit construction is not yet
         * implemented for this particular manifold.
         *
         * \exception FileError The construction needs to be read from file (as
         * opposed to computed on the fly), but the file is inaccessible or its
         * contents cannot be read and parsed correctly.  Currently this can
         * only happen for the subclass SnapPeaCensusManifold, which reads its
         * triangulations from the SnapPea census databases that are installed
         * with Regina.
         *
         * \return a triangulation of this manifold, if this construction has
         * been implemented.
         */
        virtual Triangulation<3> construct() const;

        /**
         * Returns the first homology group of this manifold, if this is known
         * to Regina.
         *
         * Subclasses of Manifold may choose whether or not to implement this
         * routine.  Moreover, if a subclass describes a parameterised family
         * of manifolds, it may choose to implement this only for some members
         * of the family.  If homology has not been implemented for this
         * particular manifold, then this routine should throw a NotImplemented
         * exception.
         *
         * Individual subclasses of Manifold should explain in their class
         * notes whether they implement homology().  The default implementation
         * provided by this base class just throws a NotImplemented exception.
         *
         * It is expected that over time, more subclasses of Manifold
         * will implement homology() in future releases of Regina.
         *
         * \exception NotImplemented Homology calculation is not yet
         * implemented for this particular manifold.
         *
         * \exception FileError The homology needs to be read from file (as
         * opposed to computed), but the file is inaccessible or its contents
         * cannot be read and parsed correctly.  Currently this can only happen
         * for the subclass SnapPeaCensusManifold, which reads its results from
         * the SnapPea census databases that are installed with Regina.
         *
         * \return the first homology group, if this is implemented for this
         * particular manifold.
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
         * \return a result that indicates how this and the given 3-manifold
         * representation should be ordered with respect to each other.
         */
        std::weak_ordering operator <=> (const Manifold& rhs) const;

        /**
         * Writes the common name of this manifold as a human-readable
         * string to the given output stream.
         *
         * \nopython Instead use name(), which takes no arguments and
         * returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;

        /**
         * Writes the common name of this manifold in TeX format to the
         * given output stream.
         *
         * The TeX should assume that we are within math mode, and no leading
         * or trailing dollar signs should be included.
         *
         * \nopython Instead use texName(), which takes no arguments and
         * returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        /**
         * Writes details of the structure of this manifold that might not be
         * evident from its common name to the given output stream.
         * For instance, for an orbit space S³/G this routine might write the
         * full Seifert structure.
         *
         * This routine may write nothing if no additional details are deemed
         * necessary.  The default implementation of this routine behaves in
         * this way.
         *
         * \nopython Instead use structure(), which takes no arguments and
         * returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeStructure(std::ostream& out) const {
            return out;
        }

        /**
         * Writes a short text representation of this object to the given
         * output stream.
         *
         * Subclasses must not override this routine.  They should override
         * writeName() instead.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const {
            writeName(out);
        }

        /**
         * Writes a detailed text representation of this object to the given
         * output stream.
         *
         * Subclasses must not override this routine.  They should override
         * writeName() and writeStructure() instead.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const {
            writeName(out);
            std::string details = structure();
            if (! details.empty())
                out << " ( " << details << " )";
            out << std::endl;
        }

    protected:
        /**
         * Default constructor.  This does nothing, since the base class
         * Manifold does not hold any data of its own.
         */
        Manifold() = default;
        /**
         * Copy constructor.  This does nothing, since the base class
         * Manifold does not hold any data of its own.
         */
        Manifold(const Manifold&) = default;
        /**
         * Copy assignment operator.  This does nothing, since the base class
         * Manifold does not hold any data of its own.
         */
        Manifold& operator = (const Manifold&) = default;
};

} // namespace regina

#endif

