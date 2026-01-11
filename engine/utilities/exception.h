
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

/*! \file utilities/exception.h
 *  \brief Defines some general exception types thrown by Regina.
 *  Note that some exceptions that are specific to a particular piece of
 *  machinery (e.g., snapshotting, or errors within the SnapPea kernel) are
 *  defined in other headers alongside that machinery.
 */

#ifndef __REGINA_EXCEPTION_H
#ifndef __DOXYGEN
#define __REGINA_EXCEPTION_H
#endif

#include <stdexcept>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * A base class for all of the exceptions that are thrown by Regina's
 * native mathematical code.
 *
 * This does _not_ include SnapPeaFatalError and SnapPeaMemoryFull,
 * which represent critical errors thrown from within the SnapPea kernel;
 * it also does not include SnapshotWriteError, which is part of the generic
 * (non-mathematical) snapshotting machinery.
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
         * \param msg a human-readable description of the error.
         */
        ReginaException(const std::string& msg) : std::runtime_error(msg) {
        }

        /**
         * Creates a new exception with the given error message.
         *
         * This constructor may throw std::bad_alloc.
         *
         * \param msg a human-readable description of the error.
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
         * \return a reference to this exception.
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
 * data from an input stream (or more generally, an input source).
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
 * An exception thrown when some functionality is not yet implemented.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class NotImplemented : public ReginaException {
    public:
        NotImplemented(const std::string& msg) : ReginaException(msg) {}
        NotImplemented(const char* msg) : ReginaException(msg) {}
        NotImplemented(const NotImplemented&) noexcept = default;
        NotImplemented& operator = (const NotImplemented&) noexcept = default;
};

/**
 * An exception thrown when trying to access data from the filesystem.
 *
 * This could occur, for instance, when trying to read data from the
 * Regina or SnapPea census databases.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class FileError : public ReginaException {
    public:
        FileError(const std::string& msg) : ReginaException(msg) {}
        FileError(const char* msg) : ReginaException(msg) {}
        FileError(const FileError&) noexcept = default;
        FileError& operator = (const FileError&) noexcept = default;
};

/**
 * An exception thrown when Regina encounters some kind of numerical or
 * arithmetical error.
 *
 * An example here might be division by zero.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class NumericalError : public ReginaException {
    public:
        NumericalError(const std::string& msg) : ReginaException(msg) {}
        NumericalError(const char* msg) : ReginaException(msg) {}
        NumericalError(const NumericalError&) noexcept = default;
        NumericalError& operator = (const NumericalError&) noexcept = default;
};

/**
 * An exception thrown when the result of some calculation cannot fit into
 * an appropriate native C++ integer type.
 *
 * Note that the constructor for this exception class follows a different
 * pattern from most of Regina's exception classes.
 *
 * \ingroup utilities
 */
class IntegerOverflow : public ReginaException {
    public:
        /**
         * Creates a new exception with a stock error message.
         */
        IntegerOverflow() : ReginaException("Integer overflow") {}
        /**
         * Creates a new copy of the given exception.
         */
        IntegerOverflow(const IntegerOverflow&) noexcept = default;
        /**
         * Sets this to be a copy of the given exception.
         *
         * \return a reference to this exception.
         */
        IntegerOverflow& operator = (const IntegerOverflow&) noexcept = default;
};

/**
 * An exception thrown when Regina has certified that a mathematical
 * problem has no solution.
 *
 * Note that the constructor for this exception class follows a different
 * pattern from most of Regina's exception classes.
 *
 * \ingroup utilities
 */
class NoSolution : public ReginaException {
    public:
        /**
         * Creates a new exception with a stock error message.
         */
        NoSolution() : ReginaException("No solution") {}
        /**
         * Creates a new copy of the given exception.
         */
        NoSolution(const NoSolution&) noexcept = default;
        /**
         * Sets this to be a copy of the given exception.
         *
         * \return a reference to this exception.
         */
        NoSolution& operator = (const NoSolution&) noexcept = default;
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

/**
 * An exception thrown when an attempt is made to violate a simplex or facet
 * lock.  See Simplex<dim>::lock() and Simplex<dim>::lockFacet() for further
 * details on simplex/facet locks and what restrictions they impose.
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class LockViolation : public ReginaException {
    public:
        LockViolation(const std::string& msg) : ReginaException(msg) {}
        LockViolation(const char* msg) : ReginaException(msg) {}
        LockViolation(const LockViolation&) noexcept = default;
        LockViolation& operator = (const LockViolation&) noexcept = default;
};

/**
 * An exception thrown when an unexpected scenario arises that should never
 * be possible.
 *
 * By "unexpected", we do not mean (for example) that a precondition was
 * violated, or an invalid argument was passed to some function.  Instead, we
 * mean that within the implementation of some function, a situation arises
 * that should be provably impossible.  Seeing this exception would most likely
 * indicate a bug in Regina's own source code (as opposed to an error by the
 * user, or an incorrect use of Regina's API).
 *
 * Such exceptions will typically not be mentioned in the API documentation
 * (since, by their nature, they should never be thrown).
 *
 * All member functions follow the same pattern as the parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup utilities
 */
class ImpossibleScenario : public ReginaException {
    public:
        ImpossibleScenario(const std::string& msg) :
            ReginaException("An impossible scenario has occurred - "
                "please report this to the Regina developers: " + msg) {}
        ImpossibleScenario(const char* msg) :
            ReginaException(std::string("An impossible scenario has occurred - "
                "please report this to the Regina developers: ") + msg) {}
        ImpossibleScenario(const ImpossibleScenario&) noexcept = default;
        ImpossibleScenario& operator = (const ImpossibleScenario&) noexcept =
            default;
};

/**
 * An exception thrown when the SnapPea kernel is not able to perform some
 * computation.
 *
 * This is a "regular" exception type derived from ReginaException, and
 * is typically thrown in scenarios where some SnapPea function returns
 * cleanly with an error condition.  This is contrast to SnapPeaFatalError
 * and SnapPeaMemoryFull, which are more severe errors that occur when the
 * SnapPea kernel aborts processing unexpectedly, and which are not part
 * of the regular ReginaException class hierarchy.
 *
 * All member functions follow the same pattern as the (grand)parent class
 * ReginaException, and are not documented again here.
 *
 * \ingroup snappea
 */
class SnapPeaUnsolvedCase : public UnsolvedCase {
    public:
        SnapPeaUnsolvedCase(const std::string& msg) : UnsolvedCase(msg) {}
        SnapPeaUnsolvedCase(const char* msg) : UnsolvedCase(msg) {}
        SnapPeaUnsolvedCase(const SnapPeaUnsolvedCase&) noexcept = default;
        SnapPeaUnsolvedCase& operator = (const SnapPeaUnsolvedCase&) noexcept =
            default;
};

/**
 * An exception thrown when the SnapPea kernel is asked to work with a
 * null SnapPea triangulation.
 *
 * This is a "regular" exception type derived from ReginaException, and is
 * typically thrown in scenarios where the user asks Regina to call some
 * SnapPea kernel function but Regina has no native SnapPea triangulation
 * to call it with.  This is contrast to SnapPeaFatalError and
 * SnapPeaMemoryFull, which are more severe errors that occur within the
 * SnapPea kernel when it aborts processing unexpectedly, and which are not
 * part of the regular ReginaException class hierarchy.
 *
 * Note that the constructor for this exception class follows a different
 * pattern from most of Regina's exception classes.
 *
 * \ingroup snappea
 */
class SnapPeaIsNull : public ReginaException {
    public:
        /**
         * Creates a new exception, and marks it as having occurred
         * within the given Regina function.
         *
         * \param fromFunction the name of the Regina function that the user
         * called (in particular, this is not the name of the SnapPea kernel
         * function that would have been called as a result).  An example
         * might be "SnapPeaTriangulation::homologyFilled".
         */
        SnapPeaIsNull(const char* fromFunction) :
            ReginaException(std::string(fromFunction) +
                "() cannot work with a null SnapPea triangulation") {}
        /**
         * Creates a new copy of the given exception.
         */
        SnapPeaIsNull(const SnapPeaIsNull&) noexcept = default;
        /**
         * Sets this to be a copy of the given exception.
         *
         * \return a reference to this exception.
         */
        SnapPeaIsNull& operator = (const SnapPeaIsNull&) noexcept = default;
};

} // namespace regina

#endif
