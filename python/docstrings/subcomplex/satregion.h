/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SatBlockSpec
static const char *SatBlockSpec =
R"doc(Describes how a single saturated block forms a part of a larger
saturated region.

A saturated region consists of several saturated blocks joined
together along their boundary annuli. This is a helper class
containing a single saturated block along with details of its
orientation within a larger region.

The ring of saturated annuli around the boundary of the block gives a
natural orientation to the block within the context of the base
orbifold, just as the ring of edges around a polygon would give a
natural orientation to that polygon within the context of a
surrounding surface. Again drawing an analogy with the orientation of
polygons within a surface, each block can be considered to have a
correct or reflected orientation according to whether this ring of
annuli runs clockwise or anticlockwise in the base orbifold.

The precise orientation of a block is described using two booleans. A
block may be reflected _horizontally_, which preserves the directions
of Seifert fibres but which reverses the clockwise/anticlockwise
orientation as discussed above. A block may also be reflected
_vertically_, which preserves the clockwise/anticlockwise orientation
but which reverses the directions of the Seifert fibres. A block may
of course be reflected both horizontally and vertically, or it may not
be reflected at all.

Since Regina 7.0, this helper structure now owns its internal
SatBlock, and will destroy this SatBlock when this structure itself is
destroyed. This is a change in behaviour from Regina 6.0.1 and
earlier.

Because of these new ownership semantics, SatBlockSpec is no longer
copyable. However, it is both movable and swappable (it implements C++
moves semantics and adheres to the C++ Swappable requirement).)doc";

// Docstring regina::python::doc::SatRegion
static const char *SatRegion =
R"doc(A large saturated region in a Seifert fibred space formed by joining
together saturated blocks.

Like a saturated block (described in the class SatBlock), a saturated
region is a connected set of tetrahedra built from a subset of fibres.
Unlike a saturated block however, a saturated region has no
constraints on its boundary - it may have several boundary components
or it may have none. For instance, a saturated region might be an
entire closed Seifert fibred space, or it might describe a Seifert
fibred component of a JSJ decomposition.

A saturated region is formed from a collection of saturated blocks by
joining the boundary annuli of these blocks together in pairs. The
joins must be made so that the fibres are consistent, though it is
allowable to reverse the directions of the fibres. There is no problem
with joining two boundary annuli from the same block to each other.

Any boundary annulus of a block that is not joined to some other
boundary annulus of a block becomes a boundary annulus of the entire
region. In this way, each boundary component of the region (if there
are any at all) is formed from a ring of boundary annuli, in the same
way that the boundary of a block is. Note that the routine
SatBlock::nextBoundaryAnnulus() can be used to trace around a region
boundary. Like block boundaries, the boundary of a saturated region
need not be part of the boundary of the larger triangulation (i.e.,
there may be adjacent tetrahedra that are not recognised as part of
this saturated structure).

The SatRegion class stores a list of its constituent blocks, but it
does not directly store which block boundary annuli are joined to
which. This adjacency information is stored within the blocks
themselves; see the notes regarding adjacency in the SatBlock class
description.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that the only way to create objects of this class (aside from
copying or moving) is via the static search functions, such as find()
or beginsRegion().

.. warning::
    It is crucial that the adjacency information stored in the blocks
    is consistent with the region containing them. All this requires
    is that the blocks are not manipulated externally (e.g.,
    SatBlock::setAdjacent() is not called on any of the blocks), but
    instead all adjacency information is managed by this class.)doc";

namespace SatBlockSpec_ {

// Docstring regina::python::doc::SatBlockSpec_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure contain
combinatorially equivalent information.

Specifically, to compare as equal, two SatBlockSpec objects must hold
blocks of the same type with the same combinatorial parameters (as
tested by the SatBlock equality comparison), and they must use the
same horizontal/vertical reflection parameters within the larger
region (as returned by refVert() and refHoriz()).

Parameter ``other``:
    the structure to compare against this.

Returns:
    ``True`` if and only if this and *other* contain combinatorially
    equivalent information.)doc";

// Docstring regina::python::doc::SatBlockSpec_::block
static const char *block =
R"doc(Returns the full combinatorial structure of the saturated block.

Returns:
    the saturated block structure.)doc";

// Docstring regina::python::doc::SatBlockSpec_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

In particular, the internal SatBlock pointers will be swapped; that
is, the pointers ``a.block()`` and ``b.block()`` after the move will
be the same as ``b.block()`` and ``a.block()`` were respectively
before the move.

This global routine simply calls SatBlockSpec::swap(); it is provided
so that SatBlockSpec meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::SatBlockSpec_::refHoriz
static const char *refHoriz =
R"doc(Returns whether the block is reflected horizontally within the larger
region. See the class notes for details.

Returns:
    ``True`` if and only if the block is reflected horizontally.)doc";

// Docstring regina::python::doc::SatBlockSpec_::refVert
static const char *refVert =
R"doc(Returns whether the block is reflected vertically within the larger
region. See the class notes for details.

Returns:
    ``True`` if and only if the block is reflected vertically.)doc";

// Docstring regina::python::doc::SatBlockSpec_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

In particular, the internal SatBlock pointers will be swapped; that
is, the pointers ``this->block()`` and ``other.block()`` after the
move will be the same as ``other.block()`` and ``this->block()`` were
respectively before the move.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

namespace SatRegion_ {

// Docstring regina::python::doc::SatRegion_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given region.

Parameter ``src``:
    the region to copy.)doc";

// Docstring regina::python::doc::SatRegion_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
combinatorial presentation of a saturated region.

Specifically, in order to compare as equal, two saturated regions must
be formed from saturated blocks with the same combinatorial parameters
(as returned by the SatBlock comparison operators), and these blocks
must be presented in the same order with the same horizontal/vertical
reflections and joined together in the same way.

Like the comparison operators for most parameterised subclasses of
StandardTriangulation, it does not matter how the constituent
tetrahedra and/or their vertices are numbered. However, this test is
more specific than combinatorial isomorphism of the underlying
subcomplex of the triangulation, since it does not account for the
many symmetries in how the same saturated region can be presented.

Parameter ``other``:
    the saturated region to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same combinatorial presentation of a saturated region.)doc";

// Docstring regina::python::doc::SatRegion_::beginsRegion
static const char *beginsRegion =
R"doc(Determines whether the given annulus is in fact a boundary annulus for
a saturated region. The annulus should be represented from the inside
of the proposed saturated region.

All tetrahedra in the given list *avoidTets* will be ignored by this
routine, and so if a region is found then it is guaranteed not to
include any of them. As a consequence, if the given annulus uses any
of these tetrahedra then ``None`` will be returned.

If a region is found, it will be expanded as far as possible, and all
of the tetrahedra within it will be added to the list *avoidTets*.
Moreover, it is guaranteed that the given annulus will be listed as
annulus number 0 in the block that contains it, without any horizontal
or vertical reflection.

Precondition:
    Either the given annulus lies on the boundary of the
    triangulation, or else the (one or two) tetrahedra attached to the
    other side of it are already in the list *avoidTets*. This is
    necessary to ensure that the saturated region does not expand
    through the annulus to the other side.

Parameter ``annulus``:
    the proposed boundary annulus that should form part of the new
    saturated region.

Parameter ``avoidTets``:
    the list of tetrahedra that should not be considered, and to which
    any new tetrahedra will be added.

Returns:
    details of the saturated region if one was found, or ``None`` if
    none was found.)doc";

// Docstring regina::python::doc::SatRegion_::block
static const char *block =
R"doc(Returns details of the requested saturated block within this region.
The information will returned will include structural information for
the block, along with details of how the block is aligned (e.g.,
reflected vertically or horizontally) within the larger region.

Parameter ``which``:
    indicates which of the constituent blocks should be returned; this
    must be between 0 and countBlocks()-1 inclusive.

Returns:
    details of the requested saturated block.)doc";

// Docstring regina::python::doc::SatRegion_::blockAbbrs
static const char *blockAbbrs =
R"doc(Returns an abbreviated list of blocks within this region in string
format. The string that is returned will consist of the abbreviated
names of all blocks in this region, separated by commas and spaces.
See SatBlock::abbr() for further details.

The blocks within this region will be sorted before their abbreviated
names are output. The particular method of sorting is an arbitrary
aesthetic decision on the part of the author, and is subject to change
in future versions of Regina.

Parameter ``tex``:
    ``True`` if the output should be formatted for TeX, or ``False``
    if it should be written as plain text.

Returns:
    the abbreviated list of all blocks.)doc";

// Docstring regina::python::doc::SatRegion_::blockIndex
static const char *blockIndex =
R"doc(Returns the index of the given block within this region. This index
corresponds to the integer parameter that is passed to the routine
block().

.. warning::
    This routine is slow, since it simply scans through the blocks in
    this region one by one until the given block is found (or until
    all blocks are exhausted).

Returns:
    the index of the given block (as an integer between 0 and
    countBlocks()-1 inclusive), or -1 if the block is not part of this
    region.)doc";

// Docstring regina::python::doc::SatRegion_::boundaryAnnulus
static const char *boundaryAnnulus =
R"doc(Returns details of the requested saturated annulus on the boundary of
this region.

The saturated annuli that together form the boundary components of
this region are numbered from 0 to countBoundaryAnnuli()-1 inclusive.
The argument *which* specifies which one of these annuli should be
returned.

Currently the annuli are numbered lexicographically by block and then
by annulus number within the block, although this ordering is subject
to change in future versions of Regina. In particular, the annuli are
_not_ necessarily numbered in order around the region boundaries, and
each region boundary component might not even be given a consecutive
range of numbers.

It is guaranteed however that, if the starter block passed to the
SatRegion constructor provides any boundary annuli for the overall
region, then the first such annulus in the starter block will be
numbered 0 here.

Be aware that the block containing the requested annulus might be
reflected horizontally and/or vertically within the overall region, as
discussed in the SatBlockSpec class notes. This will affect how the
annulus is positioned as part of the overall region boundary (e.g.,
the annulus might be positioned upside-down in the overall region
boundary, or it might be positioned with its second triangle appearing
before its first triangle as one walks around the boundary). To
account for this, the return value will include two booleans *refVert*
and *refHoriz* to indicate if and how the block is reflected.

.. warning::
    This routine is quite slow, since it currently scans through every
    annulus of every saturated block. Use it sparingly!

Exception ``InvalidArgument``:
    The argument *which* was not between 0 and countBoundaryAnnuli()-1
    inclusive.

Parameter ``which``:
    specifies which boundary annulus of this region to return; this
    must be between 0 and countBoundaryAnnuli()-1 inclusive.

Returns:
    a tuple (*block*, annulus, refVert, refHoriz), where: *block* is a
    pointer to the the particular saturated block containing the
    requested annulus; *annulus* indicates which annulus number in the
    returned block is the requested annulus, as an integer between 0
    and block->countAnnuli()-1 inclusive; *refVert* is ``True`` if and
    only if the block containing the requested annulus is vertically
    reflected within this region; and *refHoriz* is ``True`` if and
    only if the block containing the requested annulus is horizontally
    reflected within this region. See SatBlockSpec for further details
    on these reflections.)doc";

// Docstring regina::python::doc::SatRegion_::countBlocks
static const char *countBlocks =
R"doc(Returns the number of saturated blocks that come together to form this
saturated region.

Returns:
    the total number of blocks.)doc";

// Docstring regina::python::doc::SatRegion_::countBoundaryAnnuli
static const char *countBoundaryAnnuli =
R"doc(Returns the number of saturated annuli that together form the boundary
components of this region.

Returns:
    the number of boundary annuli.)doc";

// Docstring regina::python::doc::SatRegion_::createSFS
static const char *createSFS =
R"doc(Returns details of the Seifert fibred space represented by this
region.

Each boundary component of this region will be formed from a ring of
saturated annuli, which together form a torus or a Klein bottle. For
torus boundary components, the oriented curves representing the fibres
and base orbifold on the boundary (see sfsnotation) will be as
follows.

* Consider the 0/1/2 markings on the first and second triangles of
  each saturated annulus, as described in the SatAnnulus class notes.

* The fibres are represented by the oriented edge joining markings 1
  and 0 on the first triangle (or 0 and 1 on the second triangle).
  This is reversed if the block containing the boundary annulus is
  vertically reflected.

* The curve representing the base orbifold run along the oriented edge
  joining markings 0 and 2 on the first triangle (or 2 and 0 on the
  second triangle). This is reversed if the block containing the
  boundary annulus is horizontally reflected.

* See the SatBlockSpec overview for descriptions of horizontal and
  vertical reflection.

If the argument *reflect* is ``True``, the Seifert fibred space will
be created as though the entire region had been reflected. In
particular, each twist or exceptional fibre will be negated before
being added to the Seifert structure.

For Klein bottle boundary components, these curves must (for now) be
analysed by hand.

There are situations in which Regina is not (yet) capable of
determining the Seifert fibred space precisely. This can only happen
if the Seifert fibred space is closed, non-orientable, belongs to the
class ``n3`` or ``n4``, and has sufficiently large base orbifold
genus. In such situations this routine will throw an exception.

Exception ``NotImplemented``:
    This is one of the closed non-orientable cases where Regina cannot
    distinguish between classes ``n3`` and ``n4``, as described above.

Parameter ``reflect``:
    ``True`` if this region is to be reflected as the Seifert fibred
    space is created, or ``False`` if not.

Returns:
    the structure of the underlying Seifert fibred space.)doc";

// Docstring regina::python::doc::SatRegion_::find
static const char *find =
R"doc(Runs a search for every isomorphic embedding of every starter block
from the global SatBlockStarterSet within the given triangulation.

Each time an embedding of a starter block is discovered, the block
will be wrapped in a new SatRegion which describes how the block
appears within the given triangulation. The region will be expanded to
encompass as many saturated blocks as possible, and then passed to
*action*, which must be a function or some other callable object.

* The first argument to *action* must be of type
  std::unique_ptr<SatRegion>; this will be the newly constructed and
  expanded region that contains the starter block that was found. As
  expected from std::unique_ptr, this function will have relinquished
  all ownership of the region, and your action can do what it likes
  with it.

* The second argument to *action* must be of type SatBlock::TetList&.
  This list will contain all tetrahedra currently used by the region,
  and *action* is welcome to modify the list as it pleases. This
  function will clear and reuse the list after *action* returns.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return a ``bool``. A return value of ``False``
  indicates that the search for starter blocks should continue, and a
  return value of ``True`` indicates that the search should terminate
  immediately.

Note that different embeddings of the same starter block within *tri*
will result in the action being called multiple times (with different
containing regions).

If you are searching for a region that fills an entire triangulation
component (i.e., every boundary annulus of the region in fact forms
part of the boundary of the triangulation), then you should pass
*mustBeComplete* as ``True``. If a region expansion does not fill the
entire component, then it will be discarded and *action* will not be
called for that particular embeddeding of that particular starter
block.

Python:
    This function is available, and *action* may be a pure Python
    function. However, *action* must take only one argument: the newly
    constructed SatRegion (i.e., the first argument in the description
    above). The SatBlock::TetList& argument will _not_ be passed, and
    there can be no additional argument list *args*.

Parameter ``tri``:
    the triangulation in which to search for starter blocks.

Parameter ``mustBeComplete``:
    ``True`` if you are searching for a region that fills an entire
    triangulation component, as described above.

Parameter ``action``:
    a function (or other callable object) to call for each embedding
    of a starter block that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial region and tetrahedron list arguments.

Returns:
    ``True`` if *action* ever terminated the search by returning
    ``True``, or ``False`` if the search was allowed to run to
    completion.)doc";

// Docstring regina::python::doc::SatRegion_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given regions.

This global routine simply calls SatRegion::swap(); it is provided so
that SatRegion meets the C++ Swappable requirements.

Parameter ``a``:
    the first region whose contents should be swapped.

Parameter ``b``:
    the second region whose contents should be swapped.)doc";

// Docstring regina::python::doc::SatRegion_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given region.

Parameter ``other``:
    the region whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

