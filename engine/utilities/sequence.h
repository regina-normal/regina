
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file utilities/sequence.h
 *  \brief Support for temporary lightweight sequences.
 */

#ifndef __SEQUENCE_H
#ifndef __DOXYGEN
#define __SEQUENCE_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * TODO: Document.
 */
class REGINA_API LightweightSequence {
    public:
        typedef int* iterator;
        typedef const int* const_iterator;

    private:
        int* data_;
        size_t size_;

    public:
        LightweightSequence();
        LightweightSequence(size_t size);
        ~LightweightSequence();

        void init(size_t size);
        void destroy();

        size_t size() const;
        int operator [] (size_t pos) const;
        int& operator [] (size_t pos);

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        struct REGINA_API Less {
            // TODO: PRE:: Same size.
            bool operator () (const LightweightSequence* a,
                    const LightweightSequence* b) const;
        };
};

std::ostream& operator << (std::ostream& out, const LightweightSequence& s);

// Inline functions:

inline LightweightSequence::LightweightSequence() : data_(0), size_(0) {
}

inline LightweightSequence::LightweightSequence(size_t size) :
        data_(new int[size]), size_(size) {
}

inline LightweightSequence::~LightweightSequence() {
    delete[] data_;
}

inline void LightweightSequence::init(size_t size) {
    delete[] data_;
    data_ = new int[size];
    size_ = size;
}

inline void LightweightSequence::destroy() {
    if (data_) {
        delete[] data_;
        data_ = 0;
    }
}

inline size_t LightweightSequence::size() const {
    return size_;
}

inline int LightweightSequence::operator [] (size_t pos) const {
    return data_[pos];
}

inline int& LightweightSequence::operator [] (size_t pos) {
    return data_[pos];
}

inline LightweightSequence::iterator LightweightSequence::begin() {
    return data_;
}

inline LightweightSequence::const_iterator LightweightSequence::begin() const {
    return data_;
}

inline LightweightSequence::iterator LightweightSequence::end() {
    return data_ + size_;
}

inline LightweightSequence::const_iterator LightweightSequence::end() const {
    return data_ + size_;
}

inline bool LightweightSequence::Less::operator () (
        const LightweightSequence* a, const LightweightSequence* b) const {
    for (size_t i = 0; i < a->size_; ++i)
        if (a->data_[i] < b->data_[i])
            return true;
        else if (a->data_[i] > b->data_[i])
            return false;
    return false;
}

inline std::ostream& operator << (std::ostream& out,
        const LightweightSequence& s) {
    out << '(';
    for (size_t i = 0; i < s.size(); ++i) {
        if (i > 0)
            out << ", ";
        out << s[i];
    }
    return out << ')';
}

} // namespace regina

#endif

