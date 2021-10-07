
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
 *  \brief Defines some general exception types thrown by Regina.
 *  Note that some exceptions that are specific to a particular piece of
 *  machinery (e.g., snapshotting and SnapPea-related exceptions) are
 *  defined in other headers alongside that machinery.
 */

#ifndef __REGINA_EXCEPTION_H
#ifndef __DOXYGEN
#define __REGINA_EXCEPTION_H
#endif

#include <exception>

namespace regina {

/**
 * A base class for all of the exceptions that are thrown by Regina's
 * native mathematical code.
 *
 * This does \e not include exceptions thrown due to errors in the
 * SnapPea kernel, and it does not include SnapshotWriteError, which is part
 * of the generic (non-mathematical) snapshotting machinery.
 *
 * Details of the error can be accessed through the inherited member
 * function what().
 *
 * \ingroup utilities
 */
class ReginaException : public std::runtime_error {
    public:
        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        ReginaException(const std::string& msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * @param msg a human-readable description of the error.
         */
        ReginaException(const char* msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new copy of the given exception.
         */
        ReginaException(const ReginaException&) noexcept = default;

        /**
         * Sets this to be a copy of the given exception.
         *
         * @return a reference to this exception.
         */
        ReginaException& operator = (const ReginaException&) noexcept = default;
};

/**
 * An exception thrown when a function detects that its preconditions
 * have been violated.
 *
 * The line between failed preconditions and invalid input arguments is not
 * always clear, and so the roles of FailedPrecondition versus InvalidArgument
 * are not clear-cut.  Where relevant, each routine will document clearly
 * which exception type it uses.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class FailedPrecondition : public ReginaException {
    public:
        FailedPrecondition(const std::string& msg) : ReginaException(msg) {}
        FailedPrecondition(const char* msg) : ReginaException(msg) {}
        FailedPrecondition(const FailedPrecondition&) noexcept = default;
        FailedPrecondition& operator = (const FailedPrecondition&) noexcept =
            default;
};

/**
 * An exception thrown when invalid or unsupported arguments are passed
 * into a function.
 *
 * The line between failed preconditions and invalid input arguments is not
 * always clear, and so the roles of FailedPrecondition versus InvalidArgument
 * are not clear-cut.  Where relevant, each routine will document clearly
 * which exception type it uses.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class InvalidArgument : public ReginaException {
    public:
        InvalidArgument(const std::string& msg) : ReginaException(msg) {}
        InvalidArgument(const char* msg) : ReginaException(msg) {}
        InvalidArgument(const InvalidArgument&) noexcept = default;
        InvalidArgument& operator = (const InvalidArgument&) noexcept = default;
};

/**
 * An exception thrown when a function reads unexpected or incomplete
 * data from an input stream.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class InvalidInput : public ReginaException {
    public:
        InvalidInput(const std::string& msg) : ReginaException(msg) {}
        InvalidInput(const char* msg) : ReginaException(msg) {}
        InvalidInput(const InvalidInput&) noexcept = default;
        InvalidInput& operator = (const InvalidInput&) noexcept = default;
};

/**
 * An exception thrown when a mathematical function is not able to solve
 * a particular instance of a problem.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class UnsolvedCase : public ReginaException {
    public:
        UnsolvedCase(const std::string& msg) : ReginaException(msg) {}
        UnsolvedCase(const char* msg) : ReginaException(msg) {}
        UnsolvedCase(const UnsolvedCase&) noexcept = default;
        UnsolvedCase& operator = (const UnsolvedCase&) noexcept = default;
};

} // namespace regina

#endif
