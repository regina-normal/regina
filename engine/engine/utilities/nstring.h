
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

/*! \file nstring.h
 *  \brief Provides a string class based upon a public domain string class
 *  written by Scott Robert Ladd.
 *  \author Scott Robert Ladd, with modifications by Ben Burton.
 */

#ifndef __NSTRING_H
#ifndef __DOXYGEN
#define __NSTRING_H
#endif

#include <iostream>
#include <hash_set>
#include <string>

/**
 * A string class based upon a public domain string class written by
 * Scott Robert Ladd.  The original was entitled
 * <i>String (Dynamic Strings)</i>, version 2.01 (26 Oct 1989).
 * It was released in the public domain and can be located by searching
 * for <i>slstr201.zip</i> on the web.
 *
 * I have made improvements and alterations, fixed many bugs, probably
 * introduced many new bugs, documented it thoroughly and rearranged it
 * to fit my general coding style.
 *
 * \todo \opt Check out tjstring.zip for a copy-on-write class.
 *
 * \ifaces Not present.
 * @author Scott Robert Ladd, with modifications by Ben Burton.
 */
class NString {
    public:
        /**
         * Represents the result obtained when comparing two strings.
         */
        enum strCompVal {
            scLess,       /**< First string is smaller than the second. */
            scEqual,      /**< Both strings are identical. */
            scGreater     /**< First string is greater than the second. */
        };
        /**
         * Represents the available modes for comparing two strings.
         */
        enum strCompMode {
            smSensitive,  /**< Case-sensitive comparison. */
            smIgnore      /**< Ignore case during comparison. */
        };
        /**
         * Represents the types of error that can arise.
         */
        enum strError {
            seAlloc,      /**< Memory allocation failed. */
            seTooLong     /**< The string was sufficiently long that the
                           *   length could not be stored as an integer. */
        };

    private:
        unsigned len;
            /**< Current length of the string. */
        unsigned siz;
            /**< Amount of memory allocated for the string. */
        char* txt;
            /**< Pointer to the corresponding allocated C string. */

        static unsigned allocIncr;
            /**< Specifies by how many characters at a time the
             *   allocated memory is increased or decreased. */
        static void (*errorHandler)(strError);
            /**< A pointer to the function to call when an error occurs.
             *   Such a function should take one argument of type
             *   NString::strError, which will specify what type of error
             *   has occurred. */

    public:
        /**
         * Creates a new empty string.
         */
        NString();
        /**
         * Creates a new string equal to the given string.
         *
         * @param str the string to clone.
         */
        NString(const NString& str);
        /**
         * Creates a new string equal to the given C string.
         *
         * @param cStr the string to clone.
         */
        NString(const char* cStr);
        /**
         * Creates a new string equal to the given C++ string.
         *
         * @param str the string to clone.
         */
        NString(const std::string& str);
        /**
         * Creates a new string containing the given number of copies of
         * the given character.
         *
         * @param fillCh the character with which to fill the new
         * string.
         * @param count the number of copies of the given character to
         * use; this must be strictly positive.
         */
        NString(char fillCh, unsigned count);
        /**
         * Destroys this string.
         */
        ~NString();

        /**
         * Returns the length of this string.
         *
         * @return the string length.
         */
        unsigned length() const;
        /**
         * Returns the number of characters allocated for this string.
         *
         * @return the number of characters allocated.
         */
        unsigned size() const;

        /**
         * Assigns a new error handler.
         * The default error handler writes a description of the error
         * to stderr and then exits the entire program.
         *
         * @param newHandler a pointer to the new function to call when
         * an error occurs.  This function should take one argument of
         * type NString::strError, which will specify what type of error
         * has occurred.
         */
        static void setErrorHandler(void (*newHandler)(strError));

        /**
         * Copies the contents of this string into the given C string.
         * Any preexisting data stored in the C string will be replaced.
         * A null character will be placed at the end of the C string.
         *
         * @param cStr the destination C string.
         * @param bufSize the number of characters allocated for the
         * given C string.  At most <i>bufSize</i>-1 characters from
         * this string will be copied, not including the terminating
         * null.
         */
        void copy(char* cStr, unsigned bufSize) const;
        /**
         * Creates a new C string containing the same contents as this
         * string.
         *
         * It is the caller's responsibility to deallocate the
         * new string when it is no longer needed.  Note that this
         * deallocation should use <tt>delete[]</tt> and not just
         * <tt>delete</tt>.
         *
         * @return a newly allocated C string.
         */
        char* dupe() const;

        /**
         * Assigns the value of the given string to this string.
         *
         * @param str the string to clone.
         */
        void operator = (const NString& str);
        /**
         * Assigns the value of the given string to this string.
         *
         * @param str the string to clone.
         */
        void operator = (const char* str);
        /**
         * Returns the concatenation of this and the given string.
         * This string is not changed.
         *
         * @param other the string to concatenate to this.
         * @return the concatenation of this and the given string.
         */
        NString operator + (const NString& other) const;
        /**
         * Returns the concatenation of this and the given string.
         * This string is not changed.
         *
         * @param other the C string to concatenate to this.
         * @return the concatenation of this and the given string.
         */
        NString operator + (const char* other) const;
        /**
         * Adds the given string to the end of this string.
         *
         * @param str the string to add to the end of this string.
         */
        void operator += (const NString& str);
        /**
         * Adds the given string to the end of this string.
         *
         * @param str the C string to add to the end of this string.
         */
        void operator += (const char* str);

        /**
         * Determines if this string is less than the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is less than
         * the given string.
         */
        int operator < (const NString& str) const;
        /**
         * Determines if this string is greater than the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is greater than
         * the given string.
         */
        int operator > (const NString& str) const;
        /**
         * Determines if this string is less than or equal to the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is less than or
         * equal to the given string.
         */
        int operator <= (const NString& str) const;
        /**
         * Determines if this string is greater than or equal to
         * the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is greater than or
         * equal to the given string.
         */
        int operator >= (const NString& str) const;
        /**
         * Determines if this string is equal to the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is equal to
         * the given string.
         */
        int operator == (const NString& str) const;
        /**
         * Determines if this string is not equal to the given string.
         * Comparison will be case sensitive.
         *
         * @param str the string with which to compare this.
         * @return \c true if and only if this string is not equal to
         * the given string.
         */
        int operator != (const NString& str) const;

        /**
         * Lexicographically compares this with the given string, with
         * the option of specifying case sensitivity.
         *
         * @param str the string to compare with this.
         * @param mode specifies whether or not the comparison should be
         * case sensitive.  The default is NString::smSensitive (case
         * sensitive).
         * @return the result of the comparison (see
         * NString::strCompVal), where this is the first string and
         * \a str is the second.
         */
        strCompVal compare(const NString& str,
            strCompMode mode = smSensitive) const;

        /**
         * Finds the first location of the given substring inside this
         * string, with the option of specifying case sensitivity.
         * If the substring is found, its location within this string will
         * be returned in the parameter \a pos.
         *
         * @param str the substring to find.
         * @param pos the variable in which the location of \a str
         * within this string will be returned.  If \a str is not found,
         * the value returned in this variable will be undefined.
         * This variable may have any value when initially passed.
         * @param mode specifies whether or not comparisons should be
         * case sensitive.  The default is NString::smSensitive (case
         * sensitive).
         * @return \c true if and only if the given substring was found.
         */
        bool find(const NString& str, unsigned& pos,
            strCompMode mode = smSensitive) const;

        /**
         * Removes the substring of the given length from the given
         * position in this string.  Only characters up until the end of
         * the string will be deleted, even if \a count is larger.
         *
         * @param pos the position of the first character to delete;
         * this must be between 0 and length() inclusive.
         * @param count the number of characters to delete.
         * @count the number of consecutive characters to delete.
         */
        void remove(unsigned pos, unsigned count);

        /**
         * Inserts the given character into the given position in this
         * string.
         *
         * @param pos the position at which to insert the given
         * character; this must be between 0 and length() inclusive.
         * @param ch the character to insert.
         */
        void insert(unsigned pos, char ch);
        /**
         * Inserts the given substring into the given position in this
         * string.
         *
         * @param pos the position at which to insert the given
         * substring; this must be between 0 and length() inclusive.
         * @param str the substring to insert.
         */
        void insert(unsigned pos, const NString& str);
        /**
         * Inserts the given substring into the given position in this
         * string.
         *
         * @param pos the position at which to insert the given
         * substring; this must be between 0 and length() inclusive.
         * @param str the C substring to insert.
         */
        void insert(unsigned pos, const char* str);

        /**
         * Returns the given substring of this string.
         * This string is not altered.
         *
         * \pre <tt>start + count <= length()</tt>.
         * @param start the starting position of the given substring in
         * this string; this must be between 0 and length() inclusive.
         * @param count the length of the given substring.
         * @return the given substring.
         */
        NString subStr(unsigned start, unsigned count) const;

        /**
         * Returns the character at the given position in the string.
         *
         * \pre The given position is between 0 and length()-1
         * inclusive.
         *
         * @param pos the position of the character to look up.
         * @return the character at the given position.
         */
        char operator [] (unsigned pos) const;

        /**
         * Returns an all upper-case version of this string.
         * This string is not changed.
         *
         * @return an upper-case version of this string.
         */
        NString toUpper() const;
        /**
         * Returns an all lower-case version of this string.
         * This string is not changed.
         *
         * @return a lower-case version of this string.
         */
        NString toLower() const;

        /**
         * Calculates a hash value for this string.
         *
         * @return the hash value.
         */
        size_t hashValue() const;

    private:
        /**
         * Shrinks the string to its minimum allocation.
         */
        void shrink();
        /**
         * Determines the number of characters to allocate for the given
         * length string.  The answer will be the smallest positive
         * multiple of \a allocLen that is at least as large as \a len.
         *
         * @len the length of the string.
         * @return the corresponding number of characters to allocate.
         */
        static unsigned allocLen(unsigned len);

    friend std::istream& operator >> (std::istream& input, NString& str);
    friend std::ostream& operator << (std::ostream& output, const NString& str);
};

/**
 * Reads a string from the given input stream and stores it in the given
 * string.  Any preexisting data stored in the given string will be
 * replaced by the new string that is read.
 *
 * Any initial whitespace is skipped.  The string is then read and
 * terminates at the next occurrence of whitespace in the input stream.
 * The first whitespace character following the string will be removed
 * from the input stream.
 *
 * If the string is terminated by end-of-file instead of by whitespace,
 * the input stream EOF flag will be set, although \a str will still
 * contain the string that was read.
 *
 * @param input the input stream from which to read.
 * @param str the object in which to store the string that is read.
 * @return a reference to the given input stream.
 */
std::istream& operator >> (std::istream& input, NString& str);

/**
 * Writes the given string to the given output stream.
 *
 * @param output the output stream to which to write.
 * @param str the string to write.
 * @return a reference to the given output stream.
 */
std::ostream& operator << (std::ostream& output, const NString& str);

namespace std {
    /**
     * A hash function for use with the Standard Template Library.
     * This class calculates hash values for NString objects.
     *
     * \ifaces Not present.
     */
    template<> struct hash<NString> {
        /**
         * Returns a hash value for the given string.
         *
         * @param s the string whose hash value should be calculated.
         * @return the corresponding hash value.
         */
        size_t operator() (const NString& s) const {
            return s.hashValue();
        }
    };
}

// Inline functions for NString

inline NString::NString() : len(0), siz(allocIncr), txt(new char[allocIncr]) {
    if (txt == 0)
        errorHandler(seAlloc);
}
inline NString::~NString() {
    delete[] txt;
}

inline unsigned NString::length() const {
    return len;
}
inline unsigned NString::size() const {
    return siz;
}

inline int NString::operator < (const NString& str) const {
    return (compare(str) == scLess);
}
inline int NString::operator > (const NString& str) const {
    return (compare(str) == scGreater);
}
inline int NString::operator <= (const NString& str) const {
    return (compare(str) != scGreater);
}
inline int NString::operator >= (const NString& str) const {
    return (compare(str) != scLess);
}
inline int NString::operator == (const NString& str) const {
    return (compare(str) == scEqual);
}
inline int NString::operator != (const NString& str) const {
    return (compare(str) != scEqual);
}

inline char NString::operator [] (unsigned pos) const {
    return txt[pos];
}

#endif

