
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ncensus.h
 *  \brief Deals with forming a census of all triangulations of a given
 *  size.
 */

#ifndef __NCENSUS_H
#ifndef __DOXYGEN
#define __NCENSUS_H
#endif

#include "census/nfacepairing.h"
#include "utilities/nbooleans.h"

namespace regina {

class NGluingPerms;
class NPacket;
class NProgressManager;
class NProgressMessage;
class NTriangulation;

/**
 * \addtogroup census Census of Triangulations
 * Census building for 3-manifold triangulations.
 * @{
 */

/**
 * A routine used to determine whether a particular triangulation should be
 * included in a census.  Such routines are used by NCensus::formCensus()
 * and NCensus::findAllCompletions().
 *
 * The first parameter passed should be a triangulation currently under
 * consideration.
 * The second parameter may contain arbitrary data as passed to
 * NCensus::formCensus() or NCensus::findAllCompletions().
 *
 * The return value should be \c true if the triangulation passed should
 * be included in the census, or \c false otherwise.
 */
typedef bool (*AcceptTriangulation)(NTriangulation*, void*);

/**
 * A utility class used to form a complete census of 3-manifold
 * triangulations satisfying certain constraints.  Other tasks (such as
 * finding all completions of a triangulation with boundary) are also
 * offered.
 *
 * \testpart
 */
class NCensus {
    public:
        static const int PURGE_NON_MINIMAL;
            /**< Indicates that non-minimal triangulations may be ignored. */
        static const int PURGE_NON_PRIME;
            /**< Indicates that any triangulation that is not prime (i.e.,
                 can be written as a non-trivial connected sum) and any
                 bounded triangulation that is reducible over a disc may be
                 ignored. */
        static const int PURGE_NON_MINIMAL_PRIME;
            /**< Indicates that any triangulation that is not prime (i.e.,
                 can be written as a non-trivial connected sum), any
                 bounded triangulation that is reducible over a disc and
                 any triangulation that is non-minimal may be ignored.
                 Note that this is simply a combination of the constants
                 \a PURGE_NON_MINIMAL and \a PURGE_NON_PRIME. */
        static const int PURGE_P2_REDUCIBLE;
            /**< Indicates that any triangulation containing an embedded
                 two-sided projective plane may be ignored. */

    private:
        NPacket* parent;
            /**< The argument passed to formCensus(). */
        NBoolSet finiteness;
            /**< The argument passed to formCensus(). */
        NBoolSet orientability;
            /**< The argument passed to formCensus(). */

        int whichPurge;
            /**< The argument passed to formCensus(). */

        AcceptTriangulation sieve;
            /**< The arbitrary constraint function to run triangulations
                 through. */
        void* sieveArgs;
            /**< The second argument to pass to function \a sieve. */

        NProgressMessage* progress;
            /**< Reports the current state of progress of the census
                 generation.  This will be 0 if progress reporting is
                 not required. */

        unsigned long whichSoln;
            /**< The number of the solution we are up to. */

    public:
        /**
         * Fills the given packet with all triangulations in a census of
         * 3-manifold triangulations satisfying the given constraints.
         * Each triangulation in the census will appear as a child of the
         * given packet.
         *
         * This routine will conduct a census of all valid triangulations
         * containing a given number of tetrahedra.  All such triangulations
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
         * Parameter \a whichPurge may be used to further avoid constructing
         * triangulations satisfying particular constraints (such as
         * non-minimality).  This can significantly speed up the census.
         * In this case however not all such triangulations will be
         * avoided, but it is guaranteed that every triangulation that
         * does \e not satisfy the constraints defined by \a whichPurge
         * will be produced.
         *
         * Only valid triangulations will be produced; see
         * NTriangulation::isValid() for further details.
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
         * @param nTetrahedra the number of tetrahedra in each triangulation
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
         * and/or without boundary faces.  The set should contain \c true
         * if triangulations with boundary faces are to be included, and
         * should contain \c false if triangulations with only internal
         * faces are to be included.
         * @param nBdryFaces specifies the precise number of boundary faces
         * that should be present in the triangulations produced.  If this
         * parameter is negative, it is ignored and no additional restriction
         * is imposed.  See the documentation for routine
         * NFacePairing::findAllPairings() for details regarding this
         * parameter and how it interacts with parameter \a boundary.
         * @param whichPurge specifies which triangulations we may further 
         * avoid constructing (see the function notes above for details).
         * This should be a bitwise OR of purge constants defined in this
         * class, or 0 if no additional pruning should take place.
         * If a variety of purge constants are bitwise ORed together, a
         * triangulation satisfying \e any of these constraints may be
         * avoided.  Note that not all such triangulations will be
         * avoided, but enough are avoided that the performance increase
         * is noticeable.
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
        static unsigned long formCensus(NPacket* parent, unsigned nTetrahedra,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFaces, int whichPurge, AcceptTriangulation sieve = 0,
            void* sieveArgs = 0, NProgressManager* manager = 0);

        /**
         * Fills the given packet with all triangulations in a partial census
         * of 3-manifold triangulations satisfying the given constraints.
         * Each triangulation in the partial census will appear as a child of
         * the given packet.
         *
         * This routine will conduct a census of all valid triangulations
         * that are modelled by the given tetrahedron face pairing.
         * All such triangulations are included in the census up to
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
         * Parameter \a whichPurge may be used to further avoid constructing
         * triangulations satisfying particular constraints (such as
         * non-minimality).  The use of this parameter, combined with
         * parameters \a finiteness and \a orientability, can significantly
         * speed up the census.  For some combinations of these
         * parameters entirely different algorithms are used.
         *
         * Note however that not all triangulations described by
         * parameter \a whichPurge will be avoided.  It is guaranteed
         * however that every triangulation that does \e not satisfy the
         * constraints defined by \a whichPurge will be produced.
         *
         * Only valid triangulations will be produced; see
         * NTriangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the partial census
         * contains a small enough total number of triangulations to
         * avoid any memory disasters.
         *
         * The partial census will run in the current thread.  This
         * routine will only return once the partial census is complete.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         *
         * \ifacespython Parameters \a sieve and \a sieveArgs
         * are not present (and will be treated as 0).
         *
         * @param pairing the tetrahedron face pairing that
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
         * @param whichPurge specifies which triangulations we may further 
         * avoid constructing (see the function notes above for details).
         * This should be a bitwise OR of purge constants defined in this
         * class, or 0 if no additional pruning should take place.
         * If a variety of purge constants are bitwise ORed together, a
         * triangulation satisfying \e any of these constraints may be
         * avoided.  Note that not all such triangulations will be
         * avoided, but enough are avoided that the performance increase
         * is noticeable.
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
        static unsigned long formPartialCensus(const NFacePairing* pairing,
            NPacket* parent, NBoolSet finiteness, NBoolSet orientability,
            int whichPurge, AcceptTriangulation sieve = 0, void* sieveArgs = 0);

        /**
         * Determines whether the given triangulation even has a chance
         * at being minimal.  This routine can be passed as parameter
         * \a sieve to routine NCensus::formCensus() to exclude obviously
         * non-minimal triangulations from a census.
         *
         * A variety of tests will be performed; these tests are subject
         * to change between Regina releases.  Currently this routine
         * counts vertices and also tries to simplify the triangulation using
         * NTriangulation::simplifyToLocalMinimum().
         *
         * Currently this routine is only useful for triangulations whose
         * faces are all internal; if the given triangulation has
         * boundary faces then this routine will simply return \c true.
         *
         * \ifacespython Parameter \a ignore is not present (and will
         * be treated as 0).
         *
         * @param tri the triangulation to examine.
         * @param ignore a parameter that is ignored.
         * @return \c false if the given triangulation is known to be
         * non-minimal, or \c true if minimality of the given
         * triangulation has not been determined.
         */
        static bool mightBeMinimal(NTriangulation* tri, void* ignore);

        /**
         * Fills the given packet with all completions of the given base
         * triangulation.  The base triangulation should have boundary
         * faces; a completion is simply a new triangulation formed from
         * the base triangulation by gluing all of the boundary faces to
         * each other in some fashion (a completion will have no
         * boundary faces at all).
         *
         * Each completion of the given base triangulation will appear
         * as a child of the given parent packet.
         *
         * This routine currently enumerates \e all completions,
         * regardless of combinatorial isomorphism.  This behaviour may
         * change when this routine become more mature.
         *
         * The set of completions can be optionally restricted to only
         * include triangulations satisfying further constraints (such
         * as orientability and finiteness); see the individual
         * parameter descriptions for further details.  In particular,
         * parameter \a sieve can be used to impose artibrary
         * restrictions that are not hard-coded into this class.
         *
         * Note that if constraints may be imposed using the hard-coded
         * parameters (such as orientability and finiteness), it is
         * generally better to do this than to use the arbitrary
         * constraint parameter \a sieve.  Hard-coded parameters will be
         * tested earlier, and some (such as orientability) can be
         * incorporated directly into the completion algorithm to give a
         * vast performance increase.
         *
         * Only valid triangulations will be produced; see
         * NTriangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the set of
         * completions is small enough to avoid any memory disasters.
         *
         * If a progress manager is passed, the calculation will run in a new
         * thread and this routine will return immediately.  Otherwise
         * the calculation will run in the current thread and this
         * routine will only return once the census is complete.
         *
         * \todo \bugurgent This routine currently does nothing!
         *
         * \ifacespython Parameters \a sieve, \a sieveArgs and \a manager
         * are not present (and will be treated as 0).
         *
         * @param parent the packet beneath which the completions that
         * are constructed will be placed.
         * @param base the base triangulation from which completions
         * will be generated.
         * @param finiteness determines whether to include finite and/or ideal
         * triangulations.  The set should contain \c true if finite (non-ideal)
         * triangulations are to be included, and should contain \c false if
         * ideal triangulations are to be included.
         * @param orientability determines whether to include orientable
         * and/or non-orientable triangulations.  The set should contain \c true
         * if orientable triangulations are to be included, and should contain
         * \c false if non-orientable triangulations are to be included.
         * @param sieve an additional constraint function that may be
         * used to exclude certain triangulations from the set of results.
         * If this parameter is non-zero, each triangulation produced (after
         * passing all other criteria) will be passed through this
         * function.  If this function returns \c true then the triangulation
         * will be included in the set of results; otherwise it will not.
         * When this function is called, the first (triangulation)
         * argument will be a completion under consideration for
         * inclusion in the results.  The second argument will be
         * parameter \a sieveArgs as passed to findAllCompletions().
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
         * @return the total number of completions produced, or 0 if
         * a progress manager was passed.
         */
        static unsigned long findAllCompletions(NPacket* parent,
            NTriangulation* base, NBoolSet finiteness,
            NBoolSet orientability, AcceptTriangulation sieve = 0,
            void* sieveArgs = 0, NProgressManager* manager = 0);

    private:
        /**
         * Creates a new structure to hold the given information.
         * All parameters not explained are taken directly from
         * formCensus().
         *
         * \pre \a nTetrahedra is at least 1.
         *
         * @param newProgress the object with which to report progress
         * for the census generation, or 0 if progress reporting is not
         * required.
         */
        NCensus(NPacket* newParent, const NBoolSet& newFiniteness,
            const NBoolSet& newOrientability, int newWhichPurge,
            AcceptTriangulation newSieve, void* newSieveArgs,
            NProgressMessage* newProgress);

        /**
         * Called when a particular tetrahedron face pairing has been
         * found.  This routine hooks up the face pairing generation with
         * the gluing permutation generation.
         *
         * @param pairing the face pairing that has been found.
         * @param autos the set of all automorphisms of the given face
         * pairing.
         * @param census the census currently being generated;
         * this must really be of class NCensus.
         */
        static void foundFacePairing(const NFacePairing* pairing,
            const NFacePairingIsoList* autos, void* census);

        /**
         * Called when a particular set of gluing permutations has been
         * found.  This routine generates the corresponding triangulation
         * and decides whether it really belongs in the census.
         *
         * @param perms the gluing permutation set that has been found.
         * @param census the census currently being generated;
         * this must really be of class NCensus.
         */
        static void foundGluingPerms(const NGluingPerms* perms, void* census);
};

/*@}*/

} // namespace regina

#endif

