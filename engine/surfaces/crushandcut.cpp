
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <algorithm>
#include "surfaces/nnormalsurface.h"
#include "surfaces/nprism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    class Bdry;

    class Block {
        protected:
            NTetrahedron* outerTet_;
            NTetrahedron** innerTet_; /* will always have enough room for
                                         boundary layerings */
            unsigned nInnerTet_;
            Bdry* bdry_[4]; /* indexed by face */

            // TODO: Corners!

        public:
            virtual ~Block();

            NTetrahedron* outerTet();

            void joinTo(int face, Block* other);
            void insertInto(NTriangulation* tri);
            NTetrahedron* layeringTetrahedron(); /* PRE: enough space,
                                                    will layer on block bdry */

        protected:
            Block(NTetrahedron* outerTet, unsigned initialNumTet,
                unsigned maxLayerings);
    };

    class TriPrism : public Block {
        public:
            TriPrism(NTetrahedron *outerTet, int type);
                /* which tri type bounds it? */
    };

    class QuadPrism : public Block {
        public:
            QuadPrism(NTetrahedron *outerTet, int type);
                /* which quad type bounds it? */
    };

    class TruncHalfTet : public Block {
        public:
            TruncHalfTet(NTetrahedron *outerTet, int type);
                /* which edge does it miss entirely? */
    };

    class TruncTet : public Block {
        public:
            TruncTet(NTetrahedron *outerTet);
    };

    class Bdry {
        protected:
            Block* block_;
            NPerm outerVertices_; /* maps (0,1,2) of boundary to
                                     vertices of block_->outerTet_. */

        public:
            virtual ~Bdry();
            virtual void joinTo(Bdry* other) = 0; /* PRE: other is same shape */

        protected:
            Bdry(Block* block, NPerm outerVertices);
    };

    class BdryQuad : public Bdry {
        private:
            NTetrahedron* innerTet_[2]; /* tetrahedra providing bdry faces */
            NPerm innerVertices_[2]; /* maps (0,1,2) of bdry face i to vertices
                                        of innerTet_[i]. */

        public:
            virtual void joinTo(Bdry* other);

        private:
            BdryQuad(Block* block, NPerm outerVertices);
            void reflect();

        friend class TriPrism;
        friend class QuadPrism;
        friend class TruncHalfTet;
    };

    class BdryHex : public Bdry {
        private:
            NTetrahedron* innerTet_[4]; /* tetrahedra providing bdry faces */
            NPerm innerVertices_[4]; /* maps (0,1,2) of bdry face i to vertices
                                        of innerTet_[i]. */

        public:
            virtual void joinTo(Bdry* other);

        private:
            BdryHex(Block* block, NPerm outerVertices);
            void reflect();
            void rotate();

        friend class TruncHalfTet;
        friend class TruncTet;
    };

    class TetBlockSet {
        private:
            unsigned long triCount_[4]; /* indexed by vertex */
            unsigned long quadCount_;
            int quadType_; /* -1 if no quads. */

            Block** triPrism_[4]; /* indexed by vertex; counting outwards towards
                                     the centre */
            Block** quadPrism_; /* counting away from vertex 0 */
            Block* truncHalfTet_[2]; /* counting away from vertex 0 */
            Block* truncTet_;

        public:
            TetBlockSet(const NNormalSurface* s, unsigned long tetIndex);
            ~TetBlockSet();

            unsigned long numQuadBlocks(int face, int fromVertex);
            Block* quadBlock(int fromVertex, unsigned long whichBlock);
            Block* hexBlock(int face);

            void insertInto(NTriangulation* tri);
    };

    inline Block::~Block() {
        for (unsigned i = 0; i < 4; ++i)
            delete bdry_[i];
    }

    inline NTetrahedron* Block::outerTet() {
        return outerTet_;
    }

    inline void Block::joinTo(int face, Block* other) {
        bdry_[face]->joinTo(other->bdry_[outerTet_->adjacentFace(face)]);
    }

    inline void Block::insertInto(NTriangulation* tri) {
        for (unsigned i = 0; i < nInnerTet_; ++i)
            tri->addTetrahedron(innerTet_[i]);
    }

    inline NTetrahedron* Block::layeringTetrahedron() {
        return (innerTet_[nInnerTet_++] = new NTetrahedron());
    }

    inline Block::Block(NTetrahedron *outerTet, unsigned initialNumTet,
            unsigned maxLayerings) :
            outerTet_(outerTet),
            innerTet_(new NTetrahedron*[initialNumTet + maxLayerings]),
            nInnerTet_(initialNumTet) {
        unsigned i;
        for (i = 0; i < nInnerTet_; ++i)
            innerTet_[i] = new NTetrahedron();
        std::fill(bdry_, bdry_ + 4, static_cast<Bdry*>(0));
    }

    TriPrism::TriPrism(NTetrahedron *outerTet, int type) :
            Block(outerTet, 3, 3) {
        innerTet_[1]->joinTo(1, innerTet_[0], NPerm());
        innerTet_[1]->joinTo(3, innerTet_[2], NPerm());

        NPerm vertices = NPerm(0, type);

        BdryQuad* q;

        bdry_[vertices[0]] = 0;

        q = new BdryQuad(this, vertices * NPerm(0, 2, 3, 1));
        q->innerTet_[0] = innerTet_[1];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = NPerm(2, 3, 1, 0);
        q->innerVertices_[1] = NPerm(1, 3, 2, 0);
        bdry_[vertices[1]] = q;

        q = new BdryQuad(this, vertices * NPerm(2, 3));
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = NPerm(2, 1, 0, 3);
        q->innerVertices_[1] = NPerm(0, 3, 2, 1);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices);
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[1];
        q->innerVertices_[0] = NPerm(3, 1, 0, 2);
        q->innerVertices_[1] = NPerm(0, 1, 3, 2);
        bdry_[vertices[3]] = q;
    }

    QuadPrism::QuadPrism(NTetrahedron *outerTet, int type) :
            Block(outerTet, 5, 4) {
        innerTet_[4]->joinTo(2, innerTet_[0], NPerm());
        innerTet_[4]->joinTo(3, innerTet_[1], NPerm());
        innerTet_[4]->joinTo(0, innerTet_[2], NPerm());
        innerTet_[4]->joinTo(1, innerTet_[3], NPerm());

        NPerm vertices(
            regina::vertexSplitDefn[type][0],
            regina::vertexSplitDefn[type][2],
            regina::vertexSplitDefn[type][1],
            regina::vertexSplitDefn[type][3]);

        BdryQuad* q;

        q = new BdryQuad(this, vertices * NPerm(2, 3, 1, 0));
        q->innerTet_[0] = innerTet_[2];
        q->innerTet_[1] = innerTet_[1];
        q->innerVertices_[0] = NPerm(1, 0, 2, 3);
        q->innerVertices_[1] = NPerm(2, 3, 1, 0);
        bdry_[vertices[0]] = q;

        q = new BdryQuad(this, vertices * NPerm(3, 0, 2, 1));
        q->innerTet_[0] = innerTet_[3];
        q->innerTet_[1] = innerTet_[2];
        q->innerVertices_[0] = NPerm(2, 1, 3, 0);
        q->innerVertices_[1] = NPerm(3, 0, 2, 1);
        bdry_[vertices[1]] = q;

        q = new BdryQuad(this, vertices * NPerm(0, 1, 3, 2));
        q->innerTet_[0] = innerTet_[0];
        q->innerTet_[1] = innerTet_[3];
        q->innerVertices_[0] = NPerm(3, 2, 0, 1);
        q->innerVertices_[1] = NPerm(0, 1, 3, 2);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices * NPerm(1, 2, 0, 3));
        q->innerTet_[0] = innerTet_[1];
        q->innerTet_[1] = innerTet_[0];
        q->innerVertices_[0] = NPerm(0, 3, 1, 2);
        q->innerVertices_[1] = NPerm(1, 2, 0, 3);
        bdry_[vertices[3]] = q;
    }

    TruncHalfTet::TruncHalfTet(NTetrahedron *outerTet, int type) :
            Block(outerTet, 8, 10) {
        innerTet_[1]->joinTo(2, innerTet_[0], NPerm());
        innerTet_[1]->joinTo(1, innerTet_[2], NPerm());
        innerTet_[1]->joinTo(0, innerTet_[3], NPerm());
        innerTet_[2]->joinTo(0, innerTet_[4], NPerm());
        innerTet_[3]->joinTo(1, innerTet_[4], NPerm());
        innerTet_[3]->joinTo(3, innerTet_[5], NPerm());
        innerTet_[5]->joinTo(2, innerTet_[6], NPerm());
        innerTet_[4]->joinTo(2, innerTet_[7], NPerm());

        NPerm vertices(
            NEdge::edgeVertex[type][0],
            NEdge::edgeVertex[type][1],
            NEdge::edgeVertex[5 - type][0],
            NEdge::edgeVertex[5 - type][1]);

        BdryQuad* q;
        BdryHex* h;

        h = new BdryHex(this, vertices * NPerm(1, 3, 2, 0));
        h->innerTet_[0] = innerTet_[2];
        h->innerTet_[1] = innerTet_[7];
        h->innerTet_[2] = innerTet_[5];
        h->innerTet_[3] = innerTet_[4];
        h->innerVertices_[0] = NPerm(2, 0, 1, 3);
        h->innerVertices_[1] = NPerm(1, 2, 0, 3);
        h->innerVertices_[2] = NPerm(0, 3, 2, 1);
        h->innerVertices_[3] = NPerm(0, 2, 1, 3);
        bdry_[vertices[0]] = h;

        h = new BdryHex(this, vertices * NPerm(0, 3, 2, 1));
        h->innerTet_[0] = innerTet_[0];
        h->innerTet_[1] = innerTet_[7];
        h->innerTet_[2] = innerTet_[6];
        h->innerTet_[3] = innerTet_[3];
        h->innerVertices_[0] = NPerm(1, 2, 3, 0);
        h->innerVertices_[1] = NPerm(3, 2, 0, 1);
        h->innerVertices_[2] = NPerm(0, 2, 1, 3);
        h->innerVertices_[3] = NPerm(0, 1, 3, 2);
        bdry_[vertices[1]] = h;

        q = new BdryQuad(this, vertices * NPerm(3, 1, 0, 2));
        q->innerTet_[0] = innerTet_[2];
        q->innerTet_[1] = innerTet_[0];
        q->innerVertices_[0] = NPerm(3, 1, 0, 2);
        q->innerVertices_[1] = NPerm(0, 2, 3, 1);
        bdry_[vertices[2]] = q;

        q = new BdryQuad(this, vertices * NPerm(2, 0, 1, 3));
        q->innerTet_[0] = innerTet_[6];
        q->innerTet_[1] = innerTet_[5];
        q->innerVertices_[0] = NPerm(3, 2, 1, 0);
        q->innerVertices_[1] = NPerm(1, 2, 3, 0);
        bdry_[vertices[3]] = q;
    }

    TruncTet::TruncTet(NTetrahedron *outerTet) :
            Block(outerTet, 11, 16) {
        innerTet_[0]->joinTo(2, innerTet_[4], NPerm());
        innerTet_[1]->joinTo(3, innerTet_[7], NPerm());
        innerTet_[2]->joinTo(0, innerTet_[6], NPerm());
        innerTet_[3]->joinTo(1, innerTet_[9], NPerm());
        innerTet_[5]->joinTo(3, innerTet_[4], NPerm());
        innerTet_[5]->joinTo(1, innerTet_[6], NPerm());
        innerTet_[8]->joinTo(0, innerTet_[7], NPerm());
        innerTet_[8]->joinTo(2, innerTet_[9], NPerm());
        innerTet_[4]->joinTo(1, innerTet_[10], NPerm());
        innerTet_[6]->joinTo(3, innerTet_[10], NPerm());
        innerTet_[7]->joinTo(2, innerTet_[10], NPerm());
        innerTet_[9]->joinTo(0, innerTet_[10], NPerm());

        BdryHex* h;

        h = new BdryHex(this, NPerm(2, 1, 3, 0));
        h->innerTet_[0] = innerTet_[2];
        h->innerTet_[1] = innerTet_[8];
        h->innerTet_[2] = innerTet_[3];
        h->innerTet_[3] = innerTet_[9];
        h->innerVertices_[0] = NPerm(2, 0, 1, 3);
        h->innerVertices_[1] = NPerm(1, 2, 0, 3);
        h->innerVertices_[2] = NPerm(0, 1, 2, 3);
        h->innerVertices_[3] = NPerm(0, 2, 1, 3);
        bdry_[0] = h;

        h = new BdryHex(this, NPerm(3, 2, 0, 1));
        h->innerTet_[0] = innerTet_[3];
        h->innerTet_[1] = innerTet_[5];
        h->innerTet_[2] = innerTet_[0];
        h->innerTet_[3] = innerTet_[4];
        h->innerVertices_[0] = NPerm(3, 1, 2, 0);
        h->innerVertices_[1] = NPerm(2, 3, 1, 0);
        h->innerVertices_[2] = NPerm(1, 2, 3, 0);
        h->innerVertices_[3] = NPerm(1, 3, 2, 0);
        bdry_[1] = h;

        h = new BdryHex(this, NPerm(0, 3, 1, 2));
        h->innerTet_[0] = innerTet_[0];
        h->innerTet_[1] = innerTet_[8];
        h->innerTet_[2] = innerTet_[1];
        h->innerTet_[3] = innerTet_[7];
        h->innerVertices_[0] = NPerm(0, 2, 3, 1);
        h->innerVertices_[1] = NPerm(3, 0, 2, 1);
        h->innerVertices_[2] = NPerm(2, 3, 0, 1);
        h->innerVertices_[3] = NPerm(2, 0, 3, 1);
        bdry_[2] = h;

        h = new BdryHex(this, NPerm(1, 0, 2, 3));
        h->innerTet_[0] = innerTet_[1];
        h->innerTet_[1] = innerTet_[5];
        h->innerTet_[2] = innerTet_[2];
        h->innerTet_[3] = innerTet_[6];
        h->innerVertices_[0] = NPerm(1, 3, 0, 2);
        h->innerVertices_[1] = NPerm(0, 1, 3, 2);
        h->innerVertices_[2] = NPerm(3, 0, 1, 2);
        h->innerVertices_[3] = NPerm(3, 1, 0, 2);
        bdry_[3] = h;
    }

    inline Bdry::~Bdry() {
        // Empty virtual destructor.
    }

    inline Bdry::Bdry(Block* block, NPerm outerVertices) :
            block_(block), outerVertices_(outerVertices) {
    }

    void BdryQuad::joinTo(Bdry* other) {
        // Assume other is a BdryQuad.
        BdryQuad* dest = static_cast<BdryQuad*>(other);

        // Get the map from *this* 012 to *dest* tetrahedron vertices.
        NPerm destMap = block_->outerTet()->
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
            innerTet_[i]->joinTo(innerVertices_[i][3], dest->innerTet_[i],
                dest->innerVertices_[i] * innerVertices_[i].inverse());
    }

    inline BdryQuad::BdryQuad(Block* block, NPerm outerVertices) :
            Bdry(block, outerVertices) {
    }

    void BdryQuad::reflect() {
        NTetrahedron* layering = block_->layeringTetrahedron();

        layering->joinTo(0, innerTet_[1],
            innerVertices_[1] * NPerm(3, 2, 1, 0));
        layering->joinTo(2, innerTet_[0],
            innerVertices_[0] * NPerm(1, 0, 3, 2));

        innerTet_[0] = innerTet_[1] = layering;
        innerVertices_[0] = NPerm();
        innerVertices_[1] = NPerm(2, 3, 0, 1);

        outerVertices_ = outerVertices_ * NPerm(1, 2);
    }

    void BdryHex::joinTo(Bdry* other) {
        // Assume other is a BdryQuad.
        BdryHex* dest = static_cast<BdryHex*>(other);

        // Get the map from *this* 012 to *dest* tetrahedron vertices.
        NPerm destMap = block_->outerTet()->
            adjacentGluing(outerVertices_[3]) * outerVertices_;

        if (destMap.sign() != dest->outerVertices_.sign())
            dest->reflect();

        while (destMap != dest->outerVertices_)
            dest->rotate();

        // Now we match up perfectly.
        for (int i = 0; i < 4; ++i)
            innerTet_[i]->joinTo(innerVertices_[i][3], dest->innerTet_[i],
                dest->innerVertices_[i] * innerVertices_[i].inverse());
    }

    inline BdryHex::BdryHex(Block* block, NPerm outerVertices) :
            Bdry(block, outerVertices) {
    }

    void BdryHex::reflect() {
        NTetrahedron* layering0 = block_->layeringTetrahedron();
        NTetrahedron* layering1 = block_->layeringTetrahedron();
        NTetrahedron* layering2 = block_->layeringTetrahedron();
        NTetrahedron* layering3 = block_->layeringTetrahedron();

        layering0->joinTo(1, innerTet_[3], innerVertices_[3] * NPerm(1, 3));
        layering0->joinTo(2, innerTet_[2], innerVertices_[2] * NPerm(2, 3));
        layering1->joinTo(3, layering0, NPerm());
        layering1->joinTo(1, innerTet_[1],
            innerVertices_[1] * NPerm(2, 3, 0, 1));
        layering2->joinTo(0, layering0, NPerm());
        layering2->joinTo(1, innerTet_[0],
            innerVertices_[0] * NPerm(1, 3, 2, 0));
        layering3->joinTo(0, layering1, NPerm());
        layering3->joinTo(3, layering2, NPerm());

        innerTet_[0] = layering2;
        innerTet_[1] = layering1;
        innerTet_[2] = layering3;
        innerTet_[3] = layering3;

        innerVertices_[0] = NPerm(0, 3, 1, 2);
        innerVertices_[1] = NPerm(1, 0, 3, 2);
        innerVertices_[2] = NPerm(3, 2, 0, 1);
        innerVertices_[3] = NPerm(3, 0, 1, 2);

        outerVertices_ = outerVertices_ * NPerm(1, 2);
    }

    void BdryHex::rotate() {
        NTetrahedron* t = innerTet_[0];
        innerTet_[0] = innerTet_[1];
        innerTet_[1] = innerTet_[2];
        innerTet_[2] = t;

        NPerm p = innerVertices_[0];
        innerVertices_[0] = innerVertices_[1];
        innerVertices_[1] = innerVertices_[2];
        innerVertices_[2] = p;
        innerVertices_[3] = innerVertices_[3] * NPerm(1, 2, 0, 3);

        outerVertices_ = outerVertices_ * NPerm(1, 2, 0, 3);
    }

    TetBlockSet::TetBlockSet(const NNormalSurface* s, unsigned long tetIndex) {
        unsigned long i, j;
        for (i = 0; i < 4; ++i)
            triCount_[i] = s->getTriangleCoord(tetIndex, i).longValue();

        NLargeInteger coord;
        if ((coord = s->getQuadCoord(tetIndex, 0)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 0;
        } else if ((coord = s->getQuadCoord(tetIndex, 1)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 1;
        } else if ((coord = s->getQuadCoord(tetIndex, 2)) > 0) {
            quadCount_ = coord.longValue();
            quadType_ = 2;
        } else {
            quadCount_ = 0;
            quadType_ = -1;
        }

        NTetrahedron* tet = s->getTriangulation()->getTetrahedron(tetIndex);

        // Build the blocks.
        // Note in all of this that we insert an extra "fake" triangle at each
        // vertex (i.e., the entire surface gains a fake set of extra vertex links).
        for (i = 0; i < 4; ++i) {
            if (triCount_[i] == 0)
                triPrism_[i] = 0;
            else {
                triPrism_[i] = new Block*[triCount_[i]];
                for (j = 0; j < triCount_[i]; ++j)
                    triPrism_[i][j] = new TriPrism(tet, i);
            }
        }

        if (quadCount_ == 0) {
            quadPrism_ = 0;
            truncHalfTet_[0] = truncHalfTet_[1] = 0;
            truncTet_ = new TruncTet(tet);
        } else {
            if (quadCount_ > 1) {
                quadPrism_ = new Block*[quadCount_ - 1];
                for (j = 0; j < quadCount_ - 1; ++j)
                    quadPrism_[j] = new QuadPrism(tet, quadType_);
            } else
                quadPrism_ = 0;

            truncHalfTet_[0] = new TruncHalfTet(tet, 5 - quadType_);
            truncHalfTet_[1] = new TruncHalfTet(tet, quadType_);

            truncTet_ = 0;
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

        if (quadType_ == regina::vertexSplit[face][fromVertex]) {
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
                    fromVertex == NEdge::edgeVertex[quadType_][1])
                return truncHalfTet_[0];
            else
                return truncHalfTet_[1];
        }

        // Finally we have the quad prisms.
        if (fromVertex == 0 || fromVertex == NEdge::edgeVertex[quadType_][1])
            return quadPrism_[whichBlock - triCount_[fromVertex] - 1];
        else
            return quadPrism_[
                quadCount_ - (whichBlock - triCount_[fromVertex]) - 1];
    }

    Block* TetBlockSet::hexBlock(int face) {
        if (quadCount_ == 0)
            return truncTet_;

        if (face == 0 || face == NEdge::edgeVertex[quadType_][1])
            return truncHalfTet_[1];
        return truncHalfTet_[0];
    }

    void TetBlockSet::insertInto(NTriangulation* tri) {
        unsigned long i, j;
        for (i = 0; i < 4; ++i)
            if (triPrism_[i])
                for (j = 0; j < triCount_[i]; ++j)
                    triPrism_[i][j]->insertInto(tri);

        if (quadCount_ == 0)
            truncTet_->insertInto(tri);
        else {
            if (quadPrism_)
                for (j = 0; j < quadCount_ - 1; ++j)
                    quadPrism_[j]->insertInto(tri);

            truncHalfTet_[0]->insertInto(tri);
            truncHalfTet_[1]->insertInto(tri);
        }
    }
}

NTriangulation* NNormalSurface::cutAlong() const {
    NTriangulation* ans = new NTriangulation();

    unsigned long nTet = getTriangulation()->getNumberOfTetrahedra();
    if (nTet == 0)
        return ans;

    unsigned long i;
    TetBlockSet** sets = new TetBlockSet*[nTet];
    for (i = 0; i < nTet; ++i)
        sets[i] = new TetBlockSet(this, i);

    NTriangulation::FaceIterator fit;
    NFace* f;
    unsigned long tet0, tet1;
    int face0, face1;
    int fromVertex0, fromVertex1;
    unsigned long quadBlocks;
    for (fit = getTriangulation()->getFaces().begin();
            fit != getTriangulation()->getFaces().end(); ++fit) {
        f = *fit;
        if (f->isBoundary())
            continue;

        tet0 = f->getEmbedding(0).getTetrahedron()->markedIndex();
        tet1 = f->getEmbedding(1).getTetrahedron()->markedIndex();
        face0 = f->getEmbedding(0).getFace();
        face1 = f->getEmbedding(1).getFace();

        for (fromVertex0 = 0; fromVertex0 < 4; ++fromVertex0) {
            if (fromVertex0 == face0)
                continue;
            fromVertex1 = f->getEmbedding(0).getTetrahedron()->
                adjacentGluing(face0)[fromVertex0];

            quadBlocks = sets[tet0]->numQuadBlocks(face0, fromVertex0);
            for (i = 0; i < quadBlocks; ++i)
                sets[tet0]->quadBlock(fromVertex0, i)->joinTo(
                    face0, sets[tet1]->quadBlock(fromVertex1, i));
        }
        sets[tet0]->hexBlock(face0)->joinTo(face0, sets[tet1]->hexBlock(face1));
    }

    for (i = 0; i < nTet; ++i) {
        sets[i]->insertInto(ans);
        delete sets[i];
    }
    delete[] sets;

    return ans;
}

NTriangulation* NNormalSurface::crush() const {
    NTriangulation* ans = new NTriangulation(*triangulation);
    unsigned long nTet = ans->getNumberOfTetrahedra();
    if (nTet == 0)
        return new NTriangulation();

    // Work out which tetrahedra contain which quad types.
    int* quads = new int[nTet];
    long whichTet = 0;
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++) {
        if (getQuadCoord(whichTet, 0) != 0)
            quads[whichTet] = 0;
        else if (getQuadCoord(whichTet, 1) != 0)
            quads[whichTet] = 1;
        else if (getQuadCoord(whichTet, 2) != 0)
            quads[whichTet] = 2;
        else
            quads[whichTet] = -1;
    }

    // Run through and fix the tetrahedron gluings.
    NTetrahedron* tet;
    NTetrahedron* adj;
    int adjQuads;
    NPerm adjPerm;
    NPerm swap;
    int face, adjFace;
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++)
        if (quads[whichTet] == -1) {
            // We want to keep this tetrahedron, so make sure it's glued
            // up correctly.
            tet = ans->getTetrahedron(whichTet);
            for (face = 0; face < 4; face++) {
                adj = tet->adjacentTetrahedron(face);
                if (! adj)
                    continue;
                adjQuads = quads[ans->tetrahedronIndex(adj)];
                if (adjQuads == -1)
                    continue;

                // We're glued to a bad tetrahedron.  Follow around
                // until we reach a good tetrahedron or a boundary.
                adjPerm = tet->adjacentGluing(face);
                adjFace = adjPerm[face];
                while (adj && (adjQuads >= 0)) {
                    swap = NPerm(adjFace,
                        vertexSplitPartner[adjQuads][adjFace]);

                    adjFace = swap[adjFace];
                    adjPerm = adj->adjacentGluing(adjFace) *
                        swap * adjPerm;
                    adj = adj->adjacentTetrahedron(adjFace);
                    adjFace = adjPerm[face];

                    if (adj)
                        adjQuads = quads[ans->tetrahedronIndex(adj)];
                }

                // Reglue the tetrahedron face accordingly.
                tet->unjoin(face);
                if (! adj)
                    continue;

                // We haven't yet unglued the face of adj since there is
                // at least one bad tetrahedron between tet and adj.
                adj->unjoin(adjFace);
                tet->joinTo(face, adj, adjPerm);
            }
        }

    // Delete unwanted tetrahedra.
    for (whichTet = nTet - 1; whichTet >= 0; whichTet--)
        if (quads[whichTet] >= 0)
            ans->removeTetrahedronAt(whichTet);

    delete[] quads;
    return ans;
}

} // namespace regina

