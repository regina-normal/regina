
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

/*! \file nsfs.h
 *  \brief Deals with general orientable Seifert fibred spaces.
 */

#ifndef __NSFS_H
#ifndef __DOXYGEN
#define __NSFS_H
#endif

#include <list>
#include "nmanifold.h"

namespace regina {

class NAbelianGroup;
class NLensSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents an exceptional (<i>alpha</i>, <i>beta</i>) fibre in a Seifert
 * fibred space.
 *
 * The first parameter \a alpha must be strictly positive, and
 * the two parameters \a alpha and \a beta must be coprime.
 */
struct NExceptionalFibre {
    long alpha;
        /**< The first parameter of this (<i>alpha</i>, <i>beta</i>) fibre.
             Note that this is the index of the exceptional fibre.
             This parameter must always be strictly positive. */
    long beta;
        /**< The second parameter of this (<i>alpha</i>, <i>beta</i>) fibre.
             This parameter must have no common factors with \a alpha. */

    /**
     * Creates a new uninitialised exceptional fibre.
     */
    NExceptionalFibre();
    /**
     * Creates a new exceptional fibre with the given parameters.
     *
     * @param newAlpha the first parameter (the index) of this
     * exceptional fibre; this must be strictly positive.
     * @param newBeta the second parameter of this exceptional fibre;
     * this must have no common factors with the first parameter \a newAlpha.
     */
    NExceptionalFibre(long newAlpha, long newBeta);
    /**
     * Creates a new exceptional fibre that is a clone of the given
     * fibre.
     *
     * @param cloneMe the exceptional fibre to clone.
     */
    NExceptionalFibre(const NExceptionalFibre& cloneMe);

    /**
     * Makes this exceptional fibre a clone of the given fibre.
     *
     * @param cloneMe the exceptional fibre to clone.
     */
    void operator = (const NExceptionalFibre& cloneMe);
    /**
     * Determines if this and the given exceptional fibre are identical.
     *
     * @param compare the fibre with which this will be compared.
     * @return \c true if and only if this and the given fibre are
     * identical.
     */
    bool operator == (const NExceptionalFibre& compare) const;
    /**
     * Determines if this exceptional fibre is smaller than the given
     * fibre.  Fibres are sorted by their indices (their first parameters),
     * with the exception that
     * a fibre of index 1 is larger than any other fibre.  In the case
     * of a tie, fibres are sorted by their second parameter reduced
     * modulo the index.  So, for instance, fibre (3,1) is less than
     * fibre (3,-1) (which is equivalent to (3,2)).  If there is still a
     * tie, the fibres are considered equivalent and this routine will
     * return \c false.
     *
     * @param compare the fibre with which this will be compared.
     * @return \c true if and only if this is smaller than the given fibre.
     */
    bool operator < (const NExceptionalFibre& compare) const;

    friend std::ostream& operator << (std::ostream& out,
        const NExceptionalFibre& f);
};

/**
 * Writes the given fibre in human-readable format to the given output
 * stream.  The fibre will be written in the format
 * <tt>(alpha,beta)</tt> with no newline appended.
 *
 * @param out the output stream to which to write.
 * @param f the fibre to write.
 * @return the output stream \a out.
 */
std::ostream& operator << (std::ostream& out, const NExceptionalFibre& f);

/**
 * Represents a general orientable Seifert fibred space.
 *
 * When queried, this class will return the list of exceptional fibres
 * in standard form.  This is a form similar to that used by Matveev and
 * satisfies the following properties.
 *
 * There will be no fibres of index 1; these will instead be merged with
 * other exceptional fibres.  If there are no exceptional fibres, all
 * fibres of index 1 will be merged into a single (1,k) fibre which will
 * be presented if it is non-trivial (i.e., \a k is non-zero).
 *
 * The exceptional fibres will be sorted as described by
 * NExceptionalFibre::operator<.  All fibres except for the last will
 * have NExceptionalFibre::beta between 0 and NExceptionalFibre::alpha-1
 * inclusive.
 *
 * The NManifold routine getHomologyH1() is implemented for all
 * instances of this class, whereas the NManifold routine construct() is
 * only implemented for a restricted class of Seifert fibred spaces.
 *
 * \todo Implement recognition of more common names.
 * \todo Implement triangulation construction for more Seifert fibred spaces.
 */
class NSFS : public NManifold {
    private:
        unsigned long orbitGenus;
            /**< The genus of the orbit manifold.  For non-orientable
                 orbit manifolds this is the non-orientable genus. */
        bool orbitOrientable;
            /**< Is the orbit manifold orientable? */
        unsigned long orbitPunctures;
            /**< The number of punctures in the orbit manifold. */

        std::list<NExceptionalFibre> fibres;
            /**< The exceptional fibres.  This list will be sorted, will
                 contain no fibres of index <= 1 and will have all its fibres
                 of the form <tt>0 <= beta < alpha</tt> with \a alpha and
                 \a beta coprime. */
        unsigned long nFibres;
            /**< The size of the \a fibres list, used to avoid calling
                 the linear time fibres.size(). */
        long k;
            /**< We will assume there is one additional (1,k) fibre with
                 no restrictions on \a k. */

    public:
        /**
         * Creates a new orientable Seifert fibred space with orbit
         * manifold the sphere and no exceptional fibres.
         */
        NSFS();
        /**
         * Creates a new orientable Seifert fibred space with the given orbit
         * manifold and no exceptional fibres.
         *
         * @param newOrbitGenus the genus of the orbit manifold (the
         * number of tori or projective planes that it contains).
         * @param newOrbitOrientable \c true if and only if the orbit
         * manifold is orientable.
         * @param newOrbitPunctures the number of boundary components of
         * the orbit manifold.
         */
        NSFS(unsigned long newOrbitGenus, bool newOrbitOrientable,
            unsigned long newOrbitPunctures = 0);
        /**
         * Creates a new orientable Seifert fibred space that is a clone of
         * the given space.
         *
         * @param cloneMe the Seifert fibred space to clone.
         */
        NSFS(const NSFS& cloneMe);
        /**
         * Destroys this Seifert fibred space.
         */
        virtual ~NSFS();

        /**
         * Modifies this Seifert fibred space to be a clone of the given
         * space.
         *
         * @param cloneMe the Seifert fibred space to clone.
         */
        void operator = (const NSFS& cloneMe);

        /**
         * Returns the genus of the orbit manifold.  This is the number
         * of tori or projective planes that it contains (once all its
         * boundary components are filled in with discs).
         *
         * @return the genus of the orbit manifold.
         */
        unsigned long getOrbitGenus() const;
        /**
         * Returns whether or not the orbit manifold is orientable.
         *
         * @return \c true if and only if the orbit manifold is
         * orientable.
         */
        bool isOrbitOrientable() const;
        /**
         * Returns the number of boundary components of the orbit
         * manifold.
         *
         * @return the number of boundary components of the orbit
         * manifold.
         */
        unsigned long getOrbitPunctures() const;

        /**
         * Returns the number of exceptional fibres required to describe
         * this Seifert fibred space.
         *
         * Note that (1,k) fibres are not included in this count, with
         * the following exception.  If there are no exceptional fibres
         * but there is a single (1,k) fibre with \a k non-zero, then
         * this routine will return 1 to include this (1,k) fibre.
         *
         * See the general class notes for further details.
         *
         * @return the number of exceptional fibres.
         */
        unsigned long getFibreCount() const;

        /**
         * Returns the requested exceptional fibre.  Note that all
         * fibres will be in standard form, as described in the general
         * class notes.
         *
         * @param which determines which fibre to return; this must be between
         * 0 and getFibreCount()-1 inclusive.
         * @return the requested fibre.
         */
        NExceptionalFibre getFibre(unsigned long which) const;

        /**
         * Adds the given fibre to this Seifert fibred space.
         * Note that there is no restriction on the range of the second
         * parameter of the fibre.  The index of this fibre may be 1.
         *
         * Once this fibre is added, the fibres of this space will be
         * converted to standard form as described in the NSFS class
         * notes.
         *
         * @param fibre the fibre to insert.  The first parameter of
         * this fibre (i.e., its index) must be strictly positive, and
         * the two parameters of this fibre must be coprime.
         */
        void insertFibre(const NExceptionalFibre& fibre);

        /**
         * Adds the given fibre to this Seifert fibred space.
         * Note that there is no restriction on the range of the second
         * parameter of the fibre.  The index of this fibre may be 1.
         *
         * Once this fibre is added, the fibres of this space will be
         * converted to standard form as described in the NSFS class
         * notes.
         *
         * @param alpha the first parameter (i.e., the index) of the
         * fibre to insert; this must be strictly positive.
         * @param beta the second parameter of the fibre to insert; this
         * must have no common factors with the first parameter \a alpha.
         */
        void insertFibre(long alpha, long beta);

        /**
         * Reduces the parameters of this Seifert fibred space to a
         * simpler form if possible, without changing the orbit manifold
         * or the fibres.
         *
         * Currently only one operation is performed.  If the final fibre
         * has negative NExceptionalFibre::beta, consider the integer
         * division <tt>(-beta)/alpha</tt>.  If this quotient can be
         * reduced by negating every fibre in the entire space, this will
         * be done.
         *
         * Note that this operation essentially reflects the 3-manifold,
         * so inserting additional fibres after performing this
         * operation may give unexpected results.
         */
        void reduce();

        /**
         * Determines if this Seifert fibred space is a Lens space.
         *
         * If this is a Lens space, the NLensSpace returned will be
         * newly created and it will be up to the caller
         * of this routine to destroy it.
         *
         * @return a structure containing the details of this Lens
         * space, or \c null if this is not a Lens space.
         */
        NLensSpace* isLensSpace() const;

        NTriangulation* construct() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        std::ostream& writeStructure(std::ostream& out) const;

    private:
        /**
         * Returns the final exceptional fibre modified so that the
         * additional (1,k) fibre is absorbed into it.  Thus it may have
         * second parameter \a beta outside the usual range.
         *
         * If there are no exceptional fibres in this space, the (1,k)
         * fibre itself will be returned.
         *
         * @return the modified final exceptional fibre.
         */
        NExceptionalFibre getModifiedFinalFibre() const;

        /**
         * Provides the actual implementation of routines writeName() and
         * writeTeXName().  These routines are implemented together in
         * writeCommonName() since they share a common internal structure.
         *
         * @param out the output stream to which to write.
         * @param tex \c true if we are handling writeTeXName(), or
         * \c false if we are handling writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;
};

/*@}*/

// Inline functions for NExceptionalFibre

inline NExceptionalFibre::NExceptionalFibre() {
}
inline NExceptionalFibre::NExceptionalFibre(long newAlpha, long newBeta) :
        alpha(newAlpha), beta(newBeta) {
}
inline NExceptionalFibre::NExceptionalFibre(const NExceptionalFibre& cloneMe) :
        alpha(cloneMe.alpha), beta(cloneMe.beta) {
}

inline void NExceptionalFibre::operator = (const NExceptionalFibre& cloneMe) {
    alpha = cloneMe.alpha;
    beta = cloneMe.beta;
}
inline bool NExceptionalFibre::operator == (const NExceptionalFibre& compare)
        const {
    return (alpha == compare.alpha && beta == compare.beta);
}

// Inline functions for NSFS

inline NSFS::NSFS() : orbitGenus(0), orbitOrientable(true), orbitPunctures(0),
        nFibres(0), k(0) {
}

inline NSFS::NSFS(unsigned long newOrbitGenus, bool newOrbitOrientable,
        unsigned long newOrbitPunctures) : orbitGenus(newOrbitGenus),
        orbitOrientable(newOrbitOrientable), orbitPunctures(newOrbitPunctures),
        nFibres(0), k(0) {
}

inline NSFS::NSFS(const NSFS& cloneMe) : NManifold(),
        orbitGenus(cloneMe.orbitGenus),
        orbitOrientable(cloneMe.orbitOrientable),
        orbitPunctures(cloneMe.orbitPunctures), fibres(cloneMe.fibres),
        nFibres(cloneMe.nFibres), k(cloneMe.k) {
}

inline NSFS::~NSFS() {
}

inline unsigned long NSFS::getOrbitGenus() const {
    return orbitGenus;
}

inline bool NSFS::isOrbitOrientable() const {
    return orbitOrientable;
}

inline unsigned long NSFS::getOrbitPunctures() const {
    return orbitPunctures;
}

inline void NSFS::insertFibre(const NExceptionalFibre& fibre) {
    insertFibre(fibre.alpha, fibre.beta);
}

} // namespace regina

#endif

