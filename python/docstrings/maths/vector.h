/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Vector
static const char *Vector =
R"doc(An optimised vector class of elements from a given ring T. Various
mathematical vector operations are available.

This class is intended for serious computation, and as a result it has
a streamlined implementation with no virtual methods. It can be
subclassed, but since there are no virtual methods, type information
must generally be known at compile time. Nevertheless, in many
respects, different subclasses of Vector<T> can happily interact with
one another.

This class is written with bulky types in mind (such as arbitrary
precision integers), and so creations and operations are kept to a
minimum.

As of Regina 7.0, this class explicitly supports zero-length vectors.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

.. warning::
    As of Regina 4.90, this class merges the old functionality of
    NFastVector and the NVector hierarchy from Regina 4.6. As a side-
    effect, the hierarchy has been compressed into just one class
    (NVectorUnit, NVectorMatrix and NVectorDense are gone), elements
    are always stored as dense vectors, and functions are no longer
    virtual (since the storage model is now fixed). The virtual
    clone() method is gone completely (since there are no longer
    virtual functions you should use the copy constructor instead),
    and the old makeLinComb() method is also gone (just use operator
    *= and addCopies()).

Precondition:
    Type T has a copy constructor. That is, if ``a`` and ``b`` are of
    type T, then ``a`` can be initialised to the value of ``b`` using
    ``a(b)``.

Precondition:
    Type T has a default constructor. That is, an object of type T can
    be declared with no arguments. No specific default value is
    required.

Precondition:
    Type T allows for operators ``=``, ``==``, ``+=``, ``-=``, ``*=``,
    ``+``, ``-`` and ``*``.

Precondition:
    Type T has an integer constructor. That is, if ``a`` is of type T,
    then ``a`` can be initialised to an integer ``l`` using ``a(l)``.

Precondition:
    An element ``t`` of type T can be written to an output stream
    ``out`` using the standard expression ``out << t``.

Python:
    Only the specific types Vector<Integer> and Vector<LargeInteger>
    are available, under the names VectorInt and VectorLarge
    respectively.)doc";

namespace Vector_ {

// Docstring regina::python::doc::Vector_::__add
static const char *__add =
R"doc(Adds the given vector to this vector, and returns the result. This
vector will not be changed.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector to add to this vector.

Returns:
    the sum ``this + other``.)doc";

// Docstring regina::python::doc::Vector_::__array
static const char *__array =
R"doc(Returns the element at the given index in the vector. A constant
reference to the element is returned; the element may not be altered.

Precondition:
    ``index`` is between 0 and size()-1 inclusive.

Parameter ``index``:
    the vector index to examine.

Returns:
    the vector element at the given index.)doc";

// Docstring regina::python::doc::Vector_::__array_2
static const char *__array_2 =
R"doc(Gives write access to the element at the given index in the vector.

Precondition:
    ``index`` is between 0 and size()-1 inclusive.

Parameter ``index``:
    the vector index to access.

Returns:
    a reference to the vector element at the given index.)doc";

// Docstring regina::python::doc::Vector_::__copy
static const char *__copy =
R"doc(Creates a new vector that is a clone of the given vector.

Parameter ``src``:
    the vector to clone.)doc";

// Docstring regina::python::doc::Vector_::__eq
static const char *__eq =
R"doc(Determines if this vector is equal to the given vector.

It is safe to call this operator if this and the given vector have
different sizes (in which case the return value will be ``False``).

Parameter ``compare``:
    the vector with which this will be compared.

Returns:
    ``True`` if and only if the this and the given vector are equal.)doc";

// Docstring regina::python::doc::Vector_::__iadd
static const char *__iadd =
R"doc(Adds the given vector to this vector. This vector will be changed
directly. This behaves correctly in the case where *other* is
``this``.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector to add to this vector.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector_::__imul
static const char *__imul =
R"doc(Multiplies this vector by the given scalar. This vector will be
changed directly.

Parameter ``factor``:
    the scalar with which this will be multiplied.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector_::__init
static const char *__init =
R"doc(Creates a new vector.

All entries will be initialised using their default constructors. In
particular, this means that for Regina's own integer classes (Integer,
LargeInteger and NativeInteger), all entries will be initialised to
zero.

.. warning::
    If *T* is a native C++ integer type (such as ``int`` or ``long``),
    then the elements will not be initialised to any particular value.

Parameter ``size``:
    the number of elements in the new vector.)doc";

// Docstring regina::python::doc::Vector_::__init_2
static const char *__init_2 =
R"doc(Creates a new vector and initialises every element to the given value.

Parameter ``size``:
    the number of elements in the new vector.

Parameter ``initValue``:
    the value to assign to every element of the vector.)doc";

// Docstring regina::python::doc::Vector_::__init_3
static const char *__init_3 =
R"doc(Creates a new vector containing the given sequence of elements.

This constructor induces a deep copy of the given range.

Precondition:
    Objects of type *T* can be assigned values from dereferenced
    iterators of type *iterator*.

.. warning::
    This routine computes the length of the given sequence by
    subtracting ``end - begin``, and so ideally *iterator* should be a
    random access iterator type for which this operation is constant
    time.

Python:
    Instead of a pair of iterators, this routine takes a python list
    of coefficients.

Parameter ``begin``:
    the beginning of the sequence of elements.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    elements.)doc";

// Docstring regina::python::doc::Vector_::__init_4
static const char *__init_4 =
R"doc(Creates a new clone of the given vector, which may hold objects of a
different type.

This constructor is marked as explicit in the hope of avoiding
accidental (and unintentional) mixing of vector classes.

Python:
    Using this constructor, Python allows you to construct a
    Vector<Integer> from a Vector<LargeInteger> or vice versa.

Template parameter ``U``:
    the type of object held by the given vector *src*. It must be
    possible to _assign_ an object of type *U* to an object of type
    *T*.

Parameter ``src``:
    the vector to clone.)doc";

// Docstring regina::python::doc::Vector_::__isub
static const char *__isub =
R"doc(Subtracts the given vector from this vector. This vector will be
changed directly. This behaves correctly in the case where *other* is
``this``.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector to subtract from this vector.

Returns:
    a reference to this vector.)doc";

// Docstring regina::python::doc::Vector_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the elements of this vector.

Returns:
    an iterator over the elements of this vector.)doc";

// Docstring regina::python::doc::Vector_::__mul
static const char *__mul =
R"doc(Multiplies this vector by the given scalar, and returns the result.
This vector will not be changed.

Parameter ``factor``:
    the scalar to multiply this vector by.

Returns:
    the product ``this * factor``.)doc";

// Docstring regina::python::doc::Vector_::__mul_2
static const char *__mul_2 =
R"doc(Calculates the dot product of this vector and the given vector.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector with which this will be multiplied.

Returns:
    the dot product of this and the given vector.)doc";

// Docstring regina::python::doc::Vector_::__ne
static const char *__ne =
R"doc(Determines if this vector is different from the given vector.

It is safe to call this operator if this and the given vector have
different sizes (in which case the return value will be ``True``).

Parameter ``compare``:
    the vector with which this will be compared.

Returns:
    ``True`` if and only if the this and the given vector are not
    equal.)doc";

// Docstring regina::python::doc::Vector_::__sub
static const char *__sub =
R"doc(Subtracts the given vector from this vector, and returns the result.
This vector will not be changed.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector to subtract from this vector.

Returns:
    the difference ``this - other``.)doc";

// Docstring regina::python::doc::Vector_::addCopies
static const char *addCopies =
R"doc(Adds the given multiple of the given vector to this vector. This
behaves correctly in the case where *other* is ``this``.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector a multiple of which will be added to this vector.

Parameter ``multiple``:
    the multiple of *other* to be added to this vector.)doc";

// Docstring regina::python::doc::Vector_::elementSum
static const char *elementSum =
R"doc(Returns the sum of all elements of this vector.

Returns:
    the sum of the elements of this vector.)doc";

// Docstring regina::python::doc::Vector_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given vectors.

This global routine simply calls Vector<T>::swap(); it is provided so
that Vector<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first vector whose contents should be swapped.

Parameter ``b``:
    the second vector whose contents should be swapped.)doc";

// Docstring regina::python::doc::Vector_::isZero
static const char *isZero =
R"doc(Determines whether this is the zero vector.

Returns:
    ``True`` if and only if all elements of the vector are zero.)doc";

// Docstring regina::python::doc::Vector_::negate
static const char *negate = R"doc(Negates every element of this vector.)doc";

// Docstring regina::python::doc::Vector_::norm
static const char *norm =
R"doc(Returns the norm of this vector. This is the dot product of the vector
with itself.

Returns:
    the norm of this vector.)doc";

// Docstring regina::python::doc::Vector_::scaleDown
static const char *scaleDown =
R"doc(Scales this vector down by the greatest common divisor of all its
elements. The resulting vector will be the smallest multiple of the
original that maintains integral entries, and these entries will have
the same signs as the originals.

In particular, if this vector is being used to represent a ray
emanating from the origin, then this routine reduces the ray to its
smallest possible integer representation.

This routine poses no problem for vectors containing infinite
elements; such elements are simply ignored and left at infinity.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Returns:
    the integer by which this vector was divided (i.e., the gcd of its
    original elements). This will be strictly positive.)doc";

// Docstring regina::python::doc::Vector_::size
static const char *size =
R"doc(Returns the number of elements in the vector.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the vector size.)doc";

// Docstring regina::python::doc::Vector_::subtractCopies
static const char *subtractCopies =
R"doc(Subtracts the given multiple of the given vector to this vector. This
behaves correctly in the case where *other* is ``this``.

Precondition:
    This and the given vector have the same size.

Parameter ``other``:
    the vector a multiple of which will be subtracted from this
    vector.

Parameter ``multiple``:
    the multiple of *other* to be subtracted from this vector.)doc";

// Docstring regina::python::doc::Vector_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given vector.

Parameter ``other``:
    the vector whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::Vector_::unit
static const char *unit =
R"doc(Returns the given unit vector.

The vector will have length *dimension*. The element in position
*coordinate* will be set to 1, and all other elements will be set to
0.

Parameter ``dimension``:
    the number of elements in the vector.

Parameter ``coordinate``:
    the coordinate position that should hold the value 1; this must be
    between 0 and (*dimension* - 1) inclusive.

Returns:
    the requested unit vector.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

