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

/*! \file subcomplex/standardtri4.h
 *  \brief Deals with 4-dimensional triangulations whose structures are
 *  well-understood.
 */

#ifndef __REGINA_STANDARDTRI4_H
#ifndef __DOXYGEN
#define __REGINA_STANDARDTRI4_H
#endif

#include <memory>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/forward.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Describes a 4-dimensional triangulation or subcomplex whose structure is
 * well-understood.
 *
 * This is a deliberately small analogue of StandardTriangulation for
 * dimension four.  It only provides the naming and text-output machinery
 * needed by the 4-dimensional standard subcomplexes that currently exist.
 * It does not include any of the 3-dimensional manifold or homology routines.
 *
 * Subclasses must override writeName() and writeTeXName(), and may override
 * writeTextShort() and writeTextLong().
 *
 * \ingroup subcomplex
 */
class StandardTriangulation4 : public Output<StandardTriangulation4> {
    public:
        /**
         * A destructor that does nothing.
         */
        virtual ~StandardTriangulation4() = default;

        /**
         * Returns the name of this specific triangulation or subcomplex as a
         * human-readable string.
         *
         * \return the name of this structure.
         */
        std::string name() const;
        /**
         * Returns the name of this specific triangulation or subcomplex in TeX
         * format.  No leading or trailing dollar signs will be included.
         *
         * \return the name of this structure in TeX format.
         */
        std::string texName() const;

        /**
         * Writes the name of this structure as a human-readable string to the
         * given output stream.
         *
         * \nopython Instead use the variant name() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeName(std::ostream& out) const = 0;
        /**
         * Writes the name of this structure in TeX format to the given output
         * stream.  No leading or trailing dollar signs will be included.
         *
         * \nopython Instead use the variant texName() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         */
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        /**
         * Writes a short text representation of this object to the given
         * output stream.
         *
         * This may be reimplemented by subclasses, but the parent class offers
         * a default implementation based on writeName().
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        virtual void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the given
         * output stream.
         *
         * This may be reimplemented by subclasses, but the parent class offers
         * a default implementation based on writeTextShort().
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        virtual void writeTextLong(std::ostream& out) const;

        /**
         * Determines whether the given triangulation represents one of the
         * standard 4-dimensional triangulations understood by Regina.
         *
         * At present this recognises only the one-pentachoron snapped 4-ball.
         *
         * \param tri the triangulation under examination.
         * \return the details of the standard triangulation if the given
         * triangulation is recognised, or \c null otherwise.
         */
        static std::unique_ptr<StandardTriangulation4> recognise(
            const Triangulation<4>& tri);

    protected:
        /**
         * Default constructor.  This is here for subclasses to implicitly call
         * from their own constructors.
         */
        StandardTriangulation4() = default;
};

} // namespace regina

#endif
