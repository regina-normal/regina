/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PosOrder
static const char *PosOrder =
R"doc(A comparison object that sorts hyperplanes by position vectors. This
ordering is described in "Optimizing the double description method for
normal surface enumeration", B.A. Burton, Mathematics of Computation
79 (2010), 453-484.

This comparison object is used to sort hyperplanes into a good order
before enumerating vertex or fundamental normal surfaces A hyperplane
is described by a row of the *subspace* matrix, as passed to an
enumeration routine such as DoubleDescription::enumerate() or
HilbertDual::enumerate().

The ordering is defined as follows. For each hyperplane, we create a
_position vector_ (h_1, ..., h_f), where h_i is 0 if the hyperplane
contains the ith coordinate axis, or 1 if not. We then compare these
position vectors lexicographically.

A PosOrder object behaves like a reference: it is lightweight and can
be copy-constructed cheaply, but it does not support assignments or
swaps.)doc";

namespace PosOrder_ {

// Docstring regina::python::doc::PosOrder_::__call
static const char *__call =
R"doc(Determines whether the hyperplane described by row *i* of the matrix
is smaller than the hyperplane described by row *j*. Here "smaller" is
defined by position vectors; see the PosOrder class notes for details.

Parameter ``i``:
    the first matrix row index; this must be between 0 and
    matrix.rows()-1 inclusive, where *matrix* is the matrix passed to
    the class constructor.

Parameter ``j``:
    the second matrix row index; this must also be between 0 and
    matrix.rows()-1 inclusive.

Returns:
    ``True`` if and only if the hyperplane described by row *i* is
    smaller than the hyperplane described by row *j*.)doc";

// Docstring regina::python::doc::PosOrder_::__copy
static const char *__copy = R"doc(Creates a clone of the given helper objet.)doc";

// Docstring regina::python::doc::PosOrder_::__init
static const char *__init =
R"doc(Creates a new helper object for comparing hyperplanes.

Parameter ``matrix``:
    the *subspace* matrix as passed to the normal surface enumeration
    routine.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

