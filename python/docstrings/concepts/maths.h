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
arbitrary-precision integer types (Integer or LargeInteger).)doc";

}; // struct ArbitraryPrecisionIntegerVector

struct IntegerVector {

// Docstring regina::python::doc::IntegerVector::__concept
static constexpr const char __concept[] =
R"doc(A mathematical vector type derived from (or the same as)
``Vector<T>``, where the element type *T* is one of Regina's own
integer types (Integer, LargeInteger, or NativeInteger).)doc";

}; // struct IntegerVector

struct SignatureReconstructible {

// Docstring regina::python::doc::SignatureReconstructible::__concept
static constexpr const char __concept[] =
R"doc(One of Regina's mathematical types that allows reconstruction from
string-based signatures, up to combinatorial isomorphism.

Examples of such types include ``Triangulation<dim>`` and Link.)doc";

}; // struct SignatureReconstructible

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
``regina::ByteSequence``.)doc";

}; // struct SignatureType

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

