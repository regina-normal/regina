
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

/*! \file subcomplex/satregion.h
 *  \brief Supports connected regions of saturated blocks in triangulations
 *  of Seifert fibred spaces.
 */

#ifndef __SATREGION_H
#ifndef __DOXYGEN
#define __SATREGION_H
#endif

#include "regina-core.h"
#include "output.h"
#include "subcomplex/satblock.h"
#include <vector>
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Describes how a single saturated block forms a part of a larger
 * saturated region.
 *
 * A saturated region consists of several saturated blocks joined
 * together along their boundary annuli.  This is a helper structure
 * containing a single saturated block along with details of its
 * orientation within a larger region.
 *
 * The ring of saturated annuli around the boundary of the block gives a
 * natural orientation to the block within the context of the base orbifold,
 * just as the ring of edges around a polygon would give a natural
 * orientation to that polygon within the context of a surrounding surface.
 * Again drawing an analogy with the orientation of polygons within a surface,
 * each block can be considered to have a correct or reflected orientation
 * according to whether this ring of annuli runs clockwise or anticlockwise
 * in the base orbifold.
 *
 * The precise orientation of a block is described using two booleans.
 * A block may be reflected \e horizontally, which preserves the
 * directions of Seifert fibres but which reverses the
 * clockwise/anticlockwise orientation as discussed above.  A block may
 * also be reflected \e vertically, which preserves the
 * clockwise/anticlockwise orientation but which reverses the directions
 * of the Seifert fibres.  A block may of course be reflected both
 * horizontally and vertically, or it may not be reflected at all.
 *
 * This helper structure is small, and can be copied by value if
 * necessary.  Be aware that when this structure is destroyed, the
 * internal block structure of type SatBlock is \e not destroyed.
 *
 * \ifacespython The data members \a block, \a refVert and \a refHoriz
 * are read-only.
 */
struct REGINA_API SatBlockSpec {
    SatBlock* block;
        /**< Details of the saturated block structure. */
    bool refVert;
        /**< Indicates whether the block is reflected vertically within
             the larger region.  See the class notes for details. */
    bool refHoriz;
        /**< Indicates whether the block is reflected horizontally within
             the larger region.  See the class notes for details. */

    /**
     * Creates a new structure that is completely uninitialised.
     */
    SatBlockSpec();
    /**
     * Creates a new structure that is initialised to the given set of
     * values.
     *
     * @param useBlock details of the saturated block structure.
     * @param useRefVert \c true if the block is reflected vertically
     * within the larger region, or \c false otherwise.
     * @param useRefHoriz \c true if the block is reflected horizontally
     * within the larger region, or \c false otherwise.
     */
    SatBlockSpec(SatBlock* useBlock, bool useRefVert, bool useRefHoriz);
    /**
     * Determines whether this and the given structure contain identical
     * information.  In particular, their \a block pointers must be
     * identical (not pointing to different blocks with the same contents).
     *
     * @param other the structure to compare against this.
     * @return \c true if and only if this and \a other contain
     * identical information.
     */
    bool operator == (const SatBlockSpec& other) const;
    /**
     * Determines whether this and the given structure contain different
     * information.  In particular, they are considered different if their
     * \a block pointers are different (even if they point to different blocks
     * with the same contents).
     *
     * @param other the structure to compare against this.
     * @return \c true if and only if this and \a other contain
     * different information.
     */
    bool operator != (const SatBlockSpec& other) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatBlockSpec has now been renamed to
 * SatBlockSpec.
 */
REGINA_DEPRECATED typedef SatBlockSpec NSatBlockSpec;

/**
 * A large saturated region in a Seifert fibred space formed by joining
 * together saturated blocks.
 *
 * Like a saturated block (described in the class SatBlock), a
 * saturated region is a connected set of tetrahedra built from a subset
 * of fibres.  Unlike a saturated block however, a saturated region has
 * no constraints on its boundary - it may have several boundary
 * components or it may have none.  For instance, a saturated region
 * might be an entire closed Seifert fibred space, or it might describe
 * a Seifert fibred component of a JSJ decomposition.
 *
 * A saturated region is formed from a collection of saturated blocks by
 * joining the boundary annuli of these blocks together in pairs.  The
 * joins must be made so that the fibres are consistent, though it is
 * allowable to reverse the directions of the fibres.  There is no problem
 * with joining two boundary annuli from the same block to each other.
 *
 * Any boundary annulus of a block that is not joined to some other
 * boundary annulus of a block becomes a boundary annulus of the entire
 * region.  In this way, each boundary component of the region (if there
 * are any at all) is formed from a ring of boundary annuli, in the same
 * way that the boundary of a block is.  Note that the routine
 * SatBlock::nextBoundaryAnnulus() can be used to trace around a region
 * boundary.  Like block boundaries, the boundary of a saturated region
 * need not be part of the boundary of the larger triangulation (i.e.,
 * there may be adjacent tetrahedra that are not recognised as part of
 * this saturated structure).
 *
 * The SatRegion class stores a list of its constituent blocks, but it
 * does not directly store which block boundary annuli are joined to
 * which.  This adjacency information is stored within the blocks
 * themselves; see the notes regarding adjacency in the SatBlock class
 * description.
 *
 * Blocks cannot be added to a region by hand.  The way a region is
 * constructed is by locating some initial block within a triangulation
 * and passing this to the SatRegion constructor, and then by calling
 * expand() to locate adjacent blocks and expand the region as far as
 * possible.  For locating initial blocks, the class
 * SatBlockStarterSearcher may be of use.
 *
 * \warning It is crucial that the adjacency information stored in the
 * blocks is consistent with the region containing them.  All this
 * requires is that the blocks are not manipulated externally (e.g.,
 * SatBlock::setAdjacent() is not called on any of the blocks), but
 * instead all adjacency information is managed by this class.  Routines
 * such as expand() which may add more blocks to the region will update
 * the block adjacencies accordingly.
 *
 * \todo \feature Have this class track the boundary components properly,
 * with annuli grouped and oriented according to the region boundaries (as
 * opposed to individual block boundaries).
 */
class REGINA_API SatRegion :
        public Output<SatRegion>,
        public boost::noncopyable {
    private:
        typedef std::vector<SatBlockSpec> BlockSet;
            /**< The data structure used to store the list of
                 constituent blocks. */

        BlockSet blocks_;
            /**< The set of blocks from which this region is formed,
                 along with details of how they are oriented within this
                 larger region. */
        long baseEuler_;
            /**< The Euler characteristic of the base orbifold if we
                 assume that each block contributes a trivial disc to
                 the base orbifold. */
        bool baseOrbl_;
            /**< Denotes whether the base orbifold is orientable if we
                 assume that each block contributes a trivial disc to
                 the base orbifold. */
        bool hasTwist_;
            /**< Denotes whether we can find a fibre-reversing path that does
                 not step inside the interior of any constituent blocks. */
        bool twistsMatchOrientation_;
            /**< Denotes whether set of fibre-reversing paths corresponds
                 precisely to the set of orientation-reversing paths on the
                 base orbifold, where we do not allow paths that step inside
                 the interior of any constituent blocks. */
        long shiftedAnnuli_;
            /**< The number of additional (1,1) twists added to the underlying
                 Seifert fibred space due to blocks being sheared up or down
                 as they are joined together.  This typically happens when
                 the triangulations of two boundary annuli are not compatible
                 when joined (e.g., they provide opposite diagonal edges) */
        unsigned long twistedBlocks_;
            /**< The number of constituent blocks with twisted boundary.
                 Each such block provides an additional twisted reflector
                 boundary to the base orbifold; see SatBlock::adjustSFS()
                 for details. */

        unsigned long nBdryAnnuli_;
            /**< The number of saturated annuli forming the boundary
                 components (if any) of this region. */

    public:
        /**
         * Constructs a new region containing just the given block.
         * All boundary annuli of the given block will become boundary
         * annuli of this region.  It is guaranteed that this block will
         * be stored in the region without any kind of reflection (see
         * SatBlockSpec for details).
         *
         * Typically a region is initialised using this constructor, and
         * then grown using the expand() routine.  For help in finding
         * an initial starter block, see the SatBlockStarterSearcher class.
         *
         * This region will claim ownership of the given block, and upon
         * destruction it will destroy this block also.
         *
         * \pre The given block has no adjacencies listed.  That is,
         * for every boundary annulus of the given block,
         * SatBlock::hasAdjacentBlock() returns \c false.
         *
         * @param starter the single block that this region will describe.
         */
        SatRegion(SatBlock* starter);

        /**
         * Destroys this structure and all of its internal data,
         * including the individual blocks that make up this region.
         */
        ~SatRegion();

        /**
         * Returns the number of saturated blocks that come together
         * to form this saturated region.
         */
        unsigned long numberOfBlocks() const;
        /**
         * Returns details of the requested saturated block within this
         * region.  The information will returned will include
         * structural information for the block, along with details of
         * how the block is aligned (e.g., reflected vertically or
         * horizontally) within the larger region.
         *
         * @param which indicates which of the constituent blocks should
         * be returned; this must be between 0 and numberOfBlocks()-1
         * inclusive.
         * @return details of the requested saturated block.
         */
        const SatBlockSpec& block(unsigned long which) const;
        /**
         * Returns the index of the given block within this region.
         * This index corresponds to the integer parameter that is passed
         * to the routine block().
         *
         * \warning This routine is slow, since it simply scans through
         * the blocks in this region one by one until the given block is
         * found (or until all blocks are exhausted).
         *
         * @return the index of the given block (as an integer between
         * 0 and numberOfBlocks()-1 inclusive), or -1 if the block is
         * not part of this region.
         */
        long blockIndex(const SatBlock* block) const;

        /**
         * Returns the number of saturated annuli that together form the
         * boundary components of this region.
         *
         * @return the number of boundary annuli.
         */
        unsigned long numberOfBoundaryAnnuli() const;
        /**
         * Returns the requested saturated annulus on the boundary of
         * this region.
         *
         * The saturated annuli that together form the boundary
         * components of this region are numbered from 0 to
         * numberOfBoundaryAnnuli()-1 inclusive.  The argument \a which
         * specifies which one of these annuli should be returned.
         *
         * Currently the annuli are numbered lexicographically by
         * block and then by annulus number within the block, although
         * this ordering is subject to change in future versions of Regina.
         * In particular, the annuli are \e not necessarily numbered in
         * order around the region boundaries, and each region boundary
         * component might not even be given a consecutive range of numbers.
         *
         * It is guaranteed however that, if the starter block passed to
         * the SatRegion constructor provides any boundary annuli for
         * the overall region, then the first such annulus in the starter
         * block will be numbered 0 here.
         *
         * The structure returned will be the annulus precisely as it
         * appears within its particular saturated block.  As discussed
         * in the SatBlockSpec class notes, the block might be
         * reflected horizontally and/or vertically within the overall
         * region, which will affect how the annulus is positioned as
         * part of the overall region boundary (e.g., the annulus might
         * be positioned upside-down in the overall region boundary,
         * or it might be positioned with its second triangle appearing before
         * its first triangle as one walks around the boundary).  To account
         * for this, the two boolean arguments \a blockRefVert and
         * \a blockRefHoriz will be modified to indicate if and how the
         * block is reflected.
         *
         * \warning This routine is quite slow, since it currently scans
         * through every annulus of every saturated block.  Use it
         * sparingly!
         *
         * \ifacespython Both variants of boundaryAnnulus() are
         * combined into a single routine, which takes the integer
         * \a which as its only argument and returns its results as a
         * tuple.  See the alternate version of boundaryAnnulus() for
         * details on how the return tuple is structured.
         *
         * @param which specifies which boundary annulus of this region
         * to return; this must be between 0 and numberOfBoundaryAnnuli()-1
         * inclusive.
         * @param blockRefVert used to return whether the block
         * containing the requested annulus is vertically reflected
         * within this region (see SatBlockSpec for details).  This
         * will be set to \c true if the block is vertically reflected,
         * or \c false if not.
         * @param blockRefHoriz used to return whether the block
         * containing the requested annulus is horizontally reflected
         * within this region (see SatBlockSpec for details).  This
         * will be set to \c true if the block is horizontally reflected,
         * or \c false if not.
         * @return details of the requested boundary annulus, precisely
         * as it appears within its particular saturated block.
         */
        const SatAnnulus& boundaryAnnulus(unsigned long which,
            bool& blockRefVert, bool& blockRefHoriz) const;
        /**
         * Returns fine details of the requested saturated annulus on
         * the boundary of this region.
         *
         * The argument \a which specifies which one of these
         * annuli should be returned.  See the
         * boundaryAnnulus(unsigned long, bool&, bool&) documentation
         * for details on how the boundary annuli are numbered.
         *
         * Various details of the requested boundary annulus are
         * returned in the various arguments, as described below.
         *
         * Be aware that the block containing the requested annulus
         * might be reflected horizontally and/or vertically within the
         * overall region, as discussed in the SatBlockSpec class notes.
         * This will affect how the annulus is positioned as
         * part of the overall region boundary (e.g., the annulus might
         * be positioned upside-down in the overall region boundary,
         * or it might be positioned with its second triangle appearing before
         * its first triangle as one walks around the boundary).  The two
         * boolean arguments \a blockRefVert and \a blockRefHoriz will
         * be modified to indicate if and how the block is reflected.
         *
         * \warning This routine is quite slow, since it currently scans
         * through every annulus of every saturated block.  Use it
         * sparingly!
         *
         * \ifacespython This routine only takes a single argument (the
         * integer \a which).  The return value is a tuple of four values:
         * the block returned in \a block, the integer returned in \a annulus,
         * the boolean returned in \a blockRefVert, and the boolean returned
         * in \a blockRefHoriz.
         *
         * @param which specifies which boundary annulus of this region
         * to return; this must be between 0 and numberOfBoundaryAnnuli()-1
         * inclusive.
         * @param block used to return the particular saturated block
         * containing the requested annulus.
         * @param annulus used to return which annulus number in the
         * returned block is the requested annulus; this will be between
         * 0 and block->nAnnuli() inclusive.
         * @param blockRefVert used to return whether the block
         * containing the requested annulus is vertically reflected
         * within this region (see SatBlockSpec for details).  This
         * will be set to \c true if the block is vertically reflected,
         * or \c false if not.
         * @param blockRefHoriz used to return whether the block
         * containing the requested annulus is horizontally reflected
         * within this region (see SatBlockSpec for details).  This
         * will be set to \c true if the block is horizontally reflected,
         * or \c false if not.
         */
        void boundaryAnnulus(unsigned long which,
            SatBlock*& block, unsigned& annulus,
            bool& blockRefVert, bool& blockRefHoriz) const;

        /**
         * Returns details of the Seifert fibred space represented by
         * this region.
         *
         * Each boundary component of this region will be formed from a ring
         * of saturated annuli, which together form a torus or a Klein bottle.
         * For torus boundary components, the oriented curves
         * representing the fibres and base orbifold on the boundary
         * (see \ref sfsnotation) will be as follows.
         *
         * - Consider the 0/1/2 markings on the first and second triangles
         *   of each saturated annulus, as described in the SatAnnulus
         *   class notes.
         * - The fibres are represented by the oriented edge joining
         *   markings 1 and 0 on the first triangle (or 0 and 1 on the
         *   second triangle).  This is reversed if the block containing the
         *   boundary annulus is vertically reflected.
         * - The curve representing the base orbifold run along the
         *   oriented edge joining markings 0 and 2 on the first triangle
         *   (or 2 and 0 on the second triangle).  This is reversed if the
         *   block containing the boundary annulus is horizontally
         *   reflected.
         * - See the SatBlockSpec overview for descriptions of
         *   horizontal and vertical reflection.
         *
         * If the argument \a reflect is \c true, the Seifert fibred
         * space will be created as though the entire region had been
         * reflected.  In particular, each twist or exceptional fibre
         * will be negated before being added to the Seifert structure.
         *
         * For Klein bottle boundary components, these curves must (for
         * now) be analysed by hand.
         *
         * @param reflect \c true if this region is to be reflected
         * as the Seifert fibred space is created, or \c false if not.
         * @return the newly created structure of the underlying Seifert
         * fibred space.
         */
        SFSpace* createSFS(bool reflect) const;

        /**
         * Expands this region as far as possible within the overall
         * triangulation.  This routine will hunt for new saturated
         * blocks, and will also hunt for new adjacencies between
         * existing blocks.
         *
         * The first argument to this routine is the tetrahedron list
         * \a avoidTets.  This is a list of tetrahedra that will not be
         * considered when examining potential new blocks.  This list
         * will be modified by this routine; in particular, it will be
         * expanded to include all tetrahedra for any new blocks that
         * are found.  Before calling this routine it should contain
         * tetrahedra for blocks already in this region, as discussed in
         * the preconditions below.
         *
         * It may be that you are searching for a region that fills an entire
         * triangulation component (i.e., every boundary annulus of the
         * region in fact forms part of the boundary of the triangulation).
         * In this case you may pass the optional argument
         * \a stopIfIncomplete as \c true.  This means that if this routine
         * ever discovers an annulus that is not part of the
         * triangulation boundary and that it cannot match with some
         * adjacent block, it will exit immediately and return \c false.
         * Note that the region structure will be incomplete and/or
         * inconsistent if this happens; in this case the unfinished
         * region should be destroyed completely and never used.
         *
         * For internal purposes, it should be noted that any new blocks
         * that are discovered will be added to the end of the internal
         * block list (thus the indices of existing blocks will not change).
         *
         * \warning When joining blocks together, it is possible to
         * create invalid edges (e.g., by joining a one-annulus
         * untwisted boundary to a one-annulus twisted boundary).
         * This routine does \e not check for such conditions.  It is
         * recommended that you run Triangulation<3>::isValid() before
         * calling this routine.
         *
         * \pre If any blocks already belonging to this region have
         * adjacencies listed in their SatBlock structures, then these
         * adjacent blocks belong to this region also.  This precondition
         * is easily satisfied if you let the SatRegion constructor
         * and expand() do all of the adjacency handling, as described
         * in the class notes.
         * \pre The list \a avoidTets includes all tetrahedra on the
         * boundaries of any blocks already contained in this region.
         *
         * \ifacespython The first argument \a avoidTets is not present.
         * An empty list will be passed instead.
         *
         * @param avoidTets a list of tetrahedra that should not be
         * considered for new blocks, as discussed above.  Note that
         * this list may be modified by this routine.
         * @param stopIfIncomplete \c true if you are filling an entire
         * triangulation component with this region and you wish this
         * routine to exit early if this is not possible, or \c false
         * (the default) if you simply wish to expand this region as far
         * as you can.  See above for further discussion.
         * @return \c false if the optional argument \a stopIfIncomplete
         * was passed as \c true but expansion did not fill the entire
         * triangulation component as described above, or \c true in all
         * other cases.
         */
        bool expand(SatBlock::TetList& avoidTets,
            bool stopIfIncomplete = false);

        /**
         * Writes an abbreviated list of blocks within this region to
         * the given output stream.  Blocks will be written using their
         * abbreviated names, and these names will be separated by
         * commas.  See SatBlock::writeAbbr() for further details.
         *
         * The blocks within this region will be sorted before their
         * abbreviated names are output.  The particular method of sorting
         * is an arbitrary aesthetic decision on the part of the author,
         * and is subject to change in future versions of Regina.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @param tex \c true if the output should be formatted for TeX,
         * or \c false if it should be written as plain text.
         */
        void writeBlockAbbrs(std::ostream& out, bool tex = false) const;

        /**
         * Writes details of the composition of this region to the given
         * output stream.
         *
         * The output will consist of several lines.  The first line
         * will contain the title string (passed as a separate argument
         * to this routine), followed by a colon.  Following this will be
         * a number of lines describing the individual blocks that make
         * up this region and the various adjacencies between them.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @param title the name of this region, to be written on the
         * first line of output.
         */
        void writeDetail(std::ostream& out, const std::string& title) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Runs through the region structure and recalculates the
         * \a baseEuler_ data member.
         *
         * No assumptions are made about whether edges of the boundary
         * annuli become identified due to features outside the region.
         * That is, this routine is safe to call even when this region
         * is joined to some other not-yet-understood sections of the
         * triangulation.
         */
        void calculateBaseEuler();

        /**
         * Each boundary component of this region will be formed from a ring
         * of saturated annuli, which is either untwisted (forming a
         * torus), or twisted (forming a Klein bottle).
         * This routine counts the total number of boundaries of each type.
         *
         * @param untwisted returns the number of untwisted (torus)
         * boundary components.
         * @param twisted returns the number of twisted (Klein bottle)
         * boundary components.
         */
        void countBoundaries(unsigned& untwisted, unsigned& twisted) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSatRegion has now been renamed to
 * SatRegion.
 */
REGINA_DEPRECATED typedef SatRegion NSatRegion;

/*@}*/

// Inline functions for SatBlockSpec

inline SatBlockSpec::SatBlockSpec() {
}

inline SatBlockSpec::SatBlockSpec(SatBlock* useBlock, bool useRefVert,
        bool useRefHoriz) : block(useBlock), refVert(useRefVert),
        refHoriz(useRefHoriz) {
}

inline bool SatBlockSpec::operator == (const SatBlockSpec& other) const {
    return block == other.block && refVert == other.refVert &&
        refHoriz == other.refHoriz;
}

inline bool SatBlockSpec::operator != (const SatBlockSpec& other) const {
    return block != other.block || refVert != other.refVert ||
        refHoriz != other.refHoriz;
}

// Inline functions for SatRegion

inline unsigned long SatRegion::numberOfBlocks() const {
    return blocks_.size();
}

inline const SatBlockSpec& SatRegion::block(unsigned long which) const {
    return blocks_[which];
}

inline unsigned long SatRegion::numberOfBoundaryAnnuli() const {
    return nBdryAnnuli_;
}

inline void SatRegion::writeTextLong(std::ostream& out) const {
    writeDetail(out, "Saturated region");
}

} // namespace regina

#endif

