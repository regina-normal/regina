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

/*! \file subcomplex/conical.h
 *  \brief Deals with conical subcomplexes in a 4-dimensional triangulation.
 */

#ifndef __REGINA_CONICAL_H
#ifndef __DOXYGEN
#define __REGINA_CONICAL_H
#endif

#include <utility>
#include <vector>
#include "regina-core.h"
#include "subcomplex/standardsubcomplex.h"
#include "triangulation/dim4.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a 4-dimensional subcomplex that is a cone over a 3-dimensional
 * triangulation.
 *
 * This class is deliberately lightweight.  Recognition works directly in the
 * local star of a 4-dimensional vertex, using the same induced face-pairing
 * formula as Vertex<4>::buildLink(), but without constructing a separate
 * Triangulation<3> object.
 *
 * A conical subcomplex consists of a common apex vertex and a collection of
 * pentachora incident to that vertex.  These pentachora correspond to
 * tetrahedra in the link of the apex vertex, i.e., in the 3-dimensional base
 * of the cone.
 *
 * At present this class can recognise:
 *
 * - the full cone over the complete link of a vertex;
 * - cones over layered solid tori, recognised inside this virtual link using
 *   the local layered-solid-torus algorithm.
 *
 * \ingroup subcomplex
 */
class Conical : public StandardSubcomplex<4> {
    public:
        /**
         * Describes what is currently known about the 3-dimensional base.
         */
        enum class BaseType {
            /**
             * No special structure has been recognised in the base.
             */
            Unknown,
            /**
             * The base is a layered solid torus.
             */
            LayeredSolidTorus
        };

    private:
        Vertex<4>* apex_;
            /**< The apex vertex of the cone. */
        std::vector<Pentachoron<4>*> pent_;
            /**< The pentachora in this conical subcomplex. */
        BaseType baseType_;
            /**< The type of 3-dimensional base that was recognised. */

        size_t baseSize_;
            /**< For layered solid tori, the number of tetrahedra in the base. */
        Pentachoron<4>* basePent_;
            /**< For layered solid tori, the base pentachoron. */
        Pentachoron<4>* topPent_;
            /**< For layered solid tori, the top-level pentachoron. */
        long meridinalCuts_[3];
            /**< For layered solid tori, the meridinal cuts of the base. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        Conical(const Conical&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        Conical& operator = (const Conical&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(Conical& other) noexcept;

        /**
         * Returns the apex vertex of this cone.
         *
         * \return the apex vertex.
         */
        Vertex<4>* apex() const;

        /**
         * Returns the number of pentachora in this conical subcomplex.
         *
         * \return the number of pentachora.
         */
        size_t size() const;
        /**
         * Returns one of the pentachora in this conical subcomplex.
         *
         * \param index specifies which pentachoron to return; this must be
         * between 0 and size()-1 inclusive.
         * \return the requested pentachoron.
         */
        Pentachoron<4>* pentachoron(size_t index) const;

        /**
         * Returns what is currently known about the 3-dimensional base.
         *
         * \return the recognised base type.
         */
        BaseType baseType() const;
        /**
         * Returns the name of the recognised base type.
         *
         * \return a human-readable name for the base type.
         */
        const char* baseTypeName() const;

        /**
         * Returns the number of tetrahedra in the layered-solid-torus base.
         *
         * \pre baseType() is BaseType::LayeredSolidTorus.
         *
         * \return the number of tetrahedra.
         */
        size_t layeredSolidTorusSize() const;
        /**
         * Returns the pentachoron corresponding to the base tetrahedron of
         * the layered-solid-torus base.
         *
         * \pre baseType() is BaseType::LayeredSolidTorus.
         *
         * \return the base pentachoron.
         */
        Pentachoron<4>* layeredSolidTorusBase() const;
        /**
         * Returns the pentachoron corresponding to the top-level tetrahedron
         * of the layered-solid-torus base.
         *
         * \pre baseType() is BaseType::LayeredSolidTorus.
         *
         * \return the top-level pentachoron.
         */
        Pentachoron<4>* layeredSolidTorusTop() const;
        /**
         * Returns one of the meridinal cut numbers for the
         * layered-solid-torus base.
         *
         * \pre baseType() is BaseType::LayeredSolidTorus.
         *
         * \param index specifies which cut number to return; this must be
         * 0, 1 or 2.
         * \return the requested meridinal cut number.
         */
        long meridinalCuts(int index) const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a conical subcomplex.
         *
         * Specifically, two conical subcomplexes compare as equal if and only
         * if they use the same numbered apex vertex, the same numbered
         * pentachora in the same order, and the same recognised base type.
         * In particular, it is still meaningful to compare conical
         * subcomplexes within different triangulations.
         *
         * \param other the conical subcomplex to compare with this.
         * \return \c true if and only if this and the given object represent
         * the same specific presentation of a conical subcomplex.
         */
        bool operator == (const Conical& other) const;

        /**
         * Recognises the full cone over the complete link of the given vertex.
         *
         * This returns the entire local star of the vertex, provided each
         * selected pentachoron contains the vertex in only one local position.
         *
         * \param apex the candidate apex vertex.
         * \return details of the conical subcomplex, or \c null if the local
         * star is not handled by this lightweight model.
         */
        static std::unique_ptr<Conical> recognise(Vertex<4>* apex);

        /**
         * Looks for a cone over a layered solid torus in the link of the
         * given vertex.
         *
         * This searches all virtual tetrahedra in the vertex link as possible
         * base tetrahedra, and returns the first layered solid torus that it
         * finds.
         *
         * \param apex the candidate apex vertex.
         * \return details of the conical subcomplex, or \c null if no cone
         * over a layered solid torus was found.
         */
        static std::unique_ptr<Conical> recogniseLayeredSolidTorus(
            Vertex<4>* apex);
        /**
         * Looks for a cone over a layered solid torus in the link of the
         * given vertex, using the given virtual tetrahedron as the base.
         *
         * The virtual tetrahedra in the vertex link are numbered according to
         * the ordering of apex->embeddings().
         *
         * \param apex the candidate apex vertex.
         * \param baseTet the virtual tetrahedron to use as the base of the
         * layered solid torus.
         * \return details of the conical subcomplex, or \c null if no such
         * layered solid torus was found.
         */
        static std::unique_ptr<Conical> recogniseLayeredSolidTorus(
            Vertex<4>* apex, size_t baseTet);

        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given internal data.
         */
        Conical(Vertex<4>* apex, std::vector<Pentachoron<4>*>&& pent,
            BaseType baseType, size_t baseSize, Pentachoron<4>* basePent,
            Pentachoron<4>* topPent, long cuts0, long cuts1, long cuts2);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls Conical::swap(); it is provided so that
 * Conical meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(Conical& a, Conical& b) noexcept;

// Inline functions for Conical

inline Conical::Conical(Vertex<4>* apex,
        std::vector<Pentachoron<4>*>&& pent, BaseType baseType,
        size_t baseSize, Pentachoron<4>* basePent, Pentachoron<4>* topPent,
        long cuts0, long cuts1, long cuts2) :
        apex_(apex), pent_(std::move(pent)), baseType_(baseType),
        baseSize_(baseSize), basePent_(basePent), topPent_(topPent),
        meridinalCuts_{cuts0, cuts1, cuts2} {
}

inline void Conical::swap(Conical& other) noexcept {
    std::swap(apex_, other.apex_);
    std::swap(pent_, other.pent_);
    std::swap(baseType_, other.baseType_);
    std::swap(baseSize_, other.baseSize_);
    std::swap(basePent_, other.basePent_);
    std::swap(topPent_, other.topPent_);
    std::swap(meridinalCuts_, other.meridinalCuts_);
}

inline Vertex<4>* Conical::apex() const {
    return apex_;
}

inline size_t Conical::size() const {
    return pent_.size();
}

inline Pentachoron<4>* Conical::pentachoron(size_t index) const {
    return pent_[index];
}

inline Conical::BaseType Conical::baseType() const {
    return baseType_;
}

inline const char* Conical::baseTypeName() const {
    return baseType_ == BaseType::LayeredSolidTorus ?
        "layered solid torus" : "unknown";
}

inline size_t Conical::layeredSolidTorusSize() const {
    return baseSize_;
}

inline Pentachoron<4>* Conical::layeredSolidTorusBase() const {
    return basePent_;
}

inline Pentachoron<4>* Conical::layeredSolidTorusTop() const {
    return topPent_;
}

inline long Conical::meridinalCuts(int index) const {
    return meridinalCuts_[index];
}

inline bool Conical::operator == (const Conical& other) const {
    if (apex_->index() != other.apex_->index() ||
            baseType_ != other.baseType_ || pent_.size() != other.pent_.size())
        return false;
    for (size_t i = 0; i < pent_.size(); ++i)
        if (pent_[i]->index() != other.pent_[i]->index())
            return false;
    return true;
}

inline std::ostream& Conical::writeName(std::ostream& out) const {
    if (baseType_ == BaseType::LayeredSolidTorus)
        return out << "Cone over layered solid torus";
    else
        return out << "Cone";
}

inline std::ostream& Conical::writeTeXName(std::ostream& out) const {
    if (baseType_ == BaseType::LayeredSolidTorus)
        return out << "C(\\mathrm{LST})";
    else
        return out << "C";
}

inline void Conical::writeTextShort(std::ostream& out) const {
    out << name() << ", apex vertex " << apex_->index() << ", "
        << pent_.size() << (pent_.size() == 1 ? " pentachoron" :
            " pentachora");
}

inline void swap(Conical& a, Conical& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
