
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file nsurfaceset.h
 *  \brief Provides access to a collection of normal surfaces.
 */

#ifndef __NSURFACESET_H
#ifndef __DOXYGEN
#define __NSURFACESET_H
#endif

#include <iostream>

namespace regina {

class NNormalSurface;
class NTriangulation;
class ShareableObject;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Represents a collection of normal surfaces in a 3-manifold.
 * This is a completely abstract base class used to provide a common
 * interface to different types of surface sets.
 *
 * Any class derived from NSurfaceSet must maintain a \a fixed set of
 * surfaces; no surfaces may be added to or removed from the set.
 * Surfaces in the set may however be modified.
 *
 * <b>When deriving classes from NSurfaceSet:</b>
 * <ul>
 *   <li>Follow the instructions on the \ref HowTo page for
 *   adding a new type of normal surface set.</li>
 *   <li>All abstract functions should be implemented.</li>
 * </ul>
 *
 * \pre No surfaces will ever be added to or removed from this set after
 * its creation.
 *
 * \warning You should \b never cast down from NSurfaceSet; instead you
 * should always call getShareableObject() and cast down from there.
 *
 * \ifaces This simply defines an interface that derived classes must
 * adhere to; NSurfaceSet is not a wrapper class itself.
 *
 * \ifacesjava NSurfaceSet will be a Java interface, which must be
 * implemented by all wrapper classes for engine classes derived from
 * NSurfaceSet.
 */
class NSurfaceSet {
    public:
        /**
         * Returns the flavour of coordinate system being used by the
         * surfaces stored in this set.  This will be one of the
         * predefined coordinate system constants in NNormalSurfaceList.
         *
         * @return the flavour of coordinate system used.
         */
        virtual int getFlavour() const = 0;
        /**
         * Determines if the flavour of coordinate system being used
         * allows for almost normal surfaces, that is, allows for
         * octahedral discs.
         *
         * @return \c true if and only if almost normal surfaces are
         * allowed.
         */
        virtual bool allowsAlmostNormal() const = 0;
        /**
         * Returns whether this set is known to contain only embedded normal
         * surfaces.
         *
         * If it is possible that there are non-embedded surfaces in this
         * set but it is not known whether any are actually present or
         * not, this routine should return \c false.
         *
         * @return \c true if it is known that only embedded normal surfaces
         * exist in this list, or \c false if immersed and/or singular normal
         * surfaces might be present.
         */
        virtual bool isEmbeddedOnly() const = 0;
        /**
         * Returns the triangulation upon which these normal surfaces are
         * based.
         * 
         * @return the triangulation upon which these surfaces are based.
         */
        virtual NTriangulation* getTriangulation() const = 0;

        /**
         * Returns the number of surfaces stored in this set.
         *
         * @return the number of surfaces.
         */
        virtual unsigned long getNumberOfSurfaces() const = 0;
        /**
         * Returns the surface at the requested index in this set.
         *
         * @param index the index of the requested surface in this set;
         * this must be between 0 and getNumberOfSurfaces()-1 inclusive.
         *
         * @return the normal surface at the requested index in this set.
         */
        virtual const NNormalSurface* getSurface(unsigned long index)
                const = 0;

        /**
         * Returns this object cast as a ShareableObject.
         * Generally the implementation of this routine will simply be
         * <tt>return this;</tt>.
         *
         * This routine is necessary because NSurfaceSet is
         * \a not of type ShareableObject; however, it is presumed that
         * each of its derived classes will be.
         * The aim here is to reduce the mess
         * that could arise combining virtual multiple inheritance
         * with the voluminous casting and recasting that takes place
         * when working with external interfaces.
         *
         * @return this object cast as a ShareableObject.
         */
        virtual ShareableObject* getShareableObject() = 0;

        /**
         * Writes the number of surfaces in this set followed by the
         * details of each surface to the given output stream.  Output
         * will be over many lines.
         *
         * \ifaces Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeAllSurfaces(std::ostream& out) const;
};

/*@}*/

} // namespace regina

#endif

