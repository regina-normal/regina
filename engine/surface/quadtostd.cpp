
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "regina-config.h"
#include "enumerate/doubledescription.h"
#include "maths/matrix.h"
#include "maths/vector.h"
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/bitmask.h"
#include <iterator>
#include <vector>

namespace regina {

// Although the conversion routines are template routines, we implement
// them here in this C++ file to avoid dragging them into the headers.

/**
 * Put helper classes and constants into an anonymous namespace.
 */
namespace {
    /**
     * A helper class for converting between reduced and standard
     * solution sets, describing a single ray (which is typically a
     * vertex in some partial solution space).
     *
     * This class derives from Vector, which stores the coordinates of
     * the ray itself in standard coordinates.  This RaySpec class also
     * stores a bitmask indicating which of these coordinates are set to zero.
     *
     * The \a BitmaskType template argument describes how the bitmask of
     * zero coordinates will be stored.  The <i>i</i>th coordinate position
     * corresponds to the <i>i</i>th bit in the bitmask, and each bit is set
     * to \c true if and only if the corresponding coordinate is zero.
     *
     * Since this class is used heavily, faster bitmask types such as
     * Bitmask1 and Bitmask2 are preferred; however, if the number
     * of coordinates is too large then the slower general-use Bitmask
     * class will need to be used instead.
     *
     * \pre The template argument \a BitmaskType is one of Regina's
     * bitmask types, such as Bitmask, Bitmask1 or Bitmask2.
     */
    template <class BitmaskType>
    class RaySpec : private Vector<LargeInteger> {
        private:
            BitmaskType facets_;
                /**< A bitmask listing which coordinates of this ray are
                     currently set to zero. */

        public:
            /**
             * Creates a new ray whose coordinates are a clone of the
             * given vector.
             *
             * \param v the vector to clone.
             */
            RaySpec(const Vector<LargeInteger>& v) :
                    Vector<LargeInteger>(v.size()), facets_(v.size()) {
                // Note that the vector is initialised to zero since
                // this is what LargeInteger's default constructor does.
                for (size_t i = 0; i < v.size(); ++i)
                    if ((elts_[i] = v[i]) == 0)
                        facets_.set(i, true);
            }

            /**
             * Creates a new ray that represents the _negative_ of
             * the link of the given vertex.
             *
             * \param tri the underlying triangulation.
             * \param whichLink the index of the vertex whose link
             * we should negate; this must be strictly less than
             * `tri->countVertices()`.
             * \param coordsPerTet the number of standard coordinate
             * positions for each tetrahedron (that is, 7 if we are
             * working with normal surfaces, or 10 if we are working
             * with almost normal surfaces).
             */
            RaySpec(const Triangulation<3>& tri, unsigned long whichLink,
                    unsigned coordsPerTet) :
                    Vector<LargeInteger>(coordsPerTet * tri.size()),
                    facets_(coordsPerTet * tri.size()) {
                // Note that the vector is initialised to zero since
                // this is what LargeInteger's default constructor does.
                for (size_t i = 0; i < size(); ++i)
                    if (i % coordsPerTet > 3) {
                        // Not a triangular coordinate.
                        facets_.set(i, true);
                    } else if (tri.tetrahedron(i / coordsPerTet)->
                            vertex(i % coordsPerTet)->markedIndex()
                            == whichLink) {
                        // A triangular coordinate in our vertex link.
                        elts_[i] = -1;
                    } else {
                        // A triangular coordinate not in our vertex link.
                        facets_.set(i, true);
                    }
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
             * \param pos the first of the given rays, in which the given
             * coordinate is positive.
             * \param neg the second of the given rays, in which the given
             * coordinate is negative.
             * \param coord the index of the coordinate that we must set
             * to zero to form the intersecting hyperplane.
             */
            RaySpec(const RaySpec& pos, const RaySpec& neg, size_t coord) :
                    Vector<LargeInteger>(pos.size()), facets_(pos.facets_) {
                facets_ &= neg.facets_;

                // Note that we may need to re-enable some bits in \a facets_,
                // since we may end up setting some triangle coordinates
                // to zero that were not zero in either \a pos or \a neg.

                LargeInteger posDiff = pos[coord];
                LargeInteger negDiff = neg[coord];

                for (size_t i = 0; i < size(); ++i)
                    if ((elts_[i] = neg[i] * posDiff - pos[i] * negDiff)
                            == 0)
                        facets_.set(i, true);

                scaleDown();
            }

            /**
             * Moves the contents of the given ray into this new ray.
             */
            RaySpec(RaySpec&&) noexcept = default;

            /**
             * Moves the contents of the given ray into this ray.
             */
            RaySpec& operator = (RaySpec&&) noexcept = default;

            /**
             * Returns the bitmask listing which coordinates of this ray
             * are currently set to zero.  See the class notes for details.
             *
             * The length of this bitmask is the same as the length of the
             * underlying vector for this ray.
             *
             * \return the bitmask of zero coordinates.
             */
            inline const BitmaskType& facets() const {
                return facets_;
            }

            /**
             * Determines whether this ray has zero coordinates in every
             * position where _both_ of the given rays simultaneously
             * have zero coordinates.
             *
             * The bitmask \a ignoreFacets represents a list of coordinate
             * positions that should be ignored for the purposes of this
             * routine.
             *
             * \param x the first of the two given rays to examine.
             * \param y the second of the two given rays to examine.
             * \param ignoreFacets a bitmask of coordinate positions to
             * ignore.
             * \return \c false if there is some coordinate position
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
             * \param link the vertex link to subtract copies of.
             */
            void reduce(const RaySpec& link) {
                if (! (facets_ <= link.facets_))
                    return;

                bool start = true;
                LargeInteger max; // max we are allowed to subtract
                size_t i;
                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i)) {
                        if (start) {
                            max = elts_[i];
                            start = false;
                        } else if (max > elts_[i])
                            max = elts_[i];
                    }

                // If start == true then this next loop is harmless, since
                // link.facets_.get(i) must always be true.
                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i))
                        if ((elts_[i] -= max) == 0)
                            facets_.set(i, true);
            }

            /**
             * Returns a new normal (or almost normal) surface whose
             * coordinates are described by this vector.
             *
             * The normal coordinates will be moved out of this vector, and
             * so this object will become unsable (as indicated by the
             * rvalue reference qualifier).
             *
             * \param tri the underlying triangulation.
             * \param enc the encoding used by this vector to describe a
             * normal surface.
             * \return a normal surface based on this vector.
             */
            NormalSurface recover(const SnapshotRef<Triangulation<3>>& tri,
                    NormalEncoding enc) && {
                return NormalSurface(tri, enc, std::move(*this));
            }

            /**
             * Returns the sign of the given element of this vector.
             *
             * \return 1, 0 or -1 according to whether the <i>index</i>th
             * element of this vector is positive, zero or negative
             * respectively.
             */
            inline int sign(size_t index) const {
                if (facets_.get(index))
                    return 0;
                return (elts_[index] > 0 ? 1 : -1);
            }

            using Vector<LargeInteger>::scaleDown;
    };
} // anonymous namespace

void NormalSurfaces::buildStandardFromReduced(
        const std::vector<NormalSurface>& reducedList,
        ProgressTracker* tracker) {
    const size_t nFacets = NormalEncoding(coords_).block() *
        triangulation_->size();

    // Get the empty triangulation out of the way now.
    if (nFacets == 0)
        return;

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real conversion routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        buildStandardFromReducedUsing<
            Bitmask1<unsigned>>(reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long))
        buildStandardFromReducedUsing<
            Bitmask1<unsigned long>>(reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long))
        buildStandardFromReducedUsing<
            Bitmask1<unsigned long long>>(reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        buildStandardFromReducedUsing<
            Bitmask2<unsigned long long, unsigned>>(reducedList,
                tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        buildStandardFromReducedUsing<
            Bitmask2<unsigned long long, unsigned long>>(reducedList,
                tracker);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        buildStandardFromReducedUsing<
            Bitmask2<unsigned long long>>(reducedList, tracker);
    else
        buildStandardFromReducedUsing<Bitmask>(reducedList, tracker);
}

template <class BitmaskType>
void NormalSurfaces::buildStandardFromReducedUsing(
        const std::vector<NormalSurface>& reducedList,
        ProgressTracker* tracker) {
    const Triangulation<3>& tri(*triangulation_);

    // Prepare for the reduced-to-standard double description run.
    const NormalEncoding stdEnc(coords_);
    const size_t n = tri.size();
    const size_t stdLen = stdEnc.block() * n;
    const size_t nLinks = tri.countVertices(); // # vertex links

    // Recreate the quadrilateral constraints (or the corresponding
    // constraints for almost normal surfaces) as bitmasks.
    // Since we have a non-empty triangulation, we know the list of
    // constraints is non-empty.
    auto constraints = makeEmbeddedConstraints(tri, coords_).
        bitmasks<BitmaskType>(stdLen);

    // Create all vertex links.
    // TODO: Do this by value.
    auto* link = new Vector<LargeInteger>*[nLinks];

    for (size_t i = 0; i < nLinks; ++i) {
        link[i] = new Vector<LargeInteger>(stdLen);

        for (auto& emb : *tri.vertex(i))
            (*link[i])[stdEnc.block() * emb.tetrahedron()->markedIndex() +
                emb.vertex()] = 1;
    }

    // Create the initial set of rays:
    // TODO: Keep these by value.
    using RaySpecList = std::vector<RaySpec<BitmaskType>*>;
    RaySpecList list[2];

    // TODO: Check that s uses the right encoding.
    for (auto& s : reducedList)
        list[0].push_back(new RaySpec<BitmaskType>(s.vector()));

    // Each additional inequality is of the form tri_coord >= 0.
    // We will therefore just create them on the fly as we need them.

    // And run!
    BitmaskType ignoreFacets(stdLen);
    for (size_t i = 0; i < stdLen; ++i)
        if (i % stdEnc.block() < 4)
            ignoreFacets.set(i, true);

    int workingList = 0;

    unsigned vtx;
    size_t tcoord;
    RaySpec<BitmaskType>* linkSpec;

    RaySpecList pos, neg;

    int sign;
    bool broken;

    unsigned long slices = 0;
    unsigned iterations;
    for (vtx = 0; vtx < nLinks; ++vtx) {
        linkSpec = new RaySpec<BitmaskType>(*link[vtx]);
        delete link[vtx];

        list[workingList].push_back(
            new RaySpec<BitmaskType>(tri, vtx, stdEnc.block()));

        for (auto& emb : *tri.vertex(vtx)) {
            // Update the state of progress and test for cancellation.
            if (tracker && ! tracker->setPercent(25.0 * slices++ / n)) {
                for (auto r : list[workingList])
                    delete r;
                delete linkSpec;
                for (++vtx; vtx < nLinks; ++vtx)
                    delete link[vtx];
                delete[] link;
                return;
            }

            tcoord = stdEnc.block() * emb.tetrahedron()->markedIndex() +
                emb.vertex();

            // Add the inequality v[tcoord] >= 0.
            for (RaySpec<BitmaskType>* r : list[workingList]) {
                sign = r->sign(tcoord);

                if (sign == 0)
                    list[1 - workingList].push_back(r);
                else if (sign > 0) {
                    list[1 - workingList].push_back(r);
                    pos.push_back(r);
                } else
                    neg.push_back(r);
            }

            iterations = 0;
            for (RaySpec<BitmaskType>* posRay : pos)
                for (RaySpec<BitmaskType>* negRay : neg) {
                    // Test for cancellation, but not every time (since
                    // this involves expensive mutex locking).
                    if (tracker && ++iterations == 100) {
                        iterations = 0;
                        if (tracker->isCancelled()) {
                            for (auto r : list[1 - workingList])
                                delete r;
                            for (auto r : neg)
                                delete r;
                            delete linkSpec;
                            for (++vtx; vtx < nLinks; ++vtx)
                                delete link[vtx];
                            delete[] link;
                            return;
                        }
                    }

                    // Find the facets that both rays have in common.
                    BitmaskType join(posRay->facets());
                    join &= (negRay->facets());

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
                    for (const BitmaskType& constraint : constraints) {
                        BitmaskType mask(join);
                        mask &= constraint;
                        if (! mask.atMostOneBit()) {
                            broken = true;
                            break;
                        }
                    }
                    if (broken)
                        continue;

                    // Are these vectors adjacent?
                    broken = false;
                    for (RaySpec<BitmaskType>* r : list[workingList]) {
                        if (r != posRay && r != negRay &&
                                r->onAllCommonFacets(*posRay, *negRay,
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
                        *posRay, *negRay, tcoord));
                }

            // Clean up and prepare for the next iteration.
            for (auto ray : neg)
                delete ray;

            pos.clear();
            neg.clear();
            list[workingList].clear();

            ignoreFacets.set(tcoord, false);

            workingList = 1 - workingList;
        }

        // We're done cancelling this vertex link.
        // Now add the vertex link itself, and cancel any future vertex
        // links that we might have created.
        // Note that cancelling future vertex links might introduce
        // new common factors that can be divided out.
        list[workingList].push_back(linkSpec);

        for (auto ray : list[workingList]) {
            for (size_t i = vtx + 1; i < nLinks; ++i)
                ray->reduce(*link[i]);
            ray->scaleDown();
        }
    }

    // All done!  Put the solutions into the normal surface list and clean up.
    for (auto ray : list[workingList]) {
        surfaces_.push_back(std::move(*ray).recover(triangulation_, stdEnc));
        delete ray;
    }

    delete[] link;

    if (tracker)
        tracker->setPercent(100);
}

} // namespace regina

