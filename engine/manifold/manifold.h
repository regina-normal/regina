
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

#include <sstream>
#include "regina-core.h"
#include "algebra/abeliangroup.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class AbelianGroup;

/**
 * \defgroup manifold Standard Manifolds
 * Standard manifolds whose structures are well-understood.
 */

/**
 * Optional features that manifold classes may choose to implement.
 *
 * As described in the Manifold class notes, each subclass of `Manifold<dim>`
 * describes a particular type of well-understood <i>dim</i>-dimensional
 * manifold.
 *
 * Every such subclass _must_ implement basic features, such as writing the
 * manifold name in English and in TeX.  However, there are other features
 * that are optional for subclasses to implement: examples in dimension 3
 * include hyperbolicity testing and first homology.
 *
 * This base class `ManifoldOptions<dim>` describes those optional features as
 * virtual functions, and provides default implementations that indicate to the
 * user in some appropriate way that the feature is not implemented (e.g., by
 * returning `null`, or throwing an exception).  Each subclass of
 * `Manifold<dim>` may, if desired, override any of these virtual functions to
 * provide a real implementation.  If a subclass describes a parameterised
 * family of manifolds, it may even choose to implement these features only
 * for some members of the family (falling back to the default "not impemented"
 * behaviour for the others).
 *
 * These optional features are specific to the dimension, and so this class
 * should be specialised where necessary (for example, the virtual functions
 * `isHyperbolic()` and `homology()` as discussed earlier are provided by the
 * specialisation `ManifoldOptions<3>`).  The generic implementation of
 * ManifoldOptions has no functions at all.
 *
 * \python Python does not support templates.  Instead this class can be used
 * by appending the dimension as a suffix (e.g., ManifoldOptions3 for
 * dimension 3).
 *
 * \tparam dim the dimension of manifold under consideration.
 *
 * \ingroup manifold
 */
template <int dim> requires (dim == 3)
class ManifoldOptions {
    protected:
        /**
         * Default constructor.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions() = default;
        /**
         * Copy constructor.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions(const ManifoldOptions&) = default;
        /**
         * Copy assignment operator.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions& operator = (const ManifoldOptions&) = default;
};

/**
 * Describes optional features that 3-manifold classes may choose to implement.
 * See the generic ManifoldOptions class notes for an overview of how such
 * optional features work.
 *
 * This class provides these optional features as virtual functions, with
 * default implementations that indicate to the user in some appropriate way
 * that the feature is not implemented (e.g., by returning `null`, or throwing
 * an exception).  Each subclass of `Manifold<3>` may, if desired, override
 * any of these virtual functions to provide a real implementation.
 *
 * Individual manifold classes should explain in their class notes which of
 * these optional features they implement.
 *
 * \pyclassname{ManifoldOptions3}
 *
 * \ingroup manifold
 */
template <>
class ManifoldOptions<3> {
    public:
        /**
         * Returns the first homology group of this 3-manifold, if this is
         * known to Regina.
         *
         * This is an optional feature that 3-manifold classes may choose
         * whether to implement; see the ManifoldOptions class notes
         * for an overview of how such optional features work.
         *
         * If homology has not been implemented for this particular 3-manifold,
         * this routine should return throw a NotImplemented exception (which is
         * what the default implementation does).
         *
         * It is expected that over time, more 3-manifold classes will
         * implement homology() in future releases of Regina.
         *
         * \exception NotImplemented Homology calculation is not yet
         * implemented for this particular 3-manifold.
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
        virtual AbelianGroup homology() const {
            throw NotImplemented("homology() is not implemented "
                "for this particular 3-manifold");
        }

        /**
         * Returns whether or not this is a finite-volume hyperbolic
         * 3-manifold, if this is known to Regina.
         *
         * This is an optional feature that 3-manifold classes may choose
         * whether to implement; see the ManifoldOptions class notes
         * for an overview of how such optional features work.
         *
         * If hyperbolicity testing has not been implemented for this
         * particular 3-manifold, this routine should throw a NotImplemented
         * exception (which is what the default implementation does).
         *
         * As of Regina 8.0, _all_ 3-manifold classes implement isHyperbolic()
         * (though it is possible that the future there will be new manifold
         * classes that do not).
         *
         * \exception NotImplemented Hyperbolicity testing is not yet
         * implemented for this particular 3-manifold.
         *
         * \return \c true if this is a finite-volume hyperbolic
         * 3-manifold, or \c false if not.
         */
        virtual bool isHyperbolic() const {
            throw NotImplemented("isHyperbolic() is not implemented "
                "for this particular 3-manifold");
        }

    protected:
        /**
         * Default constructor.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions() = default;
        /**
         * Copy constructor.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions(const ManifoldOptions&) = default;
        /**
         * Copy assignment operator.  This does nothing, since this base class
         * does not hold any data of its own.
         */
        ManifoldOptions& operator = (const ManifoldOptions&) = default;
};

/**
 * An abstract base class for various well-understood <i>dim</i>-dimensional
 * -manifolds.
 *
 * Each subclass of Manifold describes a particular type of well-understood
 * <i>dim</i>-manifold.  A single subclass could describe one particular
 * manifold (e.g., the <i>dim</i>-dimensional sphere), or an entire
 * parameterised family (e.g., lens spaces).  Manifolds do _not_ encode
 * specific triangulations; instead they describe the topology of a manifold,
 * independent of how it might be triangulated.
 *
 * Importantly, these subclasses do not aim to classify all manifolds:
 * there are many manifolds that are not represented by any subclass of
 * Manifold at all.
 *
 * When defining a new subclass of `Manifold<dim>`:
 *
 * - you must override the pure virtual functions writeName() and
 *   writeTeXName();
 *
 * - optionally, you can implement the extra features offered through
 *   `ManifoldOptions<dim>` (such as isHyperbolic() and homology() in
 *   dimension 3);
 *
 * - optionally, you can override construct() and/or writeStructure() if your
 *   class is able to provide this functionality;
 *
 * - you must _not_ override writeTextShort() or writeTextLong(), since these
 *   routines are not virtual, but are provided directly by the Manifold base
 *   class;
 *
 * - you must provide value semantics (including at least a copy constructor
 *   and assignment operator);
 *
 * - you must provide comparison operators (== and !=) that compare two objects
 *   of your subclass;
 *
 * - you must provide member and global swap functions, for consistency across
 *   all Manifold subclasses;
 *
 * - optionally, for dimension 3, you may incorporate your subclass into the
 *   global ordering on 3-manifolds (operator <=>).
 *
 * \python Python does not support templates.  Instead this class can be used
 * by appending the dimension as a suffix (e.g., Manifold3 for dimensions 3).
 * Moreover, in Python the name Manifold is now a deprecated alias for
 * Manifold3 (for backward compatibility with code written for Regina ≤ 7.x).
 *
 * \tparam dim the dimension of manifold under consideration.  Currently this
 * is restricted to dimension 3 only, but the intent is to support other
 * dimensions in future versions of Regina.
 *
 * \ingroup manifold
 */
template <int dim> requires (dim == 3)
class Manifold : public ManifoldOptions<dim>, public Output<Manifold<dim>> {
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
        inline std::string name() const {
            std::ostringstream ans;
            writeName(ans);
            return std::move(ans).str();
        }

        /**
         * Returns the common name of this manifold in TeX format.
         *
         * The TeX will assume that we are within math mode, and no leading
         * or trailing dollar signs will be included.
         *
         * \return the common name of this manifold in TeX format.
         */
        inline std::string texName() const {
            std::ostringstream ans;
            writeTeXName(ans);
            return std::move(ans).str();
        }

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
        inline std::string structure() const {
            std::ostringstream ans;
            writeStructure(ans);
            return std::move(ans).str();
        }

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
         * \exception NotImplemented Triangulation is not yet implemented for
         * this particular manifold.
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
        virtual Triangulation<dim> construct() const;

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

/**
 * Compares representations of two 3-manifolds according to an aesthetic
 * ordering.
 *
 * The only purpose of this routine is to implement a consistent ordering of
 * 3-manifold representations.  The specific ordering used is purely aesthetic
 * on the part of the author, and is subject to change in future versions of
 * Regina.
 *
 * It does not matter whether the two 3-manifolds are homeomorphic; this
 * routine compares the specific _representations_ of these manifolds (and so
 * in particular, different representations of the same 3-manifold might well
 * be ordered differently).
 *
 * This operator generates all of the usual comparison operators, including
 * `<`, `<=`, `>`, and `>=`.
 *
 * \warning Currently this routine is only implemented in full for closed
 * 3-manifolds.  For most classes of bounded 3-manifolds, this routine simply
 * compares the strings returned by name().  For this reason, the return value
 * is currently marked as a weak ordering, since it is possible that different
 * representations of the same 3-manifold will produce the same string name.
 *
 * \python This spaceship operator `x <=> y` is not available, but the other
 * comparison operators that it generates _are_ available.
 *
 * \param x the first 3-manifold representation to compare.
 * \param y the second 3-manifold representation to compare.
 * \return a result that indicates how the representations of \a x and \a y
 * should be ordered with respect to each other.
 */
std::weak_ordering operator <=> (const Manifold<3>& x, const Manifold<3>& y);

} // namespace regina

#endif

