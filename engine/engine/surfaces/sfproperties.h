
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file sfproperties.h
 *  \brief Contains a normal surface filter that filters by basic
 *  properties.
 */

#ifndef __SFPROPERTIES_H
#ifndef __DOXYGEN
#define __SFPROPERTIES_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsurfacefilter.h"
    #include "nbooleans.h"
    #include "nmpi.h"
    #include "ndynamicarray.h"
#else
    #include "engine/surfaces/nsurfacefilter.h"
    #include "engine/utilities/nbooleans.h"
    #include "engine/utilities/nmpi.h"
    #include "engine/utilities/ndynamicarray.h"
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
class NSurfaceFilterProperties : public NSurfaceFilter {
    public:
        static const int filterID;

    private:
        NDynamicArray<NLargeInteger> eulerCharacteristic;
            /**< The set of allowable Euler characteristics.  An empty
                 set signifies that any Euler characteristic is allowed.
                 This array will be kept in ascending order and all elements
                 will be unique. */
        NBoolSet orientability;
            /**< The set of allowable orientability properties. */
        NBoolSet compactness;
            /**< The set of allowable compactness properties. */
        NBoolSet realBoundary;
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
         * \ifaces Not present.
         *
         * @return the set of allowable Euler characteristics.
         */
        const NDynamicArray<NLargeInteger>& getECs() const;
        /**
         * Returns the number of allowable Euler characteristics.
         * See getECs() for further details.
         *
         * @return the number of allowable Euler characteristics.
         */
        unsigned long getNumberOfECs() const;
        /**
         * Returns the allowable Euler characteristic at the given index
         * in the set.  See getECs() for further details.
         *
         * @param the index in the set of allowable Euler
         * characteristics; this must be between 0 and getNumberOfECs()-1
         * inclusive.
         * @return the requested allowable Euler characteristic.
         */
        NLargeInteger getEC(unsigned long index) const;
        /**
         * Returns the set of allowable orientabilities.  Note that this
         * is a subset of <tt>{ true, false }</tt>.
         * Any surface whose orientability is not in this set will not be
         * accepted by this filter.
         *
         * @return the set of allowable orientabilities.
         */
        NBoolSet getOrientability() const;
        /**
         * Returns the set of allowable compactness properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose compactness property is not in this set will
         * not be accepted by this filter.
         *
         * @return the set of allowable compactness properties.
         */
        NBoolSet getCompactness() const;
        /**
         * Returns the set of allowable has-real-boundary properties.
         * Note that this is a subset of <tt>{ true, false }</tt>.
         * Any surface whose has-real-boundary property is not in this set
         * will not be accepted by this filter.
         *
         * @return the set of allowable has-real-boundary properties.
         */
        NBoolSet getRealBoundary() const;

        /**
         * Adds the given Euler characteristic to the set of allowable
         * Euler characteristics.  See getECs() for further details.
         *
         * \pre The given Euler characteristic is not already in the
         * allowable set.
         *
         * @param ec the new allowable Euler characteristic.
         */
        void addEC(const NLargeInteger& ec);
        /**
         * Removes the given Euler characteristic from the set of allowable
         * Euler characteristics.  See getECs() for further details.
         *
         * Note that if the allowable set is completely emptied, this
         * filter will allow <i>any</i> Euler characteristic to pass.
         *
         * \pre The given Euler characteristic is currently in the
         * allowable set.
         *
         * @param ec the allowable Euler characteristic to remove.
         */
        void removeEC(const NLargeInteger& ec);
        /**
         * Empties the set of allowable Euler characteristics.  See
         * getECs() for further details.
         *
         * Note that this will mean that this filter will allow
         * <i>any</i> Euler characteristic to pass.
         */
        void removeAllECs();
        /**
         * Sets the set of allowable orientabilities.
         * See getOrientability() for further details.
         *
         * @param value the new set of allowable orientabilities.
         */
        void setOrientability(const NBoolSet& value);
        /**
         * Sets the set of allowable compactness properties.
         * See getCompactness() for further details.
         *
         * @param value the new set of allowable compactness properties.
         */
        void setCompactness(const NBoolSet& value);
        /**
         * Sets the set of allowable has-real-boundary properties.
         * See getRealBoundary() for further details.
         *
         * @param value the new set of allowable has-real-boundary
         * properties.
         */
        void setRealBoundary(const NBoolSet& value);
        
        virtual bool accept(NNormalSurface& surface) const;
        virtual void writeTextLong(ostream& o) const;
        virtual void writeFilter(NFile& out) const;
        virtual void writeProperties(NFile& out) const;
        static NSurfaceFilter* readFilter(NFile& in, NPacket* parent);

        virtual int getFilterID() const;
        virtual NString getFilterName() const;

    protected:
        virtual void readIndividualProperty(NFile& in, unsigned propType);
        virtual void initialiseAllProperties();
};

// Inline functions for NSurfaceFilterProperties

inline NSurfaceFilterProperties::NSurfaceFilterProperties() :
        orientability(NBoolSet::sBoth),
        compactness(NBoolSet::sBoth),
        realBoundary(NBoolSet::sBoth) {
}

inline const NDynamicArray<NLargeInteger>& NSurfaceFilterProperties::getECs()
        const {
    return eulerCharacteristic;
}
inline unsigned long NSurfaceFilterProperties::getNumberOfECs() const {
    return eulerCharacteristic.size();
}
inline NLargeInteger NSurfaceFilterProperties::getEC(unsigned long index)
        const {
    return eulerCharacteristic[index];
}
inline NBoolSet NSurfaceFilterProperties::getOrientability() const {
    return orientability;
}
inline NBoolSet NSurfaceFilterProperties::getCompactness() const {
    return compactness;
}
inline NBoolSet NSurfaceFilterProperties::getRealBoundary() const {
    return realBoundary;
}

inline void NSurfaceFilterProperties::addEC(const NLargeInteger& ec) {
    eulerCharacteristic.addSort(ec);
}
inline void NSurfaceFilterProperties::removeEC(const NLargeInteger& ec) {
    eulerCharacteristic.remove(ec);
}
inline void NSurfaceFilterProperties::removeAllECs() {
    eulerCharacteristic.flush();
}
inline void NSurfaceFilterProperties::setOrientability(const NBoolSet& value) {
    orientability = value;
}
inline void NSurfaceFilterProperties::setCompactness(const NBoolSet& value) {
    compactness = value;
}
inline void NSurfaceFilterProperties::setRealBoundary(const NBoolSet& value) {
    realBoundary = value;
}

inline void NSurfaceFilterProperties::writeFilter(NFile&) const {
}

#endif

