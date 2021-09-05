
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

/*! \file snappea/snappeatriangulation.h
 *  \brief Provides access to the SnapPea kernel.
 */

#ifndef __REGINA_SNAPPEATRIANGULATION_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEATRIANGULATION_H
#endif

#include "regina-core.h"
#include "triangulation/dim3.h"
#include <complex>
#include <functional>

// Forward declaration of SnapPea structures.
namespace regina::snappea {
    struct Triangulation;
}

namespace regina {

class SnapPeaTriangulation;
class XMLSnapPeaReader;
class XMLLegacySnapPeaReader;

template <typename, bool> class Matrix;
typedef Matrix<Integer, true> MatrixInt;

/**
 * \addtogroup snappea SnapPea Triangulations
 * Interfaces for accessing the SnapPea kernel
 * @{
 */

/**
 * An exception that is thrown when the SnapPea kernel encounters a
 * fatal error.
 *
 * Details of the error can be accessed through the inherited member
 * function what().
 *
 * \ifacespython Not present.
 */
class SnapPeaFatalError : public std::runtime_error {
    public:
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

        /**
         * Creates a new copy of the given exception.
         */
        SnapPeaFatalError(const SnapPeaFatalError&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        SnapPeaFatalError& operator = (const SnapPeaFatalError&) noexcept =
            default;
};

/**
 * An exception that is thrown when the SnapPea kernel finds that all
 * available memory has been exhausted.
 *
 * Details of the error can be accessed through the member function what().
 *
 * \ifacespython Not present.
 */
class SnapPeaMemoryFull : public std::exception {
    public:
        /**
         * Creates a new exception.
         */
        SnapPeaMemoryFull() noexcept = default;

        /**
         * Creates a new copy of the given exception.
         */
        SnapPeaMemoryFull(const SnapPeaMemoryFull&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        SnapPeaMemoryFull& operator = (const SnapPeaMemoryFull&) noexcept =
            default;

        /**
         * Returns a human-readable description of the error that occurred.
         *
         * @return a description of the error.
         */
        virtual const char* what() const noexcept override {
            return "SnapPea reports that memory is full";
        }
};

/**
 * Represents a single cusp of a SnapPea triangulation.
 * See the SnapPeaTriangulation class for further details.
 *
 * Cusp objects should be considered temporary only.  They are preserved
 * if you change the fillings (via SnapPeaTriangulation::fill()
 * or SnapPeaTriangulation::unfill()).  However, if you change the SnapPea
 * triangulation itself (e.g., via randomise()), then all cusp objects will
 * be deleted and replaced with new ones (using fresh data re-fetched from
 * the SnapPea kernel).
 *
 * Cusps do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the SnapPeaTriangulation to which they belong.
 */
class Cusp : public ShortOutput<Cusp> {
    private:
        Vertex<3>* vertex_;
            /**< The corresponding vertex of the Regina triangulation. */
        int m_;
            /**< The first (meridian) filling coefficient, or 0 if this
                 cusp is complete. */
        int l_;
            /**< The second (longitude) filling coefficient, or 0 if this
                 cusp is complete. */

    public:
        /**
         * Returns the corresponding vertex of the Regina triangulation
         * (i.e., of the Triangulation<3> structure that is inherited by
         * SnapPeaTriangulation).
         *
         * Note that cusp and vertex indexing might not be in sync; that is,
         * SnapPea's <tt>cusp(i)</tt> need not correspond to Regina's
         * <tt>vertex(i)</tt>.
         *
         * This routine can be used to detect if/when cusp numbering
         * and vertex numbering fall out of sync, and to translate
         * between them if/when this happens.
         */
        Vertex<3>* vertex() const;

        /**
         * Returns whether this cusp is complete.
         *
         * \snappy In SnapPy, this field corresponds to querying
         * <tt>Manifold.cusp_info('is_complete')[cusp_number]</tt>.
         *
         * @return \c true if this cusp is complete, or \c false if it is
         * filled.
         */
        bool complete() const;

        /**
         * Returns the first (meridian) filling coefficient on this cusp,
         * or 0 if this cusp is complete.
         *
         * \snappy In SnapPy, this field corresponds to querying
         * <tt>Manifold.cusp_info('filling')[cusp_number][0]</tt>.
         *
         * @return the first filling coefficient.
         */
        int m() const;

        /**
         * Returns the second (longitude) filling coefficient on this cusp,
         * or 0 if this cusp is complete.
         *
         * \snappy In SnapPy, this field corresponds to querying
         * <tt>Manifold.cusp_info('filling')[cusp_number][1]</tt>.
         *
         * @return the second filling coefficient.
         */
        int l() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Make this class non-copyable.
        Cusp(const Cusp&) = delete;
        Cusp& operator = (const Cusp&) = delete;

    private:
        /**
         * A default constructor that performs no initialisation whatsoever.
         */
        Cusp() = default;

    friend class SnapPeaTriangulation;
        /**< Allow access to private members. */
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
 *   (see for instance SnapPeaTriangulation::snapPea()).
 *
 * Regarding the inherited Triangulation<3> interface:
 *
 * - You can happily query this object using both SnapPea functions (such as
 *   SnapPeaTriangulation::volume(), and others specific to this class)
 *   and Regina's native triangulation functions (such as
 *   Triangulation<3>::homology(), and others inherited from Triangulation<3>).
 *   This is because an object of this class stores \e two representations of
 *   the triangulation (SnapPea's and Regina's), which are always kept in sync.
 *
 * - However, you may <b>only edit this object using the SnapPea functions
 *   specific to this class</b> (such as SnapPeaTriangulation::randomise()).
 *   This is essentially because the synchronisation is one-way only (from
 *   SnapPea to Regina, but not in the other direction).
 *
 * - Any attempt to edit this triangulation via the inherited Triangulation<3>
 *   interface (for instance, by calling Triangulation<3>::pachner()) will
 *   automatically cause this to become a <b>null triangulation</b>,
 *   with no tetrahedra and no SnapPea data at all.
 *
 * - In particular, if you wish to swap the contents of two SnapPea
 *   triangulations, you \e must cast both to SnapPeaTriangulation before
 *   calling swap().  If either argument is presented as the parent class
 *   Triangulation<3> then the inherited Triangulation<3>::swap() will be
 *   called instead, which will (as above) nullify both SnapPea triangulations.
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
 *   Triangulation<3> interface (as discussed above);
 *
 * - attempting to import a SnapPea triangulation that uses unsupported
 *   (e.g., non-integer or non-coprime) filling coefficients, as discussed
 *   below).
 *
 * Regarding fillings:  SnapPea can store and manipulate Dehn fillings on
 * cusps, and the SnapPeaTriangulation class respects these where it can (but
 * with restrictions on the possible filling coefficients; see below).
 * However, Regina's own Triangulation<3> class knows nothing about fillings
 * at all.  Therefore:
 *
 * - Routines inherited through the Triangulation<3> interface will ignore
 *   fillings completely (so, for instance, homology() will return the
 *   first homology of the unfilled manifold, even if SnapPea has
 *   designated fillings on the cusps).
 *
 * - Routines that are defined here in the SnapPeaTriangulation class
 *   are generally aware of fillings (so, for instance, homologyFilled()
 *   will return the first homology of the filled manifold).  See the
 *   individual notes for each member function for details on how it
 *   handles fillings.
 *
 * For now, SnapPeaTriangulation only supports the following types of filling
 * coefficients: on orientable cusps the filling coefficients must be
 * coprime integers, and non non-orientable cusps the filling coefficients
 * must be the integers (±1, 0).  Any attempt to import a triangulation from
 * a SnapPea file with filling coefficients outside these requirements will
 * result in a null triangulation (as discussed above).
 *
 * There are many places in the SnapPea kernel where SnapPea throws a
 * fatal error.  As of Regina 4.96, these fatal errors are converted
 * into exceptions (subclassed from SnapPeaException), which can be caught
 * and handled politely.
 *
 * This class implements the C++ Swappable requirement by providing member
 * and global swap() functions.  However, like all packet types, it does
 * \e not implement a move constructor or move assignment, since this would
 * interfere with the structure of the packet tree.
 *
 * Regina uses the variant of the SnapPea kernel that is shipped with
 * SnapPy (standard precision), as well as some additional code
 * written explicitly for SnapPy.  The header regina-config.h includes a
 * macro SNAPPY_VERSION that gives the exact version of SnapPy that is
 * bundled into Regina, and you can query this at runtime by calling
 * Regina's function regina::versionSnapPy().
 *
 * The SnapPea kernel was originally written by Jeff Weeks.  SnapPy,
 * where this kernel is now maintained, is primarily developed by Marc Culler,
 * Nathan Dunfield and Matthias Goerner, with contributions from many people.
 * SnapPy and the corresponding SnapPea kernel are distributed under the terms
 * of the GNU General Public License, version 2 or any later version, as
 * published by the Free Software Foundation.
 *
 * See http://snappy.computop.org/ for further information on
 * SnapPea and its successor SnapPy.
 */
class SnapPeaTriangulation : public Triangulation<3>, public PacketListener {
    REGINA_PACKET(SnapPeaTriangulation, PACKET_SNAPPEATRIANGULATION,
        "SnapPea Triangulation")

    public:
        /**
         * Describes the different types of solution that can be found when
         * solving for a hyperbolic structure.
         *
         * This enumeration is identical to SnapPea's own SolutionType enum;
         * it is declared again here because Regina code should not in
         * general interact directly with the SnapPea kernel.  Values may be
         * freely converted between the two enumeration types by simple
         * assignment and/or typecasting.
         *
         * \warning This enumeration must always be kept in sync with
         * SnapPea's own SolutionType enumeration.
         */
        typedef enum {
            not_attempted,
                /**< A solution has not been attempted. */
            geometric_solution,
                /**< All tetrahedra are positively oriented. */
            nongeometric_solution,
                /**< The overall volume is positive, but some tetrahedra are
                     flat or negatively oriented.  No tetrahedra have
                     shape 0, 1 or infinity. */
            flat_solution,
                /**< All tetrahedra are flat, but none have shape 0, 1 or
                     infinity. */
            degenerate_solution,
                /**< At least one tetrahedron has shape 0, 1 or infinity. */
            other_solution,
                /**< The volume is zero or negative, but the solution is
                     neither flat nor degenerate. */
            no_solution,
                /**< The gluing equations could not be solved. */
            externally_computed
                /**< Tetrahedron shapes were inserted into the triangulation. */
        } SolutionType;

        /**
         * Indicates which types of covers should be enumerated when calling
         * enumerateCovers().
         *
         * This enumeration is identical to SnapPea's PermutationSubgroup enum,
         * though the values here are named differently.  The enumeration
         * is declared again here because Regina code should not in
         * general interact directly with the SnapPea kernel.  Values may be
         * freely converted between the two enumeration types by simple
         * assignment and/or typecasting.
         *
         * \warning This enumeration must always be kept in sync with
         * SnapPea's PermutationSubgroup enumeration.
         */
        typedef enum {
            cyclic_covers,
                /**< Indicates that only cyclic covers should be enumerated.
                     This corresponds to the SnapPea constant
                     \a permutation_subgroup_Zn. */
            all_covers
                /**< Indicates that all covers should be enumerated.
                     This corresponds to the SnapPea constant
                     \a permutation_subgroup_Sn. */
        } CoverEnumerationType;

        /**
         * Indicates the different types of covers that can be produced
         * by enumerateCovers().
         *
         * This enumeration is identical to SnapPea's own CoveringType enum;
         * it is declared again here because Regina code should not in
         * general interact directly with the SnapPea kernel.  Values may be
         * freely converted between the two enumeration types by simple
         * assignment and/or typecasting.
         *
         * \warning This enumeration must always be kept in sync with
         * SnapPea's own CoveringType enumeration.
         */
        typedef enum {
            unknown_cover,
                /**< Indicates that SnapPea has not yet computed the covering
                     type. */
            irregular_cover,
                /**< Indicates a covering where there exist two lifts of a
                     point in the base manifold with no covering transformation
                     that takes one to the other. */
            regular_cover,
                /**< Indicates a covering that is not cyclic, and where for
                     any two lifts of a point in the base manfiold, there is a
                     covering transformation taking one to the other. */
            cyclic_cover
                /**< Indicates a cyclic covering. */
        } CoverType;

    private:
        regina::snappea::Triangulation* data_;
            /**< The triangulation stored in SnapPea's native format,
                 or \c nullptr if this is a null triangulation. */
        std::complex<double>* shape_;
            /**< The array of tetrahedron shapes, in rectangular form, using a
                 fixed coordinate system (fixed alignment in SnapPea's
                 terminology).  All shapes are with respect to the Dehn filled
                 hyperbolic structure.  If this is a null triangulation, or if
                 the solution type is no_solution or not_attempted, then
                 shape_ will be \c nullptr. */
        Cusp* cusp_;
            /**< An array that caches information about each cusp of the
                 internal SnapPea triangulation.  If this is a null
                 triangulation then cusp_ will be \c nullptr. */
        unsigned filledCusps_;
            /**< The number of cusps that are currently filled. */

        mutable std::optional<GroupPresentation> fundGroupFilled_;
            /**< The fundamental group of the filled triangulation.
                 This is std::nullopt if it has not yet been computed. */
        mutable std::variant<bool, AbelianGroup> h1Filled_;
            /**< The first homology group of the filled triangulation,
                 or a boolean if this unknown.  The boolean will be
                 \c false if if the computation has not yet been attempted,
                 or \c true if the computation failed (which means SnapPea
                 overflowed when building the matrix of relations). */

        bool syncing_;
            /**< Set to \c true whilst sync() is being called.  This allows the
                 internal packet listener to distinguish between "legitimate"
                 changes to the inherited Triangulation<3> via sync(), versus
                 "illegitimate" changes from elsewhere through the inherited
                 Triangulation<3> interface. */

        static bool kernelMessages_;
            /**< Should the SnapPea kernel write diagnostic messages to
                 standard output? */
        static constexpr std::complex<double> zero_ { 0, 0 };
            /**< The complex number 0.  This is defined as a (static) data
                 member so that shape() can still return a reference even when
                 no tetrahedron shapes have been computed. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Creates a null triangulation, with no internal SnapPea data at all.
         */
        SnapPeaTriangulation();

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
        SnapPeaTriangulation(const std::string& fileNameOrContents);

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
        SnapPeaTriangulation(const SnapPeaTriangulation& tri);

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
         * - If \a tri is of the subclass SnapPeaTriangulation, then
         *   this effectively acts as a copy constructor: all
         *   SnapPea-specific information will be cloned directly
         *   through the SnapPea kernel.  If \a tri is a null SnapPea
         *   triangulation then this copy will be a null triangulation also.
         *
         * - If \a tri is of the parent class Triangulation<3>, then
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
         *   then on each torus cusp the meridian and longitude are chosen to
         *   be the (shortest, second shortest) basis, and their orientations
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
         * Regarding internal vertices (i.e., vertices whose links are spheres):
         * SnapPea is designed to work only with triangulations where
         * every vertex is ideal.  As a result:
         *
         * - You may pass a closed triangulation to this constructor, but
         *   SnapPea will automatically convert this into a filling of a
         *   cusped manifold, using an ideal triangulation.
         *
         * - You may also pass a triangulation that uses both ideal and
         *   internal vertices.  In this case, SnapPea will retriangulate
         *   the manifold so that it uses ideal vertices only.
         *
         * Even if SnapPea does not retriangulate the manifold (for the
         * reasons described above), it is possible that the tetrahedron and
         * vertex numbers might be changed in the new SnapPea triangulation.
         * In particular, if the given Regina triangulation is orientable but
         * not oriented, then you should \e expect these numbers to change.
         *
         * @param tri the Regina triangulation to clone.
         * @param ignored a legacy parameter that is now ignored.
         * (This argument was once required if you wanted to pass a
         * closed triangluation to SnapPea.)
         */
        SnapPeaTriangulation(const Triangulation<3>& tri, bool ignored = false);

        /**
         * Creates a new ideal SnapPea triangulation representing the
         * complement of the given link in the 3-sphere.
         *
         * This is the same triangulation that would be produced by calling
         * Link::complement() to build a native Triangulation<3>, and then
         * passing the resulting triangulation to the constructor
         * SnapPeaTriangulation(const Triangulation<3>&).
         *
         * Be aware that the peripheral curves are chosen by SnapPea
         * according to the method outlined in the constructor
         * SnapPeaTriangulation(const Triangulation<3>&); in particular,
         * this is done with no knowledge of the original link diagram.
         *
         * Also, be aware that this could be a null SnapPea triangulation
         * (if, for whatever reason, SnapPea is unable to do the conversion
         * from the intermediate native Regina triangulation).  As always,
         * you can (and should) test this by calling isNull().
         *
         * @param link the link whose complement we should build.
         */
        SnapPeaTriangulation(const Link& link);

        /**
         * Destroys this triangulation.  All internal SnapPea data will
         * also be destroyed.
         */
        ~SnapPeaTriangulation();

        /*@}*/
        /**
         * \name Tetrahedra
         */
        /*@{*/

        /**
         * Swaps the contents of this and the given SnapPea triangulation.
         *
         * All information contained in this triangulation, including both
         * Regina's and SnapPea's internal data as well as all cached
         * properties, will be moved to \a other.  Likewise, all information
         * contained in \a other will be moved to this triangulation.
         *
         * In particular, any pointers or references to Tetrahedron<3>,
         * Face<3, subdim> and/or Cusp objects will remain valid.
         *
         * The structure of the packet tree will \e not be swapped:
         * both packets being swapped will remain with their original parents,
         * and their original children will remain with them.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * \warning If you wish to swap the contents of two SnapPea
         * triangulations, you \e must cast both to SnapPeaTriangulation
         * before calling swap().  If either argument is presented as the
         * parent class Triangulation<3>, then the Triangulation<3>
         * swap() will be called instead; the result will be that (just
         * like when you call any of the Triangulation<3> edit routines)
         * both SnapPea triangulations will be reset to null triangulations.
         * See the SnapPeaTriangulation class notes for further discussion.
         *
         * \note This swap function is \e not marked \c noexcept, since it
         * fires packet change events which may in turn call arbitrary
         * code via any registered packet listeners.
         *
         * @param other the SnapPea triangulation whose contents should
         * be swapped with this.
         */
        void swap(SnapPeaTriangulation& other);

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
         * See the SnapPeaTriangulation class notes for details.
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
         * gluing equations.  This will be with respect to the current
         * Dehn filling (if any).
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
         * This will be with respect to the current Dehn filling (if any).
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
         * Determines whether the current solution to the gluing equations
         * has volume approximately zero.  This test is \e not rigorous.
         *
         * This requires (i) the volume itself to be very close to
         * zero in an absolute sense, (ii) the volume to be zero
         * within SnapPea's own estimated precision, and (iii) SnapPea's
         * estimated precision to be sufficiently good in an absolute sense.
         *
         * @return \c true if and only if the volume of the current
         * solution is approximately zero according to the constraints
         * outlined above.
         */
        bool volumeZero() const;

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
         * 0 and size()-1 inclusive.
         * @return the shape of the given tetrahedron, in rectangular form.
         */
        const std::complex<double>& shape(unsigned tet) const;

        /**
         * Returns the minimum imaginary part found amongst all tetrahedron
         * shapes, with respect to the Dehn filled hyperbolic structure.
         *
         * Tetrahedron shapes are given in rectangular form using a fixed
         * coordinate system, as described in the documentation for shape().
         *
         * If this is a null triangulation, or if solutionType() is no_solution
         * or not_attempted (i.e., we did not or could not solve for a
         * hyperbolic structure), then this routine will simply return zero.
         *
         * \snappy This has no corresponding routine in SnapPy,
         * though the information is easily acessible via
         * <tt>Manifold.tetrahedra_shapes(part='rect')</tt>.
         *
         * @return the minimum imaginary part amongst all tetrahedron shapes.
         */
        double minImaginaryShape() const;

        /**
         * Returns a matrix describing Thurston's gluing equations.
         * This will be with respect to the current Dehn filling (if any).
         *
         * Each row of this matrix will describe a single equation.
         * The first countEdges() rows will list the edge equations,
         * and the following 2 * countCompleteCusps() + countFilledCusps()
         * rows will list the cusp equations.
         *
         * The edge equations will be ordered arbitrarily.  The cusp equations
         * will be presented in pairs ordered by cusp index (as stored by
         * SnapPea); within each pair the meridian equation will appear before
         * the longitude equation.  The Cusp::vertex() method (which
         * is accessed through the cusp() routine) can help translate
         * between SnapPea's cusp numbers and Regina's vertex numbers.
         *
         * The matrix will contain <tt>3 * size()</tt> columns.
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
         * \pre This is not a null triangulation.
         *
         * @return a matrix with (\a number_of_rows + \a number_of_cusps) rows
         * and (3 * \a number_of_tetrahedra) columns as described above.
         */
        MatrixInt gluingEquations() const;

        /**
         * Returns a matrix describing Thurston's gluing equations in a
         * streamlined form.  This will be with respect to the current
         * Dehn filling (if any).
         *
         * Each row of this matrix will describe a single equation.
         * The rows begin with the edge equations (in arbitrary order)
         * followed by the cusp equations (ordered by cusp index); for
         * precise details see the documentation for gluingEquations(),
         * which uses the same ordering.
         *
         * The matrix will contain <tt>2 * size() + 1</tt>
         * columns.  Let \a k = size()-1, and suppose the
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
         * \pre This is not a null triangulation.
         *
         * @return a matrix with (\a number_of_rows + \a number_of_cusps) rows
         * and (2 * \a number_of_tetrahedra + 1) columns as described above.
         */
        MatrixInt gluingEquationsRect() const;

        /*@}*/
        /**
         * \name Cusps
         */
        /*@{*/

        /**
         * Returns the total number of cusps (both filled and complete).
         *
         * This returns the same value as the inherited function
         * Triangulation<3>::countBoundaryComponents().
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.num_cusps()</tt>.
         *
         * @return the total number of cusps.
         */
        unsigned countCusps() const;

        /**
         * Returns the total number of complete cusps (that is, unfilled cusps).
         *
         * It is always true that
         * <tt>countCompleteCusps() + countFilledCusps() == countCusps()</tt>.
         *
         * \snappy This has no corresponding routine in SnapPy,
         * though the information is easily acessible via
         * <tt>Manifold.cusp_info('is_complete')</tt>.
         *
         * @return the total number of complete cusps.
         */
        unsigned countCompleteCusps() const;

        /**
         * Returns the total number of filled cusps.
         *
         * It is always true that
         * <tt>countCompleteCusps() + countFilledCusps() == countCusps()</tt>.
         *
         * \snappy This has no corresponding routine in SnapPy,
         * though the information is easily acessible via
         * <tt>Manifold.cusp_info('is_complete')</tt>.
         *
         * @return the total number of filled cusps.
         */
        unsigned countFilledCusps() const;

        /**
         * Returns information about the given cusp of this manifold.
         * This information includes the filling coefficients (if any),
         * along with other combinatorial information.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.cusp_info()[c]</tt>, though the set of
         * information returned about each cusp is different.
         *
         * These Cusp objects should be considered temporary only.  They are
         * preserved if you change the fillings (via fill() or unfill()).
         * However, if you change the SnapPea triangulation itself
         * (e.g., via randomise()), then all cusp objects will be deleted
         * and replaced with new ones (using fresh data re-fetched from
         * the SnapPea kernel).
         *
         * \warning Be warned that cusp \a i might not correspond to vertex
         * \a i of the triangulation.  The Cusp::vertex() method (which
         * is accessed through the cusp() routine) can help translate
         * between SnapPea's cusp numbers and Regina's vertex numbers.
         *
         * @param whichCusp the index of a cusp according to SnapPea;
         * this must be between 0 and countCusps()-1 inclusive.
         * @return information about the given cusp, or \c nullptr if this is a
         * null triangulation.
         */
        const Cusp* cusp(unsigned whichCusp = 0) const;

        /**
         * Assigns a Dehn filling to the given cusp.  This routine will
         * automatically ask SnapPea to update the hyperbolic structure
         * according to the new filling coefficients.
         *
         * The triangulation itself will not change; this routine will
         * simply ask SnapPea to store the given filling coefficients
         * alongside the cusp, to be used in operations such as computing
         * hyperbolic structures.  If you wish to retriangulate to permanently
         * fill the cusp, call filledTriangulation() instead.
         *
         * For orientable cusps only coprime filling coefficients are allowed,
         * and for non-orientable cusps only (±1, 0) fillings are allowed.
         * Although SnapPea can handle more general fillings, Regina
         * will enforce these conditions; if they are not satisfied then
         * it will do nothing and simply return \c false.
         *
         * As a special case however, you may pass (0, 0) as the filling
         * coefficients, in which case this routine will behave
         * identically to unfill().
         *
         * It is possible that, if the given integers are extremely
         * large, SnapPea cannot convert the filling coefficients to its
         * own internal floating-point representation.  If this happens
         * then this routine will again do nothing and simply return \c false.
         *
         * \warning Be warned that cusp \a i might not correspond to vertex
         * \a i of the triangulation.  The Cusp::vertex() method (which
         * is accessed through the cusp() routine) can help translate
         * between SnapPea's cusp numbers and Regina's vertex numbers.
         *
         * @param m the first (meridional) filling coefficient.
         * @param l the second (longitudinal) filling coefficient.
         * @param whichCusp the index of the cusp to fill according to
         * SnapPea; this must be between 0 and countCusps()-1 inclusive.
         * @return \c true if and only if the filling coefficients were
         * accepted (according to the conditions outlined above).
         */
        bool fill(int m, int l, unsigned whichCusp = 0);

        /**
         * Removes any filling on the given cusp.  After removing the filling,
         * this routine will automatically ask SnapPea to update the
         * hyperbolic structure.
         *
         * If the given cusp is already complete, then this routine
         * safely does nothing.
         *
         * \warning Be warned that cusp \a i might not correspond to vertex
         * \a i of the triangulation.  The Cusp::vertex() method (which
         * is accessed through the cusp() routine) can help translate
         * between SnapPea's cusp numbers and Regina's vertex numbers.
         *
         * @param whichCusp the index of the cusp to unfill according to
         * SnapPea; this must be between 0 and countCusps()-1 inclusive.
         */
        void unfill(unsigned whichCusp = 0);

        /**
         * Retriangulates to permanently fill the given cusp.  This uses
         * the current Dehn filling coefficients on the cusp, as set by fill().
         *
         * If this triangulation has more than one cusp to begin with,
         * then the result will be a new instance of SnapPeaTriangulation,
         * and will have one fewer cusp.  Note that the remaining cusps
         * may be reindexed, and all Cusp structures will be destroyed
         * and rebuilt.  Auxiliary information on the remaining cusps (such
         * as filling coefficients and peripheral curves) will be preserved,
         * and SnapPea will automatically attempt to compute a hyperbolic
         * structure on the new triangulation.
         *
         * If this triangulation has only one cusp, then the result will
         * be a new instance of Triangulation<3> (not SnapPeaTriangulation),
         * and will represent a closed manifold.
         *
         * Either way, the result will be a newly allocated triangulation, and
         * it is the responsibility of the caller of this routine to destroy it.
         * The original triangulation (this object) will be left unchanged.
         * If the given cusp is complete or if this is a null triangulation,
         * then this routine will simply return \c nullptr.
         *
         * \warning Be warned that cusp \a i might not correspond to vertex
         * \a i of the triangulation.  The Cusp::vertex() method (which
         * is accessed through the cusp() routine) can help translate
         * between SnapPea's cusp numbers and Regina's vertex numbers.
         *
         * @param whichCusp the index of the cusp to permanently fill according
         * to SnapPea; this must be between 0 and countCusps()-1 inclusive.
         * @return the new filled triangulation or \c nullptr if the filling was
         * not possible (as described above).
         */
        Triangulation<3>* filledTriangulation(unsigned whichCusp) const;

        /**
         * Retriangulates to permanently fill all non-complete cusps.
         * This uses the current Dehn filling coefficients on the cusps,
         * as set by fill().
         *
         * If every cusp of this triangulation is complete, this routine
         * will simply return a new clone of this triangulation.
         *
         * If some but not all cusps are complete, then the result will
         * be a new instance of SnapPeaTriangulation, and will have
         * fewer cusps.  Note that the remaining cusps may be reindexed,
         * and all Cusp structures will be destroyed and rebuilt.  Auxiliary
         * information on the remaining cusps (such as peripheral curves)
         * will be preserved, and SnapPea will automatically attempt to
         * compute a hyperbolic structure on the new triangulation.
         *
         * If all cusps of this triangulation have filling coefficients
         * assigned, then the result will be a new instance of Triangulation<3>
         * (not SnapPeaTriangulation), and will represent a closed manifold.
         *
         * Whatever happens, the result will be a newly allocated triangulation,
         * and it is the responsibility of the caller of this routine to
         * destroy it.  The original triangulation (this object) will be left
         * unchanged.  If this is a null triangulation, then this routine
         * will simply return \c nullptr.
         *
         * @return the new filled triangulation, or \c nullptr if this is a null
         * triangulation.
         */
        Triangulation<3>* filledTriangulation() const;

        /**
         * Returns a matrix for computing boundary slopes of
         * spun-normal surfaces at the cusps of the triangulation.  This
         * matrix includes a pair of rows for each cusp in the triangulation:
         * one row for determining the algebraic intersection number
         * with the meridian, followed by one row for determining the
         * algebraic intersection number with the longitude.
         *
         * If the triangulation has more than one cusp, these pairs are
         * ordered by cusp index (as stored by SnapPea).  You can examine
         * <tt>cusp(cusp_number).vertex()</tt> to map these to Regina's
         * vertex indices if needed.
         *
         * For the purposes of this routine, any fillings on the cusps of
         * this SnapPea triangulation will be ignored.
         *
         * This matrix is constructed so that, if \a M and \a L are the
         * rows for the meridian and longitude at some cusp, then for
         * any spun-normal surface with quadrilateral coordinates
         * \a q, the boundary curves have algebraic intersection number
         * <i>M.q</i> with the meridian and <i>L.q</i> with the longitude.
         * Equivalently, the boundary curves pass <i>L.q</i> times around the
         * meridian and <i>-M.q</i> times around the longitude.
         * To compute these slopes directly from a normal surface, see
         * NormalSurface::boundaryIntersections().
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
         * triangulation (of class Triangulation<3>), then Regina will have
         * no information about what meridian and longitude the user wishes
         * to use (since Regina does not keep track of peripheral curves
         * on cusps).  Therefore Regina will give boundary slopes relative
         * to the (shortest, second-shortest) basis, as described in the
         * constructor SnapPeaTriangulation(const Triangulation<3>&, bool).
         * This might not be what the user expects.
         *
         * @author William Pettersson and Stephan Tillmann
         *
         * @return a matrix with (2 * \a number_of_cusps) rows
         * and (3 * \a number_of_tetrahedra) columns as described above,
         * or no value if this is a null triangulation.
         */
        std::optional<MatrixInt> slopeEquations() const;

        /*@}*/
        /**
         * \name Algebraic Invariants
         */
        /*@{*/

        /**
         * Returns the first homology group of the manifold with respect
         * to the current Dehn filling (if any).  Any complete cusps
         * (without fillings) will be treated as though they had been
         * truncated.
         *
         * This is different from the inherited homology() routine from
         * the parent Triangulation<3> class:
         *
         * - This routine homologyFilled() respects Dehn fillings, and uses
         *   a combination of both SnapPea's and Regina's code to compute
         *   homology groups.  There may be situations in which the SnapPea
         *   kernel cannot perform its part of the computation (see below),
         *   in which case this routine will return \c null.
         *
         * - The inherited homology() routine uses only Regina's code, and
         *   works purely within Regina's parent Triangulation<3> class.
         *   Since Triangulation<3> knows nothing about SnapPea or fillings,
         *   this means that any fillings on the cusps (which are
         *   specific to SnapPea triangulations) will be ignored.
         *   The homology() routine will always return a solution.
         *
         * This routine uses exact arithmetic, and so you are guaranteed
         * that - if it returns a result at all - that this result does
         * not suffer from integer overflows.  Essentially, the process
         * is this:
         *
         * - SnapPea constructs a filled relation matrix using machine integer
         *   arithmetic, but detects overflow (in which case this routine
         *   will return \c null);
         *
         * - Regina then uses exact integer arithmetic to solve for the
         *   abelian group invariants (i.e., Smith normal form).
         *
         * Note that each time the triangulation changes, the homology
         * group will be deleted.  Thus the pointer that is returned
         * from this routine should not be kept for later use.  Instead,
         * homologyFilled() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This is not a null triangulation.
         *
         * @return the first homology group of the filled manifold, or
         * \c null if an overflow occurred inside the SnapPea kernel.
         */
        const AbelianGroup* homologyFilled() const;

        /**
         * Returns the fundamental group of the manifold with respect to
         * the current Dehn filling (if any).  Any complete cusps (without
         * fillings) will be treated as though they had been truncated.
         *
         * This is different from the inherited fundamentalGroup() routine
         * from the parent Triangulation<3> class:
         *
         * - This routine fundamentalGroupFilled() respects Dehn fillings, and
         *   directly uses SnapPea's code to compute fundamental groups.
         *
         * - The inherited fundamentalGroup() routine uses only Regina's code,
         *   and works purely within Regina's parent Triangulation<3> class.
         *   Since Triangulation<3> knows nothing about SnapPea or fillings,
         *   this means that any fillings on the cusps (which are specific
         *   to SnapPea triangulations) will be ignored.
         *
         * Note that each time the triangulation changes, the fundamental
         * group will be deleted.  Thus the reference that is returned
         * from this routine should not be kept for later use.  Instead,
         * fundamentalGroupFilled() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This is not a null triangulation.
         *
         * @param simplifyPresentation \c true if SnapPea should attempt
         * to simplify the group presentation, or \c false if it should
         * be left unsimplified.  Even if \a simplifyPresentation is \c false,
         * this routine will always eliminate adjacent (x, x^-1) pairs.
         * @param fillingsMayAffectGenerators \c true if SnapPea's choice of
         * generators is allowed to depend on the Dehn fillings, or \c false
         * if the choice of generators should be consistent across different
         * fillings.
         * @param minimiseNumberOfGenerators \c true if SnapPea's group
         * simplification code should try to reduce the number of
         * generators at the expense of increasing the total length of
         * the relations, or \c false if it should do the opposite.
         * @param tryHardToShortenRelators \c true if SnapPea's group
         * simplification code should try to reduce the length of the relations
         * by inserting one relation into another.  In general this is a
         * good thing, but it can be very costly for large presentations.
         * @return the fundamental group of the filled manifold.
         */
        const GroupPresentation& fundamentalGroupFilled(
            bool simplifyPresentation = true,
            bool fillingsMayAffectGenerators = true,
            bool minimiseNumberOfGenerators = true,
            bool tryHardToShortenRelators = true) const;

        /**
         * Enumerates connected <i>k</i>-sheeted covers of the underlying
         * manifold.  The number of sheets \a k is passed as the first argument
         * to this function.
         *
         * Regina does this (with the help of SnapPea) by:
         *
         * - enumerating all transitive representations of the fundamental
         *   group into either the symmetric group <i>S(k)</i> or the cyclic
         *   group <i>Z_k</i> (according to the parameter \a type), using the
         *   SnapPea function <tt>find_representations()</tt>; and then
         *
         * - building the cover that corresponds to each representation,
         *   using the SnapPea function <tt>construct_cover()</tt>.
         *
         * If you are only interested in the corresponding index \a k subgroups
         * of the fundamental group and not the triangulated covers themselves,
         * then you may wish to consider the native Regina function
         * GroupPresentation::enumerateCovers() instead.  That function is
         * highly optimised, and should be considerably faster as \a k grows.
         *
         * Each covering space is produced once up to equivalence; here
         * equivalent covers correspond to conjugate representations of
         * the fundamental group.
         *
         * To enumerate \e all <i>k</i>-sheeted covers up to equivalence,
         * set \a type to \a all_covers.  Be warned, however, that this becomes
         * enormously slow as the number of sheets \a k grows.  An alternative
         * is to enumerate only cyclic covers by setting \a type to
         * \a cyclic_covers; this significantly limits the set of covers
         * produced but is also much faster for larger \a k.
         *
         * For each cover that is produced, this routine will call \a action
         * (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a reference to a
         *   SnapPeaTriangulation.  This will be the newly produced cover.
         *   \a action may, if it chooses, modify the triangulation that
         *   it receives (though it must not delete it).  The triangulation
         *   will be destroyed immediately after \a action is called.
         *
         * - The second argument to \a action must be of type
         *   \a SnapPeaTriangulation::CoverType.
         *   This will indicate the type of cover that has been constructed;
         *   see the SnapPeaTriangulation::CoverType documentation for details.
         *   In the same call to enumerateCovers() you may observe different
         *   types of covers being produced (i.e., this value is computed
         *   individually for each cover).  You should, however, never see
         *   the value \a unknown_cover.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * - \a action must not make changes to this original triangulation
         *   (i.e., the SnapPeaTriangulation upon which enumerateCovers()
         *   is being called).
         *
         * Be aware that this routine does the bulk of its work before
         * any covers are produced.  Specifically, because of the design of
         * the SnapPea kernel, this routine first enumerates all requested
         * representations of the fundamental group (which is the slow part),
         * and only then does it construct the cover and call \a action for
         * each representation, one at a time.
         *
         * \pre This is not a null triangulation.
         *
         * \warning If you are enumerating all covers, then the argument
         * \a sheets should be \e very small.
         *
         * \warning The covers that are produced will typically use far more
         * tetrahedra than necessary.  If size is important then you should
         * take a copy using Regina's native type Triangulation<3> and call
         * intelligentSimplify() on that.  Note that you cannot call
         * intelligentSimplify() directly on a SnapPeaTriangulation, since
         * (like all modifying functions inherited from Triangulation<3>)
         * this will nullify the SnapPeaTriangulation.  See the class
         * notes for further explanation.
         *
         * \warning By default, the arguments \a args will be copied (or moved)
         * when they are passed to \a action.  If you need to pass some
         * argument(s) by reference, you must wrap them in std::ref or
         * std::cref.
         *
         * \apinotfinal
         *
         * \ifacespython There are two versions of this function available
         * in Python.  The first form is
         * <tt>enumerateCovers(sheets, type, action)</tt>, which mirrors the
         * C++ function: it takes \a action which may be a pure Python function,
         * it returns the number of covers found, but it does \e not take an
         * addition argument list (\a args).  The second form is
         * <tt>enumerateCovers(sheets, type)</tt>, which returns a Python list
         * containing all of the triangulated covers, each given as a
         * pair (SnapPeaTriangulation, SnapPeaTriangulation::CoverType).
         *
         * @param sheets the number of sheets in the covers to produce
         * (i.e., the number \a k in the description above); this must
         * be a positive integer.
         * @param type indicates whether to enumerate all covers (up to
         * equivalence) or only cyclic covers.
         * @param action a function (or other callable object) to call
         * for each cover that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial triangulation and type arguments.
         * @return the total number of covers found.
         */
        template <typename Action, typename... Args>
        size_t enumerateCovers(int sheets, CoverEnumerationType type,
            Action&& action, Args&&... args) const;

        /*@}*/
        /**
         * \name Manipulating SnapPea triangulations
         */
        /*@{*/

        /**
         * Constructs the canonical cell decomposition, using an
         * arbitrary retriangulation if this decomposition contains
         * non-tetrahedron cells.
         *
         * Any fillings on the cusps of this SnapPea triangulation will be
         * ignored for the purposes of canonisation, though they will be
         * copied over to the new SnapPea triangulation that is returned.
         *
         * The canonical cell decomposition is the one described in
         * "Convex hulls and isometries of cusped hyperbolic 3-manifolds",
         * Jeffrey R. Weeks, Topology Appl. 52 (1993), 127-149.
         *
         * If the canonical cell decomposition is already a triangulation then
         * we leave it untouched, and otherwise we triangulate it arbitrarily.
         * Either way, we preserve the hyperbolic structure.
         *
         * If you need a canonical triangulation (as opposed to an arbitrary
         * retriangulation), then you should call canonise() instead.
         *
         * The resulting triangulation will be newly allocated, and it
         * is the responsibility of the caller of this routine to destroy it.
         *
         * If for any reason either Regina or SnapPea are unable to
         * construct a triangulation of the canonical cell decomposition,
         * then this routine will return \c nullptr.
         *
         * \snappy The function <tt>canonise()</tt> means different
         * things for SnapPy versus the SnapPea kernel.  Here Regina follows
         * the naming convention used in the SnapPea kernel.  Specifically:
         * Regina's routine SnapPeaTriangulation::protoCanonise()
         * corresponds to SnapPy's <tt>Manifold.canonize()</tt> and the
         * SnapPea kernel's <tt>proto_canonize(manifold)</tt>.
         * Regina's routine SnapPeaTriangulation::canonise()
         * corresponds to the SnapPea kernel's <tt>canonize(manifold)</tt>,
         * and is not available through SnapPy at all.
         *
         * \warning The SnapPea kernel does not always compute the canonical
         * cell decomposition correctly.  However, it guarantees that
         * the manifold that it does compute is homeomorphic to the original.
         *
         * @return the canonical triangulation of the canonical cell
         * decomposition, or \c nullptr if this could not be constructed.
         */
        SnapPeaTriangulation* protoCanonise() const;

        /**
         * An alias for protoCanonise(), which constructs the canonical
         * cell decomposition using an arbitrary retriangulation if necessary.
         * See protoCanonise() for further details.
         *
         * This alias is provided as "glue" between the British spelling
         * used throughout Regina and the American spelling used
         * throughout the SnapPea kernel.
         */
        SnapPeaTriangulation* protoCanonize() const;

        /**
         * Constructs the canonical retriangulation of the canonical
         * cell decomposition.
         *
         * Any fillings on the cusps of this SnapPea triangulation will be
         * ignored.  In the resulting canonical triangulation (which is
         * one of Regina's native Triangulation<3> objects, not a SnapPea
         * triangulation), these fillings will be completely forgotten.
         *
         * The canonical cell decomposition is the one described in
         * "Convex hulls and isometries of cusped hyperbolic 3-manifolds",
         * Jeffrey R. Weeks, Topology Appl. 52 (1993), 127-149.
         *
         * If the canonical cell decomposition is already a triangulation
         * then we leave it untouched.  Otherwise, the canonical
         * retriangulation introduces internal (finite) vertices, and
         * is defined as follows:
         *
         * - within each 3-cell of the original complex we introduce a new
         *   internal vertex, and cone the 3-cell boundary to this new vertex;
         *
         * - through each 2-cell of the original complex we insert the
         *   dual edge (joining the two new finite vertices on either side),
         *   and we replace the two cones on either side of the 2-cell with
         *   a ring of tetrahedra surrounding this dual edge.
         *
         * See canonize_part_2.c in the SnapPea source code for details.
         *
         * This routine discards the hyperbolic structure along with all
         * SnapPea-specific information (such as peripheral curves and
         * fillings), and simply returns one of Regina's native triangulations.
         * If you need to preserve SnapPea-specific information then you
         * should call protoCanonise() instead.
         *
         * The resulting triangulation will be newly allocated, and it
         * is the responsibility of the caller of this routine to destroy it.
         *
         * If for any reason either Regina or SnapPea are unable to
         * construct the canonical retriangulation of the canonical cell
         * decomposition, this routine will return \c nullptr.
         *
         * \snappy The function <tt>canonise()</tt> means different
         * things for SnapPy versus the SnapPea kernel.  Here Regina follows
         * the naming convention used in the SnapPea kernel.  Specifically:
         * Regina's routine SnapPeaTriangulation::protoCanonise()
         * corresponds to SnapPy's <tt>Manifold.canonize()</tt> and the
         * SnapPea kernel's <tt>proto_canonize(manifold)</tt>.
         * Regina's routine SnapPeaTriangulation::canonise()
         * corresponds to the SnapPea kernel's <tt>canonize(manifold)</tt>,
         * and is not available through SnapPy at all.
         *
         * \warning The SnapPea kernel does not always compute the canonical
         * cell decomposition correctly.  However, it guarantees that
         * the manifold that it does compute is homeomorphic to the original.
         *
         * @return the canonical triangulation of the canonical cell
         * decomposition, or \c nullptr if this could not be constructed.
         */
        Triangulation<3>* canonise() const;

        /**
         * An alias for canonise(), which constructs the canonical
         * retriangulation of the canonical cell decomposition.
         * See canonise() for further details.
         *
         * This alias is provided as "glue" between the British spelling
         * used throughout Regina and the American spelling used
         * throughout the SnapPea kernel.
         */
        Triangulation<3>* canonize() const;

        /**
         * Asks SnapPea to randomly retriangulate this manifold, using
         * local moves that preserve the topology.  This can help when SnapPea
         * is having difficulty finding a hyperbolic structure.
         *
         * This routine uses SnapPea's own internal retriangulation code.
         *
         * After randomising, this routine will immediately ask SnapPea
         * to try to find a hyperbolic structure.
         *
         * If this is a null SnapPea triangulation, this routine does nothing.
         *
         * \snappy In SnapPy, this routine corresponds to calling
         * <tt>Manifold.randomize()</tt>.
         */
        void randomise();

        /**
         * An alias for randomise(), which asks SnapPea to randomly
         * retriangulate this manifold.  See randomise() for further details.
         *
         * This alias is provided as "glue" between the British spelling
         * used throughout Regina and the American spelling used
         * throughout the SnapPea kernel.
         */
        void randomize();

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

        virtual std::string snapPea() const override;
        virtual void snapPea(std::ostream& out) const override;
        virtual bool saveSnapPea(const char* filename) const override;

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const override;
        virtual void writeTextLong(std::ostream& out) const override;

        /*@}*/
        /**
         * \name Packet Listener Interface
         */
        /*@{*/

        virtual void packetWasChanged(Packet* packet) override;

        /*@}*/

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const override;
        virtual void writeXMLPacketData(std::ostream& out,
            FileFormat format) const override;

    private:
        /**
         * Synchronises the inherited Triangulation<3> data so that the
         * tetrahedra and their gluings match the raw SnapPea data.
         * Also refreshes other internal properties and caches,
         * such as cusps and tetrahedron shapes.
         *
         * A change event will be fired by this routine (this will be a
         * "safe" change event that does not void the triangulation).
         *
         * SnapPea will be asked to recompute the hyperbolic structure
         * only if the current solution type is \a not_attempted.
         */
        void sync();

        /**
         * Like sync(), but assumes that \e only the filling coefficients
         * have changed, and that all other data (such as the tetrahedron
         * gluings, or peripheral curves on the cusps) is unchanged.
         *
         * Essentially this just extends fillingsHaveChanged() to also
         * fire a (safe) change event.
         */
        void syncFillings();

        /**
         * Clears and where necessary refreshes any properties of the
         * triangulation that depend only on the fillings.
         *
         * This routine assumes that the combinatorics of the triangulation
         * have not changed.  It also assumes that SnapPea has already
         * called do_Dehn_filling() (so this routine will not call it again).
         */
        void fillingsHaveChanged();

        /**
         * Copies the given SnapPea triangulation into the given Regina
         * triangulation.  Any fillings on the cusps will be ignored;
         * the Regina triangulation will simply copy the raw
         * combinatorial data of the triangulation, and nothing more.
         *
         * \pre The SnapPea triangulation \a src is non-null.
         * \pre The Regina triangulation \a dest is empty.
         *
         * @param src the SnapPea triangulation to copy from.
         * @param dest the destination Regina triangulation.
         */
        static void fillRegina(regina::snappea::Triangulation* src,
            Triangulation<3>& dest);

        /**
         * Resets the internal SnapPea data to the given SnapPea triangulation.
         * This object will take ownership of the given SnapPea data.
         *
         * @param data the new SnapPea data for this object.
         */
        void reset(regina::snappea::Triangulation* data);

        /**
         * A much less templated version of enumerateCovers().
         *
         * This is identical to enumerateCovers(), except that the type
         * of the action function is now known precisely.  This means
         * that the implementation can be kept out of the main headers.
         */
        size_t enumerateCoversInternal(int sheets, CoverEnumerationType type,
            std::function<void(SnapPeaTriangulation&, CoverType)>&& action)
            const;

    friend class regina::XMLSnapPeaReader;
    friend class regina::XMLLegacySnapPeaReader;
};

/**
 * Swaps the contents of the two given SnapPea triangulations.
 *
 * This global routine simply calls SnapPeaTriangulation::swap(); it is
 * provided so that SnapPeaTriangulation meets the C++ Swappable requirements.
 *
 * See SnapPeaTriangulation::swap() for more details and caveats.
 *
 * \warning If you wish to swap the contents of two SnapPea triangulations,
 * you \e must cast both to SnapPeaTriangulation before calling swap().  If
 * either argument is presented as the parent class Triangulation<3>, then
 * the Triangulation<3> swap() will be called instead; the result will be
 * that (just like when you call any of the Triangulation<3> edit routines)
 * both SnapPea triangulations will be reset to null triangulations.
 * See the SnapPeaTriangulation class notes for further discussion.
 *
 * \note This swap function is \e not marked \c noexcept, since it
 * fires packet change events which may in turn call arbitrary
 * code via any registered packet listeners.
 *
 * @param lhs the triangulation whose contents should be swapped with \a rhs.
 * @param rhs the triangulation whose contents should be swapped with \a lhs.
 */
void swap(SnapPeaTriangulation& lhs, SnapPeaTriangulation& rhs);

/*@}*/

// Inline functions for SnapPeaFatalError

inline SnapPeaFatalError::SnapPeaFatalError(const char* fromFunction,
        const char* fromFile) :
        std::runtime_error((std::string(fromFile) + ": ") + fromFunction) {
}

// Inline functions for Cusp

inline Vertex<3>* Cusp::vertex() const {
    return vertex_;
}

inline bool Cusp::complete() const {
    return (m_ == 0 && l_ == 0);
}

inline int Cusp::m() const {
    return m_;
}

inline int Cusp::l() const {
    return l_;
}

// Inline functions for SnapPeaTriangulation

inline SnapPeaTriangulation::SnapPeaTriangulation() :
        data_(nullptr), shape_(nullptr), cusp_(nullptr),
        filledCusps_(0), h1Filled_(false), syncing_(false) {
    listen(this);
}

inline bool SnapPeaTriangulation::isNull() const {
    return (data_ == nullptr);
}

inline const std::complex<double>& SnapPeaTriangulation::shape(unsigned tet)
        const {
    return (shape_ ? shape_[tet] : zero_);
}

inline unsigned SnapPeaTriangulation::countCusps() const {
    return countBoundaryComponents();
}

inline unsigned SnapPeaTriangulation::countCompleteCusps() const {
    return countBoundaryComponents() - filledCusps_;
}

inline unsigned SnapPeaTriangulation::countFilledCusps() const {
    return filledCusps_;
}

inline const Cusp* SnapPeaTriangulation::cusp(unsigned whichCusp) const {
    return (cusp_ ? cusp_ + whichCusp : nullptr);
}

inline SnapPeaTriangulation* SnapPeaTriangulation::protoCanonize() const {
    return protoCanonise();
}

inline Triangulation<3>* SnapPeaTriangulation::canonize() const {
    return canonise();
}

inline void SnapPeaTriangulation::randomize() {
    randomise();
}

template <typename Action, typename... Args>
inline size_t SnapPeaTriangulation::enumerateCovers(int sheets,
        CoverEnumerationType type, Action&& action, Args&&... args) const {
    return enumerateCoversInternal(sheets, type,
        std::bind(std::forward<Action>(action),
            std::placeholders::_1, std::placeholders::_2,
            std::forward<Args>(args)...));
}

inline Packet* SnapPeaTriangulation::internalClonePacket(Packet*) const {
    return new SnapPeaTriangulation(*this);
}

inline void swap(SnapPeaTriangulation& lhs, SnapPeaTriangulation& rhs) {
    lhs.swap(rhs);
}

} // namespace regina

#endif

