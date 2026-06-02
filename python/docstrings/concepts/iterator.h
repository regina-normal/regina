/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct BidirectionalIterableFor {

// Docstring regina::python::doc::BidirectionalIterableFor::__concept
static constexpr const char __concept[] =
R"doc(A container-like type that can be iterated over both forwards and
backwards via ``begin()`` and ``end()`` member functions, and whose
elements can be assigned or converted to the type *Target*.

The corresponding iterator type must be a bidirectional iterator.

Here _elements_ means the values obtained when dereferencing
iterators. When converting elements to the type *Target*, both
construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    BidirectionalIterableFor is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct BidirectionalIterableFor

struct BidirectionalIteratorFor {

// Docstring regina::python::doc::BidirectionalIteratorFor::__concept
static constexpr const char __concept[] =
R"doc(A bidirectional iterator whose dereferenced values can be assigned or
converted to the type *Target*.

Both construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    BidirectionalIteratorFor is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct BidirectionalIteratorFor

struct CharIterator {

// Docstring regina::python::doc::CharIterator::__concept
static constexpr const char __concept[] =
R"doc(An input iterator that iterates over characters. Such an iterator
would typically be used when reading characters from an input stream
or a string.

Dereferencing the iterator should yield a ``char``, possibly as a
reference and possibly ``const``.

Concepts:
    CharIterator is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct CharIterator

struct ForwardIteratorFor {

// Docstring regina::python::doc::ForwardIteratorFor::__concept
static constexpr const char __concept[] =
R"doc(A forward iterator whose dereferenced values can be assigned or
converted to the type *Target*.

Both construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    ForwardIteratorFor is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct ForwardIteratorFor

struct IndexedContainer {

// Docstring regina::python::doc::IndexedContainer::__concept
static constexpr const char __concept[] =
R"doc(A container-like type whose elements can be access via indexing.

We do _not_ require that the elements be accessible via iteration. We
do however require some other parts of a standard container interface,
including ``T::value_type`` and ``T::size()``.

Concepts:
    IndexedContainer is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct IndexedContainer

struct InputIteratorFor {

// Docstring regina::python::doc::InputIteratorFor::__concept
static constexpr const char __concept[] =
R"doc(An input iterator whose dereferenced values can be assigned or
converted to the type *Target*.

Both construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    InputIteratorFor is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct InputIteratorFor

struct Iterable {

// Docstring regina::python::doc::Iterable::__concept
static constexpr const char __concept[] =
R"doc(A container-like type that can be iterated over via ``begin()`` and
``end()`` member functions.

The corresponding iterator type must be a forward iterator.

Concepts:
    Iterable is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct Iterable

struct IterableFor {

// Docstring regina::python::doc::IterableFor::__concept
static constexpr const char __concept[] =
R"doc(A container-like type that can be iterated over via ``begin()`` and
``end()`` member functions, and whose elements can be assigned or
converted to the type *Target*.

The corresponding iterator type must be a forward iterator.

Here _elements_ means the values obtained when dereferencing
iterators. When converting elements to the type *Target*, both
construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    IterableFor is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct IterableFor

struct OutputIterator {

// Docstring regina::python::doc::OutputIterator::__concept
static constexpr const char __concept[] =
R"doc(An output iterator type.

The reason for using OutputIterator instead of std::output_iterator is
that this concept does not require you to specify the output type in
advance. Instead, the output type is deduced automatically via
``std::iter_value_t``.

Concepts:
    OutputIterator is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct OutputIterator

struct PacketIterator {

// Docstring regina::python::doc::PacketIterator::__concept
static constexpr const char __concept[] =
R"doc(An input iterator that iterates over packets.

Dereferencing the iterator should yield a reference type ``Packet&``
(possibly ``const``). Moreover, the iterator should have a ``bool``
operator that returns ``True`` if and only if the iterator is
dereferenceable.

Concepts:
    PacketIterator is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct PacketIterator

struct RandomAccessIterableFor {

// Docstring regina::python::doc::RandomAccessIterableFor::__concept
static constexpr const char __concept[] =
R"doc(A container-like type that can be iterated over in a random access
manner via ``begin()`` and ``end()`` member functions, and whose
elements can be assigned or converted to the type *Target*.

The corresponding iterator type must be a random access iterator.

Here _elements_ means the values obtained when dereferencing
iterators. When converting elements to the type *Target*, both
construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    RandomAccessIterableFor is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct RandomAccessIterableFor

struct RandomAccessIteratorFor {

// Docstring regina::python::doc::RandomAccessIteratorFor::__concept
static constexpr const char __concept[] =
R"doc(A random access iterator whose dereferenced values can be assigned or
converted to the type *Target*.

Both construction and the assignment operator should be supported, and
implicit conversion should be supported also.

Concepts:
    RandomAccessIteratorFor is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct RandomAccessIteratorFor

struct SelfSentinelInputIterator {

// Docstring regina::python::doc::SelfSentinelInputIterator::__concept
static constexpr const char __concept[] =
R"doc(An input iterator that knows when iteration has finished.

Specifically, such an iterator should have a ``bool`` operator that
returns ``True`` if and only if the iterator is dereferenceable.

Concepts:
    SelfSentinelInputIterator is a C++ concept. Concepts work with the
    C++ compiler at build time: you cannot test in Python which
    concepts are satisfied by which types. Instead, what this Python
    wrapper offers is the concept _documentation_ (which you are
    reading now).)doc";

}; // struct SelfSentinelInputIterator

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

