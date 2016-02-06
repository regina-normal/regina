
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
#include "maths/ninteger.h"
#include "surfaces/nsurfacefilter.h"
#include "utilities/nbooleans.h"

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
        std::set<NLargeInteger> eulerChar_;
            /**< The set of allowable Euler characteristics.  An empty
                 set signifies that any Euler characteristic is allowed. */
        NBoolSet orientability_;
            /**< The set of allowable orientability properties. */
        NBoolSet compactness_;
            /**< The set of allowable compactness properties. */
        NBoolSet realBoundary_;
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
        const std::set<NLargeInteger>& eulerChars() const;
        /**
         * Deprecated routine that returns the set of allowable
         * Euler characteristics.
         *
         * \deprecated This routine has been renamed to eulerChars().
         * See the eulerChars() documentation for further details.
         */
        REGINA_DEPRECATED const std::set<NLargeInteger>& getECs() const;
        /**
         * Returns the number of allowable Euler characteristics.
         * See eulerChars() for further details.
         *
         * @return the number of allowable Euler characteristics.
         */
        size_t countEulerChars() const;
        /**
         * Deprecated routine that returns the number of allowable
         * Euler characteristics.
         *
         * \deprecated This routine has been renamed to countEulerChars().
         * See the countEulerChars() documentation for further details.
         */
        REGINA_DEPRECATED size_t getNumberOfECs() const;
        /**
         * Returns the allowable Euler characteristic at the given index
         * in the set.  See eulerChars() for further details.
         *
         * @param index the index in the set of allowable Euler
         * characteristics; this must be between 0 and countEulerChars()-1
         * inclusive.
         * @return the requested allowable Euler characteristic.
         */
        NLargeInteger eulerChar(size_t index) const;
        /**
         * Deprecated routine that returns the allowable Euler characteristic
         * at the given index in the set.
         *
         * \deprecated This routine has been renamed to eulerChar().
         * See the eulerChar() documentation for further details.
         */
        REGINA_DEPRECATED NLargeInteger getEC(size_t index) const;
        /**
         * Returns the set of allowable orientabilities.  Note that this
         * is a subset of <tt>{ true, false }</tt>.
         * Any surface whose orientability is not in this set will not be
         * accepted by this filter.
         *
         * @return the set of allowable orientabilities.
         */
        NBoolSet orientability() const;
        /**
         * Deprecated routine that returns the set of allowable orientabilities.
         *
         * \deprecated This routine has been renamed to orientability().
         * See the orientability() documentation for further details.
         */
        REGINA_DEPRECATED NBoolSet getOrientability() const;
        /**
         * Returns the set of allowable compactness properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose compactness property is not in this set will
         * not be accepted by this filter.
         *
         * @return the set of allowable compactness properties.
         */
        NBoolSet compactness() const;
        /**
         * Deprecated routine that returns the set of allowable
         * compactness properties.
         *
         * \deprecated This routine has been renamed to compactness().
         * See the compactness() documentation for further details.
         */
        REGINA_DEPRECATED NBoolSet getCompactness() const;
        /**
         * Returns the set of allowable has-real-boundary properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose has-real-boundary property is not in this set
         * will not be accepted by this filter.
         *
         * @return the set of allowable has-real-boundary properties.
         */
        NBoolSet realBoundary() const;
        /**
         * Deprecated routine that returns the set of allowable
         * has-real-boundary properties.
         *
         * \deprecated This routine has been renamed to realBoundary().
         * See the realBoundary() documentation for further details.
         */
        REGINA_DEPRECATED NBoolSet getRealBoundary() const;

        /**
         * Sets the allowable Euler characteristics to the given set.
         * See eulerChars() for further details.
         *
         * \ifaces Not present.
         *
         * @param s the new set of allowable Euler characteristics.
         */
        void setEulerChars(const std::set<NLargeInteger>& s);

        /**
         * Adds the given Euler characteristic to the set of allowable
         * Euler characteristics.  See eulerChars() for further details.
         *
         * @param ec the new allowable Euler characteristic.
         */
        void addEulerChar(const NLargeInteger& ec);
        /**
         * Deprecated routine that adds the given Euler characteristic
         * to the set of allowable Euler characteristics.
         *
         * \deprecated This routine has been renamed to addEulerChar().
         * See the addEulerChar() documentation for further details.
         */
        REGINA_DEPRECATED void addEC(const NLargeInteger& ec);
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
        void removeEulerChar(const NLargeInteger& ec);
        /**
         * Deprecated routine that removes the given Euler characteristic
         * from the set of allowable Euler characteristics.
         *
         * \deprecated This routine has been renamed to removeEulerChar().
         * See the removeEulerChar() documentation for further details.
         */
        REGINA_DEPRECATED void removeEC(const NLargeInteger& ec);
        /**
         * Empties the set of allowable Euler characteristics.  See
         * eulerChars() for further details.
         *
         * Note that this will mean that this filter will allow
         * <i>any</i> Euler characteristic to pass.
         */
        void removeAllEulerChars();
        /**
         * Deprecated routine that empties the set of allowable Euler
         * characteristics.
         *
         * \deprecated This routine has been renamed to removeAllEulerChars().
         * See the removeAllEulerChars() documentation for further details.
         */
        REGINA_DEPRECATED void removeAllECs();
        /**
         * Sets the set of allowable orientabilities.
         * See orientability() for further details.
         *
         * @param value the new set of allowable orientabilities.
         */
        void setOrientability(const NBoolSet& value);
        /**
         * Sets the set of allowable compactness properties.
         * See compactness() for further details.
         *
         * @param value the new set of allowable compactness properties.
         */
        void setCompactness(const NBoolSet& value);
        /**
         * Sets the set of allowable has-real-boundary properties.
         * See realBoundary() for further details.
         *
         * @param value the new set of allowable has-real-boundary
         * properties.
         */
        void setRealBoundary(const NBoolSet& value);

        virtual bool accept(const NNormalSurface& surface) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLFilterReader* xmlFilterReader(NPacket* parent);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLFilterData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSurfaceFilterProperties

inline NSurfaceFilterProperties::NSurfaceFilterProperties() :
        orientability_(NBoolSet::sBoth),
        compactness_(NBoolSet::sBoth),
        realBoundary_(NBoolSet::sBoth) {
}
inline NSurfaceFilterProperties::NSurfaceFilterProperties(
        const NSurfaceFilterProperties& cloneMe) :
        NSurfaceFilter(),
        eulerChar_(cloneMe.eulerChar_),
        orientability_(cloneMe.orientability_),
        compactness_(cloneMe.compactness_),
        realBoundary_(cloneMe.realBoundary_) {
}

inline const std::set<NLargeInteger>& NSurfaceFilterProperties::eulerChars()
        const {
    return eulerChar_;
}
inline const std::set<NLargeInteger>& NSurfaceFilterProperties::getECs() const {
    return eulerChar_;
}
inline size_t NSurfaceFilterProperties::countEulerChars() const {
    return eulerChar_.size();
}
inline size_t NSurfaceFilterProperties::getNumberOfECs() const {
    return eulerChar_.size();
}
inline NLargeInteger NSurfaceFilterProperties::getEC(size_t index) const {
    return eulerChar(index);
}
inline NBoolSet NSurfaceFilterProperties::orientability() const {
    return orientability_;
}
inline NBoolSet NSurfaceFilterProperties::getOrientability() const {
    return orientability_;
}
inline NBoolSet NSurfaceFilterProperties::compactness() const {
    return compactness_;
}
inline NBoolSet NSurfaceFilterProperties::getCompactness() const {
    return compactness_;
}
inline NBoolSet NSurfaceFilterProperties::realBoundary() const {
    return realBoundary_;
}
inline NBoolSet NSurfaceFilterProperties::getRealBoundary() const {
    return realBoundary_;
}

inline void NSurfaceFilterProperties::setEulerChars(
        const std::set<NLargeInteger>& s) {
    if (eulerChar_ != s) {
        ChangeEventSpan span(this);
        eulerChar_ = s;
    }
}
inline void NSurfaceFilterProperties::addEulerChar(const NLargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.insert(ec);
}
inline void NSurfaceFilterProperties::addEC(const NLargeInteger& ec) {
    addEulerChar(ec);
}
inline void NSurfaceFilterProperties::removeEulerChar(const NLargeInteger& ec) {
    ChangeEventSpan span(this);
    eulerChar_.erase(ec);
}
inline void NSurfaceFilterProperties::removeEC(const NLargeInteger& ec) {
    removeEulerChar(ec);
}
inline void NSurfaceFilterProperties::removeAllEulerChars() {
    ChangeEventSpan span(this);
    eulerChar_.clear();
}
inline void NSurfaceFilterProperties::removeAllECs() {
    removeAllEulerChars();
}
inline void NSurfaceFilterProperties::setOrientability(const NBoolSet& value) {
    if (orientability_ != value) {
        ChangeEventSpan span(this);
        orientability_ = value;
    }
}
inline void NSurfaceFilterProperties::setCompactness(const NBoolSet& value) {
    if (compactness_ != value) {
        ChangeEventSpan span(this);
        compactness_ = value;
    }
}
inline void NSurfaceFilterProperties::setRealBoundary(const NBoolSet& value) {
    if (realBoundary_ != value) {
        ChangeEventSpan span(this);
        realBoundary_ = value;
    }
}

inline NPacket* NSurfaceFilterProperties::internalClonePacket(NPacket*) const {
    return new NSurfaceFilterProperties(*this);
}

} // namespace regina

#endif

