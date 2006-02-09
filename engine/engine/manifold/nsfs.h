
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

/*! \file nsfs.h
 *  \brief Deals with general Seifert fibred spaces.
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
 *
 * Note that we allow regular fibres with \a alpha = 1, and we do not
 * impose range limits on \a beta (thus \a beta may be negative, or it
 * may be larger than \a alpha).  This is to allow more flexibility in
 * routines such as NSFSpace::insertFibre().
 *
 * \warning In Regina 4.2.1 and earlier, this class was named
 * NExceptionalFibre.  The new NSFSFibre class was introduced with
 * Regina 4.3, and has changed its behaviour (in particular, the
 * natural ordering of fibres has changed).  Code that was written
 * to work with the old NExceptionalFibre class should be looked at closely
 * before being adapted to the new NSFSFibre class (i.e., it may require
 * more than just substituting class names).
 */
struct NSFSFibre {
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
    NSFSFibre();
    /**
     * Creates a new exceptional fibre with the given parameters.
     *
     * @param newAlpha the first parameter (the index) of this
     * exceptional fibre; this must be strictly positive.
     * @param newBeta the second parameter of this exceptional fibre;
     * this must have no common factors with the first parameter \a newAlpha.
     */
    NSFSFibre(long newAlpha, long newBeta);
    /**
     * Creates a new exceptional fibre that is a clone of the given
     * fibre.
     *
     * @param cloneMe the exceptional fibre to clone.
     */
    NSFSFibre(const NSFSFibre& cloneMe);

    /**
     * Makes this exceptional fibre a clone of the given fibre.
     *
     * @param cloneMe the exceptional fibre to clone.
     */
    void operator = (const NSFSFibre& cloneMe);
    /**
     * Determines if this and the given exceptional fibre are identical.
     * This requires both fibres to have the same values for \a alpha
     * and the same values for \a beta.
     *
     * @param compare the fibre with which this will be compared.
     * @return \c true if and only if this and the given fibre are
     * identical.
     */
    bool operator == (const NSFSFibre& compare) const;
    /**
     * Determines if this exceptional fibre is smaller than the given
     * fibre.  Fibres are sorted by \a alpha and then by \a beta.
     *
     * @param compare the fibre with which this will be compared.
     * @return \c true if and only if this is smaller than the given fibre.
     */
    bool operator < (const NSFSFibre& compare) const;
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
std::ostream& operator << (std::ostream& out, const NSFSFibre& f);

/**
 * Represents a general Seifert fibred space, which may be orientable or
 * non-orientable.  Punctures and reflector boundaries in the base orbifold
 * are supported.
 *
 * The Seifert fibred space can be placed into one of the six classes
 * \c o1, \c o2, \c n1, \c n2, \c n3 and \c n4, as detailed on page 88
 * of "Seifert Manifolds", Peter Orlik, Springer-Verlag, 1972.
 * Consider the simplified "base surface", which is the closed surface
 * obtained by replacing all punctures and reflector boundaries in the
 * base orbifold with ordinary discs (this corresponds to replacing the
 * relevant sections of the 3-manifold with trivially fibred solid tori).
 * These classes describe whether this base surface is orientable, as well
 * as how many of its generators give fibre-reversing paths in the 3-manifold.
 *
 * Exceptional fibres are sorted first by \a alpha (the index) and then
 * by \a beta.  The obstruction constant \a b is stored separately,
 * though in output routines such as getName() and getStructure() it is
 * merged in with the exceptional fibres.  Specifically, it is merged in
 * with the \a beta of the final exceptional fibre (replacing it with
 * <tt>beta + b.alpha</tt>), or if there are no exceptional fibres then
 * it is presented as a single (1,b) fibre.
 *
 * The NManifold routines getHomologyH1() and construct() are only
 * implemented in some cases.  The getHomologyH1() routine is
 * implemented if and only if the base orbifold has no punctures or
 * reflector boundary components.  The construct() routine is
 * implemented only for lens spaces and Seifert fibred spaces over the
 * 2-sphere with three exceptional fibres.
 *
 * \warning In Regina 4.2.1 and earlier, this class was named NSFS.
 * The new NSFSpace class was introduced with Regina 4.3, and has changed
 * its behaviour in siginficant ways (becoming more general, as well as
 * keeping the obstruction parameter \a b separate).  Code that was written
 * to work with the old NSFS class should be looked at closely before
 * being adapted to the new NSFSpace class (i.e., it may require more
 * than just substituting class names).
 *
 * \todo Implement recognition of more common names.
 * \todo Implement triangulation construction and homology calculation
 * for more Seifert fibred spaces.
 */
class NSFSpace : public NManifold {
    public:
        /**
         * Lists the six classes \c o1, \c o2, \c n1, \c n2, \c n3 and \c n4
         * as described in the class notes above.
         */
        enum classType {
            o1,
                /**< Indicates that the base surface is orientable, and that
                     none of its generators give fibre-reversing paths. */
            o2,
                /**< Indicates that the base surface is orientable, and that
                     all of its generators give fibre-reversing paths. */
            n1,
                /**< Indicates that the base surface is non-orientable, and
                     that none of its generators give fibre-reversing paths. */
            n2,
                /**< Indicates that the base surface is non-orientable, and
                     that all of its generators give fibre-reversing paths. */
            n3,
                /**< Indicates that the base surface is non-orientable, that it
                     has non-orientable genus at least two, and that precisely
                     one of its generators give a fibre-reversing path. */
            n4
                /**< Indicates that the base surface is non-orientable, that it
                     has non-orientable genus at least three, and that precisely
                     two of its generators give fibre-reversing paths. */
        };
    private:
        classType baseClass;
            /**< Indicates which of the six classes above this space
                 belongs to. */
        unsigned long baseGenus;
            /**< The genus of the base orbifold.  For non-orientable
                 base orbifolds this is the non-orientable genus. */
        unsigned long basePunctures;
            /**< The number of punctures in the base orbifold.  That is,
                 the number of ordinary boundary components.  Reflector
                 boundary components should not be counted here. */
        unsigned long baseReflectors;
            /**< The number of reflector boundary components in the
                 base orbifold.  These are in addition to the regular
                 boundary components described by \a basePunctures. */

        std::list<NSFSFibre> fibres;
            /**< The exceptional fibres.  This list will be sorted, and will
                 only contain fibres for which \a alpha and \a beta are
                 coprime and <tt>0 <= beta < alpha > 1</tt>. */
        unsigned long nFibres;
            /**< The size of the \a fibres list, used to avoid calling
                 the linear time fibres.size(). */
        long b;
            /**< The obstruction parameter \a b, which corresponds to an
                 additional (1,b) fibre. */

    public:
        /**
         * Creates a new Seifert fibred space with base orbifold the
         * 2-sphere and no exceptional fibres.
         */
        NSFSpace();
        /**
         * Creates a new Seifert fibred space of the given class with the
         * given base orbifold and no exceptional fibres.
         *
         * @param newBaseClass indicates whether the base surface is
         * orientable, as well as how many of its generators give
         * fibre-reversing paths in the 3-manifold.  This ignores
         * punctures and reflector boundary components of the base
         * orbifold.  See the NSFSpace class notes and the classType
         * enumeration notes for details.
         * @param newBaseGenus the genus of the base orbifold (the
         * number of tori or projective planes that it contains).
         * Note that for non-orientable base surfaces, this is the
         * non-orientable genus.
         * @param newBasePunctures the number of ordinary boundary
         * components of the base orbifold.  All of these must give rise
         * to real 3-manifold boundaries, i.e., reflector boundary
         * components should not be counted here.
         * @param newBaseReflectors the number of reflector boundary
         * components of the base orbifold.  These are in addition to
         * the ordinary boundary components described by \a
         * newBasePunctures (i.e, they are not modifying these ordinary
         * boundary components).
         */
        NSFSpace(classType newBaseClass, unsigned long newBaseGenus,
            unsigned long newBasePunctures = 0,
            unsigned long newBaseReflectors = 0);
        /**
         * Creates a new Seifert fibred space that is a clone of
         * the given space.
         *
         * @param cloneMe the Seifert fibred space to clone.
         */
        NSFSpace(const NSFSpace& cloneMe);
        /**
         * Destroys this Seifert fibred space.
         */
        virtual ~NSFSpace();

        /**
         * Modifies this Seifert fibred space to be a clone of the given
         * space.
         *
         * @param cloneMe the Seifert fibred space to clone.
         */
        void operator = (const NSFSpace& cloneMe);

        /**
         * Returns which of the six classes \a o1, \a o2, \a n1, \a n2,
         * \a n3 or \a n4 this Seifert fibred space belongs to.
         *
         * The specific class indicates whether or not the base surface
         * is orientable, as well as how many of its generators give
         * fibre-reversing paths in the 3-manifold.  Note that punctures
         * and reflector boundary components in the base orbifold are
         * ignored here.
         *
         * The class can be (indirectly) modified by calling
         * addHandle() or addCrosscap().
         *
         * For more information on the six classes, see the NSFSpace
         * class notes or the classType enumeration notes.
         *
         * @return the particular class to which this space belongs.
         * @see addHandle(), addCrosscap()
         */
        classType getBaseClass() const;
        /**
         * Returns the genus of the base surface.  As with the base
         * class, all punctures and reflector boundaries in the base
         * orbifold are ignored (i.e., are replaced with ordinary
         * filled discs).
         *
         * The genus is the number of tori or projective planes that the
         * base surface is formed from.  In particular, if the base
         * surface is non-orientable then this is the non-orientable genus.
         *
         * @return the genus of the base orbifold.
         * @see addHandle(), addCrosscap()
         */
        unsigned long getBaseGenus() const;
        /**
         * Returns whether or not the base surface is orientable.
         * Reflector boundary components of the base orbifold are not
         * considered here.
         *
         * The orientability of the base surface can be (indirectly)
         * modified by calling addCrosscap().
         *
         * @return \c true if and only if the base surface is orientable.
         * @see addCrosscap()
         */
        bool isBaseOrientable() const;
        /**
         * Returns the number of ordinary boundary components of the
         * base orbifold.  In other words, this routine returns the
         * number of punctures in the base orbifold.
         *
         * Reflector boundary components are not counted here; only the
         * ordinary boundary components that give rise to real 3-manifold
         * boundaries are included.
         *
         * @return the number of ordinary boundary components.
         * @see addPuncture()
         */
        unsigned long getBasePunctures() const;
        /**
         * Returns the number of reflector boundary components of the
         * base orbifold.
         *
         * @return the number of reflector boundary components.
         * @see addReflector()
         */
        unsigned long getBaseReflectors() const;

        /**
         * Returns the number of exceptional fibres in this Seifert fibred
         * space.
         *
         * Note that the obstruction parameter \a b is not included in
         * this count.  That is, any (1,k) fibres are ignored.
         *
         * @return the number of exceptional fibres.
         */
        unsigned long getFibreCount() const;
        /**
         * Returns the requested exceptional fibre.  Fibres are stored
         * in sorted order by \a alpha (the index) and then by \a beta.
         * See the NSFSpace class notes for details.
         *
         * \warning This routine takes linear time (specifically,
         * linear in the argument \a which).
         *
         * @param which determines which fibre to return; this must be between
         * 0 and getFibreCount()-1 inclusive.
         * @return the requested fibre.
         */
        NSFSFibre getFibre(unsigned long which) const;

        /**
         * Returns the obstruction constant \a b for this Seifert fibred
         * space.
         *
         * The obstruction constant corresponds to the insertion of an
         * additional (1,\a b) fibre.  It can be modified by calling
         * insertFibre() with a value of \a alpha = 1.  It will also be
         * modified whenever insertFibre() is called with \a beta out of
         * range (\a beta < 0 or \a beta >= \a alpha), since each exceptional
         * fibre must be stored in standard form (0 <= \a beta < \a alpha).
         *
         * @return the obstruction constant \a b.
         * @see insertFibre()
         */
        long getObstruction() const;

        /**
         * Inserts a new handle into the base orbifold.
         *
         * This increases the orientable genus of the base orbifold by
         * one, or the non-orientable genus by two.  It is equivalent to
         * removing a disc from the base orbifold and replacing it with
         * a punctured torus.
         *
         * Note that this operation may alter which of the six classes
         * this Seifert fibred space belongs to.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * @param fibreReversing \c true if one or both generators of
         * the new handle should give fibre-reversing curves in the
         * overall 3-manifold, or \c false (the default) if both
         * generators should preserve the directions of the fibres.
         */
        void addHandle(bool fibreReversing = false);
        /**
         * Inserts a new crosscap into the base orbifold.
         *
         * This makes the base orbifold non-orientable, and increases
         * its non-orientable genus by one.  It is equivalent to
         * removing a disc from the base orbifold and replacing it with
         * a Mobius band.
         *
         * Note that this operation may alter which of the six classes
         * this Seifert fibred space belongs to.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * @param fibreReversing \c true if the generator of the new
         * crosscap should give a fibre-reversing curve in the
         * overall 3-manifold, or \c false (the default) if it should
         * preserve the directions of the fibres.
         */
        void addCrosscap(bool fibreReversing = false);
        /**
         * Inserts a new puncture into the base orbifold.
         *
         * This is equivalent to removing a disc from the base orbifold
         * (or a trivially fibred solid torus from the overall 3-manifold).
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         */
        void addPuncture();
        /**
         * Inserts several new punctures into the base orbifold.
         *
         * Each puncture insertion is equivalent to removing a disc from
         * the base orbifold (or a trivially fibred solid torus from the
         * overall 3-manifold).
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * @param nPunctures the number of new punctures to insert.
         */
        void addPuncture(unsigned long nPunctures);
        /**
         * Adds a new reflector boundary component to the base orbifold.
         *
         * This is equivalent to removing a disc from the base orbifold
         * and replacing it with an annulus with one reflector boundary.
         *
         * It has the effect of removing a trivially fibred solid torus
         * from the overall 3-manifold and replacing it with an
         * appropriately fibred twisted I-bundle over the torus.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         */
        void addReflector();
        /**
         * Adds several new reflector boundary components to the base orbifold.
         *
         * Each addition of a reflector boundary component is equivalent to
         * removing a disc from the base orbifold and replacing it with an
         * annulus with one reflector boundary.
         *
         * It has the effect of removing a trivially fibred solid torus
         * from the overall 3-manifold and replacing it with an
         * appropriately fibred twisted I-bundle over the torus.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * @param nReflectors the number of new reflector boundaries to add.
         */
        void addReflector(unsigned long nReflectors);

        /**
         * Adds the given fibre to this Seifert fibred space.
         *
         * This may be an exceptional fibre (\a alpha > 1) or it may be
         * a regular fibre (\a alpha = 1).  If it is a regular fibre,
         * the obstruction constant \a b will be adjusted according to
         * the value of \a beta.
         *
         * Note that there is no restriction on the range of the second
         * parameter \a beta.  If it is out of the usual range
         * 0 <= \a beta < \a alpha, it will be pulled back into this
         * range and the excess will be pushed into the obstruction
         * constant \a b.
         *
         * @param fibre the fibre to insert.  The first parameter of
         * this fibre (i.e., its index) must be strictly positive, and
         * the two parameters of this fibre must be coprime.
         */
        void insertFibre(const NSFSFibre& fibre);

        /**
         * Adds the given fibre to this Seifert fibred space.
         *
         * This may be an exceptional fibre (\a alpha > 1) or it may be
         * a regular fibre (\a alpha = 1).  If it is a regular fibre,
         * the obstruction constant \a b will be adjusted according to
         * the value of \a beta.
         *
         * Note that there is no restriction on the range of the second
         * parameter \a beta.  If it is out of the usual range
         * 0 <= \a beta < \a alpha, it will be pulled back into this
         * range and the excess will be pushed into the obstruction
         * constant \a b.
         *
         * @param alpha the first parameter (i.e., the index) of the
         * fibre to insert; this must be strictly positive.
         * @param beta the second parameter of the fibre to insert; this
         * must have no common factors with the first parameter \a alpha.
         */
        void insertFibre(long alpha, long beta);

        /**
         * Reduces the parameters of this Seifert fibred space to a
         * simpler form if possible, without changing the underlying
         * fibration.
         *
         * In some cases the parameters of the Seifert fibred space may
         * be simplified by taking a mirror image of the entire 3-manifold.
         * The argument \a mayReflect signifies whether this is allowed.
         *
         * This routine will not change the curves made by the fibres
         * and the base orbifold on any boundary components (i.e.,
         * boundaries caused by punctures in the base orbifold).
         *
         * \warning If \a mayReflect is \c true then the entire 3-manifold
         * might be replaced with its mirror image, in which case any
         * subsequent modifications (such as inserting additional fibres
         * or altering the base orbifold) may give unexpected results.
         *
         * @param mayReflect \c true if we are allowed to take a mirror
         * image of the entire 3-manifold, or \c false if we are not.
         */
        void reduce(bool mayReflect = true);

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
         * Replaces the fibre (\a alpha, \a beta) at the given iterator
         * with the fibre (\a alpha, \alpha - \a beta) instead.  The fibre
         * is also moved backwards through the list in order to maintain
         * sorted order.
         *
         * The given iterator will be invalidated (since the
         * corresponding list element will be erased).
         *
         * \pre The fibre at the given iterator satisfies
         * \a beta * 2 > \a alpha.
         *
         * @return the iterator that was immediately after the given
         * iterator before this routine was called.  Note that the given
         * iterator will be invalidated.
         */
        std::list<NSFSFibre>::iterator negateFibreDown(
            std::list<NSFSFibre>::iterator it);
        /**
         * Replaces each exceptional fibre of the form (\a alpha, \a beta)
         * with a fibre of the form (\a alpha, \a alpha - \a beta).
         * The obstruction constant \a b is not touched.
         *
         * This routine ensures that the list of exceptional fibres
         * remains in sorted order (assuming it was sorted beforehand).
         */
        void negateAllFibres();

        /**
         * Internal to writeCommonBase().
         *
         * Writes a particular countable feature of the base orbifold to
         * the given output stream in either TeX or plain format.
         */
        static void writeBaseExtraCount(std::ostream& out, unsigned long count,
            const char* object, bool tex);

        /**
         * Writes the base orbifold to the given output stream in either
         * TeX or plain format.
         *
         * @param out the output stream to which to write.
         * @param tex \c true if we are writing in TeX format, or
         * \c false if we are writing in plain text format.
         * @return a reference to \a out.
         */
        std::ostream& writeCommonBase(std::ostream& out, bool tex) const;

        /**
         * Provides the implementation of writeStructure() in both TeX
         * and plain formats.  This is so that both writeName() and
         * writeTeXName() can call upon it if required.
         *
         * @param out the output stream to which to write.
         * @param tex \c true if we are writing in TeX format, or
         * \c false if we are writing in plain text format.
         * @return a reference to \a out.
         */
        std::ostream& writeCommonStructure(std::ostream& out, bool tex) const;

        /**
         * Provides the implementation of both routines writeName() and
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

// Inline functions for NSFSFibre

inline NSFSFibre::NSFSFibre() {
}
inline NSFSFibre::NSFSFibre(long newAlpha, long newBeta) :
        alpha(newAlpha), beta(newBeta) {
}
inline NSFSFibre::NSFSFibre(const NSFSFibre& cloneMe) :
        alpha(cloneMe.alpha), beta(cloneMe.beta) {
}

inline void NSFSFibre::operator = (const NSFSFibre& cloneMe) {
    alpha = cloneMe.alpha;
    beta = cloneMe.beta;
}
inline bool NSFSFibre::operator == (const NSFSFibre& compare)
        const {
    return (alpha == compare.alpha && beta == compare.beta);
}
inline bool NSFSFibre::operator < (const NSFSFibre& compare)
        const {
    return (alpha < compare.alpha ||
        (alpha == compare.alpha && beta < compare.beta));
}

// Inline functions for NSFSpace

inline NSFSpace::NSFSpace() : baseClass(o1), baseGenus(0),
        basePunctures(0), baseReflectors(0), nFibres(0), b(0) {
}

inline NSFSpace::NSFSpace(NSFSpace::classType newBaseClass,
        unsigned long newBaseGenus, unsigned long newBasePunctures,
        unsigned long newBaseReflectors) :
        baseClass(newBaseClass), baseGenus(newBaseGenus),
        basePunctures(newBasePunctures), baseReflectors(newBaseReflectors),
        nFibres(0), b(0) {
}

inline NSFSpace::NSFSpace(const NSFSpace& cloneMe) : NManifold(),
        baseClass(cloneMe.baseClass), baseGenus(cloneMe.baseGenus),
        basePunctures(cloneMe.basePunctures),
        baseReflectors(cloneMe.baseReflectors),
        fibres(cloneMe.fibres), nFibres(cloneMe.nFibres), b(cloneMe.b) {
}

inline NSFSpace::~NSFSpace() {
}

inline NSFSpace::classType NSFSpace::getBaseClass() const {
    return baseClass;
}

inline unsigned long NSFSpace::getBaseGenus() const {
    return baseGenus;
}

inline bool NSFSpace::isBaseOrientable() const {
    return (baseClass == o1 || baseClass == o2);
}

inline unsigned long NSFSpace::getBasePunctures() const {
    return basePunctures;
}

inline unsigned long NSFSpace::getBaseReflectors() const {
    return baseReflectors;
}

inline unsigned long NSFSpace::getFibreCount() const {
    return nFibres;
}

inline long NSFSpace::getObstruction() const {
    return b;
}

inline void NSFSpace::addPuncture() {
    basePunctures++;
}

inline void NSFSpace::addPuncture(unsigned long nPunctures) {
    basePunctures += nPunctures;
}

inline void NSFSpace::addReflector() {
    baseReflectors++;
}

inline void NSFSpace::addReflector(unsigned long nReflectors) {
    baseReflectors += nReflectors;
}

inline void NSFSpace::insertFibre(const NSFSFibre& fibre) {
    insertFibre(fibre.alpha, fibre.beta);
}

inline std::ostream& NSFSpace::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& NSFSpace::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

inline std::ostream& NSFSpace::writeStructure(std::ostream& out) const {
    return writeCommonStructure(out, false);
}

} // namespace regina

#endif

