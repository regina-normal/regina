
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file utilities/tightencoding.h
 *  \brief Provides short ASCII printable encodings for various objects.
 */

#ifndef __REGINA_TIGHTENCODING_H
#ifndef __DOXYGEN
#define __REGINA_TIGHTENCODING_H
#endif

#include <iostream>
#include <string>
#include <sstream>
#include "concepts/core.h"
#include "concepts/iterator.h"
#include "utilities/exception.h"

namespace regina {

template <bool>
class IntegerBase;

/*! \page tight Tight encodings of data
 *
 * Regina includes support for _tight encodings_, which are
 * encodings of various data types as short printable strings.
 *
 * Tight encodings have the following properties:
 *
 * - They use only printable ASCII characters (the 94 ASCII values from
 *   33 to 126 inclusive), and do not contain any whitespace.  This means
 *   (for example) you can use them as whitespace-separated tokens in plain
 *   text files.  However, they do make use of _all_ of the ASCII punctuation
 *   symbols, and so you must take care when (for example) trying to hard-code
 *   them as strings in source code, or using them as components of filenames.
 *
 * - They aim to be short (typically much shorter than the usual human-readable
 *   representations, such as decimal representations of integers or full text
 *   representations of polynomials).
 *
 * - When reading an encoded object character-by-character, the encoding
 *   contains enough information to know when the last character has been read
 *   (even without the foreknowledge of whether the input stream contains more
 *   characters).
 *
 * - Objects with the same inherent value, even if they use different
 *   underlying C++ types, will encode to the same string.  For example,
 *   the integer 7 will have the same encoding regardless of whether it
 *   is stored as an \c int, a \c long, or a regina::Integer.  Note that
 *   this guarantee only extends to types that "conceptually" intend to
 *   represent the same broad types of objects, possibly with different
 *   limitations.  So, for example, there is no guarantee that the integer 7,
 *   the rational 7/1, and/or the constant _polynomial_ 7 would encode to
 *   the same string.
 *
 * - Conversely, objects of the same type but with different inherent values
 *   will encode to different strings.  So, for example, the integers 7 and -7
 *   will have different encodings.
 *
 * A consequence of the last two points is that, if the _type_ of an object
 * is known in advance, then its _value_ can be recovered from its encoding.
 * However, the encoding does not contain enough information to deduce the
 * type if this is not already known.
 *
 * Because encodings contain enough information to identify where they end,
 * this means that you can encode a _sequence_ of objects by concatenating
 * the individual encodings with no separators, and (assuming the types
 * of the objects are fixed) this will be enough to guarantee that
 * different _sequences_ likewise have different encodngs.  Of course, a
 * _tight encoding_ of a sequence must encode extra information so that the
 * sequence itself maintains the property that the end of its encoding can
 * be detected without look-ahead - typically this extra information would be
 * either a length written at the beginning, or a sentinel written at the end.
 *
 * Tight encodings were originally designed to support perfect hashing
 * (essentially "compressing" data into a short printable string whilst
 * preserving the correctness of equality tests).  As a result, they were
 * originally intended to be used only in one direction.  However, Regina
 * does provide matching _decoding_ routines if you need to reconstruct
 * objects from their tight encodings.
 *
 * For native C++ data types where tight encodings and decodings are supported,
 * these are provided in the header utilities/tightencoding.h through overloads
 * of the global functions tightEncoding() and tightDecoding() (which work
 * with strings), and tightEncode() and tightDecode() (which work with
 * input/output streams).
 *
 * For Regina's own data types where tight encodings and decodings are
 * supported, these are provided through member functions tightEncoding(),
 * tightEncode(), tightDecoding() and tightDecode() within the corresponding
 * classes.
 *
 * Note that classes that provide a tightEncoding() member function will
 * typically also provide a hash() member function (which often uses the
 * tight encoding in its implementation).  Unlike tight encodings, which are
 * string-based and preserve equality/inequality perfectly, hashes map into a
 * fixed-size integer range and so may have collisions (i.e., different objects
 * may have the same hash value).  In short: tight encodings are designed for
 * compression and printability, whereas hashes are designed to be used as
 * integer keys in hash tables (and, for Python users, dictionaries and sets).
 */

/**
 * A base class that assists with support for tight encodings and
 * corresponding decodings.
 *
 * If a class \a T supports tight encodings, then it _may_ derive from
 * TightEncodable<T>.  If it does, then your derived class must provide
 * the following two functions, which implement tight encodings and decodings
 * via input/output streams:
 *
 * - `void tightEncode(std::ostream&) const`, which writes a tight
 *   encoding of the object to the given output stream.  This is allowed
 *   to (but not required to) throw a FailedPrecondition if the object
 *   is in an invalid state; if so then the exception should be documented
 *   in this member function T::tightEncode().
 *
 * - `static T tightDecode(std::istream&)`, which reconstructs an
 *   object of type \a T from a tight encoding that is read from the given
 *   input stream.  This routine must not skip leading whitespace, and must
 *   leave the input stream positioned immediately after the encoding
 *   (without consuming any trailing whitespace or other characters).
 *   This should throw an InvalidInput exception if the input stream
 *   does not begin with a valid tight encoding of an object of type \a T.
 *
 * In return, this base class will provide the following three functions,
 * which work with simpler (non-stream) data types, and which are documented
 * in full below:
 *
 * - `std::string tightEncoding() const`;
 *
 * - `static T tightDecoding(const std::string&)`; and
 *
 * - `size_t hash() const`.
 *
 * A class \a T that supports tight encodings does not _need_ to derive from
 * TightEncodable.  However, if it does not then it should implement all five
 * of the above functions itself.  Examples of this include the permutation
 * classes (which have optimised implementations due to their very small space
 * requirements), and the arbitrary-precision integer classes (which use the
 * global integer encoding/decoding routines and a simple arithmetic hash).
 *
 * \tparam T the type of object being encoded/decoded; this must derive
 * from TightEncodable<T>.
 *
 * \note Every object of this class that is ever instantiated _must_ be
 * derived from the class \a T.  In other words, end users cannot
 * construct objects of the parent class TightEncodable<T>.
 *
 * \python Not present, but the routines tightEncoding(), tightDecoding()
 * and hash() will be provided directly through the various subclasses.
 *
 * \ingroup utilities
 */
template <typename T>
struct TightEncodable {
    /**
     * Returns the tight encoding of this object.
     * See the page on \ref tight "tight encodings" for details.
     *
     * \exception FailedPrecondition This may be thrown for some classes
     * \a T if the object is in an invalid state.  If this is possible, then
     * a more detailed explanation of "invalid" can be found in the class
     * documentation for \a T, under the member function T::tightEncode().
     * See FacetPairing::tightEncode() for an example of this.
     *
     * \return the resulting encoded string.
     */
    std::string tightEncoding() const {
        std::ostringstream out;
        static_cast<const T*>(this)->tightEncode(out);
        return out.str();
    }

    /**
     * Reconstructs an object of type \a T from its given tight encoding.
     * See the page on \ref tight "tight encodings" for details.
     *
     * The tight encoding should be given as a string.  If this string
     * contains leading whitespace or any trailing characters at all
     * (including trailing whitespace), then it will be treated as
     * an invalid encoding (i.e., this routine will throw an exception).
     *
     * \exception InvalidArgument The given string is not a tight encoding
     * of an object of type \a T.
     *
     * \param enc the tight encoding for an object of type \a T.
     * \return the object represented by the given tight encoding.
     */
    static T tightDecoding(const std::string& enc) {
        std::istringstream in(enc);
        try {
            T ans = T::tightDecode(in);
            if (in.get() != EOF)
                throw InvalidArgument(
                    "The tight encoding has trailing characters");
            return ans;
        } catch (const InvalidInput& exc) {
            // For string-based decoding we use a different exception type.
            throw InvalidArgument(exc.what());
        }
    }

    /**
     * Hashes this object to a non-negative integer, allowing it to be used
     * for keys in hash tables.
     *
     * This hash function makes use of Regina's tight encodings.  In
     * particular, any two objects with the same tight encoding will have equal
     * hashes.  This implementation (and therefore the specific hash value for
     * each object) is subject to change in future versions of Regina.
     *
     * \python For Python users, this function uses the standard Python
     * name __hash__().  This allows objects of this type to be used as
     * keys in Python dictionaries and sets.
     *
     * \return The integer hash of this object.
     */
    size_t hash() const {
        return std::hash<std::string>{}(tightEncoding());
    }
};

/**
 * Writes the tight encoding of the given signed integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \nopython Use regina::tightEncoding(int) instead, which returns a string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, int value);

/**
 * Returns the tight encoding of the given signed integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(int value);

/**
 * Writes the tight encoding of the given signed long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \nopython Use regina::tightEncoding(long) instead, which returns a string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, long value);

/**
 * Returns the tight encoding of the given signed long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(long value);

/**
 * Writes the tight encoding of the given signed long long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \nopython Use regina::tightEncoding(long long) instead, which returns a
 * string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, long long value);

/**
 * Returns the tight encoding of the given signed long long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(long long value);

/**
 * Writes the tight encoding of the given unsigned integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \nopython Use regina::tightEncoding(int) instead, which returns a string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned value);

/**
 * Returns the tight encoding of the given unsigned integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned value);

/**
 * Writes the tight encoding of the given unsigned long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \nopython Use regina::tightEncoding(long) instead, which returns a string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned long value);

/**
 * Returns the tight encoding of the given unsigned long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned long value);

/**
 * Writes the tight encoding of the given unsigned long long integer to the
 * given output stream.  See the page on \ref tight "tight encodings" for
 * details.
 *
 * \nopython Use regina::tightEncoding(long long) instead, which returns a
 * string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned long long value);

/**
 * Returns the tight encoding of the given unsigned long long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned long long value);

/**
 * Writes the tight encoding of the given boolean to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * The booleans \c true and \c false are guaranteed to have the same
 * tight encodings as the integers 1 and 0 respectively.
 *
 * \nopython Use regina::tightEncoding(bool) instead, which returns a string.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the boolean to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, bool value);

/**
 * Returns the tight encoding of the given boolean.
 * See the page on \ref tight "tight encodings" for details.
 *
 * The booleans \c true and \c false are guaranteed to have the same
 * tight encodings as the integers 1 and 0 respectively.
 *
 * \param value the boolean to encode.
 * \return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(bool value);

/**
 * Reconstructs an integer or boolean from its given tight encoding.
 * See the page on \ref tight "tight encodings" for details.
 *
 * The tight encoding should be given as a string.  If this string contains
 * leading whitespace or any trailing characters at all (including trailing
 * whitespace), then it will be treated as an invalid encoding (i.e., this
 * routine will throw an exception).
 *
 * This routine does recognise infinity in the case where \a Int is the type
 * regina::LargeInteger.
 *
 * If \a Int is one of Regina's arbitrary precision integer types, then
 * this routine is identical to calling Int::tightDecoding().
 *
 * \headers Some components of this routine are implemented in a separate
 * header (tightencoding-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for all of the
 * types that have corresponding global tightEncode() functions
 * (i.e., \c bool; signed and unsigned \c int, \c long, and \c long \c long;
 * and regina::Integer and regina::LargeInteger).
 *
 * \exception InvalidArgument The given string is not a tight encoding of an
 * integer/boolean of type \a Int.  This includes the case where the encoding
 * _is_ a valid integer encoding but the integer itself is outside the
 * allowed range for the \a Int type.
 *
 * \python Since Python does not support templates, the interface
 * for this routine is a little different.  The global routine
 * regina::tightDecoding() will return a Python integer; since these are
 * arbitrary precision, the decoding will never encounter an out-of-range
 * exceptions as it might with a native C++ integer type.  If you are trying
 * to reconstruct a boolean, then the integer you receive will be either 1 or 0
 * to represent \c true or \c false respectively.  If you are trying to
 * reconstruct one of Regina's arbitrary precision integer types, you should
 * instead call Integer::tightDecoding() or LargeInteger::tightDecoding(), which
 * will return a Regina integer instead of a Python integer.
 *
 * \tparam Int The type of integer/boolean to reconstruct; this must be either
 * (i) a native C++ integer type or \c bool, or (ii) one of Regina's arbitrary
 * precision integer types (i.e., regina::Integer or regina::LargeInteger).
 *
 * \param enc the tight encoding for an integer or boolean.
 * \return the integer or boolean represented by the given tight encoding.
 */
template<typename Int>
Int tightDecoding(const std::string& enc);

/**
 * Reconstructs an integer or boolean from its given tight encoding.
 * See the page on \ref tight "tight encodings" for details.
 *
 * The tight encoding will be read from the given input stream.  If the input
 * stream contains leading whitespace then it will be treated as an invalid
 * encoding (i.e., this routine will throw an exception).  The input stream
 * _may_ contain further data: if this routine is successful then the input
 * stream will be left positioned immediately after the encoding, without
 * skipping any trailing whitespace.
 *
 * This routine does recognise infinity in the case where \a Int is the type
 * regina::LargeInteger.
 *
 * If \a Int is one of Regina's arbitrary precision integer types, then
 * this routine is identical to calling Int::tightDecode().
 *
 * \headers Some components of this routine are implemented in a separate
 * header (tightencoding-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for all of the
 * types that have corresponding global tightEncode() functions
 * (i.e., \c bool; signed and unsigned \c int, \c long, and \c long \c long;
 * and regina::Integer and regina::LargeInteger).
 *
 * \exception InvalidInput The given input stream does not begin with a tight
 * encoding of an integer/boolean of type \a Int.  This includes the case
 * where the encoding _is_ a valid integer encoding but the integer itself
 * is outside the allowed range for the \a Int type.
 *
 * \nopython Use regina::tightDecoding() instead, which takes a string as its
 * argument.
 *
 * \tparam Int The type of integer/boolean to reconstruct; this must be either
 * (i) a native C++ integer type or \c bool, or (ii) one of Regina's arbitrary
 * precision integer types (i.e., regina::Integer or regina::LargeInteger).
 *
 * \param input an input stream that begins with the tight encoding for an
 * integer or boolean.
 * \return the integer or boolean represented by the given tight encoding.
 */
template<typename Int>
Int tightDecode(std::istream& input);

namespace detail {
    /**
     * Internal function that writes the tight encoding of the given integer
     * to the given output stream.  This should not be called directly;
     * its purpose is to provide a common implementation for tightEncode()
     * and tightEncoding() for all integer types.
     *
     * This routine does support passing infinity as the given value (which is
     * only relevant when the integer type \a Int is regina::LargeInteger).
     *
     * \nopython Use regina::tightEncoding(...) instead.
     *
     * \tparam Int The type of integer to encode; this must be either
     * (i) a native C++ integer type, or (ii) one of Regina's arbitrary
     * precision integer types (i.e., regina::Integer or regina::LargeInteger).
     * In particular, \c bool is not allowed here.
     *
     * \param out the output stream to which the encoded string will be written.
     * \param value the integer to encode.
     *
     * \ingroup utilities
     */
    template <typename Int>
    requires StandardCppInteger<Int> || ArbitraryPrecisionInteger<Int>
    void tightEncodeInteger(std::ostream& out, Int value);

    /**
     * Internal function that reconstructs an integer from its given tight
     * encoding.  This should not be called directly; its purpose is to provide
     * a common implementation for tightDecoding() and tightDecode() for all
     * integer types.
     *
     * The tight encoding will be extracted one character at a time
     * beginning with the iterator \a start, in a single pass, without
     * skipping any leading whitespace.  If the iterator ever reaches
     * \a limit before the encoding is complete then the encoding is
     * treated as invalid (i.e., this routine will throw an exception).
     *
     * If \a noTrailingData is \c true then the iterator is required to
     * _finish_ at \a limit, or else the encoding will be considered
     * invalid also; if \a noTrailingData is \c false then there is no
     * constraint on the final state of the iterator.
     *
     * \exception InvalidInput The given iterator does not point to
     * a tight encoding of an integer of type \a Int.  This includes the
     * case where the encoding _is_ a valid integer encoding but the integer
     * itself is outside the allowed range for the \a Int type.
     *
     * This routine does recognise infinity in the case where \a Int is
     * the type regina::LargeInteger.
     *
     * \nopython Use regina::tightDecoding() instead.
     *
     * \tparam Int The type of integer to reconstruct; this must be either
     * (i) a native C++ integer type, or (ii) one of Regina's arbitrary
     * precision integer types (i.e., regina::Integer or regina::LargeInteger).
     * In particular, \c bool is not allowed here.
     *
     * \param start an iterator that points to the beginning of a
     * tight encoding.
     * \param limit an iterator that, if reached, indicates that no more
     * characters are available.
     * \param noTrailingData \c true if iteration should reach \a limit
     * immediately after the encoding is read, or \c false if there is
     * allowed to be additional unread data.
     * \return the integer represented by the given tight encoding.
     *
     * \ingroup utilities
     */
    template <typename Int, InputIteratorFor<char> iterator>
    requires StandardCppInteger<Int> || ArbitraryPrecisionInteger<Int>
    Int tightDecodeInteger(iterator start, iterator limit, bool noTrailingData);

    /**
     * Internal function that writes the tight encoding of an integer
     * whose value is either non-negative or -1.
     *
     * This should _not_ be used for encoding standalone integers, since it
     * uses a more compact format that is not compatible with Regina's general
     * integer encodings.  Instead, it is intended to be used as part of the
     * encoding for larger objects (e.g., triangulations or isomorphisms).
     *
     * \exception InvalidArgument The given integer is less than -1.
     *
     * \nopython
     *
     * \tparam Int The type of integer to encode.  Currently this must be
     * either \c ssize_t (the only allowed signed type), or one of the
     * unsigned native C++ integer types that holds at least 16 bits.
     * This list of types may be expanded in future versions of Regina.
     *
     * \param out the output stream to which the encoded string will be written.
     * \param value the integer to encode.
     *
     * \ingroup utilities
     */
    template <typename Int>
    void tightEncodeIndex(std::ostream& out, Int value);

    /**
     * Internal function that writes the tight encoding of -1, using an
     * encoding that is compatible with tightEncodeIndex().  Note that this is
     * _not_ compatible with Regina's general integer encodings.
     *
     * This is identical to calling `tightEncodeIndex<ssize_t>(out, -1)`.
     *
     * \nopython
     *
     * \param out the output stream to which the encoded string will be written.
     *
     * \ingroup utilities
     */
    void tightEncodeNoIndex(std::ostream& out);

    /**
     * Internal function that reconstructs an integer that was encoded
     * using tightEncodeIndex().  This encoding method is used for integers
     * that are either non-negative or -1, and is not compatible with
     * Regina's general integer encodings.
     *
     * The tight encoding will be read from the given input stream.  If the
     * input stream contains leading whitespace then it will be treated as an
     * invalid encoding (i.e., this routine will throw an exception).  The
     * input stream _may_ contain further data: if this routine is successful
     * then the input stream will be left positioned immediately after the
     * encoding, without skipping any trailing whitespace.
     *
     * \exception InvalidInput The given input stream does not begin with a
     * tight encoding of an integer of type \a Int using the encoding scheme
     * defined by tightEncodeIndex().  This includes the case where the
     * encoding _is_ a valid non-negative integer encoding but the integer
     * itself is outside the allowed range for the \a Int type.
     *
     * \nopython
     *
     * \tparam Int The type of integer to reconstruct.  Currently this must be
     * either \c ssize_t (the only allowed signed type), or one of the
     * unsigned native C++ integer types that holds at least 16 bits.
     * This list of types may be expanded in future versions of Regina.
     *
     * \param input an input stream that begins with a tight encoding.
     * \return the integer represented by the given tight encoding.
     *
     * \ingroup utilities
     */
    template <typename Int>
    Int tightDecodeIndex(std::istream& input);
}

// Inline functions:

inline void tightEncode(std::ostream& out, int value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(int value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, long long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(long long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned long long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned long long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, bool value) {
    if (value)
        out << 'N'; // encoding of 1
    else
        out << 'M'; // encoding of 0
}

inline std::string tightEncoding(bool value) {
    if (value)
        return "N"; // encoding of 1
    else
        return "M"; // encoding of 0
}

#ifndef __DOXYGEN
// Doxygen gets confused by these specialisatons.
template <>
inline bool tightDecoding<bool>(const std::string& enc) {
    if (enc.empty())
        throw InvalidArgument("The tight encoding is incomplete");
    bool ans;
    switch (enc.front()) {
        case 'M': ans = false; break;
        case 'N': ans = true; break;
        default: throw InvalidInput("The tight encoding is invalid");
    }
    if (enc.length() > 1)
        throw InvalidArgument("The tight encoding has trailing characters");
    return ans;
}

template <>
inline bool tightDecode<bool>(std::istream& input) {
    int c = input.get();
    if (c == EOF)
        throw InvalidInput("The tight encoding is incomplete");
    switch (c) {
        case 'M': return false;
        case 'N': return true;
        default: throw InvalidInput("The tight encoding is invalid");
    }
}
#endif

template<typename Int>
inline Int tightDecoding(const std::string& enc) {
    try {
        return regina::detail::tightDecodeInteger<Int>(
            enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

template<typename Int>
inline Int tightDecode(std::istream& input) {
    return regina::detail::tightDecodeInteger<Int>(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

namespace detail {

inline void tightEncodeNoIndex(std::ostream& out) {
    tightEncodeIndex<ssize_t>(out, -1);
}

} // namespace detail

} // namespace regina

#endif
