
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

/*! \file shareableobject.h
 *  \brief Deals with objects that can be shared from the
 *  calculation engine with the outside world.
 */

#ifndef __SHAREABLEOBJECT_H
#ifndef __DOXYGEN
#define __SHAREABLEOBJECT_H
#endif

#include <iostream>
#include <string>

#include "utilities/boostutils.h"

namespace regina {

/**
 * \weakgroup engine
 * @{
 */

/**
 * Facilitates mirroring objects in the underlying C++ calculation
 * engine using the various wrapper classes provided in the various
 * external interfaces (such as the Python interface).
 *
 * In the underlying C++ engine, a ShareableObject is an object that can
 * be shared with the outside world.  In the external
 * interfaces, a ShareableObject is a vacuous wrapper that allows access
 * to the data and methods of the corresponding object in the underlying
 * engine.
 *
 * See the various interface notes pages for more details regarding
 * using classes derived from ShareableObject.
 */
class ShareableObject : public regina::boost::noncopyable {
    public:
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

        /**
         * Writes this object in short text format to the given output stream.
         * The output should fit on a single line and no newline should
         * be written.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const = 0;
        /**
         * Writes this object in long text format to the given output
         * stream.  The output should provided the user with all the
         * information they could want.  The output should end with a
         * newline.
         *
         * The default implementation of this routine merely calls
         * writeTextShort() and adds a newline.
         *
         * \ifacespython The parameter \a out does not exist;
         * standard output will be used.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const;
        /**
         * Returns the output from writeTextShort() as a string.
         *
         * \ifacespython This implements the <tt>__str__()</tt> function.
         *
         * @return a short text representation of this object.
         */
        std::string toString() const;
        /**
         * Returns the output from writeTextLong() as a string.
         *
         * @return a long text representation of this object.
         */
        std::string toStringLong() const;
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
