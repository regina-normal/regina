
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

/*! \file hypersurface/normalhypersurfaces.h
 *  \brief Contains a packet representing a collection of normal
 *  hypersurfaces in a 4-manifold triangulation.
 */

#ifndef __NORMALHYPERSURFACES_H
#ifndef __DOXYGEN
#define __NORMALHYPERSURFACES_H
#endif

#include <algorithm>
#include <iterator>
#include <vector>
#include "regina-core.h"
#include "hypersurface/hypercoords.h"
#include "hypersurface/hyperflags.h"
#include "hypersurface/normalhypersurface.h"
#include "packet/packet.h"
#include "utilities/memutils.h"

namespace regina {

class NormalHypersurface;
class NormalHypersurfaces;
class ProgressTracker;
class XMLPacketReader;

template <typename> class MatrixIntDomain;
typedef MatrixIntDomain<Integer> MatrixInt;

/**
 * \weakgroup hypersurface
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_NORMALHYPERSURFACES> {
    typedef NormalHypersurfaces Class;
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
 * See the NormalHypersurfaceVector class notes for details of what to do
 * when introducing a new coordinate system.
 *
 * Normal hypersurface lists should be created using the routine enumerate().
 */
class REGINA_API NormalHypersurfaces : public Packet {
    REGINA_PACKET(NormalHypersurfaces, PACKET_NORMALHYPERSURFACES)

    public:
        class VectorIterator;

    protected:
        std::vector<NormalHypersurface*> surfaces_;
            /**< Contains the normal hypersurfaces stored in this packet. */
        HyperCoords coords_;
            /**< Stores which coordinate system is being
                 used by the normal hypersurfaces in this packet. */
        HyperList which_;
            /**< Indicates which normal hypersurfaces these represent
                 within the underlying triangulation. */
        HyperAlg algorithm_;
            /**< Stores the details of the enumeration algorithm that
                 was used to generate this list.  This might not be the
                 same as the \a algorithmHints flag passed to the
                 corresponding enumeration routine (e.g., if invalid or
                 inappropriate flags were passed). */

    public:
        /**
         * Destroys this list and all the hypersurfaces within.
         */
        virtual ~NormalHypersurfaces();

        /**
         * A unified routine for enumerating various classes of normal
         * hypersurfaces within a given triangulation.
         *
         * The HyperCoords argument allows you to specify an underlying
         * coordinate system.
         *
         * The HyperList argument is a combination of flags that
         * allows you to specify exactly which normal hypersurfaces you require.
         * This includes (i) whether you want all vertex hypersurfaces
         * or all fundamental hypersurfaces, which defaults to HS_VERTEX
         * if you specify neither or both; and (ii) whether you want only
         * properly embedded surfaces or you also wish to include
         * immersed and/or singular surfaces, which defaults to
         * HS_EMBEDDED_ONLY if you specify neither or both.
         *
         * The HyperAlg argument is a combination of flags that allows
         * you to control the underlying enumeration algorithm.  These
         * flags are treated as hints only: if your selection of
         * algorithm is invalid, unavailable or unsupported then Regina
         * will choose something more appropriate.  Unless you have
         * some specialised need, the default HS_ALG_DEFAULT (which
         * makes no hints at all) will allow Regina to choose what it
         * thinks will be the most efficient method.
         *
         * The enumerated hypersurfaces will be stored in a new normal
         * hypersurface list, and their representations will be scaled down
         * to use the smallest possible integer coordinates.
         * This normal hypersurface list will be inserted into the packet tree
         * as the last child of the given triangulation.  This triangulation
         * \b must remain the parent of this normal hypersurface list, and must
         * not change while this normal hypersurface list remains in existence.
         *
         * If a progress tracker is passed, the normal hypersurface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  If the user cancels the operation
         * from another thread, then the normal surface list will \e not
         * be inserted into the packet tree (but the caller of this
         * routine will still need to delete it).  Regarding progress tracking,
         * this routine will declare and work through a series of stages
         * whose combined weights sum to 1; typically this means that the
         * given tracker must not have been used before.
         *
         * If no progress tracker is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * hypersurfaces will be based.
         * @param coords the coordinate system to be used.
         * @param which indicates which normal hypersurfaces should be
         * enumerated.
         * @param algHints passes requests to Regina for which specific
         * enumeration algorithm should be used.
         * @param tracker a progress tracker through which progress will
         * be reported, or 0 if no progress reporting is required.
         * @return the newly created normal hypersurface list.  Note that if
         * a progress tracker is passed then this list may not be completely
         * filled when this routine returns.  If a progress tracker is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal hypersurface list is created).
         */
        static NormalHypersurfaces* enumerate(Triangulation<4>* owner,
            HyperCoords coords,
            HyperList which = HS_LIST_DEFAULT,
            HyperAlg algHints = HS_ALG_DEFAULT,
            ProgressTracker* tracker = 0); // TODO

        /**
         * Returns the coordinate system being used by the
         * hypersurfaces stored in this set.
         *
         * @return the coordinate system used.
         */
        HyperCoords coords() const;
        /**
         * Returns details of which normal hypersurfaces this list represents
         * within the underlying triangulation.
         *
         * This may not be the same HyperList that was passed to enumerate().
         * In particular, default values will have been explicitly
         * filled in (such as HS_VERTEX and/or HS_EMBEDDED_ONLY), and
         * invalid and/or redundant values will have been removed.
         *
         * @return details of what this list represents.
         */
        HyperList which() const;
        /**
         * Returns details of the algorithm that was used to enumerate
         * this list.
         *
         * These may not be the same HyperAlg flags that were passed to
         * enumerate().  In particular, default values will have been
         * explicitly filled in, invalid and/or redundant values will have
         * been removed, and unavailable and/or unsupported combinations
         * of algorithm flags will be replaced with whatever algorithm
         * was actually used.
         *
         * @return details of the algorithm used to enumerate this list.
         */
        HyperAlg algorithm() const;
        /**
         * Returns whether this set is known to contain only embedded normal
         * hypersurfaces.
         *
         * If this returns \c false, it does not guarantee that immersed
         * and/or singular hypersurfaces are present; it merely indicates
         * that they were not deliberately excluded (for instance, the
         * prism constraints were not enforced).
         *
         * @return \c true if this list was constructed to contain only
         * properly embedded hypersurfaces, or \c false otherwise.
         */
        bool isEmbeddedOnly() const;
        /**
         * Returns the triangulation in which these normal hypersurfaces live.
         *
         * @return the triangulation in which these hypersurfaces live.
         */
        Triangulation<4>* triangulation() const;

        /**
         * Returns the number of hypersurfaces stored in this list.
         *
         * @return the number of hypersurfaces.
         */
        size_t size() const;
        /**
         * Returns the hypersurface at the requested index in this list.
         *
         * @param index the index of the requested hypersurface in this list;
         * this must be between 0 and size()-1 inclusive.
         *
         * @return the normal hypersurface at the requested index in this list.
         */
        const NormalHypersurface* hypersurface(size_t index) const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
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
        MatrixInt* recreateMatchingEquations() const;

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
                std::bidirectional_iterator_tag, Ray> {
            private:
                std::vector<NormalHypersurface*>::const_iterator it_;
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
                const Ray& operator *() const;

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
                    const std::vector<NormalHypersurface*>::const_iterator& i);

            friend class NormalHypersurfaces;
        };

    protected:
        /**
         * Creates an empty list of normal hypersurfaces with the given
         * parameters.
         *
         * @param coords the coordinate system to be used for filling
         * this list.
         * @param which indicates which normal hypersurfaces these will
         * represent within the underlying triangulation.
         * @param algorithm details of the enumeration algorithm that
         * will be used to fill this list.
         */
        NormalHypersurfaces(HyperCoords coords, HyperList which,
            HyperAlg algorithm);

        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * An output iterator used to insert hypersurfaces into an
         * NormalHypersurfaces.
         *
         * Objects of type <tt>NormalHypersurface*</tt> and
         * <tt>NormalHypersurfaceVector*</tt> can be assigned to this
         * iterator.  In the latter case, a surrounding NormalHypersurface
         * will be automatically created.
         */
        struct HypersurfaceInserter : public std::iterator<
                std::output_iterator_tag, NormalHypersurfaceVector*> {
            NormalHypersurfaces* list_;
                /**< The list into which hypersurfaces will be inserted. */
            Triangulation<4>* owner_;
                /**< The triangulation in which the hypersurfaces to be
                 *   inserted are contained. */

            /**
             * Creates a new output iterator.  The member variables of
             * this iterator will be initialised according to the
             * parameters passed to this constructor.
             *
             * @param list the list into which hypersurfaces will be inserted.
             * @param owner the triangulation in which the hypersurfaces
             * to be inserted are contained.
             */
            HypersurfaceInserter(NormalHypersurfaces& list,
                Triangulation<4>* owner);
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
                NormalHypersurface* hypersurface);
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
                NormalHypersurfaceVector* vector);

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
         * A functor that performs all normal hypersurface enumeration.
         */
        class Enumerator {
            private:
                NormalHypersurfaces* list_;
                    /**< The hypersurface list to be filled. */
                Triangulation<4>* triang_;
                    /**< The triangulation in which these hypersurfaces lie. */
                ProgressTracker* tracker_;
                    /**< The progress tracker through which progress is
                         reported and cancellation requests are accepted,
                         or 0 if no progress tracker is in use. */

            public:
                /**
                 * Creates a new functor with the given parameters.
                 *
                 * @param list the hypersurface list to be filled.
                 * @param triang the triangulation in which these
                 * hypersurfaces lie.
                 * @param tracker the progress tracker to use for
                 * progress reporting and cancellation polling, or 0 if
                 * these capabilities are not required.
                 */
                Enumerator(NormalHypersurfaces* list,
                    Triangulation<4>* triang, ProgressTracker* tracker);

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
                 *
                 * \tparam Coords an instance of the HyperInfo<> template class.
                 */
                template <typename Coords>
                void operator() ();

            private:
                /**
                 * The enumeration code for enumerating vertex hypersurfaces.
                 * This is internal to operator().
                 *
                 * We assume that the flag set which_ is set correctly,
                 * and we do not alter it here.
                 * We make no assumptions about the state of algorithm_,
                 * and we set this during the course of this routine.
                 *
                 * This routine only fills \a list_ with hypersurfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 */
                template <typename Coords>
                void fillVertex();

                /**
                 * The enumeration code for enumerating fundamental
                 * hypersurfaces.  This is internal to operator().
                 *
                 * We assume that the flag set which_ is set correctly,
                 * and we do not alter it here.
                 * We make no assumptions about the state of algorithm_,
                 * and we set this during the course of this routine.
                 *
                 * This routine only fills \a list_ with surfaces.
                 * It does not make any adjustments to the structure of
                 * the packet tree.
                 *
                 * If \a tracker_ is non-null, this routine declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 */
                template <typename Coords>
                void fillFundamental();

                /**
                 * The enumeration code for enumerating vertex surfaces
                 * using the double description method.
                 * This is internal to fillVertex().
                 *
                 * This routine assumes that \a algorithm_ has been set
                 * correctly, and does not alter it.
                 *
                 * If \a tracker_ is non-null, this routine assumes that
                 * an appropriate tracker stage has already been
                 * declared, and works through that stage only.
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Coords>
                void fillVertexDD();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the primal method.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Coords>
                void fillFundamentalPrimal();

                /**
                 * The enumeration code for enumerating fundamental surfaces
                 * using the dual method.
                 * This is internal to fillFundamental().
                 *
                 * This routine assumes nothing about the state of the
                 * \a algorithm_ flag set, and sets it appropriately.
                 *
                 * If \a tracker_ is non-null, this routine will declare and
                 * work through a series of tracker stages whose
                 * combined weights sum to 1.  It will not, however,
                 * call ProgressTracker::setFinished().
                 *
                 * \pre The underlying triangulation is non-empty.
                 */
                template <typename Coords>
                void fillFundamentalDual();
        };

    friend class XMLNormalHypersurfacesReader;
};

/**
 * Returns a new normal hypersurface vector of the appropriate length for the
 * given triangulation and the given coordinate system.
 * All elements of this vector will be initialised to zero.
 *
 * The new vector will be of the subclass of NormalHypersurfaceVector
 * corresponding to the given coordinate system.  The caller
 * of this routine is responsible for destroying the new vector.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param coords the coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NormalHypersurfaces.
 * @return a new zero vector of the correct class and length.
 */
REGINA_API NormalHypersurfaceVector* makeZeroVector(
    const Triangulation<4>* triangulation, HyperCoords coords);
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
 * constants in NormalHypersurfaces.
 * @return a newly allocated set of matching equations.
 */
REGINA_API MatrixInt* makeMatchingEquations(
    const Triangulation<4>* triangulation, HyperCoords coords);
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
 * constants in NormalHypersurfaces.
 * @return a newly allocated set of constraints.
 */
REGINA_API EnumConstraints* makeEmbeddedConstraints(
    const Triangulation<4>* triangulation, HyperCoords coords);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalHypersurfaceList has now been renamed to
 * NormalHypersurfaces.
 */
REGINA_DEPRECATED typedef NormalHypersurfaces NNormalHypersurfaceList;

/*@}*/

// Inline functions for NormalHypersurfaces

inline NormalHypersurfaces::~NormalHypersurfaces() {
    for_each(surfaces_.begin(), surfaces_.end(),
        FuncDelete<NormalHypersurface>());
}

inline HyperCoords NormalHypersurfaces::coords() const {
    return coords_;
}

inline HyperList NormalHypersurfaces::which() const {
    return which_;
}

inline HyperAlg NormalHypersurfaces::algorithm() const {
    return algorithm_;
}

inline bool NormalHypersurfaces::isEmbeddedOnly() const {
    return which_.has(HS_EMBEDDED_ONLY);
}

inline size_t NormalHypersurfaces::size() const {
    return surfaces_.size();
}

inline const NormalHypersurface* NormalHypersurfaces::hypersurface(
        size_t index) const {
    return surfaces_[index];
}

inline bool NormalHypersurfaces::dependsOnParent() const {
    return true;
}

inline MatrixInt* NormalHypersurfaces::recreateMatchingEquations() const {
    return makeMatchingEquations(triangulation(), coords_);
}

inline NormalHypersurfaces::VectorIterator::VectorIterator() {
}

inline NormalHypersurfaces::VectorIterator::VectorIterator(
        const NormalHypersurfaces::VectorIterator& cloneMe) :
        it_(cloneMe.it_) {
}

inline NormalHypersurfaces::VectorIterator&
        NormalHypersurfaces::VectorIterator::operator =(
        const NormalHypersurfaces::VectorIterator& cloneMe) {
    it_ = cloneMe.it_;
    return *this;
}

inline bool NormalHypersurfaces::VectorIterator::operator ==(
        const NormalHypersurfaces::VectorIterator& other) const {
    return (it_ == other.it_);
}

inline bool NormalHypersurfaces::VectorIterator::operator !=(
        const NormalHypersurfaces::VectorIterator& other) const {
    return (it_ != other.it_);
}

inline const Ray& NormalHypersurfaces::VectorIterator::operator *() const {
    return (*it_)->rawVector();
}

inline NormalHypersurfaces::VectorIterator&
        NormalHypersurfaces::VectorIterator::operator ++() {
    ++it_;
    return *this;
}

inline NormalHypersurfaces::VectorIterator
        NormalHypersurfaces::VectorIterator::operator ++(int) {
    return NormalHypersurfaces::VectorIterator(it_++);
}

inline NormalHypersurfaces::VectorIterator&
        NormalHypersurfaces::VectorIterator::operator --() {
    --it_;
    return *this;
}

inline NormalHypersurfaces::VectorIterator
        NormalHypersurfaces::VectorIterator::operator --(int) {
    return NormalHypersurfaces::VectorIterator(it_--);
}

inline NormalHypersurfaces::VectorIterator::VectorIterator(
        const std::vector<NormalHypersurface*>::const_iterator& i) : it_(i) {
}

inline NormalHypersurfaces::VectorIterator
        NormalHypersurfaces::beginVectors() const {
    return VectorIterator(surfaces_.begin());
}

inline NormalHypersurfaces::VectorIterator
        NormalHypersurfaces::endVectors() const {
    return VectorIterator(surfaces_.end());
}

inline NormalHypersurfaces::HypersurfaceInserter::HypersurfaceInserter(
        NormalHypersurfaces& list, Triangulation<4>* owner) :
        list_(&list), owner_(owner) {
}

inline NormalHypersurfaces::HypersurfaceInserter::HypersurfaceInserter(
        const HypersurfaceInserter& cloneMe) : list_(cloneMe.list_),
        owner_(cloneMe.owner_) {
}


inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator =(
        const HypersurfaceInserter& cloneMe) {
    list_ = cloneMe.list_;
    owner_ = cloneMe.owner_;
    return *this;
}

inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator =(
        NormalHypersurface* surface) {
    list_->surfaces_.push_back(surface);
    return *this;
}

inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator =(
        NormalHypersurfaceVector* vector) {
    list_->surfaces_.push_back(new NormalHypersurface(owner_, vector));
    return *this;
}

inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator *() {
    return *this;
}

inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator ++() {
    return *this;
}

inline NormalHypersurfaces::HypersurfaceInserter&
        NormalHypersurfaces::HypersurfaceInserter::operator ++(int) {
    return *this;
}

inline NormalHypersurfaces::NormalHypersurfaces(HyperCoords coords,
        HyperList which, HyperAlg algorithm) :
        coords_(coords), which_(which), algorithm_(algorithm) {
}

inline NormalHypersurfaces::Enumerator::Enumerator(
        NormalHypersurfaces* list, Triangulation<4>* triang,
        ProgressTracker* tracker) :
        list_(list), triang_(triang), tracker_(tracker) {
}

} // namespace regina

#endif

