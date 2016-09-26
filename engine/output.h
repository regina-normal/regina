
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

/*! \file output.h
 *  \brief Provides a common interface for human-readable text output.
 */

#ifndef __OUTPUT_H
#ifndef __DOXYGEN
#define __OUTPUT_H
#endif

#include <sstream>
#include <string>
#include <type_traits>

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup engine
 * @{
 */

#ifndef __DOXYGEN
template <class T, bool supportsUtf8 = false> struct Output;
#else
/**
 * A common base class for objects that write human-readable text output.
 * This class ensures that text output routines have consistent
 * names and behaviours across Regina's entire API.
 *
 * Three types of output are supported:
 *
 * - \e short output, which fits on a single line and uses plain ASCII
 *   characters wherever possible;
 * - \e utf8 output, which is like short output but supports the much richer
 *   unicode character set; and
 * - \e detailed output, which may be arbitrarily long.
 *
 * Any class that provides text output should ultimately inherit from
 * this base class.  Your derived class must provide two functions:
 *
 * - <tt>writeTextShort(std::ostream& out, bool utf8 = false)</tt>, which writes
 *   either the short output or the utf8 output to the given output stream,
 *   according to whether \a utf8 is \c false or \c true respectively;
 *
 * - <tt>writeTextLong(std::ostream& out)</tt>, which writes the detailed
 *   output to the given output stream.
 *
 * The boolean \a utf8 argument to writeTextShort() must be optional.
 * Moreover, if your class does not benefit from unicode characters (i.e.,
 * the short and utf8 outputs are identical), then you may omit the \a utf8
 * argument entirely; in this case, you must set the template argument
 * \a supportsUtf8 as \c false.  Both \a writeTextShort() and
 * \a writeTextLong() may take additional arguments, as long as they are
 * optional.
 *
 * The documentation for str(), utf8() and detail() gives guidelines as to how
 * the various types of output should be formatted.
 *
 * In return, this class will provide the functions str(), utf8() and detail(),
 * which return the short, utf8 and detailed outputs respectively in
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
 * \tparam supportsUtf8 \c true if the class \a T can make use of the
 * richer unicode character set, or \c false if the short and utf8 outputs
 * are identical.  If this is \c false then T::writeTextShort() will only
 * ever be called in the form <tt>writeTextShort(std::ostream&)</tt>,
 * and you may for simplicity omit the second boolean \a utf8 argument.
 * This Output base class will still provide a utf8() function, but it will
 * return the same output as short().
 *
 * \note Every object of this class that is ever instantiated \e must be
 * derived from the class \a T.  In other words, end users can construct
 * objects of type \a T (which derives from Output<T>), but they cannot
 * construct objects of the parent class Output<T> itself.
 *
 * \ifacespython Not present, but the output routines str(), utf8() and
 * detail() will be provided directly through the various subclasses.
 */
template <class T, bool supportsUtf8 = false>
struct Output {
    /**
     * Returns a short text representation of this object.
     * This text should be human-readable, should fit on a single line,
     * and should not end with a newline.  Where possible, it should use
     * plain ASCII characters.
     *
     * \ifacespython In addition to str(), this is also used as the
     * Python "stringification" function <tt>__str__()</tt>.
     *
     * @return a short text representation of this object.
     */
    std::string str() const;
    /**
     * Returns a short text representation of this object using unicode
     * characters.  Like str(), this text should be human-readable, should
     * fit on a single line, and should not end with a newline.  In
     * addition, it may use unicode characters to make the output more
     * pleasant to read.  This string will be encoded in UTF-8.
     *
     * @return a short text representation of this object.
     */
    std::string utf8() const;
    /**
     * Returns a detailed text representation of this object.
     * This text may span many lines, and should provide the user
     * with all the information they could want.  It should be
     * human-readable, should not contain extremely long lines
     * (which cause problems for users reading the output in a terminal),
     * and should end with a final newline.  There are no restrictions
     * on the underlying character set.
     *
     * @return a detailed text representation of this object.
     */
    std::string detail() const;
};

/**
 * Writes the short text representation of the given object to the
 * given output stream.
 *
 * This is equivalent to calling <tt>out << object.str()</tt>.
 *
 * @param out the output stream to which to write.
 * @param object the object to write.
 * @return a reference to the given output stream.
 */
template <class T, bool supportsUtf8>
std::ostream& operator << (std::ostream& out,
    const Output<T, supportsUtf8>& object);

#endif // __DOXYGEN

/**
 * A common base class for objects that provide short text output only.
 *
 * All classes that provide human-readable text output should ultimately
 * inherit from Output, which provides support for short output (both
 * with and without unicode support) as well as detailed output.
 * However, if your class is simple and has no need for detailed output,
 * then you may inherit from ShortOutput instead.
 *
 * If you inherit from this class, then you only need to provide a single
 * function \a writeTextShort(), as described in the Output class notes.
 * You do not need to provide \a writeTextLong(); this class will implement
 * it for you.
 *
 * As with Output, this class will provide three routines str(), utf8() and
 * detail().  The function detail() will return the same output as str(),
 * but with a final newline added.
 *
 * \tparam T the class that provides the implementation of \a writeTextShort().
 * Typically this will be your own class (i.e., your class \a C derives from
 * ShortOutput<C>).  However, this may be deeper in the class hierarchy.
 *
 * \tparam supportsUtf8 \c true if the class \a T can make use of the
 * richer unicode character set, or \c false if the short and utf8 outputs
 * are identical.  If this is \c false then T::writeTextShort() will only
 * ever be called in the form <tt>writeTextShort(std::ostream&)</tt>,
 * and you may for simplicity omit the second boolean \a utf8 argument.
 * This ShortOutput base class will still provide a utf8() function, but it
 * will return the same output as short().
 *
 * \ifacespython Not present, but the output routines str(), utf8() and
 * detail() will be provided directly through the various subclasses.
 */
template <class T, bool supportsUtf8 = false>
struct ShortOutput : public Output<T, supportsUtf8> {
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

/**
 * Provides a typedef to help identify where in the class hierarchy the
 * output functions T::str() and T::detail() are implemented.
 *
 * If \a T is a class derived (directly or indirectly) from some class
 * Output<...>, then <tt>OutputBase<T>::type</tt> is defined to be this
 * parent class Output<...>.
 *
 * If \a T is not derived from any class Output<...>, then
 * <tt>OutputBase<T>::type</tt> is defined to be \a T itself.
 *
 * This helper class can be useful when trying to disambiguate between the
 * implementation of str() that is inherited from Output, versus an extended
 * implementation of str() (perhaps with more arguments) that is implemented
 * in the class \a T itself.
 *
 * \pre \a T is a class or struct type.
 */
template <class T>
struct OutputBase {
    private:
        // Implementation details:
        static T& test(...);

        template <typename U, bool supportsUtf8>
        static Output<U, supportsUtf8>& test(const Output<U, supportsUtf8>&);

    public:
        /**
         * The class in which T::str() and T::detail() are implemented.
         *
         * If \a T is derived from the Output template class, then this
         * type is the corresponding Output<X> base class.
         * Otherwise, this type is \a T itself.
         *
         * \note The implementation of this typedef does not look for
         * str() or detail() at all.  Instead, it is based purely on the
         * inheritance condition as stated above.
         */
        typedef typename std::remove_reference<
            decltype(test(std::declval<T>()))>::type type;
};

/*@}*/

// Inline functions

#ifndef __DOXYGEN
template <class T>
struct Output<T, true> {
    inline std::string str() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextShort(out, false);
        return out.str();
    }
    inline std::string utf8() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextShort(out, true);
        return out.str();
    }
    inline std::string detail() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextLong(out);
        return out.str();
    }
};

template <class T>
struct Output<T, false> {
    inline std::string str() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextShort(out);
        return out.str();
    }
    inline std::string utf8() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextShort(out);
        return out.str();
    }
    inline std::string detail() const {
        std::ostringstream out;
        static_cast<const T*>(this)->writeTextLong(out);
        return out.str();
    }
};

template <class T>
inline std::ostream& operator << (std::ostream& out,
        const Output<T, true>& object) {
    static_cast<const T&>(object).writeTextShort(out, false);
    return out;
}

template <class T>
inline std::ostream& operator << (std::ostream& out,
        const Output<T, false>& object) {
    static_cast<const T&>(object).writeTextShort(out);
    return out;
}
#endif // __DOXYGEN

template <class T, bool supportsUtf8>
inline void ShortOutput<T, supportsUtf8>::writeTextLong(std::ostream& out)
        const {
    // Plain ASCII short output, with an extra newline:
    static_cast<const T*>(this)->writeTextShort(out);
    out << '\n';
}

} // namespace regina

#endif
