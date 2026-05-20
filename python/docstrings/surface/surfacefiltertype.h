/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct SurfaceFilterType {

// Docstring regina::python::doc::SurfaceFilterType::Combination
static constexpr const char Combination[] =
R"doc(Represents the SurfaceFilterCombination subclass: a filter that
combines other filters using boolean AND or OR.)doc";

// Docstring regina::python::doc::SurfaceFilterType::LegacyDefault
static constexpr const char LegacyDefault[] =
R"doc(A legacy constant representing a do-nothing filter that accepts any
normal surface.

This type of filter could technically appear in a second-generation
Regina data file, though it is unlikely that this feature was ever
used in practice (in particular, filters of this type could not be
created through the GUI).)doc";

// Docstring regina::python::doc::SurfaceFilterType::Properties
static constexpr const char Properties[] =
R"doc(Represents the SurfaceFilterProperties subclass: a filter that
examines simple properties of a normal surface.)doc";

// Docstring regina::python::doc::SurfaceFilterType::__class
static constexpr const char __class[] =
R"doc(Represents different types of filter classes that can be used to
filter lists of normal surfaces in 3-manifold triangulations.)doc";

}; // struct SurfaceFilterType

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

