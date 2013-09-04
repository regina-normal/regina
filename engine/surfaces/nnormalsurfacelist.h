
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

/*! \file surfaces/nnormalsurfacelist.h
 *  \brief Contains a packet representing a collection of normal
 *  surfaces in a 3-manifold.
 */

#ifndef __NNORMALSURFACELIST_H
#ifndef __DOXYGEN
#define __NNORMALSURFACELIST_H
#endif

#include <algorithm>
#include <iterator>
#include <vector>
#include "regina-core.h"
#include "packet/npacket.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/normalflags.h"
#include "surfaces/normalcoords.h"
#include "utilities/memutils.h"
#include "utilities/nthread.h"

namespace regina {

class NTriangulation;
class NMatrixInt;
class NProgressManager;
class NProgressMessage;
class NProgressNumber;
class NXMLPacketReader;
class NXMLNormalSurfaceListReader;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A packet representing a collection of normal surfaces in a 3-manifold.
 * Such a packet must always be a child packet of the triangulation from
 * which the surfaces were obtained.  If this triangulation changes, the
 * information contained in this packet will become invalid.
 *
 * See the NNormalSurfaceVector class notes for details of what to do
 * when introducing a new flavour of coordinate system.
 *
 * Normal surface lists should be created using the routine enumerate(),
 * which is new as of Regina 3.95.
 *
 * \testpart
 *
 * \todo \feature Allow custom matching equations.
 * \todo \feature Allow enumeration with some coordinates explicitly set
 * to zero.
 * \todo \feature Allow generating only closed surfaces.
 * \todo \feature Generate facets of the solution space representing
 * embedded surfaces.
 */
class REGINA_API NNormalSurfaceList : public NPacket {
    public:
        static const int packetType;

        /**
         * Represents standard triangle-quadrilateral coordinates for
         * normal surfaces.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_STANDARD directly.
         */
        static const NormalCoords STANDARD;
        /**
         * Represents standard triangle-quadrilateral-octagon coordinates
         * for octagonal almost normal surfaces.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_AN_STANDARD directly.
         */
        static const NormalCoords AN_STANDARD;
        /**
         * Represents quadrilateral coordinates for normal surfaces.
         * For details, see "Normal surface Q-theory", Jeffrey L. Tollefson,
         * Pacific J. Math. 183 (1998), no. 2, 359--374.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_QUAD directly.
         */
        static const NormalCoords QUAD;
        /**
         * Represents quadrilateral-octagon coordinates for octagonal
         * almost normal surfaces.  For details, see
         * "Quadrilateral-octagon coordinates for almost normal surfaces",
         * Benjamin A. Burton, Experiment. Math. 19 (2010), 285-315.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_AN_QUAD_OCT directly.
         */
        static const NormalCoords AN_QUAD_OCT;

        /**
         * Represents edge weight coordinates for normal surfaces.
         * This flavour is for representation only; surface
         * vectors and lists of this flavour cannot be created.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_EDGE_WEIGHT directly.
         */
        static const NormalCoords EDGE_WEIGHT;
        /**
         * Represents face arc coordinates for normal surfaces.
         * This flavour is for representation only; surface
         * vectors and lists of this flavour cannot be created.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_FACE_ARCS directly.
         */
        static const NormalCoords FACE_ARCS;
        /**
         * Indicates that a list of almost normal surfaces was created
         * using Regina 4.5.1 or earlier, where surfaces with more than
         * one octagon of the same type were stripped out of the final
         * solution set.  As of Regina 4.6 such surfaces are now
         * included in the solution set, since we need them if we
         * wish to enumerate \e all almost normal surfaces (not just
         * the \e vertex almost normal surfaces).
         *
         * This flavour is only used with legacy data files; new vectors
         * and lists of this flavour cannot be created.  The underlying
         * coordinate system is identical to AN_STANDARD.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_AN_LEGACY directly.
         */
        static const NormalCoords AN_LEGACY;
        /**
         * Represents standard triangle-quadrilateral coordinates for
         * transversely oriented normal surfaces.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_ORIENTED directly.
         */
        static const NormalCoords ORIENTED;
        /**
         * Represents quadrilateral coordinates for transversely oriented 
         * normal surfaces.
         *
         * \deprecated Instead of this class constant, you should use
         * the NormalCoords enum value NS_ORIENTED_QUAD directly.
         */
        static const NormalCoords ORIENTED_QUAD;

        class VectorIterator;

    protected:
        std::vector<NNormalSurface*> surfaces;
            /**< Contains the normal surfaces stored in this packet. */
        NormalCoords flavour_;
            /**< Stores which flavour of coordinate system is being
                 used by the normal surfaces in this packet. */
        NormalList which_;
            /**< Indicates which normal surfaces these represent within
                 the underlying triangulation. */
        NormalAlg algorithm_;
            /**< Stores the details of the enumeration algorithm that
                 was used to generate this list.  This might not be the
                 same as the \a algorithmHints flag passed to the
                 corresponding enumeration routine (e.g., if invalid
                 or inappropriate flags were passed). */

    public:
        /**
         * Destroys this list and all the surfaces within.
         */
        virtual ~NNormalSurfaceList();

        /**
         * A unified routine for enumerating various classes of normal
         * surfaces within a given triangulation.
         *
         * The NormalCoords argument allows you to specify an underlying
         * flavour of coordinate system (e.g., standard coordinates,
         * quadrilateral coordinates or almost normal coordinates).
         *
         * The NormalList argument is a combination of flags that
         * allows you to specify exactly which normal surfaces you require.
         * This includes (i) whether you want all vertex surfaces
         * or all fundamental surfaces, which defaults to NS_VERTEX
         * if you specify neither or both; and (ii) whether you want only
         * properly embedded surfaces or you also wish to include
         * immersed and/or singular surfaces, which defaults to
         * NS_EMBEDDED_ONLY if you specify neither or both.
         *
         * The NormalAlg argument is a combination of flags that allows
         * you to control the underlying enumeration algorithm.  These
         * flags are treated as hints only: if your selection of
         * algorithm is invalid, unavailable or unsupported then Regina
         * will choose something more appropriate.  Unless you have
         * some specialised need, the default NS_ALG_DEFAULT (which
         * makes no hints at all) will allow Regina to choose what it
         * thinks will be the most efficient method.
         *
         * The enumerated surfaces will be stored in a new normal
         * surface list, and their representations will be scaled down
         * to use the smallest possible integer coordinates.
         * This normal surface list will be inserted into the packet tree as
         * the last child of the given triangulation.  This triangulation
         * \b must remain the parent of this normal surface list, and must not
         * change while this normal surface list remains in existence.
         *
         * If a progress manager is passed, the normal surface
         * enumeration will take place in a new thread and this routine
         * will return immediately.
         *
         * If no progress manager is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param flavour the flavour of coordinate system to be used.
         * @param which indicates which normal surfaces should be enumerated.
         * @param algHints passes requests to Regina for which specific
         * enumeration algorithm should be used.
         * @param manager a progress manager through which progress will
         * be reported, or 0 if no progress reporting is required.  If
         * non-zero, \a manager must point to a progress manager for
         * which NProgressManager::isStarted() is still \c false.
         * @return the newly created normal surface list.  Note that if
         * a progress manager is passed then this list may not be completely
         * filled when this routine returns.  If a progress manager is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal surface list is created).
         */
        static NNormalSurfaceList* enumerate(NTriangulation* owner,
            NormalCoords flavour,
            NormalList which = NS_LIST_DEFAULT,
            NormalAlg algHints = NS_ALG_DEFAULT,
            NProgressManager* manager = 0);

        /**
         * Deprecated method for enumerating all vertex normal surfaces
         * using the given flavour of coordinate system.
         *
         * Users should now call enumerate(NTriangulation*, NormalCoords,
         * NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details,
         * including all arguments, returns values, preconditions and
         * postconditions.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, flavour, NS_EMBEDDED_ONLY,
         * NS_ALG_DEFAULT, manager)</tt> if \a embeddedOnly is \c true, or
         * <tt>enumerate(owner, flavour, NS_IMMERSED_SINGULAR,
         * NS_ALG_DEFAULT, manager)</tt> if \a embeddedOnly is \c false.
         */
        static NNormalSurfaceList* enumerate(NTriangulation* owner,
            NormalCoords flavour, bool embeddedOnly,
            NProgressManager* manager = 0);

        /**
         * Deprecated method that uses a slow-but-direct procedure to
         * enumerate all embedded vertex normal surfaces in standard
         * coordinates, without using the faster procedure that works
         * via quadrilateral coordinates.
         *
         * Users can still access this slower procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, NS_STANDARD, NS_LIST_DEFAULT,
         * NS_VERTEX_STD_DIRECT)</tt>.
         *
         * \warning This routine is slow, and users will not want to
         * call it unless they have some specialised need.
         *
         * @param owner the triangulation upon which this list of
         * surfaces will be based.
         * @return the newly created normal surface list.
         */
        static NNormalSurfaceList* enumerateStandardDirect(
            NTriangulation* owner);

        /**
         * Deprecated method that uses a slow-but-direct procedure to
         * enumerate all embedded vertex almost normal surfaces in standard
         * almost normal coordinates, without using the faster procedure
         * that works via quadrilateral-octagon coordinates.
         *
         * Users can still access this slower procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, NS_AN_STANDARD, NS_LIST_DEFAULT,
         * NS_VERTEX_STD_DIRECT)</tt>.
         *
         * \warning This routine is slow, and users will not want to
         * call it unless they have some specialised need.
         *
         * @param owner the triangulation upon which this list of
         * surfaces will be based.
         * @return the newly created normal surface list.
         */
        static NNormalSurfaceList* enumerateStandardANDirect(
            NTriangulation* owner);

        /**
         * Deprecated method that enumerates all fundamental normal surfaces
         * in the given triangulation using the primal Hilbert basis algorithm.
         * For details of the algorithm, see B. A. Burton, "Fundamental normal
         * surfaces and the enumeration of Hilbert bases", arXiv:1111.7055,
         * Nov 2011.
         *
         * Users can still access this procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \warning As of Regina 4.94, the \a vtxSurfaces argument is ignored.
         * Future versions of Regina will automatically search existing surface
         * lists in the packet tree for a ready-made list of vertex normal
         * surfaces that can be used.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_EMBEDDED_ONLY,
         * NS_HILBERT_PRIMAL, manager)</tt> if \a embeddedOnly is \c true, or
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_IMMERSED_SINGULAR,
         * NS_HILBERT_PRIMAL, manager)</tt> if \a embeddedOnly is \c false.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param flavour the flavour of coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal surfaces
         * are to be produced, or \c false if immersed and singular
         * normal surfaces are also to be produced; this defaults to \c true.
         * @param vtxSurfaces the set of all \e vertex normal surfaces
         * as enumerated under the same coordinate system and
         * constraints as given here; this may be 0 if unknown.
         * @param manager a progress manager through which progress will
         * be reported, or 0 if no progress reporting is required.  If
         * non-zero, \a manager must point to a progress manager for
         * which NProgressManager::isStarted() is still \c false.
         * @return the newly created normal surface list.  Note that if
         * a progress manager is passed then this list may not be completely
         * filled when this routine returns.  If a progress manager is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal surface list is created).
         */
        static NNormalSurfaceList* enumerateFundPrimal(
            NTriangulation* owner, NormalCoords flavour,
            bool embeddedOnly = true,
            NNormalSurfaceList* vtxSurfaces = 0,
            NProgressManager* manager = 0);

        /**
         * Deprecated method that enumerates all fundamental normal surfaces
         * in the given triangulation using the dual Hilbert basis algorithm.
         * For details of the algorithm, see B. A. Burton, "Fundamental normal
         * surfaces and the enumeration of Hilbert bases", arXiv:1111.7055,
         * Nov 2011.
         *
         * Users can still access this procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_EMBEDDED_ONLY,
         * NS_HILBERT_DUAL, manager)</tt> if \a embeddedOnly is \c true, or
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_IMMERSED_SINGULAR,
         * NS_HILBERT_DUAL, manager)</tt> if \a embeddedOnly is \c false.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param flavour the flavour of coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal surfaces
         * are to be produced, or \c false if immersed and singular
         * normal surfaces are also to be produced; this defaults to \c true.
         * @param manager a progress manager through which progress will
         * be reported, or 0 if no progress reporting is required.  If
         * non-zero, \a manager must point to a progress manager for
         * which NProgressManager::isStarted() is still \c false.
         * @return the newly created normal surface list.  Note that if
         * a progress manager is passed then this list may not be completely
         * filled when this routine returns.  If a progress manager is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal surface list is created).
         */
        static NNormalSurfaceList* enumerateFundDual(
            NTriangulation* owner, NormalCoords flavour,
            bool embeddedOnly = true,
            NProgressManager* manager = 0);

        /**
         * Deprecated method that uses an extremely slow procedure to enumerate
         * all embedded fundamental surfaces in the given triangulation,
         * by running Normaliz over the full (and typically very large)
         * solution cone, and only enforcing embedded constraints (such as
         * the quadrilateral constraints) afterwards.
         *
         * Users can still access this slower procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_EMBEDDED_ONLY,
         * NS_HILBERT_FULLCONE, manager)</tt> if \a embeddedOnly is \c true, or
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_IMMERSED_SINGULAR,
         * NS_HILBERT_FULLCONE, manager)</tt> if \a embeddedOnly is \c false.
         *
         * \warning This routine is extremely slow, and users will not want to
         * call it unless they have some specialised need.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param flavour the flavour of coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal surfaces
         * are to be produced, or \c false if immersed and singular
         * normal surfaces are also to be produced; this defaults to \c true.
         * @return the newly created normal surface list.
         */
        static NNormalSurfaceList* enumerateFundFullCone(
            NTriangulation* owner, NormalCoords flavour,
            bool embeddedOnly = true);

        /**
         * Deprecated method that uses an extremely slow modified
         * Contejean-Devie procedure to enumerate all embedded fundamental
         * surfaces in the given triangulation.  For details of the
         * algorithm, see B. A. Burton, "Fundamental normal surfaces and the
         * enumeration of Hilbert bases", arXiv:1111.7055, Nov 2011.
         *
         * Users can still access this slower procedure if they need to;
         * however, they should do this via enumerate(NTriangulation*,
         * NormalCoords, NormalList, NormalAlg, NProgressManager*) instead.
         * See the documentation for that routine for further details.
         *
         * \deprecated The correct way to access this procedure is to call
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_EMBEDDED_ONLY,
         * NS_HILBERT_CD, manager)</tt> if \a embeddedOnly is \c true, or
         * <tt>enumerate(owner, flavour, NS_FUNDAMENTAL | NS_IMMERSED_SINGULAR,
         * NS_HILBERT_CD, manager)</tt> if \a embeddedOnly is \c false.
         *
         * \warning This routine is extremely slow, and users will not want to
         * call it unless they have some specialised need.
         *
         * @param owner the triangulation upon which this list of normal
         * surfaces will be based.
         * @param flavour the flavour of coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal surfaces
         * are to be produced, or \c false if immersed and singular
         * normal surfaces are also to be produced; this defaults to \c true.
         * @return the newly created normal surface list.
         */
        static NNormalSurfaceList* enumerateFundCD(
            NTriangulation* owner, NormalCoords flavour,
            bool embeddedOnly = true);

        /**
         * Deprecated routine to return the flavour of coordinate system
         * being used by the surfaces stored in this set.
         *
         * \deprecated Users should switch to the identical routine
         * flavour() instead.
         *
         * @return the flavour of coordinate system used.
         */
        NormalCoords getFlavour() const;
        /**
         * Returns the flavour of coordinate system being used by the
         * surfaces stored in this set.
         *
         * @return the flavour of coordinate system used.
         */
        NormalCoords flavour() const;
        /**
         * Returns details of which normal surfaces this list represents
         * within the underlying triangulation.
         *
         * This may not be the same NormalList that was passed to
         * enumerate().  In particular, default values will have be
         * explicitly filled in (such as NS_VERTEX and/or NS_EMBEDDED_ONLY),
         * and invalid and/or redundant values will have been removed.
         *
         * @return details of what this list represents.
         */
        NormalList which() const;
        /**
         * Returns details of the algorithm that was used to enumerate
         * this list.
         *
         * These may not be the same NormalAlg flags that were passed to
         * enumerate().  In particular, default values will have be
         * explicitly filled in, invalid and/or redundant values will have
         * been removed, and unavailable and/or unsupported combinations
         * of algorithm flags will be replaced with whatever algorithm was
         * actually used.
         *
         * @return details of the algorithm used to enumerate this list.
         */
        NormalAlg algorithm() const;
        /**
         * Determines if the flavour of coordinate system being used
         * allows for almost normal surfaces, that is, allows for
         * octagonal discs.
         *
         * @return \c true if and only if almost normal surfaces are
         * allowed.
         */
        bool allowsAlmostNormal() const;
        /**
         * Determines if the flavour of coordinate system being used
         * allows for spun normal surfaces.
         *
         * @return \c true if and only if spun normal surface are
         * supported.
         */
        bool allowsSpun() const;
        /**
         * Determines if the flavour of coordinate system being used
         * allows for transversely oriented normal surfaces.
         *
         * @return \c true if and only if transverse orientations are
         * supported.
         */
        bool allowsOriented() const;
        /**
         * Returns whether this list was constructed to contain only
         * properly embedded surfaces.
         *
         * If this returns \c false, it does not guarantee that immersed
         * and/or singular surfaces are present; it merely indicates
         * that they were not deliberately excluded (for instance, the
         * quadrilateral constraints were not enforced).
         *
         * @return \c true if this list was constructed to contain only
         * properly embedded surfaces, or \c false otherwise.
         */
        bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal surfaces live.
         * 
         * @return the triangulation in which these surfaces live.
         */
        NTriangulation* getTriangulation() const;
        /**
         * Returns the number of surfaces stored in this set.
         *
         * @return the number of surfaces.
         */
        unsigned long getNumberOfSurfaces() const;
        /**
         * Returns the surface at the requested index in this set.
         *
         * @param index the index of the requested surface in this set;
         * this must be between 0 and getNumberOfSurfaces()-1 inclusive.
         *
         * @return the normal surface at the requested index in this set.
         */
        const NNormalSurface* getSurface(unsigned long index) const;
        /**
         * Writes the number of surfaces in this set followed by the
         * details of each surface to the given output stream.  Output
         * will be over many lines.
         *
         * \ifacespython Parameter \a out is not present and is assumed
         * to be standard output.
         *
         * @param out the output stream to which to write.
         */
        void writeAllSurfaces(std::ostream& out) const;

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual bool dependsOnParent() const;

        /**
         * Converts the set of all embedded vertex normal surfaces in
         * quadrilateral space to the set of all embedded vertex normal
         * surfaces in standard (tri-quad) space.  The initial list in
         * quadrilateral space is taken to be this normal surface list;
         * the final list in standard space will be inserted as a new
         * child packet of the underlying triangulation (specifically, as
         * the final child).  As a convenience, the final list will also
         * be returned from this routine.
         *
         * This routine can only be used with normal surfaces, not almost
         * normal surfaces.  For almost normal surfaces, see the similar
         * routine quadOctToStandardAN().
         *
         * This procedure is available for any triangulation whose vertex
         * links are all spheres and/or discs, and is \e much faster than
         * enumerating surfaces directly in standard tri-quad coordinates.
         * The underlying algorithm is described in detail in "Converting
         * between quadrilateral and standard solution sets in normal
         * surface theory", Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009),
         * 2121-2174.
         *
         * Typically users do not need to call this routine directly,
         * since the standard enumerate() routine will use it implicitly
         * where possible.  That is, when asked for standard vertex surfaces,
         * enumerate() will first find all \e quadrilateral vertex surfaces
         * and then use this procedure to convert them to standard vertex
         * surfaces; this is generally orders of magnitude faster than
         * enumerating surfaces directly in standard coordinates.
         *
         * Nevertheless, this standalone routine is provided as a convenience
         * for users who already have a set of quadrilateral vertex surfaces,
         * and who simply wish to convert them to a set of standard
         * vertex surfaces without the cost of implicitly enumerating the
         * quadrilateral vertex surfaces again.
         *
         * It should be noted that this routine does \e not simply convert
         * vectors from one form to another; instead it converts a full
         * solution set of vertex surfaces in quadrilateral coordinates to a
         * full solution set of vertex surfaces in standard coordinates.
         * Typically there are many more vertex surfaces in standard
         * coordinates (all of which this routine will find).
         *
         * This routine will run some very basic sanity checks before
         * starting.  Specifically, it will check the validity and vertex
         * links of the underlying triangulation, and will verify that
         * the coordinate flavour and embedded-only flag are set to
         * NS_QUAD and \c true respectively.  If any of
         * these checks fails, this routine will do nothing and return 0.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex normal surfaces in quadrilateral space; no more,
         * no less.  Moreover, these vectors are stored using quadrilateral
         * coordinates.  Typically this means that it was obtained through
         * enumerate(), with the flavour set to NS_QUAD and
         * with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex normal surfaces in standard (tri-quad)
         * coordinates, or 0 if any of the basic sanity checks failed.
         */
        NNormalSurfaceList* quadToStandard() const;

        /**
         * Converts the set of all embedded vertex almost normal surfaces in
         * quadrilateral-octagon space to the set of all embedded vertex
         * almost normal surfaces in the standard tri-quad-oct space.
         *
         * This routine is the almost normal analogue to the
         * quadToStandard() conversion routine; see the quadToStandard()
         * documentation for further information.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This surface list is precisely the set of all embedded vertex
         * almost normal surfaces in quadrilateral-octagon space; no more,
         * no less.  Moreover, these vectors are stored using
         * quadrilateral-octagon coordinates.  Typically this means that it
         * was obtained through enumerate(), with the flavour set to
         * NS_AN_QUAD_OCT and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex almost normal surfaces in standard
         * tri-quad-oct coordinates, or 0 if any of the basic sanity checks
         * failed.
         */
        NNormalSurfaceList* quadOctToStandardAN() const;

        /**
         * Converts the set of all embedded vertex normal surfaces in
         * standard (tri-quad) space to the set of all embedded vertex
         * normal surfaces in quadrilateral space.  The initial list in
         * standard space is taken to be this normal surface list;
         * the final list in quadrilateral space will be inserted as a new
         * child packet of the underlying triangulation (specifically, as
         * the final child).  As a convenience, the final list will also
         * be returned from this routine.
         *
         * This routine can only be used with normal surfaces, not almost
         * normal surfaces.  For almost normal surfaces, see the similar
         * routine standardANToQuadOct().
         *
         * This procedure is available for any triangulation whose vertex
         * links are all spheres and/or discs.  The underlying algorithm
         * is described in detail in "Converting between quadrilateral and
         * standard solution sets in normal surface theory",
         * Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.
         *
         * It should be noted that this routine does \e not simply convert
         * vectors from one form to another; instead it converts a full
         * solution set of vertex surfaces in standard coordinates to a
         * full solution set of vertex surfaces in quadrilateral coordinates.
         * Typically there are far fewer vertex surfaces in quadrilateral
         * coordinates (all of which this routine will find).
         *
         * This routine will run some very basic sanity checks before
         * starting.  Specifically, it will check the validity and vertex
         * links of the underlying triangulation, and will verify that
         * the coordinate flavour and embedded-only flag are set to
         * NS_STANDARD and \c true respectively.  If any of
         * these checks fails, this routine will do nothing and return 0.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex normal surfaces in standard (tri-quad) space;
         * no more, no less.  Moreover, these vectors are stored using
         * standard coordinates.  Typically this means that this list was
         * obtained through enumerate(), with the flavour set to
         * NS_STANDARD and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex normal surfaces in quadrilateral
         * coordinates, or 0 if any of the basic sanity checks failed.
         */
        NNormalSurfaceList* standardToQuad() const;

        /**
         * Converts the set of all embedded vertex almost normal surfaces in
         * standard tri-quad-oct space to the set of all embedded vertex
         * almost normal surfaces in the smaller quadrilateral-octagon space.
         *
         * This routine is the almost normal analogue to the
         * standardToQuad() conversion routine; see the standardToQuad()
         * documentation for further information.
         *
         * \pre The underlying triangulation (the parent packet of this
         * normal surface list) is valid, and the link of every vertex
         * is either a sphere or a disc.
         * \pre This normal surface list is precisely the set of all
         * embedded vertex almost normal surfaces in standard tri-quad-oct
         * space; no more, no less.  Typically this means that it was obtained
         * through enumerate(), with the flavour set to
         * NS_AN_STANDARD and with \a embeddedOnly set to \c true.
         *
         * @return a full list of vertex almost normal surfaces in
         * quadrilateral-octagon coordinates, or 0 if any of the basic
         * sanity checks failed.
         */
        NNormalSurfaceList* standardANToQuadOct() const;

        /**
         * Creates a new list filled with the surfaces from this list
         * that have at least one locally compatible partner.
         * In other words, a surface \a S from this list will be placed
         * in the new list if and only if there is some other surface \a T
         * in this list for which \a S and \a T are locally compatible.
         * See NNormalSurface::locallyCompatible() for further details on
         * compatibility testing.
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         *
         * \warning If this list contains a vertex link (plus at least
         * one other surface), then the new list will be identical to
         * the old (i.e., every surface will be copied across).
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NNormalSurfaceList* filterForLocallyCompatiblePairs() const;

        /**
         * Creates a new list filled with the surfaces from this list
         * that have at least one disjoint partner.
         * In other words, a surface \a S from this list will be placed
         * in the new list if and only if there is some other surface \a T
         * in this list for which \a S and \a T can be made to intersect
         * nowhere at all, without changing either normal isotopy class.
         * See NNormalSurface::disjoint() for further details on disjointness
         * testing.
         *
         * This routine cannot deal with empty, disconnected or
         * non-compact surfaces.  Such surfaces will be silently
         * ignored, and will not be used in any disjointness tests (in
         * particular, they will never be considered as a "disjoint partner"
         * for any other surface).
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         * \pre All surfaces within this list are stored using the same
         * flavour of coordinate system (i.e., the same subclass of
         * NNormalSurfaceVector).
         *
         * \warning If this list contains a vertex link (plus at least
         * one other surface), then the new list will be identical to
         * the old (i.e., every surface will be copied across).
         *
         * \todo Deal properly with surfaces that are too large to handle.
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NNormalSurfaceList* filterForDisjointPairs() const;

        /**
         * Creates a new list filled with only the surfaces from this list
         * that "might" represent two-sided incompressible surfaces.
         * More precisely, we consider all two-sided surfaces in this list,
         * as well as the two-sided double covers of all one-sided surfaces
         * in this list (see below for details on how one-sided surfaces
         * are handled).  Each of these surfaces is examined using
         * relatively fast heuristic tests for incompressibility.  Any
         * surface that is definitely \e not incompressible is thrown
         * away, and all other surfaces are placed in the new list.
         *
         * Therefore, it is guaranteed that every incompressible surface
         * from the old list will be placed in the new list.  However,
         * it is not known whether any given surface in the new list is
         * indeed incompressible.
         *
         * See NNormalSurface::isIncompressible() for the definition of
         * incompressibility that is used here.  Note in particular that
         * spheres are \e never considered incompressible.
         *
         * As indicated above, this filter works exclusively with two-sided
         * surfaces.  If a surface in this list is one-sided, the heuristic
         * incompressibility tests will be run on its two-sided double cover.
         * Nevertheless, if the tests pass, the original one-sided surface
         * (not the double cover) will be added to the new list.
         *
         * The new list will be inserted as a new child packet of the
         * underlying triangulation (specifically, as the final child).  As a
         * convenience, the new list will also be returned from this routine.
         *
         * This original list is not altered in any way.  Likewise,
         * the surfaces in the new list are deep copies of the originals
         * (so they can be altered without affecting the original surfaces).
         *
         * Currently the heuristic tests include (i) throwing away
         * all vertex links and thin edge links, and then
         * (ii) cutting along the remaining surfaces and running
         * NTriangulation::hasSimpleCompressingDisc() on the resulting
         * bounded triangulations.  For more details on these tests
         * see "The Weber-Seifert dodecahedral space is non-Haken",
         * Benjamin A. Burton, J. Hyam Rubinstein and Stephan Tillmann,
         * Trans. Amer. Math. Soc. 364:2 (2012), pp. 911-932.
         *
         * \pre The underlying 3-manifold triangulation is valid and closed.
         * In particular, it has no ideal vertices.
         * \pre This list contains only embedded normal surfaces.  More
         * precisely, isEmbeddedOnly() must return \c true.
         * \pre This list contains only compact, connected normal surfaces.
         * \pre No surfaces in this list contain any octagonal discs.
         *
         * \warning The behaviour of this routine is subject to change
         * in future versions of Regina, since additional tests may be
         * added to improve the power of this filtering.
         *
         * \todo Add progress tracking.
         *
         * @return the new list, which will also have been inserted as
         * a new child packet of the underlying triangulation.
         */
        NNormalSurfaceList* filterForPotentiallyIncompressible() const;

        /**
         * Returns a newly created matrix containing the matching
         * equations that were used to create this normal surface list.
         * The destruction of this matrix is the responsibility of the
         * caller of this routine.  Multiple calls to this routine will
         * result in the construction of multiple matrices.  This
         * routine in fact merely calls makeMatchingEquations() with the
         * appropriate parameters.
         *
         * The format of the matrix is identical to that returned by
         * makeMatchingEquations().
         * 
         * @return the matching equations used to create this normal
         * surface list.
         */
        NMatrixInt* recreateMatchingEquations() const;

        /**
         * An iterator that gives access to the raw vectors for surfaces in
         * this list, pointing to the beginning of this surface list.
         *
         * \ifacespython Not present.
         *
         * @return an iterator at the beginning of this surface list.
         */
        VectorIterator beginVectors() const;

        /**
         * An iterator that gives access to the raw vectors for surfaces in
         * this list, pointing past the end of this surface list.
         * This iterator is not dereferenceable.
         *
         * \ifacespython Not present.
         *
         * @return an iterator past the end of this surface list.
         */
        VectorIterator endVectors() const;

        /**
         * A bidirectional iterator that runs through the raw vectors for
         * surfaces in this list.
         *
         * \ifacespython Not present.
         */
        class VectorIterator : public std::iterator<
                std::bidirectional_iterator_tag, const NNormalSurfaceVector*> {
            private:
                std::vector<NNormalSurface*>::const_iterator it_;
                    /**< An iterator into the underlying list of surfaces. */

            public:
                /**
                 * Creates a new uninitialised iterator.
                 */
                VectorIterator();

                /**
                 * Creates a copy of the given iterator.
                 *
                 * @param cloneMe the iterator to clone.
                 */
                VectorIterator(const VectorIterator& cloneMe);

                /**
                 * Makes this a copy of the given iterator.
                 *
                 * @param cloneMe the iterator to clone.
                 * @return a reference to this iterator.
                 */
                VectorIterator& operator = (const VectorIterator& cloneMe);

                /**
                 * Compares this with the given operator for equality.
                 *
                 * @param other the iterator to compare this with.
                 * @return \c true if the iterators point to the same
                 * element of the same normal surface list, or \c false
                 * if they do not.
                 */
                bool operator == (const VectorIterator& other) const;

                /**
                 * Compares this with the given operator for inequality.
                 *
                 * @param other the iterator to compare this with.
                 * @return \c false if the iterators point to the same
                 * element of the same normal surface list, or \c true
                 * if they do not.
                 */
                bool operator != (const VectorIterator& other) const;

                /**
                 * Returns the raw vector for the normal surface that this
                 * iterator is currently pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * @return the corresponding normal surface vector.
                 */
                const NNormalSurfaceVector* operator *() const;

                /**
                 * The preincrement operator.
                 *
                 * @return a reference to this iterator after the increment.
                 */
                VectorIterator& operator ++();

                /**
                 * The postincrement operator.
                 *
                 * @return a copy of this iterator before the
                 * increment took place.
                 */
                VectorIterator operator ++(int);

                /**
                 * The predecrement operator.
                 *
                 * @return a reference to this iterator after the decrement.
                 */
                VectorIterator& operator --();

                /**
                 * The postdecrement operator.
                 *
                 * @return a copy of this iterator before the
                 * decrement took place.
                 */
                VectorIterator operator --(int);

            private:
                /**
                 * Initialise a new vector iterator using an iterator for
                 * the internal list of normal surfaces.
                 */
                VectorIterator(
                    const std::vector<NNormalSurface*>::const_iterator& i);

            friend class NNormalSurfaceList;
        };

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * An output iterator used to insert surfaces into an
         * NNormalSurfaceList.
         *
         * Objects of type <tt>NNormalSurface*</tt> and
         * <tt>NNormalSurfaceVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NNormalSurface
         * will be automatically created.
         *
         * \warning The behaviour of this class has changed!
         * As of Regina 4.6, this class happily inserts every surface or
         * vector that it is given.  In previous versions it checked
         * almost normal surface vectors for multiple octagonal discs;
         * this check has been removed to support conversions between
         * quad-oct space and standard almost normal space, and to support
         * the enumeration of \e all almost normal surfaces (as opposed to
         * just vertex surfaces).  Such checks are now left to the user
         * interface (and indeed are now optional, at the user's discretion).
         */
        struct SurfaceInserter : public std::iterator<
                std::output_iterator_tag, NNormalSurfaceVector*> {
            NNormalSurfaceList* list;
                /**< The list into which surfaces will be inserted. */
            NTriangulation* owner;
                /**< The triangulation in which the surfaces to be
                 *   inserted are contained. */

            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param newList the list into which surfaces will be
             * inserted.
             * @param newOwner the triangulation in which the surfaces
             * to be inserted are contained.
             */
            SurfaceInserter(NNormalSurfaceList& newList,
                NTriangulation* newOwner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            SurfaceInserter(const SurfaceInserter& cloneMe);

            /**
             * Sets this iterator to be a clone of the given output iterator.
             *
             * @param cloneMe the output iterator to clone.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(const SurfaceInserter& cloneMe);

            /**
             * Appends a normal surface to the end of the appropriate
             * surface list.
             *
             * The given surface will be deallocated with the other
             * surfaces in this list when the list is eventually destroyed.
             *
             * @param surface the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NNormalSurface* surface);
            /**
             * Appends the normal surface corresponding to the given
             * vector to the end of the appropriate surface list.
             *
             * The given vector will be owned by the newly created
             * normal surface and will be deallocated with the other
             * surfaces in this list when the list is eventually destroyed.
             *
             * \warning The behaviour of this routine has changed!
             * As of Regina 4.6, this routine no longer checks for
             * multiple octagonal discs.  See the SurfaceInserter
             * class notes for details.
             *
             * @param vector the vector of the normal surface to insert.
             * @return this output iterator.
             */
            SurfaceInserter& operator =(NNormalSurfaceVector* vector);

            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator *();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            SurfaceInserter& operator ++(int);
        };

    private:
        /**
         * A helper class containing constants, typedefs and operations
         * for working with normal (as opposed to almost normal) surfaces.
         *
         * This class and its partner AlmostNormalSpec can be used to
         * write generic template code that works with both normal
         * \e and almost normal surfaces.
         *
         * The full definition of this class is in the file normalspec.tcc .
         */
        struct NormalSpec;

        /**
         * A helper class containing constants, typedefs and operations
         * for working with almost normal (as opposed to normal) surfaces.
         *
         * This class and its partner NormalSpec can be used to
         * write generic template code that works with both normal
         * \e and almost normal surfaces.
         *
         * The full definition of this class is in the file normalspec.tcc .
         */
        struct AlmostNormalSpec;

        /**
         * Creates an empty list of normal surfaces with the given
         * parameters.
         *
         * @param flavour the flavour of coordinate system to be used
         * for filling this list.
         * @param which indicates which normal surfaces these will
         * represent within the underlying triangulation.
         * @param algorithm details of the enumeration algorithm that
         * will be used to fill this list.
         */
        NNormalSurfaceList(NormalCoords flavour, NormalList which,
            NormalAlg algorithm);

        /**
         * Converts a set of embedded vertex normal surfaces in
         * (quad or quad-oct) space to a set of embedded vertex normal
         * surfaces in (standard normal or standard almost normal) space.
         * The original (quad or quad-oct) space surfaces are passed in
         * the argument \a quadList, and the resulting (standard normal
         * or standard almost normal) space surfaces will be inserted
         * directly into this list.
         *
         * See quadToStandard() and quadOctToStandardAN() for full details
         * and preconditions for this procedure.
         *
         * This routine is designed to work with surface lists that are
         * still under construction.  As such, it ignores the packet
         * tree completely.  The parent packet is ignored (and not changed);
         * instead the underlying triangulation is passed explicitly as
         * the argument \a owner.
         *
         * Although this routine takes a vector of non-const pointers, it
         * guarantees not to modify (or destroy) any of the contents.
         *
         * Although this is a template function, it is a private
         * template function and so is only defined in the one <tt>.cpp</tt>
         * file that needs it.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are doing the
         * work for quadToStandard() or quadOctToStandardAN() respectively.
         * \pre The flavour for this surface list is set to
         * NS_STANDARD or NS_AN_STANDARD, according to whether we are doing
         * the work for quadToStandard() or quadOctToStandardAN() respectively,
         * and the embedded-only flag is set to \c true.
         * \pre The given triangulation is valid and non-empty, and the link
         * of every vertex is either a sphere or a disc.
         *
         * @param owner the triangulation upon which this list of
         * surfaces is to be based.
         * @param reducedList a full list of vertex surfaces in
         * (quad or quad-oct) coordinates for the given triangulation.
         */
        template <class Variant>
        void buildStandardFromReduced(NTriangulation* owner,
            const std::vector<NNormalSurface*>& reducedList);

        /**
         * Implements the one-template-argument version of
         * buildStandardFromReduced() using the specified bitmask type
         * to store zero sets.  See the one-template-argument
         * buildStandardFromReduced() for further information on this routine,
         * including important preconditions.
         *
         * The one-template-argument buildStandardFromReduced() simply
         * chooses an appropriate bitmask type and then calls this
         * routine, which does the real work.
         *
         * \pre The template argument \a BitmaskType can support
         * bitmasks of size 7 \a n (if we are using normal surfaces) or size
         * 10 \a n (if we are using almost normal surfaces), where \a n is
         * the number of tetrahedra in the given triangulation.
         */
        template <class Variant, class BitmaskType>
        void buildStandardFromReducedUsing(NTriangulation* owner,
            const std::vector<NNormalSurface*>& reducedList);

        /**
         * Converts a set of embedded vertex surfaces in (quad or quad-oct)
         * space to a set of embedded vertex surfaces in (standard normal or
         * standard almost normal) space.
         *
         * This is a generic implementation that performs the real work
         * for both quadToStandard() and quadOctToStandardAN().  See each
         * of those routines for further details as well as relevant
         * preconditions and postconditions.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are implementing
         * quadToStandard() or quadOctToStandardAN() accordingly.
         */
        template <class Variant>
        NNormalSurfaceList* internalReducedToStandard() const;

        /**
         * Converts a set of embedded vertex surfaces in
         * (standard normal or standard almost normal) space to a set of
         * embedded vertex surfaces in (quad or quad-oct) space.
         *
         * This is a generic implementation that performs the real work
         * for both standardToQuad() and standardANToQuadOct().  See each
         * of those routines for further details as well as relevant
         * preconditions and postconditions.
         *
         * \pre The template argument \a Variant is either NormalSpec
         * or AlmostNormalSpec, according to whether we are implementing
         * standardToQuad() or standardANToQuadOct() accordingly.
         */
        template <class Variant>
        NNormalSurfaceList* internalStandardToReduced() const;

        /**
         * A thread class that performs all normal surface enumeration.
         *
         * The "real work" is in operator(), where the coordinate system
         * becomes a compile-time constant.  The run() routine simply
         * farms out the real work to some instantiation of operator().
         */
        class Enumerator : public NThread {
            private:
                NNormalSurfaceList* list_;
                    /**< The surface list to be filled. */
                NTriangulation* triang_;
                    /**< The triangulation in which these surfaces lie. */
                NProgressManager* manager_;
                    /**< The progress manager through which progress is
                         reported, or 0 if no progress manager is in use. */

            public:
                /**
                 * Creates a new enumerator thread with the given
                 * parameters.
                 *
                 * @param list the surface list to be filled.
                 * @param triang the triangulation in which these surfaces lie.
                 * @param manager the progress manager to use for progress
                 * reporting, or 0 if progress reporting is not required.
                 */
                Enumerator(NNormalSurfaceList* list,
                    NTriangulation* triang, NProgressManager* manager);

                void* run(void*);

                /**
                 * Performs the real enumeration work, in a setting
                 * where the underlying flavour of coordinate system is
                 * a compile-time constant.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ into the packet
                 * tree as a child of \a triang_.
                 */
                template <typename Flavour>
                void operator() (Flavour);

            private:
                /**
                 * The enumeration code for enumerating vertex surfaces.
                 * This is internal to operator().
                 *
                 * We assume that the flag set list_->which_ has been
                 * cleaned up (and includes NS_VERTEX).  We make no
                 * assumptions about the state of list_->algorithm_.
                 *
                 * This routine only fills \a list_ with surfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 */
                template <typename Flavour>
                void fillVertex();

                /**
                 * The enumeration code for enumerating fundamental surfaces.
                 * This is internal to operator().
                 *
                 * We assume that the flag set list_->which_ has been
                 * cleaned up (and includes NS_FUNDAMENTAL).  We make no
                 * assumptions about the state of list_->algorithm_.
                 *
                 * This routine only fills \a list_ with surfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 */
                template <typename Flavour>
                void fillFundamental();

                /**
                 * The enumeration code for enumerating vertex surfaces
                 * using the double description method.
                 * This is internal to fillVertex().
                 *
                 * This routine does not initialise or finalise progress
                 * reporting.  However, it tracks progress numerically:
                 * if the \a progress argument is non-null then
                 * when this routine begins the total steps required will be
                 * raised by some amount, and over the course of this routine
                 * the total steps completed will be raised by this same amount.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillVertexDD(NProgressNumber* progress);

                /**
                 * The enumeration code for enumerating vertex surfaces
                 * using the tree traversal method.
                 * This is internal to fillVertex().
                 *
                 * This routine does not initialise or finalise progress
                 * reporting.  However, it tracks progress numerically:
                 * if the \a progress argument is non-null then
                 * when this routine begins the total steps required will be
                 * raised by some amount, and over the course of this routine
                 * the total steps completed will be raised by this same amount.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillVertexTree(NProgressNumber* progress);

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the primal method.
                 * This is internal to fillFundamental().
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillFundamentalPrimal();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the dual method.
                 * This is internal to fillFundamental().
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillFundamentalDual();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using a slow Contejean-Devie method.
                 * This is internal to fillFundamental().
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillFundamentalCD();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using a slow full cone enumeration.
                 * This is internal to fillFundamental().
                 *
                 * This routine initialises and finalises progress
                 * reporting if \a manager is non-null.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Flavour>
                void fillFundamentalFullCone();
        };

    friend class regina::NXMLNormalSurfaceListReader;
};

/**
 * Returns a new normal surface vector of the appropriate length for the
 * given triangulation and the given flavour of coordinate system.
 * All elements of this vector will be initialised to zero.
 *
 * The new vector will be of the subclass of NNormalSurfaceVector
 * corresponding to the given flavour of coordinate system.  The caller
 * of this routine is responsible for destroying the new vector.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param flavour the flavour of coordinate system to be used.
 * @return a new zero vector of the correct class and length.
 */
REGINA_API NNormalSurfaceVector* makeZeroVector(
        const NTriangulation* triangulation,
        NormalCoords flavour);
/**
 * Creates a new set of normal surface matching equations for the
 * given triangulation using the given flavour of coordinate system.
 * The returned matrix will be newly allocated and its destruction will
 * be the responsibility of the caller of this routine.
 *
 * Each equation will be represented as a row of the matrix.
 * Each column of the matrix represents a coordinate in the given
 * flavour of coordinate system.
 *
 * @param triangulation the triangulation upon which these matching equations
 * will be based.
 * @param flavour the flavour of coordinate system to be used.
 * @return a newly allocated set of matching equations.
 */
REGINA_API NMatrixInt* makeMatchingEquations(NTriangulation* triangulation,
    NormalCoords flavour);
/**
 * Creates a new set of validity constraints representing the condition that
 * normal surfaces be embedded.  The validity constraints will be expressed
 * relative to the given flavour of coordinate system.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which these validity constraints
 * will be based.
 * @param flavour the flavour of coordinate system to be used.
 * @return a newly allocated set of constraints.
 */
REGINA_API NEnumConstraintList* makeEmbeddedConstraints(
    NTriangulation* triangulation, NormalCoords flavour);

/*@}*/

// Inline functions for NNormalSurfaceList

inline NNormalSurfaceList::~NNormalSurfaceList() {
    for_each(surfaces.begin(), surfaces.end(), FuncDelete<NNormalSurface>());
}

inline NormalCoords NNormalSurfaceList::getFlavour() const {
    return flavour_;
}

inline NormalCoords NNormalSurfaceList::flavour() const {
    return flavour_;
}

inline NormalList NNormalSurfaceList::which() const {
    return which_;
}

inline NormalAlg NNormalSurfaceList::algorithm() const {
    return algorithm_;
}

inline bool NNormalSurfaceList::isEmbeddedOnly() const {
    return which_.has(NS_EMBEDDED_ONLY);
}

inline unsigned long NNormalSurfaceList::getNumberOfSurfaces() const {
    return surfaces.size();
}

inline const NNormalSurface* NNormalSurfaceList::getSurface(
        unsigned long index) const {
    return surfaces[index];
}

inline bool NNormalSurfaceList::dependsOnParent() const {
    return true;
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerate(
        NTriangulation* owner, NormalCoords flavour,
        bool embeddedOnly, NProgressManager* manager) {
    return enumerate(owner, flavour,
        NS_VERTEX | (embeddedOnly ? NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        NS_ALG_DEFAULT, manager);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateStandardDirect(
        NTriangulation* owner) {
    return enumerate(owner, NS_STANDARD, NS_VERTEX | NS_EMBEDDED_ONLY,
        NS_VERTEX_STD_DIRECT);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateStandardANDirect(
        NTriangulation* owner) {
    return enumerate(owner, NS_AN_STANDARD, NS_VERTEX | NS_EMBEDDED_ONLY,
        NS_VERTEX_STD_DIRECT);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateFundPrimal(
        NTriangulation* owner, NormalCoords flavour,
        bool embeddedOnly, NNormalSurfaceList*, NProgressManager* manager) {
    return enumerate(owner, flavour,
        NS_FUNDAMENTAL |
            (embeddedOnly ? NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        NS_HILBERT_PRIMAL, manager);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateFundDual(
        NTriangulation* owner, NormalCoords flavour,
        bool embeddedOnly, NProgressManager* manager) {
    return enumerate(owner, flavour,
        NS_FUNDAMENTAL |
            (embeddedOnly ? NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        NS_HILBERT_DUAL, manager);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateFundCD(
        NTriangulation* owner, NormalCoords flavour,
        bool embeddedOnly) {
    return enumerate(owner, flavour,
        NS_FUNDAMENTAL |
            (embeddedOnly ? NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        NS_HILBERT_CD);
}

inline NNormalSurfaceList* NNormalSurfaceList::enumerateFundFullCone(
        NTriangulation* owner, NormalCoords flavour,
        bool embeddedOnly) {
    return enumerate(owner, flavour,
        NS_FUNDAMENTAL |
            (embeddedOnly ? NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
        NS_HILBERT_FULLCONE);
}

inline NMatrixInt* NNormalSurfaceList::recreateMatchingEquations() const {
    return makeMatchingEquations(getTriangulation(), flavour_);
}

inline NNormalSurfaceList::VectorIterator::VectorIterator() {
}

inline NNormalSurfaceList::VectorIterator::VectorIterator(
        const NNormalSurfaceList::VectorIterator& cloneMe) :
        it_(cloneMe.it_) {
}

inline NNormalSurfaceList::VectorIterator& NNormalSurfaceList::VectorIterator::
        operator =(const NNormalSurfaceList::VectorIterator& cloneMe) {
    it_ = cloneMe.it_;
    return *this;
}

inline bool NNormalSurfaceList::VectorIterator::operator ==(
        const NNormalSurfaceList::VectorIterator& other) const {
    return (it_ == other.it_);
}

inline bool NNormalSurfaceList::VectorIterator::operator !=(
        const NNormalSurfaceList::VectorIterator& other) const {
    return (it_ != other.it_);
}

inline const NNormalSurfaceVector* NNormalSurfaceList::VectorIterator::
        operator *() const {
    return (*it_)->rawVector();
}

inline NNormalSurfaceList::VectorIterator& NNormalSurfaceList::VectorIterator::
        operator ++() {
    ++it_;
    return *this;
}

inline NNormalSurfaceList::VectorIterator NNormalSurfaceList::VectorIterator::
        operator ++(int) {
    return NNormalSurfaceList::VectorIterator(it_++);
}

inline NNormalSurfaceList::VectorIterator& NNormalSurfaceList::VectorIterator::
        operator --() {
    --it_;
    return *this;
}

inline NNormalSurfaceList::VectorIterator NNormalSurfaceList::VectorIterator::
        operator --(int) {
    return NNormalSurfaceList::VectorIterator(it_--);
}

inline NNormalSurfaceList::VectorIterator::VectorIterator(
        const std::vector<NNormalSurface*>::const_iterator& i) : it_(i) {
}

inline NNormalSurfaceList::VectorIterator NNormalSurfaceList::beginVectors()
        const {
    return VectorIterator(surfaces.begin());
}

inline NNormalSurfaceList::VectorIterator NNormalSurfaceList::endVectors()
        const {
    return VectorIterator(surfaces.end());
}

inline NNormalSurfaceList::SurfaceInserter::SurfaceInserter(
        NNormalSurfaceList& newList, NTriangulation* newOwner) :
        list(&newList), owner(newOwner) {
}

inline NNormalSurfaceList::SurfaceInserter::SurfaceInserter(
        const SurfaceInserter& cloneMe) : list(cloneMe.list),
        owner(cloneMe.owner) {
}


inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        const SurfaceInserter& cloneMe) {
    list = cloneMe.list;
    owner = cloneMe.owner;
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        NNormalSurface* surface) {
    list->surfaces.push_back(surface);
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator =(
        NNormalSurfaceVector* vector) {
    list->surfaces.push_back(new NNormalSurface(owner, vector));
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator *() {
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator ++() {
    return *this;
}

inline NNormalSurfaceList::SurfaceInserter&
        NNormalSurfaceList::SurfaceInserter::operator ++(int) {
    return *this;
}

inline NNormalSurfaceList::NNormalSurfaceList(NormalCoords flavour,
        NormalList which, NormalAlg algorithm) :
        flavour_(flavour), which_(which), algorithm_(algorithm) {
}

inline NNormalSurfaceList::Enumerator::Enumerator(NNormalSurfaceList* list,
        NTriangulation* triang, NProgressManager* manager) :
        list_(list), triang_(triang), manager_(manager) {
}

} // namespace regina

#endif

