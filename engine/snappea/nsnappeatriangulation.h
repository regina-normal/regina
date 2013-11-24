
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file snappea/nsnappeatriangulation.h
 *  \brief Allows Regina triangulations to interact with the SnapPea kernel.
 */

#ifndef __NSNAPPEATRIANGULATION_H
#ifndef __DOXYGEN
#define __NSNAPPEATRIANGULATION_H
#endif

#include "regina-config.h" // For EXCLUDE_SNAPPEA

#include "regina-core.h"
#include "shareableobject.h"

// Forward declaration of SnapPea structures.
struct Triangulation;

namespace regina {

class NMatrixInt;
class NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

#ifndef EXCLUDE_SNAPPEA

/**
 * A base class for all exceptions that are thrown from within the
 * SnapPea kernel.
 */
struct SnapPeaException {
};

/**
 * An exception that is thrown when the SnapPea kernel encounters a
 * fatal error.
 */
struct SnapPeaFatalError : public SnapPeaException {
    std::string function;
        /**< The function from the SnapPea kernel in which the
             fatal error occurred. */
    std::string file;
        /**< The source file from the SnapPea kernel in which the
             fatal error occurred. */

    /**
     * Creates a new exception, indicating where in the SnapPea kernel
     * the error occurred.
     *
     * @param fromFunction the function from the SnapPea kernel in which
     * the error occurred.
     * @param fromFile the source file from the SnapPea kernel in which
     * the error occurred.
     */
    SnapPeaFatalError(const char* fromFunction, const char* fromFile);
};

/**
 * An exception that is thrown when the SnapPea kernel finds that all
 * available memory has been exhausted.
 */
struct SnapPeaMemoryFull : public SnapPeaException {
};

/**
 * Offers direct access to the SnapPea kernel from within Regina.
 *
 * An object of this class represents a 3-manifold triangulation, stored in
 * SnapPea's internal format.  Such an object may be constructed by cloning
 * either a standard Regina triangulation or another SnapPea triangulation.
 *
 * Note that not all Regina triangulations can be represented in SnapPea
 * format, since Regina works with more general kinds of triangulation
 * than SnapPea does.  You should always call isNull() to test whether any
 * Regina-to-SnapPea conversion was successful.
 *
 * This class is designed to act as the sole conduit between the Regina
 * calculation engine and the SnapPea kernel.  Regina code should not
 * interact with the SnapPea kernel other than through this class.
 *
 * There are many places in the SnapPea kernel where SnapPea throws a
 * fatal error.  As of Regina 4.96, these fatal errors are converted
 * into exceptions (subclassed from SnapPeaException), which can be caught
 * and handled politely.
 *
 * Regina uses the variant of the SnapPea kernel that is shipped with
 * SnapPy, as well as some additional code written explicitly for SnapPy.
 * The SnapPea kernel was written by Jeff Weeks, and SnapPy was written by
 * Marc Culler, Nathan Dunfield, and others.  SnapPy and the corresponding
 * SnapPea kernel are distributed under the terms of the GNU General
 * Public License, version 2 or any later version, as published by the
 * Free Software Foundation.
 *
 * See http://snappy.computop.org/ for further information on
 * SnapPea and its successor SnapPy.
 *
 * \testpart
 */
class REGINA_API NSnapPeaTriangulation : public ShareableObject {
    public:
        /**
         * Describes the different types of solution that can be found when
         * solving for a complete hyperbolic structure.
         *
         * Although this enumeration is identical to SnapPea's own
         * SolutionType, it is declared again in this class because Regina
         * code should not in general be interacting directly with the
         * SnapPea kernel.  Values may be freely converted between the
         * two enumeration types by simple assignment and/or typecasting.
         *
         * \warning This enumeration must always be kept in sync with
         * SnapPea's own SolutionType enumeration.
         */
        typedef enum {
            not_attempted,
                /**< A solution has not been attempted. */
            geometric_solution,
                /**< All tetrahedra are either positively oriented or
                     flat, though the entire solution is not flat and
                     no tetrahedra are degenerate. */
            nongeometric_solution,
                /**< The volume is positive, but some tetrahedra are
                     negatively oriented. */
            flat_solution,
                /**< All tetrahedra are flat, but none have shape 0, 1 or
                     infinity. */
            degenerate_solution,
                /**< At least one tetrahedron has shape 0, 1 or infinity. */
            other_solution,
                /**< The volume is zero or negative, but the solution is
                     neither flat nor degenerate. */
            no_solution
                /**< The gluing equations could not be solved. */
        } SolutionType;

    private:
        ::Triangulation* snappeaData;
            /**< The triangulation stored in SnapPea's native format. */
        static bool kernelMessages;
            /**< Should the SnapPea kernel write diagnostic messages to
                 standard output? */

    public:
        /**
         * Creates a SnapPea clone of the given SnapPea triangulation.
         * This SnapPea triangulation stands independent of \a tri,
         * so this triangulation will not be affected if \a tri is later
         * changed or destroyed.
         *
         * If \a tri is a null triangulation then this will be a null
         * triangulation also.  See isNull() for further details.
         *
         * Note that the tetrahedron and vertex numbers might be changed
         * in the new SnapPea triangulation.
         *
         * @param tri the SnapPea triangulation to clone.
         */
        NSnapPeaTriangulation(const NSnapPeaTriangulation& tri);

        /**
         * Creates a SnapPea clone of the given Regina triangulation.
         * This SnapPea triangulation stands independent of \a tri,
         * so this triangulation will not be affected if \a tri is later
         * changed or destroyed.
         *
         * Note that, since Regina works with more general kinds of
         * trianguations than SnapPea, not all Regina triangulations can be
         * represented in SnapPea format.  If the conversion is
         * unsuccessful, this will be marked as a null triangulation.
         * You should always test isNull() to determine whether the
         * conversion was successful.
         *
         * If the conversion is successful, this constructor will immediately
         * ask SnapPea to try to find a complete hyperbolic structure.
         *
         * SnapPea is designed primarily to work with ideal
         * triangulations only.  Passing closed triangulations can
         * occasionally cause the SnapPea kernel to raise a fatal error,
         * which in turn will crash the entire program.  Thus by default,
         * closed triangulations are never converted (a null SnapPea
         * triangulation will be created instead).  See the optional
         * argument \a allowClosed for how to change this behaviour.
         *
         * Note also that the tetrahedron and vertex numbers might be changed
         * in the new SnapPea triangulation.  In particular, if the
         * given triangulation is orientable but not oriented, then you
         * should expect these numbers to change.
         *
         * \warning Passing \a allowClosed as \c true can occasionally
         * cause the program to crash!  See the notes above for details.
         *
         * @param tri the Regina triangulation to clone.
         * @param allowClosed \c true if closed triangulations should be
         * considered, or \c false if all closed triangulations should give
         * null SnapPea data (the default).  See above for details.
         */
        NSnapPeaTriangulation(const NTriangulation& tri,
            bool allowClosed = false);

        /**
         * Destroys this triangulation.  All internal SnapPea data will
         * also be destroyed.
         */
        ~NSnapPeaTriangulation();

        /**
         * Determines whether this triangulation contains valid SnapPea
         * data.
         *
         * A null SnapPea triangulation can occur when converting unusual
         * types of Regina triangulation into SnapPea format, since
         * Regina is written to deal with more general types of triangulations
         * than SnapPea.
         *
         * @return \c true if this is a null triangulation, or \c false
         * if this triangulation contains valid SnapPea data.
         */
        bool isNull() const;

        /**
         * Returns the type of solution found when solving for a complete
         * hyperbolic structure.
         *
         * Note that SnapPea distinguishes between a complete hyperbolic
         * structure and a Dehn filled hyperbolic structure.  At the present
         * time Regina does not concern itself with Dehn fillings, so only
         * the complete solution type is offered here.
         *
         * @return the solution type.
         */
        SolutionType solutionType() const;

        /**
         * Computes the volume of the underlying 3-manifold.
         *
         * @return the volume of the underlying 3-manifold, or 0 if this
         * is a null triangulation.
         */
        double volume() const;

        /**
         * Computes the volume of the underlying 3-manifold and
         * estimates the accuracy of the answer.
         *
         * @param precision used to return an estimate of the number of
         * decimal places of accuracy in the calculated volume.
         *
         * \ifacespython The \a precision argument is not present.
         * Instead, two routines are offered.  The routine \a volume()
         * takes no arguments and returns the volume only, whereas the
         * routine \a volumeWithPrecision() takes no arguments and
         * returns a (\a volume, \a precision) tuple.
         *
         * @return the volume of the underlying 3-manifold, or 0 if this
         * is a null triangulation.
         */
        double volume(int& precision) const;

        /**
         * Returns a matrix for computing boundary slopes of
         * spun-normal surfaces at the cusps of the triangulation.  This
         * matrix includes a pair of rows for each cusp in the triangulation:
         * one row for determining the algebraic intersection number
         * with the meridian, and one row for determining the algebraic
         * intersection number with the longitude.
         * If the triangulation has more than one cusp, these pairs are
         * ordered by vertex number in the triangulation.  Within each
         * pair, the meridian row always appears before the longitude row.
         *
         * This matrix is constructed so that, if \a M and \a L are the
         * rows for the meridian and longitude at some cusp, then for
         * any spun-normal surface with quadrilateral coordinates
         * \a q, the boundary curves have algebraic intersection number
         * <i>M.q</i> with the meridian and <i>L.q</i> with the longitude.
         * Equivalently, the boundary curves pass <i>L.q</i> times around the
         * meridian and <i>-M.q</i> times around the longitude.
         * To compute these slopes directly from a normal surface, see
         * NNormalSurface::boundarySlopes().
         *
         * This code makes use of the \e SnapPy kernel, and the choice
         * of meridian and longitude on each cusp follows \e SnapPy's
         * conventions.  In particular, we use the orientations for
         * meridian and longitude from \e SnapPy. The orientations of the
         * boundary curves of a spun-normal surface are chosen so
         * that \e if meridian and longitude are a positive basis as
         * vieved from the cusp, then as one travels along an oriented
         * boundary curve, the spun-normal surface spirals into the cusp
         * to one's right and down into the manifold to one's left.
         *
         * \pre All vertex links in this triangulation must be tori.
         *
         * @author William Pettersson and Stephan Tillmann
         *
         * @return a newly allocated matrix with (2 * \a number_of_cusps) rows
         * and (3 * \a number_of_tetrahedron) columns as described above,
         * or 0 if this is a null triangulation.
         */
        NMatrixInt* slopeEquations() const;

        /**
         * Constructs the canonical retriangulation of the canonical
         * cell decomposition.
         *
         * The canonical cell decomposition is the one described in
         * "Convex hulls and isometries of cusped hyperbolic 3-manifolds",
         * Jeffrey R. Weeks, Topology Appl. 52 (1993), 127-149.
         * The canoical retriangulation is defined as follows: If the
         * canonical cell decomposition is already a triangulation then
         * we leave it untouched.  Otherwise: (i) within each 3-cell of
         * the original complex we introduce a new internal (finite)
         * vertex and cone the 3-cell boundary to this new vertex, and
         * (ii) for each 2-cell of the original complex we replace the
         * two new cones on either side with a ring of tetrahedra surrounding
         * a new edge that connects the two new vertices on either side.
         * See canonize_part_2.c in the SnapPea source code for details.
         *
         * The resulting triangulation will be newly allocated, and it
         * is the responsibility of the caller of this routine to destroy it.
         *
         * If for any reason either Regina or SnapPea are unable to
         * construct the canonical retriangulation of the canonical cell
         * decomposition, this routine will return 0.
         *
         * \warning This matches the triangulation produced by SnapPea's
         * version of canonize().  However, it does not match the
         * triangulation produced by SnapPy's version of canonize().
         * This is because SnapPy returns an arbitrary simplicial subdivision
         * of the canonical cell decomposition, whereas SnapPea follows
         * this with a canonical retriangulation.
         *
         * \pre This is an ideal triangulation, not a closed triangulation.
         *
         * @return the canonical triangulation of the canonical cell
         * decomposition, or 0 if this could not be constructed.
         */
        NTriangulation* canonize() const;

        /**
         * Asks SnapPea to randomly retriangulate its internal
         * representation of the manifold.  This can help when SnapPea
         * is having difficulty finding a complete hyperbolic structure.
         *
         * This routine uses SnapPea's own retriangulation code.  It is highly
         * likely that, after calling randomize(), the internal SnapPea
         * representation of the manifold will \e not match the original
         * triangulation passed from Regina.  In other words, calling
         * verifyTriangulation() with your original Regina triangulation
         * will most likely return \c false.
         *
         * After randomizing, this routine will immediately ask SnapPea
         * to try to find a complete hyperbolic structure.
         *
         * If this is a null SnapPea triangulation, this routine does nothing.
         */
        void randomize();

        /**
         * Verifies that the tetrahedron face gluings from this SnapPea
         * triangulation match the given Regina triangulation precisely.
         *
         * This is useful if you are not sure whether SnapPea will relabel
         * and/or retriangulate.
         *
         * This routine is equivalent to testing whether the given
         * triangulation is identical to the triangulation returned by
         * toRegina().
         *
         * @param triangulation the triangulation to compare with this
         * SnapPea triangulation.
         * @return \c true if the tetrahedron face gluings match precisely, or
         * \c false if the face gluings do not match or if this is a
         * null triangulation.
         */
        bool verifyTriangulation(const NTriangulation& triangulation) const;

        /**
         * Creates a new Regina triangulation that mirrors the internal
         * SnapPea structure.
         *
         * Note that this might be different from the original triangulation
         * that was passed into the NSnapPeaTriangulation constructor, since
         * the SnapPea kernel can sometimes relabel tetrahedra and/or
         * retriangulate the manifold.
         *
         * The resulting triangulation will be newly created, and it is
         * the responsibility of the caller of this routine to
         * eventually delete it.
         *
         * @return a new Regina triangulation, or \c null if this is a
         * null SnapPea triangulation (see isNull()).
         */
        NTriangulation* toRegina() const;

        /**
         * Dumps the underlying SnapPea data to standard output.
         *
         * This routine should be regarded primarily as a diagnostic tool
         * for investigating how the SnapPea kernel has modified and/or
         * analysed a triangulation.
         *
         * Note that the SnapPea data is written using C-style output
         * (i.e., using the \a stdout file pointer), which may or may not
         * cause unexpected behaviour when used in conjunction with
         * \a std::cout.
         *
         * If this triangulation does not contain any valid SnapPea
         * data, this routine will do nothing.  See isNull() for further
         * details.
         */
        void dump() const;

        /**
         * Saves the underlying triangulation as a native SnapPea file.
         * Like dump(), this routine is provided primarily as a diagnostic
         * tool.
         *
         * For a general export-to-SnapPea method, users are referred to
         * regina::writeSnapPea() instead, which avoids the internal SnapPea
         * conversion entirely and simply writes Regina's native
         * triangulation data in SnapPea's text format.
         *
         * Passing an empty string as the filename will cause the
         * SnapPea data to be written to standard output, just like
         * dump().  See the dump() documentation for caveats when
         * combining the C-style output of this routine with \a std::cout.
         *
         * If this triangulation does not contain any valid SnapPea
         * data, this routine will do nothing.  See isNull() for further
         * details.
         *
         * @param filename the name of the SnapPea file to write.
         */
        void saveAsSnapPea(const char* filename) const;

        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Returns whether or not the SnapPea kernel writes diagnostic
         * messages to standard output.
         *
         * By default such diagnostic messages are disabled.  To enable
         * them, call enableKernelMessages().
         *
         * This routine (which interacts with static data) is thread-safe.
         *
         * @return \c true if and only if diagonstic messages are enabled.
         */
        static bool kernelMessagesEnabled();

        /**
         * Configures whether or not the SnapPea kernel should write
         * diagnostic messages to standard output.
         *
         * By default such diagnostic messages are disabled.
         *
         * This routine (which interacts with static data) is thread-safe.
         *
         * @param enabled \c true if diagnostic messages should be
         * enabled, or \c false otherwise.
         */
        static void enableKernelMessages(bool enabled = true);

        /**
         * Specifies that the SnapPea kernel should not write diagnostic
         * messages to standard output.
         *
         * Calling this routine is equivalent to calling
         * enableKernelMessages(false).
         *
         * Note that diagnostic messages are already disabled by default.
         *
         * This routine (which interacts with static data) is thread-safe.
         */
        static void disableKernelMessages();

    private:
        /**
         * Creates a new raw SnapPea structure mirroring the given Regina
         * triangulation.
         *
         * Note that the tetrahedron and vertex numbers might be changed
         * in the new SnapPea triangulation.
         *
         * The resulting structure should be destroyed by calling
         * free_triangulation() in the SnapPea kernel.
         *
         * Note that not all Regina triangulations can be successfully
         * converted into SnapPea triangulations.  If the conversion is
         * unsuccessful, 0 will be returned.
         *
         * \warning Passing \a allowClosed as \c true can occasionally
         * cause the program to crash!  This is because SnapPea is
         * primarily designed to work with ideal triangulations only.  See
         * the NSnapPeaTriangulation constructor notes for further details.
         *
         * @param tri the Regina triangulation to clone.
         * @param allowClosed \c true if closed triangulations should be
         * considered, or \c false if all closed triangulations should
         * return null.  See the NSnapPeaTriangulation constructor notes
         * for details.
         * @return a corresponding SnapPea structure, or 0 if the
         * conversion was unsuccessful.
         */
        static ::Triangulation* reginaToSnapPea(const NTriangulation& tri,
            bool allowClosed);

        /**
         * Creates a new native Regina triangulation that mirrors the
         * given raw SnapPea triangulation.
         *
         * The resulting triangulation will be newly allocated, and it
         * is the responsibility of the caller of this routine to destroy it.
         *
         * @param tri the SnapPea triangulation to clone.
         * @return a corresponding Regina triangulation, or 0 if
         * \a tri is a null pointer.
         */
        static NTriangulation* snapPeaToRegina(::Triangulation* tri);
};

/*@}*/

// Inline functions for SnapPeaFatalError

inline SnapPeaFatalError::SnapPeaFatalError(
        const char* fromFunction, const char* fromFile) :
        function(fromFunction), file(fromFile) {
}

// Inline functions for NSnapPeaTriangulation

inline bool NSnapPeaTriangulation::isNull() const {
    return (snappeaData == 0);
}

inline NTriangulation* NSnapPeaTriangulation::toRegina() const {
    return snapPeaToRegina(snappeaData);
}

inline void NSnapPeaTriangulation::dump() const {
    saveAsSnapPea("");
}

#endif // EXCLUDE_SNAPPEA

} // namespace regina

#endif

