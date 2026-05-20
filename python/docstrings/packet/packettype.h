/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::isTriangulation
inline constexpr const char isTriangulation[] =
R"doc(Identifies whether the given packet type represents a native Regina
triangulation, and if so, returns the dimension.

Note that this routine does _not_ recognise SnapPea triangulations:
calling ``isTriangulation(PacketType::SnapPea)`` will return 0.

This routine is intended to help with generic code, where one might
need to query a packet type at runtime and from this extract a
compile-time dimension constant for use as a template parameter. For
example:

```
if (int d = isTriangulation(packet.type())) {
     // Our packet holds a triangulation of dimension d.
     select_constexpr<2, regina::maxDim() + 1, void>(d, [&](auto dim) {
         // The runtime variable d is now a compile-time constant dim.
         do_something<dim>(packet);
     });
}
```

Parameter ``type``:
    a packet type constant.

Returns:
    the dimension of triangulation that *type* represents, or 0 if
    *type* does not represent a native Regina triangulation.)doc";

struct PacketInfo {

// Docstring regina::python::doc::PacketInfo::__class
static constexpr const char __class[] =
R"doc(A class used to query general information about different packet
types.

This class has become much simpler since Regina 7.0. It is no longer
templated, and instead just offers a name() function that is
accessible at both compile time and runtime.)doc";

// Docstring regina::python::doc::PacketInfo::name
static constexpr const char name[] =
R"doc(Returns a human-readable name for the given packet type.

The first letter of the returned string will be upper-case, and all
subsequent letters will be lower-case (except for special words such
as "PDF" and "SnapPea", which will retain their internal upper-case
letters where required).

This routine is guaranteed to return a non-null string, even if
*packetType* is not one of the PacketType enum values.

Parameter ``packetType``:
    the packet type being queried.

Returns:
    the name of the given packet type.)doc";

}; // struct PacketInfo

struct PacketType {

// Docstring regina::python::doc::PacketType::AngleStructures
static constexpr const char AngleStructures[] = R"doc(Represents an angle structure list, of class AngleStructures.)doc";

// Docstring regina::python::doc::PacketType::Attachment
static constexpr const char Attachment[] = R"doc(Represents an arbitrary file attachment, of class Attachment.)doc";

// Docstring regina::python::doc::PacketType::Container
static constexpr const char Container[] = R"doc(Represents a container packet, of class Container.)doc";

// Docstring regina::python::doc::PacketType::Link
static constexpr const char Link[] =
R"doc(Represents a combinatorial diagram of a knot or link in the 3-sphere,
of class Link.)doc";

// Docstring regina::python::doc::PacketType::None
static constexpr const char None[] =
R"doc(Does not represent any of Regina's packet types.

This can be used (for example) for initialising a PacketType variable.)doc";

// Docstring regina::python::doc::PacketType::NormalHypersurfaces
static constexpr const char NormalHypersurfaces[] = R"doc(Represents a normal hypersurface list, of class NormalHypersurfaces.)doc";

// Docstring regina::python::doc::PacketType::NormalSurfaces
static constexpr const char NormalSurfaces[] = R"doc(Represents a normal surface list, of class NormalSurfaces.)doc";

// Docstring regina::python::doc::PacketType::Script
static constexpr const char Script[] = R"doc(Represents a script packet, of class Script.)doc";

// Docstring regina::python::doc::PacketType::SnapPea
static constexpr const char SnapPea[] =
R"doc(Represents a triangulation in the embedded SnapPea kernel, of class
SnapPeaTriangulation.)doc";

// Docstring regina::python::doc::PacketType::SpatialLink
static constexpr const char SpatialLink[] =
R"doc(Represents a specific embedding of a knot or link in 3-dimensional
space, of class SpatialLink.)doc";

// Docstring regina::python::doc::PacketType::SurfaceFilter
static constexpr const char SurfaceFilter[] =
R"doc(Represents a normal surface filter, of class SurfaceFilter or one of
its descendant classes.)doc";

// Docstring regina::python::doc::PacketType::Text
static constexpr const char Text[] = R"doc(Represents a text packet, of class Text.)doc";

// Docstring regina::python::doc::PacketType::Triangulation2
static constexpr const char Triangulation2[] = R"doc(Represents a 2-dimensional triangulation, of class Triangulation<2>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation3
static constexpr const char Triangulation3[] = R"doc(Represents a 3-dimensional triangulation, of class Triangulation<3>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation4
static constexpr const char Triangulation4[] = R"doc(Represents a 4-dimensional triangulation, of class Triangulation<4>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation5
static constexpr const char Triangulation5[] = R"doc(Represents a 5-dimensional triangulation, of class Triangulation<5>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation6
static constexpr const char Triangulation6[] = R"doc(Represents a 6-dimensional triangulation, of class Triangulation<6>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation7
static constexpr const char Triangulation7[] = R"doc(Represents a 7-dimensional triangulation, of class Triangulation<7>.)doc";

// Docstring regina::python::doc::PacketType::Triangulation8
static constexpr const char Triangulation8[] = R"doc(Represents a 8-dimensional triangulation, of class Triangulation<8>.)doc";

// Docstring regina::python::doc::PacketType::__class
static constexpr const char __class[] = R"doc(Represents the different types of packet that are available in Regina.)doc";

}; // struct PacketType

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

