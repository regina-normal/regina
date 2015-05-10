
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file shareableobject.h
 *  \brief Provides a deprecated base class for many objects in Regina.
 */

#ifndef __SHAREABLEOBJECT_H
#ifndef __DOXYGEN
#define __SHAREABLEOBJECT_H
#endif

#include <iostream>
#include <string>
#include <boost/noncopyable.hpp>
#include "output.h"

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup engine
 * @{
 */

/**
 * A deprecated base class for many objects in Regina.
 *
 * \deprecated For a long time now, the only real functionality that this
 * class has provided is the common output routines str() and detail().
 * Classes that provide text output should now inherit from the appropriate
 * Output template class instead.
 */
class REGINA_API ShareableObject :
        public Output<ShareableObject>,
        public boost::noncopyable {
    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor that does nothing.
         *
         * \ifacespython Not present.
         */
        ShareableObject();
        /**
         * Default destructor that does nothing.
         */
        virtual ~ShareableObject();

        /*@}*/
        /**
         * \name Input and Output
         */
        /*@{*/

        /**
         * Writes this object in short text format to the given output stream.
         * The output should be human-readable, should fit on a single line,
         * and should not end with a newline.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const = 0;
        /**
         * Writes this object in long text format to the given output stream.
         * The output should provide the user with all the information they
         * could want.  The output should be human-readable, should not
         * contain extremely long lines (so users can read the output
         * in a terminal), and should end with a final newline.
         *
         * The default implementation of this routine merely calls
         * writeTextShort() and adds a newline.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const;

        /*@}*/
};

/*@}*/

// Inline functions for ShareableObject

inline ShareableObject::ShareableObject() {
}
inline ShareableObject::~ShareableObject() {
}

inline void ShareableObject::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << '\n';
}

} // namespace regina

#endif
