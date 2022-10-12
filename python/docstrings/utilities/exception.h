/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FailedPrecondition
static const char *FailedPrecondition =
R"doc(An exception thrown when a function detects that its preconditions
have been violated.

The line between failed preconditions and invalid input arguments is
not always clear, and so the roles of FailedPrecondition versus
InvalidArgument are not clear-cut. Where relevant, each routine will
document clearly which exception type it uses.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

// Docstring regina::python::doc::FileError
static const char *FileError =
R"doc(An exception thrown when trying to access data from the filesystem.

This could occur, for instance, when trying to read data from the
Regina or SnapPea census databases.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

// Docstring regina::python::doc::InvalidArgument
static const char *InvalidArgument =
R"doc(An exception thrown when invalid or unsupported arguments are passed
into a function.

The line between failed preconditions and invalid input arguments is
not always clear, and so the roles of FailedPrecondition versus
InvalidArgument are not clear-cut. Where relevant, each routine will
document clearly which exception type it uses.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

// Docstring regina::python::doc::InvalidInput
static const char *InvalidInput =
R"doc(An exception thrown when a function reads unexpected or incomplete
data from an input stream.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

// Docstring regina::python::doc::NoSolution
static const char *NoSolution =
R"doc(An exception thrown when Regina has certified that a mathematical
problem has no solution.

Note that the constructor for this exception class follows a different
pattern from most of Regina's exception classes.)doc";

// Docstring regina::python::doc::NotImplemented
static const char *NotImplemented =
R"doc(An exception thrown when some functionality is not yet implemented.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

// Docstring regina::python::doc::ReginaException
static const char *ReginaException =
R"doc(A base class for all of the exceptions that are thrown by Regina's
native mathematical code.

This does *not* include SnapPeaFatalError and SnapPeaMemoryFull, which
represent critical errors thrown from within the SnapPea kernel; it
also does not include SnapshotWriteError, which is part of the generic
(non-mathematical) snapshotting machinery.

Details of the error can be accessed through the inherited member
function what().)doc";

// Docstring regina::python::doc::SnapPeaIsNull
static const char *SnapPeaIsNull =
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

// Docstring regina::python::doc::SnapPeaUnsolvedCase
static const char *SnapPeaUnsolvedCase =
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

// Docstring regina::python::doc::UnsolvedCase
static const char *UnsolvedCase =
R"doc(An exception thrown when a mathematical function is not able to solve
a particular instance of a problem.

All member functions follow the same pattern as the parent class
ReginaException, and are not documented again here.)doc";

namespace NoSolution_ {

// Docstring regina::python::doc::NoSolution_::NoSolution
static const char *NoSolution = R"doc(Creates a new exception with a stock error message.)doc";

// Docstring regina::python::doc::NoSolution_::NoSolution_2
static const char *NoSolution_2 = R"doc(Creates a new copy of the given exception.)doc";

}

namespace ReginaException_ {

// Docstring regina::python::doc::ReginaException_::ReginaException
static const char *ReginaException =
R"doc(Creates a new exception with the given error message.

This constructor may throw std::bad_alloc.

Parameter ``msg``:
    a human-readable description of the error.)doc";

// Docstring regina::python::doc::ReginaException_::ReginaException_2
static const char *ReginaException_2 =
R"doc(Creates a new exception with the given error message.

This constructor may throw std::bad_alloc.

Parameter ``msg``:
    a human-readable description of the error.)doc";

// Docstring regina::python::doc::ReginaException_::ReginaException_3
static const char *ReginaException_3 = R"doc(Creates a new copy of the given exception.)doc";

}

namespace SnapPeaIsNull_ {

// Docstring regina::python::doc::SnapPeaIsNull_::SnapPeaIsNull
static const char *SnapPeaIsNull =
R"doc(Creates a new exception, and marks it as having occurred within the
given Regina function.

Parameter ``fromFunction``:
    the name of the Regina function that the user called (in
    particular, this is not the name of the SnapPea kernel function
    that would have been called as a result). An example might be
    "SnapPeaTriangulation::homologyFilled".)doc";

// Docstring regina::python::doc::SnapPeaIsNull_::SnapPeaIsNull_2
static const char *SnapPeaIsNull_2 = R"doc(Creates a new copy of the given exception.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

