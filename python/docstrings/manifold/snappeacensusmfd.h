/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnapPeaCensusManifold
static const char *SnapPeaCensusManifold =
R"doc(Represents a 3-manifold from the SnapPea cusped census.

The SnapPea cusped census is the census of cusped hyperbolic
3-manifolds formed from up to seven tetrahedra. This census was
tabulated by Callahan, Hildebrand and Weeks, and is shipped with
SnapPea 3.0d3 (and also with Regina).

.. note::
    The modern cusped hyperbolic census now extends to nine
    tetrahedra, and indeed the 9-tetrahedron database is accessible
    through the Census lookup routines. However, for the time being,
    the scope of these SnapPeaCensusManifold and SnapPeaCensusTri
    classes is restricted to the original Callahan-Hildebrand-Weeks
    7-tetrahedron census only.

The census is split into five different sections according to number
of tetrahedra and orientability. Each of these sections corresponds to
one of the section constants defined in this class.

For further details regarding the SnapPea census, see "A census of
cusped hyperbolic 3-manifolds", Patrick J. Callahan, Martin V.
Hildebrand and Jeffrey R. Weeks, Math. Comp. 68 (1999), no. 225, pp.
321--332.

Note that this class is closely tied to SnapPeaCensusTri. In
particular, the section constants defined in SnapPeaCensusTri and
SnapPeaCensusManifold are identical, and so may be freely mixed.
Furthermore, the section and index parameters of a SnapPeaCensusTri
are identical to those of its corresponding SnapPeaCensusManifold.

All of the optional Manifold routines are implemented for this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
manifold classes.)doc";

namespace SnapPeaCensusManifold_ {

// Docstring regina::python::doc::SnapPeaCensusManifold_::__copy
static const char *__copy = R"doc(Creates a new copy of the given SnapPea census manifold.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
3-manifold from the SnapPea census.

As of Regina 5.0, this test respects the recent discovery that the
manifolds ``x101`` and ``x103`` are homeomorphic. For details, see
B.B., _A duplicate pair in the SnapPea census_, Experimental
Mathematics, 23:170-173, 2014.

Parameter ``compare``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same SnapPea census manifold.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::__init
static const char *__init =
R"doc(Creates a new SnapPea census manifold with the given parameters.

Parameter ``section``:
    the section of the SnapPea census to which this manifold belongs.
    This must be one of the section constants defined in this class.

Parameter ``index``:
    specifies which particular manifold within the given section is
    represented. The indices for each section begin counting at zero,
    and so this index must be between 0 and *k*-1, where *k* is the
    total number of manifolds in the given section.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given SnapPea census manifolds.

This global routine simply calls SnapPeaCensusManifold::swap(); it is
provided so that SnapPeaCensusManifold meets the C++ Swappable
requirements.

Parameter ``a``:
    the first census manifold whose contents should be swapped.

Parameter ``b``:
    the second census manifold whose contents should be swapped.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::index
static const char *index =
R"doc(Returns the index of this manifold within its particular section of
the SnapPea census. Note that indices for each section begin counting
at zero.

Returns:
    the index of this manifold within its section.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::section
static const char *section =
R"doc(Returns the section of the SnapPea census to which this manifold
belongs. This will be one of the section constants defined in this
class.

Returns:
    the section of the SnapPea census.)doc";

// Docstring regina::python::doc::SnapPeaCensusManifold_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given SnapPea census manifold.

Parameter ``other``:
    the census manifold whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

