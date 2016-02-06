
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

/*! \file utilities/intrusiverefcounter.h
 *  \brief Provides a temporary replacement for boost::intrusive_ref_counter.
 */

#ifndef __INTRUSIVEREFCOUNTER_H
#ifndef __DOXYGEN
#define __INTRUSIVEREFCOUNTER_H
#endif

#ifndef __DOXYGEN
// Since this class is just a band-aid for older versions of boost, we
// hide it from doxygen completely.

namespace regina {
namespace temporary {

/**
 * Deprecated replacement for boost::intrusive_ref_counter.
 *
 * See boost::intrusive_ref_counter for details on how this class is
 * intended to behave.
 *
 * \deprecated This class is a crude replacement for
 * boost::intrusive_ref_counter, designed so that Regina can build against
 * versions of boost prior to 1.55.  This is just a stop-gap measure:
 * at some point in the future Regina will insist on a newer version of boost,
 * at which point this class will be removed.
 */
template<typename T> class IntrusiveRefCounter {
public:
    IntrusiveRefCounter() : refCounter(0) { }
    mutable int refCounter;
};

} } // namespace regina::temporary

namespace boost {

template<typename T>
inline void intrusive_ptr_add_ref(
        const ::regina::temporary::IntrusiveRefCounter<T>* p) {
    ++p->refCounter;
}

template<typename T>
inline void intrusive_ptr_release(
        const ::regina::temporary::IntrusiveRefCounter<T>* p) {
    --p->refCounter;
    if (p->refCounter == 0) {
        delete static_cast<const T*>(p);
    }
}

} // namespace boost

#endif // __DOXYGEN

#endif
