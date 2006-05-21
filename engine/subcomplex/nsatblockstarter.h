
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nsatblockstarter.h
 *  \brief Provides a hard-coded list of saturated blocks to use as starting
 *  points for recognising larger Seifert fibred spaces.
 */

#ifndef __NSATBLOCKSTARTER_H
#ifndef __DOXYGEN
#define __NSATBLOCKSTARTER_H
#endif

#include "subcomplex/nsatblock.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nlistoncall.h"

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
 * This is a support class for NSatBlockStarterSet, and as such it is a
 * read-only class to the rest of the world.
 *
 * This class is well-suited for subcomplex testing: if the
 * triangulation here is found to be a subcomplex of some larger
 * triangulation (see NTriangulation::isContainedIn()), then the
 * corresponding isomorphism can be used to copy this block structure
 * and transform it to describe the corresponding block in the larger
 * triangulation.
 *
 * As such, one of the core uses of this class is as a starting point
 * for identifying regions within triangulations that are formed by
 * joining saturated blocks together along their boundary annuli.  See
 * the routines NSatBlockStarterSearcher::findStarterBlocks() and
 * NSatRegion::expand() for implementations of this.
 *
 * \ifacespython Not present.
 */
class NSatBlockStarter : regina::boost::noncopyable {
    private:
        NTriangulation triangulation_;
            /**< The triangulation of the saturated block. */
        NSatBlock* block_;
            /**< Structural details of the saturated block. */

    public:
        /**
         * Destroys both the internal triangulation and block structure.
         */
        ~NSatBlockStarter();

        /**
         * Returns a reference to the triangulation of the saturated
         * block.
         *
         * @return the block triangulation.
         */
        const NTriangulation& triangulation() const;

        /**
         * Returns details that describe the structure of the saturated
         * block.
         *
         * @return the block structure.
         */
        const NSatBlock* block() const;

    private:
        /**
         * Creates a new starter block.  The triangulation will be empty
         * and the block pointer set to null.
         *
         * The triangulation must be fleshed out and the block structure
         * created before this object can be used.
         */
        NSatBlockStarter();

    friend class NSatBlockStarterSet;
};

/**
 * Represents a set of starter blocks that can be used for identifying
 * triangulations of Seifert fibred spaces.
 *
 * This class provides a list of saturated blocks that can be used as
 * starting points for recognising triangulations; see the
 * NSatBlockStarter class notes for details.
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
class NSatBlockStarterSet : private NListOnCall<NSatBlockStarter> {
    public:
        /**
         * An iterator over the starter blocks in this list.  This operates
         * as a forward iterator in a manner consistent with the standard C++
         * library.
         */
        typedef NListOnCall<NSatBlockStarter>::iterator iterator;

    private:
        static const NSatBlockStarterSet blocks;
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
        NSatBlockStarterSet();
};

/**
 * A helper class for locating and using starter blocks within a
 * triangulation.
 *
 * This class provides a means for searching for each starter
 * block in the global hard-coded NSatBlockStarterSet within a
 * given triangulation.  More specifically, given some triangulation \a t,
 * this class can locate every isomorphic embedding of every starter
 * block in the global NSatBlockStarterSet as a subcomplex of \a t (see
 * NTriangulation::isContainedIn() for what is meant by "isomorphic
 * embedding").
 *
 * The routine findStarterBlocks() runs the search.  Each time an
 * isomorphic embedding of a starter block is discovered within the
 * given triangulation, the pure virtual routine useStarterBlock() will
 * be called.  The block that is passed to useStarterBlock() will be a
 * new block that refers to the particular embedding of the starter block
 * within the given triangulation (as opposed to the original block
 * structure referring to the prebuilt triangulation in NSatBlockStarter).
 *
 * For each situation that requires searching for starter blocks, a
 * subclass of NSatBlockStarterSearcher will be required.  This subclass
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
class NSatBlockStarterSearcher {
    protected:
        NSatBlock::TetList usedTets;
            /**< Keeps track of which tetrahedra have used by the
                 current embedding of the current starter block.
                 See useStarterBlock() for further details. */

    public:
        /**
         * Destroys this object and its internal structures.
         */
        virtual ~NSatBlockStarterSearcher();

        /**
         * Runs a search for every isomorphic embedding of every
         * starter block from the global NSatBlockStarterSet within the
         * given triangulation.  Each time an embedding is discovered,
         * the pure virtual routine useStarterBlock() will be called.
         *
         * See the NSatBlockStarterSearcher class notes for greater
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
        void findStarterBlocks(NTriangulation* tri);

    protected:
        /**
         * Used by subclasses to process each starter block embedding that
         * is found.
         *
         * Suppose that the main search routine findStarterBlocks() has
         * been called with some triangulation \a t.  Each time it
         * locates an isomorphic embedding of a starter block within \a t,
         * it will call useStarterBlock().  Subclasses of
         * NSatBlockStarterSearcher should therefore override
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
         * NSatBlock::isBlock() or NSatRegion::expand() as the list of
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
        virtual bool useStarterBlock(NSatBlock* starter) = 0;
};

/*@}*/

// Inline functions for NSatBlockStarter

inline NSatBlockStarter::NSatBlockStarter() : block_(0) {
}

inline NSatBlockStarter::~NSatBlockStarter() {
    if (block_)
        delete block_;
}

inline const NTriangulation& NSatBlockStarter::triangulation() const {
    return triangulation_;
}

inline const NSatBlock* NSatBlockStarter::block() const {
    return block_;
}

// Inline functions for NSatBlockStarterSet

inline NSatBlockStarterSet::NSatBlockStarterSet() {
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::begin() {
    return blocks.NListOnCall<NSatBlockStarter>::begin();
}

inline NSatBlockStarterSet::iterator NSatBlockStarterSet::end() {
    return blocks.NListOnCall<NSatBlockStarter>::end();
}

// Inline functions for NSatBlockStarterSearcher

inline NSatBlockStarterSearcher::~NSatBlockStarterSearcher() {
}

} // namespace regina

#endif

