/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SatAnnulus
static const char *SatAnnulus =
R"doc(Represents an annulus formed from a pair of triangles in a Seifert
fibred space. This annulus is saturated, i.e., a union of fibres. More
than that, the fibres run parallel to the two boundary edges of the
annulus.

The annulus is described from one side only. The description includes
an array of indices *tet*[] describing which two tetrahedra provide
the triangles of the annulus, as well as an array of permutations
*roles*[] detailing how the annulus matches up with the individual
tetrahedron vertices.

The annulus can be drawn as follows, with the upper edge identified
with the lower:

```
           *--->---*
           |0  2 / |
   First   |    / 1|  Second
  triangle |   /   | triangle
           |1 /    |
           | / 2  0|
           *--->---*
```

Suppose that *tet*[0] and *tet*[1] are the tetrahedra providing the
first and second triangles respectively. Then the markings 0..2 on the
first triangle above correspond to vertices *roles*[0][0..2] of
tetrahedron *tet*[0], and likewise the markings 0..2 on the second
triangle above correspond to vertices *roles*[1][0..2] of tetrahedron
*tet*[1].

Note that the diagram above can also be drawn as follows.

```
           *--->---*
           | \ 2  1|
   First   |0 \    |  Second
  triangle |   \   | triangle
           |    \ 0|
           |1  2 \ |
           *--->---*
```

Note also that the labelling of the tetrahedra and their vertices
establishes an orientation on the vertical fibres, as well as a left-
to-right direction across the annulus.

For convenience we refer to edges *roles*[][0-1] as _vertical_, edges
*roles*[][0-2] as _horizontal_, and edge *roles*[][1-2] as _diagonal_.
This is illustrated in the following diagrams.

```
        V  Horizontal       V   Diagonal
        e  *--->---*        e  *--->---*
        r  |   g / |        r  |H\ 2  1|
        t  |  a / 1|        t  | o\    |
        i  | i /   |        i  |  r\   |
        c  |D /    |        c  |   i\ 0|
        a  | / 2  0|        a  |    z\ |
        l  *--->---*        l  *--->---*
```

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    The member arrays *tet* and *roles* are accessed for reading
    through functions *tet*() and *roles*() respectively. For
    instance, the first triangle tetrahedron for the saturated annulus
    *a* can be accessed as ``a.tet(0)``. These same member arrays are
    accessed for writing through functions *setTet*() and
    *setRoles*(), so for instance the second triangle vertex roles for
    the saturated annulus *a* can be modified by calling
    ``a.setRoles(1, newRoles)``.)doc";

namespace SatAnnulus_ {

// Docstring regina::python::doc::SatAnnulus_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::SatAnnulus_::__default
static const char *__default =
R"doc(Creates a new uninitialised structure. Both tetrahedra will be set to
null pointers.)doc";

// Docstring regina::python::doc::SatAnnulus_::__eq
static const char *__eq =
R"doc(Determines whether or not this and the given structure describe the
same specific presentation of a saturated annulus.

Specifically, in order to compare as equal, two saturated annuli must
use the same two numbered tetrahedra, presented in the same order, and
with the same *roles* permutations.

Because this operation compares tetrahedron _numbers_ and not the
underlying Tetrahedron objects, it is meaningful to compare saturated
annuli from different triangulations.

Parameter ``other``:
    the structure to compare with this.

Returns:
    ``True`` if and only if both structures describe the same specific
    presentation of a saturated annulus.)doc";

// Docstring regina::python::doc::SatAnnulus_::__init
static const char *__init =
R"doc(Creates a new structure initialised to the given values. See the class
notes for what the various tetrahedra and permutations mean.

Parameter ``t0``:
    the tetrahedron to assign to *tet*[0].

Parameter ``r0``:
    the permutation to assign to *roles*[0].

Parameter ``t1``:
    the tetrahedron to assign to *tet*[1].

Parameter ``r1``:
    the permutation to assign to *roles*[1].)doc";

// Docstring regina::python::doc::SatAnnulus_::__ne
static const char *__ne =
R"doc(Determines whether or not this and the given structure describe
different specific presentations of a saturated annulus.

Specifically, in order to compare as equal, two saturated annuli must
use the same two numbered tetrahedra, presented in the same order, and
with the same *roles* permutations.

Because this operation compares tetrahedron _numbers_ and not the
underlying Tetrahedron objects, it is meaningful to compare saturated
annuli from different triangulations.

Parameter ``other``:
    the structure to compare with this.

Returns:
    ``True`` if and only if both structures describe different
    specific presentations of a saturated annulus.)doc";

// Docstring regina::python::doc::SatAnnulus_::attachLST
static const char *attachLST =
R"doc(Attaches a layered solid torus to the given saturated annulus. Instead
of passing a SatAnnulus (which only offers const access to the
underlying triangluation), you must pass the individual tetrahedra and
permutations that describe it.

The layered solid torus will be attached so that the given values
*alpha* and *beta* describe how the meridinal disc cuts the vertical
and horizontal edges of the annulus respectively.

The result will effectively insert an (*alpha*, *beta*) exceptional
fibre into the Seifert fibred space space, where the vertical edges
run parallel to the fibres and the horizontal edges represent the base
orbifold. The sign of the fibre is consistent with the fibre inserted
by SatLST::adjustSFS() (in particular, negating *beta* will negate the
fibre).

In the case of a (2,1) fibre, the layered solid torus will be
degenerate (i.e., the two triangles of the annulus will simply be
joined together).

Precondition:
    The given value *alpha* is not zero.

Precondition:
    The given values *alpha* and *beta* are coprime.

Precondition:
    The two faces of the given saturated annulus (i.e., face ``r0[3]``
    of *t0* and face ``r1[3]`` of *t1*) are boundary faces.

Exception ``InvalidArgument``:
    At least one of the conditions above fails; that is, either
    *alpha* is zero, or *alpha* and *beta* are not coprime, or at
    least one of the two faces of the saturated annulus is already
    joined to something. Note that the operation may already be
    partially complete by the time the exception is thrown (i.e., the
    layered solid torus might be partially constructed and/or might be
    already attached to one face of the annulus that was indeed
    boundary).

Exception ``LockViolation``:
    At least one face of the given saturated annulus (i.e., face
    ``r0[3]`` of *t0* and/or face ``r1[3]`` of *t1*) is locked. See
    Simplex<3>::lockFacet() for further details on how facet locks
    work and what their implications are. Note that the operation may
    already be partially complete by the time the exception is thrown
    (i.e., the layered solid torus might be already constructed and/or
    might be attached to one face of the annulus that was indeed
    unlocked).

Parameter ``t0``:
    the tetrahedron corresponding to SatAnnulus::tet[0].

Parameter ``r0``:
    the permutation corresponding to SatAnnulus::roles[0].

Parameter ``t1``:
    the tetrahedron corresponding to SatAnnulus::tet[1].

Parameter ``r1``:
    the permutation corresponding to SatAnnulus::roles[1].

Parameter ``alpha``:
    describes how the meridinal disc of the torus should cut the
    vertical edges. This may be positive or negative.

Parameter ``beta``:
    describes how the meridinal disc of the torus should cut the
    horizontal edges. Again this may be positive or negative.)doc";

// Docstring regina::python::doc::SatAnnulus_::halfTurnRotation
static const char *halfTurnRotation =
R"doc(Returns a 180 degree rotated representation of this annulus. This
structure will not be changed. See rotateHalfTurn() for further
details.

Returns:
    a new 180 degree rotation of this annulus.)doc";

// Docstring regina::python::doc::SatAnnulus_::horizontalReflection
static const char *horizontalReflection =
R"doc(Returns a left-to-right reflected representation of this annulus. This
structure will not be changed. See reflectHorizontal() for further
details.

Returns:
    a new left-to-right reflection of this annulus.)doc";

// Docstring regina::python::doc::SatAnnulus_::image
static const char *image =
R"doc(Returns the image of this annulus representation under the given
isomorphism between triangulations. This annulus representation will
not be changed. See transform() for further details.

Parameter ``originalTri``:
    the triangulation currently used by this annulus representation.

Parameter ``iso``:
    the mapping from *originalTri* to *newTri*.

Parameter ``newTri``:
    the triangulation to be used by the new annulus representation.)doc";

// Docstring regina::python::doc::SatAnnulus_::isAdjacent
static const char *isAdjacent =
R"doc(Determines whether this and the given annulus are adjacent, possibly
modulo vertical or horizontal reflections. That is, this routine
determines whether this and the given structure represent opposite
sides of the same saturated annulus, where the fibres for both
structures are consistent (though possibly reversed). See
switchSides() for details on what "opposite sides" means in this
context, and see reflectVertical() and reflectHorizontal() for
descriptions of the various types of reflection.

Information regarding reflections is returned via the second and third
elements of the returned tuple (call these *refVert* and *refHoriz*).
If the two annuli are identically opposite each other as described by
switchSides(), both booleans will be ``False``. If the two annuli are
identically opposite after one undergoes a vertical and/or horizontal
reflection, then the booleans *refVert* and/or *refHoriz* will be
``True`` accordingly.

The critical difference between this routine and isJoined() is that
this routine insists that the fibres on each annulus be consistent.
This routine is thus suitable for examining joins between different
sections of the same Seifert fibred space, for example.

Parameter ``other``:
    the annulus to compare with this.

Returns:
    a tuple of booleans (*adj*, *refVert*, *refHoriz*), where: *adj*
    is ``True`` iff some adjacency was found (either with or without
    reflections); *refVert* is ``True`` iff a vertical reflection is
    required; and *refHoriz* is ``True`` iff a horizontal reflection
    is required. If no adjacency was found at all, then both *refVert*
    and *refHoriz* will be ``False``.)doc";

// Docstring regina::python::doc::SatAnnulus_::isJoined
static const char *isJoined =
R"doc(Determines whether this and the given annulus are joined in some form,
even if the fibres on each annulus are not consistent.

This routine treats each annulus as though its boundaries are
identified to form a torus (though it does not actually test whether
this is true). It then examines whether this and the given annulus
represent opposite sides of the same torus. More specifically, it
tests whether both annuli are formed from the same pair of triangles,
and whether the mapping of 0/1/2 markings from one annulus to the
other is the same for each triangle. Note that the triangles are
allowed to be switched (i.e., the first triangle of one annulus may be
the second triangle of the other).

The critical difference between this routine and isAdjacent() is that
this routine allows the fibres on each annulus to be inconsistent.
This routine is thus suitable for examining joins between different
Seifert fibred blocks in a graph manifold, for example.

If the two annuli are joined, the precise relationship between the
curves on each annulus will be returned in the matrix *matching*.
Specifically, let *x* and *y* be the oriented curves running from
markings 0-1 and 0-2 respectively on the first triangle of this
annulus. Likewise, let *x*' and *y*' run from markings 0-1 and 0-2
respectively on the first triangle of the annulus *other*. Then the
joining between the two annuli can be expressed as follows:

```
    [x ]                [x']
    [  ]  =  matching * [  ].
    [y ]                [y']
```

Parameter ``other``:
    the annulus to compare with this.

Parameter ``matching``:
    returns details on how the curves on each annulus are related. If
    the this and the given annulus are not joined, then this matrix is
    not touched.

Returns:
    ``True`` if this and the given annulus are found to be joined, or
    ``False`` if they are not.)doc";

// Docstring regina::python::doc::SatAnnulus_::isTwoSidedTorus
static const char *isTwoSidedTorus =
R"doc(Determines whether this annulus has its boundaries identified to form
an embedded two-sided torus within the surrounding triangulation.

It will be verified that:

* the two triangles of this annulus are joined along all three pairs
  of edges to form a torus;

* the three edges of this torus remain distinct (i.e., different edges
  of the torus do not become identified within the larger
  triangulation);

* this torus is two-sided within the surrounding triangulation.

Returns:
    ``True`` if this annulus forms an embedded two-sided torus as
    described above, or ``False`` if it does not.)doc";

// Docstring regina::python::doc::SatAnnulus_::meetsBoundary
static const char *meetsBoundary =
R"doc(Determines how many triangles of this annulus lie on the boundary of
the triangulation.

Note that this routine can also be used as a boolean function to
determine whether any triangles of the annulus lie on the
triangulation boundary.

Returns:
    the number of triangles of this annulus that lie on the boundary
    of the triangulation; this will be 0, 1 or 2.)doc";

// Docstring regina::python::doc::SatAnnulus_::otherSide
static const char *otherSide =
R"doc(Returns a representation of the same annulus from the other side. This
structure will not be changed. See switchSides() for further details.

Precondition:
    Neither triangle of this annulus is a boundary triangle of the
    triangulation.

Returns:
    a new representation of this annulus from the other side.)doc";

// Docstring regina::python::doc::SatAnnulus_::reflectHorizontal
static const char *reflectHorizontal =
R"doc(Performs a left-to-right reflection of this annulus representation.
The vertical direction of the fibres will remain unchanged, but the
first and second triangles will be switched (and the 0..2 markings
changed to compensate).)doc";

// Docstring regina::python::doc::SatAnnulus_::reflectVertical
static const char *reflectVertical =
R"doc(Reverses the direction of the vertical fibres in this annulus
representation. The first and second triangles (as described in the
class notes) will remain unchanged, but the markings 0 and 1 on each
triangle will be switched.)doc";

// Docstring regina::python::doc::SatAnnulus_::roles
static const char *roles =
R"doc(A Python-only routine that allows you to query the *roles* field.

The *roles* field describes how the two triangles that make up this
match up with the individual vertices of their corresponding
tetrahedra. See the class notes for details.

Parameter ``which``:
    identifies whether we are querying information for the first or
    second triangle of this annulus. This argument must be 0 or 1
    respectively.

Returns:
    the permutation that describes how the given triangle matches up
    with the individual vertices of its corresponding tetrahedron.)doc";

// Docstring regina::python::doc::SatAnnulus_::rotateHalfTurn
static const char *rotateHalfTurn =
R"doc(Rotates the representation of this annulus by 180 degrees. This has
the effect of switching the first and second triangles and also
reversing the direction of the vertical fibres.

Calling this routine is equivalent to calling reflectVertical() and
then reflectHorizontal().)doc";

// Docstring regina::python::doc::SatAnnulus_::setRoles
static const char *setRoles =
R"doc(A Python-only routine that allows you to set the *roles* field.

The *roles* field describes how the two triangles that make up this
match up with the individual vertices of their corresponding
tetrahedra. See the class notes for details.

Parameter ``which``:
    identifies whether we are setting information for the first or
    second triangle of this annulus. This argument must be 0 or 1
    respectively.

Parameter ``value``:
    the permutation that describes how the given triangle matches up
    with the individual vertices of its corresponding tetrahedron.)doc";

// Docstring regina::python::doc::SatAnnulus_::setTet
static const char *setTet =
R"doc(A Python-only routine that allows you to set the *tet* field.

The *tet* field describes which two tetrahedra provide the two
triangles that make up this annulus. See the class notes for details.

Parameter ``which``:
    identifies whether we are setting information for the first or
    second triangle of this annulus. This argument must be 0 or 1
    respectively.

Parameter ``value``:
    the tetrahedron that provides the given triangle.)doc";

// Docstring regina::python::doc::SatAnnulus_::switchSides
static const char *switchSides =
R"doc(Converts this into a representation of the same annulus from the other
side. The first and second triangles and their 0..2 markings (as
described in the class notes) remain unchanged. However, the two
tetrahedra that are used to describe the annulus will be replaced by
their counterparts on the other side of the annulus (i.e., the two new
tetrahedra that meet the two original tetrahedra along the annulus
itself).

Precondition:
    Neither triangle of this annulus is a boundary triangle of the
    triangulation.)doc";

// Docstring regina::python::doc::SatAnnulus_::tet
static const char *tet =
R"doc(A Python-only routine that allows you to query the *tet* field.

The *tet* field describes which two tetrahedra provide the two
triangles that make up this annulus. See the class notes for details.

Parameter ``which``:
    identifies whether we are querying information for the first or
    second triangle of this annulus. This argument must be 0 or 1
    respectively.

Returns:
    the tetrahedron that provides the given triangle.)doc";

// Docstring regina::python::doc::SatAnnulus_::transform
static const char *transform =
R"doc(Adjusts this annulus representation according to the given isomorphism
between triangulations.

The given isomorphism must describe a mapping from *originalTri* to
*newTri*, and this annulus must refer to tetrahedra in *originalTri*.
This routine will adjust this annulus according to the given
isomorphism, so that it refers to the corresponding tetrahedra in
*newTri* (with the *roles* permutations also updated accordingly).

Precondition:
    This annulus refers to tetrahedra in *originalTri*, and *iso*
    describes a mapping from *originalTri* to *newTri*.

Parameter ``originalTri``:
    the triangulation currently used by this annulus representation.

Parameter ``iso``:
    the mapping from *originalTri* to *newTri*.

Parameter ``newTri``:
    the triangulation to be used by the updated annulus
    representation.)doc";

// Docstring regina::python::doc::SatAnnulus_::verticalReflection
static const char *verticalReflection =
R"doc(Returns a representation of this annulus in which the vertical
direction of the fibres has been reversed. This structure will not be
changed. See reflectVertical() for further details.

Returns:
    a new representation of this annulus in which fibres have been
    reversed.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

