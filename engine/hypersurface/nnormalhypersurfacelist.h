
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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
#include "hypersurface/nnormalhypersurface.h"
#include "packet/npacket.h"
#include "utilities/memutils.h"
#include "utilities/nthread.h"

namespace regina {

class Dim4Triangulation;
class NMatrixInt;
class NNormalHypersurface;
class NProgressManager;
class NProgressNumber;
class NXMLNormalHypersurfaceListReader;
class NXMLPacketReader;

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * A packet representing a collection of normal hypersurfaces in a 4-manifold
 * triangulation.  Such a packet must always be a child packet of the
 * triangulation from which the surfaces were obtained.  If this triangulation
 * changes, the information contained in this packet will become invalid.
 *
 * See the NNormalHypersurfaceVector class notes for details of what to do
 * when introducing a new flavour of coordinate system.
 *
 * Normal hypersurface lists should be created using the routine enumerate().
 */
class NNormalHypersurfaceList : public NPacket {
    public:
        static const int packetType;

        /**
         * Represents standard tetrahedron-prism coordinates for
         * normal hypersurfaces.
         */
        static const int STANDARD;

        /**
         * Represents edge weight coordinates for normal hypersurfaces.
         * This flavour is for representation only; hypersurface
         * vectors and lists of this flavour cannot be created.
         */
        static const int EDGE_WEIGHT;

    protected:
        std::vector<NNormalHypersurface*> surfaces_;
            /**< Contains the normal hypersurfaces stored in this packet. */
        int flavour_;
            /**< Stores which flavour of coordinate system is being
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
         * triangulation using the given flavour of coordinate system.
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
         * If a progress manager is passed, the normal hypersurface
         * enumeration will take place in a new thread and this routine
         * will return immediately.  The NProgress object assigned to
         * this progress manager is guaranteed to be of the class
         * NProgressNumber.
         *
         * If no progress manager is passed, the enumeration will run
         * in the current thread and this routine will return only when
         * the enumeration is complete.  Note that this enumeration can
         * be extremely slow for larger triangulations.
         *
         * @param owner the triangulation upon which this list of normal
         * hypersurfaces will be based.
         * @param flavour the flavour of coordinate system to be used;
         * this must be one of the predefined coordinate system
         * constants in NNormalHypersurfaceList.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced; this defaults to
         * \c true.
         * @param manager a progress manager through which progress will
         * be reported, or 0 if no progress reporting is required.  If
         * non-zero, \a manager must point to a progress manager for
         * which NProgressManager::isStarted() is still \c false.
         * @return the newly created normal hypersurface list.  Note that if
         * a progress manager is passed then this list may not be completely
         * filled when this routine returns.  If a progress manager is
         * passed and a new thread could not be started, this routine
         * returns 0 (and no normal hypersurface list is created).
         */
        static NNormalHypersurfaceList* enumerate(Dim4Triangulation* owner,
            int flavour, bool embeddedOnly = true,
            NProgressManager* manager = 0);

        /**
         * Returns the flavour of coordinate system being used by the
         * hypersurfaces stored in this set.  This will be one of the
         * predefined coordinate system constants in NNormalHypersurfaceList.
         *
         * @return the flavour of coordinate system used.
         */
        virtual int getFlavour() const;
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
        virtual unsigned long getNumberOfHypersurfaces() const;
        /**
         * Returns the hypersurface at the requested index in this list.
         *
         * @param index the index of the requested hypersurface in this list;
         * this must be between 0 and getNumberOfHypersurfaces()-1 inclusive.
         *
         * @return the normal hypersurface at the requested index in this list.
         */
        virtual const NNormalHypersurface* getHypersurface(
            unsigned long index) const;

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual void writePacket(NFile& out) const;
        static NNormalHypersurfaceList* readPacket(NFile& in, NPacket* parent);
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
         * @param flavour the flavour of coordinate system to be used
         * for filling this list; this must be one of the predefined
         * coordinate system constants in NNormalHypersurfaceList.
         * @param embeddedOnly \c true if only embedded normal hypersurfaces
         * are to be produced, or \c false if immersed and singular
         * normal hypersurfaces are also to be produced.
         */
        NNormalHypersurfaceList(int flavour, bool embeddedOnly);

        /**
         * A thread class that actually performs the normal hypersurface
         * enumeration.
         */
        class Enumerator : public NThread {
            private:
                NNormalHypersurfaceList* list_;
                    /**< The normal hypersurface list to be filled. */
                Dim4Triangulation* triang_;
                    /**< The triangulation upon which this normal
                         hypersurface list will be based. */
                NProgressManager* manager_;
                    /**< The progress manager through which progress is
                         reported, or 0 if no progress manager is in use. */

            public:
                /**
                 * Creates a new enumerator thread with the given
                 * parameters.
                 *
                 * @param list the normal hypersurface list to be filled.
                 * @param triang the triangulation upon which this
                 * normal hypersurface list will be based.
                 * @param manager the progress manager to use for
                 * progress reporting, or 0 if progress reporting is not
                 * required.
                 */
                Enumerator(NNormalHypersurfaceList* list,
                    Dim4Triangulation* triang, NProgressManager* manager);

                void* run(void*);
        };

    friend class regina::NXMLNormalHypersurfaceListReader;
};

/**
 * Returns a new normal hypersurface vector of the appropriate length for the
 * given triangulation and the given flavour of coordinate system.
 * All elements of this vector will be initialised to zero.
 *
 * The new vector will be of the subclass of NNormalHypersurfaceVector
 * corresponding to the given flavour of coordinate system.  The caller
 * of this routine is responsible for destroying the new vector.
 *
 * \ifacespython Not present.
 *
 * @param triangulation the triangulation upon which the underlying
 * coordinate system is based.
 * @param flavour the flavour of coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalHypersurfaceList.
 * @return a new zero vector of the correct class and length.
 */
NNormalHypersurfaceVector* makeZeroVector(
    const Dim4Triangulation* triangulation, int flavour);
/**
 * Creates a new set of normal hypersurface matching equations for the
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
 * @param flavour the flavour of coordinate system to be used;
 * this must be one of the predefined coordinate system
 * constants in NNormalHypersurfaceList.
 * @return a newly allocated set of matching equations.
 */
NMatrixInt* makeMatchingEquations(Dim4Triangulation* triangulation,
    int flavour);

/*@}*/

// Inline functions for NNormalHypersurfaceList

inline NNormalHypersurfaceList::NNormalHypersurfaceList() {
}

inline NNormalHypersurfaceList::~NNormalHypersurfaceList() {
    for_each(surfaces_.begin(), surfaces_.end(),
        FuncDelete<NNormalHypersurface>());
}

inline int NNormalHypersurfaceList::getFlavour() const {
    return flavour_;
}

inline bool NNormalHypersurfaceList::isEmbeddedOnly() const {
    return embedded_;
}

inline unsigned long NNormalHypersurfaceList::getNumberOfHypersurfaces() const {
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
    return makeMatchingEquations(getTriangulation(), flavour_);
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

inline NNormalHypersurfaceList::NNormalHypersurfaceList(int flavour,
        bool embeddedOnly) : flavour_(flavour), embedded_(embeddedOnly) {
}

inline NNormalHypersurfaceList::Enumerator::Enumerator(
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NProgressManager* manager) :
        list_(list), triang_(triang), manager_(manager) {
}

inline void NNormalHypersurfaceList::writePacket(Nfile&) const {
    // No need to add support for the old pre-2002 binary format.
    // This format is deprecated and about to be removed from Regina entirely.
}

inline Dim4Triangulation* Dim4Triangulation::readPacket(NFile&, NPacket*) {
    // Don't bother supporting the old pre-2002 binary format.
    return 0;
}

} // namespace regina

#endif

