
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

/*! \file utilities/base64.h
 *  \brief Routines for base64 encoding and decoding taken and modified
 *  from the \a Base64 project at base64.sourceforge.net.
 *
 *  The \a Base64 copyright notice is as follows:
 *
 *  Copyright (c) 2001 Bob Trower, Trantor Standard Systems Inc.
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated
 *  documentation files (the "Software"), to deal in the
 *  Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute,
 *  sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall
 *  be included in all copies or substantial portions of the
 *  Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 *  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 *  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BASE64_H
#ifndef __DOXYGEN
#define __BASE64_H
#endif

#include "regina-core.h"
#include <cstddef> // for size_t

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Returns the number of base64 characters required to encode the given
 * number of bytes.  This is the number of characters used (excluding the
 * null terminator) by the routine base64Encode(const char*, size_t, char**).
 *
 * \ifacespython Not present.
 *
 * @param bytes the number of raw input bytes.
 * @return the corresponding number of base64 printable output characters.
 */
REGINA_API size_t base64Length(size_t bytes);

/**
 * Determines whether the given character is a base64 printable character as
 * used by the base64 routines in Regina.  The base64 printable characters
 * are the letters (both upper-case and lower-case), digits, plus (+), and
 * forward slash (/).
 *
 * Note that the equals sign (=) is padding, and is not considered by
 * this routine to be a base64 printable character.
 *
 * \ifacespython Not present.
 *
 * @param ch any character.
 * @return \c true if the given character is one of the base64 printable
 * characters used in Regina, or \c false if it is not.
 */
REGINA_API bool isBase64(char ch);

/**
 * Encodes the given sequence of raw bytes in base64, and writes the
 * results into a preallocated output buffer.
 *
 * The length of the output buffer is passed as the argument \a outlen.
 * If the number of base64 characters required is less than \a outlen,
 * a terminating \c null will be written to the end of the output sequence.
 * If the number of base64 characters is \a outlen or greater, this
 * routine will output as many base64 characters as possible, up to a
 * maximum of \a outlen.
 *
 * The routine base64Length() can be used to precalculate precisely how
 * many output characters will be required.
 *
 * \ifacespython Not present.
 *
 * @param in the sequence of input bytes; this does not need to be
 * terminated in any special way.
 * @param inlen the length of the input sequence.
 * @param out the output buffer into which the resulting base64
 * characters will be written.
 * @param outlen the length of the output buffer.
 *
 * @author This routine is based on the \a Base64 project at
 * base64.sourceforge.net.  The original was written by Bob Trower, and is
 * licensed under the MIT license.  See the base64.h notes for details.
 */
REGINA_API void base64Encode(const char* in, size_t inlen, char* out,
    size_t outlen);

/**
 * Encodes the given sequence of raw bytes in base64, and passes back a
 * newly allocated array containing the results.  The \a out pointer will
 * be set to this new array, which will be null-terminated.  This array will
 * be allocated using \c new[], and the caller is responsible for destroying
 * it using \c delete[].
 *
 * If the output array is too large (in particular, the expected size
 * will overflow a \c size_t), the \a out pointer will be set to \c null.
 *
 * \ifacespython Not present.
 *
 * @param in the sequence of input bytes; this does not need to be
 * terminated in any special way.
 * @param inlen the length of the input sequence.
 * @param out the address of a pointer which will be set to the output
 * array of base64 characters.
 * @return the length of the output array, not counting the terminating null.
 *
 * @author This routine is based on the \a Base64 project at
 * base64.sourceforge.net.  The original was written by Bob Trower, and is
 * licensed under the MIT license.  See the base64.h notes for details.
 */
REGINA_API size_t base64Encode(const char* in, size_t inlen, char** out);

/**
 * Decodes the given sequence of base64 characters, and writes the
 * resulting raw bytes into a preallocated output buffer.
 *
 * The given base64 sequence should not contain any unexpected characters;
 * even whitespace will cause the decoding procedure to abort.
 *
 * The length of the output buffer is passed as the argument \a outlen.
 * If an unexpected or invalid character is found, or the output
 * buffer is exhausted, this routine will write as many output bytes as
 * it can and then return \c false.  Otherwise (on success) it will return
 * \c true.  Either way, it will reset \a outlen to the total number of
 * bytes that were written.
 *
 * The total number of output bytes is important to know, since the output
 * array is not terminated in any special way.
 *
 * \ifacespython Not present.
 *
 * @param in the input sequence of base64 characters; this does not need
 * to be terminated in any special way.
 * @param inlen the length of the input sequence.
 * @param out the output buffer into which the resulting raw bytes
 * will be written.
 * @param outlen must contain the length of the output buffer on entry, and
 * on exit contains the number of output bytes that were successfully written.
 * @return \c true if decoding was successful, or \c false if the output
 * buffer was exhausted or an unexpected input character was found.
 *
 * @author This routine is based on the \a Base64 project at
 * base64.sourceforge.net.  The original was written by Bob Trower, and is
 * licensed under the MIT license.  See the base64.h notes for details.
 */
REGINA_API bool base64Decode(const char* in, size_t inlen, char* out,
    size_t* outlen);

/**
 * Decodes the given sequence of base64 characters, and passes back a
 * newly allocated array containing the results.  The \a out pointer
 * will be set to this new array, and \a outlen will be set to the
 * number of raw bytes in this output array.  This array will be
 * allocated using \c new[], and the caller is responsible for
 * destroying it using \c delete[].
 *
 * The given base64 sequence should not contain any unexpected characters;
 * even whitespace will cause the decoding procedure to abort.
 *
 * The length of the output buffer is passed as the argument \a outlen.
 * If an unexpected or invalid character is found or the output
 * buffer is exhausted, this routine will return \c false, set \a out
 * to \c null, and leave \a outlen undefined.  Otherwise (on success) it
 * will return \c true and set \a outlen to the total number of output bytes.
 *
 * If the user is not interested in the length of the output array, a
 * null pointer may be passed in the \a outlen argument.  Note however
 * that the output array is not terminated in any special way.
 *
 * \ifacespython Not present.
 *
 * @param in the input sequence of base64 characters; this does not need
 * to be terminated in any special way.
 * @param inlen the length of the input sequence.
 * @param out the address of a pointer which will be set to the output
 * array of raw bytes (or which will be set to \c null on failure).
 * @param outlen the address of an integer which will be set to the
 * length of the output array (or which will be left undefined on failure).
 * @return \c true if decoding was successful, or \c false if an unexpected
 * input character was found or some other error occurred.
 *
 * @author This routine is based on the \a Base64 project at
 * base64.sourceforge.net.  The original was written by Bob Trower, and is
 * licensed under the MIT license.  See the base64.h notes for details.
 */
REGINA_API bool base64Decode(const char* in, size_t inlen, char** out,
    size_t* outlen);

/*@}*/

// Inline functions:

inline size_t base64Length(size_t bytes) {
    return (((bytes + 2) / 3) * 4);
}

inline bool isBase64(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9') || ch == '+' || ch == '/');
}

} // namespace regina

#endif
