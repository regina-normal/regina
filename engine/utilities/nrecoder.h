
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nrecoder.h
 *  \brief Provides basic facilities for converting between character sets.
 */

#ifndef __NRECODER_H
#ifndef __DOXYGEN
#define __NRECODER_H
#endif

#include "utilities/nthread.h"
#include <iconv.h>
#include <string>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An object for converting between different character encodings.
 *
 * To convert one or more strings from one encoding to another, (i) create a
 * new NRecoder object with the relevant encodings, and then (ii) use
 * the bracket operator to convert the strings.  For example:
 *
 * \code
 * const std::string latinA = "...";
 * const std::string latinB = "...";
 * std::string utfA, utf8B;
 * {
 *     regina::NRecoder r("ISO-8859-1", "UTF-8");
 *     utfA = r(latinA);
 *     utfB = r(latinB);
 * }
 * \endcode
 *
 * If one or both encodings are not recognised, isValid() will return
 * \c false but otherwise the bracket operator will silently pass
 * strings through without modifications.
 *
 * This class uses \e libiconv to do its work.  Since the \e libiconv
 * library does not support multithreading, at most one NRecoder object
 * may exist at any given time.  This is enforced by a mutex.  As a
 * result it is advised to destroy an NRecoder object as soon as it is
 * no longer required.
 *
 * \warning NRecoder objects should be given as small a scope as
 * possible, i.e., they should be destroyed as soon as they are no
 * longer required.  See the multithreading note above for reasons why.
 */
class NRecoder {
    private:
        static NMutex mutex;
            /**< A global lock that ensures that only one NRecoder is
                 operating at any given time. */
        NMutex::MutexLock lock;
            /**< The lock that this converter holds on the global mutex. */

        iconv_t converter;
            /**< The internal iconv conversion descriptor. */

    public:
        /**
         * Creates a new object for converting between the given
         * encodings.  The encodings should be specified by plain C
         * strings.  See the \e iconv documentation (for instance,
         * the \e iconv_open() man page, or the output from
         * <tt>iconv --list</tt>) for a list of supported encodings.
         *
         * This routine locks the global NRecoder mutex.  If some other
         * NRecoder object exists in another thread, this constructor
         * will block until it has been destroyed.
         *
         * @param from the encoding to convert from.
         * @param to the encoding to convert to.
         */
        NRecoder(const char* from, const char* to);
        /**
         * Destroys this converter and unlocks the global NRecoder mutex.
         */
        ~NRecoder();

        /**
         * Is this converter able to convert between the requested
         * encodings?
         *
         * @return \c true if both character sets are recognised and the
         * conversion is supported by the underlying \e libiconv, or
         * \c false otherwise.
         */
        bool isValid() const;

        /**
         * Converts the given string from one encoding to another.
         * The relevant encodings are passed in the class constructor.
         *
         * If the underlying \e libiconv does not recognise one or both
         * encodings or cannot perform the conversion for some other
         * reason, the string will be passed through unchanged.  The
         * isValid() routine can be used to explicitly check for error
         * conditions such as this.
         *
         * \warning By default, this routine resets the state of the internal
         * \e iconv converter after each conversion.  If you expect to
         * be splittng multibyte sequences across different strings, the
         * \a reset argument must be explicitly set to \c false.
         *
         * @param convertMe the string to convert.
         * @param reset \c true if the internal \e iconv converter
         * should reset its state after the conversion, or \c false if
         * it should not.
         * @return the given string, converted to the new encoding.
         */
        std::string operator() (const std::string& convertMe,
            bool reset = true);
};

/*@}*/

// Inline functions for NRecoder

inline NRecoder::NRecoder(const char* from, const char* to) :
        lock(mutex), converter(iconv_open(to, from)) {
}

inline NRecoder::~NRecoder() {
}

inline bool NRecoder::isValid() const {
    return (converter != (iconv_t)(-1));
}

} // namespace regina

#endif

