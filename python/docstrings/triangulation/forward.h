/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct FaceClass {

// Docstring regina::python::doc::FaceClass::__concept
static constexpr const char __concept[] =
R"doc(Represents one of Regina's ``Face<dim, subdim>`` classes.

If a type adheres to this concept, the precise dimensions can be
accessed via the class constants ``T::dimension`` and
``T::subdimension``.

Concepts:
    FaceClass is a C++ concept. Concepts work with the C++ compiler at
    build time: you cannot test in Python which concepts are satisfied
    by which types. Instead, what this Python wrapper offers is the
    concept _documentation_ (which you are reading now).)doc";

}; // struct FaceClass

struct FaceOrSimplexClass {

// Docstring regina::python::doc::FaceOrSimplexClass::__concept
static constexpr const char __concept[] =
R"doc(Represents one of Regina's ``Face<dim, subdim>`` or ``Simplex<dim>``
classes.

If a type adheres to this concept, the precise dimensions can be
accessed via the class constants ``T::dimension`` and
``T::subdimension``. These can also be used to identify whether the
type is a Face (where ``subdim < dim``) or a Simplex (where ``subdim
== dim``).

Concepts:
    FaceOrSimplexClass is a C++ concept. Concepts work with the C++
    compiler at build time: you cannot test in Python which concepts
    are satisfied by which types. Instead, what this Python wrapper
    offers is the concept _documentation_ (which you are reading now).)doc";

}; // struct FaceOrSimplexClass

struct SimplexClass {

// Docstring regina::python::doc::SimplexClass::__concept
static constexpr const char __concept[] =
R"doc(Represents one of Regina's ``Simplex<dim>`` classes.

If a type adheres to this concept, the precise dimension can be
accessed via the class constant ``T::dimension``.

Concepts:
    SimplexClass is a C++ concept. Concepts work with the C++ compiler
    at build time: you cannot test in Python which concepts are
    satisfied by which types. Instead, what this Python wrapper offers
    is the concept _documentation_ (which you are reading now).)doc";

}; // struct SimplexClass

namespace detail {

struct SafeFaceHelper {

// Docstring regina::python::doc::detail::SafeFaceHelper::__class
static constexpr const char __class[] =
R"doc(Implementation details for SafeFace<dim, subdim>. See SafeFace for
further information.)doc";

}; // struct SafeFaceHelper

struct SafeHypersurfaceHelper {

// Docstring regina::python::doc::detail::SafeHypersurfaceHelper::__class
static constexpr const char __class[] =
R"doc(Implementation details for SafeHypersurface<dim> and
SafeHypersurfaces<dim>. See those types for further information.)doc";

}; // struct SafeHypersurfaceHelper

struct SafeTriangulationHelper {

// Docstring regina::python::doc::detail::SafeTriangulationHelper::__class
static constexpr const char __class[] =
R"doc(Implementation details for SafeTriangulation<dim>. See
SafeTriangulation for further information.)doc";

}; // struct SafeTriangulationHelper

} // namespace detail

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

