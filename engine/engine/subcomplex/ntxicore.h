
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file ntxicore.h
 *  \brief Deals with layered surface bundle triangulations.
 */

#ifndef __NTXICORE_H
#ifndef __DOXYGEN
#define __NTXICORE_H
#endif

#include "triangulation/ntriangulation.h"
#include "utilities/nmatrix2.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

class NTxICore : public ShareableObject {
    protected:
        NTriangulation core_;
        unsigned bdryTet_[2][2];
        NPerm bdryRoles_[2][2];
            /**< Upper boundary is [0][], lower boundary is [1][]. */
        NMatrix2 bdryReln_[2];
            /**< Express bdry[i] alpha/beta in terms of roles 01/02. */
            /**< Must have determinant +/- 1. */
        NMatrix2 parallelReln_;
            /**< Express lower alpha/beta in terms of upper. */

    public:
        const NTriangulation& core() const;
        unsigned bdryTet(unsigned whichBdry, unsigned whichTet) const;
        NPerm bdryRoles(unsigned whichBdry, unsigned whichTet) const;
        const NMatrix2& bdryReln(unsigned whichBdry) const;
        const NMatrix2& parallelReln() const;

        /**
         * Wrappers dump to cout.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        void writeTextShort(std::ostream& out) const;

    protected:
        NTxICore();
};

class NTxIDiagonalCore : public NTxICore {
    private:
        unsigned long size_;
        unsigned long k_;

    public:
        NTxIDiagonalCore(unsigned long newSize, unsigned long newK);

        unsigned long size() const;
        unsigned long k() const;

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

class NTxIParallelCore : public NTxICore {
    public:
        NTxIParallelCore();

        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/*@}*/

// Inline functions for NTxICore

inline NTxICore::NTxICore() {
}

inline const NTriangulation& NTxICore::core() const {
    return core_;
}

inline unsigned NTxICore::bdryTet(unsigned whichBdry, unsigned whichTet) const {
    return bdryTet_[whichBdry][whichTet];
}

inline NPerm NTxICore::bdryRoles(unsigned whichBdry, unsigned whichTet) const {
    return bdryRoles_[whichBdry][whichTet];
}

inline const NMatrix2& NTxICore::bdryReln(unsigned whichBdry) const {
    return bdryReln_[whichBdry];
}

inline const NMatrix2& NTxICore::parallelReln() const {
    return parallelReln_;
}

inline void NTxICore::writeTextShort(std::ostream& out) const {
    out << "TxI core: ";
    writeName(out);
}

// Inline functions for NTxIDiagonalCore

inline unsigned long NTxIDiagonalCore::size() const {
    return size_;
}

inline unsigned long NTxIDiagonalCore::k() const {
    return k_;
}

inline std::ostream& NTxIDiagonalCore::writeName(std::ostream& out) const {
    return out << 'T' << size_ << ':' << k_;
}

inline std::ostream& NTxIDiagonalCore::writeTeXName(std::ostream& out) const {
    return out << "T_{" << size_ << ':' << k_ << '}';
}

inline std::ostream& NTxIParallelCore::writeName(std::ostream& out) const {
    return out << "T6*";
}

inline std::ostream& NTxIParallelCore::writeTeXName(std::ostream& out) const {
    return out << "T_{6\\ast}";
}

} // namespace regina

#endif

