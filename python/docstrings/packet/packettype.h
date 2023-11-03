/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PacketInfo
static const char *PacketInfo =
R"doc(A class used to query general information about different packet
types.

This class has become much simpler since Regina 7.0. It is no longer
templated, and instead just offers a name() function that is
accessible at both compile time and runtime.)doc";

// Docstring regina::python::doc::PacketType
static const char *PacketType = R"doc(Represents the different types of packet that are available in Regina.)doc";

namespace PacketInfo_ {

// Docstring regina::python::doc::PacketInfo_::name
static const char *name =
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

}

namespace PacketType_ {

// Docstring regina::python::doc::PacketType_::AngleStructures
static const char *AngleStructures = R"doc(Represents an angle structure list, of class AngleStructures.)doc";

// Docstring regina::python::doc::PacketType_::Attachment
static const char *Attachment = R"doc(Represents an arbitrary file attachment, of class Attachment.)doc";

// Docstring regina::python::doc::PacketType_::Container
static const char *Container = R"doc(Represents a container packet, of class Container.)doc";

// Docstring regina::python::doc::PacketType_::Link
static const char *Link = R"doc(Represents a knot or link in the 3-sphere, of class Link.)doc";

// Docstring regina::python::doc::PacketType_::None
static const char *None =
R"doc(Does not represent any of Regina's packet types.

This can be used (for example) for initialising a PacketType variable.)doc";

// Docstring regina::python::doc::PacketType_::NormalHypersurfaces
static const char *NormalHypersurfaces = R"doc(Represents a normal hypersurface list, of class NormalHypersurfaces.)doc";

// Docstring regina::python::doc::PacketType_::NormalSurfaces
static const char *NormalSurfaces = R"doc(Represents a normal surface list, of class NormalSurfaces.)doc";

// Docstring regina::python::doc::PacketType_::Script
static const char *Script = R"doc(Represents a script packet, of class Script.)doc";

// Docstring regina::python::doc::PacketType_::SnapPea
static const char *SnapPea =
R"doc(Represents a triangulation in the embedded SnapPea kernel, of class
SnapPeaTriangulation.)doc";

// Docstring regina::python::doc::PacketType_::SurfaceFilter
static const char *SurfaceFilter =
R"doc(Represents a normal surface filter, of class SurfaceFilter or one of
its descendant classes.)doc";

// Docstring regina::python::doc::PacketType_::Text
static const char *Text = R"doc(Represents a text packet, of class Text.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation2
static const char *Triangulation2 = R"doc(Represents a 2-dimensional triangulation, of class Triangulation<2>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation3
static const char *Triangulation3 = R"doc(Represents a 3-dimensional triangulation, of class Triangulation<3>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation4
static const char *Triangulation4 = R"doc(Represents a 4-dimensional triangulation, of class Triangulation<4>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation5
static const char *Triangulation5 = R"doc(Represents a 5-dimensional triangulation, of class Triangulation<5>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation6
static const char *Triangulation6 = R"doc(Represents a 6-dimensional triangulation, of class Triangulation<6>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation7
static const char *Triangulation7 = R"doc(Represents a 7-dimensional triangulation, of class Triangulation<7>.)doc";

// Docstring regina::python::doc::PacketType_::Triangulation8
static const char *Triangulation8 = R"doc(Represents a 8-dimensional triangulation, of class Triangulation<8>.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

