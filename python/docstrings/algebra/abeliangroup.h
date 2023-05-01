/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::AbelianGroup
static const char *AbelianGroup =
R"doc(Represents a finitely generated abelian group.

The torsion elements of the group are stored in terms of their
invariant factors. For instance, Z_2+Z_3 will appear as Z_6, and
Z_2+Z_2+Z_3 will appear as Z_2+Z_6.

In general the factors will appear as Z_*d0*+...+Z_*dn*, where the
invariant factors *di* are all greater than 1 and satisfy
*d0*|*d1*|...|*dn*. Note that this representation is unique.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace AbelianGroup_ {

// Docstring regina::python::doc::AbelianGroup_::__copy
static const char *__copy = R"doc(Creates a clone of the given group.)doc";

// Docstring regina::python::doc::AbelianGroup_::__default
static const char *__default = R"doc(Creates a new trivial group.)doc";

// Docstring regina::python::doc::AbelianGroup_::__eq
static const char *__eq =
R"doc(Determines whether this and the given abelian group have identical
presentations (which means they are isomorphic).

Since the AbelianGroup class stores _only_ the invariants required to
identify the isomorphism type, two groups will compare as equal if and
only if they are isomorphic. This is in contrast to the comparisons
for GroupPresentation (which tests for identical generators and
relations), or for MarkedAbelianGroup (which tests for identical chain
complex presentations).

Parameter ``other``:
    the group with which this should be compared.

Returns:
    ``True`` if and only if the two groups have identical
    presentations (i.e., they are isomorphic).)doc";

// Docstring regina::python::doc::AbelianGroup_::__init
static const char *__init =
R"doc(Creates a free abelian group of the given rank.

Parameter ``rank``:
    the rank of the new group.)doc";

// Docstring regina::python::doc::AbelianGroup_::__init_2
static const char *__init_2 =
R"doc(Creates a new group with the given rank and invariant factors.

Exception ``InvalidArgument``:
    The invariant factors were not all greater than 1, and/or they did
    not satisfy the divisibily requirement (where each invariant
    factor must divide the one after it).

Template parameter ``Container``:
    a container or view that supports reverse iteration via rbegin(),
    rend(), that has an empty() function, and whose elements may be of
    a native C++ integer type or one of Regina's own integer types. A
    suitable example might be std::vector<int>.

Parameter ``rank``:
    the rank of the new group (i.e., the number of copies of *Z*).

Parameter ``invFac``:
    the list of invariant factors *d0*, *d1*, ..., as described in the
    class notes, where each invariant factor is greater than 1 and
    divides the invariant factor after it.)doc";

// Docstring regina::python::doc::AbelianGroup_::__init_3
static const char *__init_3 =
R"doc(Creates the abelian group defined by the given presentation matrix.

Each column of the matrix represents a generator, and each row of the
matrix represents a relation.

Parameter ``presentation``:
    a presentation matrix for the new group.)doc";

// Docstring regina::python::doc::AbelianGroup_::__init_4
static const char *__init_4 =
R"doc(Creates an abelian group as the homology of a chain complex. The
abelian group is the kernel of *M* modulo the image of *N*.

The matrices should be thought of as acting on column vectors: this
means that the product ``B×A`` applies the linear transformation *A*,
then the linear transformation *B*. This is consistent (for example)
with the convention that Regina uses for for multiplying permutations.

Precondition:
    M.columns() = N.rows(). This condition will be tested, and an
    exception will be thrown if it does not hold.

Precondition:
    The product M×N = 0. This condition will _not_ be tested (for
    efficiency reasons); this is left to the user/programmer to
    ensure.

Exception ``InvalidArgument``:
    The number of columns in *M* does not match the number of rows in
    *N*.

Parameter ``M``:
    the `right' matrix in the chain complex; that is, the matrix that
    one takes the kernel of when computing homology.

Parameter ``N``:
    the `left' matrix in the chain complex; that is, the matrix that
    one takes the image of when computing homology.)doc";

// Docstring regina::python::doc::AbelianGroup_::__init_5
static const char *__init_5 =
R"doc(Creates an abelian group as the homology of a chain complex, using
mod-*p* coefficients. The abelian group is the kernel of *M* modulo
the image of *N*.

The matrices should be thought of as acting on column vectors: this
means that the product ``B×A`` applies the linear transformation *A*,
then the linear transformation *B*. This is consistent (for example)
with the convention that Regina uses for for multiplying permutations.

Precondition:
    M.columns() = N.rows(). This condition will be tested, and an
    exception will be thrown if it does not hold.

Precondition:
    The product M×N = 0. This condition will _not_ be tested (for
    efficiency reasons); this is left to the user/programmer to
    ensure.

Exception ``InvalidArgument``:
    The number of columns in *M* does not match the number of rows in
    *N*.

Parameter ``M``:
    the `right' matrix in the chain complex; that is, the matrix that
    one takes the kernel of when computing homology.

Parameter ``N``:
    the `left' matrix in the chain complex; that is, the matrix that
    one takes the image of when computing homology.

Parameter ``p``:
    the modulus, which may be any Integer. Zero is interpreted as a
    request for integer coefficents, which will give the same result
    as the AbelianGroup(MatrixInt, MatrixInt) constructor.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::AbelianGroup_::__ne
static const char *__ne =
R"doc(Determines whether this and the given abelian group have different
presentations (which means they are non-isomorphic).

Since the AbelianGroup class stores _only_ the invariants required to
identify the isomorphism type, two groups will compare as equal if and
only if they are isomorphic. This is in contrast to the comparisons
for GroupPresentation (which tests for identical generators and
relations), or for MarkedAbelianGroup (which tests for identical chain
complex presentations).

Parameter ``other``:
    the group with which this should be compared.

Returns:
    ``True`` if and only if the two groups have different
    presentations (i.e., they are non-isomorphic).)doc";

// Docstring regina::python::doc::AbelianGroup_::addGroup
static const char *addGroup =
R"doc(Adds the abelian group defined by the given presentation to this
group. Note that this routine might be slow since calculating the new
invariant factors is not trivial.

Parameter ``presentation``:
    a presentation matrix for the group to be added to this group,
    where each column represents a generator and each row a relation.)doc";

// Docstring regina::python::doc::AbelianGroup_::addGroup_2
static const char *addGroup_2 =
R"doc(Adds the given abelian group to this group.

Parameter ``group``:
    the group to add to this one.)doc";

// Docstring regina::python::doc::AbelianGroup_::addRank
static const char *addRank =
R"doc(Increments the rank of the group by the given integer. This integer
may be positive, negative or zero.

Precondition:
    The current rank plus the given integer is non-negative. In other
    words, if we are subtracting rank then we are not trying to
    subtract more rank than the group actually has.

Parameter ``extraRank``:
    the extra rank to add; this defaults to 1.)doc";

// Docstring regina::python::doc::AbelianGroup_::addTorsion
static const char *addTorsion =
R"doc(Adds the given torsion element to the group.

As of Regina 7.0, this routine is much faster than it used to be. In
particular, if you have many torsion elements to add, it is now
efficient just to call addTorsion() for each new torsion element, one
at a time.

In this routine we add a single copy of Z_*d*, where *d* is the given
degree.

Parameter ``degree``:
    the degree of the new torsion element; this must be strictly
    positive.)doc";

// Docstring regina::python::doc::AbelianGroup_::countInvariantFactors
static const char *countInvariantFactors =
R"doc(Returns the number of invariant factors that describe the torsion
elements of this group. See the AbelianGroup class notes for further
details.

Returns:
    the number of invariant factors.)doc";

// Docstring regina::python::doc::AbelianGroup_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given abelian groups.

This global routine simply calls AbelianGroup::swap(); it is provided
so that AbelianGroup meets the C++ Swappable requirements.

Parameter ``lhs``:
    the group whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the group whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::AbelianGroup_::invariantFactor
static const char *invariantFactor =
R"doc(Returns the given invariant factor describing the torsion elements of
this group. See the AbelianGroup class notes for further details.

If the invariant factors are *d0*|*d1*|...|*dn*, this routine will
return *di* where *i* is the value of parameter *index*.

Parameter ``index``:
    the index of the invariant factor to return; this must be between
    0 and countInvariantFactors()-1 inclusive.

Returns:
    the requested invariant factor.)doc";

// Docstring regina::python::doc::AbelianGroup_::isFree
static const char *isFree =
R"doc(Determines whether this is the free abelian group of the given rank.

Parameter ``r``:
    the rank of the free abelian group that we are testing for.

Returns:
    ``True`` if and only if this is the free abelian group of rank
    *r*.)doc";

// Docstring regina::python::doc::AbelianGroup_::isTrivial
static const char *isTrivial =
R"doc(Determines whether this is the trivial (zero) group.

Returns:
    ``True`` if and only if this is the trivial group.)doc";

// Docstring regina::python::doc::AbelianGroup_::isZ
static const char *isZ =
R"doc(Determines whether this is the infinite cyclic group (Z).

Returns:
    ``True`` if and only if this is the infinite cyclic group.)doc";

// Docstring regina::python::doc::AbelianGroup_::isZn
static const char *isZn =
R"doc(Determines whether this is the non-trivial cyclic group on the given
number of elements.

As a special case, if *n* = 0 then this routine will test for the
infinite cyclic group (i.e., it will behave the same as isZ()). If *n*
= 1, then this routine will test for the trivial group (i.e., it will
behave the same as isTrivial()).

Parameter ``n``:
    the number of elements of the cyclic group in question.

Returns:
    ``True`` if and only if this is the cyclic group Z_n.)doc";

// Docstring regina::python::doc::AbelianGroup_::rank
static const char *rank =
R"doc(Returns the rank of the group. This is the number of included copies
of *Z*.

Equivalently, the rank is the maximum number of linearly independent
elements, and it indicates the size of the largest free abelian
subgroup. The rank effectively ignores all torsion elements.

.. warning::
    SnapPy users should be aware that SnapPy defines rank differently.
    Specifically, SnapPy's AbelianGroup.rank() computation includes
    torsion factors also.

Returns:
    the number of included copies of *Z*.)doc";

// Docstring regina::python::doc::AbelianGroup_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given abelian group.

Parameter ``other``:
    the group whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::AbelianGroup_::torsionRank
static const char *torsionRank =
R"doc(Returns the rank in the group of the torsion term of given degree. If
the given degree is *d*, this routine will return the largest *m* for
which *m*Z_*d* is a subgroup of this group.

For instance, if this group is Z_6+Z_12, the torsion term of degree 2
has rank 2 (one occurrence in Z_6 and one in Z_12), and the torsion
term of degree 4 has rank 1 (one occurrence in Z_12).

Precondition:
    The given degree is at least 2.

Parameter ``degree``:
    the degree of the torsion term to query.

Returns:
    the rank in the group of the given torsion term.)doc";

// Docstring regina::python::doc::AbelianGroup_::torsionRank_2
static const char *torsionRank_2 =
R"doc(Returns the rank in the group of the torsion term of given degree. If
the given degree is *d*, this routine will return the largest *m* for
which *m*Z_*d* is a subgroup of this group.

For instance, if this group is Z_6+Z_12, the torsion term of degree 2
has rank 2 (one occurrence in Z_6 and one in Z_12), and the torsion
term of degree 4 has rank 1 (one occurrence in Z_12).

Precondition:
    The given degree is at least 2.

Parameter ``degree``:
    the degree of the torsion term to query.

Returns:
    the rank in the group of the given torsion term.)doc";

// Docstring regina::python::doc::AbelianGroup_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this abelian group.

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

