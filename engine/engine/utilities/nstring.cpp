
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

// A string class based upon a public domain string class written by
// Scott Robert Ladd; see the header file for further details.

#include "utilities/nstring.h"

#include <cstring>
#include <cctype>
#include <climits>

static void defaultHandler(NString::strError err);

unsigned NString::allocIncr = 16;
void (*NString::errorHandler)(strError) = defaultHandler;

static void defaultHandler(NString::strError err) {
    // Default error handler.
    cerr << "\aERROR in NString object: ";

    switch (err) {
        case NString::seAlloc :
            cout << "Memory allocation failure.";
            break;
        case NString::seTooLong :
            cout << "Exceeded " << UINT_MAX << " character limit.";
            break;
    }
    cerr << '\n';
    exit(1);
}

void NString::shrink() {
    if ((siz - len) > allocIncr) {
        siz = allocLen(len);
        char* temp = new char[siz];

        if (temp != 0) {
            memcpy(temp, txt, len);
            delete[] txt;
            txt = temp;
        } else
            errorHandler(seAlloc);
    }
}

unsigned NString::allocLen(unsigned len) {
    if (len == 0)
        return allocIncr;
    else
        return ((len + allocIncr - 1) / allocIncr) * allocIncr;
}

NString::NString(const NString& str) : len(str.len), siz(str.siz),
        txt(new char[str.siz]) {
    if (txt == 0)
        errorHandler(seAlloc);
    memcpy(txt, str.txt, len);
}

NString::NString(const char* cStr) : len(strlen(cStr)) {
    siz = allocLen(len);
    txt = new char[siz];

    if (txt == 0)
        errorHandler(seAlloc);
    memcpy(txt, cStr, len);
}

NString::NString(const string& str) : len(str.length()) {
    siz = allocLen(len);
    txt = new char[siz];

    if (txt == 0)
        errorHandler(seAlloc);
    memcpy(txt, str.data(), len);
}

NString::NString(char fillCh, unsigned count) : len(count) {
    siz = allocLen(count);
    txt = new char[siz];

    if (txt == 0)
        errorHandler(seAlloc);
    memset(txt, fillCh, count);
}

void NString::setErrorHandler(void (*newHandler)(strError)) {
    errorHandler = newHandler;
}

void NString::copy(char* cStr, unsigned bufSize) const {
    if (bufSize == 0)
        return;

    unsigned copyLen;
    if (len >= bufSize)
        copyLen = bufSize - 1;
    else
        copyLen = len;

    memcpy(cStr, txt, copyLen);
    cStr[copyLen] = 0;
}

char* NString::dupe() const {
    char* new_cstr = new char[len + 1];

    memcpy(new_cstr, txt, len);
    new_cstr[len] = 0;

    return new_cstr;
}

void NString::operator = (const NString& str) {
    delete[] txt;

    len = str.len;
    siz = str.siz;
    txt = new char[siz];

    if (txt == 0)
        errorHandler(seAlloc);
    memcpy(txt, str.txt, len);
}

void NString::operator = (const char* str) {
    delete[] txt;

    len = strlen(str);
    siz = allocLen(len);
    txt = new char[siz];

    if (txt == 0)
        errorHandler(seAlloc);
    memcpy(txt, str, len);
}

NString NString::operator + (const NString& other) const {
    unsigned long totalLen = ((unsigned long)len) +
        ((unsigned long)(other.len));
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);

    NString ans;
    delete[] ans.txt;
    ans.len = len + other.len;
    ans.siz = allocLen(ans.len);
    ans.txt = new char[ans.siz];
    if (ans.txt == 0)
        errorHandler(seAlloc);

    memcpy(ans.txt, txt, len);
    memcpy(ans.txt + len, other.txt, other.len);

    return ans;
}

NString NString::operator + (const char* other) const {
    unsigned otherLen = strlen(other);
    unsigned long totalLen = ((unsigned long)len) +
        ((unsigned long)(otherLen));
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);

    NString ans;
    delete[] ans.txt;
    ans.len = len + otherLen;
    ans.siz = allocLen(ans.len);
    ans.txt = new char[ans.siz];
    if (ans.txt == 0)
        errorHandler(seAlloc);

    memcpy(ans.txt, txt, len);
    memcpy(ans.txt + len, other, otherLen);

    return ans;
}

void NString::operator += (const NString& str) {
    unsigned long totalLen = ((unsigned long)len) + ((unsigned long)(str.len));
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);
    
    siz = allocLen(len + str.len);
    char* ans = new char[siz];
    if (ans == 0)
        errorHandler(seAlloc);
    
    memcpy(ans, txt, len);
    memcpy(ans + len, str.txt, str.len);

    len = len + str.len;
    delete[] txt;
    txt = ans;
}

void NString::operator += (const char* str) {
    unsigned strLen = strlen(str);
    unsigned long totalLen = ((unsigned long)len) + ((unsigned long)strLen);
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);
    
    siz = allocLen(len + strLen);
    char* ans = new char[siz];
    if (ans == 0)
        errorHandler(seAlloc);
    
    memcpy(ans, txt, len);
    memcpy(ans + len, str, strLen);

    len = len + strLen;
    delete[] txt;
    txt = ans;
}

NString::strCompVal NString::compare(const NString& str,
        NString::strCompMode mode) const {
    unsigned index = 0;
    unsigned maxIndex = len > str.len ? str.len : len;

    if (mode == smIgnore) {
        // Ignore case.
        while (index < maxIndex) {
            if (toupper(txt[index]) == toupper(str.txt[index]))
                ++index;
            else if (toupper(txt[index]) < toupper(str.txt[index]))
                return scLess;
            else
                return scGreater;
        }
    } else {
        // Case sensitive.
        while (index < maxIndex) {
            if (txt[index] == str.txt[index])
                ++index;
            else if (txt[index] < str.txt[index])
                return scLess;
            else
                return scGreater;
        }
    }

    if (len < str.len)
        return scLess;
    if (len > str.len)
        return scGreater;
    return scEqual;
}

bool NString::find(const NString& str, unsigned& pos,
        NString::strCompMode mode) const {
    char* tempStr1 = new char[len + 1];
    if (tempStr1 == 0)
        errorHandler(seAlloc);
    memcpy(tempStr1, txt, len);
    tempStr1[len] = 0;

    char* tempStr2 = new char[str.len + 1];
    if (tempStr2 == 0)
        errorHandler(seAlloc);
    memcpy(tempStr2, str.txt, str.len);
    tempStr2[str.len] = 0;

    if (mode == smIgnore) {
        char* ptr;
        for (ptr = tempStr1; *ptr; ptr++)
            *ptr = toupper(*ptr);
        for (ptr = tempStr2; *ptr; ptr++)
            *ptr = toupper(*ptr);
    }

    unsigned lastPos = len - str.len;
    bool found = false;
    pos = 0;
    while ((pos <= lastPos) && ! found) {
        if (strncmp(tempStr1 + pos, tempStr2, str.len) == 0)
            found = true;
        else
            ++pos;
    }

    delete[] tempStr1;
    delete[] tempStr2;
    return found;
}

void NString::remove(unsigned pos, unsigned count) {
    if (count == 0)
        return;

    if (pos + count > len)
        len = pos;
    else {
        unsigned copyPos = pos + count;
        while (copyPos < len) {
            txt[pos] = txt[copyPos];
            ++pos;
            ++copyPos;
        }
        len -= count;
    }

    shrink();
}

void NString::insert(unsigned pos, char ch) {
    if (len == siz) {
        siz += allocIncr;
        char* temp = new char[siz];

        if (temp == 0)
            errorHandler(seAlloc);

        memcpy(temp, txt, len);
        delete[] txt;
        txt = temp;
    }

    if (pos < len)
        for (unsigned col = len + 1; col > pos; --col)
            txt[col] = txt[col-1];

    txt[pos] = ch;
    ++len;
}

void NString::insert(unsigned pos, const NString& str) {
    if (str.len == 0)
        return;

    unsigned long totalLen = (unsigned long)str.len + (unsigned long)len;
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);

    siz = allocLen(len + str.len);
    char* ans = new char[siz];
    if (ans == 0)
        errorHandler(seAlloc);

    memcpy(ans, txt, pos);
    memcpy(ans + pos, str.txt, str.len);
    memcpy(ans + pos + str.len, txt + pos, len - pos);

    len += str.len;
    delete[] txt;
    txt = ans;
}

void NString::insert(unsigned pos, const char* str) {
    unsigned strLen = strlen(str);
    if (strLen == 0)
        return;

    unsigned long totalLen = (unsigned long)strLen + (unsigned long)len;
    if (totalLen > UINT_MAX)
        errorHandler(seTooLong);

    siz = allocLen(len + strLen);
    char* ans = new char[siz];
    if (ans == 0)
        errorHandler(seAlloc);

    memcpy(ans, txt, pos);
    memcpy(ans + pos, str, strLen);
    memcpy(ans + pos + strLen, txt + pos, len - pos);

    len += strLen;
    delete[] txt;
    txt = ans;
}

NString NString::subStr(unsigned start, unsigned count) const {
    if (count == 0)
        return NString();

    NString ans;
    delete[] ans.txt;
    ans.len = count;
    ans.siz = allocLen(ans.len);
    ans.txt = new char[ans.siz];

    if (ans.txt == 0)
        errorHandler(seAlloc);
    memcpy(ans.txt, txt + start, count);

    return ans;
}

NString NString::toUpper() const {
    NString tempStr(*this);
    for (unsigned pos = 0; pos < len; ++pos)
        tempStr.txt[pos] = char(toupper(tempStr.txt[pos]));
    return tempStr;
}

NString NString::toLower() const {
    NString tempStr(*this);
    for (unsigned pos = 0; pos < len; ++pos)
        tempStr.txt[pos] = char(tolower(tempStr.txt[pos]));
    return tempStr;
}

#define NCHARS 10

std::istream& operator >> (std::istream& input, NString& str) {
    // Skip initial whitespace.
    char c = input.get();
    while ((! input.eof()) && isspace(c))
        c = input.get();
    
    if (input.eof()) {
        // If EOF was reached, return with an empty string.
        str.len = 0;
        str.siz = NString::allocIncr;
        delete[] str.txt;
        str.txt = new char[str.siz];
        if (str.txt == 0)
            NString::errorHandler(NString::seAlloc);
        return input;
    }

    // Start reading the string.
    // Read in characters NCHARS at a time.
    char buffer[NCHARS + 1];
    bool firstAddition = true;
    int pos = 0;
    
    while ((! input.eof()) && (! isspace(c))) {
        buffer[pos] = c;
        pos++;
        if (pos == NCHARS) {
            buffer[pos] = 0;
            pos = 0;
            if (firstAddition) {
                str = buffer;
                firstAddition = false;
            } else
                str += buffer;
        }
        c = input.get();
    }

    // We know an empty string has not been read.
    if (pos != 0) {
        buffer[pos] = 0;
        if (firstAddition)
            str = buffer;
        else
            str += buffer;
    }

    return input;
}

std::ostream& operator << (std::ostream& output, const NString& str) {
    for (unsigned index = 0; index < str.len; ++index)
        output << str.txt[index];
    return output;
}

size_t NString::hashValue() const {
    // Clone the STL hash function for now until we replace NString
    // completely with a standard string class.

    unsigned long ans = 0;
    for (unsigned index = 0; index < len; ++index)
        ans = 5 * ans + txt[index];
    return size_t(ans);
}

