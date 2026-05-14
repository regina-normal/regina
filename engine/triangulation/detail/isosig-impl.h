
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

                        curr.facetType_.set(facetPos--, false);
                    }
                } else {
                    // Our source facet lies on the boundary.
                    curr.facetType_.set(facetPos--, true);
                    curr.adjSimplex_[simplexPos++] = component->size();
                }
            }
        }

        // We are done building the canonical relabelling.
        // Is this the best we've seen so far?
        if (first || curr < best) {
            best.swap(curr);
            first = false;

            // Record the canonical isomorphism if required.
            if (relabelling)
                for (size_t i = 0; i < component->size(); ++i) {
                    size_t origIndex = preImage[i];
                    relabelling->simpImage(origIndex) = i; // image[origIndex]
                    relabelling->facetPerm(origIndex) = vertexMap[origIndex];
                }
        }
    } while (type.next());

    best.oriented_ = true;
    for (auto g : best.adjGluing_)
        if (! (g & 1)) {
            best.oriented_ = false;
            break;
        }

    return best;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintable::encode(const IsoSigData<1, dim>& data) {
    Base64Encoder enc;
    enc.reserve(length(data));

    int intWidth = enc.encodeSize(data.size());
    enc.encodeTrits(data.facetTypes());
    enc.encodeInts(data.adjacentSimplices(), intWidth);
    enc.encodeInts(data.adjacentGluings(), charsPerPerm<dim>);

    if (data.hasLocks()) {
        // Each lock mask holds dim+2 bits.
        enc.append(Base64Encoder::spare[1]);
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
        int width = Base64Encoder::integerWidth(data.size());
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
    Base64BitEncoder enc;
    enc.reserveChars(length(data));

    // Begin with the size, encoded in a way that is compatible with
    // first-generation signatures.
    enc.encodeSize(data.size());

    // Continue with a bit-by-bit encoding.
    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    // We begin by encoding the bits [11], which can never appear at the
    // beginning of a first-generation signature.  This will allow fromSig()
    // to determine which generation a given base64 signature is.
    enc.encodeInt<unsigned>(2 /* encode two bits */, 3 /* the bits: 11 */);

    enc.encodeBit(oriented);

    using UnsignedPermIndex = typename IsoSigData<2, dim>::UnsignedPermIndex;
    for (auto s : data.adjacentSimplices())
        enc.encodeInt(intBits, s);
    enc.encodeBitmask(data.countFacetBits(), data.facetTypes());
    if (oriented) {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits - 1, g >> 1);
    } else {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits, g);
    }
    if (data.hasLocks()) {
        // Write locks using a visually obvious suffix, as we do with
        // first-generation signatures.  This wastes ≤ 2 bytes, but it has the
        // advantage that the corresponding lock-free signature is a
        // _string prefix_ of the with-locks signature.
        enc.flushAndAppend(Base64Encoder::spare[1]);
        for (auto m : data.locks())
            enc.encodeInt(IsoSigData<2, dim>::lockBits, m);
    }

    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintable::length(const IsoSigData<2, dim>& data) {
    // The size will be written in the same way as a first-generation signature.
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and is not explicitly encoded.
        ans = 1;
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        ans = 2 + Base64Encoder::integerWidth(data.size());
    }

    // From here on the encoding is bit-by-bit.
    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    // The constant 8 below includes:
    // - 3 initial bits (11 marker, followed by oriented flag);
    // - an extra +5 since we need to round up when dividing by 6.
    ans += ((8 +
        (intBits * data.adjacentSimplices().size()) +
        data.countFacetBits() +
        (oriented ? IsoSigData<2, dim>::permBits - 1 :
            IsoSigData<2, dim>::permBits) * data.adjacentGluings().size()) / 6);

    if (data.hasLocks()) {
        // The constant 11 below includes:
        // - 6 bits for the hard-coded "lock suffix" character;
        // - an extra +5 again to round up when dividing by 6.
        ans += (IsoSigData<2, dim>::lockBits * data.locks().size() + 11) / 6;
    }

    return ans;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintableLockFree::encode(const IsoSigData<1, dim>& data) {
    Base64Encoder enc;
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
        int width = Base64Encoder::integerWidth(data.size());
        ans = 2 + (1 + data.adjacentSimplices().size()) * width;
    }
    ans += ((data.facetTypes().size() + 2) / 3);
    ans += (data.adjacentGluings().size() * charsPerPerm<dim>);
    return ans;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigPrintableLockFree::encode(const IsoSigData<2, dim>& data) {
    Base64BitEncoder enc;
    enc.reserveChars(length(data));

    // Begin with the size, encoded in a way that is compatible with
    // first-generation signatures.
    enc.encodeSize(data.size());

    // Continue with a bit-by-bit encoding.
    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    // We begin by encoding the bits [11], which can never appear at the
    // beginning of a first-generation signature.  This will allow fromSig()
    // to determine which generation a given base64 signature is.
    enc.encodeInt<unsigned>(2 /* encode two bits */, 3 /* the bits: 11 */);

    enc.encodeBit(oriented);

    using UnsignedPermIndex = typename IsoSigData<2, dim>::UnsignedPermIndex;
    for (auto s : data.adjacentSimplices())
        enc.encodeInt(intBits, s);
    enc.encodeBitmask(data.countFacetBits(), data.facetTypes());
    if (oriented) {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits - 1, g >> 1);
    } else {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits, g);
    }

    return std::move(enc).str();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigPrintableLockFree::length(const IsoSigData<2, dim>& data) {
    // The size will be written in the same way as a first-generation signature.
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and is not explicitly encoded.
        ans = 1;
    } else {
        // We begin with two extra characters: 63 (a marker that the
        // component is large), and the encoding of the integer width.
        ans = 2 + Base64Encoder::integerWidth(data.size());
    }

    // From here on the encoding is bit-by-bit.
    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    // The constant 8 below includes:
    // - 3 initial bits (11 marker, followed by oriented flag);
    // - an extra +5 since we need to round up when dividing by 6.
    ans += ((8 +
        (intBits * data.adjacentSimplices().size()) +
        data.countFacetBits() +
        (oriented ? IsoSigData<2, dim>::permBits - 1 :
            IsoSigData<2, dim>::permBits) * data.adjacentGluings().size()) / 6);

    return ans;
}

template <int dim> requires (supportedDim(dim))
ByteSequence IsoSigBinary::encode(const IsoSigData<2, dim>& data) {
    // We begin by encoding the integer n as follows:
    // - if n < 128, we simply write n in the first byte;
    // - if n ≥ 128, we set the highest-order bit of the first byte, use the
    //   seven lower-order bits to encode the _number_ of bits b required for
    //   any integer in the range [0..n], and then encode n using b bits
    //   beginning at the second byte.
    //
    // This impose the restriction that n < 2^128, but this is wildly more
    // than enough for any triangulation.
    //
    // Following this, we encode two flags:
    // - whether all gluings are orientation-preserving;
    // - whether we need to encode locks.

    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    BitEncoder enc;
    enc.reserveBytes(length(data));

    if (data.size() < 128) {
        enc.encodeInt(8, data.size());
    } else {
        if (intBits > 127)
            throw ImpossibleScenario("IsoSigBinary::encode(): "
                "triangulation has ≥ 2^127 top-dimensional simplices");
        enc.encodeInt(8, static_cast<unsigned>(intBits | 128));
        enc.encodeInt(intBits, data.size());
    }

    enc.encodeBit(oriented);
    enc.encodeBit(data.hasLocks());

    using UnsignedPermIndex = typename IsoSigData<2, dim>::UnsignedPermIndex;
    for (auto s : data.adjacentSimplices())
        enc.encodeInt(intBits, s);
    enc.encodeBitmask(data.countFacetBits(), data.facetTypes());
    if (oriented) {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits - 1, g >> 1);
    } else {
        for (UnsignedPermIndex g : data.adjacentGluings())
            enc.encodeInt(IsoSigData<2, dim>::permBits, g);
    }
    for (auto m : data.locks())
        enc.encodeInt(IsoSigData<2, dim>::lockBits, m);

    return std::move(enc).bytes();
}

template <int dim> requires (supportedDim(dim))
size_t IsoSigBinary::length(const IsoSigData<2, dim>& data) {
    int intBits = bitsRequired(data.size() + 1);
    bool oriented = data.isOriented();

    size_t prefixBits;
    if (data.size() < 128) {
        // The size is written to the first byte, and the integer
        // bitwidth is not encoded separately.
        prefixBits = 8;
    } else {
        // The integer bitwidth (and a marker) is written to the first byte,
        // and the size is written separately beginning at the second byte.
        prefixBits = 8 + intBits;
    }

    return (prefixBits + 2 + intBits * data.adjacentSimplices().size() +
        data.countFacetBits() +
        (oriented ? IsoSigData<2, dim>::permBits - 1 :
            IsoSigData<2, dim>::permBits) * data.adjacentGluings().size() +
        IsoSigData<2, dim>::lockBits * data.locks().size() + 7) / 8;
}

template <int dim> requires (supportedDim(dim))
std::string IsoSigBinary::asString(const ByteSequence& sig) {
    // Get the empty triangulation out of the way first.
    if (sig.empty())
        return IsoSigPrintable::encodeEmpty();

    try {
        // Both IsoSigPrintable and IsoSigBinary encode exactly the same
        // combinatorial information for a second-generation signature; it is
        // just a matter of converting between base64 and bit-packed formats.
        BitDecoder dec(sig.begin(), sig.end());
        Base64BitEncoder enc;
        while (! dec.noMoreBits()) {
            // Re-encode one component of the triangulation at a time.
            size_t nSimp = dec.template decodeInt<size_t>(8);
            int intBits;
            if (nSimp & 128) {
                intBits = static_cast<int>(nSimp ^ 128);
                if (intBits == 0)
                    throw InvalidArgument(
                        "IsoSigBinary::asString(): invalid integer bitwidth");
                nSimp = dec.template decodeInt<size_t>(intBits);
            } else if (nSimp == 0) {
                throw InvalidArgument(
                    "IsoSigBinary::asString(): invalid component size");
            } else {
                intBits = bitsRequired(nSimp + 1);
            }

            bool oriented = dec.decodeBit();
            bool hasLocks = dec.decodeBit();

            enc.encodeSize(nSimp);
            enc.encodeInt<unsigned>(2 /* two bits */, 3 /* the bits: 11 */);
            enc.encodeBit(oriented);

            size_t nBdry = 0;
            size_t nDest = 2 * (nSimp - 1);
            while (nDest < nSimp * (dim + 1)) {
                size_t dest = dec.template decodeInt<size_t>(intBits);
                enc.encodeInt(intBits, dest);
                if (dest == nSimp) {
                    ++nBdry;
                    ++nDest;
                } else {
                    nDest += 2;
                }
            }

            size_t nFacets = (nDest + nBdry) / 2;
            enc.encodeBitmask(nFacets, dec.decodeBitmask(nFacets));

            using UnsignedPermIndex =
                typename IsoSigData<2, dim>::UnsignedPermIndex;
            int w = (oriented ? IsoSigData<2, dim>::permBits - 1 :
                IsoSigData<2, dim>::permBits);
            for (size_t i = 0; i < nFacets - nBdry + 1 - nSimp; ++i)
                enc.encodeInt(w, dec.template decodeInt<UnsignedPermIndex>(w));

            if (hasLocks) {
                enc.flushAndAppend(Base64Encoder::spare[1]);
                for (size_t i = 0; i < nSimp; ++i)
                    enc.encodeInt(IsoSigData<2, dim>::lockBits,
                        dec.template decodeInt<typename Simplex<dim>::LockMask>(
                        IsoSigData<2, dim>::lockBits));
            }

            dec.flushByte();
            enc.flushChar();
        }
        return std::move(enc).str();
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by BitDecoder.
        throw InvalidArgument("IsoSigBinary::asString(): "
            "incomplete or invalid encoding");
    }
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
template <CharIterator Iterator>
requires std::bidirectional_iterator<Iterator>
void TriangulationBase<dim>::fillComponentFromSig1(
        const FixedArray<Simplex<dim>*>& simplices,
        Base64Decoder<Iterator>& decoder, int intWidth) {
    FixedArray<uint8_t> facetAction((dim+1) * simplices.size() + 2);
    size_t nFacets = 0;
    auto facetPos = facetAction.begin();
    size_t nJoins = 0;

    for ( ; nFacets < (dim+1) * simplices.size(); facetPos += 3) {
        decoder.decodeTrits(facetPos);
        for (int j = 0; j < 3; ++j) {
            // If we're already finished, make sure the leftover trits
            // are zero.
            if (nFacets == (dim+1) * simplices.size()) {
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
            if (nFacets > (dim+1) * simplices.size()) {
                throw InvalidArgument("fromSig(): facet actions "
                    "do not match triangulation size");
            }
        }
    }

    auto joinDest = decoder.template decodeInts<size_t>(nJoins, intWidth);
    auto joinGluing =
        decoder.template decodeInts<typename Perm<dim+1>::Index>(
        nJoins, IsoSigPrintable::charsPerPerm<dim>);

    // This ends the gluings for this component!
    //
    // We still need to read facet/simplex locks, if they are present.
    // We will do this after constructing the triangulation.

    facetPos = facetAction.begin();
    size_t nextUnused = 1;
    size_t joinPos = 0;
    Perm<dim+1> gluing;
    for (size_t pos = 0; pos < simplices.size(); ++pos)
        for (int j = 0; j <= dim; ++j) {
            // Already glued from the other side:
            if (simplices[pos]->adjacentSimplex(j))
                continue;

            if (*facetPos == 0) {
                // Boundary facet.
            } else if (*facetPos == 1) {
                // Join to new simplex.
                if (nextUnused >= simplices.size()) {
                    throw InvalidArgument(
                        "fromSig(): gluing to non-existent simplex");
                }
                simplices[pos]->join(j, simplices[nextUnused++], {});
            } else {
                // Join to existing simplex.
                if (joinGluing[joinPos] >= Perm<dim+1>::nPerms ||
                        joinGluing[joinPos] < 0) {
                    throw InvalidArgument(
                        "fromSig(): invalid gluing permutation");
                }
                gluing = Perm<dim+1>::orderedSn[joinGluing[joinPos]];
                if (joinDest[joinPos] >= nextUnused ||
                        simplices[joinDest[joinPos]]->adjacentSimplex(
                        gluing[j])) {
                    throw InvalidArgument(
                        "fromSig(): invalid gluing destination");
                }
                simplices[pos]->join(j, simplices[joinDest[joinPos]], gluing);
                ++joinPos;
            }

            ++facetPos;
        }

    // Read simplex/facet locks, if these are present.
    if (decoder.peek() == Base64Encoder::spare[1]) {
        decoder.skip();

        using LockMask = typename Simplex<dim>::LockMask;
        // Each lock mask encodes dim+2 bits.
        static constexpr LockMask maskChars = (dim + 7) / 6;
        auto lockMasks = decoder.template decodeInts<LockMask>(
            simplices.size(), maskChars);

        // We will set lock masks directly instead of using lock()
        // functions.  This means we don't get change spans (but that
        // is fine since we have computed nothing about the
        // triangulation and nobody else has a reference to it yet).
        // It also means that we need to run our own sanity checks,
        // which we will do shortly.
        for (size_t i = 0; i < simplices.size(); ++i) {
            if (lockMasks[i] >> (dim + 2) != 0)
                throw InvalidArgument(
                    "fromSig(): invalid lock mask");
            simplices[i]->locks_ = lockMasks[i];
        }

        verifyLockConsistency(simplices);
    }
}

template <int dim> requires (supportedDim(dim))
template <CharIterator Iterator>
requires std::bidirectional_iterator<Iterator>
void TriangulationBase<dim>::fillComponentFromSig2(
        const FixedArray<Simplex<dim>*>& simplices,
        Base64BitDecoder<Iterator>& decoder) {
    int intBits = bitsRequired(simplices.size() + 1);

    if (decoder.template decodeInt<unsigned>(2) != 3)
        throw InvalidArgument("fromSig(): missing second-generation marker");
    bool oriented = decoder.decodeBit();

    size_t nBdry = 0;
    size_t nDest = 2 * (simplices.size() - 1);
    std::vector<size_t> adjSimplex;
    adjSimplex.reserve((simplices.size() * (dim + 1) + 1) / 2); // a lower bound
    while (nDest < simplices.size() * (dim + 1)) {
        size_t dest = decoder.template decodeInt<size_t>(intBits);
        if (dest == simplices.size()) {
            ++nBdry;
            ++nDest;
        } else {
            nDest += 2;
        }
        adjSimplex.push_back(dest);
    }

    size_t nFacets = (nDest + nBdry) / 2;
    Bitmask facetType = decoder.decodeBitmask(nFacets);

    using UnsignedPermIndex = typename IsoSigData<2, dim>::UnsignedPermIndex;
    FixedArray<UnsignedPermIndex> adjGluing(
        nFacets - nBdry + 1 - simplices.size());
    if (oriented) {
        for (auto& index : adjGluing)
            index = decoder.template decodeInt<UnsignedPermIndex>(
                IsoSigData<2, dim>::permBits - 1) * 2 + 1;
    } else {
        for (auto& index : adjGluing)
            index = decoder.template decodeInt<UnsignedPermIndex>(
                IsoSigData<2, dim>::permBits);
    }

    // This ends the gluings for this component!
    //
    // We still need to read facet/simplex locks, if they are present.
    // We will do this after constructing the triangulation.

    size_t facetPos = nFacets - 1;
    size_t nextUnused = 1;
    auto destPos = adjSimplex.begin();
    auto gluingPos = adjGluing.begin();
    for (auto s : simplices)
        for (int f = 0; f <= dim; ++f) {
            // Already glued from the other side:
            if (s->adjacentSimplex(f))
                continue;

            if (facetType.get(facetPos--)) {
                size_t dest = *destPos++;

                if (dest != simplices.size()) {
                    // A non-boundary facet, joined to a simplex we
                    // have already seen.
                    UnsignedPermIndex index = *gluingPos++;
                    if (index >= Perm<dim+1>::nPerms)
                        throw InvalidArgument(
                            "fromSig(): invalid gluing permutation");
                    Perm<dim+1> gluing = Perm<dim+1>::Sn[index];

                    if (dest >= nextUnused ||
                            simplices[dest]->adjacentSimplex(gluing[f])) {
                        throw InvalidArgument(
                            "fromSig(): invalid gluing destination");
                    }
                    s->join(f, simplices[dest], gluing);
                }
            } else {
                // Join to the next new simplex.
                if (nextUnused >= simplices.size())
                    throw InvalidArgument(
                        "fromSig(): gluing to non-existent simplex");
                s->join(f, simplices[nextUnused++], { 0, 1 });
            }
        }

    decoder.flushChar();
    char next = decoder.peek();

    // Read simplex/facet locks, if these are present.
    if (next == Base64Encoder::spare[1]) {
        decoder.skip();

        // We will set lock masks directly instead of using lock() functions;
        // see the first-generation decoding procedure for further explanation.
        for (auto s : simplices)
            s->locks_ =
                decoder.template decodeInt<typename Simplex<dim>::LockMask>(
                IsoSigData<2, dim>::lockBits);

        verifyLockConsistency(simplices);
        decoder.flushChar();
    }
}

template <int dim> requires (supportedDim(dim))
void TriangulationBase<dim>::verifyLockConsistency(
        const FixedArray<Simplex<dim>*>& simplices) {
    for (auto s : simplices)
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

template <int dim> requires (supportedDim(dim))
Triangulation<dim> TriangulationBase<dim>::fromSig(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace

    Triangulation<dim> ans;

    try {
        // Read the size of the first component.
        std::pair<size_t, int> sizeAndWidth = dec.decodeSize();
        if (sizeAndWidth.first == 0) {
            // This is the signature for the empty triangulation.
            if (! dec.done())
                throw InvalidArgument("fromSig(): "
                    "unexpected additional characters");
            return ans;
        }

        // Look at the next character: this will tell us whether we have a
        // first-generation or second-generation signature.
        char next = dec.peek();
        if (next == 0)
            throw InvalidArgument("fromSig(): incomplete base64 encoding");
        if ((dec.decode(next) & 3) == 3) {
            // This is a second-generation signature: we see the marker bits 11.
            // Switch to a bitwise decoder from here on.
            auto bitwise = dec.unreadBitDecoder();
            while (true) {
                // Finish decoding this connected component.
                FixedArray<Simplex<dim>*> simp(sizeAndWidth.first);
                for (auto& s : simp)
                    s = ans.newSimplex();

                fillComponentFromSig2(simp, bitwise);

                if (bitwise.noMoreBits())
                    return ans;

                // Prepare to read another component.
                sizeAndWidth.first = bitwise.decodeSize();
                if (sizeAndWidth.first == 0)
                    throw InvalidArgument("fromSig(): invalid component size");
            }
        } else {
            // This is a first-generation signature: the character we peeked at
            // encodes one or more trits, and a trit cannot take the value 11.
            while (true) {
                // Finish decoding this connected component.
                FixedArray<Simplex<dim>*> simp(sizeAndWidth.first);
                for (auto& s : simp)
                    s = ans.newSimplex();

                fillComponentFromSig1(simp, dec, sizeAndWidth.second);

                if (dec.done())
                    return ans;

                // Prepare to read another component.
                sizeAndWidth = dec.decodeSize();
                if (sizeAndWidth.first == 0)
                    throw InvalidArgument("fromSig(): invalid component size");
            }
        }
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by the decoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid base64 encoding");
    }
}

template <int dim> requires (supportedDim(dim))
size_t TriangulationBase<dim>::isoSigComponentSize(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace
    try {
        return dec.decodeSize().first;
    } catch (const InvalidInput&) {
        throw InvalidArgument("isoSigComponentSize(): invalid signature");
    }
}

template <int dim> requires (supportedDim(dim))
Triangulation<dim> TriangulationBase<dim>::fromSig(const ByteSequence& sig) {
    BitDecoder dec(sig.begin(), sig.end());

    try {
        Triangulation<dim> ans;

        while (! dec.noMoreBits()) {
            // Read one component at a time.

            size_t nSimp = dec.template decodeInt<size_t>(8);
            int intBits;
            if (nSimp & 128) {
                intBits = static_cast<int>(nSimp ^ 128);
                if (intBits == 0)
                    throw InvalidArgument(
                        "fromSig(): invalid integer bitwidth");
                nSimp = dec.template decodeInt<size_t>(intBits);
            } else if (nSimp == 0) {
                throw InvalidArgument("fromSig(): invalid component size");
            } else {
                intBits = bitsRequired(nSimp + 1);
            }

            bool oriented = dec.decodeBit();
            bool hasLocks = dec.decodeBit();

            size_t nBdry = 0;
            size_t nDest = 2 * (nSimp - 1);
            std::vector<size_t> adjSimplex;
            adjSimplex.reserve((nSimp * (dim + 1) + 1) / 2); // a lower bound
            while (nDest < nSimp * (dim + 1)) {
                size_t dest = dec.template decodeInt<size_t>(intBits);
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

            using UnsignedPermIndex =
                typename IsoSigData<2, dim>::UnsignedPermIndex;
            FixedArray<UnsignedPermIndex> adjGluing(
                nFacets - nBdry + 1 - nSimp);
            if (oriented) {
                for (auto& index : adjGluing)
                    index = dec.template decodeInt<UnsignedPermIndex>(
                        IsoSigData<2, dim>::permBits - 1) * 2 + 1;
            } else {
                for (auto& index : adjGluing)
                    index = dec.template decodeInt<UnsignedPermIndex>(
                        IsoSigData<2, dim>::permBits);
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
                            UnsignedPermIndex index = *gluingPos++;
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

            if (hasLocks) {
                // We set lock masks directly instead of using lock() functions;
                // see the first-gen decoding procedure for further explanation.
                for (auto s : simp)
                    s->locks_ =
                        dec.template decodeInt<typename Simplex<dim>::LockMask>(
                        IsoSigData<2, dim>::lockBits);

                verifyLockConsistency(simp);
            }

            dec.flushByte();
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by BitDecoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid binary encoding");
    }
}

} } // namespace regina::detail

#endif
