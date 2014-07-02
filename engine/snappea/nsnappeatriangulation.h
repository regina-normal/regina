
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
 *  \brief Provides access to the SnapPea kernel.
 */

#ifndef __NSNAPPEATRIANGULATION_H
#ifndef __DOXYGEN
#define __NSNAPPEATRIANGULATION_H
#endif

#include "regina-config.h" // For EXCLUDE_SNAPPEA

#include "regina-core.h"
#include "triangulation/ntriangulation.h"
#include <complex>

namespace regina {

// Forward declaration of SnapPea structures.
namespace snappea {
    struct Triangulation;
}

class NMatrixInt;
class NSnapPeaTriangulation;
class NTriangulation;
class NXMLSnapPeaReader;

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
 * Stores information about the SnapPea triangulation packet.
 * See the general PacketInfo template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct PacketInfo<PACKET_SNAPPEATRIANGULATION> {
    typedef NSnapPeaTriangulation Class;
    inline static const char* name() {
        return "SnapPea Triangulation";
    }
};

/**
 * Offers direct access to the SnapPea kernel from within Regina.
 * An object of this class represents a 3-manifold triangulation, stored
 * directly in the SnapPea kernel using SnapPea's internal format.
 *
 * Regarding interaction with the SnapPea kernel:
 *
 * - This class acts as the <b>sole C/C++ conduit</b> between the Regina
 *   calculation engine and Regina's inbuilt version of the SnapPea kernel.
 *   Regina should not interact with the SnapPea kernel at the C/C++ level
 *   other than through this class.
 *
 * - Regina can, however, interact with SnapPy at the Python level.
 *   Regina's version of the SnapPea kernel lives within a separate namespace
 *   (regina::snappea), and so there should be no conflicts between Regina's
 *   copy and SnapPy's copy of the SnapPea kernel (which may even be different
 *   versions).  You can pass triangulations back and forth between Regina and
 *   SnapPy using strings that contain the contents of a SnapPea data file
 *   (see for instance NSnapPeaTriangulation::snapPea()).
 *
 * Regarding the inherited NTriangulation interface:
 *
 * - You can happily query this object using both SnapPea functions (such as
 *   NSnapPeaTriangulation::volume(), and others specific to this class)
 *   and Regina's native triangulation functions (such as
 *   NTriangulation::getHomologyH1(), and others inherited from NTriangulation).
 *   This is because an object of this class stores \e two representations of
 *   the triangulation (SnapPea's and Regina's), which are always kept in sync.
 *
 * - However, you may <b>only edit this object using the SnapPea functions
 *   specific to this class</b> (such as NSnapPeaTriangulation::randomise()).
 *   This is essentially because the synchronisation is one-way only (from
 *   SnapPea to Regina, but not in the other direction).
 *
 * - Any attempt to edit this triangulation via the inherited NTriangulation
 *   interface (for instance, by calling NTriangulation::twoThreeMove()) will
 *   automatically cause this to become a <b>null triangulation</b>,
 *   with no tetrahedra and no SnapPea data at all.
 *
 * Null triangulations appear more generally when Regina is unable to
 * represent data in SnapPea's native format.  You can test for a
 * null triangulation by calling isNull().  Null triangulations can occur
 * for several reasons, such as (but not limited to):
 *
 * - attempting to build a SnapPea triangulation from a Regina triangulation
 *   that is invalid, has boundary faces, or has higher genus vertex
 *   links (none of which SnapPea can handle);
 *
 * - attempting to read a broken SnapPea data file;
 *
 * - attempting to change a SnapPea triangulation using the inherited
 *   NTriangulation interface (as discussed above).
 *
 * There are many places in the SnapPea kernel where SnapPea throws a
 * fatal error.  As of Regina 4.96, these fatal errors are converted
 * into exceptions (subclassed from SnapPeaException), which can be caught
 * and handled politely.
 *
 * Regina uses the variant of the SnapPea kernel that is shipped with
 * SnapPy 2.2 (standard precision), as well as some additional code
 * written explicitly for SnapPy.
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
class REGINA_API NSnapPeaTriangulation : public NTriangulation,
        public NPacketListener {
    REGINA_PACKET(NSnapPeaTriangulation, PACKET_SNAPPEATRIANGULATION)

    public:
        /**
         * Describes the different types of solution that can be found when
         * solving for a hyperbolic structure.
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
        /**
         * A private structure used to cache information about each cusp of
         * the internal SnapPea triangulation.
         */
        struct CuspInfo {
            NVertex* vertex;
                /**< The corresponding vertex of the Regina triangulation. */
            bool complete;
                /**< \c true if the cusp is complete, or \c false if it
                     is filled. */
        };

        regina::snappea::Triangulation* data_;
            /**< The triangulation stored in SnapPea's native format,
                 or 0 if this is a null triangulation. */
        std::complex<double>* shape_;
            /**< The array of tetrahedron shapes, in rectangular form, using a
                 fixed coordinate system (fixed alignment in SnapPea's
                 terminology).  All shapes are with respect to the Dehn filled
                 hyperbolic structure.  If this is a null triangulation, or if
                 the solution type is no_solution or not_attempted, then
                 shape_ will be 0. */
        CuspInfo* cusp_;
            /**< An array that caches information about each cusp of the
                 internal SnapPea triangulation.  If this is a null
                 triangulation then cusp_ will be 0. */
        bool syncing_;
            /**< Set to \c true whilst sync() is being called.  This allows the
                 internal packet listener to distinguish between "legitimate"
                 changes to the inherited NTriangulation via sync(), versus
                 "illegitimate" changes from elsewhere through the inherited
                 NTriangulation interface. */
        static bool kernelMessages_;
            /**< Should the SnapPea kernel write diagnostic messages to
                 standard output? */
        static std::complex<double> zero_;
            /**< The complex number 0.  This is defined as a data member
                 so that shape() can still return a reference even when
                 no tetrahedron shapes have been computed. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Creates a null triangulation, with no internal SnapPea data at all.
         */
        NSnapPeaTriangulation();

        /**
         * Creates a new SnapPea triangulation from the contents of
         * SnapPea data file.  The argument may be the \e name of a
         * SnapPea file, or it may also be the \e contents of a SnapPea file
         * (so the file itself need not actually exist on the filesystem).
         *
         * This routine uses the SnapPea kernel to read the data file,
         * and so all SnapPea-specific information will be preserved
         * (including information that Regina itself does not store,
         * such as peripheral curves).
         *
         * If this operation is successful, this constructor will immediately
         * ask SnapPea to try to find a hyperbolic structure.
         *
         * If this operation fails (e.g., if the given string does not
         * represent a valid SnapPea data file), then this will be a
         * null triangulation.  You can test for this by calling isNull().
         *
         * The triangulation will automatically be given a packet label
         * based on the manifold name stored in the second line of the
         * SnapPea data file.
         *
         * \note This constructor can be used in a Python session to
         * pass data from SnapPy through to Regina's copy of the SnapPea
         * kernel (which is strictly separate from SnapPy's), without
         * losing any of SnapPy's internal information.
         *
         * \warning If (for some reason) you pass a filename that begins
         * with "% Triangulation", then Regina will interpret this as
         * the contents of a SnapPea file (not a filename).
         *
         * \i18n If the given argument is a filename, then this routine makes
         * no assumptions about the \ref i18n "character encoding" used in the
         * filename, and simply passes it through unchanged to low-level C/C++
         * file I/O routines.  This routine assumes that the file \e contents,
         * however, are in UTF-8 (the standard encoding used throughout Regina).
         *
         * @param fileNameOrContents either the name of a SnapPea data
         * file, or the contents of a SnapPea data file (which need not
         * actually exist on the filesystem).
         */
        NSnapPeaTriangulation(const std::string& fileNameOrContents);

        /**
         * Creates a clone of the given SnapPea triangulation.
         * This copy will be independent (i.e., this triangulation will
         * not be affected if \a tri is later changed or destroyed).
         *
         * If \a tri is a null triangulation then this will be a null
         * triangulation also.  See isNull() for further details.
         *
         * @param tri the SnapPea triangulation to clone.
         */
        NSnapPeaTriangulation(const NSnapPeaTriangulation& tri);

        /**
         * Converts the given Regina triangulation to a SnapPea triangulation.
         * This copy will be independent (i.e., this triangulation will
         * not be affected if \a tri is later changed or destroyed).
         *
         * Since Regina works with more general kinds of
         * trianguations than SnapPea, not all Regina triangulations can be
         * represented in SnapPea format.  If the conversion is
         * unsuccessful, this will be marked as a null triangulation.
         * You should always test isNull() to determine whether the
         * conversion was successful.
         *
         * Regarding the conversion:
         *
         * - If \a tri is of the subclass NSnapPeaTriangulation, then
         *   this effectively acts as a copy constructor: all
         *   SnapPea-specific information will be cloned directly
         *   through the SnapPea kernel.  If \a tri is a null SnapPea
         *   triangulation then this copy will be a null triangulation also.
         *
         * - If \a tri is of the parent class NTriangulation, then
         *   Regina will attempt to convert this triangulation to
         *   SnapPea format.  If the conversion is successful, this
         *   constructor will immediately ask SnapPea to try to find a
         *   hyperbolic structure.
         *
         * Regarding peripheral curves: native Regina triangulations do not
         * store or use peripheral curves themselves, and so this constructor
         * makes a default choice during the conversion process.  Specifically:
         *
         * - If solution_type() is geometric_solution or nongeometric_solution,
         *   then on each cusp the meridian and longitude are chosen to be the
         *   (shortest, second shortest) basis, and their orientations
         *   follow the convention used by the \e SnapPy kernel.  Be warned,
         *   however, that this choice might not be unique for some cusp shapes,
         *   and the resolution of such ambiguities might be machine-dependent.
         *
         * - If solution_type() is something else (e.g., degenerate or flat),
         *   or if SnapPea throws a fatal error when attempting to install the
         *   (shortest, second shortest) basis as described above, then Regina
         *   will accept whatever basis SnapPea installs by default.  Be warned
         *   that this default basis may change (and indeed has changed in the
         *   past) across different versions of the SnapPea kernel.
         *
         * SnapPea is designed primarily to work with ideal
         * triangulations only.  Passing closed triangulations can
         * occasionally cause the SnapPea kernel to crash the entire program.
         * Thus by default, closed triangulations are never converted (a null
         * SnapPea triangulation will be created instead).  See the optional
         * argument \a allowClosed for how to change this behaviour.
         *
         * It is possible that the tetrahedron and vertex numbers might be
         * changed in the new SnapPea triangulation.  In particular, if the
         * given Regina triangulation is orientable but not oriented, then you
         * should \e expect these numbers to change.
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

        /*@}*/
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Determines whether this triangulation contains valid SnapPea data.
         *
         * A null SnapPea triangulation can occur (for instance)
         * when converting unusual types of Regina triangulation into
         * SnapPea format, or when reading broken SnapPea data files.
         * See the NSnapPeaTriangulation class notes for details.
         *
         * @return \c true if this is a null triangulation, or \c false
         * if this triangulation contains valid SnapPea data.
         */
        bool isNull() const;

        /**
         * Returns SnapPea's internal name for this triangulation.
         *
         * This is the manifold name stored in the SnapPea kernel, which
         * is typically different from the packet label assigned by Regina.
         *
         * If this is a null triangulation then the empty string will be
         * returned.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.name()</tt>.
         *
         * @return SnapPea's name for this triangulation.
         */
        std::string name() const;

        /*@}*/
        /**
         * \name Hyperbolic Structures
         */
        /*@{*/

        /**
         * Returns the type of solution found when solving for a hyperbolic
         * structure, with respect to the current Dehn filling (if any).
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.solution_type()</tt>.
         *
         * @return the solution type.
         */
        SolutionType solutionType() const;

        /**
         * Computes the volume of the current solution to the hyperbolic
         * gluing equations.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.volume()</tt>.
         *
         * @return the estimated volume of the underlying 3-manifold,
         * or 0 if this is a null triangulation.
         */
        double volume() const;

        /**
         * Computes the volume of the current solution to the hyperbolic
         * gluing equations, and estimates the accuracy of the answer.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.volume(accuracy=True)</tt>.
         *
         * \ifacespython The \a precision argument is not present.
         * Instead, two routines are offered.  The routine \a volume()
         * takes no arguments and returns the volume only, whereas the
         * routine \a volumeWithPrecision() takes no arguments and
         * returns a (\a volume, \a precision) tuple.
         *
         * @param precision used to return an estimate of the number of
         * decimal places of accuracy in the calculated volume.
         *
         * @return the estimated volume of the underlying 3-manifold,
         * or 0 if this is a null triangulation.
         */
        double volume(int& precision) const;

        /**
         * Returns the shape of the given tetrahedron, with respect to
         * the Dehn filled hyperbolic structure.
         *
         * Tetrahedron shapes are given in rectangular form, and using a
         * fixed coordinate system (fixed alignment, in SnapPea's terminology).
         *
         * If this is a null triangulation, or if solutionType() is no_solution
         * or not_attempted (i.e., we did not or could not solve for a
         * hyperbolic structure), then this routine will simply return zero.
         *
         * This routine is fast constant time (unlike in SnapPea, where
         * the corresponding routine \a get_tet_shape takes linear time).
         * Therefore you can happily call this routine repeatedly without a
         * significant performance penalty.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.tetrahedra_shapes(part='rect')[tet]</tt>.
         *
         * @param tet the index of a tetrahedron; this must be between
         * 0 and getNumberOfTetrahedra()-1 inclusive.
         * @return the shape of the given tetrahedron, in rectangular form.
         */
        const std::complex<double>& shape(unsigned tet) const;

        /**
         * Returns a matrix describing Thurston's gluing equations.
         *
         * Each row of this matrix will describe a single equation.
         * The first getNumberOfEdges() rows will list the edge equations,
         * and the following 2 * getNumberOfBoundaryComponents() rows
         * will list the cusp equations.
         *
         * The edge equations will be ordered arbitrarily.  The cusp equations
         * will be presented in pairs ordered by cusp index (as stored by
         * SnapPea); within each pair the meridian equation will appear before
         * the longitude equation.  You can use cuspVertex() to help translate
         * between SnapPea's cusp indices and Regina's vertex indices.
         *
         * The matrix will contain <tt>3 * getNumberOfTetrahedra()</tt> columns.
         * The first three columns represent shape parameters <tt>z</tt>,
         * <tt>1/(1-z)</tt> and <tt>(z-1)/z</tt> for the first tetrahedron;
         * the next three columns represent shape parameters <tt>z</tt>,
         * <tt>1/(1-z)</tt> and <tt>(z-1)/z</tt> for the second tetrahedron,
         * and so on.  By Regina's edge numbering conventions,
         * <tt>z</tt> corresponds to edges 0 and 5 of the tetrahedron,
         * <tt>1/(1-z)</tt> corresponds to edges 1 and 4 of the tetrahedron, and
         * <tt>(z-1)/z</tt> corresponds to edges 2 and 3 of the tetrahedron.
         *
         * More specifically, a row of the form <tt>a b c d e f ...</tt>
         * describes an equation with left hand side
         * <tt>a * log(z0) + b * log(1/(1-z0)) + c * log((z0-1)/z) +
         * d * log(z1) + ... = 2 pi i</tt>,
         * and with right hand side <tt>2 pi i</tt> for an edge equation
         * or 0 for a cusp equation.
         *
         * See also gluingEquationsRect(), which returns the gluing
         * equations in a more streamlined form.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.gluing_equations()</tt>.
         *
         * @return a newly allocated matrix with (\a number_of_rows +
         * \a number_of_cusps) rows and (3 * \a number_of_tetrahedra) columns
         * as described above, or 0 if this is a null triangulation.
         */
        NMatrixInt* gluingEquations() const;

        /**
         * Returns a matrix describing Thurston's gluing equations in a
         * streamlined form.
         *
         * Each row of this matrix will describe a single equation.
         * The rows begin with the edge equations (in arbitrary order)
         * followed by the cusp equations (ordered by cusp index); for
         * precise details see the documentation for gluingEquations(),
         * which uses the same ordering.
         *
         * The matrix will contain <tt>2 * getNumberOfTetrahedra() + 1</tt>
         * columns.  Let \a k = getNumberOfTetrahedra()-1, and suppose the
         * shape parameters for tetrahedra 0, 1, ..., k are
         * \a z0, \a z1, ..., \a zk (here each shape parameter corresponds
         * to edges 0 and 5 of the corresponding tetrahedron).
         * Then a row of the form <tt>a0 a1 ... ak b0 b1 ... bk c</tt>
         * describes the equation
         * <tt>z0^a0 z1^a1 ... zk^ak (1-z0)^b0 (1-z1)^b1 ... (1-zk)^bk = c</tt>,
         * where \a c will always be 1 or -1.
         *
         * See also gluingEquations(), which returns the gluing
         * equations in a more transparent term-by-term form.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.gluing_equations(form='rect')</tt>.
         *
         * @return a newly allocated matrix with (\a number_of_rows +
         * \a number_of_cusps) rows and (2 * \a number_of_tetrahedra + 1)
         * columns as described above, or 0 if this is a null triangulation.
         */
        NMatrixInt* gluingEquationsRect() const;

        /*@}*/
        /**
         * \name Cusps
         */
        /*@{*/

        /**
         * Identifies which vertex of the inherited NTriangulation
         * corresponds to the given SnapPea cusp.
         *
         * When the triangulation is first constructed, SnapPea's cusp \a i
         * will typically correspond to Regina's vertex \a i.  However,
         * if SnapPea manipulates the triangulation (e.g., through randomize()),
         * there is no guarantee that SnapPea and Regina will maintain the
         * same numbering (since in general Regina does not preserve indices
         * of skeletal objects through changes to a triangulation, whereas
         * SnapPea maintains the cusp indices across changes).
         * This routine can be used to detect when SnapPea's and Regina's
         * numbering fall out of sync, and to translate between them
         * if/when this happens.
         *
         * @param cusp the index of a cusp according to SnapPea; this must be
         * between 0 and getNumberOfBoundaryComponents()-1 inclusive.
         * @return the corresponding vertex of the triangulation according
         * to Regina.
         */
        NVertex* cuspVertex(unsigned cusp) const;

        /**
         * Determines whether the given cusp is complete or filled.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.cusp_info('is_complete')[cusp]</tt>.
         *
         * @param cusp the index of a cusp according to SnapPea; this must be
         * between 0 and getNumberOfBoundaryComponents()-1 inclusive.
         * @return \c true if the cusp is complete, or \c false if it is filled.
         */
        bool cuspComplete(unsigned cusp) const;

        /**
         * Returns a matrix for computing boundary slopes of
         * spun-normal surfaces at the cusps of the triangulation.  This
         * matrix includes a pair of rows for each cusp in the triangulation:
         * one row for determining the algebraic intersection number
         * with the meridian, followed by one row for determining the
         * algebraic intersection number with the longitude.
         *
         * If the triangulation has more than one cusp, these pairs are
         * ordered by cusp index (as stored by SnapPea).  You can call
         * cuspVertex() to map these to Regina's vertex indices if needed.
         *
         * All cusps are treated as complete.  That is, any Dehn fillings
         * stored in the SnapPea triangulation will be ignored.
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
         * The orientations of the
         * boundary curves of a spun-normal surface are chosen so
         * that \e if meridian and longitude are a positive basis as
         * vieved from the cusp, then as one travels along an oriented
         * boundary curve, the spun-normal surface spirals into the cusp
         * to one's right and down into the manifold to one's left.
         *
         * \snappy This has no corresponding routine in SnapPy.
         *
         * \pre All vertex links in this triangulation must be tori.
         *
         * \warning If this triangulation was constructed from a Regina
         * triangulation (of class NTriangulation), then Regina will have
         * no information about what meridian and longitude the user wishes
         * to use (since Regina does not keep track of peripheral curves
         * on cusps).  Therefore Regina will give boundary slopes relative
         * to the (shortest, second-shortest) basis, as described in the
         * constructor NSnapPeaTriangulation(const NTriangulation&, bool).
         * This might not be what the user expects.
         *
         * @author William Pettersson and Stephan Tillmann
         *
         * @return a newly allocated matrix with (2 * \a number_of_cusps) rows
         * and (3 * \a number_of_tetrahedra) columns as described above,
         * or 0 if this is a null triangulation.
         */
        NMatrixInt* slopeEquations() const;

        /*@}*/
        /**
         * \name Manipulating SnapPea triangulations
         */
        /*@{*/

        /**
         * Constructs the canonical retriangulation of the canonical
         * cell decomposition.
         *
         * The canonical cell decomposition is the one described in
         * "Convex hulls and isometries of cusped hyperbolic 3-manifolds",
         * Jeffrey R. Weeks, Topology Appl. 52 (1993), 127-149.
         * The canonical retriangulation is defined as follows: If the
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
         * \snappy This has no corresponding routine in SnapPy.  This is
         * because, if the canonical cell decomposition is not a triangulation,
         * SnapPy retriangulates in a way that is non-canonical but so
         * that all vertices are ideal.
         *
         * \warning The SnapPea kernel does not always compute the canonical
         * cell decomposition correctly.  However, it guarantees that
         * the manifold that it does compute is homeomorphic to the original.
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
        NSnapPeaTriangulation* canonize() const;

        /**
         * A synonym for canonize(), which constructs the canonical
         * retriangulation of the canonical cell decomposition.
         * See canonize() for further details.
         */
        NSnapPeaTriangulation* canonise() const;

        /**
         * Asks SnapPea to randomly retriangulate this manifold, using
         * local moves that preserve the topology.  This can help when SnapPea
         * is having difficulty finding a hyperbolic structure.
         *
         * This routine uses SnapPea's own internal retriangulation code.
         *
         * After randomizing, this routine will immediately ask SnapPea
         * to try to find a hyperbolic structure.
         *
         * If this is a null SnapPea triangulation, this routine does nothing.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.randomize()</tt>.
         */
        void randomize();

        /**
         * A synonym for randomize(), which asks SnapPea to randomly
         * retriangulate this manifold.  See randomize() for further details.
         */
        void randomise();

        /*@}*/
        /**
         * \name Deprecated routines
         */
        /*@{*/

        /**
         * Deprecated routine that verifies whether the tetrahedron face
         * gluings from this SnapPea triangulation match the given Regina
         * triangulation precisely.  This is useful if you need to test
         * whether SnapPea has relabelled and/or retriangulated.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Simply call NTriangulation::isIdenticalTo() instead
         * (though note that there is a slight change in behaviour:
         * isIdenticalTo() will consider a null SnapPea triangulation
         * identical to an empty Regina triangulation).
         */
        bool verifyTriangulation(const NTriangulation& triangulation) const;

        /**
         * Deprecated routine to create a new Regina triangulation that
         * mirrors the internal SnapPea structure.
         *
         * The resulting triangulation will be newly created, and it is the
         * responsibility of the caller of this routine to eventually delete it.
         * The packet label of the new triangulation will be the internal
         * SnapPea manifold name, as returned by name().
         *
         * \deprecated This routine will be removed in a future version of
         * Regina.  Instead, you should simply call the NTriangulation copy
         * constructor (since NSnapPeaTriangulation is a subclass of
         * NTriangulation).
         */
        NTriangulation* toRegina() const;

        /**
         * Deprecated routine that dumps the full internal SnapPea data to
         * standard output, using SnapPea's file format.
         *
         * Note that the SnapPea data is written using C-style output
         * (i.e., using the \a stdout file pointer), which may or may not
         * cause unexpected behaviour when used in conjunction with
         * \a std::cout.
         *
         * If this triangulation does not contain any valid SnapPea data
         * (i.e., isNull() returns \c true), then this routine will do nothing.
         *
         * \deprecated This routine will be removed in a future version of
         * Regina.  Instead you should simply print the string returned from
         * snapPea().
         */
        void dump() const;

        /**
         * Deprecated routine that saves the underlying triangulation as
         * a native SnapPea data file.
         *
         * \deprecated This routine has been renamed as saveSnapPea().
         * This old routine behaves exactly as in the past, but the new
         * routine has a slight change in behaviour: it now requires a
         * non-empty filename.  To send data to standard output (as in
         * the old routine), you can simply print the string returned from
         * snapPea().  See the saveSnapPea() documentation for further details.
         */
        void saveAsSnapPea(const char* filename) const;

        /*@}*/
        /**
         * \name SnapPea kernel messages
         */
        /*@{*/

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

        /*@}*/
        /**
         * \name SnapPea Input and Output
         */
        /*@{*/

        virtual std::string snapPea() const;
        virtual void snapPea(std::ostream& out) const;
        virtual bool saveSnapPea(const char* filename) const;

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;

        virtual bool dependsOnParent() const;
        static NXMLPacketReader* getXMLReader(NPacket* parent,
            NXMLTreeResolver& resolver);

        /*@}*/
        /**
         * \name Packet Listener Interface
         */
        /*@{*/

        virtual void packetWasChanged(NPacket* packet);

        /*@}*/

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Synchronises the inherited NTriangulation data so that the
         * tetrahedra and their gluings match the raw SnapPea data.
         * Also refreshes the internal array of tetrahedron shapes.
         */
        void sync();

        /**
         * Resets the internal SnapPea data to the given SnapPea triangulation.
         * This object will take ownership of the given SnapPea data.
         *
         * @param data the new SnapPea data for this object.
         */
        void reset(regina::snappea::Triangulation* data);

    friend class regina::NXMLSnapPeaReader;
};

/*@}*/

// Inline functions for SnapPeaFatalError

inline SnapPeaFatalError::SnapPeaFatalError(
        const char* fromFunction, const char* fromFile) :
        function(fromFunction), file(fromFile) {
}

// Inline functions for NSnapPeaTriangulation

inline NSnapPeaTriangulation::NSnapPeaTriangulation() :
        data_(0), shape_(0), cusp_(0), syncing_(false) {
    listen(this);
}

inline bool NSnapPeaTriangulation::isNull() const {
    return (data_ == 0);
}

inline const std::complex<double>& NSnapPeaTriangulation::shape(unsigned tet)
        const {
    return (shape_ ? shape_[tet] : zero_);
}

inline NVertex* NSnapPeaTriangulation::cuspVertex(unsigned cusp) const {
    return (cusp_ ? cusp_[cusp].vertex : 0);
}

inline bool NSnapPeaTriangulation::cuspComplete(unsigned cusp) const {
    return (cusp_ ? cusp_[cusp].complete : false);
}

inline bool NSnapPeaTriangulation::dependsOnParent() const {
    return false;
}

inline NSnapPeaTriangulation* NSnapPeaTriangulation::canonise() const {
    return canonize();
}

inline void NSnapPeaTriangulation::randomise() {
    randomize();
}

inline NPacket* NSnapPeaTriangulation::internalClonePacket(NPacket*) const {
    return new NSnapPeaTriangulation(*this);
}

#endif // EXCLUDE_SNAPPEA

} // namespace regina

#endif

