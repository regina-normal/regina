
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

/*! \file subcomplex/satblock.h
 *  \brief Deals with saturated blocks in triangulations of Seifert fibred
 *  spaces.
 */

#ifndef __SATBLOCK_H
#ifndef __DOXYGEN
#define __SATBLOCK_H
#endif

#include "regina-core.h"
#include "output.h"
#include "subcomplex/satannulus.h"
#include <set>
#include <boost/noncopyable.hpp>

namespace regina {

struct SatAnnulus;
class SFSpace;

/**
 * \weakgroup subcomplex
 * @{
 */

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
 * \warning In addition to mandatory overrides such as clone() and
 * adjustSFS(), some subclasses will need to override the virtual
 * routine transform() in order to correctly adjust additional
 * triangulation-specific information stored in the subclass.  See the
 * transform() documentation for further details.
 */
class REGINA_API SatBlock :
        public Output<SatBlock>,
        public boost::noncopyable {
    public:
        typedef std::set<Tetrahedron<3>*> TetList;
            /**< The data structure used to store a list of tetrahedra
                 that should not be examined by isBlock(). */

    protected:
        unsigned nAnnuli_;
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
        unsigned* adjAnnulus_;
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
         * Destroys all internal arrays.  Note that any adjacent blocks
         * that are referenced by the \a adjBlock array will \e not be
         * destroyed.
         */
        virtual ~SatBlock();

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
         * Returns the number of annuli on the boundary of this
         * saturated block.
         *
         * @return the number of boundary annuli.
         */
        unsigned nAnnuli() const;

        /**
         * Returns details of the requested annulus on the boundary of
         * this saturated block.  Annuli are numbered from 0 to
         * nAnnuli()-1 as described in the class notes.
         *
         * @param which indicates which boundary annulus is requested;
         * this must be between 0 and nAnnuli()-1 inclusive.
         * @return a reference to the requested boundary annulus.
         */
        const SatAnnulus& annulus(unsigned which) const;

        /**
         * Is the ring of boundary annuli twisted to form a long Mobius
         * strip?
         *
         * Recall from the class notes that the twist occurs between
         * boundary annuli nAnnuli()-1 and 0.
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
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return \c true if the given boundary annulus has an adjacent
         * block listed, or \c false otherwise.
         */
        bool hasAdjacentBlock(unsigned whichAnnulus) const;

        /**
         * Returns the saturated block listed as being adjacent to the
         * given boundary annulus of this block.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return the other block adjacent along this annulus, or 0
         * if there is no adjacent block listed.
         */
        SatBlock* adjacentBlock(unsigned whichAnnulus) const;

        /**
         * Returns which specific annulus of the adjacent block is
         * listed as being adjacent to the given boundary annulus of
         * this block.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return the corresponding annulus number on the other block
         * that is adjacent along this annulus.
         */
        unsigned adjacentAnnulus(unsigned whichAnnulus) const;

        /**
         * Returns whether the adjacency along the given boundary annulus
         * of this block is reflected.  See the class notes for a
         * discussion of reflected adjacencies.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return \c true if the corresponding adjacency is reflected,
         * or \c false if it is not.
         */
        bool adjacentReflected(unsigned whichAnnulus) const;

        /**
         * Returns whether the adjacency along the given boundary annulus
         * of this block is backwards.  See the class notes for a
         * discussion of backwards adjacencies.
         *
         * \pre The given annulus of this block actually has an adjacent
         * block listed.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * should be examined; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @return \c true if the corresponding adjacency is backwards,
         * or \c false if it is not.
         */
        bool adjacentBackwards(unsigned whichAnnulus) const;

        /**
         * Lists the given saturated block as being adjacent to the
         * given boundary annulus of this block.  Both block structures
         * (this and the given block) will be updated.
         *
         * @param whichAnnulus indicates which boundary annulus of this block
         * has the new adjacency; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @param adjBlock the other saturated block that is adjacent to
         * this.
         * @param adjAnnulus indicates which boundary annulus of the
         * adjacent block meets the given boundary annulus of this block;
         * this must be between 0 and adjBlock->nAnnuli()-1 inclusive.
         * @param adjReflected indicates whether the new adjacency is
         * reflected (see the class notes for details).
         * @param adjBackwards indicates whether the new adjacency is
         * backwards (see the class notes for details).
         */
        void setAdjacent(unsigned whichAnnulus, SatBlock* adjBlock,
                unsigned adjAnnulus, bool adjReflected, bool adjBackwards);

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
        virtual void transform(const Triangulation<3>* originalTri,
                const Isomorphism<3>* iso, Triangulation<3>* newTri);

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
         * returned in the arguments \a nextBlock and \a nextAnnulus
         * respectively.
         *
         * It is possible that the next/previous annulus as it appears within
         * the returned block is oriented differently from how it appears
         * within this large boundary ring.  For this reason, two
         * booleans are returned also.  The argument \a refVert will
         * describe whether the annulus is reflected vertically as it
         * appears within the large boundary ring (i.e., the first and
         * second triangles remain the same but the fibre direction is
         * reversed).  Similarly, the argument \a refHoriz will describe
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
         * the starting annulus but with the \a refVert flag set.
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
         * \ifacespython This routine only takes two arguments (\a thisAnnulus
         * and \a followPrev). The return value is a tuple of four
         * values: the block returned in \a nextBlock, the integer
         * returned in \a nextAnnulus, the boolean returned in \a refVert,
         * and the boolean returned in \a refHoriz.
         *
         * @param thisAnnulus describes which original boundary annulus of
         * this block to examine; this must be between 0 and nAnnuli()-1
         * inclusive.
         * @param nextBlock a reference used to return the block
         * containing the next boundary annulus around from \a thisAnnulus.
         * @param nextAnnulus a reference used to return the specific
         * annulus number within \a nextBlock of the next annulus
         * around; this will be between 0 and \a nextBlock->nAnnuli()-1
         * inclusive, and the corresponding annulus will have no block
         * adjacent to it.
         * @param refVert a reference used to return \c true if the next
         * annulus around is vertically reflected, or \c false if not;
         * see above for details.
         * @param refHoriz a reference used to return \c true if the next
         * annulus around is horizontally reflected, or \c false if not;
         * see above for details.
         * @param followPrev \c true if we should find the previous boundary
         * annulus, or \c false if we should find the next boundary annulus.
         */
        void nextBoundaryAnnulus(unsigned thisAnnulus, SatBlock*& nextBlock,
                unsigned& nextAnnulus, bool& refVert, bool& refHoriz,
                bool followPrev);

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
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
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
         * Determines whether the given annulus is in fact a boundary
         * annulus for a recognised type of saturated block.  The
         * annulus should be represented from the inside of the proposed
         * saturated block.
         *
         * Only certain types of saturated block are recognised by this
         * routine.  More exotic saturated blocks will not be identified,
         * and this routine will return \c null in such cases.
         *
         * The given list of tetrahedra will not be examined by this
         * routine.  That is, only saturated blocks that do not contain
         * any of these tetrahedra will be considered.  As a consequence,
         * if the given annulus uses any of these tetrahedra then \c null
         * will be returned.
         *
         * If a block is found on the other hand, all of the tetrahedra
         * within this block will be added to the given list.
         *
         * In the event that a block is found, it is guaranteed that the
         * given annulus will be listed as annulus number 0 in the block
         * structure, without any horizontal or vertical reflection.
         *
         * \ifacespython The second argument \a avoidTets is not
         * present.  An empty list will be passed instead.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated block.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated block if one was found, or
         * \c null if none was found.
         */
        static SatBlock* isBlock(const SatAnnulus& annulus,
            TetList& avoidTets);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * This must be implemented by subclasses.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
         virtual void writeTextShort(std::ostream& out) const = 0;

         /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * This may be reimplemented by subclasses, but the parent
         * SatBlock class offers a reasonable default implementation.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
         virtual void writeTextLong(std::ostream& out) const;

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
        SatBlock(unsigned nAnnuli, bool twistedBoundary = false);

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
        static bool isBad(Tetrahedron<3>* t, const TetList& list);
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
        static bool isBad(Tetrahedron<3>* t, const List& list) {
            for (typename List::const_iterator it = list.begin();
                    it != list.end(); ++it)
                if (*it == t)
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
        static bool notUnique(Tetrahedron<3>* test);
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
        static bool notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1);
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
        static bool notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
            Tetrahedron<3>* other2);
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
        static bool notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
            Tetrahedron<3>* other2, Tetrahedron<3>* other3);
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
        static bool notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
            Tetrahedron<3>* other2, Tetrahedron<3>* other3, Tetrahedron<3>* other4);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatBlock has now been renamed to
 * SatBlock.
 */
REGINA_DEPRECATED typedef SatBlock NSatBlock;

/*@}*/

// Inline functions for SatBlock

inline SatBlock::SatBlock(unsigned nAnnuli, bool twistedBoundary) :
        nAnnuli_(nAnnuli),
        annulus_(new SatAnnulus[nAnnuli]),
        twistedBoundary_(twistedBoundary),
        adjBlock_(new SatBlock*[nAnnuli]),
        adjAnnulus_(new unsigned[nAnnuli]),
        adjReflected_(new bool[nAnnuli]),
        adjBackwards_(new bool[nAnnuli]) {
    for (unsigned i = 0; i < nAnnuli; i++)
        adjBlock_[i] = 0;
}

inline SatBlock::~SatBlock() {
    delete[] annulus_;
    delete[] adjBlock_;
    delete[] adjAnnulus_;
    delete[] adjReflected_;
    delete[] adjBackwards_;
}

inline void SatBlock::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << '\n';
}

inline unsigned SatBlock::nAnnuli() const {
    return nAnnuli_;
}

inline const SatAnnulus& SatBlock::annulus(unsigned which) const {
    return annulus_[which];
}

inline bool SatBlock::twistedBoundary() const {
    return twistedBoundary_;
}

inline bool SatBlock::hasAdjacentBlock(unsigned whichAnnulus) const {
    return (adjBlock_[whichAnnulus] != 0);
}

inline SatBlock* SatBlock::adjacentBlock(unsigned whichAnnulus) const {
    return adjBlock_[whichAnnulus];
}

inline unsigned SatBlock::adjacentAnnulus(unsigned whichAnnulus) const {
    return adjAnnulus_[whichAnnulus];
}

inline bool SatBlock::adjacentReflected(unsigned whichAnnulus) const {
    return adjReflected_[whichAnnulus];
}

inline bool SatBlock::adjacentBackwards(unsigned whichAnnulus) const {
    return adjBackwards_[whichAnnulus];
}

inline void SatBlock::setAdjacent(unsigned whichAnnulus, SatBlock* adjBlock,
        unsigned adjAnnulus, bool adjReflected, bool adjBackwards) {
    adjBlock_[whichAnnulus] = adjBlock;
    adjAnnulus_[whichAnnulus] = adjAnnulus;
    adjReflected_[whichAnnulus] = adjReflected;
    adjBackwards_[whichAnnulus] = adjBackwards;

    adjBlock->adjBlock_[adjAnnulus] = this;
    adjBlock->adjAnnulus_[adjAnnulus] = whichAnnulus;
    adjBlock->adjReflected_[adjAnnulus] = adjReflected;
    adjBlock->adjBackwards_[adjAnnulus] = adjBackwards;
}

inline bool SatBlock::notUnique(Tetrahedron<3>* test) {
    return (test == 0);
}

inline bool SatBlock::notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1) {
    return (test == 0 || test == other1);
}

inline bool SatBlock::notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
        Tetrahedron<3>* other2) {
    return (test == 0 || test == other1 || test == other2);
}

inline bool SatBlock::notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
        Tetrahedron<3>* other2, Tetrahedron<3>* other3) {
    return (test == 0 || test == other1 || test == other2 || test == other3);
}

inline bool SatBlock::notUnique(Tetrahedron<3>* test, Tetrahedron<3>* other1,
        Tetrahedron<3>* other2, Tetrahedron<3>* other3, Tetrahedron<3>* other4) {
    return (test == 0 || test == other1 || test == other2 || test == other3 ||
        test == other4);
}

} // namespace regina

#endif

