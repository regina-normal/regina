/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::IntersectionForm
static const char *IntersectionForm =
R"doc(Represents a symmetric bilinear integral form, which for Regina is
typically the intersection form of a 4-manifold.

Such a bilinear form is stored as a symmetric square integer matrix.
For closed 4-manifolds, this matrix is unimodular; however, this class
can also represent more general bilinear forms (including degenerate
forms).

The main thing that this class provides in addition to the square
matrix is access to invariants of the bilinear form, including rank,
signature, and type (even vs odd). All of these invariants are
computed by the class constructor, and so querying them via routines
such as rank(), signature(), even() or odd() is essentially
instantaneous.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the two given intersection forms.

This global routine simply calls IntersectionForm::swap(); it is
provided so that IntersectionForm meets the C++ Swappable
requirements.

Parameter ``lhs``:
    the intersection form whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the intersection form whose contents should be swapped with *lhs*.)doc";

namespace IntersectionForm_ {

// Docstring regina::python::doc::IntersectionForm_::IntersectionForm
static const char *IntersectionForm =
R"doc(Constructs the bilinear form described by the given symmetric square
integer matrix.

The given matrix representation will be stored unchanged (i.e.,
matrix() will return an exact copy of the argument *form*). This
constructor will also compute all of the form invariants (a task which
requires only small polynomial time: the current algorithms are at
worst cubic in the side length of the given matrix).

Precondition:
    The given matrix is square and symmetric.

Exception ``InvalidArgument``:
    The given matrix is not both symmetric and square.

Parameter ``form``:
    a symmetric square integer matrix that describes this bilinear
    form.)doc";

// Docstring regina::python::doc::IntersectionForm_::IntersectionForm_2
static const char *IntersectionForm_2 = R"doc(Creates a clone of the given intersection form.)doc";

// Docstring regina::python::doc::IntersectionForm_::__eq
static const char *__eq =
R"doc(Determines if this and the given bilinear form are represented by the
same symmetric square integer matrices.

This does *not* test whether this and the given form are congruent
(i.e., related by a change basis in the underlying free abelian
group). Instead it is equivalent to testing whether ``matrix() ==
other.matrix()``.

Parameter ``other``:
    the bilinear form to compare with this.

Returns:
    ``True`` if and only if this and the given bilinear form are
    represented by identical matrices.)doc";

// Docstring regina::python::doc::IntersectionForm_::__ne
static const char *__ne =
R"doc(Determines if this and the given bilinear form are not represented by
the same symmetric square integer matrices.

This does *not* test whether this and the given form are congruent
(i.e., related by a change basis in the underlying free abelian
group). Instead it is equivalent to testing whether ``matrix() !=
other.matrix()``.

Parameter ``other``:
    the bilinear form to compare with this.

Returns:
    ``True`` if and only if this and the given bilinear form are not
    represented by identical matrices.)doc";

// Docstring regina::python::doc::IntersectionForm_::even
static const char *even =
R"doc(Returns ``True`` if and only if this bilinear form is of even type.

A form *Q* is *even* if and only if ``Q(x,x)`` is even for all *x*.

Returns:
    ``True`` if this bilinear form is of even type, or ``False`` if it
    is of odd type.)doc";

// Docstring regina::python::doc::IntersectionForm_::matrix
static const char *matrix =
R"doc(Returns a symmetric square integer matrix that describes this bilinear
form.

This will be an exact copy of the matrix that was passed to the class
constructor.

Returns:
    a matrix describing this bilinear form.)doc";

// Docstring regina::python::doc::IntersectionForm_::odd
static const char *odd =
R"doc(Returns ``True`` if and only if this bilinear form is of odd type.

A form *Q* is *odd* if and only if ``Q(x,x)`` is odd for some *x*.

Returns:
    ``True`` if this bilinear form is of odd type, or ``False`` if it
    is of even type.)doc";

// Docstring regina::python::doc::IntersectionForm_::rank
static const char *rank =
R"doc(Returns the rank of this bilinear form. This is the rank of the
underlying symmetric square matrix.

Returns:
    the rank of this bilinear form.)doc";

// Docstring regina::python::doc::IntersectionForm_::signature
static const char *signature =
R"doc(Returns the signature of this bilinear form. This is the number of
positive eigenvalues minus the number of negative eigenvalues of the
underlying symmetric square matrix.

Returns:
    the signature of this bilinear form.)doc";

// Docstring regina::python::doc::IntersectionForm_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given intersection form.

Parameter ``other``:
    the intersection form whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

