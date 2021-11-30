
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#ifndef __REGINA_SATREGION_H
#ifndef __DOXYGEN
#define __REGINA_SATREGION_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "subcomplex/satblock.h"
#include <optional>
#include <vector>

namespace regina {

/**
 * Describes how a single saturated block forms a part of a larger
 * saturated region.
 *
 * A saturated region consists of several saturated blocks joined
 * together along their boundary annuli.  This is a helper class
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
 * Since Regina 7.0, this helper structure now owns its internal SatBlock,
 * and will destroy this SatBlock when this structure itself is destroyed.
 * This is a change in behaviour from Regina 6.0.1 and earlier.
 *
 * Because of these new ownership semantics, SatBlockSpec is no longer
 * copyable.  However, it is both movable and swappable (it implements
 * C++ moves semantics and adheres to the C++ Swappable requirement).
 *
 * \ingroup subcomplex
 */
class SatBlockSpec {
    private:
        SatBlock* block_;
            /**< Details of the saturated block structure. */
        bool refVert_;
            /**< Indicates whether the block is reflected vertically within
                 the larger region.  See the class notes for details. */
        bool refHoriz_;
            /**< Indicates whether the block is reflected horizontally within
                 the larger region.  See the class notes for details. */

    public:
        /**
         * Moves the contents of the given structure into this new structure.
         *
         * The internal SatBlock pointer will be preserved; that is, the
         * pointer <tt>this->block()</tt> after the move will be the
         * same as the pointer <tt>src.block()</tt> before the move.
         *
         *
         * The structure that was passed (\a src) will no longer be usable.
         */
        SatBlockSpec(SatBlockSpec&& src) noexcept;

        /**
         * Destroys this structure, along with the SatBlock that it holds.
         */
        ~SatBlockSpec();

        /**
         * Moves the contents of the given structure into this structure.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * The internal SatBlock pointer will be preserved; that is, the
         * pointer <tt>this->block()</tt> after the move will be the
         * same as the pointer <tt>src.block()</tt> before the move.
         *
         * @return a reference to this structure.
         */
        SatBlockSpec& operator = (SatBlockSpec&& src) noexcept;

        /**
         * Swaps the contents of this and the given structure.
         *
         * In particular, the internal SatBlock pointers will be swapped; that
         * is, the pointers <tt>this->block()</tt> and <tt>other.block()</tt>
         * after the move will be the same as <tt>other.block()</tt> and
         * <tt>this->block()</tt> were respectively before the move.
         *
         * @param other the structure whose contents should be swapped with
         * this.
         */
        void swap(SatBlockSpec& other) noexcept;

        /**
         * Determines whether this and the given structure contain identical
         * information.  In particular, their block() pointers must be
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
         * block() pointers are different (even if they point to different
         * blocks with the same contents).
         *
         * @param other the structure to compare against this.
         * @return \c true if and only if this and \a other contain
         * different information.
         */
        bool operator != (const SatBlockSpec& other) const;

        /**
         * Returns the full combinatorial structure of the saturated block.
         *
         * @return the saturated block structure.
         */
        const SatBlock* block() const;
        /**
         * Returns whether the block is reflected vertically within
         * the larger region.  See the class notes for details.
         *
         * @return \c true if and only if the block is reflected vertically.
         */
        bool refVert() const;
        /**
         * Returns whether the block is reflected horizontally within
         * the larger region.  See the class notes for details.
         *
         * @return \c true if and only if the block is reflected horizontally.
         */
        bool refHoriz() const;

        // Ensure the class is non-constructible (to the public) and
        // non-copyable.
        SatBlockSpec() = delete;
        SatBlockSpec(const SatBlockSpec&) = delete;
        SatBlockSpec& operator = (const SatBlockSpec&) = delete;

    private:
        /**
         * Creates a new structure that is initialised to the given content.
         *
         * @param block details of the saturated block structure.
         * @param refVert \c true if the block is reflected vertically
         * within the larger region, or \c false otherwise.
         * @param refHoriz \c true if the block is reflected horizontally
         * within the larger region, or \c false otherwise.
         */
        SatBlockSpec(SatBlock* block, bool refVert, bool refHoriz);

    friend class SatRegion;
};

/**
 * Swaps the contents of the two given structures.
 *
 * In particular, the internal SatBlock pointers will be swapped; that
 * is, the pointers <tt>a.block()</tt> and <tt>b.block()</tt>
 * after the move will be the same as <tt>b.block()</tt> and
 * <tt>a.block()</tt> were respectively before the move.
 *
 * This global routine simply calls SatBlockSpec::swap(); it is provided
 * so that SatBlockSpec meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SatBlockSpec& a, SatBlockSpec& b) noexcept;

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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * the only way to create objects of this class (aside from copying or moving)
 * is via the static search functions, such as find() or beginsRegion().
 *
 * \warning It is crucial that the adjacency information stored in the
 * blocks is consistent with the region containing them.  All this
 * requires is that the blocks are not manipulated externally (e.g.,
 * SatBlock::setAdjacent() is not called on any of the blocks), but
 * instead all adjacency information is managed by this class.
 *
 * \todo \feature Have this class track the boundary components properly,
 * with annuli grouped and oriented according to the region boundaries (as
 * opposed to individual block boundaries).
 *
 * \ingroup subcomplex
 */
class SatRegion : public Output<SatRegion> {
    private:
        std::vector<SatBlockSpec> blocks_;
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
        size_t twistedBlocks_;
            /**< The number of constituent blocks with twisted boundary.
                 Each such block provides an additional twisted reflector
                 boundary to the base orbifold; see SatBlock::adjustSFS()
                 for details. */

        size_t nBdryAnnuli_;
            /**< The number of saturated annuli forming the boundary
                 components (if any) of this region. */

        static std::list<SatBlockModel> starters_;
            /**< The blocks from which find() will begin its searches.  These
                 are generated on demand, the first time find() is called. */

    public:
        /**
         * Creates a new copy of the given region.
         *
         * @param src the region to copy.
         */
        SatRegion(const SatRegion& src);
        /**
         * Moves the contents of the given region into this new region.
         * This is a fast (constant time) operation.
         *
         * The region that was passed will no longer be usable.
         */
        SatRegion(SatRegion&&) noexcept = default;
        /**
         * Sets this to be a copy of the given region.
         *
         * @param src the region to copy.
         * @return a reference to this region.
         */
        SatRegion& operator = (const SatRegion& src);
        /**
         * Moves the contents of the given region into this region.
         * This is a fast (constant time) operation.
         *
         * The region that was passed will no longer be usable.
         *
         * @return a reference to this region.
         */
        SatRegion& operator = (SatRegion&&) noexcept = default;

        /**
         * Swaps the contents of this and the given region.
         *
         * @param other the region whose contents should be swapped with this.
         */
        void swap(SatRegion& other) noexcept;

        /**
         * Returns the number of saturated blocks that come together
         * to form this saturated region.
         *
         * @return the total number of blocks.
         */
        size_t countBlocks() const;
        /**
         * Deprecated routine that returns the number of saturated blocks
         * in this saturated region.
         *
         * \deprecated This routine has been renamed countBlocks().
         *
         * @return the total number of blocks.
         */
        [[deprecated]] size_t numberOfBlocks() const;
        /**
         * Returns details of the requested saturated block within this
         * region.  The information will returned will include
         * structural information for the block, along with details of
         * how the block is aligned (e.g., reflected vertically or
         * horizontally) within the larger region.
         *
         * @param which indicates which of the constituent blocks should
         * be returned; this must be between 0 and countBlocks()-1
         * inclusive.
         * @return details of the requested saturated block.
         */
        const SatBlockSpec& block(size_t which) const;
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
         * 0 and countBlocks()-1 inclusive), or -1 if the block is
         * not part of this region.
         */
        long blockIndex(const SatBlock* block) const;

        /**
         * Returns the number of saturated annuli that together form the
         * boundary components of this region.
         *
         * @return the number of boundary annuli.
         */
        size_t countBoundaryAnnuli() const;
        /**
         * Deprecated routine that returns the number of saturated annuli
         * in the boundary of this region.
         *
         * \deprecated This routine has been renamed countBoundaryAnnuli().
         *
         * @return the total number of boundary annuli.
         */
        [[deprecated]] size_t numberOfBoundaryAnnuli() const;
        /**
         * Returns details of the requested saturated annulus on
         * the boundary of this region.
         *
         * The saturated annuli that together form the boundary
         * components of this region are numbered from 0 to
         * countBoundaryAnnuli()-1 inclusive.  The argument \a which
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
         * Be aware that the block containing the requested annulus
         * might be reflected horizontally and/or vertically within the
         * overall region, as discussed in the SatBlockSpec class notes.
         * This will affect how the annulus is positioned as
         * part of the overall region boundary (e.g., the annulus might
         * be positioned upside-down in the overall region boundary,
         * or it might be positioned with its second triangle appearing before
         * its first triangle as one walks around the boundary).  To account
         * for this, the return value will include two booleans \a refVert and
         * \a refHoriz to indicate if and how the block is reflected.
         *
         * \warning This routine is quite slow, since it currently scans
         * through every annulus of every saturated block.  Use it
         * sparingly!
         *
         * \exception InvalidArgument the argument \a which was not
         * between 0 and countBoundaryAnnuli()-1 inclusive.
         *
         * @param which specifies which boundary annulus of this region to
         * return; this must be between 0 and countBoundaryAnnuli()-1 inclusive.
         * @return a tuple (\a block, annulus, refVert, refHoriz), where:
         * \a block is a pointer to the the particular saturated block
         * containing the requested annulus;
         * \a annulus indicates which annulus number in the returned block
         * is the requested annulus, as an integer between 0 and
         * block->countAnnuli()-1 inclusive;
         * \a refVert is \c true if and only if the block containing the
         * requested annulus is vertically reflected within this region; and
         * \a refHoriz is \c true if and only if the block containing the
         * requested annulus is horizontally reflected within this region.
         * See SatBlockSpec for further details on these reflections.
         */
        std::tuple<const SatBlock*, unsigned, bool, bool> boundaryAnnulus(
            size_t which) const;

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
         * There are situations in which Regina is not (yet) capable of
         * determining the Seifert fibred space precisely.  This can only
         * happen if the Seifert fibred space is closed, non-orientable,
         * belongs to the class \c n3 or \c n4, and has sufficiently large
         * base orbifold genus.  In such situations this routine will
         * throw an exception.
         *
         * \exception NotImplemented This is one of the closed
         * non-orientable cases where Regina cannot distinguish between
         * classes \c n3 and \c n4, as described above.
         *
         * @param reflect \c true if this region is to be reflected
         * as the Seifert fibred space is created, or \c false if not.
         * @return the structure of the underlying Seifert fibred space.
         */
        SFSpace createSFS(bool reflect) const;

        /**
         * Returns an abbreviated list of blocks within this region in
         * string format.  The string that is returned will consist of
         * the abbreviated names of all blocks in this region, separated by
         * commas and spaces.  See SatBlock::abbr() for further details.
         *
         * The blocks within this region will be sorted before their
         * abbreviated names are output.  The particular method of sorting
         * is an arbitrary aesthetic decision on the part of the author,
         * and is subject to change in future versions of Regina.
         *
         * @param tex \c true if the output should be formatted for TeX,
         * or \c false if it should be written as plain text.
         * @return the abbreviated list of all blocks.
         */
        std::string blockAbbrs(bool tex = false) const;

        /**
         * Writes an abbreviated list of blocks within this region to
         * the given output stream.  Blocks will be written using their
         * abbreviated names, and these names will be separated by
         * commas and spaces.  See SatBlock::writeAbbr() for further details.
         *
         * The blocks within this region will be sorted before their
         * abbreviated names are output.  The particular method of sorting
         * is an arbitrary aesthetic decision on the part of the author,
         * and is subject to change in future versions of Regina.
         *
         * \ifacespython Not present; instead use the variant blockAbbrs()
         * that returns a string.
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
         * \ifacespython Not present; instead use detail(), which returns
         * the same detailed information in string form (but without the
         * option of using a custom title).
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
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Runs a search for every isomorphic embedding of every
         * starter block from the global SatBlockStarterSet within the
         * given triangulation.
         *
         * Each time an embedding of a starter block is discovered, the block
         * will be wrapped in a new SatRegion which describes how the block
         * appears within the given triangulation.  The region will be expanded
         * to encompass as many saturated blocks as possible, and then passed to
         * \a action, which must be a function or some other callable object.
         *
         * - The first argument to \a action must be of type
         *   std::unique_ptr<SatRegion>; this will be the newly constructed
         *   and expanded region that contains the starter block that was found.
         *   As expected from std::unique_ptr, this function will have
         *   relinquished all ownership of the region, and your action
         *   can do what it likes with it.
         *
         * - The second argument to \a action must be of type
         *   SatBlock::TetList&.  This list will contain all tetrahedra
         *   currently used by the region, and \a action is welcome to
         *   modify the list as it pleases.  This function will clear and
         *   reuse the list after \a action returns.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return a \c bool.  A return value of \c false
         *   indicates that the search for starter blocks should continue,
         *   and a return value of \c true indicates that the search
         *   should terminate immediately.
         *
         * Note that different embeddings of the same starter block within
         * \a tri will result in the action being called multiple times
         * (with different containing regions).
         *
         * If you are searching for a region that fills an entire triangulation
         * component (i.e., every boundary annulus of the region in fact forms
         * part of the boundary of the triangulation), then you should pass
         * \a mustBeComplete as \c true.  If a region expansion does not fill
         * the entire component, then it will be discarded and \a action
         * will not be called for that particular embeddeding of that
         * particular starter block.
         *
         * \headers The implementation of this template function is contained
         * in a separate header (satregion-impl.h), which is not included
         * automatically by this file.  If you wish to use this function
         * in your own code, you will need to include satregion-impl.h.
         *
         * \ifacespython This function is available, and \a action may be a
         * pure Python function.  However, \a action must take only one
         * argument: the newly constructed SatRegion (i.e., the first argument
         * in the description above).  The SatBlock::TetList& argument will
         * \e not be passed, and there can be no additional argument list
         * \a args.
         *
         * @param tri the triangulation in which to search for starter blocks.
         * @param mustBeComplete \c true if you are searching for a region
         * that fills an entire triangulation component, as described above.
         * @param action a function (or other callable object) to call
         * for each embedding of a starter block that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial region and tetrahedron list
         * arguments.
         * @return \c true if \a action ever terminated the search by returning
         * \c true, or \c false if the search was allowed to run to completion.
         */
        template <typename Action, typename... Args>
        static bool find(const Triangulation<3>& tri, bool mustBeComplete,
            Action&& action, Args&&... args);

        /**
         * Determines whether the given annulus is in fact a boundary
         * annulus for a saturated region.  The annulus should be represented
         * from the inside of the proposed saturated region.
         *
         * All tetrahedra in the given list \a avoidTets will be ignored by
         * this routine, and so if a region is found then it is guaranteed
         * not to include any of them.  As a consequence, if the given
         * annulus uses any of these tetrahedra then \c null will be returned.
         *
         * If a region is found, it will be expanded as far as possible,
         * and all of the tetrahedra within it will be added to the
         * list \a avoidTets.  Moreover, it is guaranteed that the given
         * annulus will be listed as annulus number 0 in the block
         * that contains it, without any horizontal or vertical reflection.
         *
         * \pre Either the given annulus lies on the boundary of the
         * triangulation, or else the (one or two) tetrahedra attached to
         * the other side of it are already in the list \a avoidTets.
         * This is necessary to ensure that the saturated region does not
         * expand through the annulus to the other side.
         *
         * @param annulus the proposed boundary annulus that should form
         * part of the new saturated region.
         * @param avoidTets the list of tetrahedra that should not be
         * considered, and to which any new tetrahedra will be added.
         * @return details of the saturated region if one was found, or
         * \c null if none was found.
         */
        static std::unique_ptr<SatRegion> beginsRegion(
            const SatAnnulus& annulus, SatBlock::TetList& avoidTets);

    private:
        /**
         * Constructs a new region containing just the given block.
         * All boundary annuli of the given block will become boundary
         * annuli of this region.  It is guaranteed that this block will
         * be stored in the region without any kind of reflection (see
         * SatBlockSpec for details).
         *
         * Typically a region is initialised using this constructor, and
         * then grown using the expand() routine.
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
         * Performs the first step of beginsRegion().
         *
         * This routine behaves exactly like beginsRegion(), except that
         * it will only find a single saturated block (i.e., the block
         * that meets the given annulus).
         *
         * The caller of this routine must take ownership of the block
         * that is returned (if any).
         */
        static SatBlock* hasBlock(const SatAnnulus& annulus,
            SatBlock::TetList& avoidTets);

        /**
         * Fills the \a starters_ list with an appropriate set of model
         * block to search from, if this has not yet been done.
         * This routine is thread-safe.
         */
        static void initStarters();
};

/**
 * Swaps the contents of the two given regions.
 *
 * This global routine simply calls SatRegion::swap(); it is provided
 * so that SatRegion meets the C++ Swappable requirements.
 *
 * @param a the first region whose contents should be swapped.
 * @param b the second region whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SatRegion& a, SatRegion& b) noexcept;

// Inline functions for SatBlockSpec

inline SatBlockSpec::SatBlockSpec(SatBlockSpec&& src) noexcept :
        block_(src.block_), refVert_(src.refVert_), refHoriz_(src.refHoriz_) {
    src.block_ = nullptr;
}

inline SatBlockSpec& SatBlockSpec::operator = (SatBlockSpec&& src) noexcept {
    std::swap(block_, src.block_);
    refVert_ = src.refVert_;
    refHoriz_ = src.refHoriz_;
    // Let src dispose of the original block in its own destructor.
    return *this;
}

inline void SatBlockSpec::swap(SatBlockSpec& other) noexcept {
    std::swap(block_, other.block_);
    std::swap(refVert_, other.refVert_);
    std::swap(refHoriz_, other.refHoriz_);
}

inline SatBlockSpec::~SatBlockSpec() {
    delete block_;
}

inline const SatBlock* SatBlockSpec::block() const {
    return block_;
}

inline bool SatBlockSpec::refVert() const {
    return refVert_;
}

inline bool SatBlockSpec::refHoriz() const {
    return refHoriz_;
}

inline SatBlockSpec::SatBlockSpec(SatBlock* block, bool refVert,
        bool refHoriz) :
        block_(block), refVert_(refVert), refHoriz_(refHoriz) {
}

inline bool SatBlockSpec::operator == (const SatBlockSpec& other) const {
    return block_ == other.block_ && refVert_ == other.refVert_ &&
        refHoriz_ == other.refHoriz_;
}

inline bool SatBlockSpec::operator != (const SatBlockSpec& other) const {
    return block_ != other.block_ || refVert_ != other.refVert_ ||
        refHoriz_ != other.refHoriz_;
}

inline void swap(SatBlockSpec& a, SatBlockSpec& b) noexcept {
    a.swap(b);
}

// Inline functions for SatRegion

inline SatRegion& SatRegion::operator = (const SatRegion& src) {
    // We could probably do something slicker here, but the copy
    // operation is quite a bit of work (mainly because we need to fix
    // the block adjacencies) - let's just implement it once.
    SatRegion tmp(src);
    swap(tmp);
    return *this;
}

inline void SatRegion::swap(SatRegion& other) noexcept {
    blocks_.swap(other.blocks_);
    std::swap(baseEuler_, other.baseEuler_);
    std::swap(baseOrbl_, other.baseOrbl_);
    std::swap(hasTwist_, other.hasTwist_);
    std::swap(twistsMatchOrientation_, other.twistsMatchOrientation_);
    std::swap(shiftedAnnuli_, other.shiftedAnnuli_);
    std::swap(twistedBlocks_, other.twistedBlocks_);
    std::swap(nBdryAnnuli_, other.nBdryAnnuli_);
}

inline size_t SatRegion::countBlocks() const {
    return blocks_.size();
}

inline size_t SatRegion::numberOfBlocks() const {
    return blocks_.size();
}

inline const SatBlockSpec& SatRegion::block(size_t which) const {
    return blocks_[which];
}

inline size_t SatRegion::countBoundaryAnnuli() const {
    return nBdryAnnuli_;
}

inline size_t SatRegion::numberOfBoundaryAnnuli() const {
    return nBdryAnnuli_;
}

inline std::string SatRegion::blockAbbrs(bool tex) const {
    std::ostringstream s;
    writeBlockAbbrs(s, tex);
    return s.str();
}

inline void SatRegion::writeTextLong(std::ostream& out) const {
    writeDetail(out, "Saturated region");
}

inline std::unique_ptr<SatRegion> SatRegion::beginsRegion(
        const SatAnnulus& annulus, SatBlock::TetList& avoidTets) {
    SatBlock* starter = hasBlock(annulus, avoidTets);
    if (starter) {
        std::unique_ptr<SatRegion> ans(new SatRegion(starter));
        ans->expand(avoidTets, false);
        return ans;
    } else
        return nullptr;
}

inline void swap(SatRegion& a, SatRegion& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

