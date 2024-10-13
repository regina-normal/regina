
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file manifold/sfs.h
 *  \brief Deals with general Seifert fibred spaces.
 */

#ifndef __REGINA_SFS_H
#ifndef __DOXYGEN
#define __REGINA_SFS_H
#endif

#include <list>
#include <optional>
#include "regina-core.h"
#include "manifold.h"

namespace regina {

class AbelianGroup;
class LensSpace;

/**
 * Represents an exceptional (\a alpha, \a beta) fibre in a Seifert
 * fibred space.
 *
 * The first parameter \a alpha must be strictly positive, and
 * the two parameters \a alpha and \a beta must be coprime.
 *
 * Note that we allow regular fibres with \a alpha = 1, and we do not
 * impose range limits on \a beta (thus \a beta may be negative, or it
 * may be larger than \a alpha).  This is to allow more flexibility in
 * routines such as SFSpace::insertFibre().
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \warning In Regina 4.2.1 and earlier, this class was named
 * NExceptionalFibre.  The new SFSFibre class was introduced with
 * Regina 4.3, and has changed its behaviour (in particular, the
 * natural ordering of fibres has changed).  Code that was written
 * to work with the old NExceptionalFibre class should be looked at closely
 * before being adapted to the new SFSFibre class (i.e., it may require
 * more than just substituting class names).
 *
 * \ingroup manifold
 */
struct SFSFibre {
    long alpha;
        /**< The first parameter of this (\a alpha, \a beta) fibre.
             Note that this is the index of the exceptional fibre.
             This parameter must always be strictly positive. */
    long beta;
        /**< The second parameter of this (\a alpha, \a beta) fibre.
             This parameter must have no common factors with \a alpha. */

    /**
     * Creates a new uninitialised exceptional fibre.
     */
    constexpr SFSFibre() = default;
    /**
     * Creates a new exceptional fibre with the given parameters.
     *
     * \param newAlpha the first parameter (the index) of this
     * exceptional fibre; this must be strictly positive.
     * \param newBeta the second parameter of this exceptional fibre;
     * this must have no common factors with the first parameter \a newAlpha.
     */
    constexpr SFSFibre(long newAlpha, long newBeta);
    /**
     * Creates a new exceptional fibre that is a clone of the given
     * fibre.
     */
    constexpr SFSFibre(const SFSFibre&) = default;

    /**
     * Makes this exceptional fibre a clone of the given fibre.
     */
    SFSFibre& operator = (const SFSFibre&) = default;
    /**
     * Determines if this and the given exceptional fibre are identical.
     * This requires both fibres to have the same values for \a alpha
     * and the same values for \a beta.
     *
     * \return \c true if and only if this and the given fibre are
     * identical.
     */
    constexpr bool operator == (const SFSFibre&) const = default;
    /**
     * Compares two exceptional fibres.
     * Fibres are ordered first by \a alpha and then by \a beta.
     *
     * This generates all of the usual comparison operators, including
     * `<`, `<=`, `>`, and `>=`.
     *
     * \python This spaceship operator `x <=> y` is not available, but the
     * other comparison operators that it generates _are_ available.
     *
     * \return The result of the comparison between this and the given fibre.
     */
    constexpr std::strong_ordering operator <=> (const SFSFibre&) const =
        default;
};

/**
 * Writes the given fibre in human-readable format to the given output stream.
 * The fibre will be written in the format `(alpha,beta)` with no newline.
 *
 * \param out the output stream to which to write.
 * \param f the fibre to write.
 * \return the output stream \a out.
 *
 * \ingroup manifold
 */
std::ostream& operator << (std::ostream& out, const SFSFibre& f);

/**
 * Represents a general Seifert fibred space, which may be orientable or
 * non-orientable.  Punctures and reflector boundaries in the base orbifold
 * are supported.
 *
 * A Seifert fibred space whose base orbifold has no punctures or
 * reflector boundaries can be placed into one of the six classes
 * \c o1, \c o2, \c n1, \c n2, \c n3 and \c n4, as detailed on page 88
 * of "Seifert Manifolds", Peter Orlik, Springer-Verlag, 1972.
 * These classes describe whether this base surface is orientable, as well
 * as how many of its generators give fibre-reversing paths in the 3-manifold.
 *
 * In the case where the base orbifold has punctures and/or reflector
 * boundaries, we use the five simplified classes \c bo1, \c bo2, \c bn1,
 * \c bn2 and \c bn3.  These classes are not standard terminology (i.e.,
 * they have been created explicitly for Regina), and generally they do not
 * provide enough information to uniquely identify the 3-manifold.  They do
 * however identify whether or not the base orbifold is orientable,
 * and whether or not it contains any fibre-reversing paths.
 *
 * When describing punctures and reflector boundaries, a _twisted_
 * boundary is one that gives a fibre-reversing path, and an _untwisted_
 * boundary is one around which the direction of fibres is preserved.
 *
 * Exceptional fibres are sorted first by \a alpha (the index) and then
 * by \a beta.  The obstruction constant \a b is stored separately,
 * though in output routines such as name() and structure() it is
 * merged in with the exceptional fibres.  Specifically, it is merged in
 * with the \a beta of the final exceptional fibre (replacing it with
 * `beta + b.alpha`), or if there are no exceptional fibres then
 * it is presented as a single (1,b) fibre.
 *
 * The Manifold routines homology() and construct() are only
 * implemented in some cases.  The homology() routine is
 * implemented if and only if the base orbifold has no punctures.
 * The construct() routine is implemented only for lens spaces and
 * Seifert fibred spaces over the 2-sphere without punctures or reflector
 * boundaries.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * SFSpace still requires a non-trivial (but constant sized) amount of data to
 * be copied even in a move operation.
 *
 * \warning In Regina 4.2.1 and earlier, this class was named NSFS.
 * As of Regina 4.3, this class was renamed due to significant changes of
 * behaviour (it became more general, and also now keeps the obstruction
 * parameter \a b separate).  Code that was written to work with the old
 * NSFS class should be looked at closely before being adapted to the new
 * SFSpace class (i.e., it may require more than just substituting class names).
 *
 * \todo \featurelong Implement recognition of more common names.
 * \todo \featurelong Implement triangulation construction and homology
 * calculation for more Seifert fibred spaces.
 *
 * \ingroup manifold
 */
class SFSpace : public Manifold {
    public:
        /**
         * Lists the six classes \c o1, \c o2, \c n1, \c n2, \c n3, \c n4
         * for base orbifolds without boundaries, plus five classes
         * \c bo1, \c b02, \c bn1, \c bn2, \c bn3 for base orbifolds
         * with boundaries.
         */
        enum class Class {
            /**
             * Indicates that the base orbifold is orientable with
             * no punctures or reflector boundaries, and that none
             * of its generators give fibre-reversing paths.
             */
            o1 = 101,
            /**
             * Indicates that the base orbifold is orientable with
             * no punctures or reflector boundaries, and that all
             * of its generators give fibre-reversing paths.
             */
            o2 = 102,
            /**
             * Indicates that the base orbifold is non-orientable with
             * no punctures or reflector boundaries, and that none
             * of its generators give fibre-reversing paths.
             */
            n1 = 201,
            /**
             * Indicates that the base orbifold is non-orientable with
             * no punctures or reflector boundaries, and that all of
             * its generators give fibre-reversing paths.
             */
            n2 = 202,
            /**
             * Indicates that the base orbifold is non-orientable with
             * no punctures or reflector boundaries, that it has
             * non-orientable genus at least two, and that precisely
             * one of its generators gives a fibre-reversing path.
             */
            n3 = 203,
            /**
             * Indicates that the base orbifold is non-orientable with
             * no punctures or reflector boundaries, that it has
             * non-orientable genus at least three, and that precisely
             * two of its generators give fibre-reversing paths.
             */
            n4 = 204,

            /**
             * Indicates that the base orbifold contains punctures
             * and/or reflector boundaries, that it is orientable,
             * and that it contains no fibre-reversing paths.
             */
            bo1 = 301,
            /**
             * Indicates that the base orbifold contains punctures
             * and/or reflector boundaries, that it is orientable,
             * and that it contains at least one fibre-reversing path.
             */
            bo2 = 302,
            /**
             * Indicates that the base orbifold contains punctures
             * and/or reflector boundaries, that it is non-orientable,
             * and that it contains no fibre-reversing paths.
             */
            bn1 = 401,
            /**
             * Indicates that the base orbifold contains punctures
             * and/or reflector boundaries, that it is non-orientable,
             * and that its fibre-reversing paths correspond precisely
             * to its orientation-reversing paths.
             */
            bn2 = 402,
            /**
             * Indicates that the base orbifold contains punctures
             * and/or reflector boundaries, that it is non-orientable,
             * that it contains at least one fibre-reversing path,
             * and that its fibre-reversing paths do not correspond
             * precisely to its orientation-reversing paths.
             */
            bn3 = 403
        };

        /**
         * A deprecated type alias that represents a class of base orbifold.
         *
         * \deprecated This has been renamed to Class, and is now a scoped
         * enumeration.
         */
        using ClassType [[deprecated]] = Class;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::o1.
         */
        [[deprecated]] inline static constexpr Class o1 = Class::o1;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::o2.
         */
        [[deprecated]] inline static constexpr Class o2 = Class::o2;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::n1.
         */
        [[deprecated]] inline static constexpr Class n1 = Class::n1;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::n2.
         */
        [[deprecated]] inline static constexpr Class n2 = Class::n2;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::n3.
         */
        [[deprecated]] inline static constexpr Class n3 = Class::n3;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::n4.
         */
        [[deprecated]] inline static constexpr Class n4 = Class::n4;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::bo1.
         */
        [[deprecated]] inline static constexpr Class bo1 = Class::bo1;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::bo2.
         */
        [[deprecated]] inline static constexpr Class bo2 = Class::bo2;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::bn1.
         */
        [[deprecated]] inline static constexpr Class bn1 = Class::bn1;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::bn2.
         */
        [[deprecated]] inline static constexpr Class bn2 = Class::bn2;

        /**
         * A deprecated constant indicating one of the base orbifold classes.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Class::bn3.
         */
        [[deprecated]] inline static constexpr Class bn3 = Class::bn3;

    private:
        Class class_;
            /**< Indicates which of the classes above this space belongs to. */
        unsigned long genus_;
            /**< The genus of the base orbifold.  For non-orientable
                 base orbifolds this is the non-orientable genus. */
        unsigned long punctures_;
            /**< The number of punctures in the base orbifold whose
                 boundaries are fibre-preserving.  This only counts
                 ordinary boundary components, not reflector boundary
                 components. */
        unsigned long puncturesTwisted_;
            /**< The number of punctures in the base orbifold whose
                 boundaries are fibre-reversing.  This only counts
                 ordinary boundary components, not reflector boundary
                 components. */
        unsigned long reflectors_;
            /**< The number of reflector boundary components in the
                 base orbifold whose boundaries are fibre-preserving.
                 These are in addition to the regular boundary components
                 described by \a punctures_. */
        unsigned long reflectorsTwisted_;
            /**< The number of reflector boundary components in the
                 base orbifold whose boundaries are fibre-reversing.
                 These are in addition to the regular boundary components
                 described by \a puncturesTwisted_. */

        std::list<SFSFibre> fibres_;
            /**< The exceptional fibres.  This list will be sorted, and will
                 only contain fibres for which \a alpha and \a beta are
                 coprime and 0 ≤ \a beta < \a alpha > 1. */
        unsigned long nFibres_;
            /**< The size of the \a fibres_ list, used to avoid calling
                 the linear time fibres_.size(). */
        long b_;
            /**< The obstruction parameter \a b, which corresponds to an
                 additional (1,b) fibre. */

    public:
        /**
         * Creates a new Seifert fibred space with base orbifold the
         * 2-sphere and no exceptional fibres.
         */
        SFSpace();
        /**
         * Creates a new Seifert fibred space of the given class with the
         * given base orbifold and no exceptional fibres.
         *
         * \pre If there are no punctures or reflector boundary components,
         * then \a useClass is one of the six classes \c o1, \c o2, \c n1,
         * \c n2, \c n3 or \c n4.  Likewise, if there are punctures and/or
         * reflector boundary components, then \a useClass is one of the
         * five classes \c bo1, \c bo2, \c bn1, \c bn2 or \c bn3.
         * \pre If there are any twisted punctures or reflector boundary
         * components, then \a useClass is either \c bo2 or \c bn3.
         *
         * \param useClass indicates whether the base orbifold is closed
         * and/or orientable, and gives information about fibre-reversing
         * paths in the 3-manifold.  See the SFSpace class notes and the
         * Class enumeration notes for details.
         * \param genus the genus of the base orbifold (the
         * number of tori or projective planes that it contains).
         * Note that for non-orientable base surfaces, this is the
         * non-orientable genus.
         * \param punctures the number of untwisted ordinary boundary
         * components of the base orbifold.  Here "ordinary" means that
         * the puncture gives rise to a real 3-manifold boundary (i.e.,
         * this is not a reflector boundary of the base orbifold).
         * \param puncturesTwisted the number of twisted ordinary boundary
         * components of the base orbifold.  Here "ordinary" means that
         * the puncture gives rise to a real 3-manifold boundary (i.e.,
         * this is not a reflector boundary of the base orbifold).
         * \param reflectors the number of untwisted reflector boundary
         * components of the base orbifold.  These are in addition to
         * the ordinary boundary components described by \a punctures.
         * \param reflectorsTwisted the number of twisted reflector boundary
         * components of the base orbifold.  These are in addition to
         * the ordinary boundary components described by \a puncturesTwisted.
         */
        SFSpace(Class useClass, unsigned long genus,
            unsigned long punctures = 0, unsigned long puncturesTwisted = 0,
            unsigned long reflectors = 0, unsigned long reflectorsTwisted = 0);
        /**
         * Creates a new copy of the given Seifert fibred space.
         */
        SFSpace(const SFSpace&) = default;
        /**
         * Moves the contents of the given Seifert fibred space into this
         * new Seifert fibred space.  This is a fast (constant time) operation.
         *
         * The space that was passed will no longer be usable.
         */
        SFSpace(SFSpace&&) noexcept = default;

        /**
         * Sets this to be a copy of the given Seifert fibred space.
         */
        SFSpace& operator = (const SFSpace&) = default;
        /**
         * Moves the contents of the given Seifert fibred space into this
         * Seifert fibred space.  This is a fast (constant time) operation.
         *
         * The space that was passed will no longer be usable.
         *
         * \return a reference to this space.
         */
        SFSpace& operator = (SFSpace&&) noexcept = default;
        /**
         * Swaps the contents of this and the given Seifert fibred space.
         *
         * \param other the space whose contents should be swapped with this.
         */
        void swap(SFSpace& other) noexcept;

        /**
         * Returns which of the eleven predefined classes this space
         * belongs to.  The specific class indicates whether the
         * base orbifold has punctures and/or reflector boundaries,
         * whether the base orbifold is orientable, and gives information
         * on fibre-reversing paths.
         *
         * The class can be (indirectly) modified by calling
         * addHandle(), addCrosscap(), addPuncture() or addReflector().
         *
         * For more information on the eleven predefined classes, see the
         * SFSpace class notes or the Class enumeration notes.
         *
         * \return the particular class to which this space belongs.
         */
        Class baseClass() const;
        /**
         * Returns the genus of the base orbifold.  All punctures and
         * reflector boundaries in the base orbifold are ignored (i.e.,
         * they are treated as though they had been replaced with ordinary
         * filled discs).
         *
         * The genus is the number of tori or projective planes that the
         * base surface is formed from.  In particular, if the base
         * surface is non-orientable then this is the non-orientable genus.
         *
         * \return the genus of the base orbifold.
         */
        unsigned long baseGenus() const;
        /**
         * Returns whether or not the base surface is orientable.
         * Reflector boundary components of the base orbifold are not
         * considered here.
         *
         * The orientability of the base surface can be (indirectly)
         * modified by calling addCrosscap().
         *
         * \return \c true if and only if the base surface is orientable.
         */
        bool baseOrientable() const;
        /**
         * Returns whether or not this space contains any fibre-reversing
         * paths.
         *
         * \return \c true if and only if a fibre-reversing path exists.
         */
        bool fibreReversing() const;
        /**
         * Returns whether or not we can negate an exceptional fibre by
         * passing it around the interior of the base orbifold.  That is,
         * this routine determines whether a (\a p, \a q) exceptional
         * fibre can become a (\a p, -\a q) exceptional fibre simply by
         * sliding it around.
         *
         * This is possible if either
         * - the base orbifold has an orientation-reversing loop that
         *   does not reverse fibres in the 3-manifold, or
         * - the base orbifold has an orientation-preserving loop that
         *   does reverse fibres in the 3-manifold.
         *
         * Note that reflector boundary components, whilst making the
         * overall 3-manifold non-orientable, have no bearing on the
         * outcome of this routine.
         *
         * \return \c true if and only an exceptional fibre can be
         * reflected as described above.
         */
        bool fibreNegating() const;
        /**
         * Returns the total number of punctures in the base orbifold.
         * In other words, this routine returns the total number of real
         * torus or Klein bottle boundary components in the overall
         * 3-manifold.
         *
         * Note that reflector boundaries on the base orbifold are _not_
         * counted here; only the ordinary boundary components that give rise
         * to real 3-manifold boundaries are included.
         *
         * Both untwisted and twisted punctures (giving rise to torus
         * and Klein bottle boundaries respectively in the 3-manifold)
         * are counted by this routine.
         *
         * \return the total number of punctures.
         */
        unsigned long punctures() const;
        /**
         * Returns the number of punctures of the given type in the base
         * orbifold.  In other words, this routine returns the number of
         * real boundary components of the given type in the overall
         * 3-manifold.
         *
         * This routine either counts only twisted punctures (which give
         * rise to Klein bottle boundaries), or only untwisted punctures
         * (which give rise to torus boundaries).
         *
         * Either way, reflector boundaries on the base orbifold are
         * _not_ counted here; only ordinary boundary components that
         * give rise to real 3-manifold boundaries are considered.
         *
         * \param twisted \c true if only twisted punctures should be
         * counted (those that give fibre-reversing paths and Klein
         * bottle boundaries), or \c false if only untwisted punctures
         * should be counted (those that are fibre-preserving and give
         * torus boundaries).
         * \return the number of punctures of the given type.
         */
        unsigned long punctures(bool twisted) const;
        /**
         * Returns the total number of reflector boundary components of the
         * base orbifold.  This includes both twisted and untwisted
         * reflector boundaries.
         *
         * \return the total number of reflector boundary components.
         */
        unsigned long reflectors() const;
        /**
         * Returns the number of reflector boundary components of the
         * given type in the base orbifold.  This either counts only twisted
         * reflector boundaries, or only untwisted reflector boundaries.
         *
         * \param twisted \c true if only twisted reflector boundaries
         * should be counted (those that give fibre-reversing paths), or
         * \c false if only untwisted reflector boundaries should be counted.
         * \return the number of reflector boundaries of the given type.
         */
        unsigned long reflectors(bool twisted) const;

        /**
         * Returns the number of exceptional fibres in this Seifert fibred
         * space.
         *
         * Note that the obstruction parameter \a b is not included in
         * this count.  That is, any (1,k) fibres are ignored.
         *
         * \return the number of exceptional fibres.
         */
        unsigned long fibreCount() const;
        /**
         * Returns the requested exceptional fibre.  Fibres are stored
         * in sorted order by \a alpha (the index) and then by \a beta.
         * See the SFSpace class notes for details.
         *
         * \warning This routine takes linear time (specifically,
         * linear in the argument \a which).
         *
         * \param which determines which fibre to return; this must be between
         * 0 and getFibreCount()-1 inclusive.
         * \return the requested fibre.
         */
        SFSFibre fibre(unsigned long which) const;

        /**
         * Returns the obstruction constant \a b for this Seifert fibred
         * space.
         *
         * The obstruction constant corresponds to the insertion of an
         * additional (1,\a b) fibre.  It can be modified by calling
         * insertFibre() with a value of \a alpha = 1.  It will also be
         * modified whenever insertFibre() is called with \a beta out of
         * range (\a beta < 0 or \a beta ≥ \a alpha), since each exceptional
         * fibre must be stored in standard form (0 ≤ \a beta < \a alpha).
         *
         * \return the obstruction constant \a b.
         */
        long obstruction() const;

        /**
         * Inserts a new handle into the base orbifold.
         *
         * This increases the orientable genus of the base orbifold by
         * one, or the non-orientable genus by two.  It is equivalent to
         * removing a disc from the base orbifold and replacing it with
         * a punctured torus.
         *
         * Note that this operation may alter which of the classes
         * described by Class this space belongs to.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * \param fibreReversing \c true if one or both generators of
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
         * Note that this operation may alter which of the classes
         * described by Class this space belongs to.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * \param fibreReversing \c true if the generator of the new
         * crosscap should give a fibre-reversing curve in the
         * overall 3-manifold, or \c false (the default) if it should
         * preserve the directions of the fibres.
         */
        void addCrosscap(bool fibreReversing = false);
        /**
         * Inserts one or more new punctures into the base orbifold.
         * The punctures may be twisted or untwisted.
         *
         * Each puncture insertion is equivalent to removing a disc from
         * the base orbifold.  In the untwisted case this results in a
         * new torus boundary for the 3-manifold, and in the twisted
         * case it results in a new Klein bottle boundary.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * \param twisted \c true if the new punctures should be twisted
         * (i.e., their boundaries should be fibre-reversing), or \c false
         * if the new punctures should be untwisted.
         * \param nPunctures the number of new punctures to insert.
         */
        void addPuncture(bool twisted = false, unsigned long nPunctures = 1);
        /**
         * Adds one or more new reflector boundary components to the base
         * orbifold.  The new reflector boundaries may be twisted or
         * untwisted.
         *
         * Each addition of a reflector boundary component is equivalent to
         * removing a disc from the base orbifold and replacing it with an
         * annulus with one reflector boundary.
         *
         * In the untwisted case, it has the effect of removing a trivially
         * fibred solid torus from the overall 3-manifold and replacing it
         * with an appropriately fibred twisted I-bundle over the torus.
         *
         * The exceptional fibres and the obstruction constant \a b are
         * not modified by this routine.
         *
         * \param twisted \c true if the new reflector boundaries should be
         * twisted (i.e., the boundaries should be fibre-reversing), or
         * \c false if the new reflector boundaries should be untwisted.
         * \param nReflectors the number of new reflector boundaries to add.
         */
        void addReflector(bool twisted = false, unsigned long nReflectors = 1);

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
         * 0 ≤ \a beta < \a alpha, it will be pulled back into this
         * range and the excess will be pushed into the obstruction
         * constant \a b.
         *
         * \exception InvalidArgument \a alpha is zero.
         *
         * \param fibre the fibre to insert.  The first parameter of
         * this fibre (i.e., its index) must be strictly positive, and
         * the two parameters of this fibre must be coprime.
         */
        void insertFibre(const SFSFibre& fibre);

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
         * 0 ≤ \a beta < \a alpha, it will be pulled back into this
         * range and the excess will be pushed into the obstruction
         * constant \a b.
         *
         * \exception InvalidArgument \a alpha is zero.
         *
         * \param alpha the first parameter (i.e., the index) of the
         * fibre to insert; this must be strictly positive.
         * \param beta the second parameter of the fibre to insert; this
         * must have no common factors with the first parameter \a alpha.
         */
        void insertFibre(long alpha, long beta);

        /**
         * Replaces this space with its mirror image.  Specifically, all
         * exceptional fibres and the obstruction constant \a b will be
         * negated.  Note that the obstruction constant will generally
         * undergo further change as the exceptional fibres are
         * standardised into the usual 0 ≤ \a beta < \a alpha form.
         *
         * This routine will not change the curves made by the fibres
         * and the base orbifold on any boundary components (i.e.,
         * boundaries caused by punctures in the base orbifold), with
         * the exception that each base curve will be reflected.
         *
         * \warning The space is _not_ reduced after reflecting.
         * It may be that the space can be further simplified
         * (especially in the case of non-orientable manifolds).
         */
        void reflect();

        /**
         * Replaces each exceptional fibre of the form (\a alpha, \a beta)
         * with a fibre of the form (\a alpha, \a alpha - \a beta).
         * The obstruction constant \a b is not touched.
         */
        void complementAllFibres();

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
         * \param mayReflect \c true if we are allowed to take a mirror
         * image of the entire 3-manifold, or \c false if we are not.
         */
        void reduce(bool mayReflect = true);

        /**
         * Determines if this Seifert fibred space is a Lens space.
         *
         * \return a structure containing the details of this Lens
         * space, or \nullopt if this is not a Lens space.
         */
        std::optional<LensSpace> isLensSpace() const;

        /**
         * Determines whether this and the given object contain precisely
         * the same presentations of the same Seifert fibred space.
         *
         * This routine does _not_ test for homeomorphism.  Instead it
         * compares the exact presentations, including the precise details of
         * the base orbifold and the exact parameters of the exceptional fibres,
         * and determines whether or not these _presentations_ are identical.
         * If you have two different presentations of the same Seifert fibred
         * space, they will be treated as not equal by this routine.
         *
         * \param compare the presentation with which this will be compared.
         * \return \c true if and only if this and the given object contain
         * identical presentations of the same Seifert fibred space.
         */
        bool operator == (const SFSpace& compare) const;

        /**
         * Compares representations of two Seifert fibred spaces according to
         * an aesthetic ordering.
         *
         * The only purpose of this routine is to implement a consistent
         * ordering of Seifert fibred space representations.  The specific
         * ordering used is purely aesthetic on the part of the author, and
         * is subject to change in future versions of Regina.
         *
         * It does not matter whether the two spaces are homeomorphic; this
         * routine compares the specific _representations_ of these spaces
         * (and so in particular, different representations of the same
         * Seifert fibred space will be ordered differently).
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the other representation to compare this with.
         * \return A result that indicates how this and the given Seifert
         * fibred space representation should be ordered with respect to
         * each other.
         */
        std::strong_ordering operator <=> (const SFSpace& rhs) const;

        Triangulation<3> construct() const override;
        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        std::ostream& writeStructure(std::ostream& out) const override;

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
         * \return the iterator that was immediately after the given
         * iterator before this routine was called.  Note that the given
         * iterator will be invalidated.
         */
        std::list<SFSFibre>::iterator negateFibreDown(
            std::list<SFSFibre>::iterator it);
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
         * \param out the output stream to which to write.
         * \param tex \c true if we are writing in TeX format, or
         * \c false if we are writing in plain text format.
         * \return a reference to \a out.
         */
        std::ostream& writeCommonBase(std::ostream& out, bool tex) const;

        /**
         * Provides the implementation of writeStructure() in both TeX
         * and plain formats.  This is so that both writeName() and
         * writeTeXName() can call upon it if required.
         *
         * \param out the output stream to which to write.
         * \param tex \c true if we are writing in TeX format, or
         * \c false if we are writing in plain text format.
         * \return a reference to \a out.
         */
        std::ostream& writeCommonStructure(std::ostream& out, bool tex) const;

        /**
         * Provides the implementation of both routines writeName() and
         * writeTeXName().  These routines are implemented together in
         * writeCommonName() since they share a common internal structure.
         *
         * \param out the output stream to which to write.
         * \param tex \c true if we are handling writeTeXName(), or
         * \c false if we are handling writeName().
         * \return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;
};

/**
 * Swaps the contents of the two given Seifert fibred spaces.
 *
 * This global routine simply calls SFSpace::swap(); it is provided so
 * that SFSpace meets the C++ Swappable requirements.
 *
 * \param a the first space whose contents should be swapped.
 * \param b the second space whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(SFSpace& a, SFSpace& b) noexcept;

// Inline functions for SFSFibre

inline constexpr SFSFibre::SFSFibre(long newAlpha, long newBeta) :
        alpha(newAlpha), beta(newBeta) {
}

// Inline functions for SFSpace

inline SFSpace::SFSpace() : class_(Class::o1), genus_(0),
        punctures_(0), puncturesTwisted_(0),
        reflectors_(0), reflectorsTwisted_(0),
        nFibres_(0), b_(0) {
}

inline SFSpace::SFSpace(SFSpace::Class useClass, unsigned long genus,
        unsigned long punctures, unsigned long puncturesTwisted,
        unsigned long reflectors, unsigned long reflectorsTwisted) :
        class_(useClass), genus_(genus),
        punctures_(punctures), puncturesTwisted_(puncturesTwisted),
        reflectors_(reflectors), reflectorsTwisted_(reflectorsTwisted),
        nFibres_(0), b_(0) {
}

inline void SFSpace::swap(SFSpace& other) noexcept {
    std::swap(class_, other.class_);
    std::swap(genus_, other.genus_);
    std::swap(punctures_, other.punctures_);
    std::swap(puncturesTwisted_, other.puncturesTwisted_);
    std::swap(reflectors_, other.reflectors_);
    std::swap(reflectorsTwisted_, other.reflectorsTwisted_);
    fibres_.swap(other.fibres_);
    std::swap(nFibres_, other.nFibres_);
    std::swap(b_, other.b_);
}

inline SFSpace::Class SFSpace::baseClass() const {
    return class_;
}

inline unsigned long SFSpace::baseGenus() const {
    return genus_;
}

inline bool SFSpace::baseOrientable() const {
    return (class_ == Class::o1 || class_ == Class::o2 ||
        class_ == Class::bo1 || class_ == Class::bo2);
}

inline bool SFSpace::fibreReversing() const {
    return ! (class_ == Class::o1 || class_ == Class::n1 ||
        class_ == Class::bo1 || class_ == Class::bn1);
}

inline bool SFSpace::fibreNegating() const {
    return ! (class_ == Class::o1 || class_ == Class::n2 ||
        class_ == Class::bo1 || class_ == Class::bn2);
}

inline unsigned long SFSpace::punctures() const {
    return punctures_ + puncturesTwisted_;
}

inline unsigned long SFSpace::punctures(bool twisted) const {
    return (twisted ? puncturesTwisted_ : punctures_);
}

inline unsigned long SFSpace::reflectors() const {
    return reflectors_ + reflectorsTwisted_;
}

inline unsigned long SFSpace::reflectors(bool twisted) const {
    return (twisted ? reflectorsTwisted_ : reflectors_);
}

inline unsigned long SFSpace::fibreCount() const {
    return nFibres_;
}

inline long SFSpace::obstruction() const {
    return b_;
}

inline void SFSpace::insertFibre(const SFSFibre& fibre) {
    insertFibre(fibre.alpha, fibre.beta);
}

inline void SFSpace::reflect() {
    complementAllFibres();
    b_ = -b_ - static_cast<long>(nFibres_);
}

inline bool SFSpace::isHyperbolic() const {
    return false;
}

inline std::ostream& SFSpace::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& SFSpace::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

inline std::ostream& SFSpace::writeStructure(std::ostream& out) const {
    return writeCommonStructure(out, false);
}

inline void swap(SFSpace& a, SFSpace& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

