
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

/**
 * Facilitates mirroring objects in the underlying C++ calculation
 * engine using the various wrapper classes provided in the various
 * external interfaces (such as the Java interface).
 * Any object that is to be so mirrored should be derived
 * from this class.
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
class ShareableObject {
    public:
        /**
         * Default constructor that does nothing.
         *
         * \ifaces Not present.
         */
        ShareableObject();
        /**
         * Default destructor that does nothing.
         */
        virtual ~ShareableObject();

        /**
         * Determines if this and the given wrapper both refer to the
         * same underlying C++ engine object.
         * If both wrappers refer to nothing, this function will return
         * \c true.
         *
         * \ifacescpp Not present.
         * \ifacesjava The function <tt>Object.equals()</tt> is also
         * overridden to call this function after first verifying that
         * the other object is also a shareable object.
         *
         * @param other the wrapper to compare with this one.  This may be
         * a null object if such a thing exists in the interface
         * language (for instance, in Java this may be <tt>null</tt>);
         * if a null object is passed, this routine will simply return
         * \c false.
         * @return \c true if and only if both this and the other
         * wrapper refer to the same underlying engine object.
         */
        #ifdef __DOXYGEN
        bool sameObject(const ShareableObject& other) const;
        #endif
        /**
         * Destroys (deallocates) the underlying C++ engine object that
         * this wrapper refers to.  This wrapper will become invalid.
         *
         * \pre This wrapper in fact refers to an object
         * in the underlying C++ engine.
         *
         * \ifacescpp Not present.
         */
        #ifdef __DOXYGEN
        void destroy();
        #endif
        /**
         * Returns a pointer to the underlying C++ engine object that
         * this wrapper refers to.
         * The pointer will be cast as a long integer.
         *
         * \ifacescpp Not present.
         * \ifaces Not present outside the CORBA interface.
         *
         * @return a pointer to the underlying C++ engine object.
         */
        #ifdef __DOXYGEN
        long getCppPtr();
        #endif
        /**
         * Returns a new wrapper for this object using the given wrapper
         * class.
         * The new wrapper will wrap the same object as this wrapper,
         * but will be of the given subclass or superclass
         * of this wrapper's class.
         *
         * \pre The underlying C++ engine object that is being wrapped is
         * actually of the engine class corresponding to the given
         * wrapper class \a cls.
         *
         * \ifacescpp Not present.
         * \ifacescorba Not present.
         * \ifacesjava Parameter \a cls should be of type
         * <tt>java.lang.Class</tt>.  If the Java interface is being
         * used as a CORBA client, the client CORBA object
         * will also be appropriately narrowed but it will be
         * assumed that the server CORBA object
         * is already of the correct CORBA class.
         *
         * @param cls the wrapper class that the new wrapper shall be an
         * instance of; this must be an instantiatable subclass or superclass
         * of whichever class this wrapper is an instance of.
         * @return the new wrapper.
         */
        #ifdef __DOXYGEN
        ShareableObject castAs(Class cls);
        #endif

        /**
         * Writes this object in short text format to the given output stream.
         * The output should fit on a single line and no newline should
         * be written.
         *
         * \ifaces The parameter \a out does not exist;
         * standard output will be used.
         * \ifacescorba Not present.
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
         * \ifaces The parameter \a out does not exist;
         * standard output will be used.
         * \ifacescorba Not present.
         *
         * @param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const;
        /**
         * Returns the output from writeTextShort() as a string.
         *
         * \ifacesjava This overrides <tt>Object.toString()</tt>.
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

// Inline functions for ShareableObject

inline ShareableObject::ShareableObject() {
}
inline ShareableObject::~ShareableObject() {
}

inline void ShareableObject::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << '\n';
}

#endif
