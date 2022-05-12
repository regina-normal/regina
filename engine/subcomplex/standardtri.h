
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#ifndef __REGINA_STANDARDTRI_H
#ifndef __DOXYGEN
#define __REGINA_STANDARDTRI_H
#endif

#include <memory>
#include "regina-core.h"
#include "core/output.h"
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "triangulation/forward.h"

namespace regina {

class AbelianGroup;
class Manifold;

/**
 * \defgroup subcomplex Standard Triangulations and Subcomplexes
 * Standard triangulations and subcomplexes of triangulations whose
 * structures are well-understood.
 */

/**
 * Describes a triangulation or subcomplex of a triangulation whose structure
 * is well-understood.  This is an abstract base class: its subclasses
 * correspond to different combinatorial constructions (typically
 * parameterised families of triangulations or subcomplexes).
 *
 * A StandardTriangulation is generally identified with a concrete
 * triangulation, i.e., a Triangulation<3> object, which it describes some
 * portion of (or possibly all of).
 *
 * In general StandardTriangulation objects cannot be constructed directly,
 * but are instead created through static identification routines such as
 * StandardTriangulation::recognise(const Triangulation<3>&).
 *
 * Each subclass of StandardTriangulation:
 *
 * - must override all pure virtual functions (of course);
 *
 * - may optionally override manifold() and/or homology(), if they are
 *   able to provide this functionality;
 *
 * - may optionally override writeTextShort() and writeTextLong(), though this
 *   class provides sensible default implementations which use writeName()
 *   (which subclasses always override because writeName() is pure virtual);
 *
 * - provide value semantics (including at least a copy constructor and
 *   assignment operator);
 *
 * - provide member and global swap functions, for consistency across all
 *   StandardTriangulation subclasses.
 *
 * \ingroup subcomplex
 */
class StandardTriangulation : public Output<StandardTriangulation> {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~StandardTriangulation() = default;

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
        std::string texName() const;
        /**
         * Returns the 3-manifold represented by this triangulation, if
         * such a recognition routine has been implemented.  If the 3-manifold
         * cannot be recognised then this routine will return \c null.
         *
         * The details of which standard triangulations have 3-manifold
         * recognition routines can be found in the notes for the
         * corresponding subclasses of StandardTriangulation.  The
         * default implementation of this routine returns \c null.
         *
         * It is expected that the number of triangulations whose
         * underlying 3-manifolds can be recognised will grow between
         * releases.
         *
         * @return the underlying 3-manifold.
         */
        virtual std::unique_ptr<Manifold> manifold() const;
        /**
         * Returns the expected first homology group of this triangulation,
         * if such a routine has been implemented.
         *
         * This routine does not work by calling Triangulation<3>::homology()
         * on the associated real triangulation.  Instead the homology is
         * calculated directly from the known properties of this
         * standard triangulation.
         *
         * This means that homology() needs to be implemented separately
         * for each class of standard triangulation.  See the class
         * notes for each subclass of StandardTriangulation for details
         * on whether homology has been implemented for that particular
         * subclass.  The default implementation of this routine just throws a
         * NotImplemented exception.
         *
         * Most users will not need this routine, since presumably you
         * already have an explicit Triangulation<3> available and so
         * you can just call Triangulation<3>::homology() instead
         * (which, unlike this routine, \e is always implemented).
         * This StandardTriangulation::homology() routine should be seen
         * as more of a verification/validation tool for the Regina developers.
         *
         * If this StandardTriangulation describes an entire Triangulation<3>
         * (and not just a part thereof) then the results of this routine
         * should be identical to the homology group obtained by calling
         * Triangulation<3>::homology() upon the associated real triangulation.
         *
         * \exception NotImplemented homology calculation has not yet
         * been implemented for this particular type of standard triangulation.
         *
         * \exception FileError the homology needs to be read from file (as
         * opposed to computed), but the file is inaccessible or its contents
         * cannot be read and parsed correctly.  Currently this can only happen
         * for the subclass SnapPeaCensusTri, which reads its results from
         * the SnapPea census databases that are installed with Regina.
         *
         * @return the first homology group of this triangulation, if this
         * functionality has been implemented.
         */
        virtual AbelianGroup homology() const;

        /**
         * Writes the name of this triangulation as a human-readable
         * string to the given output stream.
         *
         * \ifacespython Not present; instead use the variant name() that
         * takes no arguments and returns a string.
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
         * \ifacespython Not present; instead use the variant texName() that
         * takes no arguments and returns a string.
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
         * \ifacespython Not present; use str() instead.
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
         * implementation based on writeTextShort().
         *
         * \ifacespython Not present; use detail() instead.
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
         * recognise(const Triangulation<3>&) may recognise more
         * triangulations than this routine, since passing an entire
         * triangulation allows access to more information.
         *
         * @param component the triangulation component under examination.
         * @return the details of the standard triangulation if the
         * given component is recognised, or \c null otherwise.
         */
        static std::unique_ptr<StandardTriangulation> recognise(
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
         * component-based recognise(Component<3>*), since passing an entire
         * triangulation allows access to more information.
         *
         * @param tri the triangulation under examination.
         * @return the details of the standard triangualation if the
         * given triangulation is recognised, or \c null otherwise.
         */
        static std::unique_ptr<StandardTriangulation> recognise(
            const Triangulation<3>& tri);

    protected:
        /**
         * Default constructor.  This is here for subclasses to implicitly
         * call from their own constructors.
         */
        StandardTriangulation() = default;
        /**
         * Do-nothing copy constructor.  This is here for subclasses to
         * implicitly call from their own copy constructors.
         */
        StandardTriangulation(const StandardTriangulation&) = default;
        /**
         * Do-nothing assignment operator.  This is here for subclasses to
         * implicitly call from their own assignment operators.
         */
        StandardTriangulation& operator = (const StandardTriangulation&) =
            default;

};

// Inline functions for StandardTriangulation

inline std::unique_ptr<Manifold> StandardTriangulation::manifold() const {
    return nullptr;
}

inline void StandardTriangulation::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void StandardTriangulation::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << '\n';
}

} // namespace regina

#endif

