/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TableView
constexpr const char *TableView =
R"doc(A lightweight object that can be used for random access to all
elements of a given multi-dimensional table.

This access is read-only, in the sense that both the table itself and
the table elements are read-only. (Of course, if the table elements
are non-const pointers then this means that the *pointers* cannot be
reassigned to point to different objects, but the objects they *point*
to can still be modified.)

This class is in a sense a multi-dimensional analogue to TableView
(though it does also support one-dimensional tables).

Where this class differs from ListView is:

* TableView supports multi-dimensional tables, whereas ListView only
  supports one-dimensional lists.

* TableView offers a smaller set of member functions, whereas ListView
  has a richer interface.

* TableView is (for now) only designed to work with fixed-size C-style
  arrays of the form ``Element[a][b]...[z]``, where the array
  dimensions are compile-time constants. In contrast, ListView can
  also work with rich C++ container classes and variable-sized C-style
  arrays.

* While ListView has a purpose in C++ (to hide the "real" type used by
  the underlying implementation), TableView is primary for the Python
  bindings: its main benefit is to strictly enforce read-only access
  (since Python loses all knowledge of constness, and sometimes allows
  users to change things that they should not). Typically TableView
  would be used to wrap global constant arrays (such as
  regina::quadDefn, or regina::Edge<3>::edgeNumber).

TableView comes with deduction guides for tables of dimension ≤ 3.
This means that you can simply create a TableView using the syntax
``TableView(array)``, where *array* is the underlying C-style array,
without having to specify any TableView template arguments.

TableView objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    The TableView classes are deliberately difficult to access: they
    live within their own private Python namespaces, and are all given
    the same class name (*TableView*). You would typically only
    interact with a TableView when accessing a constant array (e.g.,
    Edge<3>::edgeNumber). In most cases you would simply use array-
    like operators to access the elements of such a table without ever
    knowing its exact type.

Template parameter ``Element``:
    the type of element stored in the C-style array. This should not
    be a ``const`` type; the ``const`` modifier will be added
    automatically where necessary through the class interface.

Template parameter ``dim``:
    the dimensions of the C-style array. There must always be at least
    one dimension supplied.)doc";

namespace TableView_ {

// Docstring regina::python::doc::TableView_::__array
constexpr const char *__array =
R"doc(Returns the requested sub-array of a multi-dimensional array, or the
requested element of a one-dimensional array.

If this array is one-dimensional, then this operator simply returns
the (*index*)th element (as a const reference).

If this array has more than one dimension, then this operator returns
a TableView of smaller dimension, representing the slice of the
overall table obtained when the first array index is set to the given
value.

Typically this operator would just be used to access an individual
element using the syntax ``array[index_1][index_2]...[index_dim]``,
where *dim* is the dimension of this array.

Parameter ``index``:
    indicates which element or sub-array to return; this must be
    between 0 and (*dim1*-1) inclusive.

Returns:
    the (*index*)th sub-array.)doc";

// Docstring regina::python::doc::TableView_::__copy
constexpr const char *__copy = R"doc(Creates a new copy of the given table view.)doc";

// Docstring regina::python::doc::TableView_::__eq
constexpr const char *__eq =
R"doc(Determines whether this and the given table view are accessing the
same underlying C-style array.

To be considered the same array, the two arrays must have the same
location in memory (i.e., the pointers that define the C-style arrays
must be equal). In particular, it is not enough for the two arrays
just to have identical contents.

Parameter ``other``:
    the table view to compare with this.

Returns:
    ``True`` if and only if this and the given table use the same
    underlying C-style array.)doc";

// Docstring regina::python::doc::TableView_::__ne
constexpr const char *__ne =
R"doc(Determines whether this and the given table view are accessing
different underlying C-style arrays.

To be considered the same array, the two arrays must have the same
location in memory (i.e., the pointers that define the C-style arrays
must be equal). In particular, it is not enough for the two arrays
just to have identical contents.

Parameter ``other``:
    the table view to compare with this.

Returns:
    ``True`` if and only if this and the given table use different
    underlying C-style arrays.)doc";

// Docstring regina::python::doc::TableView_::size
constexpr const char *size =
R"doc(Returns the size of this array, across all of the array dimensions.

This returns a sequence (*s_1*, *s_2*, ..., *s_dim*), where *dim* is
the dimension of this array, and where the *k*th array subscript
ranges from 0 to (*s_k*-1) inclusive.

Python:
    In Python, the special method __len__() returns *s_1*. That is, it
    indicates the allowed range for the *first* array subscript.

Returns:
    the size of this array, across all of the array dimensions.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

