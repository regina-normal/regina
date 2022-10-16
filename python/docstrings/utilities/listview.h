/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ListView
constexpr const char *ListView =
R"doc(A lightweight object that can be used for iteration and random access
to all elements of a given list.

This access is read-only, in the sense that both the list itself and
the list elements are read-only. (Of course, if the list elements are
non-const pointers then this means that the *pointers* cannot be
reassigned to point to different objects, but the objects they *point*
to can still be modified.)

Typically a ListView would be returned from a class member function to
grant the user some basic read-only access to a much richer private
data structure, in a way that allows the internal data structure to
change at some later date without affecting the public API.

The ListView class supports several different ways of representing a
list:

* If your list is stored using a container class (e.g., std::vector or
  regina::MarkedVector), then you can create a ListView directly from
  the container using the syntax ``ListView(container)``. This uses
  the generic ListView<Container> class template. There is no need to
  explicitly specify the ListView template arguments.

* If your list is stored using a C-style array whose size is not known
  at compile-time, you can create a ListView using either the syntax
  ``ListView(array, size)`` or ``ListView(begin, end)``. Here *array*
  is a pointer to the beginning of the array, and *begin* and *end*
  behave as an iterator pair (so ``begin == array`` and ``end == array
  + size``). This syntax uses the specialised ListView<Element*> class
  template. Again, there is no need to explicitly specify the ListView
  template arguments.

* If your list is stored using a C-style array whose size is fixed at
  compile-time (i.e., the type is ``Element[n]`` for some constant
  *n*), you can create a ListView using the syntax
  ``ListView(array)``. Once again, there is no need to explicitly
  specify the ListView template arguments.

End users should always store ListView objects using ``auto``, not by
explicitly writing out the full ListView type. One reason for this is
that, if/when Regina moves to C++20, the ListView class will most
likely be removed completely (in favour of the new C++20 ranges
library).

ListView objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    The ListView classes are deliberately difficult to access: they
    live within their own private Python namespaces, and are all give
    the same class name (*ListView*). You would typically only
    interact with a ListView when it is returned from a function
    (e.g., ``Link.crossings()``), and in most cases you would simply
    iterate over this resulting ListView without ever knowing its
    exact type.

Template parameter ``Container``:
    the internal type of the list that this object grants access to.
    This type must support at least the same operations as this class
    itself, except for the copy semantics. In particular, both
    std::vector and regina::MarkedVector types (as well as many other
    standard container types) are suitable.)doc";

namespace ListView_ {

// Docstring regina::python::doc::ListView_::__array
constexpr const char *__array =
R"doc(Returns the requested element of this list.

Parameter ``index``:
    indicates which element to return; this must be between 0 and
    size()-1 inclusive.

Returns:
    the (*index*)th element in this list.)doc";

// Docstring regina::python::doc::ListView_::__eq
constexpr const char *__eq =
R"doc(Determines whether this and the given list view are accessing the same
underlying container.

Here the containers are compared by *reference* (i.e., they must be
the same container object at the same location in memory; it is not
enough to be two containers with identical contents).

Parameter ``other``:
    the list view to compare with this.

Returns:
    ``True`` if and only if this and the given list use the same
    underlying container.)doc";

// Docstring regina::python::doc::ListView_::__init
constexpr const char *__init = R"doc(Creates a new copy of the given list view.)doc";

// Docstring regina::python::doc::ListView_::__iter__
constexpr const char *__iter__ =
R"doc(Returns a Python iterator over the elements of this list.

Returns:
    an iterator over the elements of this list.)doc";

// Docstring regina::python::doc::ListView_::__ne
constexpr const char *__ne =
R"doc(Determines whether this and the given list view are accessing
different underlying containers.

Here the containers are compared by *reference* (i.e., to be
considered equal they must be the same container object at the same
location in memory; it is not enough to be two containers with
identical contents).

Parameter ``other``:
    the list view to compare with this.

Returns:
    ``True`` if and only if this and the given list use different
    underlying containers.)doc";

// Docstring regina::python::doc::ListView_::back
constexpr const char *back =
R"doc(Returns the last element of this list.

Precondition:
    This list is not empty.

Returns:
    the last element in this list.)doc";

// Docstring regina::python::doc::ListView_::empty
constexpr const char *empty =
R"doc(Determines if this list is empty.

Returns:
    ``True`` if and only if this list is empty.)doc";

// Docstring regina::python::doc::ListView_::front
constexpr const char *front =
R"doc(Returns the first element of this list.

Precondition:
    This list is not empty.

Returns:
    the first element in this list.)doc";

// Docstring regina::python::doc::ListView_::size
constexpr const char *size =
R"doc(Returns the number of elements in this list.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of elements.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

