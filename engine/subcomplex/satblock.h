
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file subcomplex/satblock.h
 *  \brief Deals with saturated blocks in triangulations of Seifert fibred
 *  spaces.
 */

#ifndef __REGINA_SATBLOCK_H
#ifndef __DOXYGEN
#define __REGINA_SATBLOCK_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "subcomplex/satannulus.h"
#include "triangulation/dim3.h"
#include <set>
#include <tuple>

namespace regina {

class SatBlockModel;
class SFSpace;

/**
 * Represents a saturated block in a Seifert fibred space.  A saturated
 * block is a connected set of tetrahedra built from a subset of fibres
 * (no fibres may enter or exit the boundary of the block).  In addition,
 * the boundary of this block must be a ring of saturated annuli, as
 * described by the SatAnnulus class.  Aside from this ring of saturated
 * annuli, there may be no other boundary triangles within the block.
 *
 * The boundary annuli are numbered consecutively as illustrated below,
 * where the markings 0 and 1 within the triangles represent the first
 * and second triangle of each annulus (see the SatAnnulus class notes for
 * details).  Note that the following diagram is viewed from \e inside
 * the block.
 *
 * <pre>
 *               -+---+---+---+---+---+---+-
 *                |0 /|0 /|0 /|0 /|0 /|0 /|
 *            ... | / | / | / | / | / | / | ...
 *                |/ 1|/ 1|/ 1|/ 1|/ 1|/ 1|
 *               -+---+---+---+---+---+---+-
 * Annulus #  ...  n-2 n-1  0   1   2   3   ...
 * </pre>
 *
 * The ring of boundary annuli may optionally be twisted, so that together
 * the annuli form a long Mobius band.  In this case, for the purposes of
 * labelling and marking annuli, the twist occurs between annuli n-1 and 0.
 * Be careful when dealing with blocks with twisted boundaries, since
 * with twists it is possible to identify an edge with itself in reverse
 * (thus producing something that is not a 3-manifold triangulation).
 *
 * Each saturated block corresponds to a piece of the base orbifold of the
 * larger Seifert fibred space.  For the purpose of connecting the base
 * orbifold together, we assume that the boundary of this particular
 * piece runs horizontally in the diagram above (specifically following
 * the horizontal edges of the boundary annuli, as described in the
 * SatAnnulus class notes).  Insisting on such a boundary may lead to
 * (1,\a k) twists within the block; these are accounted for by the
 * virtual adjustSFS() routine.
 *
 * Saturated blocks are generally joined to one another (or themselves)
 * along their boundary annuli.  For this purpose, each saturated block
 * contains a list of which annulus of this block is adjacent to which
 * annulus of which other block.  Adjacencies may be \e reflected, meaning
 * that the adjacent annulus has its fibres reversed (i.e., the adjacent
 * annulus has undergone an up-to-down reflection); they may also be
 * \e backwards, meaning that the first triangle of one annulus is joined to
 * the second triangle of the other (and vice versa).
 *
 * This is an abstract base class: its subclasses correspond to different
 * combinatorial constructions (or in some cases, parameterised families
 * of constructions).  Each subclass of SatBlock:
 *
 * - must override all pure virtual functions (of course);
 *
 * - should override transform() if the subclass contains additional
 *   data that needs to be altered when an isomorphism is applied;
 *
 * - should not override writeTextLong(), since SatBlock uses short output only.
 *
 * SatBlock does not support value semantics: blocks cannot be copied,
 * swapped, or manually constructed.  Their memory is managed by the
 * SatRegion class (or in special cases the SatBlockModel class), and their
 * locations in memory define them.  See SatRegion for further details.
 *
 * \ingroup subcomplex
 */
class SatBlock : public ShortOutput<SatBlock> {
    public:
        using TetList = std::set<const Tetrahedron<3>*>;
            /**< The data structure used to store a list of tetrahedra
                 that should not be examined when searching for
                 saturated blocks. */

    protected:
        size_t nAnnuli_;
            /**< The number of boundary annuli. */
        SatAnnulus* annulus_;
            /**< Details of each boundary annulus, as seen from the
                 inside of this saturated block. */
        bool twistedBoundary_;
            /**< Is the ring of boundary annuli twisted to form a Mobius
                 band? */

        SatBlock** adjBlock_;
            /**< The saturated block joined to each boundary annulus;
                 this may be null if there is no adjacency or if this
                 information is not known. */
        size_t* adjAnnulus_;
            /**< Describes which specific annulus of the adjacent
                 saturated block is joined to each boundary annulus of this
                 block.  Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */
        bool* adjReflected_;
            /**< Describes whether the adjacency for each boundary
                 annulus is reflected (see the class notes above).
                 Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */
        bool* adjBackwards_;
            /**< Describes whether the adjacency for each boundary
                 annulus is backwards (see the class notes above).
                 Values may be undefined if the corresponding
                 entries in the \a adjBlock array is null. */

    public:
        /**
         * Destroys all internal arrays.  Note that any adjacent blocks
         * that are referenced by the \a adjBlock array will \e not be
         * destroyed.
         */
        virtual ~SatBlock();

        /**
         * Returns the number of annuli on the boundary of this
         * saturated block.
         *
         * @return the number of boundary annuli.
         */
        size_t countAnnuli() const;

        /**
         * Returns details of the requested annulus on the boundary of
         * this saturated block.  Annuli are numbered from 0 to
         * countAnnuli()-1 as described in the class notes.
         *
         * @param which indicates which boundary annulus is requested;
         * this must be between 0 and countAnnuli()-1 inclusive.
         * @return a reference to the requested boundary annulus.
         */
        const SatAnnulus& annulus(size_t which) const;

        /**
         * Is the ring of boundary annuli twisted to form a long Mobius
         * strip?
         *
         * Recall from the class notes that the twist occurs between
         * boundary annuli countAnnuli()-1 and 0.
         *
         * @return \c true if the ring of boundary annuli is twisted, or
         * \c false if not.
         */
        bool twistedBoundary() const;

        /**
         * Returns whether there is another saturated block listed as
         * being adjacent to the given boundary annulus of this block.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @return \c true if the given boundary annulus has an adjacent
         * block listed, or \c false otherwise.
         */
        bool hasAdjacentBlock(size_t whichAnnulus) const;

        /**
         * Returns the saturated block listed as being adjacent to the
         * given boundary annulus of this block.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @return the other block adjacent along this annulus, or \c null
         * if there is no adjacent block listed.
         */
        const SatBlock* adjacentBlock(size_t whichAnnulus) const;

        /**
         * Returns which specific annulus of the adjacent block is
         * listed as being adjacent to the given boundary annulus of
         * this block.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @return the corresponding annulus number on the other block
         * that is adjacent along this annulus.
         */
        size_t adjacentAnnulus(size_t whichAnnulus) const;

        /**
         * Returns whether the adjacency along the given boundary annulus
         * of this block is reflected.  See the class notes for a
         * discussion of reflected adjacencies.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @return \c true if the corresponding adjacency is reflected,
         * or \c false if it is not.
         */
        bool adjacentReflected(size_t whichAnnulus) const;

        /**
         * Returns whether the adjacency along the given boundary annulus
         * of this block is backwards.  See the class notes for a
         * discussion of backwards adjacencies.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @return \c true if the corresponding adjacency is backwards,
         * or \c false if it is not.
         */
        bool adjacentBackwards(size_t whichAnnulus) const;

        /**
         * Adjusts the given Seifert fibred space to insert the contents
         * of this saturated block.  In particular, the space should be
         * adjusted as though an ordinary solid torus (base orbifold a
         * disc, no twists or exceptional fibres) had been replaced by
         * this block.  This description does not make sense for blocks
         * with twisted boundary; the twisted case is discussed below.
         *
         * If the argument \a reflect is \c true, it should be assumed
         * that this saturated block is being reflected before being
         * inserted into the larger Seifert fibred space.  That is, any
         * twists or exceptional fibres should be negated before being
         * added.
         *
         * Regarding the signs of exceptional fibres:  Consider a
         * saturated block containing a solid torus whose meridinal curve
         * runs \a p times horizontally around the boundary in order through
         * annuli 0,1,... and follows the fibres \a q times from bottom
         * to top (as depicted in the diagram in the SatBlock class
         * notes).  Then this saturated block adds a positive (\a p, \a q)
         * fibre to the underlying Seifert fibred space.
         *
         * If the ring of saturated annuli bounding this block is twisted
         * then the situation becomes more complex.  It can be proven
         * that such a block must contain a twisted reflector boundary
         * in the base orbifold (use Z_2 homology with fibre-reversing
         * paths to show that the base orbifold must contain another
         * twisted boundary component, and then recall that real boundaries
         * are not allowed inside blocks).
         *
         * In this twisted boundary case, it should be assumed that the
         * twisted reflector boundary is already stored in the given Seifert
         * fibred space.  This routine should make any further changes
         * that are required (there may well be none).  That is, the
         * space should be adjusted as though a trivial Seifert fibred
         * space over the annulus with one twisted reflector boundary (and
         * one twisted puncture corresponding to the block boundary) had
         * been replaced by this block.  In particular, this routine should
         * \e not add the reflector boundary itself.
         *
         * @param sfs the Seifert fibred space to adjust.
         * @param reflect \c true if this block is to be reflected, or
         * \c false if it should be inserted directly.
         */
        virtual void adjustSFS(SFSpace& sfs, bool reflect) const = 0;

        /**
         * Finds the next (or previous) boundary annulus around from this,
         * treating all adjacent blocks as part of a single large saturated
         * region.
         *
         * Suppose that all saturated blocks are merged together according
         * to adjacent boundary annuli, forming larger saturated structures.
         * The remaining annuli that do not have adjacent blocks will
         * group together to form several large boundary rings.  Note that
         * each boundary ring might involve annuli from several
         * different blocks, and might or might not have a twist (thus
         * forming a large Klein bottle instead of a large torus).
         *
         * This routine is used to trace around such a boundary ring.  It is
         * assumed that annulus \a thisAnnulus of this block forms part
         * of a boundary ring (i.e., it has no adjacent block).  This
         * routine will then return the next/previous annulus around from
         * this in the large boundary ring.  Here "next" means in the direction
         * following from the second triangle of this annulus, and
         * "previous" means in the direction following from the first triangle;
         * the boolean argument \a followPrev controls which we will be used.
         * This next/previous annulus might belong to another block, or it
         * might even be this original annulus again.
         *
         * The next/previous annulus itself is not returned, but rather a
         * reference as to how it appears within its enclosing saturated block.
         * Specifically, a block and corresponding annulus number will be
         * included as the first two elements of the returned tuple.
         *
         * It is possible that the next/previous annulus as it appears within
         * the returned block is oriented differently from how it appears
         * within this large boundary ring.  For this reason, two booleans are
         * returned also.  The third element of the returned tuple will
         * describe whether the annulus is reflected vertically as it
         * appears within the large boundary ring (i.e., the first and
         * second triangles remain the same but the fibre direction is
         * reversed).  Similarly, the fourth element of the tuple will describe
         * whether the annulus is reflected horizontally as it appears
         * within the large boundary ring (i.e., first and second triangles
         * are switched but the fibre direction is unchanged).
         *
         * It is possible that both a horizontal and vertical reflection
         * take place.  Note that any kind of reflection will also
         * affect the locations of the 0/1/2 markings as described in
         * the SatAnnulus class notes.
         *
         * Finally, note that if the large boundary ring is twisted
         * (i.e., it forms a Klein bottle), then following the entire
         * boundary ring around using this routine will bring you back to
         * the starting annulus but with the vertical reflection flag set.
         *
         * \pre Annulus \a thisAnnulus of this block has no block
         * adjacent to it.
         *
         * \warning If you wish to trace around an entire boundary
         * ring, you will need to adjust the argument \a followPrev
         * according to whether or not the current annulus
         * is reflected horizontally (since, under a horizontal
         * reflection, "next" becomes "previous" and vice versa).
         *
         * @param thisAnnulus describes which original boundary annulus of
         * this block to examine; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @param followPrev \c true if we should find the previous boundary
         * annulus, or \c false if we should find the next boundary annulus.
         * @return a tuple (\a nextBlock, \a nextAnnulus, \a refVert,
         * \a refHoriz), where: \a nextBlock is the block containing the next
         * boundary annulus around from \a thisAnnulus; \a nextAnnulus is the
         * specific annulus number within \a nextBlock of the next annulus
         * around (between 0 and \a nextBlock->countAnnuli()-1 inclusive, and
         * the corresponding annulus will have no block adjacent to it);
         * \a refVert is \c true iff the next annulus around is vertically
         * reflected; and \a refHoriz is \c true iff the next annulus around
         * is horizontally reflected (see above for details on reflections).
         */
        std::tuple<const SatBlock*, size_t, bool, bool>
            nextBoundaryAnnulus(size_t thisAnnulus, bool followPrev) const;

        /**
         * Returns an abbreviated name or symbol for this block.
         * This name will reflect the particular block type, but may not
         * provide thorough details.
         *
         * The name will be no more than a handful of characters long, and
         * will not include a newline (or surrounding dollar signs in TeX
         * mode).
         *
         * @param tex \c true if the name should be formatted for TeX,
         * or \c false if it should be in plain text format.
         * @return an abbreviated name for this block.
         */
        std::string abbr(bool tex = false) const;

        /**
         * Writes an abbreviated name or symbol for this block to the
         * given output stream.  This name should reflect the particular
         * block type, but need not provide thorough details.
         *
         * The output should be no more than a handful of characters long,
         * and no newline should be written.  In TeX mode, no leading or
         * trailing dollar signs should be written.
         *
         * \nopython Instead use the variant abbr() that returns a string.
         *
         * @param out the output stream to which to write.
         * @param tex \c true if the output should be formatted for TeX,
         * or \c false if it should be in plain text format.
         */
        virtual void writeAbbr(std::ostream& out, bool tex = false) const = 0;

        /**
         * Implements a consistent ordering of saturated blocks.
         * This ordering is purely aesthetic on the part of the author,
         * and is subject to change in future versions of Regina.
         *
         * @param compare the saturated block with which this will be
         * compared.
         * @return \c true if this block comes before the given block
         * according to the ordering of saturated blocks, or \c false
         * if either the blocks are identical or this block comes after
         * the given block.
         */
        bool operator < (const SatBlock& compare) const;

        /**
         * Determines whether this and the given object represent saturated
         * blocks of the same type with the same combinatorial parameters.
         *
         * As examples of what is meant by "combinatorial parameters":
         *
         * - Any two SatCube objects will compare as equal, since there
         *   is only one combinatorial type of SatCube.
         *
         * - Two SatReflectorStrip objects will compare as equal if their
         *   rings of bounary annuli have the same length and are either
         *   both twisted or both untwisted.
         *
         * - Two SatLST objects will compare as equal if their internal
         *   layered solid tori have the same three integer parameters
         *   (identifying how the meridinal disc meets the three boundary
         *   edges), \e and their corresponding boundary edges are attached
         *   to the horizontal/vertical/diagonal edges of the boundary
         *   annulus in the same way.
         *
         * @param other the saturated block to compare with this.
         * @return \c true if and only if this and the given object
         * represent blocks of the same type with the same parameters.
         */
        virtual bool operator == (const SatBlock& other) const = 0;

        /**
         * Determines whether this and the given object do not represent
         * saturated blocks of the same type with the same combinatorial
         * parameters.
         *
         * See the equality test operator==() for examples of what is
         * meant by "the same combinatorial parameters".
         *
         * @param other the saturated block to compare with this.
         * @return \c true if and only if this and the given object
         * do not represent blocks of the same type with the same parameters.
         */
        bool operator != (const SatBlock& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * This must be implemented by subclasses.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const = 0;

        // Mark this class as non-assignable.
        // There is a copy constructor, but that is protected.
        SatBlock& operator = (const SatBlock&) = delete;

    protected:
        /**
         * Constructor for a block with the given number of annuli on
         * the boundary.
         *
         * All arrays will be constructed but their contents will remain
         * uninitialised, with the exception that the \a adjBlock array
         * will be filled with null pointers.
         *
         * @param nAnnuli the number of annuli on the boundary of this
         * block; this must be strictly positive.
         * @param twistedBoundary \c true if the ring of boundary annuli
         * is twisted to form a long Mobius band, or \c false (the default)
         * if it is not.
         */
        SatBlock(size_t nAnnuli, bool twistedBoundary = false);
        /**
         * Creates a new clone of the given block.
         *
         * Note that the new \a adjBlock_ array will contain pointers to
         * the same adjacent blocks as the original.  That is, adjacent
         * blocks will not be cloned also; instead pointers to adjacent
         * blocks will simply be copied across.
         *
         * @param cloneMe the saturated block to clone.
         */
        SatBlock(const SatBlock& cloneMe);

        /**
         * Returns a newly created clone of this saturated block structure.
         * A clone of the correct subclass of SatBlock will be returned.
         * For this reason, each subclass of SatBlock must implement this
         * routine.
         *
         * @return a new clone of this block.
         */
        virtual SatBlock* clone() const = 0;

        /**
         * Determines whether this and the given block have identical
         * boundaries.
         *
         * This requires not just that both boundaries represent the same
         * subcomplex of the underlying triangulation, but also that the
         * boundaries use identical saturated annuli, and that these
         * annuli appear in the same order.
         *
         * @param other the block to compare with this.
         * @return \c true if and only if this and the given block have
         * identical boundaries, as described above.
         */
        bool identicalBoundary(const SatBlock& other) const;

        /**
         * Determines whether the given tetrahedron is contained within the
         * given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.
         *
         * @param t the tetrahedron to search for.
         * @param list the list in which to search.
         * @return \c true if and only if the given tetrahedron was found.
         */
        static bool isBad(const Tetrahedron<3>* t, const TetList& list);
        /**
         * Determines whether the given tetrahedron is contained within
         * the given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  It is a generic routine for working with
         * arbitrary list types.
         *
         * \pre Forward iterators of type <tt>List::const_iterator</tt>
         * that span the given list can be obtained by calling
         * <tt>list.begin()</tt> and <tt>list.end()</tt>.
         *
         * @param t the tetrahedron to search for.
         * @param list the list in which to search.
         * @return \c true if and only if the given tetrahedron was found.
         */
        template <class List>
        static bool isBad(const Tetrahedron<3>* t, const List& list) {
            for (const Tetrahedron<3>* tet : list)
                if (tet == t)
                    return true;
            return false;
        }

        /**
         * Determines whether the given tetrahedron pointer is null.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  Despite its trivial implementation, it is
         * provided to make long blocks of code easier to read and
         * distinguish by functionality.
         *
         * The name notUnique() may seem strang for what is essentially
         * a nullity test; in fact this routine is offered as a
         * degenerate case of other variants of notUnique() that take
         * more tetrahedra as arguments.
         *
         * @param test the tetrahedron pointer to test.
         * @return \c true if \a test is null, or \c false otherwise.
         */
        static bool notUnique(const Tetrahedron<3>* test);
        /**
         * Determines whether the given tetrahedron pointer is null
         * or equal to another from the given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  Despite its trivial implementation, it is
         * provided to make long blocks of code easier to read and
         * distinguish by functionality.
         *
         * @param test the tetrahedron pointer to test.
         * @param other1 another tetrahedron that will be compared with \a test.
         * @return \c true if \a test is null or equal to \a other1,
         * or \c false otherwise.
         */
        static bool notUnique(const Tetrahedron<3>* test,
            const Tetrahedron<3>* other1);
        /**
         * Determines whether the given tetrahedron pointer is null
         * or equal to another from the given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  Despite its trivial implementation, it is
         * provided to make long blocks of code easier to read and
         * distinguish by functionality.
         *
         * @param test the tetrahedron pointer to test.
         * @param other1 another tetrahedron that will be compared with \a test.
         * @param other2 another tetrahedron that will be compared with \a test.
         * @return \c true if \a test is null or equal to \a other1 or
         * \a other2, or \c false otherwise.
         */
        static bool notUnique(const Tetrahedron<3>* test,
            const Tetrahedron<3>* other1, const Tetrahedron<3>* other2);
        /**
         * Determines whether the given tetrahedron pointer is null
         * or equal to another from the given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  Despite its trivial implementation, it is
         * provided to make long blocks of code easier to read and
         * distinguish by functionality.
         *
         * @param test the tetrahedron pointer to test.
         * @param other1 another tetrahedron that will be compared with \a test.
         * @param other2 another tetrahedron that will be compared with \a test.
         * @param other3 another tetrahedron that will be compared with \a test.
         * @return \c true if \a test is null or equal to \a other1,
         * \a other2 or \a other3, or \c false otherwise.
         */
        static bool notUnique(const Tetrahedron<3>* test,
            const Tetrahedron<3>* other1, const Tetrahedron<3>* other2,
            const Tetrahedron<3>* other3);
        /**
         * Determines whether the given tetrahedron pointer is null
         * or equal to another from the given list.
         *
         * This is intended as a helper routine for isBlock() and
         * related routines.  Despite its trivial implementation, it is
         * provided to make long blocks of code easier to read and
         * distinguish by functionality.
         *
         * @param test the tetrahedron pointer to test.
         * @param other1 another tetrahedron that will be compared with \a test.
         * @param other2 another tetrahedron that will be compared with \a test.
         * @param other3 another tetrahedron that will be compared with \a test.
         * @param other4 another tetrahedron that will be compared with \a test.
         * @return \c true if \a test is null or equal to \a other1,
         * \a other2, \a other3 or \a other4, or \c false otherwise.
         */
        static bool notUnique(const Tetrahedron<3>* test,
            const Tetrahedron<3>* other1, const Tetrahedron<3>* other2,
            const Tetrahedron<3>* other3, const Tetrahedron<3>* other4);

        /**
         * Returns a new model that combines this block structure with
         * the given triangulation.  The new model will take ownership of
         * both this block and the given triangulation.
         *
         * The purpose of this routine is, essentially, to give subclasses
         * access to the private SatBlockModel constructor (which SatBlock
         * can access as a friend class, but which subclasses cannot).
         *
         * @param triangulation an explicit triangulation of this block
         * structure.
         * @return a model that holds this block and the given triangulation.
         */
        SatBlockModel modelWith(Triangulation<3>* triangulation);

        /**
         * Adjusts the structure of this block according to the given
         * isomorphism between triangulations.  Any triangulation-specific
         * information will be transformed accordingly (for instance, the
         * routine SatAnnulus::transform() will be called for each
         * boundary annulus).
         *
         * Information regarding adjacent blocks will \e not be changed.
         * Only structural information for this particular block will be
         * updated.
         *
         * The given isomorphism must describe a mapping from \a originalTri
         * to \a newTri, and this block must currently refer to tetrahedra in
         * \a originalTri.  After this routine is called the block will
         * instead refer to the corresponding tetrahedra in \a newTri (with
         * changes in vertex/face numbering also accounted for).
         *
         * \pre This block currently refers to tetrahedra in \a originalTri,
         * and \a iso describes a mapping from \a originalTri to \a newTri.
         *
         * \warning Any subclasses of SatBlock that store additional
         * triangulation-specific information will need to override this
         * routine.  When doing so, be sure to call SatBlock::transform()
         * so that the generic changes defined here will still take place.
         *
         * @param originalTri the triangulation currently used by this
         * saturated block.
         * @param iso the mapping from \a originalTri to \a newTri.
         * @param newTri the triangulation to be used by the updated
         * block structure.
         */
        virtual void transform(const Triangulation<3>& originalTri,
                const Isomorphism<3>& iso, const Triangulation<3>& newTri);

    private:
        /**
         * Lists the given saturated block as being adjacent to the
         * given boundary annulus of this block.  Both block structures
         * (this and the given block) will be updated.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * has the new adjacency; this must be between 0 and countAnnuli()-1
         * inclusive.
         * @param adjBlock the other saturated block that is adjacent to
         * this.
         * @param adjAnnulus indicates which boundary annulus of the
         * adjacent block meets the given boundary annulus of this block;
         * this must be between 0 and adjBlock->countAnnuli()-1 inclusive.
         * @param adjReflected indicates whether the new adjacency is
         * reflected (see the class notes for details).
         * @param adjBackwards indicates whether the new adjacency is
         * backwards (see the class notes for details).
         */
        void setAdjacent(size_t whichAnnulus, SatBlock* adjBlock,
                size_t adjAnnulus, bool adjReflected, bool adjBackwards);

    // The following classes are the only classes that are allowed to
    // manage a raw SatBlock pointer:
    friend class SatBlockModel;
    friend class SatRegion;
};

/**
 * Contains an explicit triangulation of a saturated block along with the
 * accompanying saturated block description.
 *
 * This class is designed to work with SatRegion::findStarterBlock(),
 * which uses such models as potential starting points for its search.
 * The ultimate aim here is to identify regions within triangulations that are
 * formed by joining saturated blocks together along their boundary annuli.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * you cannot create generate your own models manually (aside from copying or
 * moving); instead you will need to use block-specific factory routines
 * such as SatTriPrism::model(), SatCube::model(), and so on.
 *
 * \ingroup subcomplex
 */
class SatBlockModel : public ShortOutput<SatBlockModel> {
    private:
        Triangulation<3>* triangulation_;
            /**< The triangulation of the saturated block. */
        SatBlock* block_;
            /**< Structural details of the saturated block. */

    public:
        /**
         * Creates a new copy of the given model.
         * This will induce a deep copy of both the triangulation and the
         * block structure.
         *
         * @param src the model to copy.
         */
        SatBlockModel(const SatBlockModel& src);

        /**
         * Moves the contents of the given model into this new model.
         * This is a fast (constant time) operation.
         *
         * The model that was passed (\a src) will no longer be usable.
         *
         * @param src the model whose contents should be moved.
         */
        SatBlockModel(SatBlockModel&& src) noexcept;

        /**
         * Destroys both the internal triangulation and block structure.
         */
        ~SatBlockModel();

        /**
         * Sets this to be a copy of the given model.
         * This will induce a deep copy of both the triangulation and the
         * block structure.
         *
         * @param src the model to copy.
         * @return a reference to this model.
         */
        SatBlockModel& operator = (const SatBlockModel& src);

        /**
         * Moves the contents of the given model into this model.
         * This is a fast (constant time) operation.
         *
         * The model that was passed (\a src) will no longer be usable.
         *
         * @param src the model whose contents should be moved.
         * @return a reference to this model.
         */
        SatBlockModel& operator = (SatBlockModel&& src) noexcept;

        /**
         * Swaps the contents of this and the given model.
         *
         * @param other the model whose contents should be swapped with this.
         */
        void swap(SatBlockModel& other) noexcept;

        /**
         * Returns the triangulation of the saturated block.
         *
         * @return the block triangulation.
         */
        const Triangulation<3>& triangulation() const;

        /**
         * Returns the structure of the saturated block.
         *
         * @return the block structure.
         */
        const SatBlock& block() const;

        /**
         * Determines whether this and the given object model saturated
         * blocks of the same type with the same combinatorial parameters.
         *
         * This is equivalent to testing whether the blocks returned by
         * block() compare as equal.  See SatBlock::operator==() for
         * further details on what this comparison means.
         *
         * Assuming you created your models using the block-specific factory
         * routines (SatTriPrism::model(), SatCube::model(), etc.), if
         * two models compare as equal then their triangulations should
         * be combinatorially identical.  At the time of writing, the
         * converse is also true: all models created from non-equal blocks
         * yield non-identical (and moreover non-isomorphic) triangulations.
         *
         * @param other the model to compare with this.
         * @return \c true if and only if this and the given object
         * model the same block type with the same combinatorial parameters.
         */
        bool operator == (const SatBlockModel& other) const;

        /**
         * Determines whether this and the given object do not model saturated
         * blocks of the same type with the same combinatorial parameters.
         *
         * This is equivalent to testing whether the blocks returned by
         * block() compare as non-equal.  See SatBlock::operator==() for
         * further details on what this comparison means.
         *
         * Assuming you created your models using the block-specific factory
         * routines (SatTriPrism::model(), SatCube::model(), etc.), if
         * two models compare as equal then their triangulations should
         * be combinatorially identical.  At the time of writing, the
         * converse is also true: all models created from non-equal blocks
         * yield non-identical (and moreover non-isomorphic) triangulations.
         *
         * @param other the model to compare with this.
         * @return \c true if and only if this and the given object do not
         * model the same block type with the same combinatorial parameters.
         */
        bool operator != (const SatBlockModel& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new model filled with the given data.  The model will
         * take ownership of both the given triangulation and the given block.
         */
        SatBlockModel(Triangulation<3>* triangulation, SatBlock* block);

    friend class SatBlock;
};

/**
 * Swaps the contents of the two given models.
 *
 * This global routine simply calls SatBlockModel::swap(); it is provided
 * so that SatBlockModel meets the C++ Swappable requirements.
 *
 * @param a the first model whose contents should be swapped.
 * @param b the second model whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SatBlockModel& a, SatBlockModel& b) noexcept;

// Inline functions for SatBlock

inline SatBlock::SatBlock(size_t nAnnuli, bool twistedBoundary) :
        nAnnuli_(nAnnuli),
        annulus_(new SatAnnulus[nAnnuli]),
        twistedBoundary_(twistedBoundary),
        adjBlock_(new SatBlock*[nAnnuli]),
        adjAnnulus_(new size_t[nAnnuli]),
        adjReflected_(new bool[nAnnuli]),
        adjBackwards_(new bool[nAnnuli]) {
    for (size_t i = 0; i < nAnnuli; i++)
        adjBlock_[i] = nullptr;
}

inline SatBlock::~SatBlock() {
    delete[] annulus_;
    delete[] adjBlock_;
    delete[] adjAnnulus_;
    delete[] adjReflected_;
    delete[] adjBackwards_;
}

inline size_t SatBlock::countAnnuli() const {
    return nAnnuli_;
}

inline const SatAnnulus& SatBlock::annulus(size_t which) const {
    return annulus_[which];
}

inline bool SatBlock::twistedBoundary() const {
    return twistedBoundary_;
}

inline bool SatBlock::hasAdjacentBlock(size_t whichAnnulus) const {
    return (adjBlock_[whichAnnulus] != nullptr);
}

inline const SatBlock* SatBlock::adjacentBlock(size_t whichAnnulus) const {
    return adjBlock_[whichAnnulus];
}

inline size_t SatBlock::adjacentAnnulus(size_t whichAnnulus) const {
    return adjAnnulus_[whichAnnulus];
}

inline bool SatBlock::adjacentReflected(size_t whichAnnulus) const {
    return adjReflected_[whichAnnulus];
}

inline bool SatBlock::adjacentBackwards(size_t whichAnnulus) const {
    return adjBackwards_[whichAnnulus];
}

inline void SatBlock::setAdjacent(size_t whichAnnulus, SatBlock* adjBlock,
        size_t adjAnnulus, bool adjReflected, bool adjBackwards) {
    adjBlock_[whichAnnulus] = adjBlock;
    adjAnnulus_[whichAnnulus] = adjAnnulus;
    adjReflected_[whichAnnulus] = adjReflected;
    adjBackwards_[whichAnnulus] = adjBackwards;

    adjBlock->adjBlock_[adjAnnulus] = this;
    adjBlock->adjAnnulus_[adjAnnulus] = whichAnnulus;
    adjBlock->adjReflected_[adjAnnulus] = adjReflected;
    adjBlock->adjBackwards_[adjAnnulus] = adjBackwards;
}

inline bool SatBlock::operator != (const SatBlock& other) const {
    return ! ((*this) == other);
}

inline bool SatBlock::notUnique(const Tetrahedron<3>* test) {
    return (test == nullptr);
}

inline bool SatBlock::notUnique(const Tetrahedron<3>* test,
        const Tetrahedron<3>* other1) {
    return (test == nullptr || test == other1);
}

inline bool SatBlock::notUnique(const Tetrahedron<3>* test,
        const Tetrahedron<3>* other1, const Tetrahedron<3>* other2) {
    return (test == nullptr || test == other1 || test == other2);
}

inline bool SatBlock::notUnique(const Tetrahedron<3>* test,
        const Tetrahedron<3>* other1, const Tetrahedron<3>* other2,
        const Tetrahedron<3>* other3) {
    return (test == nullptr || test == other1 || test == other2 ||
        test == other3);
}

inline bool SatBlock::notUnique(const Tetrahedron<3>* test,
        const Tetrahedron<3>* other1, const Tetrahedron<3>* other2,
        const Tetrahedron<3>* other3, const Tetrahedron<3>* other4) {
    return (test == nullptr || test == other1 || test == other2 ||
        test == other3 || test == other4);
}

inline SatBlockModel SatBlock::modelWith(Triangulation<3>* triangulation) {
    return SatBlockModel(triangulation, this);
}

inline bool SatBlock::identicalBoundary(const SatBlock& other) const {
    return std::equal(annulus_, annulus_ + nAnnuli_,
        other.annulus_, other.annulus_ + other.nAnnuli_) &&
        (twistedBoundary_ == other.twistedBoundary_);

}

// Inline functions for SatBlockModel

inline SatBlockModel::SatBlockModel(Triangulation<3>* triangulation,
        SatBlock* block) : triangulation_(triangulation), block_(block) {
}

inline SatBlockModel::SatBlockModel(const SatBlockModel& src) :
        triangulation_(new Triangulation<3>(*src.triangulation_)),
        block_(src.block_->clone()) {
}

inline SatBlockModel::SatBlockModel(SatBlockModel&& src) noexcept :
        triangulation_(src.triangulation_), block_(src.block_) {
    src.triangulation_ = nullptr;
    src.block_ = nullptr;
}

inline SatBlockModel::~SatBlockModel() {
    delete triangulation_;
    delete block_;
}

inline SatBlockModel& SatBlockModel::operator = (const SatBlockModel& src) {
    if (std::addressof(src) != this) {
        delete triangulation_;
        delete block_;
        triangulation_ = new Triangulation<3>(*src.triangulation_);
        block_ = src.block_->clone();
    }
    return *this;
}

inline SatBlockModel& SatBlockModel::operator = (SatBlockModel&& src) noexcept {
    std::swap(triangulation_, src.triangulation_);
    std::swap(block_, src.block_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

inline void SatBlockModel::swap(SatBlockModel& other) noexcept {
    std::swap(triangulation_, other.triangulation_);
    std::swap(block_, other.block_);
}

inline const Triangulation<3>& SatBlockModel::triangulation() const {
    return *triangulation_;
}

inline const SatBlock& SatBlockModel::block() const {
    return *block_;
}

inline bool SatBlockModel::operator == (const SatBlockModel& other) const {
    return (*block_) == (*other.block_);
}

inline bool SatBlockModel::operator != (const SatBlockModel& other) const {
    return (*block_) != (*other.block_);
}

inline void SatBlockModel::writeTextShort(std::ostream& out) const {
    out << "Model of ";
    block_->writeTextShort(out);
}

inline void swap(SatBlockModel& a, SatBlockModel& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

