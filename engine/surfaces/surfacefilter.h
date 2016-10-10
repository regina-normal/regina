
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

/*! \file surfaces/surfacefilter.h
 *  \brief Contains a packet that filters through normal surfaces.
 */

#ifndef __SURFACEFILTER_H
#ifndef __DOXYGEN
#define __SURFACEFILTER_H
#endif

#include <set>
#include "regina-core.h"
#include "maths/integer.h"
#include "packet/packet.h"
#include "surfaces/surfacefiltertype.h"
#include "utilities/boolset.h"

namespace regina {

class NormalSurface;
class NormalSurfaces;
class SurfaceFilter;
class SurfaceFilterCombination;
class SurfaceFilterProperties;
class XMLPacketReader;
class XMLFilterReader;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A template that stores information about a particular type of normal
 * surface filter.  Much of this information is given in the
 * form of compile-time constants and types.
 *
 * To iterate through cases for a given value of SurfaceFilterInfo that is not
 * known until runtime, see the various forFilter() routines defined in
 * filterregistry.h.
 *
 * At a bare minimum, each specialisation of this template must provide:
 *
 * - a typedef \a Class that represents the corresponding
 *   SurfaceFilter descendant class;
 * - a static function name() that returns a C-style string giving the
 *   human-readable name of the filter type.
 *
 * \ifacespython Not present.
 *
 * \tparam filterType one of the #SurfaceFilterType constants, indicating
 * which type of filter we are querying.
 */
template <SurfaceFilterType filterType>
struct SurfaceFilterInfo;

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
 * @param class_ the name of this descendant class of SurfaceFilter.
 * @param id the corresponding SurfaceFilterType constant.
 */
#define REGINA_SURFACE_FILTER(class_, id) \
    public: \
        static constexpr const SurfaceFilterType filterTypeID = id; \
        inline virtual SurfaceFilterType filterType() const { \
            return id; \
        } \
        inline virtual std::string filterTypeName() const { \
            return SurfaceFilterInfo<id>::name(); \
        }

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_SURFACEFILTER> {
    typedef SurfaceFilter Class;
    inline static const char* name() {
        return "Surface Filter";
    }
};

template <>
struct SurfaceFilterInfo<NS_FILTER_DEFAULT> {
    typedef SurfaceFilter Class;
    inline static const char* name() {
        return "Default filter";
    }
};

template <>
struct SurfaceFilterInfo<NS_FILTER_COMBINATION> {
    typedef SurfaceFilterCombination Class;
    inline static const char* name() {
        return "Combination filter";
    }
};

template <>
struct SurfaceFilterInfo<NS_FILTER_PROPERTIES> {
    typedef SurfaceFilterProperties Class;
    inline static const char* name() {
        return "Filter by basic properties";
    }
};
#endif

/**
 * A packet that accepts or rejects normal surfaces.
 * Different subclasses of SurfaceFilter represent different filtering
 * methods.
 *
 * <b>When deriving classes from SurfaceFilter:</b>
 * <ul>
 *   <li>A new value must be added to the SurfaceFilterType enum in
 *   surfacefiltertype.h to represent the new filter type.</li>
 *   <li>The file filterregistry-impl.h must be updated to reflect the new
 *   filter type (the file itself contains instructions on how to do this).</li>
 *   <li>A corresponding specialisation of SurfaceFilterInfo<> must be
 *   defined, typically in the same header as the new filter class.</li>
 *   <li>The macro REGINA_SURFACE_FILTER must be added to the beginning
 *   of the new filter class.  This will declare and define various
 *   constants, typedefs and virtual functions (see the REGINA_SURFACE_FILTER
 *   macro documentation for details).</li>
 *   <li>A copy constructor <tt>class(const class& cloneMe)</tt> must
 *   be declared and implemented.  You may assume that parameter
 *   \a cloneMe is of the same class as that whose constructor you are
 *   writing.</li>
 *   <li>Virtual functions accept(), internalClonePacket(), writeTextLong() and
 *   writeXMLFilterData() must be overridden.</li>
 *   <li>Static function xmlFilterReader() must be declared and
 *   implemented as described in the documentation below.</li>
 * </ul>
 *
 * \todo \feature Implement property \a lastAppliedTo.
 */
class REGINA_API SurfaceFilter : public Packet {
    REGINA_PACKET(SurfaceFilter, PACKET_SURFACEFILTER)
    REGINA_SURFACE_FILTER(SurfaceFilter, NS_FILTER_DEFAULT)

    public:
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.
         */
        SurfaceFilter();
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.  Note that the given parameter is
         * ignored.
         *
         * @param cloneMe this parameter is ignored.
         */
        SurfaceFilter(const SurfaceFilter& cloneMe);
        /**
         * Destroys this surface filter.
         */
        virtual ~SurfaceFilter();

        /**
         * Decides whether or not the given normal surface is accepted by this
         * filter.
         *
         * The default implementation simply returns \c true.
         *
         * @param surface the normal surface under investigation.
         * @return \c true if and only if the given surface is accepted
         * by this filter.
         */
        virtual bool accept(const NormalSurface& surface) const;

#ifdef __DOXYGEN
        /**
         * Returns the unique integer ID corresponding to the filtering
         * method that is this particular subclass of SurfaceFilter.
         *
         * @return the unique integer filtering method ID.
         */
        virtual SurfaceFilterType filterType() const;
        /**
         * Returns a string description of the filtering method that is
         * this particular subclass of SurfaceFilter.
         *
         * @return a string description of this filtering method.
         */
        virtual std::string filterTypeName() const;
#endif

        /**
         * Returns a newly created XML filter reader that will read the
         * details of a particular type of surface filter.  You may
         * assume that the filter to be read is of the same type as the
         * class in which you are implementing this routine.
         *
         * The XML filter reader should read exactly what
         * writeXMLFilterData() writes, and vice versa.
         *
         * \a parent represents the packet which will become the new
         * filter's parent in the tree structure.  This information is
         * for reference only, and need not be used.
         * See the description of parameter \a parent in
         * Packet::xmlReader() for further details.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet which will become the new filter's
         * parent in the tree structure, or 0 if the new filter is to be
         * tree matriarch.
         * @return the newly created XML filter reader.
         */
        static XMLFilterReader* xmlFilterReader(Packet* parent);

        virtual void writeTextShort(std::ostream& out) const;
        static XMLPacketReader* xmlReader(Packet* parent,
            XMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

    protected:
        /**
         * Writes a chunk of XML containing the details of this filter.
         *
         * You may assume that the filter opening tag (including the
         * filter type) has already been written, and that the filter
         * closing tag will be written immediately after this routine is
         * called.  This routine need only write the additional details
         * corresponding to this particular subclass of SurfaceFilter.
         *
         * @param out the output stream to which the XML should be written.
         */
        virtual void writeXMLFilterData(std::ostream& out) const;
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
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
 */
class REGINA_API SurfaceFilterCombination : public SurfaceFilter {
    REGINA_SURFACE_FILTER(SurfaceFilterCombination, NS_FILTER_COMBINATION)

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
         * Creates a new surface filter that is a clone of the given
         * surface filter.
         *
         * @param cloneMe the surface filter to clone.
         */
        SurfaceFilterCombination(const SurfaceFilterCombination& cloneMe);

        /**
         * Determines whether this is an \a and or an \a or combination.
         *
         * @return \c true if this is an \a and combination, or \c false
         * if this is an \a or combination.
         */
        bool usesAnd() const;
        /**
         * Sets whether this is an \a and or an \a or combination.
         *
         * @param value \c true if this is to be an \a and combination,
         * or \c false if this is to be an \a or combination.
         */
        void setUsesAnd(bool value);

        virtual bool accept(const NormalSurface& surface) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLFilterReader* xmlFilterReader(Packet* parent);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLFilterData(std::ostream& out) const;
};

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
 */
class REGINA_API SurfaceFilterProperties : public SurfaceFilter {
    REGINA_SURFACE_FILTER(SurfaceFilterProperties, NS_FILTER_PROPERTIES)

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
         * Creates a new surface filter that is a clone of the given
         * surface filter.
         *
         * @param cloneMe the surface filter to clone.
         */
        SurfaceFilterProperties(const SurfaceFilterProperties& cloneMe);

        /**
         * Returns the set of allowable Euler characteristics.  Any
         * surface whose Euler characteristic is not in this set will not
         * be accepted by this filter.  The set will be given in
         * ascending order with no element repeated.
         *
         * If this set is empty, all Euler characteristics will be
         * accepted.
         *
         * \ifacespython This routine returns a python sequence.
         *
         * @return the set of allowable Euler characteristics.
         */
        const std::set<LargeInteger>& eulerChars() const;
        /**
         * Returns the number of allowable Euler characteristics.
         * See eulerChars() for further details.
         *
         * @return the number of allowable Euler characteristics.
         */
        size_t countEulerChars() const;
        /**
         * Returns the allowable Euler characteristic at the given index
         * in the set.  See eulerChars() for further details.
         *
         * @param index the index in the set of allowable Euler
         * characteristics; this must be between 0 and countEulerChars()-1
         * inclusive.
         * @return the requested allowable Euler characteristic.
         */
        LargeInteger eulerChar(size_t index) const;
        /**
         * Returns the set of allowable orientabilities.  Note that this
         * is a subset of <tt>{ true, false }</tt>.
         * Any surface whose orientability is not in this set will not be
         * accepted by this filter.
         *
         * @return the set of allowable orientabilities.
         */
        BoolSet orientability() const;
        /**
         * Returns the set of allowable compactness properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose compactness property is not in this set will
         * not be accepted by this filter.
         *
         * @return the set of allowable compactness properties.
         */
        BoolSet compactness() const;
        /**
         * Returns the set of allowable has-real-boundary properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose has-real-boundary property is not in this set
         * will not be accepted by this filter.
         *
         * @return the set of allowable has-real-boundary properties.
         */
        BoolSet realBoundary() const;

        /**
         * Sets the allowable Euler characteristics to the given set.
         * See eulerChars() for further details.
         *
         * \ifaces Not present.
         *
         * @param s the new set of allowable Euler characteristics.
         */
        void setEulerChars(const std::set<LargeInteger>& s);

        /**
         * Adds the given Euler characteristic to the set of allowable
         * Euler characteristics.  See eulerChars() for further details.
         *
         * @param ec the new allowable Euler characteristic.
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
         * @param ec the allowable Euler characteristic to remove.
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
         * @param value the new set of allowable orientabilities.
         */
        void setOrientability(const BoolSet& value);
        /**
         * Sets the set of allowable compactness properties.
         * See compactness() for further details.
         *
         * @param value the new set of allowable compactness properties.
         */
        void setCompactness(const BoolSet& value);
        /**
         * Sets the set of allowable has-real-boundary properties.
         * See realBoundary() for further details.
         *
         * @param value the new set of allowable has-real-boundary
         * properties.
         */
        void setRealBoundary(const BoolSet& value);

        virtual bool accept(const NormalSurface& surface) const;
        virtual void writeTextLong(std::ostream& out) const;
        static XMLFilterReader* xmlFilterReader(Packet* parent);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLFilterData(std::ostream& out) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSurfaceFilter has now been renamed to
 * SurfaceFilter.
 */
REGINA_DEPRECATED typedef SurfaceFilter NSurfaceFilter;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSurfaceFilterCombination has now been renamed to
 * SurfaceFilterCombination.
 */
REGINA_DEPRECATED typedef SurfaceFilterCombination NSurfaceFilterCombination;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSurfaceFilterProperties has now been renamed to
 * SurfaceFilterProperties.
 */
REGINA_DEPRECATED typedef SurfaceFilterProperties NSurfaceFilterProperties;

/*@}*/

// Inline functions for SurfaceFilter

inline SurfaceFilter::SurfaceFilter() {
}
inline SurfaceFilter::SurfaceFilter(const SurfaceFilter&) : Packet() {
}
inline SurfaceFilter::~SurfaceFilter() {
}

inline bool SurfaceFilter::accept(const NormalSurface&) const {
    return true;
}

inline void SurfaceFilter::writeXMLFilterData(std::ostream&) const {
}

inline void SurfaceFilter::writeTextShort(std::ostream& o) const {
    o << filterTypeName();
}

inline bool SurfaceFilter::dependsOnParent() const {
    return false;
}

inline Packet* SurfaceFilter::internalClonePacket(Packet*) const {
    return new SurfaceFilter();
}

// Inline functions for SurfaceFilterCombination

inline SurfaceFilterCombination::SurfaceFilterCombination() : usesAnd_(true) {
}
inline SurfaceFilterCombination::SurfaceFilterCombination(
        const SurfaceFilterCombination& cloneMe) : SurfaceFilter(),
        usesAnd_(cloneMe.usesAnd_) {
}

inline bool SurfaceFilterCombination::usesAnd() const {
    return usesAnd_;
}
inline void SurfaceFilterCombination::setUsesAnd(bool value) {
    if (usesAnd_ != value) {
        ChangeEventSpan span(this);
        usesAnd_ = value;
    }
}

inline void SurfaceFilterCombination::writeTextLong(std::ostream& o) const {
    o << (usesAnd_ ? "AND" : "OR") << " combination normal surface filter\n";
}

inline Packet* SurfaceFilterCombination::internalClonePacket(Packet*) const {
    return new SurfaceFilterCombination(*this);
}

// Inline functions for SurfaceFilterProperties

inline SurfaceFilterProperties::SurfaceFilterProperties() :
        orientability_(BoolSet::sBoth),
        compactness_(BoolSet::sBoth),
        realBoundary_(BoolSet::sBoth) {
}
inline SurfaceFilterProperties::SurfaceFilterProperties(
        const SurfaceFilterProperties& cloneMe) :
        SurfaceFilter(),
        eulerChar_(cloneMe.eulerChar_),
        orientability_(cloneMe.orientability_),
        compactness_(cloneMe.compactness_),
        realBoundary_(cloneMe.realBoundary_) {
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

inline void SurfaceFilterProperties::setEulerChars(
        const std::set<LargeInteger>& s) {
    if (eulerChar_ != s) {
        ChangeEventSpan span(this);
        eulerChar_ = s;
    }
}
inline void SurfaceFilterProperties::addEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.insert(ec);
}
inline void SurfaceFilterProperties::removeEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.erase(ec);
}
inline void SurfaceFilterProperties::removeAllEulerChars() {
    ChangeEventSpan span(this);
    eulerChar_.clear();
}
inline void SurfaceFilterProperties::setOrientability(const BoolSet& value) {
    if (orientability_ != value) {
        ChangeEventSpan span(this);
        orientability_ = value;
    }
}
inline void SurfaceFilterProperties::setCompactness(const BoolSet& value) {
    if (compactness_ != value) {
        ChangeEventSpan span(this);
        compactness_ = value;
    }
}
inline void SurfaceFilterProperties::setRealBoundary(const BoolSet& value) {
    if (realBoundary_ != value) {
        ChangeEventSpan span(this);
        realBoundary_ = value;
    }
}

inline Packet* SurfaceFilterProperties::internalClonePacket(Packet*) const {
    return new SurfaceFilterProperties(*this);
}

} // namespace regina

#endif

