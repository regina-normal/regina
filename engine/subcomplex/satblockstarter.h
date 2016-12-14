
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

/*! \file subcomplex/satblockstarter.h
 *  \brief Provides a hard-coded list of saturated blocks to use as starting
 *  points for recognising larger Seifert fibred spaces.
 */

#ifndef __SATBLOCKSTARTER_H
#ifndef __DOXYGEN
#define __SATBLOCKSTARTER_H
#endif

#include "regina-core.h"
#include "subcomplex/satblock.h"
#include "triangulation/dim3.h"
#include "utilities/listoncall.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Contains a triangulation of a saturated block along with the
 * accompanying saturated block description.  Different objects of this
 * class will correspond to different types of saturated block.
 *
 * This is a support class for SatBlockStarterSet, and as such it is a
 * read-only class to the rest of the world.
 *
 * This class is well-suited for subcomplex testing: if the
 * triangulation here is found to be a subcomplex of some larger
 * triangulation (see Triangulation<3>::isContainedIn()), then the
 * corresponding isomorphism can be used to copy this block structure
 * and transform it to describe the corresponding block in the larger
 * triangulation.
 *
 * As such, one of the core uses of this class is as a starting point
 * for identifying regions within triangulations that are formed by
 * joining saturated blocks together along their boundary annuli.  See
 * the routines SatBlockStarterSearcher::findStarterBlocks() and
 * SatRegion::expand() for implementations of this.
 *
 * \ifacespython Not present.
 */
class REGINA_API SatBlockStarter : boost::noncopyable {
    private:
        Triangulation<3> triangulation_;
            /**< The triangulation of the saturated block. */
        SatBlock* block_;
            /**< Structural details of the saturated block. */

    public:
        /**
         * Destroys both the internal triangulation and block structure.
         */
        ~SatBlockStarter();

        /**
         * Returns a reference to the triangulation of the saturated
         * block.
         *
         * @return the block triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns details that describe the structure of the saturated
         * block.
         *
         * @return the block structure.
         */
        const SatBlock* block() const;

    private:
        /**
         * Creates a new starter block.  The triangulation will be empty
         * and the block pointer set to null.
         *
         * The triangulation must be fleshed out and the block structure
         * created before this object can be used.
         */
        SatBlockStarter();

    friend class SatBlockStarterSet;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatBlockStarter has now been renamed to
 * SatBlockStarter.
 */
REGINA_DEPRECATED typedef SatBlockStarter NSatBlockStarter;

/**
 * Represents a set of starter blocks that can be used for identifying
 * triangulations of Seifert fibred spaces.
 *
 * This class provides a list of saturated blocks that can be used as
 * starting points for recognising triangulations; see the
 * SatBlockStarter class notes for details.
 *
 * More importantly, this list is global and hard-coded.  The only
 * access to the list is through the static routines begin() and end().
 *
 * Creating the list of starter blocks is expensive, and so this is not
 * done until the first time that begin() is called.  This way, if the list
 * is never used then the work is never done.  As a consequence however,
 * you must be sure to call begin() before calling end() (which is the
 * usual way in which iterator loops are structured in code).
 *
 * Be aware that this list makes no claims to be exhaustive; it is
 * expected to grow as future versions of Regina are released.
 *
 * \ifacespython Not present.
 */
class REGINA_API SatBlockStarterSet : private ListOnCall<SatBlockStarter> {
    public:
        /**
         * An iterator over the starter blocks in this list.  This operates
         * as a forward iterator in a manner consistent with the standard C++
         * library.
         */
        typedef ListOnCall<SatBlockStarter>::iterator iterator;

    private:
        static const SatBlockStarterSet blocks;
            /**< The hard-coded list of starter blocks. */

    public:
        /**
         * Returns an iterator pointing to the first block in the
         * hard-coded list.
         *
         * The very first time this routine is called, the list will be
         * filled with items (and as such the call will be expensive).
         * Every subsequent call will be very cheap.
         *
         * @return an iterator pointing to the first starter block.
         */
        static iterator begin();

        /**
         * Returns an iterator pointing past the end of the hard-coded list
         * (i.e., just after the last item).
         *
         * \pre The begin() routine has been called at least once.
         *
         * @return a past-the-end iterator.
         */
        static iterator end();

    protected:
        void initialise();

    private:
        /**
         * Creates a new list of starter blocks.  This routine is
         * private since the only list that should exist is the global
         * hard-coded list.
         */
        SatBlockStarterSet();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatBlockStarterSet has now been renamed to
 * SatBlockStarterSet.
 */
REGINA_DEPRECATED typedef SatBlockStarterSet NSatBlockStarterSet;

/**
 * A helper class for locating and using starter blocks within a
 * triangulation.
 *
 * This class provides a means for searching for each starter
 * block in the global hard-coded SatBlockStarterSet within a
 * given triangulation.  More specifically, given some triangulation \a t,
 * this class can locate every isomorphic embedding of every starter
 * block in the global SatBlockStarterSet as a subcomplex of \a t (see
 * Triangulation<3>::isContainedIn() for what is meant by "isomorphic
 * embedding").
 *
 * The routine findStarterBlocks() runs the search.  Each time an
 * isomorphic embedding of a starter block is discovered within the
 * given triangulation, the pure virtual routine useStarterBlock() will
 * be called.  The block that is passed to useStarterBlock() will be a
 * new block that refers to the particular embedding of the starter block
 * within the given triangulation (as opposed to the original block
 * structure referring to the prebuilt triangulation in SatBlockStarter).
 *
 * For each situation that requires searching for starter blocks, a
 * subclass of SatBlockStarterSearcher will be required.  This subclass
 * should override useStarterBlock() to perform whatever action is
 * necessary.
 *
 * Instead of locating all isomorphic embeddings of all starter blocks
 * in the global set, the search can be made to finish early once
 * certain conditions are met.  This is done by implementing
 * useStarterBlock() to return \c false when the search should quit.
 *
 * \ifacespython Not present.
 */
class REGINA_API SatBlockStarterSearcher {
    protected:
        SatBlock::TetList usedTets;
            /**< Keeps track of which tetrahedra have used by the
                 current embedding of the current starter block.
                 See useStarterBlock() for further details. */

    public:
        /**
         * Destroys this object and its internal structures.
         */
        virtual ~SatBlockStarterSearcher();

        /**
         * Runs a search for every isomorphic embedding of every
         * starter block from the global SatBlockStarterSet within the
         * given triangulation.  Each time an embedding is discovered,
         * the pure virtual routine useStarterBlock() will be called.
         *
         * See the SatBlockStarterSearcher class notes for greater
         * detail on what this search does and how it runs.
         *
         * For subclasses that make use of the \a usedTets data member,
         * it is worth noting that this routine empties the \a usedTets
         * list on both entry and exit, as well as every time that
         * useStarterBlock() returns after each new embedding is found.
         *
         * @param tri the triangulation in which to search for starter
         * blocks.
         */
        void findStarterBlocks(Triangulation<3>* tri);

    protected:
        /**
         * Used by subclasses to process each starter block embedding that
         * is found.
         *
         * Suppose that the main search routine findStarterBlocks() has
         * been called with some triangulation \a t.  Each time it
         * locates an isomorphic embedding of a starter block within \a t,
         * it will call useStarterBlock().  Subclasses of
         * SatBlockStarterSearcher should therefore override
         * useStarterBlock() to process each embedding in whatever way
         * is appropriate for the problem at hand.
         *
         * The block passed in the argument \a starter is a newly
         * created structure describing the starter block as it appears
         * within the triangulation \a t.  Thus different embeddings of
         * the same starter block within \a t will pass different
         * \a starter arguments to this routine.
         * It is the responsibility of useStarterBlock() to either
         * destroy the new block \a starter or pass ownership of it
         * elsewhere.
         *
         * When this routine is called, the data member \a usedTets
         * will contain a list of all tetrahedra from the triangulation
         * \a t that appear within the relevant starter block embedding.
         * The reimplementation of useStarterBlock() may modify this list
         * as it pleases, since the main search routine will empty the
         * list anyway when useStarterBlock() returns.  One possible use
         * for the \a usedTets data member is for passing to
         * SatBlock::isBlock() or SatRegion::expand() as the list of
         * tetrahedra to avoid in further searches.
         *
         * This routine must return a boolean; this allows subclasses to
         * immediately terminate the main search once they have found
         * whatever it is they were looking for.  A return value of
         * \c true signifies that the search should continue as normal,
         * whereas a return value of \c false signifies that the search
         * should end immediately (specifically, that findStarterBlocks()
         * should clean up and return before all remaining embeddings of all
         * starter blocks have been found).
         *
         * \warning Subclasses must remember to either destroy or claim
         * ownership of the newly created block \a starter.
         *
         * @param starter a newly created structure describing the
         * starter block as it appears within the larger triangulation
         * currently under examination.
         * @return \c true if the search for embeddings of starter blocks
         * should continue, or \c false if the search should stop immediately.
         */
        virtual bool useStarterBlock(SatBlock* starter) = 0;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatBlockStarterSearcher has now been renamed to
 * SatBlockStarterSearcher.
 */
REGINA_DEPRECATED typedef SatBlockStarterSearcher NSatBlockStarterSearcher;

/*@}*/

// Inline functions for SatBlockStarter

inline SatBlockStarter::SatBlockStarter() : block_(0) {
}

inline SatBlockStarter::~SatBlockStarter() {
    if (block_)
        delete block_;
}

inline const Triangulation<3>& SatBlockStarter::triangulation() const {
    return triangulation_;
}

inline const SatBlock* SatBlockStarter::block() const {
    return block_;
}

// Inline functions for SatBlockStarterSet

inline SatBlockStarterSet::SatBlockStarterSet() {
}

inline SatBlockStarterSet::iterator SatBlockStarterSet::begin() {
    return blocks.ListOnCall<SatBlockStarter>::begin();
}

inline SatBlockStarterSet::iterator SatBlockStarterSet::end() {
    return blocks.ListOnCall<SatBlockStarter>::end();
}

// Inline functions for SatBlockStarterSearcher

inline SatBlockStarterSearcher::~SatBlockStarterSearcher() {
}

} // namespace regina

#endif

