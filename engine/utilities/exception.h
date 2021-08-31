
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/exception.h
 *  \brief Defines exception types thrown by Regina.
 */

#ifndef __REGINA_EXCEPTION_H
#ifndef __DOXYGEN
#define __REGINA_EXCEPTION_H
#endif

#include <exception>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An exception thrown when a function detects that its preconditions
 * have been violated.
 *
 * Details of the error can be accessed through the inherited member
 * function what().
 *
 * \ifacespython Not present.
 */
class FailedPrecondition : public std::runtime_error {
    public:
        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        FailedPrecondition(const std::string& msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        FailedPrecondition(const char* msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new copy of the given exception.
         */
        FailedPrecondition(const FailedPrecondition&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        FailedPrecondition& operator = (const FailedPrecondition&) noexcept =
            default;
};

/**
 * An exception thrown when a function reads unexpected or incomplete
 * data from an input stream.
 *
 * Details of the error can be accessed through the inherited member
 * function what().
 *
 * \ifacespython Not present.
 */
class InvalidInput : public std::runtime_error {
    public:
        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        InvalidInput(const std::string& msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        InvalidInput(const char* msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new copy of the given exception.
         */
        InvalidInput(const InvalidInput&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        InvalidInput& operator = (const InvalidInput&) noexcept = default;
};

/**
 * An exception thrown when a mathematical function is not able to solve
 * a particular instance of a problem.
 *
 * Details of the error can be accessed through the inherited member
 * function what().
 *
 * \ifacespython Not present.
 */
class Unsolved : public std::runtime_error {
    public:
        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        Unsolved(const std::string& msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        Unsolved(const char* msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new copy of the given exception.
         */
        Unsolved(const Unsolved&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        Unsolved& operator = (const Unsolved&) noexcept =
            default;
};

/**
 * An exception thrown when a set of normal surface/hypersurface matching
 * equations could not be created for a given triangulation.
 *
 * \ifacespython Not present.
 */
class NoMatchingEquations : public std::exception {
    public:
        /**
         * Creates a new exception.
         */
        NoMatchingEquations() noexcept = default;

        /**
         * Creates a new copy of the given exception.
         */
        NoMatchingEquations(const NoMatchingEquations&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         */
        NoMatchingEquations& operator = (const NoMatchingEquations&) noexcept =
            default;

        /**
         * Returns a human-readable description of the error that occurred.
         */
        virtual const char* what() const noexcept override {
            return "Could not create a set of matching equations";
        }
};

/*@}*/

} // namespace regina

#endif
