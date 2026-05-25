
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

/*! \file subcomplex/standardsubcomplex.h
 *  \brief Deals with well-understood combinatorial subcomplexes within
 *  triangulations.
 */

#ifndef __REGINA_STANDARDSUBCOMPLEX_H
#ifndef __DOXYGEN
#define __REGINA_STANDARDSUBCOMPLEX_H
#endif

#include <memory>
#include <sstream>
#include "regina-core.h"
#include "core/output.h"
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "triangulation/forward.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class AbelianGroup;
class Manifold;

/**
 * \defgroup subcomplex Standard Triangulations and Subcomplexes
 * Standard triangulations and subcomplexes of triangulations whose
 * structures are well-understood.
 */

/**
 * An abstract base class for various types of well-understood combinatorial
 * subcomplexes within a <i>dim</i>-dimensional triangulation.
 *
 * Each subclass describes a particular type of well-understood combinatorial
 * subcomplex.  A single subclass could describe one particular subcomplex
 * (e.g., a one-tetrahedron snapped ball), or an entire parameterised family of
 * subcomplexes (e.g., layered solid tori).  These subcomplexes could describe
 * smaller parts of triangulations (e.g., layered solid tori, which appear in
 * many different larger 3-dimensional constructions), or they could describe
 * entire triangulations (e.g., the infinite family of layered lens spaces).
 *
 * A specific instance of a StandardSubcomplex would typically be associated
 * with a concrete triangulation, and would typically store details of where
 * the subcomplex is situated within the triangulation (e.g., via pointers to
 * top-dimensional simplices or lower-dimensional faces).
 *
 * End users typically cannot construct these subcomplex objects directly.
 * Instead you would typically obtain a StandardSubcomplex by passing a
 * triangulation (or part thereof) to a static recognition routine such as
 * `StandardSubcomplex<dim>::recognise(const Triangulation<dim>&)`.
 *
 * When defining a new subclass of StandardSubcomplex<dim>:
 *
 * - you must override the pure virtual functions writeName() and
 *   writeTeXName();
 *
 * - optionally, you can override manifold() and/or homology() if your class
 *   is able to provide this functionality;
 *
 * - optionally, you can override writeTextShort() and writeTextLong(), though
 *   this base class provides sensible default implementations based on
 *   writeName();
 *
 * - you must provide value semantics (including at least a copy constructor
 *   and assignment operator);
 *
 * - you must provide member and global swap functions, for consistency across
 *   all StandardSubcomplex subclasses.
 *
 * \python Python does not support templates.  Instead this class can be used
 * by appending the dimension as a suffix (e.g., StandardSubcomplex3 and
 * StandardSubcomplex4 for dimensions 3 and 4).
 *
 * \tparam dim the dimension of triangulation in which the subcomplexes live.
 *
 * \ingroup subcomplex
 */
template <int dim> requires (dim == 3 || dim == 4)
class StandardSubcomplex : public Output<StandardSubcomplex<dim>> {
    public:
        /**
         * A virtual destructor.  This does nothing, since the base class
         * StandardSubcomplex does not hold any data of its own.
         */
        virtual ~StandardSubcomplex() = default;

        /**
         * Returns the name of this subcomplex as a human-readable string.
         *
         * \return the name of this subcomplex.
         */
        std::string name() const {
            std::ostringstream ans;
            writeName(ans);
            return std::move(ans).str();
        }

        /**
         * Returns the name of this subcomplex in TeX format.
         *
         * The TeX will assume that we are within math mode, and no leading
         * or trailing dollar signs will be included.
         *
         * \return the name of this subcomplex in TeX format.
         */
        std::string texName() const {
            std::ostringstream ans;
            writeTeXName(ans);
            return std::move(ans).str();
        }

        /**
         * Returns the manifold represented by this subcomplex, if this is
         * known.  Currently this feature is only available in dimension 3.
         *
         * Subclasses of StandardSubcomplex may choose whether or not to
         * implement this routine.  Moreover, if a subclass describes a
         * parameterised family of subcomplexes, it may choose to implement
         * this only for some members of the family.  A return value of `null`
         * indicates that recognition has not been implemented for this
         * particular subcomplex.
         *
         * Individual subclasses of `StandardSubcomplex<3>` should explain
         * in their class notes whether they implement manifold().
         * The default implementation provided by this base class simply
         * returns `null`.
         *
         * It is expected that over time, more subclasses of StandardSubcomplex
         * will implement manifold() in future releases of Regina.
         *
         * \return the underlying manifold, or `null` if manifold recognition
         * is not yet implemented for this particular subcomplex.
         */
        virtual std::unique_ptr<Manifold> manifold() const {
            return {};
        }

        /**
         * Returns the first homology group of this subcomplex, if this is
         * known.
         *
         * This routine does _not_ call `Triangulation<dim>::homology()` on
         * the associated triangulation.  Instead the homology is calculated
         * directly from the known properties of this subcomplex.  This can
         * (for example) be used to assist with unit testing for subcomplex
         * recognition routines.
         *
         * Most users will not need this routine, since it is only implemented
         * for some subcomplexes (as opposed to `Triangulation<dim>::homology()`
         * which is implemented always).
         *
         * For subclasses that describe an entire triangulation (as opposed to
         * just a part thereof), the results of this routine should match the
         * homology group obtained by calling `Triangulation<dim>::homology()`.
         *
         * Subclasses of StandardSubcomplex may choose whether or not to
         * implement this routine.  Moreover, if a subclass describes a
         * parameterised family of subcomplexes, it may choose to implement
         * this only for some members of the family.  If homology has not been
         * implemented for this particular subcomplex, then this routine should
         * throw a NotImplemented exception.
         *
         * Individual subclasses of `StandardSubcomplex<3>` should explain
         * in their class notes whether they implement homology().
         * The default implementation provided by this base class just throws
         * a NotImplemented exception.
         *
         * It is expected that over time, more subclasses of StandardSubcomplex
         * will implement homology() in future releases of Regina.
         *
         * \exception NotImplemented Homology calculation is not yet
         * implemented for this particular subcomplex.
         *
         * \exception FileError The homology needs to be read from file (as
         * opposed to computed), but the file is inaccessible or its contents
         * cannot be read and parsed correctly.  Currently this can only happen
         * for the subclass SnapPeaCensusTri, which reads its results from
         * the SnapPea census databases that are installed with Regina.
         *
         * \return the first homology group, if this is implemented for this
         * particular subcomplex.
         */
        virtual AbelianGroup homology() const {
            throw NotImplemented("homology() is not implemented for this "
                "particular subcomplex");
        }

        /**
         * Writes the name of this subcomplex as a human-readable string to
         * the given output stream.
         *
         * \nopython Instead use name(), which takes no arguments and returns
         * a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;

        /**
         * Writes the name of this subcomplex in TeX format to the given
         * output stream.
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
         * Writes a short text representation of this object to the given
         * output stream.
         *
         * This may be reimplemented by subclasses, but the base class
         * StandardSubcomplex offers a reasonable default implementation
         * based on writeName().
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const {
            writeName(out);
        }

        /**
         * Writes a detailed text representation of this object to the given
         * output stream.
         *
         * This may be reimplemented by subclasses, but the base class
         * StandardSubcomplex offers a reasonable default implementation
         * based on writeTextShort().
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const {
            writeTextShort(out);
            out << '\n';
        }

        /**
         * Determines whether the given component is completely described by
         * one of the standard subcomplexes understood by Regina.  It is
         * anticipated that over time, more standard subcomplexes will be
         * recognised in future releases of Regina.
         *
         * If the subcomplex that is returned has boundary facets, then the
         * given component must have the same corresponding boundary facets.
         * That is, the component cannot have any further identifications of
         * boundary facets beyond those described by the subcomplex that is
         * returned.
         *
         * Note that the variant `recognise(const Triangulation<dim>&)` may
         * recognise more triangulations than this routine, since passing an
         * entire triangulation allows access to more information.
         *
         * \param component the triangulation component under examination.
         * \return details of the standard subcomplex if the given component
         * is recognised as such, or `null` otherwise.
         */
        static std::unique_ptr<StandardSubcomplex> recognise(
            Component<dim>* component);

        /**
         * Determines whether the given triangulation is completely described
         * by one of the standard subcomplexes understood by Regina.  It is
         * anticipated that over time, more standard subcomplexes will be
         * recognised in future releases of Regina.
         *
         * If the subcomplex that is returned has boundary facets, then the
         * given triangulation must have the same corresponding boundary facets.
         * That is, the triangulation cannot have any further identifications of
         * boundary facets beyond those described by the subcomplex that is
         * returned.
         *
         * This routine may recognise more triangulations than the variant
         * `recognise(Component<dim>*)`, since passing an entire triangulation
         * allows access to more information.
         *
         * \param tri the triangulation under examination.
         * \return details of the standard subcomplex if the given
         * triangulation is recognised as such, or `null` otherwise.
         */
        static std::unique_ptr<StandardSubcomplex> recognise(
            const Triangulation<dim>& tri);

    protected:
        /**
         * Default constructor.  This does nothing, since the base class
         * StandardSubcomplex does not hold any data of its own.
         */
        StandardSubcomplex() = default;
        /**
         * Copy constructor.  This does nothing, since the base class
         * StandardSubcomplex does not hold any data of its own.
         */
        StandardSubcomplex(const StandardSubcomplex&) = default;
        /**
         * Assignment operator.  This does nothing, since the base class
         * StandardSubcomplex does not hold any data of its own.
         */
        StandardSubcomplex& operator = (const StandardSubcomplex&) = default;
};

/**
 * A deprecated alias for StandardSubcomplex<3>, which is an abstract
 * base class for various well-understood combinatorial subcomplexes within
 * a 3-dimensional triangulation.
 *
 * \deprecated Now that combinatorial subcomplexes are supported in both
 * dimensions 3 and 4, the old 3-dimensional StandardTriangulation class
 * has been replaced with a new class template `StandardSubcomplex<dim>`.
 * The interface of the new StandardSubcomplex<3> is identical to the
 * interface of the old StandardTriangulation.
 */
using StandardTriangulation [[deprecated]] = StandardSubcomplex<3>;

} // namespace regina

#endif

