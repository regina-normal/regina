
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/*! \file output.h
 *  \brief Provides a common interface for human-readable text output.
 */

#ifndef __OUTPUT_H
#ifndef __DOXYGEN
#define __OUTPUT_H
#endif

#include <sstream>
#include <string>

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup engine
 * @{
 */

/**
 * A common base class for objects that write human-readable text output.
 * This class ensures that text output routines have consistent
 * names and behaviours across Regina's entire API.
 *
 * Two types of output are supported: \e short output, which fits on a
 * single line; and \e detailed output, which may be arbitrarily long.
 *
 * Any class that provides text output should ultimately inherit from
 * this base class.  Your derived class must provide two functions,
 * each of which takes a single std::ostream& argument:
 *
 * - \a writeTextShort(out), which writes the short output to the given
 * output stream;
 *
 * - \a writeTextLong(out), which writes the detailed output to the given
 * output stream.
 *
 * The documentation for str() and detail() gives guidelines as to how
 * this output should be formatted.
 *
 * In return, this class will provide the functions str() and detail(),
 * which return these short and detailed outputs respectively in
 * std::string format.  It will also provide a global operator << that
 * allows you to write objects of type \a T to an arbitrary output stream.
 *
 * If your class is simple and has no need for detailed output then it may
 * derive from ShortOutput instead, which provides a default
 * implementation for \a writeTextLong().
 *
 * \tparam T the class that provides the implementations of
 * \a writeTextShort() and \a writeTextLong().  Typically this will be
 * your own class (i.e., your class \a C derives from Output<C>).
 * However, this may be deeper in the class hierarchy.
 *
 * \note Every object of this class that is ever instantiated \e must be
 * derived from the class \a T.  In other words, end users can construct
 * objects of type \a T (which derives from Output<T>), but they cannot
 * construct objects of the parent class Output<T> itself.
 *
 * \ifacespython Not present, but the output routines str() and detail() will
 * be provided directly through the various subclasses.
 */
template <class T>
struct Output {
    /**
     * Returns a short text representation of this object.
     * This text should be human-readable, should fit on a single line,
     * and should not end with a newline.
     *
     * \ifacespython In addition to str(), this is also used as the
     * Python "stringification" function <tt>__str__()</tt>.
     *
     * @return a short text representation of this object.
     */
    std::string str() const;
    /**
     * Returns a detailed text representation of this object.
     * This text may span many lines, and should provide the user
     * with all the information they could want.  It should be
     * human-readable, should not contain extremely long lines
     * (which cause problems for users reading the output in a terminal),
     * and should end with a final newline.
     *
     * @return a detailed text representation of this object.
     */
    std::string detail() const;
    /**
     * A deprecated alias for str().
     *
     * \deprecated This routine has (at long last) been deprecated;
     * use the simpler-to-type str() instead.
     *
     * @return a short text representation of this object.
     */
    REGINA_DEPRECATED std::string toString() const;
    /**
     * A deprecated alias for detail().
     *
     * \deprecated This routine has (at long last) been deprecated;
     * use the simpler-to-type detail() instead.
     *
     * @return a long text representation of this object.
     */
    REGINA_DEPRECATED std::string toStringLong() const;
};

/**
 * Writes the short text representation of the given object to the
 * given output stream.
 *
 * @param out the output stream to which to write.
 * @param object the object to write.
 * @return a reference to the given output stream.
 */
template <class T>
std::ostream& operator << (std::ostream& out, const Output<T>& object);

/**
 * A common base class for objects that provide short text output only.
 *
 * All classes that provide human-readable text output should ultimately
 * inherit from Output, which provides support for both short output and
 * detailed output.  However, if your class is simple and has no need for
 * detailed output, then you may inherit from ShortOutput instead.
 *
 * You must provide a single function \a writeTextShort(out), which
 * takes a single std::ostream& argument, and which writes the short
 * output to this output stream.
 *
 * In return, this class will provide two routines str() and detail().
 * Both routines will return this short output in string format; the
 * only difference is that detail() will include a final newline,
 * whereas str() will not.
 *
 * See the documentation for Output::str() for further guidelines
 * as to how this output should be formatted.
 *
 * \tparam T the class that provides the implementation of \a writeTextShort().
 * Typically this will be your own class (i.e., your class \a C derives from
 * ShortOutput<C>).  However, this may be deeper in the class hierarchy.
 *
 * \ifacespython Not present, but the output routines str() and detail() will
 * be provided directly through the various subclasses.
 */
template <class T>
struct ShortOutput : public Output<T> {
    /**
     * A default implementation for detailed output.
     * This routine simply calls \a T::writeTextShort() and appends
     * a final newline.
     *
     * \ifacespython Not present.
     *
     * @param out the output stream to which to write.
     */
    void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions

template <class T>
inline std::string Output<T>::str() const {
    std::ostringstream out;
    static_cast<const T*>(this)->writeTextShort(out);
    return out.str();
}

template <class T>
inline std::string Output<T>::detail() const {
    std::ostringstream out;
    static_cast<const T*>(this)->writeTextLong(out);
    return out.str();
}

template <class T>
inline std::string Output<T>::toString() const {
    return str();
}

template <class T>
inline std::string Output<T>::toStringLong() const {
    return detail();
}

template <class T>
inline void ShortOutput<T>::writeTextLong(std::ostream& out) const {
    static_cast<const T*>(this)->writeTextShort(out);
    out << '\n';
}

template <class T>
std::ostream& operator << (std::ostream& out, const Output<T>& object) {
    static_cast<const T&>(object).writeTextShort(out);
    return out;
}

} // namespace regina

#endif
