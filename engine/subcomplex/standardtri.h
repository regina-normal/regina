
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file subcomplex/standardtri.h
 *  \brief Deals with triangulations whose structures are well-understood.
 */

#ifndef __STANDARDTRI_H
#ifndef __DOXYGEN
#define __STANDARDTRI_H
#endif

#include "regina-core.h"
#include "output.h"
#include "triangulation/forward.h"
#include <boost/noncopyable.hpp>

namespace regina {

class AbelianGroup;
class Manifold;

/**
 * \addtogroup subcomplex Standard Triangulations and Subcomplexes
 * Standard triangulations and subcomplexes of triangulations whose
 * structures are well-understood.
 * @{
 */

/**
 * Describes a triangulation or subcomplex of a triangulation whose structure
 * is well-understood.  A StandardTriangulation is generally connected
 * with a real triangulation, i.e., a Triangulation<3> object, which it
 * describes some portion of.
 *
 * In general StandardTriangulation objects cannot be constructed
 * directly, but are instead created through static identification
 * routines such as
 * StandardTriangulation::isStandardTriangulation(Triangulation<3>*).
 *
 * Subclasses corresponding to different families of triangulations may,
 * but not need to, override the output routines writeTextShort() and
 * writeTextLong().  This class offers sensible default implementations
 * of both routines, which call the pure virtual function writeName()
 * (which every subclass must override).
 */
class REGINA_API StandardTriangulation :
        public Output<StandardTriangulation>,
        public boost::noncopyable {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~StandardTriangulation();

        /**
         * Returns the name of this specific triangulation as a
         * human-readable string.
         *
         * @return the name of this triangulation.
         */
        std::string name() const;
        /**
         * Returns the name of this specific triangulation in TeX
         * format.  No leading or trailing dollar signs will be included.
         *
         * \warning The behaviour of this routine has changed as of
         * Regina 4.3; in earlier versions, leading and trailing dollar
         * signs were provided.
         *
         * @return the name of this triangulation in TeX format.
         */
        std::string TeXName() const;
        /**
         * Returns the 3-manifold represented by this triangulation, if
         * such a recognition routine has been implemented.  If the
         * 3-manifold cannot be recognised then this routine will return 0.
         *
         * The details of which standard triangulations have 3-manifold
         * recognition routines can be found in the notes for the
         * corresponding subclasses of StandardTriangulation.  The
         * default implementation of this routine returns 0.
         *
         * It is expected that the number of triangulations whose
         * underlying 3-manifolds can be recognised will grow between
         * releases.
         *
         * The 3-manifold will be newly allocated and must be destroyed
         * by the caller of this routine.
         *
         * @return the underlying 3-manifold.
         */
        virtual Manifold* manifold() const;
        /**
         * Returns the expected first homology group of this triangulation,
         * if such a routine has been implemented.  If the calculation of
         * homology has not yet been implemented for this triangulation
         * then this routine will return 0.
         *
         * This routine does not work by calling Triangulation<3>::homology()
         * on the associated real triangulation.  Instead the homology is
         * calculated directly from the known properties of this
         * standard triangulation.
         *
         * The details of which standard triangulations have homology
         * calculation routines can be found in the notes for the
         * corresponding subclasses of StandardTriangulation.  The
         * default implementation of this routine returns 0.
         *
         * The homology group will be newly allocated and must be
         * destroyed by the caller of this routine.
         *
         * If this StandardTriangulation describes an entire Triangulation<3>
         * (and not just a part thereof) then the results of this routine
         * should be identical to the homology group obtained by calling
         * Triangulation<3>::homology() upon the associated real triangulation.
         *
         * This routine can also be accessed via the alias homologyH1()
         * (a name that is more specific, but a little longer to type).
         *
         * @return the first homology group of this triangulation, or 0 if
         * the appropriate calculation routine has not yet been implemented.
         */
        virtual AbelianGroup* homology() const;
        /**
         * Returns the expected first homology group of this triangulation,
         * if such a routine has been implemented.  If the calculation of
         * homology has not yet been implemented for this triangulation
         * then this routine will return 0.
         *
         * This routine does not work by calling Triangulation<3>::homology()
         * on the associated real triangulation.  Instead the homology is
         * calculated directly from the known properties of this
         * standard triangulation.
         *
         * The details of which standard triangulations have homology
         * calculation routines can be found in the notes for the
         * corresponding subclasses of StandardTriangulation.  The
         * default implementation of this routine returns 0.
         *
         * The homology group will be newly allocated and must be
         * destroyed by the caller of this routine.
         *
         * If this StandardTriangulation describes an entire Triangulation<3>
         * (and not just a part thereof) then the results of this routine
         * should be identical to the homology group obtained by calling
         * Triangulation<3>::homology() upon the associated real triangulation.
         *
         * This routine can also be accessed via the alias homology()
         * (a name that is less specific, but a little easier to type).
         *
         * @return the first homology group of this triangulation, or 0 if
         * the appropriate calculation routine has not yet been implemented.
         */
        AbelianGroup* homologyH1() const;

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
         * to the given output stream.  No leading or trailing dollar
         * signs will be included.
         *
         * \warning The behaviour of this routine has changed as of
         * Regina 4.3; in earlier versions, leading and trailing dollar
         * signs were provided.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * This may be reimplemented by subclasses, but the parent
         * StandardTriangulation class offers a reasonable default
         * implementation based on writeName().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * This may be reimplemented by subclasses, but the parent
         * StandardTriangulation class offers a reasonable default
         * implementation based on writeName().
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const;

        /**
         * Determines whether the given component represents one of the
         * standard triangulations understood by Regina.  The list of
         * recognised triangulations is expected to grow between
         * releases.
         *
         * If the standard triangulation returned has boundary triangles
         * then the given component must have the same corresponding
         * boundary triangles, i.e., the component cannot have any further
         * identifications of these boundary triangles with each other.
         *
         * Note that the triangulation-based routine
         * isStandardTriangulation(Triangulation<3>*) may recognise more
         * triangulations than this routine, since passing an entire
         * triangulation allows access to more information.
         *
         * @param component the triangulation component under examination.
         * @return the details of the standard triangulation if the
         * given component is recognised, or 0 otherwise.
         */
        static StandardTriangulation* isStandardTriangulation(
            Component<3>* component);
        /**
         * Determines whether the given triangulation represents one of the
         * standard triangulations understood by Regina.  The list of
         * recognised triangulations is expected to grow between
         * releases.
         *
         * If the standard triangulation returned has boundary triangles
         * then the given triangulation must have the same corresponding
         * boundary triangles, i.e., the triangulation cannot have any further
         * identifications of these boundary triangles with each other.
         *
         * This routine may recognise more triangulations than the
         * component-based isStandardTriangulation(Component<3>*),
         * since passing an entire triangulation allows access to
         * more information.
         *
         * @param tri the triangulation under examination.
         * @return the details of the standard triangualation if the
         * given triangulation is recognised, or 0 otherwise.
         */
        static StandardTriangulation* isStandardTriangulation(
            Triangulation<3>* tri);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NStandardTriangulation has now been renamed to
 * StandardTriangulation.
 */
REGINA_DEPRECATED typedef StandardTriangulation NStandardTriangulation;

/*@}*/

// Inline functions for StandardTriangulation

inline StandardTriangulation::~StandardTriangulation() {
}

inline Manifold* StandardTriangulation::manifold() const {
    return 0;
}

inline AbelianGroup* StandardTriangulation::homology() const {
    return 0;
}

inline AbelianGroup* StandardTriangulation::homologyH1() const {
    return homology();
}

inline void StandardTriangulation::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void StandardTriangulation::writeTextLong(std::ostream& out) const {
    writeName(out);
    out << '\n';
}

} // namespace regina

#endif

