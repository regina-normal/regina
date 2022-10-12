/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SurfaceFilterType
static const char *SurfaceFilterType =
R"doc(Represents different types of filter classes that can be used to
filter lists of normal surfaces in 3-manifold triangulations.

IDs 0-9999 are reserved for future use by Regina. If you are extending
Regina to include your own filter class, you should choose an ID >=
10000.)doc";

namespace SurfaceFilterType_ {

// Docstring regina::python::doc::SurfaceFilterType_::NS_FILTER_COMBINATION
static const char *NS_FILTER_COMBINATION =
R"doc(Represents the SurfaceFilterCombination subclass: a filter that
combines other filters using boolean AND or OR.)doc";

// Docstring regina::python::doc::SurfaceFilterType_::NS_FILTER_LEGACY_DEFAULT
static const char *NS_FILTER_LEGACY_DEFAULT =
R"doc(A legacy constant representing a do-nothing filter that accepts any
normal surface.

This type of filter could technically appear in a second-generation
Regina data file, though it is unlikely that this feature was ever
used in practice (in particular, filters of this type could not be
created through the GUI).)doc";

// Docstring regina::python::doc::SurfaceFilterType_::NS_FILTER_PROPERTIES
static const char *NS_FILTER_PROPERTIES =
R"doc(Represents the SurfaceFilterProperties subclass: a filter that
examines simple properties of a normal surface.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

