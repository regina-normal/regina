
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

/*! \file split/sigcensus.h
 *  \brief Deals with forming a census of splitting surface signatures.
 */

#ifndef __SIGCENSUS_H
#ifndef __DOXYGEN
#define __SIGCENSUS_H
#endif

#include <list>
#include "regina-core.h"
#include "split/signature.h"
#include "split/sigisomorphism.h"

namespace regina {

/**
 * \weakgroup split
 * @{
 */

/**
 * A list of partial isomorphisms on splitting surface signatures.
 */
typedef std::list<SigPartialIsomorphism*> SigIsoList;

/**
 * A routine used to do arbitrary processing upon a splitting surface
 * signature and its automorphisms.  Such routines are used to process
 * signatures found when running a signature census.
 *
 * The first parameter passed should be a splitting surface signature.
 * The second parameter should be a list of all automorphisms of this signature.
 * The third parameter may contain arbitrary data as passed to
 * formSigCensus().
 */
typedef void (*UseSignature)(const Signature&, const SigIsoList&, void *);

/**
 * A utility class used by formSigCensus().  Other routines should never
 * refer to this class directly.  It is used to store temporary
 * information when forming the census.
 *
 * \ifacespython Not present.
 */
class REGINA_API SigCensus {
    private:
        Signature sig;
            /**< The signature being constructed. */
        unsigned nextLabel;
            /**< The first symbol that has not yet been used. */
        unsigned* used;
            /**< The number of times each symbol has been used so far. */
        SigIsoList* automorph;
            /**< List <tt>automorph[k]</tt> represents all automorphisms
                 of the first \a k cycle groups of the partially formed
                 signature. */

        UseSignature use;
            /**< The argument passed to formSigCensus(). */
        void* useArgs;
            /**< The argument passed to formSigCensus(). */

        unsigned long totalFound;
            /**< The total number of signatures found so far. */

    public:
        /**
         * Deallocates any memory used specifically by this structure.
         */
        ~SigCensus();

        /**
         * Runs a complete signature census generation.  At most one
         * copy of this routine should be running at any given time for
         * a particular SigCensus.
         *
         * @param param this parameter is ignored.
         * @return 0.
         */
        void* run(void* param);

    private:
        /**
         * Creates a new structure to form a signature census.
         * All parameters not explained are taken directly from
         * formSigCensus().
         *
         * \pre order is at least 1.
         */
        SigCensus(unsigned order, UseSignature newUse, void* newUseArgs);

        /**
         * Empty the list <tt>automorph[sig.nCycleGroups]</tt> and
         * destroy the corresponding partial isomorphisms.
         */
        void clearTopAutomorphisms();

        /**
         * Extend the automorphisms in list
         * <tt>automorph[sig.nCycleGroups - 1]</tt> to form the
         * automorphisms in list <tt>automorph[sig.nCycleGroups]</tt>.
         *
         * If in the processing of extending these automorphisms it is
         * discovered that the partial signature <tt>sig</tt> is not in
         * canonical form, \c false will be returned and the contents of
         * list <tt>automorph[sig.nCycleGroups]</tt> will be undefined.
         *
         * @return \c true if and only if it was confirmed during
         * processing that the partial signature <tt>sig</tt> is in
         * canonical form.
         */
        bool extendAutomorphisms();

        /**
         * Extends the partial signature created so far to add a new
         * cycle.
         *
         * @param cycleLen the length of the new cycle to add.
         * @param newCycleGroup \c true if and only if the new cycle
         * begins a new cycle group.
         * @param startPos the position within the list of symbols
         * that make up the signature at which the new cycle will begin.
         */
        void tryCycle(unsigned cycleLen, bool newCycleGroup, unsigned startPos);

    friend unsigned long formSigCensus(unsigned order, UseSignature use,
        void* useArgs);
};

/**
 * Forms a census of all splitting surface signatures of the given order.
 * The order of a signature is the number of quads in the corresponding
 * splitting surface.
 *
 * Each signature will be produced precisely once up to equivalence.
 * Signatures are considered equivalent if they are related by some
 * combination of:
 * - relabelling symbols;
 * - rotating an individual cycle;
 * - inverting an individual cycle (i.e., reversing the cycle and
 *   changing the case of each symbol in the cycle);
 * - reversing all cycles without changing the case of any symbols.
 *
 * Each signature produced will have its cycles ordered by decreasing
 * length.  Each cycle will have at least half of its symbols lower-case.
 *
 * For each signature that is generated, routine \a use (as passed to this
 * function) will be called with that signature and its automorphisms as
 * arguments.
 *
 * \warning Currently upper-case symbols in signatures are not supported
 * by this routine; only signatures whose symbols are all lower-case will
 * be produced.
 *
 * \todo \feature Add support for symbols of differing case.
 *
 * \ifacespython Not present.
 *
 * @param order the order of signatures to generate.
 * @param use the function to call upon each signature that is found.
 * The first parameter passed to this function will be a splitting
 * surface signature.  The second parameter will be a list of all its
 * automorphisms.  The third parameter will be parameter \a useArgs as
 * was passed to this routine.
 * @param useArgs the pointer to pass as the final parameter for the
 * function \a use which will be called upon each signature found.
 * @return the total number of non-equivalent signatures that were found.
 */
REGINA_API unsigned long formSigCensus(unsigned order, UseSignature use,
    void* useArgs = 0);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSigCensus has now been renamed to SigCensus.
 */
REGINA_DEPRECATED typedef SigCensus NSigCensus;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The typedef NSigIsoList has now been renamed to SigIsoList.
 */
REGINA_DEPRECATED typedef SigIsoList NSigIsoList;

/*@}*/

// Inline functions for SigCensus

inline SigCensus::SigCensus(unsigned order, UseSignature newUse,
        void* newUseArgs) : sig(order), used(new unsigned[order]),
        automorph(new SigIsoList[order + 2]), use(newUse),
        useArgs(newUseArgs) {
}

inline SigCensus::~SigCensus() {
    delete[] used;
    delete[] automorph;
}

} // namespace regina

#endif

