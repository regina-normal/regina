/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct DivisionByZero {

// Docstring regina::python::doc::DivisionByZero::__class
static constexpr const char __class[] =
R"doc(An exception thrown when Regina encounters an attempted division by
zero.

Note that, unlike most of Regina's exception classes, the constructor
for DivisionByZero takes no arguments - you do not need to (and indeed
cannot) supply a text description.)doc";

// Docstring regina::python::doc::DivisionByZero::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::DivisionByZero::__default
static constexpr const char __default[] = R"doc(Creates a new exception with a stock error message.)doc";

}; // struct DivisionByZero

struct FailedPrecondition {

// Docstring regina::python::doc::FailedPrecondition::__class
static constexpr const char __class[] =
R"doc(An exception thrown when a function detects that its preconditions
have been violated.

The line between failed preconditions and invalid input arguments is
not always clear, and so the roles of FailedPrecondition versus
InvalidArgument are not clear-cut. Where relevant, each routine will
document clearly which exception type it uses.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct FailedPrecondition

struct FileError {

// Docstring regina::python::doc::FileError::__class
static constexpr const char __class[] =
R"doc(An exception thrown when trying to read or write data to/from the
filesystem.

This could occur, for instance, when trying to read data from the
Regina or SnapPea census databases.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct FileError

struct ImpossibleScenario {

// Docstring regina::python::doc::ImpossibleScenario::__class
static constexpr const char __class[] =
R"doc(An exception thrown when an unexpected scenario arises that should
never be possible.

By "unexpected", we do not mean (for example) that a precondition was
violated, or an invalid argument was passed to some function. Instead,
we mean that within the implementation of some function, a situation
arises that should be provably impossible. Seeing this exception would
most likely indicate a bug in Regina's own source code (as opposed to
an error by the user, or an incorrect use of Regina's API).

Such exceptions will typically not be mentioned in the API
documentation (since, by their nature, they should never be thrown).

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct ImpossibleScenario

struct IntegerOverflow {

// Docstring regina::python::doc::IntegerOverflow::__class
static constexpr const char __class[] =
R"doc(An exception thrown when the result of some calculation cannot fit
into an appropriate native C++ integer type.

Note that, unlike most of Regina's exception classes, the constructor
for IntegerOverflow takes no arguments - you do not need to (and
indeed cannot) supply a text description.)doc";

// Docstring regina::python::doc::IntegerOverflow::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::IntegerOverflow::__default
static constexpr const char __default[] = R"doc(Creates a new exception with a stock error message.)doc";

}; // struct IntegerOverflow

struct InvalidArgument {

// Docstring regina::python::doc::InvalidArgument::__class
static constexpr const char __class[] =
R"doc(An exception thrown when invalid or unsupported arguments are passed
into a function.

The line between failed preconditions and invalid input arguments is
not always clear, and so the roles of FailedPrecondition versus
InvalidArgument are not clear-cut. Where relevant, each routine will
document clearly which exception type it uses.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct InvalidArgument

struct InvalidInput {

// Docstring regina::python::doc::InvalidInput::__class
static constexpr const char __class[] =
R"doc(An exception thrown when a function reads unexpected or incomplete
data from an input stream (or more generally, an input source).

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct InvalidInput

struct LockViolation {

// Docstring regina::python::doc::LockViolation::__class
static constexpr const char __class[] =
R"doc(An exception thrown when an attempt is made to violate a simplex or
facet lock. See Simplex<dim>::lock() and Simplex<dim>::lockFacet() for
further details on simplex/facet locks and what restrictions they
impose.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct LockViolation

struct NoSolution {

// Docstring regina::python::doc::NoSolution::__class
static constexpr const char __class[] =
R"doc(An exception thrown when Regina has certified that a mathematical
problem has no solution.

Note that the constructor for this exception class follows a different
pattern from most of Regina's exception classes.)doc";

// Docstring regina::python::doc::NoSolution::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::NoSolution::__default
static constexpr const char __default[] = R"doc(Creates a new exception with a stock error message.)doc";

}; // struct NoSolution

struct NotImplemented {

// Docstring regina::python::doc::NotImplemented::__class
static constexpr const char __class[] =
R"doc(An exception thrown when some functionality is not yet implemented.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct NotImplemented

struct NumericalError {

// Docstring regina::python::doc::NumericalError::__class
static constexpr const char __class[] =
R"doc(An exception thrown when Regina encounters some kind of numerical or
arithmetical error.

Examples might be integer overflows, or divisions by zero (both of
which are represented by their own subclasses of NumericalError).

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct NumericalError

struct ReginaException {

// Docstring regina::python::doc::ReginaException::__class
static constexpr const char __class[] =
R"doc(A base class for all of the exceptions that are thrown by Regina's
native mathematical code.

This does _not_ include SnapPeaFatalError and SnapPeaMemoryFull, which
represent critical errors thrown from within the SnapPea kernel; it
also does not include SnapshotWriteError, which is part of the generic
(non-mathematical) snapshotting machinery.

Details of the error can be accessed through the inherited member
function what().

Python:
    This uses Python's ``RuntimeError`` as its base class.)doc";

// Docstring regina::python::doc::ReginaException::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::ReginaException::__init
static constexpr const char __init[] =
R"doc(Creates a new exception with the given error message.

This constructor may throw std::bad_alloc.

Parameter ``msg``:
    a human-readable description of the error.)doc";

// Docstring regina::python::doc::ReginaException::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a new exception with the given error message.

This constructor may throw std::bad_alloc.

Parameter ``msg``:
    a human-readable description of the error.)doc";

}; // struct ReginaException

struct SnapPeaIsNull {

// Docstring regina::python::doc::SnapPeaIsNull::__class
static constexpr const char __class[] =
R"doc(An exception thrown when the SnapPea kernel is asked to work with a
null SnapPea triangulation.

This is a "regular" exception type derived from ReginaException, and
is typically thrown in scenarios where the user asks Regina to call
some SnapPea kernel function but Regina has no native SnapPea
triangulation to call it with. This is contrast to SnapPeaFatalError
and SnapPeaMemoryFull, which are more severe errors that occur within
the SnapPea kernel when it aborts processing unexpectedly, and which
are not part of the regular ReginaException class hierarchy.

Note that the constructor for this exception class follows a different
pattern from most of Regina's exception classes.)doc";

// Docstring regina::python::doc::SnapPeaIsNull::__copy
static constexpr const char __copy[] = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::SnapPeaIsNull::__init
static constexpr const char __init[] =
R"doc(Creates a new exception, and marks it as having occurred within the
given Regina function.

Parameter ``fromFunction``:
    the name of the Regina function that the user called (in
    particular, this is not the name of the SnapPea kernel function
    that would have been called as a result). An example might be
    "SnapPeaTriangulation::homologyFilled".)doc";

}; // struct SnapPeaIsNull

struct SnapPeaUnsolvedCase {

// Docstring regina::python::doc::SnapPeaUnsolvedCase::__class
static constexpr const char __class[] =
R"doc(An exception thrown when the SnapPea kernel is not able to perform
some computation.

This is a "regular" exception type derived from ReginaException, and
is typically thrown in scenarios where some SnapPea function returns
cleanly with an error condition. This is contrast to SnapPeaFatalError
and SnapPeaMemoryFull, which are more severe errors that occur when
the SnapPea kernel aborts processing unexpectedly, and which are not
part of the regular ReginaException class hierarchy.

All member functions follow the same pattern as the (grand)parent
class ReginaException, and are not documented again here.)doc";

}; // struct SnapPeaUnsolvedCase

struct UnsolvedCase {

// Docstring regina::python::doc::UnsolvedCase::__class
static constexpr const char __class[] =
R"doc(An exception thrown when a mathematical function is not able to solve
a particular instance of a problem.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

}; // struct UnsolvedCase

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

