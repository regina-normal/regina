
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file surface/surfacefilter.h
 *  \brief Contains a packet that filters through normal surfaces.
 */

#ifndef __REGINA_SURFACEFILTER_H
#ifndef __DOXYGEN
#define __REGINA_SURFACEFILTER_H
#endif

#include <set>
#include "regina-core.h"
#include "maths/integer.h"
#include "packet/packet.h"
#include "surface/surfacefiltertype.h"
#include "utilities/boolset.h"

namespace regina {

class NormalSurface;
class SurfaceFilter;
class SurfaceFilterCombination;
class SurfaceFilterProperties;

/**
 * Defines various constants, types and virtual functions for a
 * descendant class of SurfaceFilter.
 *
 * Every descendant class of SurfaceFilter \a must include
 * REGINA_SURFACE_FILTER at the beginning of the class definition.
 *
 * This macro provides the class with:
 *
 * - a compile-time constant \a filterTypeID, which is equal to the
 *   corresponding SurfaceFilterType constant;
 * - declarations and implementations of the virtual functions
 *   SurfaceFilter::filterType() and SurfaceFilter::filterTypeName();
 *
 * \param id the corresponding SurfaceFilterType constant.
 * \param name a human-readable name for this filter type.
 *
 * \ingroup surfaces
 */
#define REGINA_SURFACE_FILTER(id, name) \
    public: \
        static constexpr const SurfaceFilterType filterTypeID = id; \
        inline SurfaceFilterType filterType() const override { \
            return id; \
        } \
        inline std::string filterTypeName() const override { \
            return name; \
        }

/**
 * A packet that accepts or rejects normal surfaces.
 * Different subclasses of SurfaceFilter represent different filtering
 * methods.
 *
 * When deriving classes from SurfaceFilter:
 *
 * - Add a new filter constant to the SurfaceFilterType enum;
 *
 * - Create a new subclass \a C of SurfaceFilter, which begins with the
 *   REGINA_SURFACE_FILTER macro;
 *
 * - Perform all tasks required for this new innate packet type \a C, as
 *   outlined in the Packet class documentation;
 *
 * - Override the virtual function writeTextShort(), as well as all pure virtual
 *   functions from both the Packet and SurfaceFilter base classes (except
 *   for those already provided by REGINA_PACKET and REGINA_SURFACE_FILTER).
 *
 * Like all packet types, Regina's filter types do not support C++ move
 * semantics, since this would interfere with the structure of the packet tree.
 * They do support copy construction, copy assignment and swaps, but only in
 * the derived filter classes (e.g., you cannot assign from the polymorphic
 * base class SurfaceFilter).  Moreover, these operations only copy/swap the
 * filter content, not the packet infrastructure (e.g., they do not touch
 * packet labels, or the packet tree, or event listeners).
 *
 * \ingroup surfaces
 */
class SurfaceFilter : public Packet {
    REGINA_PACKET(PACKET_SURFACEFILTER, "Surface filter")

    public:
        /**
         * Decides whether or not the given normal surface is accepted by this
         * filter.
         *
         * The default implementation simply returns \c true.
         *
         * \param surface the normal surface under investigation.
         * \return \c true if and only if the given surface is accepted
         * by this filter.
         */
        virtual bool accept(const NormalSurface& surface) const = 0;

        /**
         * Returns the unique integer ID corresponding to the filtering
         * method that is this particular subclass of SurfaceFilter.
         *
         * \return the unique integer filtering method ID.
         */
        virtual SurfaceFilterType filterType() const = 0;
        /**
         * Returns a string description of the filtering method that is
         * this particular subclass of SurfaceFilter.
         *
         * \return a string description of this filtering method.
         */
        virtual std::string filterTypeName() const = 0;

    protected:
        /**
         * Default constructor.
         */
        SurfaceFilter() = default;
        /**
         * Copy constructor that does not actually copy anything.
         *
         * This is provided so that derived classes can use it implicitly
         * in their own copy constructors.
         */
        SurfaceFilter(const SurfaceFilter&) = default;
};

/**
 * A normal surface filter that simply combines other filters.
 * This filter will combine, using boolean \a and or \a or, all of the
 * filters that are immediate children of this packet.  This packet may
 * have children that are not normal surface filters; such children will
 * simply be ignored.
 *
 * If there are no immediate child filters, a normal surface will be
 * accepted if this is an \a and filter and rejected if this is an \a or
 * filter.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup surfaces
 */
class SurfaceFilterCombination : public SurfaceFilter {
    REGINA_SURFACE_FILTER(NS_FILTER_COMBINATION, "Combination filter")

    private:
        bool usesAnd_;
            /**< \c true if children are combined using boolean \a and, or
                 \c false if children are combined using boolean \a or. */

    public:
        /**
         * Creates a new surface filter that accepts all normal surfaces.
         * This will be an \a and filter.
         */
        SurfaceFilterCombination();

        /**
         * Creates a new copy of the given filter.
         *
         * Like all packet types, this only copies the filter content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         */
        SurfaceFilterCombination(const SurfaceFilterCombination&) = default;

        /**
         * Sets this to be a copy of the given filter.
         *
         * Like all packet types, this only copies the filter content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * \param src the filter whose contents should be copied.
         * \return a reference to this filter.
         */
        SurfaceFilterCombination& operator = (
            const SurfaceFilterCombination& src);

        /**
         * Swaps the contents of this and the given filter.
         *
         * Like all packet types, this only swaps the filter content, not
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         *
         * \param other the filter whose contents should be swapped with this.
         */
        void swap(SurfaceFilterCombination& other);

        /**
         * Determines whether this is an \a and or an \a or combination.
         *
         * \return \c true if this is an \a and combination, or \c false
         * if this is an \a or combination.
         */
        bool usesAnd() const;
        /**
         * Sets whether this is an \a and or an \a or combination.
         *
         * \param value \c true if this is to be an \a and combination,
         * or \c false if this is to be an \a or combination.
         */
        void setUsesAnd(bool value);

        /**
         * Determines if this and the given filter use the same boolean
         * operation.
         *
         * \param other the filter to compare with this.
         * \return \c true if and only if this and the given filter
         * use the same boolean operation.
         */
        bool operator == (const SurfaceFilterCombination& other) const;

        /**
         * Determines if this and the given filter do not use the same
         * boolean operation.
         *
         * \param other the filter to compare with this.
         * \return \c true if and only if this and the given filter
         * use different boolean operations.
         */
        bool operator != (const SurfaceFilterCombination& other) const;

        bool accept(const NormalSurface& surface) const override;
        void writeTextShort(std::ostream& o) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given combination filters.
 *
 * This global routine simply calls SurfaceFilterCombination::swap(); it is
 * provided so that SurfaceFilterCombination meets the C++ Swappable
 * requirements.
 *
 * \param a the first filter whose contents should be swapped.
 * \param b the second filter whose contents should be swapped.
 *
 * \ingroup surfaces
 */
void swap(SurfaceFilterCombination& a, SurfaceFilterCombination& b);

/**
 * A normal surface filter that filters by basic properties of the normal
 * surface.
 *
 * If a property of the surface (such as Euler characteristic or
 * orientability) cannot be determined, the surface will pass any test
 * based on that particular property.  For instance, say a surface is
 * required to be both orientable and compact, and say that orientability
 * cannot be determined.  Then the surface will be accepted solely on the
 * basis of whether or not it is compact.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup surfaces
 */
class SurfaceFilterProperties : public SurfaceFilter {
    REGINA_SURFACE_FILTER(NS_FILTER_PROPERTIES, "Filter by basic properties")

    private:
        std::set<LargeInteger> eulerChar_;
            /**< The set of allowable Euler characteristics.  An empty
                 set signifies that any Euler characteristic is allowed. */
        BoolSet orientability_;
            /**< The set of allowable orientability properties. */
        BoolSet compactness_;
            /**< The set of allowable compactness properties. */
        BoolSet realBoundary_;
            /**< The set of allowable has-real-boundary properties. */

    public:
        /**
         * Creates a new surface filter that accepts all normal surfaces.
         */
        SurfaceFilterProperties();

        /**
         * Creates a new copy of the given filter.
         *
         * Like all packet types, this only copies the filter content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         */
        SurfaceFilterProperties(const SurfaceFilterProperties&) = default;

        /**
         * Sets this to be a copy of the given filter.
         *
         * Like all packet types, this only copies the filter content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * \param src the filter whose contents should be copied.
         * \return a reference to this filter.
         */
        SurfaceFilterProperties& operator = (
            const SurfaceFilterProperties& src);

        /**
         * Swaps the contents of this and the given filter.
         *
         * Like all packet types, this only swaps the filter content, not
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         *
         * \param other the filter whose contents should be swapped with this.
         */
        void swap(SurfaceFilterProperties& other);

        /**
         * Returns the set of allowable Euler characteristics.  Any
         * surface whose Euler characteristic is not in this set will not
         * be accepted by this filter.  The set will be given in
         * ascending order with no element repeated.
         *
         * If this set is empty, all Euler characteristics will be
         * accepted.
         *
         * \return the set of allowable Euler characteristics.
         */
        const std::set<LargeInteger>& eulerChars() const;
        /**
         * Returns the number of allowable Euler characteristics.
         * See eulerChars() for further details.
         *
         * \return the number of allowable Euler characteristics.
         */
        size_t countEulerChars() const;
        /**
         * Returns the allowable Euler characteristic at the given index
         * in the set.  See eulerChars() for further details.
         *
         * \param index the index in the set of allowable Euler
         * characteristics; this must be between 0 and countEulerChars()-1
         * inclusive.
         * \return the requested allowable Euler characteristic.
         */
        LargeInteger eulerChar(size_t index) const;
        /**
         * Returns the set of allowable orientabilities.  Note that this
         * is a subset of `{ true, false }`.
         * Any surface whose orientability is not in this set will not be
         * accepted by this filter.
         *
         * \return the set of allowable orientabilities.
         */
        BoolSet orientability() const;
        /**
         * Returns the set of allowable compactness properties.
         * Note that this is a subset of `{ true, false }`.
         * Any surface whose compactness property is not in this set will
         * not be accepted by this filter.
         *
         * \return the set of allowable compactness properties.
         */
        BoolSet compactness() const;
        /**
         * Returns the set of allowable has-real-boundary properties.
         * Note that this is a subset of `{ true, false }`.
         * Any surface whose has-real-boundary property is not in this set
         * will not be accepted by this filter.
         *
         * \return the set of allowable has-real-boundary properties.
         */
        BoolSet realBoundary() const;

        /**
         * Sets the allowable Euler characteristics to be all integers
         * in the given iterator range.  See eulerChars() for further details.
         *
         * \python Instead of a pair of iterators, the set of
         * allowable Euler characteristic should be passed as a Python list.
         *
         * \tparam Iterator an iterator type that, when dereferenced,
         * can be assigned to a LargeInteger.
         *
         * \param beginEuler the beginning of an iterator range that
         * gives the new set of allowable Euler characteristics.
         * \param endEuler the end of an iterator range (i.e., an iterator
         * past the end of the list) that gives the new set of allowable
         * Euler characteristics.
         */
        template <typename Iterator>
        void setEulerChars(Iterator beginEuler, Iterator endEuler);

        /**
         * Adds the given Euler characteristic to the set of allowable
         * Euler characteristics.  See eulerChars() for further details.
         *
         * \param ec the new allowable Euler characteristic.
         */
        void addEulerChar(const LargeInteger& ec);
        /**
         * Removes the given Euler characteristic from the set of allowable
         * Euler characteristics.  See eulerChars() for further details.
         *
         * Note that if the allowable set is completely emptied, this
         * filter will allow <i>any</i> Euler characteristic to pass.
         *
         * \pre The given Euler characteristic is currently in the
         * allowable set.
         *
         * \param ec the allowable Euler characteristic to remove.
         */
        void removeEulerChar(const LargeInteger& ec);
        /**
         * Empties the set of allowable Euler characteristics.  See
         * eulerChars() for further details.
         *
         * Note that this will mean that this filter will allow
         * <i>any</i> Euler characteristic to pass.
         */
        void removeAllEulerChars();
        /**
         * Sets the set of allowable orientabilities.
         * See orientability() for further details.
         *
         * \param value the new set of allowable orientabilities.
         */
        void setOrientability(BoolSet value);
        /**
         * Sets the set of allowable compactness properties.
         * See compactness() for further details.
         *
         * \param value the new set of allowable compactness properties.
         */
        void setCompactness(BoolSet value);
        /**
         * Sets the set of allowable has-real-boundary properties.
         * See realBoundary() for further details.
         *
         * \param value the new set of allowable has-real-boundary
         * properties.
         */
        void setRealBoundary(BoolSet value);

        /**
         * Determines if this and the given filter are configured to
         * filter on the same set of constraints.
         *
         * Even if both filters are designed to reject _every_ surface
         * by having an empty BoolSet for one of the boolean conditions,
         * they will still not compare as equal unless they use an empty
         * BoolSet for the _same_ boolean condition(s).  In other words,
         * this test compares the precise configurations of the filters,
         * not their deduced behaviour.
         *
         * \param other the filter to compare with this.
         * \return \c true if and only if this and the given filters are
         * identical.
         */
        bool operator == (const SurfaceFilterProperties& other) const;

        /**
         * Determines if this and the given filter are configured to
         * filter on the different sets of constraints.
         *
         * Even if both filters are designed to reject _every_ surface
         * by having an empty BoolSet for one of the boolean conditions,
         * they will still compare as different unless they use an empty
         * BoolSet for the _same_ boolean condition(s).  In other words,
         * this test compares the precise configurations of the filters,
         * not their deduced behaviour.
         *
         * \param other the filter to compare with this.
         * \return \c true if and only if this and the given filters are
         * not identical.
         */
        bool operator != (const SurfaceFilterProperties& other) const;

        bool accept(const NormalSurface& surface) const override;
        void writeTextShort(std::ostream& o) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given property-based filters.
 *
 * This global routine simply calls SurfaceFilterProperties::swap(); it is
 * provided so that SurfaceFilterProperties meets the C++ Swappable
 * requirements.
 *
 * \param a the first filter whose contents should be swapped.
 * \param b the second filter whose contents should be swapped.
 *
 * \ingroup surfaces
 */
void swap(SurfaceFilterProperties& a, SurfaceFilterProperties& b);

// Inline functions for SurfaceFilterCombination

inline SurfaceFilterCombination::SurfaceFilterCombination() : usesAnd_(true) {
}

inline SurfaceFilterCombination& SurfaceFilterCombination::operator = (
        const SurfaceFilterCombination& src) {
    ChangeEventSpan span(*this);
    usesAnd_ = src.usesAnd_;
    return *this;
}

inline void SurfaceFilterCombination::swap(SurfaceFilterCombination& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);
    std::swap(usesAnd_, other.usesAnd_);
}

inline bool SurfaceFilterCombination::usesAnd() const {
    return usesAnd_;
}
inline void SurfaceFilterCombination::setUsesAnd(bool value) {
    if (usesAnd_ != value) {
        ChangeEventSpan span(*this);
        usesAnd_ = value;
    }
}

inline bool SurfaceFilterCombination::operator == (
        const SurfaceFilterCombination& other) const {
    return usesAnd_ == other.usesAnd_;
}

inline bool SurfaceFilterCombination::operator != (
        const SurfaceFilterCombination& other) const {
    return usesAnd_ != other.usesAnd_;
}

inline void SurfaceFilterCombination::writeTextShort(std::ostream& o) const {
    o << (usesAnd_ ? "AND" : "OR") << " filter";
}

inline std::shared_ptr<Packet> SurfaceFilterCombination::internalClonePacket()
        const {
    return std::make_shared<SurfaceFilterCombination>(*this);
}

inline void swap(SurfaceFilterCombination& a, SurfaceFilterCombination& b) {
    a.swap(b);
}

// Inline functions for SurfaceFilterProperties

inline SurfaceFilterProperties::SurfaceFilterProperties() :
        orientability_(true, true),
        compactness_(true, true),
        realBoundary_(true, true) {
}

inline SurfaceFilterProperties& SurfaceFilterProperties::operator = (
        const SurfaceFilterProperties& src) {
    ChangeEventSpan span(*this);

    eulerChar_ = src.eulerChar_;
    orientability_ = src.orientability_;
    compactness_ = src.compactness_;
    realBoundary_ = src.realBoundary_;

    return *this;
}

inline void SurfaceFilterProperties::swap(SurfaceFilterProperties& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    eulerChar_.swap(other.eulerChar_);
    std::swap(orientability_, other.orientability_);
    std::swap(compactness_, other.compactness_);
    std::swap(realBoundary_, other.realBoundary_);
}

inline const std::set<LargeInteger>& SurfaceFilterProperties::eulerChars()
        const {
    return eulerChar_;
}
inline size_t SurfaceFilterProperties::countEulerChars() const {
    return eulerChar_.size();
}
inline BoolSet SurfaceFilterProperties::orientability() const {
    return orientability_;
}
inline BoolSet SurfaceFilterProperties::compactness() const {
    return compactness_;
}
inline BoolSet SurfaceFilterProperties::realBoundary() const {
    return realBoundary_;
}

template <typename Iterator>
inline void SurfaceFilterProperties::setEulerChars(
        Iterator beginEuler, Iterator endEuler) {
    ChangeEventSpan span(*this);
    eulerChar_.clear();
    eulerChar_.insert(beginEuler, endEuler);
}

inline void SurfaceFilterProperties::addEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(*this);
    eulerChar_.insert(ec);
}
inline void SurfaceFilterProperties::removeEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(*this);
    eulerChar_.erase(ec);
}
inline void SurfaceFilterProperties::removeAllEulerChars() {
    ChangeEventSpan span(*this);
    eulerChar_.clear();
}
inline void SurfaceFilterProperties::setOrientability(BoolSet value) {
    if (orientability_ != value) {
        ChangeEventSpan span(*this);
        orientability_ = value;
    }
}
inline void SurfaceFilterProperties::setCompactness(BoolSet value) {
    if (compactness_ != value) {
        ChangeEventSpan span(*this);
        compactness_ = value;
    }
}
inline void SurfaceFilterProperties::setRealBoundary(BoolSet value) {
    if (realBoundary_ != value) {
        ChangeEventSpan span(*this);
        realBoundary_ = value;
    }
}

inline bool SurfaceFilterProperties::operator == (
        const SurfaceFilterProperties& other) const {
    return orientability_ == other.orientability_ &&
        compactness_ == other.compactness_ &&
        realBoundary_ == other.realBoundary_ &&
        eulerChar_ == other.eulerChar_;
}

inline bool SurfaceFilterProperties::operator != (
        const SurfaceFilterProperties& other) const {
    return orientability_ != other.orientability_ ||
        compactness_ != other.compactness_ ||
        realBoundary_ != other.realBoundary_ ||
        eulerChar_ != other.eulerChar_;
}

inline std::shared_ptr<Packet> SurfaceFilterProperties::internalClonePacket()
        const {
    return std::make_shared<SurfaceFilterProperties>(*this);
}

inline void swap(SurfaceFilterProperties& a, SurfaceFilterProperties& b) {
    a.swap(b);
}

} // namespace regina

#endif

