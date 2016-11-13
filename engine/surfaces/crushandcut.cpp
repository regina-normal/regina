
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

#include <algorithm>
#include <thread>
#include "enumerate/treetraversal.h"
#include "surfaces/normalsurface.h"
#include "surfaces/prism.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * The bulk of this file contains the implementation for cutAlong(),
 * which cuts along a normal surface.
 *
 * The way this routine operates is as follows:
 *
 * - We add an extra set of vertex links to the original normal surface.  We
 *   refer to the regions inside these vertex links as "vertex neighbourhoods".
 *   These neighbourhoods are typically balls (though around ideal vertices
 *   they are cones over the corresponding boundary surfaces).
 *
 * - If we cut along the new normal surface, each tetrahedron falls
 *   apart into the following types of blocks:
 *
 *   + Triangular prisms, represented by the class TriPrism.  There are
 *     four types of triangular prism, corresponding to the four triangular
 *     normal disc types that bound them.
 *
 *   + Quadrilateral prisms, represented by the class QuadPrism.  There
 *     are three types of quadrilateral prism, corresponding to the
 *     three quadrilateral normal disc types that bound them.
 *
 *   + Tetrahedra truncated at all four vertices, represented by the
 *     class TruncTet.  There is only one type of truncated tetrahedron.
 *
 *   + Truncated half-tetrahedra, obtained by slicing a truncated
 *     tetrahedron along a quadrilateral normal disc and keeping one of
 *     the two halves that results.  This is represented by the class
 *     TruncHalfTet.  There are six types of truncated half-tetrahedra,
 *     corresponding to the three choices of "slicing quadrilateral" and
 *     the two choices of which half to keep.
 *
 *   The reason we add the extra vertex links is to keep this list of
 *   block types small; otherwise we must also deal with \e partially
 *   truncated tetrahedra and half-tetrahedra.
 *
 * - We triangulate each of the blocks.  There are two types of boundary
 *   for each block:  (i) boundary faces that run along the normal
 *   surface, and (ii) boundary faces that run along the joins between
 *   adjacent tetrahedra.  Faces (i) can be left alone (they will become
 *   the boundary of the final triangulation); faces (ii) need to be
 *   joined together according to how the original tetrahedra were
 *   joined together.  Note that a handful of type (i) boundary faces
 *   run along the extra vertex links, and so these will be glued back
 *   onto the missing vertex neighbourhoods at the end of the cutting
 *   procedure.
 *
 * - For each block, we organise the boundaries of type (ii) into
 *   quadrilaterals and hexagons (each of which is the intersection of
 *   the block with a single face of the enclosing tetrahedron).  These
 *   are represented by the classes BdryQuad and BdryHex respectively.
 *
 * - The overall cutting algorithm then works as follows:
 *
 *   + Triangulate each block.  The class TetBlockSet represents a full
 *     set of triangulated blocks within a single tetrahedron of the
 *     original triangulation.
 *
 *   + Glue together the type (ii) boundaries between adjacent blocks,
 *     using layerings as needed to make the triangulated quadrilaterals
 *     and hexagons compatible.
 *
 *   + Construct the missing vertex neighbourhoods and glue them back onto
 *     the appropriate type (i) block boundaries.
 *
 * See the individual classes for further details.
 */

// ------------------------------------------------------------------------
// Supporting classes for cutAlong()
// ------------------------------------------------------------------------

namespace {
    class Bdry;

    /**
     * A single triangulated block within a single tetrahedron of the
     * original triangulation.
     */
    class Block {
        protected:
            const Tetrahedron<3>* outerTet_;
                /**< The "outer tetrahedron".  This is the tetrahedron
                     of the original triangulation that contains this block. */
            Tetrahedron<3>** innerTet_;
                /**< The "inner tetrahedra".  These are the tetrahedra
                     used to triangulate this block, and also to
                     perform any necessary boundary layerings. */
            unsigned nInnerTet_;
                /**< The number of inner tetrahedra. */

            Bdry* bdry_[4];
                /**< The four quadrilateral / hexagonal type (ii) boundaries
                     of this block.  These are boundaries that meet faces of
                     the outer tetrahedron (not boundaries that run along the
                     original normal surface).  Specifically, bdry_[i]
                     is the boundary on face i of the outer tetrahedron
                     (or 0 if this block does not actually meet face i
                     of the outer tetrahedron). */

            Tetrahedron<3>* link_[4];
                /**< Indicates which inner tetrahedra in this block (if any)
                     face the vertices of the outer tetrahedron.
                     Specifically, if this block contains a triangle on its
                     boundary surrounding vertex i of the outer tetrahedron,
                     and if this triangle is facing vertex i (so the block
                     lies on the side of the triangle away from vertex i, not
                     towards vertex i), then link_[i] is the inner
                     tetrahedron containing this triangle.  Otherwise, link_[i]
                     is null. */
            Perm<4> linkVertices_[4];
                /**< If link_[i] is non-zero, then linkVertices_[i] is
                     a mapping from vertices of the inner tetrahedron
                     \a link_[i] to vertices of the outer tetrahedron
                     \a outerTet.  Specifically, if we let V denote
                     vertex i of the outer tetrahedron, then this mapping
                     sends the three vertices of the inner vertex linking
                     triangle surrounding V to the three "parallel" vertices of
                     the triangular face opposite V in the outer tetrahedron. */

        public:
            /**
             * Destroys the four boundaries, but none of the inner
             * tetrahedra or the outer tetrahedron.
             */
            virtual ~Block();

            /**
             * Returns the outer tetrahedron.
             */
            const Tetrahedron<3>* outerTet();

            /**
             * Glues this block to the given adjacent block.  This
             * involves taking the quadrilateral or hexagon boundary of
             * this block that sits on the given face of this block's
             * outer tetrahedron, and gluing it (using layerings if need
             * be) to the corresponding quadrilateral or hexagon of the
             * adjacent block.
             */
            void join(int face, Block* other);

            /**
             * Creates a new inner tetrahedron within this block.
             * It is assumed that this tetrahedron is to be used for
             * layering on the block boundary.  However, this layering
             * will not be performed by this routine (so the new tetrahedron
             * that is returned will be isolated).
             *
             * This routine assumes that the innerTet_ array has enough
             * space for a new tetrahedron (which should be true if the
             * correct arguments were passed to the Block constructor).
             *
             * The new tetrahedron will be automatically added to the
             * same triangulation as the previous tetrahedra in this block.
             *
             * \pre This block already contains at least one inner tetrahedron.
             */
            Tetrahedron<3>* layeringTetrahedron();

            /**
             * Attaches the triangle described by link_[vertex] to the
             * given "small tetrahedron" that forms part of the corresponding
             * vertex neighbourhood.  It is assumed that the small tetrahedron
             * in the neighbourhood will have its vertices numbered in a
             * way that represents a "shrunk-down" version of the outer
             * tetrahedron (where "shrunk-down" means dilation about the
             * given outer tetrahedron vertex).
             */
            void attachVertexNbd(Tetrahedron<3>* nbd, int vertex);

        protected:
            /**
             * Creates a new block within the given outer tetrahedron.
             * This constructor creates \a initialNumTet inner tetrahedra,
             * but also leaves enough extra room in the inner tetrahedron
             * array for up to \a maxLayerings layerings on the boundaries.
             *
             * All new inner tetrahedra created now and in subsequent
             * layerings will be automatically inserted into the given
             * triangulation.
             */
            Block(const Tetrahedron<3>* outerTet, unsigned initialNumTet,
                unsigned maxLayerings, Triangulation<3>* insertInto);
    };

    /**
     * A triangular prism, triangulated using three inner tetrahedra.
     *
     * See cut-triprism.fig for details of the triangulation.
     * In this diagram, inner tetrahedra are numbered T0, T1, ..., and
     * vertices of the inner tetrahedra are indicated using plain integers.
     * For a block of type 0 (see the constructor for details), vertices
     * of the outer tetrahedron are indicated using integers in circles.
     * For blocks of other types, vertex 0 is swapped with vertex \a type
     * in the outer tetrahedron.
     */
    class TriPrism : public Block {
        public:
            /**
             * Creates a new triangular prism within the given outer
             * tetrahedron.
             *
             * The given block type is an integer between 0 and 3
             * inclusive, describing which triangle type in the
             * outer tetrahedron supplies the two ends of the prism.
             *
             * Equivalently, the block type describes which vertex of
             * the outer tetrahedron this triangular prism surrounds.
             *
             * All new inner tetrahedra will be automatically
             * inserted into the given triangulation.
             */
            TriPrism(const Tetrahedron<3> *outerTet, int type,
                Triangulation<3>* insertInto);
    };

    /**
     * A quadrilateral prism, triangulated using five inner tetrahedra.
     *
     * See cut-quadprism.fig for details of the triangulation.
     * In this diagram, inner tetrahedra are numbered T0, T1, ..., and
     * vertices of the inner tetrahedra are indicated using plain integers.
     * For a block of type 1 (see the constructor for details), vertices
     * of the outer tetrahedron are indicated using integers in circles.
     * For blocks of other types, the vertices of the outer tetrahedron
     * are permuted accordingly.
     */
    class QuadPrism : public Block {
        public:
            /**
             * Creates a new quadrilateral prism within the given outer
             * tetrahedron.
             *
             * The given block type is an integer between 0 and 2
             * inclusive, describing which quadrilateral type in the
             * outer tetrahedron supplies the two ends of the prism.
             *
             * All new inner tetrahedra will be automatically
             * inserted into the given triangulation.
             */
            QuadPrism(const Tetrahedron<3> *outerTet, int type,
                Triangulation<3>* insertInto);
    };

    /**
     * A truncated half-tetrahedron, triangulated using eight inner tetrahedra.
     *
     * See cut-trunchalftet.fig for details of the triangulation.
     * In this diagram, inner tetrahedra are numbered T0, T1, ..., and
     * vertices of the inner tetrahedra are indicated using plain integers.
     * For a block of type 0 (see the constructor for details), vertices
     * of the outer tetrahedron are indicated using integers in circles.
     * For blocks of other types, the vertices of the outer tetrahedron
     * are permuted accordingly.
     */
    class TruncHalfTet : public Block {
        public:
            /**
             * Creates a new truncated half-tetrahedron within the given
             * outer tetrahedron.
             *
             * The given block type is an integer between 0 and 5
             * inclusive, describing which edge of the outer tetrahedron
             * this half-tetrahedron does not meet at all.
             *
             * All new inner tetrahedra will be automatically
             * inserted into the given triangulation.
             */
            TruncHalfTet(const Tetrahedron<3> *outerTet, int type,
                Triangulation<3>* insertInto);
    };

    /**
     * A truncated tetrahedron, triangulated using eleven inner tetrahedra.
     *
     * See cut-trunctet.fig for details of the triangulation.
     * In this diagram, inner tetrahedra are numbered T0, T1, ...,
     * vertices of the inner tetrahedra are indicated using plain integers,
     * and vertices of the outer tetrahedron are indicated using integers in
     * circles.
     */
    class TruncTet : public Block {
        public:
            /**
             * Creates a new truncated tetrahedron within the given outer
             * tetrahedron.
             *
             * All new inner tetrahedra will be automatically
             * inserted into the given triangulation.
             */
            TruncTet(const Tetrahedron<3> *outerTet, Triangulation<3>* insertInto);
    };

    /**
     * Represents a quadrilateral or hexagonal piece of a block boundary.
     * This is the intersection of a block with a single face of its
     * outer tetrahedron.
     *
     * For each such quadrilateral or hexagon, we number the faces from
     * 0 to 1 (for a quadrilateral) or 0 to 3 (for a hexagon); these are
     * called the \e inner boundary faces.  The enclosing face of the
     * outer tetrahedron is called the \e outer boundary face.
     *
     * See boundaries.fig for details of how each quadrilateral or
     * hexagon is triangulated.  The inner boundary faces are numbered
     * T0, T1, ..., the vertices of each inner boundary face are
     * numbered using plain integers (these are the \e inner vertex
     * numbers), and the vertices of the outer boundary face are numbered
     * using integers in circles (these are the \e outer vertex numbers).
     */
    class Bdry {
        protected:
            Block* block_;
                /**< The block whose boundary this is a piece of. */
            Perm<4> outerVertices_;
                /**< A mapping from the outer vertex numbers 0, 1 and 2
                     to the corresponding vertex numbers in the
                     outer tetrahedron (block_->outerTet). */

        public:
            /**
             * A virtual destructor that does nothing.
             */
            virtual ~Bdry();

            /**
             * Identifies (i.e., glues together) this piece of boundary
             * and the given piece of boundary, performing layerings if
             * required to make sure that the boundaries are compatible.
             *
             * This routine assumes that this and the given piece of
             * boundary are the same shape (i.e., both quadrilaterals or
             * both hexagons).
             */
            virtual void join(Bdry* other) = 0; /* PRE: other is same shape */

        protected:
            /**
             * Initialises a new object with the given block and the
             * given mapping from outer vertex numbers to vertices of
             * the outer tetrahedron.
             */
            Bdry(Block* block, Perm<4> outerVertices);
    };

    /**
     * A piece of block boundary that is a triangulated quadrilateral.
     *
     * See boundaries.fig for details of how the quadrilateral is
     * triangulated, and see the Bdry class notes for what all the
     * numbers on this diagram actually mean.
     */
    class BdryQuad : public Bdry {
        private:
            Tetrahedron<3>* innerTet_[2];
                /**< The two inner tetrahedra of the block that supply the
                     two inner boundary faces for this quadrilateral. */
            Perm<4> innerVertices_[2];
                /**< For the ith inner boundary face, the permutation
                     innerVertices_[i] maps the inner vertex numbers
                     0, 1 and 2 to the corresponding vertex numbers in
                     the inner tetrahedron innerTet_[i]. */

        public:
            /**
             * See Bdry::join() for details.
             */
            virtual void join(Bdry* other);

        private:
            /**
             * Initialises a new object with the given block and the
             * given mapping from outer vertex numbers to vertices of
             * the outer tetrahedron.
             */
            BdryQuad(Block* block, Perm<4> outerVertices);

            /**
             * Layers a new tetrahedron upon the quadrilateral boundary, so
             * that the triangulated quadrilateral becomes a reflection of
             * itself.  As a result, the diagram in boundaries.fig will
             * likewise become reflected, and so the faces and vertex numbers
             * within this diagram will now refer to different tetrahedra
             * and vertices within the underlying block.
             */
            void reflect();

        friend class TriPrism;
        friend class QuadPrism;
        friend class TruncHalfTet;
    };

    /**
     * A piece of block boundary that is a triangulated hexagon.
     *
     * See boundaries.fig for details of how the hexagon is
     * triangulated, and see the Bdry class notes for what all the
     * numbers on this diagram actually mean.
     */
    class BdryHex : public Bdry {
        private:
            Tetrahedron<3>* innerTet_[4];
                /**< The four inner tetrahedra of the block that supply the
                     four inner boundary faces for this quadrilateral. */
            Perm<4> innerVertices_[4];
                /**< For the ith inner boundary face, the permutation
                     innerVertices_[i] maps the inner vertex numbers
                     0, 1 and 2 to the corresponding vertex numbers in
                     the inner tetrahedron innerTet_[i]. */

        public:
            /**
             * See Bdry::join() for details.
             */
            virtual void join(Bdry* other);

        private:
            /**
             * Initialises a new object with the given block and the
             * given mapping from outer vertex numbers to vertices of
             * the outer tetrahedron.
             */
            BdryHex(Block* block, Perm<4> outerVertices);

            /**
             * Layers four new tetrahedra upon the hexagon boundary, so
             * that the triangulated hexagon becomes a reflection of
             * itself.  As a result, the diagram in boundaries.fig will
             * likewise become reflected, and so the faces and vertex numbers
             * within this diagram will now refer to different tetrahedra
             * and vertices within the underlying block.
             */
            void reflect();

            /**
             * Rotates the diagram from boundaries.fig by a one-third turn,
             * so that the faces and vertex numbers in boundaries.fig
             * correspond to different tetrahedra and vertex numbers in
             * the underlying block.
             *
             * This is simply a relabelling operation; no layerings are
             * performed, and no changes are made to the triangulation
             * of the block itself.
             */
            void rotate();

        friend class TruncHalfTet;
        friend class TruncTet;
    };

    /**
     * Stores a full set of triangulated blocks within a single
     * "outer" tetrahedron of the original triangulation, as formed by
     * cutting along some normal surface within this original triangulation.
     */
    class TetBlockSet {
        private:
            unsigned long triCount_[4];
                /**< The number of triangular normal discs of each type
                     within this outer tetrahedron.  This does \e not
                     include the "extra" vertex links that we add to
                     slice off a neighbourhood of each vertex of the
                     original triangulation. */
            unsigned long quadCount_;
                /**< The number of quadrilateral normal discs (of any type)
                     within this outer tetrahedron.  The \e type of these
                     quadrilaterals is stored in the separate data
                     member \a quadType_. */
            int quadType_;
                /**< The unique quadrilateral normal disc \e type that appears
                     within this outer tetrahedron.  This will be 0, 1 or 2
                     if there are indeed quadrilateral discs (i.e., quadCount_
                     is positive), or -1 if this outer tetrahedron contains no
                     quadrilateral discs at all (i.e., quadCount_ is zero). */

            Block** triPrism_[4];
                /**< The array triPrism_[i] contains all of the triangular
                     prism blocks surrounding vertex \a i of the outer
                     tetrahedron, or is null if there are no such blocks.
                     Such blocks exist if and only if the normal surface
                     contains at least one triangular disc of type \a i.
                     If these blocks do exist, they are stored in order moving
                     \e away from vertex \a i of the outer tetrahedron
                     (or equivalently, moving in towards the centre of
                     the outer tetrahedron). */
            Block** quadPrism_;
                /**< An array containing all of the quadrilateral prism
                     blocks, or null if there are no such blocks within this
                     outer tetrahedron.
                     These blocks exist if and only if the normal
                     surface contains two or more quadrilateral discs.
                     If these blocks do exist, they are stored in order moving
                     \e away from vertex 0 of the outer tetrahedron. */
            Block* truncHalfTet_[2];
                /**< The two truncated half-tetrahedron blocks, or null if
                     there are no such blocks within this outer tetrahedron.
                     These blocks exist if and only if the normal
                     surface contains one or more quadrilateral discs.
                     In this case, the block truncHalfTet_[0] is closer
                     to vertex 0 of the outer tetrahedron, and the block
                     truncHalfTet_[1] is further away. */
            Block* truncTet_;
                /**< The unique truncated tetrahedron block, or null if there
                     is no such block within this outer tetrahedron.
                     This block exists if and only if the normal surface
                     contains no quadrilateral discs. */

            Tetrahedron<3>* vertexNbd_[4];
                /**< The four small tetrahedra that contribute to the
                     vertex neighbourhoods surrounding the four vertices
                     of the outer tetrahedron.  The vertices of each small
                     tetrahedron are numbered in a way that matches the
                     outer tetrahedron (so the small tetrahedron vertexNbd_[i]
                     looks like the outer tetrahedron, shrunk down using a
                     dilation about vertex \a i of the outer tetrahedron). */

        public:
            /**
             * Creates a full set of triangulated blocks within the given
             * outer tetrahedron, as formed by cutting along the given normal
             * surface.
             *
             * This contructor also creates the four small tetrahedra in
             * the vertex neighbourhoods, and glues them to the four
             * blocks closest to the outer tetrahedron vertices.
             *
             * All new inner tetrahedra (that is, the inner tetrahedra
             * from the triangulated blocks and also the small
             * tetrahedra in the vertex neighbourhoods) will be automatically
             * inserted into the given triangulation.
             */
            TetBlockSet(const NormalSurface* s, unsigned long tetIndex,
                Triangulation<3>* insertInto);

            /**
             * Destroys all block and boundary structures within this outer
             * tetrahedron.
             *
             * Note that the inner tetrahedra that make up the triangulated
             * blocks are \e not destroyed (since presumably we are keeping
             * these inner tetrahedra for the new sliced-open triangulation
             * that we plan to give back to the user).
             */
            ~TetBlockSet();

            /**
             * Returns the number of blocks that provide quadrilateral
             * boundaries on the given face of the outer tetrahedron,
             * surrounding the given vertex of the outer tetrahedron.
             *
             * It is assumed that \a face and \a fromVertex are not equal.
             */
            unsigned long numQuadBlocks(int face, int fromVertex);
            /**
             * Returns the requested block that provides a quadrilateral
             * boundary on some particular face of the outer tetrahedron,
             * surrounding the given vertex of the outer tetrahedron.
             *
             * Ordinarily the face number would be passed; however,
             * it is omitted because it is not actually necessary.
             * Nevertheless, the choice of face number affects how \e many
             * such blocks are available; see numQuadBlocks() for details.
             *
             * Blocks are numbered 0,1,... outwards from the given vertex of
             * the outer tetrahedron, in towards the centre of the outer
             * tetrahedron.  The argument \a whichBlock indicates which of
             * these blocks should be returned.
             *
             * It is assumed that \a whichBlock is strictly less than
             * numQuadBlocks(\a face, \a fromVertex), where \a face is
             * the relevant face of the outer tetrahedron.
             */
            Block* quadBlock(int fromVertex, unsigned long whichBlock);
            /**
             * Returns the (unique) block that provides a hexagon
             * boundary on the given face of the outer tetrahedron, or
             * null if there is no such block.
             */
            Block* hexBlock(int face);

            /**
             * Returns the small tetrahedron that contributes to the
             * vertex neighbourhood surrounding the given vertex of the
             * outer tetrahedron.
             *
             * See the data member \a vertexNbd_ for further details.
             */
            Tetrahedron<3>* vertexNbd(int vertex);
    };

    inline Block::~Block() {
        for (unsigned i = 0; i < 4; ++i)
            delete bdry_[i];
        delete[] innerTet_;
    }

    inline const Tetrahedron<3>* Block::outerTet() {
        return outerTet_;
    }

    inline void Block::join(int face, Block* other) {
        bdry_[face]->join(other->bdry_[outerTet_->adjacentFace(face)]);
    }

    inline Tetrahedron<3>* Block::layeringTetrahedron() {
        return (innerTet_[nInnerTet_++] =
            innerTet_[0]->triangulation()->newTetrahedron());
    }

    inline void Block::attachVertexNbd(Tetrahedron<3>* nbd, int vertex) {
        link_[vertex]->join(linkVertices_[vertex].preImageOf(vertex),
            nbd, linkVertices_[vertex]);
    }

    inline Block::Block(const Tetrahedron<3> *outerTet, unsigned initialNumTet,
            unsigned maxLayerings, Triangulation<3>* insertInto) :
            outerTet_(outerTet),
            innerTet_(new Tetrahedron<3>*[initialNumTet + maxLayerings]),
            nInnerTet_(initialNumTet) {
        unsigned i;
        for (i = 0; i < nInnerTet_; ++i)
            innerTet_[i] = insertInto->newTetrahedron();
        std::fill(link_, link_ + 4, static_cast<Tetrahedron<3>*>(0));
    }

    TriPrism::TriPrism(const Tetrahedron<3> *outerTet, int type,
            Triangulation<3>* insertInto) :
            Block(outerTet, 3, 3, insertInto) {
        innerTet_[1]->join(1, innerTet_[0], Perm<4>());
        innerTet_[1]->join(3, innerTet_[2], Perm<4>());

        Perm<4> vertices = Perm<4>(0, type);

        BdryQuad* q;

        bdry_[vertices[0]] = 0;

        q = new BdryQuad(this, vertices * Perm<4>(0, 2, 3, 1));
        q->innerTet_[0] = innerTet_[1];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = Perm<4>(2, 3, 1, 0);
        q->innerVertices_[1] = Perm<4>(1, 3, 2, 0);
        bdry_[vertices[1]] = q;

        q = new BdryQuad(this, vertices * Perm<4>(2, 3));
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = Perm<4>(2, 1, 0, 3);
        q->innerVertices_[1] = Perm<4>(0, 3, 2, 1);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices);
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[1];
        q->innerVertices_[0] = Perm<4>(3, 1, 0, 2);
        q->innerVertices_[1] = Perm<4>(0, 1, 3, 2);
        bdry_[vertices[3]] = q;

        link_[vertices[0]] = innerTet_[0];
        linkVertices_[vertices[0]] = vertices * Perm<4>(0, 1, 3, 2);
    }

    QuadPrism::QuadPrism(const Tetrahedron<3> *outerTet, int type,
            Triangulation<3>* insertInto) :
            Block(outerTet, 5, 4, insertInto) {
        innerTet_[4]->join(2, innerTet_[0], Perm<4>());
        innerTet_[4]->join(3, innerTet_[1], Perm<4>());
        innerTet_[4]->join(0, innerTet_[2], Perm<4>());
        innerTet_[4]->join(1, innerTet_[3], Perm<4>());

        Perm<4> vertices(
            regina::quadDefn[type][0],
            regina::quadDefn[type][2],
            regina::quadDefn[type][1],
            regina::quadDefn[type][3]);

        BdryQuad* q;

        q = new BdryQuad(this, vertices * Perm<4>(2, 3, 1, 0));
        q->innerTet_[0] = innerTet_[2];
        q->innerTet_[1] = innerTet_[1];
        q->innerVertices_[0] = Perm<4>(1, 0, 2, 3);
        q->innerVertices_[1] = Perm<4>(2, 3, 1, 0);
        bdry_[vertices[0]] = q;

        q = new BdryQuad(this, vertices * Perm<4>(3, 0, 2, 1));
        q->innerTet_[0] = innerTet_[3];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = Perm<4>(2, 1, 3, 0);
        q->innerVertices_[1] = Perm<4>(3, 0, 2, 1);
        bdry_[vertices[1]] = q;

        q = new BdryQuad(this, vertices * Perm<4>(0, 1, 3, 2));
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[3];
        q->innerVertices_[0] = Perm<4>(3, 2, 0, 1);
        q->innerVertices_[1] = Perm<4>(0, 1, 3, 2);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices * Perm<4>(1, 2, 0, 3));
        q->innerTet_[0] = innerTet_[1];
        q->innerTet_[1] = innerTet_[0];
        q->innerVertices_[0] = Perm<4>(0, 3, 1, 2);
        q->innerVertices_[1] = Perm<4>(1, 2, 0, 3);
        bdry_[vertices[3]] = q;
    }

    TruncHalfTet::TruncHalfTet(const Tetrahedron<3> *outerTet, int type,
            Triangulation<3>* insertInto):
            Block(outerTet, 8, 10, insertInto) {
        innerTet_[1]->join(2, innerTet_[0], Perm<4>());
        innerTet_[1]->join(1, innerTet_[2], Perm<4>());
        innerTet_[1]->join(0, innerTet_[3], Perm<4>());
        innerTet_[2]->join(0, innerTet_[4], Perm<4>());
        innerTet_[3]->join(1, innerTet_[4], Perm<4>());
        innerTet_[3]->join(3, innerTet_[5], Perm<4>());
        innerTet_[5]->join(2, innerTet_[6], Perm<4>());
        innerTet_[4]->join(2, innerTet_[7], Perm<4>());

        Perm<4> vertices(
            Edge<3>::edgeVertex[type][0],
            Edge<3>::edgeVertex[type][1],
            Edge<3>::edgeVertex[5 - type][0],
            Edge<3>::edgeVertex[5 - type][1]);

        BdryQuad* q;
        BdryHex* h;

        h = new BdryHex(this, vertices * Perm<4>(1, 3, 2, 0));
        h->innerTet_[0] = innerTet_[2];
        h->innerTet_[1] = innerTet_[7];
        h->innerTet_[2] = innerTet_[5];
        h->innerTet_[3] = innerTet_[4];
        h->innerVertices_[0] = Perm<4>(2, 0, 1, 3);
        h->innerVertices_[1] = Perm<4>(1, 2, 0, 3);
        h->innerVertices_[2] = Perm<4>(0, 3, 2, 1);
        h->innerVertices_[3] = Perm<4>(0, 2, 1, 3);
        bdry_[vertices[0]] = h;

        h = new BdryHex(this, vertices * Perm<4>(0, 3, 2, 1));
        h->innerTet_[0] = innerTet_[0];
        h->innerTet_[1] = innerTet_[7];
        h->innerTet_[2] = innerTet_[6];
        h->innerTet_[3] = innerTet_[3];
        h->innerVertices_[0] = Perm<4>(1, 2, 3, 0);
        h->innerVertices_[1] = Perm<4>(3, 2, 0, 1);
        h->innerVertices_[2] = Perm<4>(0, 2, 1, 3);
        h->innerVertices_[3] = Perm<4>(0, 1, 3, 2);
        bdry_[vertices[1]] = h;

        q = new BdryQuad(this, vertices * Perm<4>(3, 1, 0, 2));
        q->innerTet_[0] = innerTet_[2];
        q->innerTet_[1] = innerTet_[0];
        q->innerVertices_[0] = Perm<4>(3, 1, 0, 2);
        q->innerVertices_[1] = Perm<4>(0, 2, 3, 1);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices * Perm<4>(2, 0, 1, 3));
        q->innerTet_[0] = innerTet_[6];
        q->innerTet_[1] = innerTet_[5];
        q->innerVertices_[0] = Perm<4>(3, 2, 1, 0);
        q->innerVertices_[1] = Perm<4>(1, 2, 3, 0);
        bdry_[vertices[3]] = q;

        link_[vertices[2]] = innerTet_[6];
        linkVertices_[vertices[2]] = vertices * Perm<4>(3, 2, 0, 1);

        link_[vertices[3]] = innerTet_[7];
        linkVertices_[vertices[3]] = vertices * Perm<4>(3, 1, 2, 0);
    }

    TruncTet::TruncTet(const Tetrahedron<3> *outerTet, Triangulation<3>* insertInto) :
            Block(outerTet, 11, 16, insertInto) {
        innerTet_[0]->join(2, innerTet_[4], Perm<4>());
        innerTet_[1]->join(3, innerTet_[7], Perm<4>());
        innerTet_[2]->join(0, innerTet_[6], Perm<4>());
        innerTet_[3]->join(1, innerTet_[9], Perm<4>());
        innerTet_[5]->join(3, innerTet_[4], Perm<4>());
        innerTet_[5]->join(1, innerTet_[6], Perm<4>());
        innerTet_[8]->join(0, innerTet_[7], Perm<4>());
        innerTet_[8]->join(2, innerTet_[9], Perm<4>());
        innerTet_[4]->join(1, innerTet_[10], Perm<4>());
        innerTet_[6]->join(3, innerTet_[10], Perm<4>());
        innerTet_[7]->join(2, innerTet_[10], Perm<4>());
        innerTet_[9]->join(0, innerTet_[10], Perm<4>());

        BdryHex* h;

        h = new BdryHex(this, Perm<4>(2, 1, 3, 0));
        h->innerTet_[0] = innerTet_[2];
        h->innerTet_[1] = innerTet_[8];
        h->innerTet_[2] = innerTet_[3];
        h->innerTet_[3] = innerTet_[9];
        h->innerVertices_[0] = Perm<4>(2, 0, 1, 3);
        h->innerVertices_[1] = Perm<4>(1, 2, 0, 3);
        h->innerVertices_[2] = Perm<4>(0, 1, 2, 3);
        h->innerVertices_[3] = Perm<4>(0, 2, 1, 3);
        bdry_[0] = h;

        h = new BdryHex(this, Perm<4>(3, 2, 0, 1));
        h->innerTet_[0] = innerTet_[3];
        h->innerTet_[1] = innerTet_[5];
        h->innerTet_[2] = innerTet_[0];
        h->innerTet_[3] = innerTet_[4];
        h->innerVertices_[0] = Perm<4>(3, 1, 2, 0);
        h->innerVertices_[1] = Perm<4>(2, 3, 1, 0);
        h->innerVertices_[2] = Perm<4>(1, 2, 3, 0);
        h->innerVertices_[3] = Perm<4>(1, 3, 2, 0);
        bdry_[1] = h;

        h = new BdryHex(this, Perm<4>(0, 3, 1, 2));
        h->innerTet_[0] = innerTet_[0];
        h->innerTet_[1] = innerTet_[8];
        h->innerTet_[2] = innerTet_[1];
        h->innerTet_[3] = innerTet_[7];
        h->innerVertices_[0] = Perm<4>(0, 2, 3, 1);
        h->innerVertices_[1] = Perm<4>(3, 0, 2, 1);
        h->innerVertices_[2] = Perm<4>(2, 3, 0, 1);
        h->innerVertices_[3] = Perm<4>(2, 0, 3, 1);
        bdry_[2] = h;

        h = new BdryHex(this, Perm<4>(1, 0, 2, 3));
        h->innerTet_[0] = innerTet_[1];
        h->innerTet_[1] = innerTet_[5];
        h->innerTet_[2] = innerTet_[2];
        h->innerTet_[3] = innerTet_[6];
        h->innerVertices_[0] = Perm<4>(1, 3, 0, 2);
        h->innerVertices_[1] = Perm<4>(0, 1, 3, 2);
        h->innerVertices_[2] = Perm<4>(3, 0, 1, 2);
        h->innerVertices_[3] = Perm<4>(3, 1, 0, 2);
        bdry_[3] = h;

        link_[0] = innerTet_[0];
        linkVertices_[0] = Perm<4>(1, 2, 3, 0);

        link_[1] = innerTet_[1];
        linkVertices_[1] = Perm<4>(1, 2, 3, 0);

        link_[2] = innerTet_[2];
        linkVertices_[2] = Perm<4>(1, 2, 3, 0);

        link_[3] = innerTet_[3];
        linkVertices_[3] = Perm<4>(1, 2, 3, 0);
    }

    inline Bdry::~Bdry() {
        // Empty virtual destructor.
    }

    inline Bdry::Bdry(Block* block, Perm<4> outerVertices) :
            block_(block), outerVertices_(outerVertices) {
    }

    void BdryQuad::join(Bdry* other) {
        // Assume other is a BdryQuad.
        BdryQuad* dest = static_cast<BdryQuad*>(other);

        // Get the map from *this* 012 to *dest* tetrahedron vertices.
        Perm<4> destMap = block_->outerTet()->
            adjacentGluing(outerVertices_[3]) * outerVertices_;

        if (destMap != dest->outerVertices_) {
            // A reflection is our only recourse.
            dest->reflect();
            if (destMap != dest->outerVertices_) {
                // This should never happen.
                std::cerr << "ERROR: Cannot match up BdryQuad pair."
                    << std::endl;
                ::exit(1);
            }
        }

        // Now we match up perfectly.
        for (int i = 0; i < 2; ++i)
            innerTet_[i]->join(innerVertices_[i][3], dest->innerTet_[i],
                dest->innerVertices_[i] * innerVertices_[i].inverse());
    }

    inline BdryQuad::BdryQuad(Block* block, Perm<4> outerVertices) :
            Bdry(block, outerVertices) {
    }

    void BdryQuad::reflect() {
        Tetrahedron<3>* layering = block_->layeringTetrahedron();

        layering->join(0, innerTet_[1],
            innerVertices_[1] * Perm<4>(3, 2, 1, 0));
        layering->join(2, innerTet_[0],
            innerVertices_[0] * Perm<4>(1, 0, 3, 2));

        innerTet_[0] = innerTet_[1] = layering;
        innerVertices_[0] = Perm<4>();
        innerVertices_[1] = Perm<4>(2, 3, 0, 1);

        outerVertices_ = outerVertices_ * Perm<4>(1, 2);
    }

    void BdryHex::join(Bdry* other) {
        // Assume other is a BdryQuad.
        BdryHex* dest = static_cast<BdryHex*>(other);

        // Get the map from *this* 012 to *dest* tetrahedron vertices.
        Perm<4> destMap = block_->outerTet()->
            adjacentGluing(outerVertices_[3]) * outerVertices_;

        if (destMap.sign() != dest->outerVertices_.sign())
            dest->reflect();

        while (destMap != dest->outerVertices_)
            dest->rotate();

        // Now we match up perfectly.
        for (int i = 0; i < 4; ++i)
            innerTet_[i]->join(innerVertices_[i][3], dest->innerTet_[i],
                dest->innerVertices_[i] * innerVertices_[i].inverse());
    }

    inline BdryHex::BdryHex(Block* block, Perm<4> outerVertices) :
            Bdry(block, outerVertices) {
    }

    void BdryHex::reflect() {
        Tetrahedron<3>* layering0 = block_->layeringTetrahedron();
        Tetrahedron<3>* layering1 = block_->layeringTetrahedron();
        Tetrahedron<3>* layering2 = block_->layeringTetrahedron();
        Tetrahedron<3>* layering3 = block_->layeringTetrahedron();

        layering0->join(1, innerTet_[3], innerVertices_[3] * Perm<4>(1, 3));
        layering0->join(2, innerTet_[2], innerVertices_[2] * Perm<4>(2, 3));
        layering1->join(3, layering0, Perm<4>());
        layering1->join(1, innerTet_[1],
            innerVertices_[1] * Perm<4>(2, 3, 0, 1));
        layering2->join(0, layering0, Perm<4>());
        layering2->join(1, innerTet_[0],
            innerVertices_[0] * Perm<4>(1, 3, 2, 0));
        layering3->join(0, layering1, Perm<4>());
        layering3->join(3, layering2, Perm<4>());

        innerTet_[0] = layering2;
        innerTet_[1] = layering1;
        innerTet_[2] = layering3;
        innerTet_[3] = layering3;

        innerVertices_[0] = Perm<4>(0, 3, 1, 2);
        innerVertices_[1] = Perm<4>(1, 0, 3, 2);
        innerVertices_[2] = Perm<4>(3, 2, 0, 1);
        innerVertices_[3] = Perm<4>(3, 0, 1, 2);

        outerVertices_ = outerVertices_ * Perm<4>(1, 2);
    }

    void BdryHex::rotate() {
        Tetrahedron<3>* t = innerTet_[0];
        innerTet_[0] = innerTet_[1];
        innerTet_[1] = innerTet_[2];
        innerTet_[2] = t;

        Perm<4> p = innerVertices_[0];
        innerVertices_[0] = innerVertices_[1];
        innerVertices_[1] = innerVertices_[2];
        innerVertices_[2] = p;
        innerVertices_[3] = innerVertices_[3] * Perm<4>(1, 2, 0, 3);

        outerVertices_ = outerVertices_ * Perm<4>(1, 2, 0, 3);
    }

    TetBlockSet::TetBlockSet(const NormalSurface* s, unsigned long tetIndex,
            Triangulation<3>* insertInto) {
        unsigned long i, j;
        for (i = 0; i < 4; ++i)
            triCount_[i] = s->triangles(tetIndex, i).longValue();

        LargeInteger coord;
        if ((coord = s->quads(tetIndex, 0)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 0;
        } else if ((coord = s->quads(tetIndex, 1)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 1;
        } else if ((coord = s->quads(tetIndex, 2)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 2;
        } else {
            quadCount_ = 0;
            quadType_ = -1;
        }

        const Tetrahedron<3>* tet = s->triangulation()->tetrahedron(tetIndex);

        // Build the blocks.
        // Note in all of this that we insert an extra "fake" triangle at each
        // vertex (i.e., the entire surface gains a fake set of extra vertex
        // links).
        for (i = 0; i < 4; ++i) {
            if (triCount_[i] == 0)
                triPrism_[i] = 0;
            else {
                triPrism_[i] = new Block*[triCount_[i]];
                for (j = 0; j < triCount_[i]; ++j)
                    triPrism_[i][j] = new TriPrism(tet, i, insertInto);
            }
        }

        if (quadCount_ == 0) {
            quadPrism_ = 0;
            truncHalfTet_[0] = truncHalfTet_[1] = 0;
            truncTet_ = new TruncTet(tet, insertInto);
        } else {
            if (quadCount_ > 1) {
                quadPrism_ = new Block*[quadCount_ - 1];
                for (j = 0; j < quadCount_ - 1; ++j)
                    quadPrism_[j] = new QuadPrism(tet, quadType_, insertInto);
            } else
                quadPrism_ = 0;

            truncHalfTet_[0] = new TruncHalfTet(tet, 5 - quadType_, insertInto);
            truncHalfTet_[1] = new TruncHalfTet(tet, quadType_, insertInto);

            truncTet_ = 0;
        }

        for (i = 0; i < 4; ++i) {
            vertexNbd_[i] = insertInto->newTetrahedron();

            if (triCount_[i] > 0)
                triPrism_[i][0]->attachVertexNbd(vertexNbd_[i], i);
            else if (quadCount_ == 0)
                truncTet_->attachVertexNbd(vertexNbd_[i], i);
            else if (i == 0 ||
                    static_cast<int>(i) == Edge<3>::edgeVertex[quadType_][1])
                truncHalfTet_[0]->attachVertexNbd(vertexNbd_[i], i);
            else
                truncHalfTet_[1]->attachVertexNbd(vertexNbd_[i], i);
        }
    }

    TetBlockSet::~TetBlockSet() {
        unsigned long i, j;
        for (i = 0; i < 4; ++i)
            if (triPrism_[i]) {
                for (j = 0; j < triCount_[i]; ++j)
                    delete triPrism_[i][j];
                delete[] triPrism_[i];
            }

        if (quadCount_ == 0) {
            delete truncTet_;
        } else {
            if (quadPrism_) {
                for (j = 0; j < quadCount_ - 1; ++j)
                    delete quadPrism_[j];
                delete[] quadPrism_;
            }

            delete truncHalfTet_[0];
            delete truncHalfTet_[1];
        }
    }

    unsigned long TetBlockSet::numQuadBlocks(int face, int fromVertex) {
        // We see all triangular discs surrounding fromVertex.
        unsigned long ans = triCount_[fromVertex];

        if (quadType_ == regina::quadSeparating[face][fromVertex]) {
            // We also see the quadrilateral discs.
            ans += quadCount_;
        }

        return ans;
    }

    Block* TetBlockSet::quadBlock(int fromVertex,
            unsigned long whichBlock) {
        // First come the triangular prisms.
        if (whichBlock < triCount_[fromVertex])
            return triPrism_[fromVertex][whichBlock];

        // Next comes the truncated half-tetrahedron.
        if (whichBlock == triCount_[fromVertex]) {
            if (fromVertex == 0 ||
                    fromVertex == Edge<3>::edgeVertex[quadType_][1])
                return truncHalfTet_[0];
            else
                return truncHalfTet_[1];
        }

        // Finally we have the quad prisms.
        if (fromVertex == 0 || fromVertex == Edge<3>::edgeVertex[quadType_][1])
            return quadPrism_[whichBlock - triCount_[fromVertex] - 1];
        else
            return quadPrism_[
                quadCount_ - (whichBlock - triCount_[fromVertex]) - 1];
    }

    Block* TetBlockSet::hexBlock(int face) {
        if (quadCount_ == 0)
            return truncTet_;

        if (face == 0 || face == Edge<3>::edgeVertex[quadType_][1])
            return truncHalfTet_[1];
        return truncHalfTet_[0];
    }

    inline Tetrahedron<3>* TetBlockSet::vertexNbd(int vertex) {
        return vertexNbd_[vertex];
    }
}

// ------------------------------------------------------------------------
// Implementation of cutAlong()
// ------------------------------------------------------------------------

Triangulation<3>* NormalSurface::cutAlong() const {
    Triangulation<3>* ans = new Triangulation<3>();
    Packet::ChangeEventSpan span(ans);

    unsigned long nTet = triangulation()->size();
    if (nTet == 0)
        return ans;

    unsigned long i;
    TetBlockSet** sets = new TetBlockSet*[nTet];
    for (i = 0; i < nTet; ++i)
        sets[i] = new TetBlockSet(this, i, ans);

    Triangulation<3>::TriangleIterator fit;
    Triangle<3>* f;
    unsigned long tet0, tet1;
    int face0, face1;
    int fromVertex0, fromVertex1;
    Perm<4> gluing;
    unsigned long quadBlocks;
    for (fit = triangulation()->triangles().begin();
            fit != triangulation()->triangles().end(); ++fit) {
        f = *fit;
        if (f->isBoundary())
            continue;

        tet0 = f->embedding(0).tetrahedron()->markedIndex();
        tet1 = f->embedding(1).tetrahedron()->markedIndex();
        face0 = f->embedding(0).triangle();
        face1 = f->embedding(1).triangle();

        gluing = f->front().tetrahedron()->adjacentGluing(face0);

        for (fromVertex0 = 0; fromVertex0 < 4; ++fromVertex0) {
            if (fromVertex0 == face0)
                continue;
            fromVertex1 = gluing[fromVertex0];

            quadBlocks = sets[tet0]->numQuadBlocks(face0, fromVertex0);
            for (i = 0; i < quadBlocks; ++i)
                sets[tet0]->quadBlock(fromVertex0, i)->join(
                    face0, sets[tet1]->quadBlock(fromVertex1, i));

            sets[tet0]->vertexNbd(fromVertex0)->join(
                face0, sets[tet1]->vertexNbd(fromVertex1), gluing);
        }
        sets[tet0]->hexBlock(face0)->join(face0, sets[tet1]->hexBlock(face1));
    }

    // All done!  Clean up.
    for (i = 0; i < nTet; ++i)
        delete sets[i];
    delete[] sets;

    return ans;
}

// ------------------------------------------------------------------------
// Implementation of crush()
// ------------------------------------------------------------------------

Triangulation<3>* NormalSurface::crush() const {
    Triangulation<3>* ans = new Triangulation<3>(*triangulation_, false);
    unsigned long nTet = ans->size();
    if (nTet == 0)
        return ans;

    // Work out which tetrahedra contain which quad types.
    int* quadTypes = new int[nTet];
    long whichTet = 0;
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++) {
        if (quads(whichTet, 0) != 0)
            quadTypes[whichTet] = 0;
        else if (quads(whichTet, 1) != 0)
            quadTypes[whichTet] = 1;
        else if (quads(whichTet, 2) != 0)
            quadTypes[whichTet] = 2;
        else
            quadTypes[whichTet] = -1;
    }

    // Run through and fix the tetrahedron gluings.
    Tetrahedron<3>* tet;
    Tetrahedron<3>* adj;
    int adjQuads;
    Perm<4> adjPerm;
    Perm<4> swap;
    int face, adjFace;
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++)
        if (quadTypes[whichTet] == -1) {
            // We want to keep this tetrahedron, so make sure it's glued
            // up correctly.
            tet = ans->tetrahedron(whichTet);
            for (face = 0; face < 4; face++) {
                adj = tet->adjacentTetrahedron(face);
                if (! adj)
                    continue;
                adjQuads = quadTypes[adj->index()];
                if (adjQuads == -1)
                    continue;

                // We're glued to a bad tetrahedron.  Follow around
                // until we reach a good tetrahedron or a boundary.
                adjPerm = tet->adjacentGluing(face);
                adjFace = adjPerm[face];
                while (adj && (adjQuads >= 0)) {
                    swap = Perm<4>(adjFace, quadPartner[adjQuads][adjFace]);

                    adjFace = swap[adjFace];
                    adjPerm = adj->adjacentGluing(adjFace) *
                        swap * adjPerm;
                    adj = adj->adjacentTetrahedron(adjFace);
                    adjFace = adjPerm[face];

                    if (adj)
                        adjQuads = quadTypes[adj->index()];
                }

                // Reglue the tetrahedron face accordingly.
                tet->unjoin(face);
                if (! adj)
                    continue;

                // We haven't yet unglued the face of adj since there is
                // at least one bad tetrahedron between tet and adj.
                adj->unjoin(adjFace);
                tet->join(face, adj, adjPerm);
            }
        }

    // Delete unwanted tetrahedra.
    for (whichTet = nTet - 1; whichTet >= 0; whichTet--)
        if (quadTypes[whichTet] >= 0)
            ans->removeTetrahedronAt(whichTet);

    delete[] quadTypes;
    return ans;
}

bool NormalSurface::isCompressingDisc(bool knownConnected) const {
    // Is it even a disc?
    if (! hasRealBoundary())
        return false;
    if (eulerChar() != 1)
        return false;

    if (! knownConnected) {
        if (! isConnected())
            return false;
    }

    // Yep, it's a disc (and hence two-sided).

    // Count the number of boundary spheres that our triangulation has
    // to begin with.
    unsigned long origSphereCount = 0;
    Triangulation<3>::BoundaryComponentIterator bit;
    for (bit = triangulation()->boundaryComponents().begin();
            bit != triangulation()->boundaryComponents().end(); ++bit)
        if ((*bit)->eulerChar() == 2)
            ++origSphereCount;

    // Now cut along the disc, and see if we get an extra sphere as a
    // result.  If not, the disc boundary is non-trivial and so the disc
    // is compressing.
    std::unique_ptr<Triangulation<3>> cut(cutAlong());

    if (cut->countBoundaryComponents() ==
            triangulation()->countBoundaryComponents()) {
        // The boundary of the disc is not a separating curve in the
        // boundary of the triangulation.  Therefore we might end up
        // converting a torus boundary into a sphere boundary, but the
        // disc is compressing regardless.
        return true;
    }

    unsigned long newSphereCount = 0;
    for (bit = cut->boundaryComponents().begin();
            bit != cut->boundaryComponents().end(); ++bit)
        if ((*bit)->eulerChar() == 2)
            ++newSphereCount;

    if (newSphereCount == origSphereCount)
        return true;
    else
        return false;
}

/**
 * Supporting classes for isIncompressible().
 */
namespace {
    /**
     * Manages two parallel searches for compressing discs.
     * Each search works with a single connected triangulation with boundary.
     * If one search reports that it has found a compressing disc,
     * then it will cancel the other (by calling hasFound()).
     */
    class SharedSearch {
        private:
            // Information common to both threads:
            bool found_;
            std::mutex foundMutex_;

            // Information specific to each thread:
            Triangulation<3>* t_[2];
            TreeSingleSoln<LPConstraintEuler, BanNone>* currSearch_[2];
            std::mutex searchMutex_[2];

        public:
            inline SharedSearch(Triangulation<3>* t0, Triangulation<3>* t1) :
                    found_(false) {
                t_[0] = t0;
                t_[1] = t1;
                currSearch_[0] = currSearch_[1] = 0;
            }

            inline bool hasFound() {
                std::lock_guard<std::mutex> lock(foundMutex_);
                return found_;
            }

            inline void markFound() {
                std::lock_guard<std::mutex> lock(foundMutex_);
                found_ = true;
                for (int i = 0; i < 2; ++i)
                    if (t_[i]) {
                        std::lock_guard<std::mutex> lock(searchMutex_[i]);
                        if (currSearch_[i])
                            currSearch_[i]->cancel();
                    }
            }

            /**
             * Side-effect: runSearch(side) deletes t_[side].
             */
            void runSearch(const int side) {
                if (hasFound()) {
                    delete t_[side];
                    return;
                }

                t_[side]->intelligentSimplify();

                if (hasFound()) {
                    delete t_[side];
                    return;
                }

                // Try for a simple answer first.
                if (t_[side]->hasSimpleCompressingDisc()) {
                    markFound();
                    delete t_[side];
                    return;
                }

                if (hasFound()) {
                    delete t_[side];
                    return;
                }

                // The LP-and-crush method is only suitable for
                // orientable triangulations with a single boundary component.
                if (t_[side]->countBoundaryComponents() > 1 ||
                        ! t_[side]->isOrientable()) {
                    // Fall back to the slow and non-cancellable method.
                    if (t_[side]->hasCompressingDisc())
                        markFound();
                    delete t_[side];
                    return;
                }

                // Compute the Euler characteristic of the boundary component.
                long ec = t_[side]->boundaryComponent(0)->eulerChar();

                // Look for a normal disc or sphere to crush.
                NormalSurface* ans;
                Triangulation<3>* crush;
                unsigned nComp;
                bool found;
                while (true) {
                    t_[side]->intelligentSimplify();

                    // The LP-and-crushing method only works for
                    // 1-vertex triangulations (at present).
                    if (t_[side]->countVertices() > 1) {
                        // Try harder.
                        t_[side]->barycentricSubdivision();
                        t_[side]->intelligentSimplify();
                        if (t_[side]->countVertices() > 1) {
                            // Fall back to the old (slow and uncancellable)
                            // method.
                            if (t_[side]->hasCompressingDisc())
                                markFound();
                            delete t_[side];
                            return;
                        }
                    }

                    if (hasFound()) {
                        delete t_[side];
                        return;
                    }

                    TreeSingleSoln<LPConstraintEuler, BanNone> search(t_[side],
                        NS_STANDARD);
                    {
                        std::lock_guard<std::mutex> lock(searchMutex_[side]);
                        currSearch_[side] = &search;
                    }
                    found = search.find();
                    {
                        std::lock_guard<std::mutex> lock(searchMutex_[side]);
                        currSearch_[side] = 0;
                    }

                    if (hasFound()) {
                        delete t_[side];
                        return;
                    }

                    if (! found) {
                        // No discs or spheres.
                        // In particular, no compressing disc.
                        delete t_[side];
                        return;
                    }

                    // TreeSingleSoln guarantees that our solution is
                    // connected, and so it (or its double) is a sphere or
                    // a disc.
                    ans = search.buildSurface();
                    crush = ans->crush();
                    delete ans;
                    delete t_[side];

                    // Find the piece in the crushed triangulation with the
                    // right Euler characteristic on the boundary, if it exists.
                    nComp = crush->splitIntoComponents();
                    t_[side] = static_cast<Triangulation<3>*>(
                        crush->firstChild());
                    while (t_[side]) {
                        if (t_[side]->countBoundaryComponents() == 1 &&
                                t_[side]->boundaryComponent(0)->
                                    eulerChar() == ec) {
                            // Found it.
                            t_[side]->makeOrphan();
                            break;
                        }

                        t_[side] = static_cast<Triangulation<3>*>(
                            t_[side]->nextSibling());
                    }

                    delete crush;

                    if (! t_[side]) {
                        // No boundary component with the right Euler
                        // characteristic.  We must have compressed.
                        markFound();
                        return;
                    }

                    // We now have a triangulation with fewer tetrahedra,
                    // which contains a compressing disc iff the original did.
                    // Around we go again!
                }
            }
    };
} // anonymous namespace

bool NormalSurface::isIncompressible() const {
    // We don't bother making the surface two-sided.  This is because
    // cutting along the two-sided surface will produce (i) exactly what
    // you obtain from cutting along the one-sided surface, plus
    // (ii) a twisted I-bundle over a surface that will not contain any
    // compressing discs.

    // Rule out spheres.
    // From the preconditions, we can assume this surface to be
    // closed, compact and connected.
    if (eulerChar() == 2 || ((! isTwoSided()) && eulerChar() == 1))
        return false;

    if (isThinEdgeLink().first) {
        // Since the manifold is closed and this surface is not a
        // sphere, the edge it links must be a loop and the surface must
        // surround a solid torus or Klein bottle.
        return false;
    }

    // Time for the heavy machinery.
    Triangulation<3>* cut = cutAlong();
    cut->intelligentSimplify();

    Triangulation<3>* side[2];
    side[0] = side[1] = 0;

    cut->splitIntoComponents();
    int which = 0;
    for (Packet* comp = cut->firstChild(); comp; comp = comp->nextSibling())
        if (static_cast<Triangulation<3>*>(comp)->hasBoundaryTriangles()) {
            if (which == 2) {
                // We have more than two components with boundary.
                // This should never happen.
                std::cerr << "ERROR: isIncompressible() sliced to give "
                    "more than two components with boundary." << std::endl;
                delete cut;
                return false;
            }
            side[which++] = static_cast<Triangulation<3>*>(comp);
        }

    // Detach from parents so we don't run into multithreading problems
    // (e.g., when both triangulations try to delete themselves at the
    // same time).
    side[0]->makeOrphan();
    if (side[1])
        side[1]->makeOrphan();
    delete cut;

    SharedSearch ss(side[0], side[1]);

    if (! side[1]) {
        ss.runSearch(0);
    } else {
        // Test both sides for compressing discs in parallel,
        // so we can terminate early if one side finds such a disc.
        //
        // Note that we need to pass &ss as a pointer, to avoid the
        // std::thread constructor trying to make a deep copy of ss.
        std::thread t0(&SharedSearch::runSearch, &ss, 0);
        std::thread t1(&SharedSearch::runSearch, &ss, 1);
        t0.join();
        t1.join();
    }

    return ! ss.hasFound();
}

} // namespace regina

