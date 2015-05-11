
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

/*! \file hypersurface/nnormalhypersurfacelist.h
 *  \brief Contains a packet representing a collection of normal
 *  hypersurfaces in a 4-manifold triangulation.
 */

#ifndef __NNORMALHYPERSURFACELIST_H
#ifndef __DOXYGEN
#define __NNORMALHYPERSURFACELIST_H
#endif

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include "regina-core.h"
#include "hypersurface/hypercoords.h"
#include "hypersurface/nnormalhypersurface.h"
#include "packet/npacket.h"
#include "utilities/memutils.h"
#include "utilities/nthread.h"

namespace regina {

class NMatrixInt;
class NNormalHypersurface;
class NNormalHypersurfaceList;
class NProgressTracker;
class NXMLNormalHypersurfaceListReader;
class NXMLPacketReader;

template <int> class Triangulation;
typedef Triangulation<4> Dim4Triangulation;

/**
 * \weakgroup hypersurface
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_NORMALHYPERSURFACELIST> {
    typedef NNormalHypersurfaceList Class;
    inline static const char* name() {
        return "Normal Hypersurface List";
    }
};
#endif

/**
 * A packet representing a collection of normal hypersurfaces in a 4-manifold
 * triangulation.  Such a packet must always be a child packet of the
 * triangulation from which the surfaces were obtained.  If this triangulation
 * changes, the information contained in this packet will become invalid.
 *
 * See the NNormalHypersurfaceVector class notes for details of what to do
 * when introducing a new coordinate system.
 *
 * Normal hypersurface lists should be created using the routine enumerate().
 */
class REGINA_API NNormalHypersurfaceList : public NPacket {
    REGINA_PACKET(NNormalHypersurfaceList, PACKET_NORMALHYPERSURFACELIST)

    public:
        class VectorIterator;

    protected:
        std::vector<NNormalHypersurface*> surfaces_;
            /**< Contains the normal hypersurfaces stored in this packet. */
        HyperCoords coords_;
            /**< Stores which coordinate system is being
             *   used by the normal hypersurfaces in this packet. */
        bool embedded_;
            /**< Stores whether we are only interested in embedded
             *   normal hypersurfaces. */

    public:
        /**
         * Destroys this list and all the hypersurfaces within.
         */
        virtual ~NNormalHypersurfaceList();

        /**
         * Enumerates all vertex normal hypersurfaces in the given
         * triangulation using the given coordinate system.
         * These vertex normal hypersurfaces will be stored in a new normal
         * hypersurface list.  Their representations will
         * use the smallest possible integer coordinates.
         * The option is offered to find only embedded normal hypersurfaces
         * or to also include immersed and singular normal hypersurfaces.
         *
         * The normal hypersurface list that is created will be inserted as the
         * last child of the given triangulation.  This triangulation \b must
         * remain the parent of this normal hypersurface list, and must not
         * change while this normal hypersurface list remains in existence.
         *
         * If a progress tracker is passed, the normal hypersurface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the normal hypersurface list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress
         * tracking, this routine will declare and work through a series
         * of stages whose combined weights sum to 1; typically this
         * means that the given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * hypersurfaces will be based.
         * @param coords the coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced; this defaults to
         * \c true.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created normal hypersurface list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal hypersurface list is created).
         */
        static NNormalHypersurfaceList* enumerate(Dim4Triangulation* owner,
            HyperCoords coords, bool embeddedOnly = true,
            NProgressTracker* tracker = 0);

        /**
         * Enumerates all fundamental normal hypersurfaces in the given
         * triangulation using the given coordinate system,
         * using the primal Hilbert basis algorithm.
         * These fundamental normal hypersurfaces will be stored in a new normal
         * hypersurface list.  The option is offered to find only embedded
         * normal hypersurfaces or to also include immersed and singular
         * normal hypersurfaces.
         *
         * The primal algorithm is the recommended method for
         * enumerating fundamental normal hypersurfaces, although other
         * algorithms are made available in this class also.
         * The procedure is the 4-dimensional analogue to the process
         * described in "Fundamental normal surfaces and the enumeration
         * of Hilbert bases", Burton, arXiv:1111.7055, Nov 2011.
         *
         * The normal hypersurface list that is created will be inserted as the
         * last child of the given triangulation.  This triangulation \b must
         * remain the parent of this normal hypersurface list, and must not
         * change while this normal hypersurface list remains in existence.
         *
         * The first step of the primal algorithm is to enumerate all
         * vertex normal hypersurfaces.  If you have already done this, you
         * may pass the list of vertex normal hypersurfaces as the (optional)
         * parameter \a vtxSurfaces.
         *
         * If a progress tracker is passed, the normal hypersurface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the normal hypersurface list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress
         * tracking, this routine will declare and work through a series
         * of stages whose combined weights sum to 1; typically this
         * means that the given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * \pre If non-zero, the argument \a vtxSurfaces is precisely the set
         * of all vertex normal hypersurfaces in the given triangulation,
         * enumerated using the same coordinate system and embedded-only
         * constraints as given here.
         *
         * @param owner the triangulation upon which this list of normal
         * hypersurfaces will be based.
         * @param coords the coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced; this defaults to
         * \c true.
         * @param vtxSurfaces the set of all \e vertex normal hypersurfaces
         * as enumerated under the same coordinate system and
         * constraints as given here; this may be 0 if unknown.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created normal hypersurface list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal hypersurface list is created).
         */
        static NNormalHypersurfaceList* enumerateFundPrimal(
            Dim4Triangulation* owner, HyperCoords coords,
            bool embeddedOnly = true,
            NNormalHypersurfaceList* vtxSurfaces = 0,
            NProgressTracker* tracker = 0);

        /**
         * Enumerates all fundamental normal hypersurfaces in the given
         * triangulation using the given coordinate system,
         * using the dual Hilbert basis algorithm.
         * These fundamental normal hypersurfaces will be stored in a new normal
         * hypersurface list.  The option is offered to find only embedded
         * normal hypersurfaces or to also include immersed and singular
         * normal hypersurfaces.
         *
         * The dual algorithm is fast but its performance is highly variable;
         * for this reason the primal algorithm is recommended instead.
         * For full details of both procedures, see
         * "Fundamental normal surfaces and the enumeration of Hilbert bases",
         * Burton, arXiv:1111.7055, Nov 2011 (this paper describes the
         * process for normal surfaces in 3-manifold triangulations, but
         * the ideas are essentially the same).
         *
         * The normal hypersurface list that is created will be inserted as the
         * last child of the given triangulation.  This triangulation \b must
         * remain the parent of this normal hypersurface list, and must not
         * change while this normal hypersurface list remains in existence.
         *
         * If a progress tracker is passed, the normal hypersurface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the normal hypersurface list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress
         * tracking, this routine will declare and work through a series
         * of stages whose combined weights sum to 1; typically this
         * means that the given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * hypersurfaces will be based.
         * @param coords the coordinate system to be used.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced; this defaults to
         * \c true.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created normal hypersurface list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal hypersurface list is created).
         */
        static NNormalHypersurfaceList* enumerateFundDual(
            Dim4Triangulation* owner, HyperCoords coords,
            bool embeddedOnly = true,
            NProgressTracker* tracker = 0);

        /**
         * Returns the coordinate system being used by the
         * hypersurfaces stored in this set.
         *
         * @return the coordinate system used.
         */
        virtual HyperCoords coords() const;
        /**
         * Returns whether this set is known to contain only embedded normal
         * hypersurfaces.
         *
         * If it is possible that there are non-embedded hypersurfaces in this
         * set but it is not known whether any are actually present or
         * not, this routine will return \c false.
         *
         * @return \c true if it is known that only embedded normal
         * hypersurfaces exist in this list, or \c false if immersed
         * and/or singular normal hypersurfaces might be present.
         */
        virtual bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal hypersurfaces live.
         *
         * @return the triangulation in which these hypersurfaces live.
         */
        virtual Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the number of hypersurfaces stored in this list.
         *
         * @return the number of hypersurfaces.
         */
        virtual unsigned long size() const;
        /**
         * Returns the hypersurface at the requested index in this list.
         *
         * @param index the index of the requested hypersurface in this list;
         * this must be between 0 and size()-1 inclusive.
         *
         * @return the normal hypersurface at the requested index in this list.
         */
        virtual const NNormalHypersurface* getHypersurface(
            unsigned long index) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent,
            NXMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

        /**
         * Returns a newly created matrix containing the matching
         * equations that were used to create this normal hypersurface list.
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
         * hypersurface list.
         */
        NMatrixInt* recreateMatchingEquations() const;

        /**
         * An iterator that gives access to the raw vectors for hypersurfaces
         * in this list, pointing to the beginning of this hypersurface list.
         *
         * \ifacespython Not present.
         *
         * @return an iterator at the beginning of this hypersurface list.
         */
        VectorIterator beginVectors() const;

        /**
         * An iterator that gives access to the raw vectors for hypersurfaces
         * in this list, pointing past the end of this hypersurface list.
         * This iterator is not dereferenceable.
         *
         * \ifacespython Not present.
         *
         * @return an iterator past the end of this hypersurface list.
         */
        VectorIterator endVectors() const;

        /**
         * A bidirectional iterator that runs through the raw vectors for
         * hypersurfaces in this list.
         *
         * \ifacespython Not present.
         */
        class VectorIterator : public std::iterator<
                std::bidirectional_iterator_tag,
                const NNormalHypersurfaceVector*> {
            private:
                std::vector<NNormalHypersurface*>::const_iterator it_;
                    /**< An iterator into the underlying list of
                         hypersurfaces. */

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
                 * Returns the raw vector for the normal hypersurface that this
                 * iterator is currently pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * @return the corresponding normal hypersurface vector.
                 */
                const NNormalHypersurfaceVector* operator *() const;

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
                 * the internal list of normal hypersurfaces.
                 */
                VectorIterator(
                    const std::vector<NNormalHypersurface*>::const_iterator& i);

            friend class NNormalHypersurfaceList;
        };

    protected:
        /**
         * Creates a new normal hypersurface list performing no initialisation
         * whatsoever other than property initialisation.
         */
        NNormalHypersurfaceList();

        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * An output iterator used to insert hypersurfaces into an
         * NNormalHypersurfaceList.
         *
         * Objects of type <tt>NNormalHypersurface*</tt> and
         * <tt>NNormalHypersurfaceVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NNormalHypersurface
         * will be automatically created.
         */
        struct HypersurfaceInserter : public std::iterator<
                std::output_iterator_tag, NNormalHypersurfaceVector*> {
            NNormalHypersurfaceList* list_;
                /**< The list into which hypersurfaces will be inserted. */
            Dim4Triangulation* owner_;
                /**< The triangulation in which the hypersurfaces to be
                 *   inserted are contained. */

            /**
             * Creates a new uninitialised output iterator.
             *
             * \warning This iterator must not be used until its
             * hypersurface list and triangulation have been initialised.
             */
            HypersurfaceInserter();
            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param list the list into which hypersurfaces will be inserted.
             * @param owner the triangulation in which the hypersurfaces
             * to be inserted are contained.
             */
            HypersurfaceInserter(NNormalHypersurfaceList& list,
                Dim4Triangulation* owner);
            /**
             * Creates a new output iterator that is a clone of the
             * given iterator.
             *
             * @param cloneMe the output iterator to clone.
             */
            HypersurfaceInserter(const HypersurfaceInserter& cloneMe);

            /**
             * Sets this iterator to be a clone of the given output iterator.
             *
             * @param cloneMe the output iterator to clone.
             * @return this output iterator.
             */
            HypersurfaceInserter& operator = (
                const HypersurfaceInserter& cloneMe);

            /**
             * Appends a normal hypersurface to the end of the appropriate
             * surface list.
             *
             * The given hypersurface will be deallocated with the other
             * hypersurfaces in this list when the list is eventually
             * destroyed.
             *
             * @param hypersurface the normal hypersurface to insert.
             * @return this output iterator.
             */
            HypersurfaceInserter& operator = (
                NNormalHypersurface* hypersurface);
            /**
             * Appends the normal hypersurface corresponding to the given
             * vector to the end of the appropriate hypersurface list.
             *
             * The given vector will be owned by the newly created
             * normal hypersurface and will be deallocated with the other
             * hypersurfaces in this list when the list is eventually
             * destroyed.
             *
             * @param vector the vector of the normal hypersurface to insert.
             * @return this output iterator.
             */
            HypersurfaceInserter& operator = (
                NNormalHypersurfaceVector* vector);

            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            HypersurfaceInserter& operator *();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            HypersurfaceInserter& operator ++();
            /**
             * Returns a reference to this output iterator.
             *
             * @return this output iterator.
             */
            HypersurfaceInserter& operator ++(int);
        };

    private:
        /**
         * Creates an empty list of normal hypersurfaces with the given
         * parameters.
         *
         * @param coords the coordinate system to be used
         * for filling this list.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced.
         */
        NNormalHypersurfaceList(HyperCoords coords, bool embeddedOnly);

        /**
         * A thread class that actually performs the vertex normal
         * hypersurface enumeration.
         *
         * The "real work" is in operator(), where the coordinate system
         * becomes a compile-time constant.  The run() routine simply
         * farms out the real work to some instantiation of operator().
         */
        class VertexEnumerator : public NThread {
            private:
                NNormalHypersurfaceList* list_;
                    /**< The normal hypersurface list to be filled. */
                Dim4Triangulation* triang_;
                    /**< The triangulation upon which this normal
                         hypersurface list will be based. */
                NProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or 0 if no progress tracker is in use. */

            public:
                /**
                 * Creates a new enumerator thread with the given
                 * parameters.
                 *
                 * @param list the normal hypersurface list to be filled.
                 * @param triang the triangulation upon which this
                 * normal hypersurface list will be based.
                 * @param tracker the progress tracker to use for
                 * progress reporting and cancellation polling, or 0 if
                 * these capabilities are not required.
                 */
                VertexEnumerator(NNormalHypersurfaceList* list,
                    Dim4Triangulation* triang, NProgressTracker* tracker);

                void* run(void*);

                /**
                 * Performs the real enumeration work, in a setting
                 * where the underlying coordinate system is
                 * a compile-time constant.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ into the packet
                 * tree as a child of \a triang_.
                 */
                template <typename Coords>
                void operator() (Coords);
        };

        /**
         * A thread class that performs fundamental normal hypersurface
         * enumeration using the primal Hilbert basis algorithm.
         *
         * The "real work" is in operator(), where the coordinate system
         * becomes a compile-time constant.  The run() routine simply
         * farms out the real work to some instantiation of operator().
         */
        class FundPrimalEnumerator : public NThread {
            private:
                NNormalHypersurfaceList* list_;
                    /**< The normal hypersurface list to be filled. */
                Dim4Triangulation* triang_;
                    /**< The triangulation upon which this normal
                         hypersurface list will be based. */
                NNormalHypersurfaceList* vtxSurfaces_;
                    /**< The list of all vertex normal hypersurfaces in
                         \a triang_, or 0 if this information is not known. */
                NProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or 0 if no progress tracker is in use. */

            public:
                /**
                 * Creates a new enumerator thread with the given
                 * parameters.
                 *
                 * @param list the normal hypersurface list to be filled.
                 * @param triang the triangulation upon which this
                 * normal hypersurface list will be based.
                 * @param vtxSurfaces the vertex normal hypersurfaces in
                 * \a triang, or 0 if this information is not known.
                 * @param tracker the progress tracker to use for
                 * progress reporting and cancellation polling, or 0 if
                 * these capabilities are not required.
                 */
                FundPrimalEnumerator(NNormalHypersurfaceList* list,
                    Dim4Triangulation* triang,
                    NNormalHypersurfaceList* vtxSurfaces,
                    NProgressTracker* tracker);

                void* run(void*);

                /**
                 * Performs the real enumeration work, in a setting
                 * where the underlying coordinate system is
                 * a compile-time constant.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ into the packet
                 * tree as a child of \a triang_.
                 */
                template <typename Coords>
                void operator() (Coords);
        };

        /**
         * A thread class that performs fundamental normal hypersurface
         * enumeration using the dual Hilbert basis algorithm.
         *
         * The "real work" is in operator(), where the coordinate system
         * becomes a compile-time constant.  The run() routine simply
         * farms out the real work to some instantiation of operator().
         */
        class FundDualEnumerator : public NThread {
            private:
                NNormalHypersurfaceList* list_;
                    /**< The normal hypersurface list to be filled. */
                Dim4Triangulation* triang_;
                    /**< The triangulation upon which this normal
                         hypersurface list will be based. */
                NProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or 0 if no progress tracker is in use. */

            public:
                /**
                 * Creates a new enumerator thread with the given
                 * parameters.
                 *
                 * @param list the normal hypersurface list to be filled.
                 * @param triang the triangulation upon which this
                 * normal hypersurface list will be based.
                 * @param tracker the progress tracker to use for
                 * progress reporting and cancellation polling, or 0 if
                 * these capabilities are not required.
                 */
                FundDualEnumerator(NNormalHypersurfaceList* list,
                    Dim4Triangulation* triang, NProgressTracker* tracker);

                void* run(void*);

                /**
                 * Performs the real enumeration work, in a setting
                 * where the underlying coordinate system is
                 * a compile-time constant.
                 *
                 * We assume here that neither list_->which_ nor
                 * list_->algorithm_ have been sanity-checked.
                 *
                 * This routine fills \a list_ with surfaces, and then once
                 * this is finished it inserts \a list_ into the packet
                 * tree as a child of \a triang_.
                 */
                template <typename Coords>
                void operator() (Coords);
        };

    friend class regina::NXMLNormalHypersurfaceListReader;
};

/**
 * Returns a new normal hypersurface vector of the appropriate length for the
 * given triangulation and the given coordinate system.
 * All elements of this vector will be initialised to zero.
 *
 * The new vector will be of the subclass of NNormalHypersurfaceVector
 * corresponding to the given coordinate system.  The caller
 * of this routine is responsible for destroying the new vector.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param coords the coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalHypersurfaceList.
 * @return a new zero vector of the correct class and length.
 */
REGINA_API NNormalHypersurfaceVector* makeZeroVector(
    const Dim4Triangulation* triangulation, HyperCoords coords);
/**
 * Creates a new set of normal hypersurface matching equations for the
 * given triangulation using the given coordinate system.
 * The returned matrix will be newly allocated and its destruction will
 * be the responsibility of the caller of this routine.
 *
 * Each equation will be represented as a row of the matrix.
 * Each column of the matrix represents a coordinate in the given
 * coordinate system.
 *
 * @param triangulation the triangulation upon which these matching equations
 * will be based.
 * @param coords the coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalHypersurfaceList.
 * @return a newly allocated set of matching equations.
 */
REGINA_API NMatrixInt* makeMatchingEquations(
    const Dim4Triangulation* triangulation, HyperCoords coords);
/**
 * Creates a new set of validity constraints representing the condition that
 * normal hypersurfaces be embedded.  The validity constraints will be expressed
 * relative to the given coordinate system.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which these validity constraints
 * will be based.
 * @param coords the coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalHypersurfaceList.
 * @return a newly allocated set of constraints.
 */
REGINA_API NEnumConstraintList* makeEmbeddedConstraints(
    const Dim4Triangulation* triangulation, HyperCoords coords);

/*@}*/

// Inline functions for NNormalHypersurfaceList

inline NNormalHypersurfaceList::NNormalHypersurfaceList() {
}

inline NNormalHypersurfaceList::~NNormalHypersurfaceList() {
    for_each(surfaces_.begin(), surfaces_.end(),
        FuncDelete<NNormalHypersurface>());
}

inline HyperCoords NNormalHypersurfaceList::coords() const {
    return coords_;
}

inline bool NNormalHypersurfaceList::isEmbeddedOnly() const {
    return embedded_;
}

inline unsigned long NNormalHypersurfaceList::size() const {
    return surfaces_.size();
}

inline const NNormalHypersurface* NNormalHypersurfaceList::getHypersurface(
        unsigned long index) const {
    return surfaces_[index];
}

inline bool NNormalHypersurfaceList::dependsOnParent() const {
    return true;
}

inline NMatrixInt* NNormalHypersurfaceList::recreateMatchingEquations() const {
    return makeMatchingEquations(getTriangulation(), coords_);
}

inline NNormalHypersurfaceList::VectorIterator::VectorIterator() {
}

inline NNormalHypersurfaceList::VectorIterator::VectorIterator(
        const NNormalHypersurfaceList::VectorIterator& cloneMe) :
        it_(cloneMe.it_) {
}

inline NNormalHypersurfaceList::VectorIterator&
        NNormalHypersurfaceList::VectorIterator::operator =(
        const NNormalHypersurfaceList::VectorIterator& cloneMe) {
    it_ = cloneMe.it_;
    return *this;
}

inline bool NNormalHypersurfaceList::VectorIterator::operator ==(
        const NNormalHypersurfaceList::VectorIterator& other) const {
    return (it_ == other.it_);
}

inline bool NNormalHypersurfaceList::VectorIterator::operator !=(
        const NNormalHypersurfaceList::VectorIterator& other) const {
    return (it_ != other.it_);
}

inline const NNormalHypersurfaceVector*
        NNormalHypersurfaceList::VectorIterator::operator *() const {
    return (*it_)->rawVector();
}

inline NNormalHypersurfaceList::VectorIterator&
        NNormalHypersurfaceList::VectorIterator::operator ++() {
    ++it_;
    return *this;
}

inline NNormalHypersurfaceList::VectorIterator
        NNormalHypersurfaceList::VectorIterator::operator ++(int) {
    return NNormalHypersurfaceList::VectorIterator(it_++);
}

inline NNormalHypersurfaceList::VectorIterator&
        NNormalHypersurfaceList::VectorIterator::operator --() {
    --it_;
    return *this;
}

inline NNormalHypersurfaceList::VectorIterator
        NNormalHypersurfaceList::VectorIterator::operator --(int) {
    return NNormalHypersurfaceList::VectorIterator(it_--);
}

inline NNormalHypersurfaceList::VectorIterator::VectorIterator(
        const std::vector<NNormalHypersurface*>::const_iterator& i) : it_(i) {
}

inline NNormalHypersurfaceList::VectorIterator
        NNormalHypersurfaceList::beginVectors() const {
    return VectorIterator(surfaces_.begin());
}

inline NNormalHypersurfaceList::VectorIterator
        NNormalHypersurfaceList::endVectors() const {
    return VectorIterator(surfaces_.end());
}

inline NNormalHypersurfaceList::HypersurfaceInserter::HypersurfaceInserter() :
        list_(0), owner_(0) {
}

inline NNormalHypersurfaceList::HypersurfaceInserter::HypersurfaceInserter(
        NNormalHypersurfaceList& list, Dim4Triangulation* owner) :
        list_(&list), owner_(owner) {
}

inline NNormalHypersurfaceList::HypersurfaceInserter::HypersurfaceInserter(
        const HypersurfaceInserter& cloneMe) : list_(cloneMe.list_),
        owner_(cloneMe.owner_) {
}


inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator =(
        const HypersurfaceInserter& cloneMe) {
    list_ = cloneMe.list_;
    owner_ = cloneMe.owner_;
    return *this;
}

inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator =(
        NNormalHypersurface* surface) {
    list_->surfaces_.push_back(surface);
    return *this;
}

inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator =(
        NNormalHypersurfaceVector* vector) {
    list_->surfaces_.push_back(new NNormalHypersurface(owner_, vector));
    return *this;
}

inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator *() {
    return *this;
}

inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator ++() {
    return *this;
}

inline NNormalHypersurfaceList::HypersurfaceInserter&
        NNormalHypersurfaceList::HypersurfaceInserter::operator ++(int) {
    return *this;
}

inline NNormalHypersurfaceList::NNormalHypersurfaceList(HyperCoords coords,
        bool embeddedOnly) : coords_(coords), embedded_(embeddedOnly) {
}

inline NNormalHypersurfaceList::VertexEnumerator::VertexEnumerator(
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NProgressTracker* tracker) :
        list_(list), triang_(triang), tracker_(tracker) {
}

inline NNormalHypersurfaceList::FundPrimalEnumerator::FundPrimalEnumerator(
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NNormalHypersurfaceList* vtxSurfaces, NProgressTracker* tracker) :
        list_(list), triang_(triang), vtxSurfaces_(vtxSurfaces),
        tracker_(tracker) {
}

inline NNormalHypersurfaceList::FundDualEnumerator::FundDualEnumerator(
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NProgressTracker* tracker) :
        list_(list), triang_(triang), tracker_(tracker) {
}

} // namespace regina

#endif

