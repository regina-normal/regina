
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_SIGCENSUS_H
#ifndef __DOXYGEN
#define __REGINA_SIGCENSUS_H
#endif

#include <functional>
#include <list>
#include "regina-core.h"
#include "split/signature.h"
#include "split/sigisomorphism.h"

namespace regina {

/**
 * A class responsible for building censuses of splitting surface signatures.
 *
 * The main entry point for building a census is the static routine
 * formCensus().  End users cannot create their own objects of this class.
 *
 * See formCensus() for further information.
 *
 * \ingroup split
 */
class SigCensus {
    public:
        /**
         * A list of partial isomorphisms on splitting surface signatures.
         */
        using IsoList = std::list<SigPartialIsomorphism>;

    private:
        Signature sig;
            /**< The signature being constructed. */
        unsigned nextLabel;
            /**< The first symbol that has not yet been used. */
        unsigned* used;
            /**< The number of times each symbol has been used so far. */
        IsoList* automorph;
            /**< List `automorph[k]` represents all automorphisms
                 of the first \a k cycle groups of the partially formed
                 signature. */

        std::function<void(const Signature&, const IsoList&)> action_;
            /**< The action that was passed to formCensus(), also
                 encapsulating all of its user-supplied arguments. */

        size_t totalFound;
            /**< The total number of signatures found so far. */

    public:
        /**
         * Forms a census of all splitting surface signatures of the given
         * order.  The order of a signature is the number of quads in the
         * corresponding splitting surface.
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
         * length.  Each cycle will have at least half of its symbols
         * lower-case.
         *
         * For each signature that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference to a
         *   Signature.  This will be the signature that was found.
         *   If \a action wishes to keep the signature, it should take a
         *   deep copy (not a reference), since the signature may be
         *   changed and reused after \a action returns.
         *
         * - The second argument to \a action must be a const reference to a
         *   SigCensus::IsoList.  This will be the list of all automorphisms of
         *   the signature that was found.  Again, if \a action wishes to keep
         *   these automorphisms, it should take a deep copy of this list.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * \warning Currently upper-case symbols in signatures are not supported
         * by this routine; only signatures whose symbols are all lower-case
         * will be produced.
         *
         * \todo \feature Add support for symbols of differing case.
         *
         * \python This function is available, and \a action may be a
         * pure Python function.  However, \a action cannot take any additional
         * arguments beyond the signature and its automorphisms (and
         * therefore the additional \a args list is omitted here).
         *
         * \param order the order of signatures to generate.
         * \param action a function (or other callable object) to call for each
         * signature that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial signature and automorphism
         * arguments.
         * \return the total number of non-equivalent signatures that were
         * found.
         */
        template <typename Action, typename... Args>
        static size_t formCensus(unsigned order, Action&& action,
            Args&&... args);

        /**
         * Deallocates any memory used by this structure.
         */
        ~SigCensus();

        // Make this class non-copyable.
        SigCensus(const SigCensus&) = delete;
        SigCensus& operator = (const SigCensus&) = delete;

    private:
        /**
         * Creates a new structure to form a signature census.
         * All parameters are taken directly from formCensus().
         *
         * \pre order is at least 1.
         */
        template <typename Action, typename... Args>
        SigCensus(unsigned order, Action&& action, Args&&... args);

        /**
         * Runs a complete signature census generation.  At most one
         * copy of this routine should be running at any given time for
         * a particular SigCensus.
         *
         * \return the total number of non-equivalent signatures that were
         * found.
         */
        size_t run();

        /**
         * Extend the automorphisms in list
         * `automorph[sig.nCycleGroups - 1]` to form the
         * automorphisms in list `automorph[sig.nCycleGroups]`.
         *
         * If in the processing of extending these automorphisms it is
         * discovered that the partial signature `sig` is not in
         * canonical form, \c false will be returned and the contents of
         * list `automorph[sig.nCycleGroups]` will be undefined.
         *
         * \return \c true if and only if it was confirmed during
         * processing that the partial signature `sig` is in
         * canonical form.
         */
        bool extendAutomorphisms();

        /**
         * Extends the partial signature created so far to add a new
         * cycle.
         *
         * \param cycleLen the length of the new cycle to add.
         * \param newCycleGroup \c true if and only if the new cycle
         * begins a new cycle group.
         * \param startPos the position within the list of symbols
         * that make up the signature at which the new cycle will begin.
         */
        void tryCycle(unsigned cycleLen, bool newCycleGroup, unsigned startPos);
};

// Inline functions for SigCensus

template <typename Action, typename... Args>
inline size_t SigCensus::formCensus(unsigned order, Action&& action,
        Args&&... args) {
    return SigCensus(order, std::forward<Action>(action),
        std::forward<Args>(args)...).run();
}

template <typename Action, typename... Args>
inline SigCensus::SigCensus(unsigned order, Action&& action, Args&&... args) :
        sig(order), used(new unsigned[order]),
        automorph(new IsoList[order + 2]),
        action_([&](const Signature& s, const IsoList& isos) {
            action(s, isos, std::forward<Args>(args)...);
        }) {
}

inline SigCensus::~SigCensus() {
    delete[] used;
    delete[] automorph;
}

} // namespace regina

#endif

