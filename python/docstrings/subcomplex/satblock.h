/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SatBlock
static const char *SatBlock =
R"doc(Represents a saturated block in a Seifert fibred space. A saturated
block is a connected set of tetrahedra built from a subset of fibres
(no fibres may enter or exit the boundary of the block). In addition,
the boundary of this block must be a ring of saturated annuli, as
described by the SatAnnulus class. Aside from this ring of saturated
annuli, there may be no other boundary triangles within the block.

The boundary annuli are numbered consecutively as illustrated below,
where the markings 0 and 1 within the triangles represent the first
and second triangle of each annulus (see the SatAnnulus class notes
for details). Note that the following diagram is viewed from *inside*
the block.

```
              -+---+---+---+---+---+---+-
               |0 /|0 /|0 /|0 /|0 /|0 /|
           ... | / | / | / | / | / | / | ...
               |/ 1|/ 1|/ 1|/ 1|/ 1|/ 1|
              -+---+---+---+---+---+---+-
Annulus #  ...  n-2 n-1  0   1   2   3   ...
```

The ring of boundary annuli may optionally be twisted, so that
together the annuli form a long Mobius band. In this case, for the
purposes of labelling and marking annuli, the twist occurs between
annuli n-1 and 0. Be careful when dealing with blocks with twisted
boundaries, since with twists it is possible to identify an edge with
itself in reverse (thus producing something that is not a 3-manifold
triangulation).

Each saturated block corresponds to a piece of the base orbifold of
the larger Seifert fibred space. For the purpose of connecting the
base orbifold together, we assume that the boundary of this particular
piece runs horizontally in the diagram above (specifically following
the horizontal edges of the boundary annuli, as described in the
SatAnnulus class notes). Insisting on such a boundary may lead to
(1,*k*) twists within the block; these are accounted for by the
virtual adjustSFS() routine.

Saturated blocks are generally joined to one another (or themselves)
along their boundary annuli. For this purpose, each saturated block
contains a list of which annulus of this block is adjacent to which
annulus of which other block. Adjacencies may be *reflected*, meaning
that the adjacent annulus has its fibres reversed (i.e., the adjacent
annulus has undergone an up-to-down reflection); they may also be
*backwards*, meaning that the first triangle of one annulus is joined
to the second triangle of the other (and vice versa).

This is an abstract base class: its subclasses correspond to different
combinatorial constructions (or in some cases, parameterised families
of constructions). Each subclass of SatBlock:

* must override all pure virtual functions (of course);

* should override transform() if the subclass contains additional data
  that needs to be altered when an isomorphism is applied;

* should not override writeTextLong(), since SatBlock uses short
  output only.

SatBlock does not support value semantics: blocks cannot be copied,
swapped, or manually constructed. Their memory is managed by the
SatRegion class (or in special cases the SatBlockModel class), and
their locations in memory define them. See SatRegion for further
details.)doc";

// Docstring regina::python::doc::SatBlockModel
static const char *SatBlockModel =
R"doc(Contains an explicit triangulation of a saturated block along with the
accompanying saturated block description.

This class is designed to work with SatRegion::findStarterBlock(),
which uses such models as potential starting points for its search.
The ultimate aim here is to identify regions within triangulations
that are formed by joining saturated blocks together along their
boundary annuli.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that you cannot create generate your own models manually
(aside from copying or moving); instead you will need to use block-
specific factory routines such as SatTriPrism::model(),
SatCube::model(), and so on.)doc";

namespace SatBlockModel_ {

// Docstring regina::python::doc::SatBlockModel_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given model. This will induce a deep copy of
both the triangulation and the block structure.

Parameter ``src``:
    the model to copy.)doc";

// Docstring regina::python::doc::SatBlockModel_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object model saturated blocks of
the same type with the same combinatorial parameters.

This is equivalent to testing whether the blocks returned by block()
compare as equal. See SatBlock::operator==() for further details on
what this comparison means.

Assuming you created your models using the block-specific factory
routines (SatTriPrism::model(), SatCube::model(), etc.), if two models
compare as equal then their triangulations should be combinatorially
identical. At the time of writing, the converse is also true: all
models created from non-equal blocks yield non-identical (and moreover
non-isomorphic) triangulations.

Parameter ``other``:
    the model to compare with this.

Returns:
    ``True`` if and only if this and the given object model the same
    block type with the same combinatorial parameters.)doc";

// Docstring regina::python::doc::SatBlockModel_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object do not model saturated
blocks of the same type with the same combinatorial parameters.

This is equivalent to testing whether the blocks returned by block()
compare as non-equal. See SatBlock::operator==() for further details
on what this comparison means.

Assuming you created your models using the block-specific factory
routines (SatTriPrism::model(), SatCube::model(), etc.), if two models
compare as equal then their triangulations should be combinatorially
identical. At the time of writing, the converse is also true: all
models created from non-equal blocks yield non-identical (and moreover
non-isomorphic) triangulations.

Parameter ``other``:
    the model to compare with this.

Returns:
    ``True`` if and only if this and the given object do not model the
    same block type with the same combinatorial parameters.)doc";

// Docstring regina::python::doc::SatBlockModel_::block
static const char *block =
R"doc(Returns the structure of the saturated block.

Returns:
    the block structure.)doc";

// Docstring regina::python::doc::SatBlockModel_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given models.

This global routine simply calls SatBlockModel::swap(); it is provided
so that SatBlockModel meets the C++ Swappable requirements.

Parameter ``a``:
    the first model whose contents should be swapped.

Parameter ``b``:
    the second model whose contents should be swapped.)doc";

// Docstring regina::python::doc::SatBlockModel_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given model.

Parameter ``other``:
    the model whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SatBlockModel_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation of the saturated block.

Returns:
    the block triangulation.)doc";

}

namespace SatBlock_ {

// Docstring regina::python::doc::SatBlock_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent saturated
blocks of the same type with the same combinatorial parameters.

As examples of what is meant by "combinatorial parameters":

* Any two SatCube objects will compare as equal, since there is only
  one combinatorial type of SatCube.

* Two SatReflectorStrip objects will compare as equal if their rings
  of bounary annuli have the same length and are either both twisted
  or both untwisted.

* Two SatLST objects will compare as equal if their internal layered
  solid tori have the same three integer parameters (identifying how
  the meridinal disc meets the three boundary edges), *and* their
  corresponding boundary edges are attached to the
  horizontal/vertical/diagonal edges of the boundary annulus in the
  same way.

Parameter ``other``:
    the saturated block to compare with this.

Returns:
    ``True`` if and only if this and the given object represent blocks
    of the same type with the same parameters.)doc";

// Docstring regina::python::doc::SatBlock_::__lt
static const char *__lt =
R"doc(Implements a consistent ordering of saturated blocks. This ordering is
purely aesthetic on the part of the author, and is subject to change
in future versions of Regina.

Parameter ``compare``:
    the saturated block with which this will be compared.

Returns:
    ``True`` if this block comes before the given block according to
    the ordering of saturated blocks, or ``False`` if either the
    blocks are identical or this block comes after the given block.)doc";

// Docstring regina::python::doc::SatBlock_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object do not represent
saturated blocks of the same type with the same combinatorial
parameters.

See the equality test operator==() for examples of what is meant by
"the same combinatorial parameters".

Parameter ``other``:
    the saturated block to compare with this.

Returns:
    ``True`` if and only if this and the given object do not represent
    blocks of the same type with the same parameters.)doc";

// Docstring regina::python::doc::SatBlock_::abbr
static const char *abbr =
R"doc(Returns an abbreviated name or symbol for this block. This name will
reflect the particular block type, but may not provide thorough
details.

The name will be no more than a handful of characters long, and will
not include a newline (or surrounding dollar signs in TeX mode).

Parameter ``tex``:
    ``True`` if the name should be formatted for TeX, or ``False`` if
    it should be in plain text format.

Returns:
    an abbreviated name for this block.)doc";

// Docstring regina::python::doc::SatBlock_::adjacentAnnulus
static const char *adjacentAnnulus =
R"doc(Returns which specific annulus of the adjacent block is listed as
being adjacent to the given boundary annulus of this block.

Precondition:
    The given annulus of this block actually has an adjacent block
    listed.

Parameter ``whichAnnulus``:
    indicates which boundary annulus of this block should be examined;
    this must be between 0 and countAnnuli()-1 inclusive.

Returns:
    the corresponding annulus number on the other block that is
    adjacent along this annulus.)doc";

// Docstring regina::python::doc::SatBlock_::adjacentBackwards
static const char *adjacentBackwards =
R"doc(Returns whether the adjacency along the given boundary annulus of this
block is backwards. See the class notes for a discussion of backwards
adjacencies.

Precondition:
    The given annulus of this block actually has an adjacent block
    listed.

Parameter ``whichAnnulus``:
    indicates which boundary annulus of this block should be examined;
    this must be between 0 and countAnnuli()-1 inclusive.

Returns:
    ``True`` if the corresponding adjacency is backwards, or ``False``
    if it is not.)doc";

// Docstring regina::python::doc::SatBlock_::adjacentBlock
static const char *adjacentBlock =
R"doc(Returns the saturated block listed as being adjacent to the given
boundary annulus of this block.

Parameter ``whichAnnulus``:
    indicates which boundary annulus of this block should be examined;
    this must be between 0 and countAnnuli()-1 inclusive.

Returns:
    the other block adjacent along this annulus, or ``None`` if there
    is no adjacent block listed.)doc";

// Docstring regina::python::doc::SatBlock_::adjacentReflected
static const char *adjacentReflected =
R"doc(Returns whether the adjacency along the given boundary annulus of this
block is reflected. See the class notes for a discussion of reflected
adjacencies.

Precondition:
    The given annulus of this block actually has an adjacent block
    listed.

Parameter ``whichAnnulus``:
    indicates which boundary annulus of this block should be examined;
    this must be between 0 and countAnnuli()-1 inclusive.

Returns:
    ``True`` if the corresponding adjacency is reflected, or ``False``
    if it is not.)doc";

// Docstring regina::python::doc::SatBlock_::adjustSFS
static const char *adjustSFS =
R"doc(Adjusts the given Seifert fibred space to insert the contents of this
saturated block. In particular, the space should be adjusted as though
an ordinary solid torus (base orbifold a disc, no twists or
exceptional fibres) had been replaced by this block. This description
does not make sense for blocks with twisted boundary; the twisted case
is discussed below.

If the argument *reflect* is ``True``, it should be assumed that this
saturated block is being reflected before being inserted into the
larger Seifert fibred space. That is, any twists or exceptional fibres
should be negated before being added.

Regarding the signs of exceptional fibres: Consider a saturated block
containing a solid torus whose meridinal curve runs *p* times
horizontally around the boundary in order through annuli 0,1,... and
follows the fibres *q* times from bottom to top (as depicted in the
diagram in the SatBlock class notes). Then this saturated block adds a
positive (*p*, *q*) fibre to the underlying Seifert fibred space.

If the ring of saturated annuli bounding this block is twisted then
the situation becomes more complex. It can be proven that such a block
must contain a twisted reflector boundary in the base orbifold (use
Z_2 homology with fibre-reversing paths to show that the base orbifold
must contain another twisted boundary component, and then recall that
real boundaries are not allowed inside blocks).

In this twisted boundary case, it should be assumed that the twisted
reflector boundary is already stored in the given Seifert fibred
space. This routine should make any further changes that are required
(there may well be none). That is, the space should be adjusted as
though a trivial Seifert fibred space over the annulus with one
twisted reflector boundary (and one twisted puncture corresponding to
the block boundary) had been replaced by this block. In particular,
this routine should *not* add the reflector boundary itself.

Parameter ``sfs``:
    the Seifert fibred space to adjust.

Parameter ``reflect``:
    ``True`` if this block is to be reflected, or ``False`` if it
    should be inserted directly.)doc";

// Docstring regina::python::doc::SatBlock_::annulus
static const char *annulus =
R"doc(Returns details of the requested annulus on the boundary of this
saturated block. Annuli are numbered from 0 to countAnnuli()-1 as
described in the class notes.

Parameter ``which``:
    indicates which boundary annulus is requested; this must be
    between 0 and countAnnuli()-1 inclusive.

Returns:
    a reference to the requested boundary annulus.)doc";

// Docstring regina::python::doc::SatBlock_::countAnnuli
static const char *countAnnuli =
R"doc(Returns the number of annuli on the boundary of this saturated block.

Returns:
    the number of boundary annuli.)doc";

// Docstring regina::python::doc::SatBlock_::hasAdjacentBlock
static const char *hasAdjacentBlock =
R"doc(Returns whether there is another saturated block listed as being
adjacent to the given boundary annulus of this block.

Parameter ``whichAnnulus``:
    indicates which boundary annulus of this block should be examined;
    this must be between 0 and countAnnuli()-1 inclusive.

Returns:
    ``True`` if the given boundary annulus has an adjacent block
    listed, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::SatBlock_::nextBoundaryAnnulus
static const char *nextBoundaryAnnulus =
R"doc(Finds the next (or previous) boundary annulus around from this,
treating all adjacent blocks as part of a single large saturated
region.

Suppose that all saturated blocks are merged together according to
adjacent boundary annuli, forming larger saturated structures. The
remaining annuli that do not have adjacent blocks will group together
to form several large boundary rings. Note that each boundary ring
might involve annuli from several different blocks, and might or might
not have a twist (thus forming a large Klein bottle instead of a large
torus).

This routine is used to trace around such a boundary ring. It is
assumed that annulus *thisAnnulus* of this block forms part of a
boundary ring (i.e., it has no adjacent block). This routine will then
return the next/previous annulus around from this in the large
boundary ring. Here "next" means in the direction following from the
second triangle of this annulus, and "previous" means in the direction
following from the first triangle; the boolean argument *followPrev*
controls which we will be used. This next/previous annulus might
belong to another block, or it might even be this original annulus
again.

The next/previous annulus itself is not returned, but rather a
reference as to how it appears within its enclosing saturated block.
Specifically, a block and corresponding annulus number will be
included as the first two elements of the returned tuple.

It is possible that the next/previous annulus as it appears within the
returned block is oriented differently from how it appears within this
large boundary ring. For this reason, two booleans are returned also.
The third element of the returned tuple will describe whether the
annulus is reflected vertically as it appears within the large
boundary ring (i.e., the first and second triangles remain the same
but the fibre direction is reversed). Similarly, the fourth element of
the tuple will describe whether the annulus is reflected horizontally
as it appears within the large boundary ring (i.e., first and second
triangles are switched but the fibre direction is unchanged).

It is possible that both a horizontal and vertical reflection take
place. Note that any kind of reflection will also affect the locations
of the 0/1/2 markings as described in the SatAnnulus class notes.

Finally, note that if the large boundary ring is twisted (i.e., it
forms a Klein bottle), then following the entire boundary ring around
using this routine will bring you back to the starting annulus but
with the vertical reflection flag set.

Precondition:
    Annulus *thisAnnulus* of this block has no block adjacent to it.

.. warning::
    If you wish to trace around an entire boundary ring, you will need
    to adjust the argument *followPrev* according to whether or not
    the current annulus is reflected horizontally (since, under a
    horizontal reflection, "next" becomes "previous" and vice versa).

Parameter ``thisAnnulus``:
    describes which original boundary annulus of this block to
    examine; this must be between 0 and countAnnuli()-1 inclusive.

Parameter ``followPrev``:
    ``True`` if we should find the previous boundary annulus, or
    ``False`` if we should find the next boundary annulus.

Returns:
    a tuple (*nextBlock*, *nextAnnulus*, *refVert*, *refHoriz*),
    where: *nextBlock* is the block containing the next boundary
    annulus around from *thisAnnulus*; *nextAnnulus* is the specific
    annulus number within *nextBlock* of the next annulus around
    (between 0 and *nextBlock*->countAnnuli()-1 inclusive, and the
    corresponding annulus will have no block adjacent to it);
    *refVert* is ``True`` iff the next annulus around is vertically
    reflected; and *refHoriz* is ``True`` iff the next annulus around
    is horizontally reflected (see above for details on reflections).)doc";

// Docstring regina::python::doc::SatBlock_::twistedBoundary
static const char *twistedBoundary =
R"doc(Is the ring of boundary annuli twisted to form a long Mobius strip?

Recall from the class notes that the twist occurs between boundary
annuli countAnnuli()-1 and 0.

Returns:
    ``True`` if the ring of boundary annuli is twisted, or ``False``
    if not.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

