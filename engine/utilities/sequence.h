
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
 *
 * \ifacespython Not present.
 */
template <typename T>
class REGINA_API LightweightSequence {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

    private:
        T* data_;
        size_t size_;

    public:
        LightweightSequence();
        LightweightSequence(size_t size);
        ~LightweightSequence();

        void init(size_t size);
        void destroy();

        size_t size() const;
        T operator [] (size_t pos) const;
        T& operator [] (size_t pos);

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

template <typename T>
std::ostream& operator << (std::ostream& out, const LightweightSequence<T>& s);

// Inline functions:

template <typename T>
inline LightweightSequence<T>::LightweightSequence() : data_(0), size_(0) {
}

template <typename T>
inline LightweightSequence<T>::LightweightSequence(size_t size) :
        data_(new T[size]), size_(size) {
}

template <typename T>
inline LightweightSequence<T>::~LightweightSequence() {
    delete[] data_;
}

template <typename T>
inline void LightweightSequence<T>::init(size_t size) {
    delete[] data_;
    data_ = new T[size];
    size_ = size;
}

template <typename T>
inline void LightweightSequence<T>::destroy() {
    if (data_) {
        delete[] data_;
        data_ = 0;
    }
}

template <typename T>
inline size_t LightweightSequence<T>::size() const {
    return size_;
}

template <typename T>
inline T LightweightSequence<T>::operator [] (size_t pos) const {
    return data_[pos];
}

template <typename T>
inline T& LightweightSequence<T>::operator [] (size_t pos) {
    return data_[pos];
}

template <typename T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::begin() {
    return data_;
}

template <typename T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::begin() const {
    return data_;
}

template <typename T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::end() {
    return data_ + size_;
}

template <typename T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::end() const {
    return data_ + size_;
}

template <typename T>
inline bool LightweightSequence<T>::Less::operator () (
        const LightweightSequence<T>* a, const LightweightSequence<T>* b)
        const {
    for (size_t i = 0; i < a->size_; ++i)
        if (a->data_[i] < b->data_[i])
            return true;
        else if (a->data_[i] > b->data_[i])
            return false;
    return false;
}

template <typename T>
inline std::ostream& operator << (std::ostream& out,
        const LightweightSequence<T>& s) {
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

