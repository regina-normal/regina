
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file surfaces/nsurfacefilter.h
 *  \brief Contains a packet that filters through normal surfaces.
 */

#ifndef __NSURFACEFILTER_H
#ifndef __DOXYGEN
#define __NSURFACEFILTER_H
#endif

#include "regina-core.h"
#include "packet/npacket.h"

namespace regina {

class NNormalSurface;
class NNormalSurfaceList;
class NXMLPacketReader;
class NXMLFilterReader;

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A packet that accepts or rejects normal surfaces.
 * Different subclasses of NSurfaceFilter represent different filtering
 * methods.
 *
 * <b>When deriving classes from NSurfaceFilter:</b>
 * <ul>
 *   <li>The file filterregistry.h must be updated to reflect the new
 *   filter type.</li>
 *   <li>A copy constructor <tt>class(const class& cloneMe)</tt> must
 *   be declared and implemented.  You may assume that parameter
 *   \a cloneMe is of the same class as that whose constructor you are
 *   writing.</li>
 *   <li>Virtual functions accept(), writeTextLong() and
 *   writeXMLFilterData() must be overridden.</li>
 *   <li>Static function getXMLFilterReader() must be declared and
 *   implemented as described in the documentation below.</li>
 *   <li>Virtual functions getFilterID() and getFilterName()
 *   must be redeclared but not reimplemented.
 *   The registry utilities will take care of their implementation.</li>
 *   <li><tt>public static const int filterID</tt> must be declared.
 *   The registry utilities will take care of assigning it a value.</li>
 * </ul>
 *
 * \todo \feature Implement property \a lastAppliedTo.
 */
class REGINA_API NSurfaceFilter : public NPacket {
    public:
        /**
         * Contains the integer ID for this type of surface filter.
         * Each distinct filtering class must have a unique ID, and this
         * should be a non-negative integer.  See filterregistry.h for
         * further details.
         *
         * This member must be declared for every filtering class that
         * will be instantiated.  A value need not be assigned;
         * filterregistry.h will take care of this task when you register
         * the filtering class.
         */
        static const int filterID;

        static const int packetType;

    public:
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.
         */
        NSurfaceFilter();
        /**
         * Creates a new default surface filter.  This will simply accept
         * all normal surfaces.  Note that the given parameter is
         * ignored.
         *
         * @param cloneMe this parameter is ignored.
         */
        NSurfaceFilter(const NSurfaceFilter& cloneMe);
        /**
         * Destroys this surface filter.
         */
        virtual ~NSurfaceFilter();

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
        virtual bool accept(const NNormalSurface& surface) const;

        /**
         * Returns the unique integer ID corresponding to the filtering
         * method that is this particular subclass of NSurfaceFilter.
         *
         * @return the unique integer filtering method ID.
         */
        virtual int getFilterID() const;
        /**
         * Returns a string description of the filtering method that is
         * this particular subclass of NSurfaceFilter.
         *
         * @return a string description of this filtering method.
         */
        virtual std::string getFilterName() const;

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
         * NPacket::getXMLReader() for further details.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet which will become the new filter's
         * parent in the tree structure, or 0 if the new filter is to be
         * tree matriarch.
         * @return the newly created XML filter reader.
         */
        static NXMLFilterReader* getXMLFilterReader(NPacket* parent);

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;
        virtual void writeTextShort(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual bool dependsOnParent() const;

    protected:
        /**
         * Writes a chunk of XML containing the details of this filter.
         *
         * You may assume that the filter opening tag (including the
         * filter type) has already been written, and that the filter
         * closing tag will be written immediately after this routine is
         * called.  This routine need only write the additional details
         * corresponding to this particular subclass of NSurfaceFilter.
         *
         * @param out the output stream to which the XML should be written.
         */
        virtual void writeXMLFilterData(std::ostream& out) const;
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSurfaceFilter

inline NSurfaceFilter::NSurfaceFilter() {
}
inline NSurfaceFilter::NSurfaceFilter(const NSurfaceFilter&) : NPacket() {
}
inline NSurfaceFilter::~NSurfaceFilter() {
}

inline bool NSurfaceFilter::accept(const NNormalSurface&) const {
    return true;
}

inline void NSurfaceFilter::writeXMLFilterData(std::ostream&) const {
}

inline void NSurfaceFilter::writeTextShort(std::ostream& o) const {
    o << getFilterName();
}

inline bool NSurfaceFilter::dependsOnParent() const {
    return false;
}

} // namespace regina

#endif

