
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

/*! \file nsfs.h
 *  \brief Deals with general orientable Seifert fibred spaces.
 */

#ifndef __NSFS_H
#ifndef __DOXYGEN
#define __NSFS_H
#endif

#include <list>
#include "shareableobject.h"

namespace regina {

class NAbelianGroup;
class NLensSpace;

/**
 * \addtogroup subcomplex Standard Subcomplexes
 * Standard combinatorial subcomplexes in 3-manifold triangulations.
 * @{
 */

/**
 * Represents an exceptional (<i>alpha</i>, <i>beta</i>) fibre in a Seifert
 * fibred space.  The first parameter \a alpha will always be non-negative.
 *
 * \ifaces Not present.
 */
struct NExceptionalFibre {
    long alpha;
        /**< The first parameter of this (<i>alpha</i>, <i>beta</i>) fibre.
             Note that this is the index of the exceptional fibre.
             This parameter should always be non-negative. */
    long beta;
        /**< The second parameter of this (<i>alpha</i>, <i>beta</i>) fibre. */

    /**
     * Creates a new uninitialised exceptional fibre.
     */
    NExceptionalFibre();
    /**
     * Creates a new exceptional fibre with the given parameters.
     *
     * @param newAlpha the first parameter (the index) of this
     * exceptional fibre; this must be non-negative.
     * @param newBeta the second parameter of this exceptional fibre.
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
     * \pre The first parameter of each fibre is non-negative.
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
 * <tt>(alpha, beta)</tt> with no newline appended.
 *
 * @param out the output stream to which to write.
 * @param f the fibre to write.
 * @return the output stream \a out.
 */
std::ostream& operator << (std::ostream& out, const NExceptionalFibre& f);

/**
 * Represents a general orientable Seifert fibred space.
 *
 * This class will store fibres of (illegal) index 0, but in this case
 * there are no guarantees as to the structure of the corresponding
 * 3-manifold, i.e., all bets are off.  If a fibre has index 0, its
 * corresponding NExceptionalFibre::beta will always be presented as 1.
 *
 * When queried, this class will return the list of exceptional fibres
 * in standard form.  This is a form similar to that used by Matveev.
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
 */
class NSFS : public ShareableObject {
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
                 contain no fibres of index 1 and will have all its fibres
                 of the form <tt>0 <= beta < alpha</tt> (unless some alpha
                 is zero, in which case the corresponding beta will be 1). */
        long k;
            /**< We will assume there is one additional (1,k) fibre with
                 no restrictions on \a k. */
        long nNonZeroFibres;
            /**< The number of exceptional fibres stored whose second
                 parameters are non-zero.  This does not include (0,1)
                 fibres. */

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
         * \ifaces Not present.
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
         * Returns the number of exceptional fibres.
         *
         * Note that if there are no exceptional fibres but there is a
         * single (1,k) fibre with \a k non-zero, this count will be 1
         * to include this (1,k) fibre.  See the general class notes for
         * further details.
         *
         * @return the number of exceptional fibres.
         */
        unsigned long getFibreCount() const;

        /**
         * Returns the requested exceptional fibre.  Note that all
         * fibres will be in standard form, as described in the general
         * class notes.
         *
         * \ifaces This routine is replaced with the two routines
         * <tt>getAlpha(index)</tt> and <tt>getBeta(index)</tt>, each
         * returning a <tt>long</tt>, which return the first and second
         * parameters of the requested fibre respectively.
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
         * The index of this fibre may also be 0, but in this case there
         * will be no guarantees as to what the resulting 3-manifold will
         * be; all bets are off.
         *
         * Once this fibre is added, the fibres of this space will be
         * converted to standard form as described in the general class
         * notes.
         *
         * \pre The two parameters of the given fibre must be coprime
         * (have gcd=1).  This is of utmost importance; otherwise all
         * hell may break loose further down the track.
         * \pre The first parameter of the given fibre (its index) must be
         * non-negative.
         *
         * \ifaces Parameter \a fibre is replaced by two <tt>long</tt>
         * parameters \a alpha and \a beta representing the two
         * parameters of the new fibre.
         */
        void insertFibre(const NExceptionalFibre& fibre);

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
         * \todo \bug Currently this routine only recognises a Lens
         * space if its orbit manifold is the 2-sphere.
         *
         * @return a structure containing the details of this Lens
         * space, or \c null if this is not a Lens space.
         */
        NLensSpace* isLensSpace() const;

        /**
         * Returns the first homology group of this Seifert fibred space.
         *
         * The abelian group returned will be newly created and should be
         * destroyed by the caller of this routine once it is no longer
         * required.
         *
         * @return the newly created first homology group.
         */
        NAbelianGroup* getHomologyH1() const;

        /**
         * Returns the common name of this Seifert fibred space.
         * The list of common names for spaces will presumably grow
         * between Regina releases.  If no common name is known at this
         * time, the parameters of this Seifert fibred space will simply
         * be returned in human-readable form.
         *
         * Calling reduce() before invoking this routine increases the
         * chances of a successful identification of a common name.
         *
         * \todo \feature Add more common names to this routine.
         *
         * @return the common name of this Seifert fibred space.
         */
        std::string getCommonName() const;

        void writeTextShort(std::ostream& out) const;

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
        k(0), nNonZeroFibres(0) {
}

inline NSFS::NSFS(unsigned long newOrbitGenus, bool newOrbitOrientable,
        unsigned long newOrbitPunctures) : orbitGenus(newOrbitGenus),
        orbitOrientable(newOrbitOrientable), orbitPunctures(newOrbitPunctures),
        k(0), nNonZeroFibres(0) {
}

inline NSFS::NSFS(const NSFS& cloneMe) : orbitGenus(cloneMe.orbitGenus),
        orbitOrientable(cloneMe.orbitOrientable),
        orbitPunctures(cloneMe.orbitPunctures), fibres(cloneMe.fibres),
        k(cloneMe.k), nNonZeroFibres(cloneMe.nNonZeroFibres) {
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

} // namespace regina

#endif

