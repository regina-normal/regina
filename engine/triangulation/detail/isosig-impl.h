
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/detail/isosig-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __REGINA_ISOSIG_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_IMPL_H_DETAIL
#endif

#include <algorithm>
#include "triangulation/generic/triangulation.h"
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim> requires (supportedDim(dim))
void IsoSigData<1, dim>::fillFrom(const Simplex<dim>* simplex,
        Perm<dim + 1> vertices, Isomorphism<dim>* relabelling) {
    const auto& tri = simplex->triangulation();
    size_t triSize = tri.size();
    size_t simp = simplex->index();

    // The image for each simplex and its vertices (mapping from the original
    // triangulation to the canonical labelling):
    FixedArray<ssize_t> image(triSize, -1);
    FixedArray<Perm<dim+1>> vertexMap(triSize);

    // The preimage for each simplex (mapping from the canonical labelling to
    // the original triangulation):
    FixedArray<ssize_t> preImage(size_, -1);

    image[simp] = 0;
    vertexMap[simp] = vertices.inverse();
    preImage[0] = simp;

    size_t facetPos = 0;
    size_t joinPos = 0;
    size_t nextUnusedSimp = 1;

    // To obtain a canonical isomorphism, we must run through the
    // simplices and their facets in image order, not preimage order.
    //
    // This main loop is guaranteed to exit when (and only when) we have
    // exhausted the entire connected component containing the given simplex.
    for (size_t simpImg = 0; simpImg < size_; ++simpImg) {
        // We are guaranteed that preImage[simpImg] >= 0.
        size_t simpSrc = preImage[simpImg];
        const Simplex<dim>* s = tri.simplex(simpSrc);

        if (! locks_.empty()) {
            using Mask = typename Simplex<dim>::LockMask;
            if (Mask mask = s->lockMask()) {
                static constexpr Mask facetMask = (Mask(1) << (dim + 1)) - 1;
                if (mask & facetMask) {
                    // One or more facets of this simplex are locked.
                    // We need to permute the bits of lockMask
                    // according to vertexMap[simpSrc].
                    Mask permuted = (mask & (Mask(1) << (dim + 1)));
                    Mask bit = 1;
                    for (int i = 0; i <= dim; ++i, bit <<= 1)
                        if (mask & bit)
                            permuted |= (Mask(1) << vertexMap[simpSrc][i]);
                    mask = permuted;
                }
                locks_[simpImg] = mask;
            } else {
                locks_[simpImg] = 0;
            }
        }

        for (int facetImg = 0; facetImg <= dim; ++facetImg) {
            int facetSrc = vertexMap[simpSrc].pre(facetImg);

            // INVARIANTS:
            // - nextUnusedSimp > simpImg
            // - image[simpSrc], preImage[image[simpSrc]] and
            //   vertexMap[simpSrc] are already filled in.

            // Work out what happens to our source facet.
            if (auto adj = s->adjacentSimplex(facetSrc)) {
                // Our source facet is internal, i.e., glued to something else.
                size_t dest = adj->index();

                if (image[dest] >= 0) {
                    // We've seen the destination simplex before.
                    if (image[dest] < image[simpSrc] ||
                            (dest == simpSrc &&
                             vertexMap[simpSrc][s->adjacentFacet(facetSrc)]
                             < vertexMap[simpSrc][facetSrc])) {
                        // We've already seen this gluing from the other side.
                        // Skip this facet entirely.
                    } else {
                        // Record the full details of the gluing.
                        adjSimplex_[joinPos] = image[dest];
                        adjGluing_[joinPos] = (vertexMap[dest] *
                                s->adjacentGluing(facetSrc) *
                                vertexMap[simpSrc].inverse()).
                            orderedSnIndex();
                        ++joinPos;

                        facetType_[facetPos++] = 2;
                    }
                } else {
                    // The destination simplex is thus-far unseen.
                    // It must take the next available index, and the
                    // corresponding gluing must be the identity.
                    image[dest] = nextUnusedSimp++;
                    preImage[image[dest]] = dest;
                    vertexMap[dest] = vertexMap[simpSrc] *
                        s->adjacentGluing(facetSrc).inverse();

                    facetType_[facetPos++] = 1;
                }
            } else {
                // Our source facet lies on the boundary.
                facetType_[facetPos++] = 0;
            }
        }
    }

    // Record the canonical isomorphism if required.
    if (relabelling)
        for (size_t i = 0; i < size_; ++i) {
            size_t origIndex = preImage[i];
            relabelling->simpImage(origIndex) = i; // image[origIndex]
            relabelling->facetPerm(origIndex) = vertexMap[origIndex];
        }
}

template <int dim> requires (supportedDim(dim))
template <IsoSigType<dim> Type>
IsoSigData<2, dim> IsoSigData<2, dim>::minimal(Component<dim>* component,
        bool oriented, Isomorphism<dim>* relabelling) {
    // To help if/when we need to process facet (not simplex) locks:
    using Mask = typename Simplex<dim>::LockMask;
    static constexpr Mask facetMask = (Mask(1) << (dim + 1)) - 1;

    const auto& tri = component->triangulation();
    size_t triSize = tri.size();

    IsoSigData best(component); // The minimal canonical labelling seen so far
    IsoSigData curr(component); // The canonical labelling currently being built

    // The image for each simplex and its vertices (mapping from the original
    // triangulation to the canonical labelling):
    FixedArray<ssize_t> image(triSize);
    FixedArray<Perm<dim+1>> vertexMap(triSize);

    // The preimage for each simplex (mapping from the canonical labelling to
    // the original triangulation):
    FixedArray<ssize_t> preImage(component->size());

    Type type(*component, oriented);
    bool first = true;
    do {
        // We need to fill curr with a canonical relabelling, starting from the
        // current choice of simplex 0 and its vertices 0,...,dim.
        std::fill(image.begin(), image.end(), -1);
        std::fill(preImage.begin(), preImage.end(), -1);

        // Convert from component-based to triangulation-based indexing:
        size_t simp = component->simplex(type.simplex())->index();

        image[simp] = 0;
        vertexMap[simp] = type.perm().inverse();
        preImage[0] = simp;

        size_t facetPos = curr.countFacetBits() - 1;
        size_t simplexPos = 0, gluingPos = 0;
        size_t nextUnusedSimp = 1;

        // To obtain a canonical isomorphism, we must run through the
        // simplices and their facets in image order, not preimage order.
        //
        // This main loop is guaranteed to exit when (and only when) we have
        // exhausted the entire component containing the given simplex.
        for (size_t simpImg = 0; simpImg < component->size(); ++simpImg) {
            // We are guaranteed that preImage[simpImg] >= 0.
            size_t simpSrc = preImage[simpImg];
            const Simplex<dim>* s = tri.simplex(simpSrc);

            if (! curr.locks_.empty()) {
                if (Mask mask = s->lockMask()) {
                    if (mask & facetMask) {
                        // One or more facets of this simplex are locked.
                        // We need to permute the bits of lockMask
                        // according to vertexMap[simpSrc].
                        Mask permuted = (mask & (Mask(1) << (dim + 1)));
                        Mask bit = 1;
                        for (int i = 0; i <= dim; ++i, bit <<= 1)
                            if (mask & bit)
                                permuted |= (Mask(1) << vertexMap[simpSrc][i]);
                        mask = permuted;
                    }
                    curr.locks_[simpImg] = mask;
                } else {
                    curr.locks_[simpImg] = 0;
                }
            }

            for (int facetImg = 0; facetImg <= dim; ++facetImg) {
                int facetSrc = vertexMap[simpSrc].pre(facetImg);

                // INVARIANTS:
                // - nextUnusedSimp > simpImg
                // - image[simpSrc], preImage[image[simpSrc]] and
                //   vertexMap[simpSrc] are already filled in.

                // Work out what happens to our source facet.
                if (auto adj = s->adjacentSimplex(facetSrc)) {
                    // Our source facet is internal.
                    size_t dest = adj->index();

                    if (image[dest] >= 0) {
                        // We've seen the destination simplex before.
                        if (image[dest] < image[simpSrc] ||
                                (dest == simpSrc &&
                                 vertexMap[simpSrc][s->adjacentFacet(facetSrc)]
                                 < vertexMap[simpSrc][facetSrc])) {
                            // We've already seen this facet from the other
                            // side.  We should skip it entirely this time.
                        } else {
                            // Record the full details of the gluing.
                            curr.adjSimplex_[simplexPos++] = image[dest];
                            curr.adjGluing_[gluingPos++] = (vertexMap[dest] *
                                    s->adjacentGluing(facetSrc) *
                                    vertexMap[simpSrc].inverse()).
                                SnIndex();

                            curr.facetType_.set(facetPos--, true);
                        }
                    } else {
                        // The destination simplex is thus-far unseen.
                        // It must take the next available index, and the
                        // corresponding gluing must be the pair swap (0,1).
                        image[dest] = nextUnusedSimp++;
                        preImage[image[dest]] = dest;
                        vertexMap[dest] = Perm<dim + 1>(0, 1) *
                            vertexMap[simpSrc] *
                            s->adjacentGluing(facetSrc).inverse();

                        --facetPos; // leave the facet bit unset
                    }
                } else {
                    // Our source facet lies on the boundary.
                    curr.facetType_.set(facetPos--, true);
                    curr.adjSimplex_[simplexPos++] = component->size();
                }
            }
        }

        // Record the canonical isomorphism if required.
        if (relabelling)
            for (size_t i = 0; i < component->size(); ++i) {
                size_t origIndex = preImage[i];
                relabelling->simpImage(origIndex) = i; // image[origIndex]
                relabelling->facetPerm(origIndex) = vertexMap[origIndex];
            }

        // We are done building the canonical relabelling.
        // Is this the best we've seen so far?
        if (first || curr < best) {
            best.swap(curr);
            first = false;
        }
    } while (type.next());

    return best;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintable::encode(const IsoSigData<1, dim>& data) {
    Base64SigEncoder enc;
    enc.reserve(length(data));

    int intWidth = enc.encodeSize(data.size());
    enc.encodeTrits(data.facetTypes());
    enc.encodeInts(data.adjacentSimplices(), intWidth);
    enc.encodeInts(data.adjacentGluings(), charsPerPerm<dim>);

    if (data.hasLocks()) {
        // Each lock mask holds dim+2 bits.
        enc.append(Base64SigEncoder::spare[1]);
        for (auto mask : data.locks()) {
            if constexpr (dim <= 4) {
                // We can encode <= 6 bits with 1 character.
                enc.encodeSingle(mask);
            } else if constexpr (dim <= 10) {
                // We can encode <= 12 bits with 2 characters.
                enc.encodeInt(mask, 2);
            } else {
                static_assert(dim <= 16);
                // We can encode <= 18 bits with 3 characters.
                enc.encodeInt(mask, 3);
            }
        }
    }

    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintable::length(const IsoSigData<1, dim>& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly
        // encoded.
        ans = 1 + data.adjacentSimplices().size();
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.adjacentSimplices().size()) * width;
    }
    ans += ((data.facetTypes().size() + 2) / 3);
    ans += (data.adjacentGluings().size() * charsPerPerm<dim>);

    if (data.hasLocks()) {
        if constexpr (dim <= 4)
            ans += (1 + data.locks().size());
        else if constexpr (dim <= 10)
            ans += (1 + data.locks().size() * 2);
        else
            ans += (1 + data.locks().size() * 3);
    }

    return ans;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintable::encode(const IsoSigData<2, dim>& data) {
    Base64SigEncoder enc;
    enc.reserve(length(data));

    int intWidth = enc.encodeSize(data.size());
    enc.encodeInts(data.adjacentSimplices(), intWidth);
    enc.encodeBits(data.countFacetBits(), data.facetTypes());
    enc.encodeInts(data.adjacentGluings(), charsPerPerm<dim>);

    if (data.hasLocks()) {
        // Each lock mask holds dim+2 bits.
        enc.append(Base64SigEncoder::spare[1]);
        for (auto mask : data.locks()) {
            if constexpr (dim <= 4) {
                // We can encode <= 6 bits with 1 character.
                enc.encodeSingle(mask);
            } else if constexpr (dim <= 10) {
                // We can encode <= 12 bits with 2 characters.
                enc.encodeInt(mask, 2);
            } else {
                static_assert(dim <= 16);
                // We can encode <= 18 bits with 3 characters.
                enc.encodeInt(mask, 3);
            }
        }
    }

    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintable::length(const IsoSigData<2, dim>& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly
        // encoded.
        ans = 1 + data.adjacentSimplices().size();
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.adjacentSimplices().size()) * width;
    }
    ans += ((data.countFacetBits() + 5) / 6);
    ans += (data.adjacentGluings().size() * charsPerPerm<dim>);

    if (data.hasLocks()) {
        if constexpr (dim <= 4)
            ans += (1 + data.locks().size());
        else if constexpr (dim <= 10)
            ans += (1 + data.locks().size() * 2);
        else
            ans += (1 + data.locks().size() * 3);
    }

    return ans;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintableLockFree::encode(const IsoSigData<1, dim>& data) {
    Base64SigEncoder enc;
    enc.reserve(length(data));

    int intWidth = enc.encodeSize(data.size());
    enc.encodeTrits(data.facetTypes());
    enc.encodeInts(data.adjacentSimplices(), intWidth);
    enc.encodeInts(data.adjacentGluings(), charsPerPerm<dim>);
    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintableLockFree::length(const IsoSigData<1, dim>& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly
        // encoded.
        ans = 1 + data.adjacentSimplices().size();
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.adjacentSimplices().size()) * width;
    }
    ans += ((data.facetTypes().size() + 2) / 3);
    ans += (data.adjacentGluings().size() * charsPerPerm<dim>);
    return ans;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintableLockFree::encode(const IsoSigData<2, dim>& data) {
    Base64SigEncoder enc;
    enc.reserve(length(data));

    int intWidth = enc.encodeSize(data.size());
    enc.encodeInts(data.adjacentSimplices(), intWidth);
    enc.encodeBits(data.countFacetBits(), data.facetTypes());
    enc.encodeInts(data.adjacentGluings(), charsPerPerm<dim>);
    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintableLockFree::length(const IsoSigData<2, dim>& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly
        // encoded.
        ans = 1 + data.adjacentSimplices().size();
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.adjacentSimplices().size()) * width;
    }
    ans += ((data.countFacetBits() + 5) / 6);
    ans += (data.adjacentGluings().size() * charsPerPerm<dim>);
    return ans;
}

template <int dim> requires (supportedDim(dim))
ByteSequence IsoSigBinary::encode(const IsoSigData<2, dim>& data) {
    // We begin with a single byte b, where:
    //
    // - the lower six bits of b encode the number of bits required to encode
    //   any integer in the range [0..size] (note that this imposes the
    //   restriction size < 2^64, but this is enormously more than enough for
    //   any triangulation);
    // - bit 6 is set if all gluings are orientation-preserving;
    // - bit 7 is set if we need to encode locks.
    //
    // The encoding of the real data begins at the second byte.

    int intWidth = bitsRequired(data.size() + 1);
    static constexpr int permWidth = bitsRequired(Perm<dim + 1>::nPerms);
    static constexpr int lockWidth = dim + 2;

    // Get an unsigned type for the permutation index, since BitSigDecoder
    // only reads/writes unsigned integer types.
    using PermIndex = MakeUnsigned<typename Perm<dim + 1>::Index>;

    bool oriented = data.isOriented();

    BitSigEncoder enc;
    enc.reserveBits(
        8 + (intWidth * data.adjacentSimplices().size()) +
        data.countFacetBits() +
        (oriented ? (permWidth - 1) * data.adjacentGluings().size() :
            permWidth * data.adjacentGluings().size()) +
        lockWidth * data.locks().size());

    enc.encodeInt(6, static_cast<unsigned>(intWidth));
    enc.encodeBit(oriented);
    enc.encodeBit(data.hasLocks());

    enc.encodeInt(intWidth, data.size());
    for (auto s : data.adjacentSimplices())
        enc.encodeInt(intWidth, s);
    enc.encodeBitmask(data.countFacetBits(), data.facetTypes());
    if (oriented) {
        for (PermIndex g : data.adjacentGluings())
            enc.encodeInt(permWidth - 1, g >> 1);
    } else {
        for (PermIndex g : data.adjacentGluings())
            enc.encodeInt(permWidth, g);
    }
    for (auto m : data.locks())
        enc.encodeInt(lockWidth, m);

    return std::move(enc).bytes();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigBinary::length(const IsoSigData<2, dim>& data) {
    int intWidth = bitsRequired(data.size() + 1);
    static constexpr int permWidth = bitsRequired(Perm<dim + 1>::nPerms);
    static constexpr int lockWidth = dim + 2;
    bool oriented = data.isOriented();

    size_t bits = 8 + (intWidth * (data.adjacentSimplices().size() + 1)) +
        data.countFacetBits();
    if (oriented)
        bits += (permWidth - 1) * data.adjacentGluings().size();
    else
        bits += permWidth * data.adjacentGluings().size();
    bits += lockWidth * data.locks().size();

    return (bits + 7) / 8;
}

namespace detail {

template <int dim> requires (supportedDim(dim))
template <IsoSigEncoding<1, dim> Encoding, IsoSigType<dim> Type>
typename Encoding::Signature TriangulationBase<dim>::isoSig() const {
    if (isEmpty())
        return Encoding::encodeEmpty();

    if (isConnected()) {
        Component<dim>* c = components_.front();

        typename Encoding::Signature best;
        bool first = true;

        Type type(*c, false /* first-generation sigs are unoriented */);
        IsoSigData<1, dim> data(c);
        do {
            data.fillFrom(c->simplex(type.simplex()), type.perm(), nullptr);
            typename Encoding::Signature curr = Encoding::encode(data);
            if (first || curr < best) {
                best.swap(curr);
                first = false;
            }
        } while (type.next());
        return best;
    }

    // We have a multiple-component triangulation.
    // Get a signature string for each connected component.
    size_t i;

    FixedArray<typename Encoding::Signature> comp(countComponents());
    auto it = components().begin();
    for (i = 0; it != components().end(); ++it, ++i) {
        Type type(**it, false /* first-generation sigs are unoriented */);
        bool first = true;

        IsoSigData<1, dim> data(*it);
        do {
            data.fillFrom((*it)->simplex(type.simplex()), type.perm(), nullptr);
            typename Encoding::Signature curr = Encoding::encode(data);
            if (first || curr < comp[i]) {
                comp[i].swap(curr);
                first = false;
            }
        } while (type.next());
    }

    // Pack the components together.
    std::sort(comp.begin(), comp.end());

    // Precompute the entire length of the signature, so that we can
    // reserve exactly the right amount of space.
    size_t length = 0;
    for (const auto& c : comp)
        length += c.size();

    typename Encoding::Signature ans;
    ans.reserve(length);
    for (const auto& c : comp)
        ans += c;
    return ans;
}

template <int dim> requires (supportedDim(dim))
template <IsoSigEncoding<1, dim> Encoding, IsoSigType<dim> Type>
std::pair<typename Encoding::Signature, Isomorphism<dim>>
        TriangulationBase<dim>::isoSigDetail() const {
    // Make sure the user is not trying to do something illegal.
    if (isEmpty())
        throw FailedPrecondition(
            "isoSigDetail() requires a non-empty triangulation");
    if (countComponents() > 1)
        throw FailedPrecondition(
            "isoSigDetail() requires a connected triangulation");

    // The triangulation has exactly one connected component.
    Component<dim>* c = components_.front();

    std::pair<typename Encoding::Signature, Isomorphism<dim>> ans(
        std::piecewise_construct, std::tuple<>(), std::tuple(size()));
    Isomorphism<dim> currRelabelling(size());
    bool first = true;

    Type type(*c, false /* first-generation sigs are unoriented */);
    IsoSigData<1, dim> data(c);
    do {
        data.fillFrom(c->simplex(type.simplex()), type.perm(),
            std::addressof(currRelabelling));
        typename Encoding::Signature curr = Encoding::encode(data);
        if (first || curr < ans.first) {
            ans.first.swap(curr);
            ans.second.swap(currRelabelling);
            first = false;
        }
    } while (type.next());
    return ans;
}

template <int dim> requires (supportedDim(dim))
template <IsoSigEncoding<2, dim> Encoding, IsoSigType<dim> Type>
typename Encoding::Signature TriangulationBase<dim>::neoSig(bool oriented)
        const {
    if (oriented && ! isOriented())
        throw FailedPrecondition("An oriented isomorphism signature "
            "requires an oriented triangulation");

    if (isEmpty()) {
        return Encoding::encodeEmpty();
    } else if (isConnected()) {
        Component<dim>* c = components_.front();
        return Encoding::encode(IsoSigData<2, dim>::template minimal<Type>(
            c, oriented));
    } else {
        std::vector<IsoSigData<2, dim>> bits;
        for (auto c : components_)
            bits.emplace_back(IsoSigData<2, dim>::template minimal<Type>(
                c, oriented));
        std::sort(bits.begin(), bits.end());

        // Precompute the entire length of the signature, so that we can
        // reserve exactly the right amount of space.
        size_t length = 0;
        for (const auto& data : bits)
            length += Encoding::length(data);

        typename Encoding::Signature sig;
        sig.reserve(length);
        for (const auto& data : bits)
            sig += Encoding::encode(data);
        return sig;
    }
}

template <int dim> requires (supportedDim(dim))
template <IsoSigEncoding<2, dim> Encoding, IsoSigType<dim> Type>
std::pair<typename Encoding::Signature, Isomorphism<dim>>
        TriangulationBase<dim>::neoSigDetail(bool oriented) const {
    if (oriented && ! isOriented())
        throw FailedPrecondition("An oriented isomorphism signature "
            "requires an oriented triangulation");

    if (isEmpty()) {
        throw FailedPrecondition(
            "neoSigDetail() requires a non-empty triangulation");
    } else if (countComponents() > 1) {
        throw FailedPrecondition(
            "neoSigDetail() requires a connected triangulation");
    } else {
        // The triangulation has exactly one connected component.
        Component<dim>* c = components_.front();
        Isomorphism<dim> relabelling(size());
        auto sig = Encoding::encode(IsoSigData<2, dim>::template minimal<Type>(
            c, oriented, std::addressof(relabelling)));
        return { std::move(sig), std::move(relabelling) };
    }
}

template <int dim> requires (supportedDim(dim))
Triangulation<dim> TriangulationBase<dim>::fromSig(const std::string& sig) {
    Base64SigDecoder dec(sig.begin(), sig.end()); // strips whitespace

    try {
        Triangulation<dim> ans;

        while (! dec.done()) {
            // Read one component at a time.
            // Note: the call to dec.done() ignores whitespace, but if there
            // is _internal_ whitespace between components then this will be
            // caught by decodeSize() below.
            auto [ nSimp, nChars ] = dec.decodeSize();
            if (nSimp == 0)
                continue; // empty component (this should not happen..?)

            FixedArray<uint8_t> facetAction((dim+1) * nSimp + 2);
            size_t nFacets = 0;
            auto facetPos = facetAction.begin();
            size_t nJoins = 0;

            for ( ; nFacets < (dim+1) * nSimp; facetPos += 3) {
                dec.decodeTrits(facetPos);
                for (int j = 0; j < 3; ++j) {
                    // If we're already finished, make sure the leftover trits
                    // are zero.
                    if (nFacets == (dim+1) * nSimp) {
                        if (*(facetPos + j) != 0) {
                            throw InvalidArgument(
                                "fromSig(): extraneous facet actions");
                        }
                        continue;
                    }

                    switch (*(facetPos + j)) {
                        case 0:
                            ++nFacets;
                            break;
                        case 1:
                            nFacets += 2;
                            break;
                        case 2:
                            nFacets += 2;
                            ++nJoins;
                            break;
                        default:
                            throw InvalidArgument(
                                "fromSig(): invalid facet action");
                    }
                    if (nFacets > (dim+1) * nSimp) {
                        throw InvalidArgument("fromSig(): facet actions "
                            "do not match triangulation size");
                    }
                }
            }

            auto joinDest = dec.template decodeInts<size_t>(nJoins, nChars);
            auto joinGluing =
                dec.template decodeInts<typename Perm<dim+1>::Index>(
                nJoins, IsoSigPrintable::charsPerPerm<dim>);

            // This ends the gluings for this component!
            //
            // We still need to read facet/simplex locks, if they are present.
            // We will do this after constructing the triangulation.

            FixedArray<Simplex<dim>*> simp(nSimp);
            for (size_t pos = 0; pos < nSimp; ++pos)
                simp[pos] = ans.newSimplex();

            facetPos = facetAction.begin();
            size_t nextUnused = 1;
            size_t joinPos = 0;
            Perm<dim+1> gluing;
            for (size_t pos = 0; pos < nSimp; ++pos)
                for (int j = 0; j <= dim; ++j) {
                    // Already glued from the other side:
                    if (simp[pos]->adjacentSimplex(j))
                        continue;

                    if (*facetPos == 0) {
                        // Boundary facet.
                    } else if (*facetPos == 1) {
                        // Join to new simplex.
                        if (nextUnused >= nSimp) {
                            throw InvalidArgument(
                                "fromSig(): gluing to non-existent simplex");
                        }
                        simp[pos]->join(j, simp[nextUnused++], Perm<dim+1>());
                    } else {
                        // Join to existing simplex.
                        if (joinGluing[joinPos] >= Perm<dim+1>::nPerms ||
                                joinGluing[joinPos] < 0) {
                            throw InvalidArgument(
                                "fromSig(): invalid gluing permutation");
                        }
                        gluing = Perm<dim+1>::orderedSn[joinGluing[joinPos]];
                        if (joinDest[joinPos] >= nextUnused ||
                                simp[joinDest[joinPos]]->adjacentSimplex(
                                gluing[j])) {
                            throw InvalidArgument(
                                "fromSig(): invalid gluing destination");
                        }
                        simp[pos]->join(j, simp[joinDest[joinPos]], gluing);
                        ++joinPos;
                    }

                    ++facetPos;
                }

            // Read simplex/facet locks, if these are present.
            if (dec.peek() == Base64SigEncoder::spare[1]) {
                dec.skip();

                using LockMask = typename Simplex<dim>::LockMask;
                // Each lock mask encodes dim+2 bits.
                static constexpr LockMask maskChars = (dim + 7) / 6;
                auto lockMasks = dec.template decodeInts<LockMask>(
                    nSimp, maskChars);

                // We will set lock masks directly instead of using lock()
                // functions.  This means we don't get change spans (but that
                // is fine since we have computed nothing about the
                // triangulation and nobody else has a reference to it yet).
                // It also means that we need to run our own sanity checks,
                // which we will do shortly.
                for (size_t i = 0; i < nSimp; ++i) {
                    if (lockMasks[i] >> (dim + 2) != 0)
                        throw InvalidArgument(
                            "fromSig(): invalid lock mask");
                    simp[i]->locks_ = lockMasks[i];
                }

                // Check facet locks for consistency.
                for (auto s : simp)
                    if (s->locks_)
                        for (int facet = 0; facet <= dim; ++facet)
                            if (auto adj = s->adjacentSimplex(facet))
                                if (s->isFacetLocked(facet)) {
                                    auto adjFacet = s->adjacentFacet(facet);
                                    if (! adj->isFacetLocked(adjFacet))
                                        throw InvalidArgument("fromSig(): "
                                            "inconsistent lock masks");
                                }
            }
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by Base64SigDecoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid base64 encoding");
    }
}

template <int dim> requires (supportedDim(dim))
size_t TriangulationBase<dim>::isoSigComponentSize(const std::string& sig) {
    Base64SigDecoder dec(sig.begin(), sig.end()); // strips whitespace
    try {
        return dec.decodeSize().first;
    } catch (const InvalidInput&) {
        throw InvalidArgument("isoSigComponentSize(): invalid signature");
    }
}

template <int dim> requires (supportedDim(dim))
Triangulation<dim> TriangulationBase<dim>::fromSig(const ByteSequence& sig) {
    static constexpr int permWidth = bitsRequired(Perm<dim + 1>::nPerms);
    static constexpr int lockWidth = dim + 2;

    // Get an unsigned type for the permutation index, since BitSigDecoder
    // only reads/writes unsigned integer types.
    using PermIndex = MakeUnsigned<typename Perm<dim + 1>::Index>;

    BitSigDecoder dec(sig.begin(), sig.end());

    try {
        Triangulation<dim> ans;

        while (! dec.noMoreBits()) {
            // Read one component at a time.

            unsigned intWidth = dec.decodeInt<unsigned>(6);
            if (intWidth == 0)
                throw InvalidArgument(
                    "fromSig(): invalid integer width for binary encoding");
            bool oriented = dec.decodeBit();
            bool hasLocks = dec.decodeBit();

            size_t nSimp = dec.decodeInt<size_t>(intWidth);
            if (nSimp == 0)
                throw InvalidArgument(
                    "fromSig(): invalid component size for binary encoding");

            size_t nBdry = 0;
            size_t nDest = 2 * (nSimp - 1);
            std::vector<size_t> adjSimplex;
            adjSimplex.reserve((nSimp * (dim + 1) + 1) / 2); // a lower bound
            while (nDest < nSimp * (dim + 1)) {
                size_t dest = dec.decodeInt<size_t>(intWidth);
                if (dest == nSimp) {
                    ++nBdry;
                    ++nDest;
                } else {
                    nDest += 2;
                }
                adjSimplex.push_back(dest);
            }

            size_t nFacets = (nDest + nBdry) / 2;
            Bitmask facetType = dec.decodeBitmask(nFacets);

            FixedArray<PermIndex> adjGluing(nFacets - nBdry + 1 - nSimp);
            if (oriented) {
                for (auto& index : adjGluing)
                    index = dec.decodeInt<PermIndex>(permWidth - 1) * 2 + 1;
            } else {
                for (auto& index : adjGluing)
                    index = dec.decodeInt<PermIndex>(permWidth);
            }

            // This ends the gluings for this component!
            //
            // We still need to read facet/simplex locks, if they are present.
            // We will do this after constructing the triangulation.

            FixedArray<Simplex<dim>*> simp(nSimp);
            for (auto& s : simp)
                s = ans.newSimplex();

            size_t facetPos = nFacets - 1;
            size_t nextUnused = 1;
            auto destPos = adjSimplex.begin();
            auto gluingPos = adjGluing.begin();
            for (auto s : simp)
                for (int f = 0; f <= dim; ++f) {
                    // Already glued from the other side:
                    if (s->adjacentSimplex(f))
                        continue;

                    if (facetType.get(facetPos--)) {
                        size_t dest = *destPos++;

                        if (dest != nSimp) {
                            // A non-boundary facet, joined to a simplex we
                            // have already seen.
                            PermIndex index = *gluingPos++;
                            if (index >= Perm<dim+1>::nPerms)
                                throw InvalidArgument(
                                    "fromSig(): invalid gluing permutation");
                            Perm<dim+1> gluing = Perm<dim+1>::Sn[index];

                            if (dest >= nextUnused ||
                                    simp[dest]->adjacentSimplex(gluing[f])) {
                                throw InvalidArgument(
                                    "fromSig(): invalid gluing destination");
                            }
                            s->join(f, simp[dest], gluing);
                        }
                    } else {
                        // Join to the next new simplex.
                        if (nextUnused >= nSimp)
                            throw InvalidArgument(
                                "fromSig(): gluing to non-existent simplex");
                        s->join(f, simp[nextUnused++], { 0, 1 });
                    }
                }

            // Read simplex/facet locks, if these are present.
            if (hasLocks) {
                // We will set lock masks directly instead of using lock()
                // functions.  This means we don't get change spans (but that
                // is fine since we have computed nothing about the
                // triangulation and nobody else has a reference to it yet).
                // It also means that we need to run our own sanity checks,
                // which we will do shortly.
                for (auto s : simp) {
                    auto mask = dec.decodeInt<typename Simplex<dim>::LockMask>(
                        lockWidth);
                    if (mask >> (dim + 2) != 0)
                        throw InvalidArgument("fromSig(): invalid lock mask");
                    s->locks_ = mask;
                }

                // Check facet locks for consistency.
                for (auto s : simp)
                    if (s->locks_)
                        for (int facet = 0; facet <= dim; ++facet)
                            if (auto adj = s->adjacentSimplex(facet))
                                if (s->isFacetLocked(facet)) {
                                    auto adjFacet = s->adjacentFacet(facet);
                                    if (! adj->isFacetLocked(adjFacet))
                                        throw InvalidArgument("fromSig(): "
                                            "inconsistent lock masks");
                                }
            }

            dec.flushByte();
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by BitSigDecoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid binary encoding");
    }
}

} } // namespace regina::detail

#endif
