/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnapshotWriteError
static const char *SnapshotWriteError =
R"doc(An exception thrown when someone tries to modify the read-only deep
copy taken by a snapshot. See the Snapshot class notes for more
information.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the given references so that they refer to each others'
snapshots.

This global routine simply calls SnapshotRef<T>::swap(); it is
provided so that SnapshotRef<T> meets the C++ Swappable requirements.

See the Snapshot documentation for a full explanation of how Regina's
snapshotting machinery works.

Parameter ``a``:
    the first snapshot reference to swap.

Parameter ``b``:
    the second snapshot reference to swap.)doc";

namespace SnapshotWriteError_ {

// Docstring regina::python::doc::SnapshotWriteError_::SnapshotWriteError
static const char *SnapshotWriteError = R"doc(Creates a new exception.)doc";

// Docstring regina::python::doc::SnapshotWriteError_::SnapshotWriteError_2
static const char *SnapshotWriteError_2 = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::SnapshotWriteError_::what
static const char *what = R"doc(Returns a human-readable description of the error that occurred.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

