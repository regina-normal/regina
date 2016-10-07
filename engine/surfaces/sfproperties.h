
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

/*! \file surfaces/sfproperties.h
 *  \brief Contains a normal surface filter that filters by basic
 *  properties.
 */

#ifndef __SFPROPERTIES_H
#ifndef __DOXYGEN
#define __SFPROPERTIES_H
#endif

#include <set>
#include "regina-core.h"
#include "maths/integer.h"
#include "surfaces/nsurfacefilter.h"
#include "utilities/boolset.h"

namespace regina {

class NSurfaceFilterProperties;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct SurfaceFilterInfo<NS_FILTER_PROPERTIES> {
    typedef NSurfaceFilterProperties Class;
    inline static const char* name() {
        return "Filter by basic properties";
    }
};
#endif

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
class REGINA_API NSurfaceFilterProperties : public NSurfaceFilter {
    REGINA_SURFACE_FILTER(NSurfaceFilterProperties, NS_FILTER_PROPERTIES)

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
        NSurfaceFilterProperties();
        /**
         * Creates a new surface filter that is a clone of the given
         * surface filter.
         *
         * @param cloneMe the surface filter to clone.
         */
        NSurfaceFilterProperties(const NSurfaceFilterProperties& cloneMe);

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

        virtual bool accept(const NNormalSurface& surface) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLFilterReader* xmlFilterReader(Packet* parent);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const;
        virtual void writeXMLFilterData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSurfaceFilterProperties

inline NSurfaceFilterProperties::NSurfaceFilterProperties() :
        orientability_(BoolSet::sBoth),
        compactness_(BoolSet::sBoth),
        realBoundary_(BoolSet::sBoth) {
}
inline NSurfaceFilterProperties::NSurfaceFilterProperties(
        const NSurfaceFilterProperties& cloneMe) :
        NSurfaceFilter(),
        eulerChar_(cloneMe.eulerChar_),
        orientability_(cloneMe.orientability_),
        compactness_(cloneMe.compactness_),
        realBoundary_(cloneMe.realBoundary_) {
}

inline const std::set<LargeInteger>& NSurfaceFilterProperties::eulerChars()
        const {
    return eulerChar_;
}
inline size_t NSurfaceFilterProperties::countEulerChars() const {
    return eulerChar_.size();
}
inline BoolSet NSurfaceFilterProperties::orientability() const {
    return orientability_;
}
inline BoolSet NSurfaceFilterProperties::compactness() const {
    return compactness_;
}
inline BoolSet NSurfaceFilterProperties::realBoundary() const {
    return realBoundary_;
}

inline void NSurfaceFilterProperties::setEulerChars(
        const std::set<LargeInteger>& s) {
    if (eulerChar_ != s) {
        ChangeEventSpan span(this);
        eulerChar_ = s;
    }
}
inline void NSurfaceFilterProperties::addEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.insert(ec);
}
inline void NSurfaceFilterProperties::removeEulerChar(const LargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.erase(ec);
}
inline void NSurfaceFilterProperties::removeAllEulerChars() {
    ChangeEventSpan span(this);
    eulerChar_.clear();
}
inline void NSurfaceFilterProperties::setOrientability(const BoolSet& value) {
    if (orientability_ != value) {
        ChangeEventSpan span(this);
        orientability_ = value;
    }
}
inline void NSurfaceFilterProperties::setCompactness(const BoolSet& value) {
    if (compactness_ != value) {
        ChangeEventSpan span(this);
        compactness_ = value;
    }
}
inline void NSurfaceFilterProperties::setRealBoundary(const BoolSet& value) {
    if (realBoundary_ != value) {
        ChangeEventSpan span(this);
        realBoundary_ = value;
    }
}

inline Packet* NSurfaceFilterProperties::internalClonePacket(Packet*) const {
    return new NSurfaceFilterProperties(*this);
}

} // namespace regina

#endif

