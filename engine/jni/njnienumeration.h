
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

#ifndef __NJNIENUMERATION_H
#ifndef __DOXYGEN
#define __NJNIENUMERATION_H
#endif

/*! \file njnienumeration.h
 *  \brief Provides wrappers to aid JNI interaction between Java
 *  enumerations and C++ container classes.
 */

#include "shareableobject.h"

namespace regina {

namespace jni {

/**
 * \weakgroup jni
 * @{
 */

/**
 * A shareable object that can provide the underlying implementation of a
 * Java enumeration over a C++ container class.
 *
 * This class essentially links the Java interface
 * <tt>java.util.Enumeration</tt> with the C++ iterator type
 * <tt>ContainerClass::const_iterator</tt>.
 */
template <class ContainerClass>
class NJNIEnumeration : public ShareableObject {
    private:
        const ContainerClass& container;
            /**< The container whose elements we are iterating over. */
        typename ContainerClass::const_iterator current;
            /**< The iterator corresponding to this Java enumeration. */

    public:
        /**
         * Creates a new enumeration over the elements of the given
         * container.
         *
         * @param newContainer the container whose elements we will
         * iterate over.
         */
        NJNIEnumeration(const ContainerClass& newContainer) :
                container(newContainer), current(newContainer.begin()) {
        }

        /**
         * Determines whether there are still more elements to iterate
         * over.
         *
         * @return \c true if and only if there are still more elements.
         */
        bool hasMoreElements() const {
            return (current != container.end());
        }

        /**
         * Returns the next element in this enumeration.
         *
         * \pre There are still more elements to iterate over.
         *
         * @return the next element.
         */
        const typename ContainerClass::value_type& nextElement() {
            return *current++;
        }

        void writeTextShort(std::ostream& out) const {
            out << "C++ container class enumerator";
        }
};

/*@}*/

} } // namespace regina::jni

#endif

