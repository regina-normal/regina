#ifndef __REGINA_LINK_BUILDER_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_LINK_BUILDER_IMPL_H
#endif

#include "triangulation/detail/linkbuilder.h"
#include "triangulation/detail/face.h"
#include "maths/perm.h"


namespace regina::detail {

template<int dim, int subdim>
void LinkBuilder<dim, subdim>::Deleter::operator() (
    const LinkTriangulation * const trig)
{
    delete trig;
}

template<int dim, int subdim>
typename LinkBuilder<dim, subdim>::UniquePtr
LinkBuilder<dim, subdim>::build(const FaceBase<dim, subdim> &face)
{
    auto ans = new LinkTriangulation;

    typename LinkTriangulation::ChangeEventSpan span(*ans);

    ans->newSimplices(face.degree());

    constexpr int nFaces = FaceNumbering<dim, subdim>::nFaces;

    // Given an FaceEmbedding embedding, we can easily construct the pair
    // (embedding.simplex()->index, embedding.face()).
    //
    // However, the code below also needs to quickly find the embedding
    // given the pair.
    //
    // simplexAndFaceToEmbeddingIndex is there to facilitate this look-up.
    // Namely, the index of the embedding is given by
    // simplexAndFaceToEmbedding[nFaces * simplexIndex + face ].
    //
    // Note that simplexAndFaceToEmbedding will have uninitialized entries
    // but we won't query them.
    std::unique_ptr<size_t[]> simplexAndFaceToEmbeddingIndex =
        std::make_unique<size_t[]>(
            nFaces * face.triangulation().size());

    {
        size_t embeddingIndex = 0;
        for (const FaceEmbedding<dim, subdim> &embedding : face) {
            const size_t i =
                nFaces * embedding.simplex()->index() + embedding.face();
            simplexAndFaceToEmbeddingIndex[i] = embeddingIndex;
            ++embeddingIndex;
        }
    }

    size_t embeddingIndex = 0;

    for (const FaceEmbedding<dim, subdim> &embedding : face) {
        Simplex<dim>* const simplex = embedding.simplex();
        // Simplex in link corresponding to this embedding.
        Simplex<dim - subdim - 1> * const linkSimplex =
            ans->simplex(embeddingIndex);
        // Permutation such that the first subdim + 1 entries
        // correspond to the vertices of the simplex spanning
        // this face.
        //
        // The remaining dim - subdim - 1 vertices span the
        // "co-face" that forms part of the link.
        //
        // For visualization, think of the link of a vertex V
        // with dim = 3, sudim = 0.
        // vertices[0] is the vertex of this tetrahedron corresponding to
        // V.
        // vertices[1], vertices[2], vertices[3] form the triangle
        // (co-face) that is part of the link.
        const Perm<dim + 1> &vertices = embedding.vertices();

        // Consider each subface f of the "co-face". We need to
        // glue it to another subface of a "co-face".
        //
        // In the above example, we need to consider each edge f of the
        // triangle and glue it to another edge of a triangle.
        for (size_t f = 0; f < dim - subdim; ++f) {

            // The gluing has already happened.
            // 
            // In the above example, it is because we already went
            // through the neighboring triangle and glued it to this
            // triangle along the current edge.
            if (linkSimplex->adjacentSimplex(f))
                continue;

            // Find the facet (dim - 1 face) corresponding to the
            // subface f of the "co-face".
            //
            // In the above example, we are looking for the face of
            // the tetrahedron that is spanned by vertices[0]
            // (corresponding to V) and the edge f we are considering.
            const int facet = vertices[subdim + 1 + f];

            // If the facet is not glued, we have nothing to glue.
            Simplex<dim>* const adjacentSimplex =
                simplex->adjacentSimplex(facet);
            if (! adjacentSimplex)
                continue;

            const Perm<dim + 1> &adjacentGluing =
                simplex->adjacentGluing(facet);

            // Compute what the "co-face" is taken to in the neighboring
            // simplex by the the gluing.
            // Note that we using here that the index of a face and of its
            // opposite face are the same in Regina.
            //
            // For the special case of subdim = 0, we thus just need to
            // determine the image of vertices[0].
            //
            // In the above example, we are looking for the triangle in the
            // neighboring tetrahedron sharing the current edge.
            //
            const int adjacentFace =
                subdim == 0
                    ? adjacentGluing[vertices[0]]
                    : FaceNumbering<dim, subdim>::faceNumber(
                        adjacentGluing * vertices);

            const size_t i =
                nFaces * adjacentSimplex->index() + adjacentFace;

            const size_t adjacentEmbeddingIndex =
                simplexAndFaceToEmbeddingIndex[i];

            // Find the embedding corresponding to the "co-face" glued to
            // the current "co-face" along the subface f.
            //
            const FaceEmbedding<dim, subdim> &adjacentEmbedding =
                face.embedding(adjacentEmbeddingIndex);

            // In the above example, we have that the triangle in the
            // neighboring tetrahedron is spanned by adjacentVertices[1],
            // adjacentVertices[2] and adjacentVertices[3].
            const Perm<dim + 1> adjacentVertices =
                adjacentEmbedding.vertices();

            // The neighboring simplex.
            Simplex<dim - subdim - 1> * const adjacentLinkSimplex =
                ans->simplex(adjacentEmbeddingIndex);

            // Glue the subface f to the other "co-face".
            linkSimplex->join(
                f,
                adjacentLinkSimplex,
                Perm<dim - subdim>::contractFront(
                            adjacentVertices.inverse() *
                            adjacentGluing *
                            vertices));
        }
        ++embeddingIndex;
    }

    return UniquePtr(ans);
}

}

#endif
