/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct ArbitraryPrecisionIntegerVector {

// Docstring regina::python::doc::ArbitraryPrecisionIntegerVector::__concept
static constexpr const char __concept[] =
R"doc(A mathematical vector type derived from (or the same as)
``Vector<T>``, where the element type *T* is one of Regina's
arbitrary-precision integer types (Integer or LargeInteger).

Concepts:
    ArbitraryPrecisionIntegerVector is a C++ concept. Concepts work
    with the C++ compiler at build time: you cannot test in Python
    which concepts are satisfied by which types. Instead, what this
    Python wrapper offers is the concept _documentation_ (which you
    are reading now).)doc";

}; // struct ArbitraryPrecisionIntegerVector

struct IntegerVector {

// Docstring regina::python::doc::IntegerVector::__concept
static constexpr const char __concept[] =
R"doc(A mathematical vector type derived from (or the same as)
``Vector<T>``, where the element type *T* is one of Regina's own
integer types (Integer, LargeInteger, or NativeInteger).

Concepts:
    IntegerVector is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct IntegerVector

struct SignatureEncodable {

// Docstring regina::python::doc::SignatureEncodable::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's mathematical types that allows generation of and
reconstruction from string-based signatures, up to combinatorial
isomorphism.

In particular, second-generation signatures must be supported via
``T::neoSig()``.

Examples of such types include ``Triangulation<dim>`` and Link.

Concepts:
    SignatureEncodable is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct SignatureEncodable

struct SignatureType {

// Docstring regina::python::doc::SignatureType::__concept
static constexpr const char __concept[] =
R"doc(A type that can be used to hold signatures that identify mathematical
objects uniquely up to combinatorial isomorphism. Such signatures
include isomorphism signatures of triangulations, and knot/link
signatures.

Important semantic requirements for this type are:

* the operation ``x += y`` must concatenate *y* to the end of *x*;

* the default constructor must create an empty signature;

* the output operator should write printable characters (for example,
  regina::ByteSequence writes its bytes in hexadecimal, not as raw
  bytes).

Examples of such types include ``std::string`` and
``regina::ByteSequence``.

Concepts:
    SignatureType is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct SignatureType

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

