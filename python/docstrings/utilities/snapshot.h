/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnapshotWriteError
constexpr const char *SnapshotWriteError =
R"doc(An exception thrown when someone tries to modify the read-only deep
copy taken by a snapshot. See the Snapshot class notes for more
information.)doc";

// Docstring regina::python::doc::Snapshottable
constexpr const char *Snapshottable =
R"doc(A base class for images of type *T* that can be snapshotted at a
particular moment in time.

See the Snapshot documentation for a full explanation of how Regina's
snapshotting machinery works.

As a quick recap, the requirements for the class *T* are:

* *T* must derive from Snapshottable<T>, and must have a copy
  constructor.

* If *T* supports any move, copy and/or swap operations, then these
  must call the corresponding operations from Snapshottable<T>.

* Every modifying member function must call
  Snapshottable<T>::takeSnapshot() before the modification takes
  place. There are a handful of exceptions to this requirement, as
  described below.

* The destructor must likewise call Snapshottable<T>::takeSnapshot()
  before any data is destroyed.

There are some situations where an object of type *T* is modified but
does _not_ need to call takeSnapshot(). These include:

* move, copy and swap operations, since these are required to call the
  base class implementations from Snapshottable<T>, which take care of
  any snapshotting that is required;

* modifications of objects that are freshly constructed, and cannot
  possibly have snapshots that refer to them yet.

Python:
    Not present, but the routine isReadOnlySnapshot() will be provided
    directly through each corresponding subclass *T*.)doc";

namespace SnapshotWriteError_ {

// Docstring regina::python::doc::SnapshotWriteError_::__copy
constexpr const char *__copy = R"doc(Creates a new copy of the given exception.)doc";

// Docstring regina::python::doc::SnapshotWriteError_::__default
constexpr const char *__default = R"doc(Creates a new exception.)doc";

// Docstring regina::python::doc::SnapshotWriteError_::what
constexpr const char *what = R"doc(Returns a human-readable description of the error that occurred.)doc";

}

namespace Snapshottable_ {

// Docstring regina::python::doc::Snapshottable_::isReadOnlySnapshot
constexpr const char *isReadOnlySnapshot =
R"doc(Determines if this object is a read-only deep copy that was created by
a snapshot.

Recall that, if an image *I* of type *T* has a snapshot pointing to
it, and if that image *I* is about to be modified or destroyed, then
the snapshot will make an internal deep copy of *I* and refer to that
instead.

The purpose of this routine is to identify whether the current object
is such a deep copy. This may be important information, since a
snapshot's deep copy is read-only: it must not be modified or
destroyed by the outside world. (Of course the only way to access this
deep copy is via const reference from the SnapshotRef dereference
operators, but there are settings in which this constness is
"forgotten", such as Regina's Python bindings.)

Returns:
    ``True`` if and only if this object is a deep copy that was taken
    by a Snapshot object of some original type *T* image.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

