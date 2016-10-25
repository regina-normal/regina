
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

#include "regina-config.h"
#include "enumerate/doubledescription.h"
#include "maths/matrix.h"
#include "maths/ray.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "surfaces/nsvectorstandard.h"
#include "surfaces/nsvectorquad.h"
#include "surfaces/nsvectoranstandard.h"
#include "surfaces/nsvectorquadoct.h"
#include "triangulation/dim3.h"
#include "utilities/bitmask.h"
#include <iterator>
#include <vector>

namespace regina {

// Although the conversion routines are template routines, we implement
// them here in this C++ file to avoid dragging them into the headers.
//
// The following definitions and declarations should ensure that the
// templates are fully instantiated where they need to be.
//
// Since the template funtcions are private, we do not need to declare
// them with REGINA_API.

NormalSurfaces* NormalSurfaces::quadToStandard() const {
    return internalReducedToStandard<NormalSpec>();
}

NormalSurfaces* NormalSurfaces::quadOctToStandardAN() const {
    return internalReducedToStandard<AlmostNormalSpec>();
}

template void NormalSurfaces::buildStandardFromReduced<
        NormalSurfaces::NormalSpec>(Triangulation<3>*,
        const std::vector<NormalSurface*>&, ProgressTracker*);
template void NormalSurfaces::buildStandardFromReduced<
        NormalSurfaces::AlmostNormalSpec>(Triangulation<3>*,
        const std::vector<NormalSurface*>&, ProgressTracker*);

/**
 * Put helper classes and constants into an anonymous namespace.
 */
namespace {
    /**
     * A back insertion iterator that defines \a value_type, which is
     * required by DoubleDescription::enumerate().
     *
     * The standard back_insert_iterator does not provide this typedef,
     * so we subclass and provide the typedef ourselves.
     */
    class VectorInserter : public std::back_insert_iterator<
            std::vector<NormalSurfaceVector*> > {
        public:
            typedef NormalSurfaceVector* value_type;
            VectorInserter(std::vector<NormalSurfaceVector*>& v) :
                    std::back_insert_iterator<
                        std::vector<NormalSurfaceVector*> >(v) {
            }
    };

    /**
     * A helper class for converting between reduced and standard
     * solution sets, describing a single ray (which is typically a
     * vertex in some partial solution space).
     *
     * This class derives from Ray, which stores the coordinates of
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
    class RaySpec : private Ray {
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
            RaySpec(const Ray& v) : Ray(v.size()), facets_(v.size()) {
                // Note that the vector is initialised to zero since
                // this is what LargeInteger's default constructor does.
                for (size_t i = 0; i < v.size(); ++i)
                    if ((elements[i] = v[i]) == zero)
                        facets_.set(i, true);
            }

            /**
             * Creates a new ray that represents the \e negative of
             * the link of the given vertex.
             *
             * @param tri the underlying triangulation.
             * @param whichLink the index of the vertex whose link
             * we should negate; this must be strictly less than
             * <tt>tri->countVertices()</tt>.
             * @param coordsPerTet the number of standard coordinate
             * positions for each tetrahedron (that is, 7 if we are
             * working with normal surfaces, or 10 if we are working
             * with almost normal surfaces).
             */
            RaySpec(const Triangulation<3>* tri, unsigned long whichLink,
                    unsigned coordsPerTet) :
                    Ray(coordsPerTet * tri->size()),
                    facets_(coordsPerTet * tri->size()) {
                // Note that the vector is initialised to zero since
                // this is what LargeInteger's default constructor does.
                for (size_t i = 0; i < size(); ++i)
                    if (i % coordsPerTet > 3) {
                        // Not a triangular coordinate.
                        facets_.set(i, true);
                    } else if (tri->tetrahedron(i / coordsPerTet)->
                            vertex(i % coordsPerTet)->markedIndex()
                            == whichLink) {
                        // A triangular coordinate in our vertex link.
                        elements[i] = -1;
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
             * @param pos the first of the given rays, in which the given
             * coordinate is positive.
             * @param neg the second of the given rays, in which the given
             * coordinate is negative.
             * @param coord the index of the coordinate that we must set
             * to zero to form the intersecting hyperplane.
             */
            RaySpec(const RaySpec& pos, const RaySpec& neg, size_t coord) :
                    Ray(pos.size()), facets_(pos.facets_) {
                facets_ &= neg.facets_;

                // Note that we may need to re-enable some bits in \a facets_,
                // since we may end up setting some triangle coordinates
                // to zero that were not zero in either \a pos or \a neg.

                LargeInteger posDiff = pos[coord];
                LargeInteger negDiff = neg[coord];

                for (size_t i = 0; i < size(); ++i)
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

                bool start = true;
                LargeInteger max; // max we are allowed to subtract
                size_t i;
                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i)) {
                        if (start) {
                            max = elements[i];
                            start = false;
                        } else if (max > elements[i])
                            max = elements[i];
                    }

                // If start == true then this next loop is harmless, since
                // link.facets_.get(i) must always be true.
                for (i = 0; i < size(); ++i)
                    if (! link.facets_.get(i))
                        if ((elements[i] -= max) == zero)
                            facets_.set(i, true);
            }

            /**
             * Returns a new normal (or almost normal) surface whose
             * coordinates are described by this vector.  The template
             * argument dictates the class of the underlying normal surface
             * vector (i.e., the underlying coordinate system).
             *
             * @param tri the underlying triangulation.
             * @return a newly created normal surface based on this vector.
             */
            template <class VectorClass>
            NormalSurface* recover(Triangulation<3>* tri) const {
                VectorClass* v = new VectorClass(size());

                for (size_t i = 0; i < size(); ++i)
                    v->setElement(i, elements[i]);

                return new NormalSurface(tri, v);
            }

            /**
             * Returns the sign of the given element of this vector.
             *
             * @return 1, 0 or -1 according to whether the <i>index</i>th
             * element of this vector is positive, zero or negative
             * respectively.
             */
            inline int sign(size_t index) const {
                if (facets_.get(index))
                    return 0;
                return (elements[index] > zero ? 1 : -1);
            }

            using Ray::scaleDown;
    };
} // anonymous namespace

template <class Variant>
NormalSurfaces* NormalSurfaces::internalReducedToStandard() const {
    Triangulation<3>* owner = triangulation();

    // Basic sanity checks:
    if (coords_ != Variant::reducedCoords())
        return 0;
    if (which_ != (NS_EMBEDDED_ONLY | NS_VERTEX))
        return 0;
    if (owner->isIdeal() || ! owner->isValid())
        return 0;

    // Prepare a final surface list.
    NormalSurfaces* ans = new NormalSurfaces(
        Variant::standardCoords(), NS_EMBEDDED_ONLY | NS_VERTEX,
        algorithm_ | NS_VERTEX_VIA_REDUCED);

    if (! owner->isEmpty()) {
        // Run our internal conversion routine.
        ans->buildStandardFromReduced<Variant>(owner, surfaces);
    }

    // All done!
    owner->insertChildLast(ans);
    return ans;
}

template <class Variant>
void NormalSurfaces::buildStandardFromReduced(Triangulation<3>* owner,
        const std::vector<NormalSurface*>& reducedList,
        ProgressTracker* tracker) {
    size_t nFacets = Variant::stdLen(owner->size());

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real conversion routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        buildStandardFromReducedUsing<Variant,
            Bitmask1<unsigned> >(owner, reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long))
        buildStandardFromReducedUsing<Variant,
            Bitmask1<unsigned long> >(owner, reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long))
        buildStandardFromReducedUsing<Variant,
            Bitmask1<unsigned long long> >(owner, reducedList, tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        buildStandardFromReducedUsing<Variant,
            Bitmask2<unsigned long long, unsigned> >(owner, reducedList,
                tracker);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        buildStandardFromReducedUsing<Variant,
            Bitmask2<unsigned long long, unsigned long> >(owner, reducedList,
                tracker);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        buildStandardFromReducedUsing<Variant,
            Bitmask2<unsigned long long> >(owner, reducedList, tracker);
    else
        buildStandardFromReducedUsing<Variant, Bitmask>(owner, reducedList,
            tracker);
}

template <class Variant, class BitmaskType>
void NormalSurfaces::buildStandardFromReducedUsing(Triangulation<3>* owner,
        const std::vector<NormalSurface*>& reducedList,
        ProgressTracker* tracker) {
    // Prepare for the reduced-to-standard double description run.
    unsigned long n = owner->size();
    size_t slen = Variant::stdLen(n); // # standard coordinates
    unsigned long llen = owner->countVertices(); // # vertex links

    unsigned i;

    // Recreate the quadrilateral constraints (or the corresponding
    // constraints for almost normal surfaces) as bitmasks.
    // Since we have a non-empty triangulation, we know the list of
    // constraints is non-empty.
    EnumConstraints* constraints =
        Variant::StandardVector::makeEmbeddedConstraints(owner);

    BitmaskType* constraintsBegin = new BitmaskType[constraints->size()];
    BitmaskType* constraintsEnd = constraintsBegin;

    EnumConstraints::const_iterator cit;
    for (EnumConstraints::const_iterator cit = constraints->begin();
            cit != constraints->end(); ++cit, ++constraintsEnd) {
        constraintsEnd->reset(slen);
        constraintsEnd->set(cit->begin(), cit->end(), true);
    }

    delete constraints;

    // Create all vertex links.
    typename Variant::StandardVector** link =
        new typename Variant::StandardVector*[llen];

    for (i = 0; i < llen; ++i) {
        link[i] = new typename Variant::StandardVector(slen);

        for (auto& emb : *owner->vertex(i))
            link[i]->setElement(Variant::stdPos(
                emb.tetrahedron()->markedIndex(), emb.vertex()), 1);
    }

    // Create the initial set of rays:
    typedef std::vector<RaySpec<BitmaskType>*> RaySpecList;
    RaySpecList list[2];

    NormalSurfaceVector* v;
    std::vector<NormalSurface*>::const_iterator qit;
    for (qit = reducedList.begin(); qit != reducedList.end(); ++qit) {
        v = Variant::ReducedVector::makeMirror((*qit)->rawVector(), owner);
        list[0].push_back(new RaySpec<BitmaskType>(v->coords()));
        delete v;
    }

    // Each additional inequality is of the form tri_coord >= 0.
    // We will therefore just create them on the fly as we need them.

    // And run!
    BitmaskType ignoreFacets(slen);
    for (i = 0; i < slen; ++i)
        if (i % Variant::totalPerTet < 4)
            ignoreFacets.set(i, true);

    int workingList = 0;

    unsigned vtx;
    size_t tcoord;
    RaySpec<BitmaskType>* linkSpec;

    RaySpecList pos, neg;
    typename RaySpecList::iterator it, posit, negit;

    int sign;
    BitmaskType* constraintMask;
    bool broken;

    unsigned long slices = 0;
    unsigned iterations;
    for (vtx = 0; vtx < llen; ++vtx) {
        linkSpec = new RaySpec<BitmaskType>(link[vtx]->coords());
        delete link[vtx];

        list[workingList].push_back(new RaySpec<BitmaskType>(owner, vtx,
            Variant::totalPerTet));

        for (auto& emb : *owner->vertex(vtx)) {
            // Update the state of progress and test for cancellation.
            if (tracker && ! tracker->setPercent(25.0 * slices++ / n)) {
                for (it = list[workingList].begin();
                        it != list[workingList].end(); ++it)
                    delete *it;
                return;
            }

            tcoord = Variant::stdPos(emb.tetrahedron()->markedIndex(),
                emb.vertex());

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

            iterations = 0;
            for (posit = pos.begin(); posit != pos.end(); ++posit)
                for (negit = neg.begin(); negit != neg.end(); ++negit) {
                    // Test for cancellation, but not every time (since
                    // this involves expensive mutex locking).
                    if (tracker && ++iterations == 100) {
                        iterations = 0;
                        if (tracker->isCancelled()) {
                            for (it = list[1 - workingList].begin();
                                    it != list[1 - workingList].end(); ++it)
                                delete *it;
                            for (it = neg.begin(); it != neg.end(); ++it)
                                delete *it;
                            return;
                        }
                    }

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
        // Note that cancelling future vertex links might introduce
        // new common factors that can be divided out.
        list[workingList].push_back(linkSpec);

        for (it = list[workingList].begin(); it != list[workingList].end();
                ++it) {
            for (i = vtx + 1; i < llen; ++i)
                (*it)->reduce(link[i]->coords());
            (*it)->scaleDown();
        }
    }

    // All done!  Put the solutions into the normal surface list and clean up.
    for (typename RaySpecList::iterator it = list[workingList].begin();
            it != list[workingList].end(); ++it) {
        surfaces.push_back((*it)->
            template recover<typename Variant::StandardVector>(owner));
        delete *it;
    }

    delete[] link;
    delete[] constraintsBegin;

    if (tracker)
        tracker->setPercent(100);
}

} // namespace regina

