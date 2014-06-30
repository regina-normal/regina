
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
 * This class is designed to act as the sole conduit between the Regina
 * calculation engine and Regina's inbuilt version of the C/C++ SnapPea kernel.
 * Regina should not interact with the SnapPea kernel at the C/C++ level other
 * than through this class.
 *
 * As an alternative, if you have SnapPy installed then you may interact
 * directly with SnapPy via Python.  SnapPy's version of the SnapPea
 * kernel might or might not be compatible with Regina's, but this
 * should not cause problems (since Regina uses namespaces to keep the two
 * versions of the kernel strictly separate).
 *
 * An object of this class represents a 3-manifold triangulation, stored
 * directly in the SnapPea kernel using SnapPea's internal format.
 *
 * This might be a <i>null triangulation</i>, i.e., one that contains no
 * data at all.  This might occur for several reasons; examples include
 * (but are not limited to):
 *
 * - attempting to build a SnapPea triangulation from a Regina triangulation
 *   that is invalid, has boundary faces, or has higher genus vertex
 *   links (none of which SnapPea can handle);
 *
 * - attempting to read a broken SnapPea data file.
 *
 * You can call isNull() to test whether you have a null triangulation.
 *
 * There are many places in the SnapPea kernel where SnapPea throws a
 * fatal error.  As of Regina 4.96, these fatal errors are converted
 * into exceptions (subclassed from SnapPeaException), which can be caught
 * and handled politely.
 *
 * Regina uses the variant of the SnapPea kernel that is shipped with
 * SnapPy 2.1 (standard precision), as well as some additional code
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
        regina::snappea::Triangulation* data_;
            /**< The triangulation stored in SnapPea's native format,
                 or 0 if this is a null triangulation. */
        bool syncing_;
            /**< Set to \c true whilst sync() is being called.  This allows the
                 internal packet listener to distinguish between "legitimate"
                 changes to the inherited NTriangulation via sync(), versus
                 "illegitimate" changes from elsewhere through the inherited
                 NTriangulation interface. */
        static bool kernelMessages_;
            /**< Should the SnapPea kernel write diagnostic messages to
                 standard output? */

    public:
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
         * ask SnapPea to try to find a complete hyperbolic structure.
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
         * TODO: Redocument; deal with the subclass case correctly.
         *
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
         * If the conversion is successful, this constructor will immediately
         * ask SnapPea to try to find a complete hyperbolic structure.
         *
         * On each cusp, the meridian and longitude are chosen to be the
         * (shortest, second shortest) basis, and their orientations
         * follow the convention used by the \e SnapPy kernel.  Be warned,
         * however, that this choice might not be unique for some cusp shapes,
         * and the resolution of such ambiguities might be machine-dependent.
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
         * given triangulation is orientable but not oriented, then you
         * should \e expect these numbers to change.
         *
         * If you are passing another NSnapPeaTriangulation, then the
         * tetrahedron and vertex numbers should be identical for the
         * new SnapPea triangulation.
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
         * @return SnapPea's name for this triangulation.
         */
        std::string name() const;

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
         * The orientations of the
         * boundary curves of a spun-normal surface are chosen so
         * that \e if meridian and longitude are a positive basis as
         * vieved from the cusp, then as one travels along an oriented
         * boundary curve, the spun-normal surface spirals into the cusp
         * to one's right and down into the manifold to one's left.
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
        NSnapPeaTriangulation* canonize() const;

        /**
         * Asks SnapPea to randomly retriangulate this triangulation, using
         * local moves that preserve the topology.  This can help when SnapPea
         * is having difficulty finding a complete hyperbolic structure.
         *
         * This routine uses SnapPea's own internal retriangulation code.
         *
         * After randomizing, this routine will immediately ask SnapPea
         * to try to find a complete hyperbolic structure.
         *
         * If this is a null SnapPea triangulation, this routine does nothing.
         */
        void randomize();

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

        virtual std::string snapPea() const;
        virtual void snapPea(std::ostream& out) const;
        virtual bool saveSnapPea(const char* filename) const;

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

        virtual bool dependsOnParent() const;
        static NXMLPacketReader* getXMLReader(NPacket* parent,
            NXMLTreeResolver& resolver);

        virtual void packetWasChanged(NPacket* packet);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Synchronises the inherited NTriangulation data so that the
         * tetrahedra and their gluings match the raw SnapPea data.
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
        data_(0), syncing_(false) {
    listen(this);
}

inline bool NSnapPeaTriangulation::isNull() const {
    return (data_ == 0);
}

inline bool NSnapPeaTriangulation::dependsOnParent() const {
    return false;
}

inline NPacket* NSnapPeaTriangulation::internalClonePacket(NPacket*) const {
    return new NSnapPeaTriangulation(*this);
}

#endif // EXCLUDE_SNAPPEA

} // namespace regina

#endif

