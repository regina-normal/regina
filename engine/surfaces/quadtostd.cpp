
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

#include "regina-config.h"
#include "enumerate/ndoubledescriptor.h"
#include "maths/nfastray.h"
#include "maths/nmatrixint.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsquad.h"
#include "surfaces/nsstandard.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "utilities/nbitmask.h"
#include <iterator>
#include <vector>

namespace regina {

/**
 * Put helper classes and constants into an anonymous namespace.
 */
namespace {
    /**
     * How many progress "steps" shall we declare a quad-to-standard
     * conversion is worth?
     */
    const unsigned PROGRESS_CONVERSION_STEPS = 1;

    /**
     * A back insertion iterator that defines \a value_type, which is
     * required by NDoubleDescriptor::enumerate().
     *
     * The standard back_insert_iterator does not provide this typedef,
     * so we subclass and provide the typedef ourselves.
     */
    class VectorInserter : public std::back_insert_iterator<
            std::vector<NNormalSurfaceVector*> > {
        public:
            typedef NNormalSurfaceVector* value_type;
            VectorInserter(std::vector<NNormalSurfaceVector*>& v) :
                    std::back_insert_iterator<
                        std::vector<NNormalSurfaceVector*> >(v) {
            }
    };

    /**
     * A helper class for converting between quad and standard solution
     * sets, describing a single ray (which is typically a vertex in
     * some partial solution space).
     *
     * This class derives from NFastRay, which stores the coordinates of
     * the ray itself in standard (tri-quad) coordinates.  This RaySpec
     * class also stores a bitmask indicating which of these coordinates are
     * set to zero.
     *
     * The \a BitmaskType template argument describes how the bitmask of
     * zero coordinates will be stored.  The <i>i</i>th coordinate position
     * corresponds to the <i>i</i>th bit in the bitmask, and each bit is set
     * to \c true if and only if the corresponding coordinate is zero.
     *
     * Since this class is used heavily, faster bitmask types such as
     * NBitmask1 and NBitmask2 are preferred; however, if the number
     * of coordinates is too large then the slower general-use NBitmask
     * class will need to be used instead.
     *
     * \pre The template argument \a BitmaskType is one of Regina's
     * bitmask types, such as NBitmask, NBitmask1 or NBitmask2.
     */
    template <class BitmaskType>
    class RaySpec : private NFastRay {
        private:
            BitmaskType facets_;
                /**< A bitmask listing which coordinates of this ray are
                     currently set to zero. */

        public:
            /**
             * Creates a new ray whose coordinates are a clone of the
             * given vector.
             *
             * @param v the vector to clone.
             */
            RaySpec(const NNormalSurfaceVectorStandard* v) :
                    NFastRay(v->size()), facets_(v->size()) {
                // Note that the vector is initialised to zero since
                // this is what NLargeInteger's default constructor does.
                for (unsigned i = 0; i < v->size(); ++i)
                    if ((elements[i] = (*v)[i]) == zero)
                        facets_.set(i, true);
            }

            /**
             * Creates a new ray that represents the \e negative of
             * the link of the given vertex.
             *
             * @param tri the underlying triangulation.
             * @param whichLink the index of the vertex whose link
             * we should negate; this must be strictly less than
             * <tt>tri->getNumberOfVertices()</tt>.
             */
            RaySpec(const NTriangulation* tri, unsigned whichLink) :
                    NFastRay(7 * tri->getNumberOfTetrahedra()),
                    facets_(7 * tri->getNumberOfTetrahedra()) {
                // Note that the vector is initialised to zero since
                // this is what NLargeInteger's default constructor does.
                for (unsigned i = 0; i < size(); ++i)
                    if (i % 7 > 3)
                        facets_.set(i, true);
                    else if (tri->getTetrahedron(i / 7)->getVertex(i % 7)->
                            markedIndex() == whichLink)
                        elements[i] = -1;
                    else
                        facets_.set(i, true);
            }

            /**
             * Creates a new ray, describing where the plane between the
             * two given rays meets the given axis hyperplane.  Here
             * "the given axis hyperplane" means the hyperplane along which
             * the <i>coord</i>th coordinate is zero.
             *
             * \pre The <i>coord</i>th coordinates of \a pos and \a neg
             * are strictly positive and negative respectively.
             *
             * @param pos the first of the given rays, in which the given
             * coordinate is positive.
             * @param neg the second of the given rays, in which the given
             * coordinate is negative.
             * @param coord the index of the coordinate that we must set
             * to zero to form the intersecting hyperplane.
             */
            RaySpec(const RaySpec& pos, const RaySpec& neg, unsigned coord) :
                    NFastRay(pos.size()), facets_(pos.facets_) {
                facets_ &= neg.facets_;

                // Note that we may need to re-enable some bits in \a facets_,
                // since we may end up setting some triangle coordinates
                // to zero that were not zero in either \a pos or \a neg.

                NLargeInteger posDiff = pos[coord];
                NLargeInteger negDiff = neg[coord];

                for (unsigned i = 0; i < size(); ++i)
                    if ((elements[i] = neg[i] * posDiff - pos[i] * negDiff)
                            == zero)
                        facets_.set(i, true);

                scaleDown();
            }

            /**
             * Returns the bitmask listing which coordinates of this ray
             * are currently set to zero.  See the class notes for details.
             *
             * The length of this bitmask is the same as the length of the
             * underlying vector for this ray.
             *
             * @return the bitmask of zero coordinates.
             */
            inline const BitmaskType& facets() const {
                return facets_;
            }

            /**
             * Determines whether this ray has zero coordinates in every
             * position where \e both of the given rays simultaneously
             * have zero coordinates.
             *
             * The bitmask \a ignoreFacets represents a list of coordinate
             * positions that should be ignored for the purposes of this
             * routine.
             *
             * @param x the first of the two given rays to examine.
             * @param y the second of the two given rays to examine.
             * @param ignoreFacets a bitmask of coordinate positions to
             * ignore.
             * @return \c false if there is some coordinate position
             * where (i) both \a x and \a y are zero, (ii) this vector
             * is not zero, and (iii) the corresponding bit in \a ignoreFacets
             * is not set (i.e., is \c false).  Returns \c true otherwise.
             */
            inline bool onAllCommonFacets(const RaySpec& x, const RaySpec& y,
                    BitmaskType ignoreFacets) const {
                ignoreFacets |= facets_;
                return ignoreFacets.containsIntn(x.facets_, y.facets_);
            }

            /**
             * Reduces the underlying vector by subtracting as many copies
             * of the given vertex link as possible, without allowing any of
             * the corresponding coordinates in this ray to become negative.
             *
             * \pre None of the coordinates in this ray that correspond
             * to discs in the given vertex link are already negative.
             *
             * @param link the vertex link to subtract copies of.
             */
            void reduce(const RaySpec& link) {
                if (! (facets_ <= link.facets_))
                    return;

                NLargeInteger max = NLargeInteger::infinity;
                unsigned i;
                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i))
                        if (max > elements[i])
                            max = elements[i];

                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i))
                        if ((elements[i] -= max) == zero)
                            facets_.set(i, true);
            }

            /**
             * Returns a new normal surface whose standard (tri-quad)
             * coordinates are described by this vector.  The
             * corresponding normal surface vector will be of type
             * NNormalSurfaceVectorStandard.
             *
             * @param tri the underlying triangulation.
             * @return a newly created normal surface based on this vector.
             */
            NNormalSurface* recover(NTriangulation* tri) const {
                NNormalSurfaceVectorStandard* v =
                    new NNormalSurfaceVectorStandard(size());

                for (unsigned i = 0; i < size(); ++i)
                    v->setElement(i, elements[i]);

                return new NNormalSurface(tri, v);
            }

            /**
             * Returns the sign of the given element of this vector.
             *
             * @return 1, 0 or -1 according to whether the <i>index</i>th
             * element of this vector is positive, zero or negative
             * respectively.
             */
            inline int sign(unsigned index) const {
                if (facets_.get(index))
                    return 0;
                return (elements[index] > zero ? 1 : -1);
            }
    };
} // anonymous namespace

NNormalSurfaceList* NNormalSurfaceList::quadToStandard() const {
    NTriangulation* owner = getTriangulation();

    // Basic sanity checks:
    if (flavour != NNormalSurfaceList::QUAD || ! embedded)
        return 0;
    if (owner->isIdeal() || ! owner->isValid())
        return 0;

    // Prepare a final surface list.
    NNormalSurfaceList* ans = new NNormalSurfaceList(
        NNormalSurfaceList::STANDARD, true);

    // Get the empty triangulation out of the way now.
    if (owner->getNumberOfTetrahedra() == 0) {
        owner->insertChildLast(ans);
        return ans;
    }

    // Build a vector of quad vectors to pass to our internal conversion
    // routine.
    // We will need to collect non-const pointers to vectors in this list,
    // but this is okay (the internal conversion routine guarantees not
    // to modify or delete them).
    std::vector<NNormalSurfaceVector*> quadVertices;
    quadVertices.reserve(surfaces.size());

    std::vector<NNormalSurface*>::const_iterator it;
    for (it = surfaces.begin(); it != surfaces.end(); ++it)
        quadVertices.push_back(const_cast<NNormalSurfaceVector*>(
            (*it)->rawVector()));

    ans->buildStandardFromQuad(owner, quadVertices);

    // All done!
    owner->insertChildLast(ans);
    return ans;
}

void NNormalSurfaceList::enumerateStandardViaQuad(
        NTriangulation* owner, NProgressNumber* progress) {
    // Hum.  What to do with progress?
    // For now we shall treat quad surface enumeration as the main task,
    // and assign an arbitrarily chosen fixed number of "steps" for the
    // final quad-to-standard conversion.
    if (progress)
        progress->setOutOf(
            progress->getOutOf() + PROGRESS_CONVERSION_STEPS + 1);

    // Get the empty triangulation out of the way now.
    if (owner->getNumberOfTetrahedra() == 0) {
        if (progress)
            progress->incCompleted(PROGRESS_CONVERSION_STEPS + 1);
        return;
    }

    // First enumerate all quad vertex surfaces.
    NEnumConstraintList* constraints = NNormalSurfaceVectorQuad::
        makeEmbeddedConstraints(owner);
    NMatrixInt* eqns = makeMatchingEquations(owner, NNormalSurfaceList::QUAD);
    NNormalSurfaceVector* base = makeZeroVector(owner,
        NNormalSurfaceList::QUAD);

    std::vector<NNormalSurfaceVector*> quadVertices;
    NDoubleDescriptor::enumerateExtremalRays(
        VectorInserter(quadVertices),
        *base, *eqns, constraints, progress);

    delete base;
    delete eqns;
    delete constraints;

    if (progress) {
        progress->incCompleted();

        // Check for cancellation before we go any further.
        if (progress->isCancelled())
            return;
    }

    // Feed the quad vertex surfaces through the quad-to-standard
    // conversion procedure:
    buildStandardFromQuad(owner, quadVertices);

    // Delete the old quad vertex surfaces and we're done!
    std::vector<NNormalSurfaceVector*>::const_iterator qit;
    for (qit = quadVertices.begin(); qit != quadVertices.end(); ++qit)
        delete *qit;

    if (progress)
        progress->incCompleted(PROGRESS_CONVERSION_STEPS);
}

void NNormalSurfaceList::buildStandardFromQuad(NTriangulation* owner,
        const std::vector<NNormalSurfaceVector*>& quadList) {
    unsigned nFacets = 7 * owner->getNumberOfTetrahedra();

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real conversion routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        buildStandardFromQuadUsing<NBitmask1<unsigned> >(owner, quadList);
    else if (nFacets <= 8 * sizeof(unsigned long))
        buildStandardFromQuadUsing<NBitmask1<unsigned long> >(owner, quadList);
#ifdef HAVE_LONG_LONG
    else if (nFacets <= 8 * sizeof(unsigned long long))
        buildStandardFromQuadUsing<NBitmask1<unsigned long long> >(
            owner, quadList);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        buildStandardFromQuadUsing<NBitmask2<unsigned long long, unsigned> >(
            owner, quadList);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        buildStandardFromQuadUsing<NBitmask2<unsigned long long,
            unsigned long> >(owner, quadList);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        buildStandardFromQuadUsing<NBitmask2<unsigned long long> >(
            owner, quadList);
#else
    else if (nFacets <= 8 * sizeof(unsigned long) + 8 * sizeof(unsigned))
        buildStandardFromQuadUsing<NBitmask2<unsigned long, unsigned> >(
            owner, quadList);
    else if (nFacets <= 16 * sizeof(unsigned long))
        buildStandardFromQuadUsing<NBitmask2<unsigned long> >(owner, quadList);
#endif
    else
        buildStandardFromQuadUsing<NBitmask>(owner, quadList);
}

template <class BitmaskType>
void NNormalSurfaceList::buildStandardFromQuadUsing(NTriangulation* owner,
        const std::vector<NNormalSurfaceVector*>& quadList) {
    // Prepare for the quad-to-standard double description run.
    unsigned n = owner->getNumberOfTetrahedra();
    unsigned slen = 7 * n;
    unsigned llen = owner->getNumberOfVertices();

    unsigned i;

    // Recreate the quad constraints as bitmasks:
    // Since we have a non-empty triangulation, we know the list of
    // constraints is non-empty.
    NEnumConstraintList* constraints =
        NNormalSurfaceVectorStandard::makeEmbeddedConstraints(owner);

    BitmaskType* constraintsBegin = new BitmaskType[constraints->size()];
    BitmaskType* constraintsEnd = constraintsBegin;

    NEnumConstraintList::const_iterator cit;
    for (NEnumConstraintList::const_iterator cit = constraints->begin();
            cit != constraints->end(); ++cit, ++constraintsEnd) {
        constraintsEnd->reset(slen);
        constraintsEnd->set(cit->begin(), cit->end(), true);
    }

    delete constraints;

    // Create all vertex links.
    NNormalSurfaceVectorStandard** link =
        new NNormalSurfaceVectorStandard*[llen];

    for (i = 0; i < llen; ++i) {
        link[i] = new NNormalSurfaceVectorStandard(slen);

        const std::vector<NVertexEmbedding>& emb = owner->getVertex(i)->
            getEmbeddings();
        std::vector<NVertexEmbedding>::const_iterator embit;
        for (embit = emb.begin(); embit != emb.end(); ++embit)
            link[i]->setElement(7 * embit->getTetrahedron()->markedIndex() +
                embit->getVertex(), 1);
    }

    // Create the initial set of rays:
    typedef std::vector<RaySpec<BitmaskType>*> RaySpecList;
    RaySpecList list[2];

    NNormalSurfaceVector* v;
    std::vector<NNormalSurfaceVector*>::const_iterator qit;
    for (qit = quadList.begin(); qit != quadList.end(); ++qit) {
        v = static_cast<const NNormalSurfaceVectorQuad*>(*qit)->
            makeMirror(owner);
        list[0].push_back(new RaySpec<BitmaskType>(
            static_cast<NNormalSurfaceVectorStandard*>(v)));
        delete v;
    }

    // Each additional inequality is of the form tri_coord >= 0.
    // We will therefore just create them on the fly as we need them.

    // And run!
    BitmaskType ignoreFacets(slen);
    for (i = 0; i < slen; ++i)
        if (i % 7 < 4)
            ignoreFacets.set(i, true);

    int workingList = 0;

    unsigned vtx;
    unsigned tcoord;
    RaySpec<BitmaskType>* linkSpec;

    RaySpecList pos, neg;
    typename RaySpecList::iterator it, posit, negit;

    int sign;
    BitmaskType* constraintMask;
    bool broken;

    for (vtx = 0; vtx < llen; ++vtx) {
        linkSpec = new RaySpec<BitmaskType>(link[vtx]);
        delete link[vtx];

        list[workingList].push_back(new RaySpec<BitmaskType>(owner, vtx));

        const std::vector<NVertexEmbedding>& emb = owner->getVertex(vtx)->
            getEmbeddings();
        std::vector<NVertexEmbedding>::const_iterator embit;
        for (embit = emb.begin(); embit != emb.end(); ++embit) {
            tcoord = 7 * embit->getTetrahedron()->markedIndex() +
                embit->getVertex();

            // Add the inequality v[tcoord] >= 0.
            for (it = list[workingList].begin(); it != list[workingList].end();
                    ++it) {
                sign = (*it)->sign(tcoord);

                if (sign == 0)
                    list[1 - workingList].push_back(*it);
                else if (sign > 0) {
                    list[1 - workingList].push_back(*it);
                    pos.push_back(*it);
                } else
                    neg.push_back(*it);
            }

            for (posit = pos.begin(); posit != pos.end(); ++posit)
                for (negit = neg.begin(); negit != neg.end(); ++negit) {
                    // Find the facets that both rays have in common.
                    BitmaskType join((*posit)->facets());
                    join &= ((*negit)->facets());

                    // Fukuda and Prodon's dimensional filtering.
                    // Initial experimentation suggests that this
                    // is not helpful (perhaps because of the extremely
                    // nice structure of this particular enumeration problem
                    // and the consequential way in which one solution set
                    // expands to the next).  Comment it out for now.
                    /*
                    BitmaskType tmpMask(ignoreFacets);
                    tmpMask.flip();
                    tmpMask &= join;
                    if (tmpMask.bits() < 2 * n + vtx - 1)
                        continue;
                    */

                    // Are these vectors compatible?
                    // Invert join so that it has a true bit for each
                    // non-zero coordinate.
                    join.flip();
                    broken = false;
                    for (constraintMask = constraintsBegin;
                            constraintMask != constraintsEnd;
                            ++constraintMask) {
                        BitmaskType mask(join);
                        mask &= *constraintMask;
                        if (! mask.atMostOneBit()) {
                            broken = true;
                            break;
                        }
                    }
                    if (broken)
                        continue;

                    // Are these vectors adjacent?
                    broken = false;
                    for (it = list[workingList].begin();
                            it != list[workingList].end(); ++it) {
                        if (*it != *posit && *it != *negit &&
                                (*it)->onAllCommonFacets(**posit, **negit,
                                ignoreFacets)) {
                            broken = true;
                            break;
                        }
                    }
                    if (broken)
                        continue;

                    // All good!  Join them and put the intersection in the
                    // new solution set.
                    list[1 - workingList].push_back(new RaySpec<BitmaskType>(
                        **posit, **negit, tcoord));
                }

            // Clean up and prepare for the next iteration.
            for (negit = neg.begin(); negit != neg.end(); ++negit)
                delete *negit;

            pos.clear();
            neg.clear();
            list[workingList].clear();

            ignoreFacets.set(tcoord, false);

            workingList = 1 - workingList;
        }

        // We're done cancelling this vertex link.
        // Now add the vertex link itself, and cancel any future vertex
        // links that we might have created.
        list[workingList].push_back(linkSpec);

        for (it = list[workingList].begin(); it != list[workingList].end();
                ++it)
            for (i = vtx + 1; i < llen; ++i)
                (*it)->reduce(link[i]);
    }

    // All done!  Put the solutions into the normal surface list and clean up.
    for (typename RaySpecList::iterator it = list[workingList].begin();
            it != list[workingList].end(); ++it) {
        surfaces.push_back((*it)->recover(owner));
        delete *it;
    }

    delete[] link;
    delete[] constraintsBegin;
}

} // namespace regina

