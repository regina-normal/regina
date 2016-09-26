
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

/*! \file surfaces/nsurfacefilter.h
 *  \brief Contains a packet that filters through normal surfaces.
 */

#ifndef __NSURFACEFILTER_H
#ifndef __DOXYGEN
#define __NSURFACEFILTER_H
#endif

#include "regina-core.h"
#include "packet/npacket.h"
#include "surfaces/surfacefiltertype.h"

namespace regina {

class NNormalSurface;
class NNormalSurfaceList;
class NSurfaceFilter;
class NXMLPacketReader;
class NXMLFilterReader;

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
 *   NSurfaceFilter descendant class;
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
 * descendant class of NSurfaceFilter.
 *
 * Every descendant class of NSurfaceFilter \a must include
 * REGINA_SURFACE_FILTER at the beginning of the class definition.
 *
 * This macro provides the class with:
 *
 * - a compile-time constant \a filterTypeID, which is equal to the
 *   corresponding SurfaceFilterType constant;
 * - declarations and implementations of the virtual functions
 *   NSurfaceFilter::filterType() and NSurfaceFilter::filterTypeName();
 *
 * @param class_ the name of this descendant class of NSurfaceFilter.
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
    typedef NSurfaceFilter Class;
    inline static const char* name() {
        return "Surface Filter";
    }
};

template <>
struct SurfaceFilterInfo<NS_FILTER_DEFAULT> {
    typedef NSurfaceFilter Class;
    inline static const char* name() {
        return "Default filter";
    }
};
#endif

/**
 * A packet that accepts or rejects normal surfaces.
 * Different subclasses of NSurfaceFilter represent different filtering
 * methods.
 *
 * <b>When deriving classes from NSurfaceFilter:</b>
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
class REGINA_API NSurfaceFilter : public NPacket {
    REGINA_PACKET(NSurfaceFilter, PACKET_SURFACEFILTER)
    REGINA_SURFACE_FILTER(NSurfaceFilter, NS_FILTER_DEFAULT)

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

#ifdef __DOXYGEN
        /**
         * Returns the unique integer ID corresponding to the filtering
         * method that is this particular subclass of NSurfaceFilter.
         *
         * @return the unique integer filtering method ID.
         */
        virtual SurfaceFilterType filterType() const;
        /**
         * Returns a string description of the filtering method that is
         * this particular subclass of NSurfaceFilter.
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
         * NPacket::xmlReader() for further details.
         *
         * \ifacespython Not present.
         *
         * @param parent the packet which will become the new filter's
         * parent in the tree structure, or 0 if the new filter is to be
         * tree matriarch.
         * @return the newly created XML filter reader.
         */
        static NXMLFilterReader* xmlFilterReader(NPacket* parent);

        virtual void writeTextShort(std::ostream& out) const;
        static NXMLPacketReader* xmlReader(NPacket* parent,
            NXMLTreeResolver& resolver);
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
    o << filterTypeName();
}

inline bool NSurfaceFilter::dependsOnParent() const {
    return false;
}

inline NPacket* NSurfaceFilter::internalClonePacket(NPacket*) const {
    return new NSurfaceFilter();
}

} // namespace regina

#endif

