
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

/*! \file census/dim4census.h
 *  \brief Deals with forming a census of all 4-manifold triangulations
 *  of a given size.
 */

#ifndef __DIM4CENSUS_H
#ifndef __DOXYGEN
#define __DIM4CENSUS_H
#endif

#include "census/dim4facetpairing.h"
#include "utilities/nbooleans.h"

namespace regina {

class Dim4GluingPerms;
class Dim4GluingPermSearcher;
class Dim4Triangulation;
class NPacket;
class NProgressManager;
class NProgressMessage;

/**
 * \weakgroup census
 * @{
 */

/**
 * A utility class used to form a complete census of 4-manifold
 * triangulations satisfying certain constraints.
 *
 * \testpart
 */
class Dim4Census {
    public:
        /**
         * A routine used to determine whether a particular 4-manifold
         * triangulation should be included in a census.  Routines of this
         * type are used by Dim4Census::formCensus().
         *
         * The first parameter passed should be a triangulation currently
         * under consideration.
         * The second parameter may contain arbitrary data as passed to
         * Dim4Census::formCensus().
         *
         * The return value should be \c true if the triangulation passed
         * should be included in the census, or \c false otherwise.
         */
        typedef bool (*AcceptTriangulation)(Dim4Triangulation*, void*);

    private:
        NPacket* parent_;
            /**< The argument passed to formCensus(). */
        NBoolSet finiteness_;
            /**< The argument passed to formCensus(). */
        NBoolSet orientability_;
            /**< The argument passed to formCensus(). */

        AcceptTriangulation sieve_;
            /**< The arbitrary constraint function to run triangulations
                 through. */
        void* sieveArgs_;
            /**< The second argument to pass to function \a sieve. */

        NProgressMessage* progress_;
            /**< Reports the current state of progress of the census
                 generation.  This will be 0 if progress reporting is
                 not required. */

        unsigned long whichSoln_;
            /**< The number of the solution we are up to. */

    public:
        /**
         * Fills the given packet with all triangulations in a census of
         * 4-manifold triangulations satisfying the given constraints.
         * Each triangulation in the census will appear as a child of the
         * given packet.
         *
         * This routine will conduct a census of all valid triangulations
         * containing a given number of pentachora.  All such triangulations
         * are included in the census up to combinatorial isomorphism; given
         * any isomorphism class, exactly one representative will appear in
         * the census.
         *
         * The census can be optionally restricted to only include
         * triangulations satisfying further constraints (such as
         * orientability and finiteness); see the individual parameter
         * descriptions for further details.  In particular, parameter
         * \a sieve can be used to impose arbitrary restrictions that are
         * not hard-coded into this class.
         *
         * Note that if constraints may be imposed using the hard-coded
         * parameters (such as orientability and finiteness), it is
         * generally better to do this than to use the arbitrary
         * constraint parameter \a sieve.  Hard-coded parameters will be
         * tested earlier, and some (such as orientability) can be
         * incorporated directly into the census algorithm to give a vast
         * performance increase.
         *
         * Only valid triangulations will be produced; see
         * Dim4Triangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the census
         * contains a small enough total number of triangulations to
         * avoid any memory disasters.
         *
         * If a progress manager is passed, the calculation will run in a new
         * thread and this routine will return immediately.  Otherwise
         * the calculation will run in the current thread and this
         * routine will only return once the census is complete.
         *
         * \ifacespython Parameters \a sieve, \a sieveArgs and \a manager
         * are not present (and will be treated as 0).
         *
         * @param parent the packet beneath which members of the census will
         * be placed.
         * @param nPentachora the number of pentachora in each triangulation
         * in the census.
         * @param finiteness determines whether to include finite and/or ideal
         * triangulations.  The set should contain \c true if finite (non-ideal)
         * triangulations are to be included, and should contain \c false if
         * ideal triangulations are to be included.
         * @param orientability determines whether to include orientable
         * and/or non-orientable triangulations.  The set should contain \c true
         * if orientable triangulations are to be included, and should contain
         * \c false if non-orientable triangulations are to be included.
         * @param boundary determines whether to include triangulations with
         * and/or without boundary facets.  The set should contain \c true
         * if triangulations with boundary facets are to be included, and
         * should contain \c false if triangulations with only internal
         * facets are to be included.
         * @param nBdryFacets specifies the precise number of boundary facets
         * that should be present in the triangulations produced.  If this
         * parameter is negative, it is ignored and no additional restriction
         * is imposed.  See the documentation for routine
         * Dim4FacetPairing::findAllPairings() for details regarding this
         * parameter and how it interacts with parameter \a boundary.
         * @param sieve an additional constraint function that may be
         * used to exclude certain triangulations from the census.  If
         * this parameter is non-zero, each triangulation produced (after
         * passing all other criteria) will be passed through this
         * function.  If this function returns \c true then the triangulation
         * will be included in the census; otherwise it will not.
         * When this function is called, the first (triangulation)
         * argument will be a triangulation under consideration for
         * inclusion in the census.  The second argument will be
         * parameter \a sieveArgs as passed to formCensus().
         * Parameter \a sieve may be passed as \c null (in which case no
         * additional constraint function will be used).
         * @param sieveArgs the pointer to pass as the final parameter
         * for the function \a sieve which will be called upon each
         * triangulation found.  If \a sieve is \c null then \a sieveArgs
         * will be ignored.
         * @param manager a progress manager via which progess will be reported,
         * or 0 if no progress reporting is required.  If non-zero,
         * \a manager must point to a progress manager for which
         * NProgressManager::isStarted() is still \c false.
         * @return the number of triangulations produced in the census, or 0 if
         * a progress manager was passed.
         */
        static unsigned long formCensus(NPacket* parent, unsigned nPentachora,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFacets, AcceptTriangulation sieve = 0,
            void* sieveArgs = 0, NProgressManager* manager = 0);

        /**
         * Fills the given packet with all triangulations in a partial census
         * of 4-manifold triangulations satisfying the given constraints.
         * Each triangulation in the partial census will appear as a child of
         * the given packet.
         *
         * This routine will conduct a census of all valid 4-manifold
         * triangulations that are modelled by the given pentachoron facet
         * pairing.  All such triangulations are included in the census up to
         * combinatorial isomorphism; given any isomorphism class, exactly
         * one representative will appear in the census.
         *
         * The census can be optionally restricted to only include
         * triangulations satisfying further constraints (such as
         * orientability and finiteness); see the individual parameter
         * descriptions for further details.  In particular, parameter
         * \a sieve can be used to impose arbitrary restrictions that are
         * not hard-coded into this class.
         *
         * Note that if constraints may be imposed using the hard-coded
         * parameters (such as orientability and finiteness), it is
         * generally better to do this than to use the arbitrary
         * constraint parameter \a sieve.  Hard-coded parameters will be
         * tested earlier, and some (such as orientability) can be
         * incorporated directly into the census algorithm to give a vast
         * performance increase.
         *
         * Only valid triangulations will be produced; see
         * Dim4Triangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the partial census
         * contains a small enough total number of triangulations to
         * avoid any memory disasters.
         *
         * The partial census will run in the current thread.  This
         * routine will only return once the partial census is complete.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         *
         * \ifacespython Parameters \a sieve and \a sieveArgs
         * are not present (and will be treated as 0).
         *
         * @param pairing the pentachoron facet pairing that
         * triangulations in this partial census must be modelled by.
         * @param parent the packet beneath which members of the partial
         * census will be placed.
         * @param finiteness determines whether to include finite and/or ideal
         * triangulations.  The set should contain \c true if finite (non-ideal)
         * triangulations are to be included, and should contain \c false if
         * ideal triangulations are to be included.
         * @param orientability determines whether to include orientable
         * and/or non-orientable triangulations.  The set should contain \c true
         * if orientable triangulations are to be included, and should contain
         * \c false if non-orientable triangulations are to be included.
         * @param sieve an additional constraint function that may be
         * used to exclude certain triangulations from the census.  If
         * this parameter is non-zero, each triangulation produced (after
         * passing all other criteria) will be passed through this
         * function.  If this function returns \c true then the triangulation
         * will be included in the census; otherwise it will not.
         * When this function is called, the first (triangulation)
         * argument will be a triangulation under consideration for
         * inclusion in the census.  The second argument will be
         * parameter \a sieveArgs as passed to formPartialCensus().
         * Parameter \a sieve may be passed as \c null (in which case no
         * additional constraint function will be used).
         * @param sieveArgs the pointer to pass as the final parameter
         * for the function \a sieve which will be called upon each
         * triangulation found.  If \a sieve is \c null then \a sieveArgs
         * will be ignored.
         * @return the number of triangulations produced in the partial census.
         */
        static unsigned long formPartialCensus(const Dim4FacetPairing* pairing,
            NPacket* parent, NBoolSet finiteness, NBoolSet orientability,
            AcceptTriangulation sieve = 0, void* sieveArgs = 0);

    private:
        /**
         * Creates a new structure to hold the given information.
         * All parameters not explained are taken directly from
         * formCensus().
         *
         * @param progress the object with which to report progress
         * for the census generation, or 0 if progress reporting is not
         * required.
         */
        Dim4Census(NPacket* parent, const NBoolSet& finiteness,
            const NBoolSet& orientability,
            AcceptTriangulation sieve, void* sieveArgs,
            NProgressMessage* progress);

        /**
         * Called when a particular pentachoron facet pairing has been
         * found.  This routine hooks up the facet pairing generation with
         * the gluing permutation generation.
         *
         * @param pairing the facet pairing that has been found.
         * @param autos the set of all automorphisms of the given facet
         * pairing.
         * @param census the census currently being generated;
         * this must really be of class Dim4Census.
         */
        static void foundFacetPairing(const Dim4FacetPairing* pairing,
            const Dim4FacetPairingIsoList* autos, void* census);

        /**
         * Called when a particular set of gluing permutations has been
         * found.  This routine generates the corresponding triangulation
         * and decides whether it really belongs in the census.
         *
         * \pre The given set of gluing permutations is complete, i.e.,
         * it is not just a partially-complete search state.  Partial
         * searches are formed by calling NGluingPermSearcher::runSearch()
         * with a non-negative depth argument.
         *
         * @param perms the gluing permutation set that has been found.
         * @param census the census currently being generated;
         * this must really be of class Dim4Census.
         */
        static void foundGluingPerms(const Dim4GluingPermSearcher* perms,
            void* census);
};

/*@}*/

} // namespace regina

#endif

