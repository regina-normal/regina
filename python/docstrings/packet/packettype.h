/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
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
static const char *PacketType =
R"doc(Represents the different types of packet that are available in Regina.

IDs 0-9999 are reserved for future use by Regina. If you are extending
Regina to include your own packet type, you should choose an ID >=
10000.)doc";

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

// Docstring regina::python::doc::PacketType_::PACKET_ANGLESTRUCTURES
static const char *PACKET_ANGLESTRUCTURES = R"doc(Represents an angle structure list, of class AngleStructures.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_ATTACHMENT
static const char *PACKET_ATTACHMENT = R"doc(Represents an arbitrary file attachment, of class Attachment.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_CONTAINER
static const char *PACKET_CONTAINER = R"doc(Represents a container packet, of class Container.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_LINK
static const char *PACKET_LINK = R"doc(Represents a knot or link in the 3-sphere, of class Link.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_NONE
static const char *PACKET_NONE =
R"doc(Does not represent any of Regina's packet types.

This can be used (for example) for initialising a PacketType variable.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_NORMALHYPERSURFACES
static const char *PACKET_NORMALHYPERSURFACES = R"doc(Represents a normal hypersurface list, of class NormalHypersurfaces.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_NORMALSURFACES
static const char *PACKET_NORMALSURFACES = R"doc(Represents a normal surface list, of class NormalSurfaces.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_SCRIPT
static const char *PACKET_SCRIPT = R"doc(Represents a script packet, of class Script.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_SNAPPEATRIANGULATION
static const char *PACKET_SNAPPEATRIANGULATION =
R"doc(Represents a triangulation in the embedded SnapPea kernel, of class
SnapPeaTriangulation.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_SURFACEFILTER
static const char *PACKET_SURFACEFILTER =
R"doc(Represents a normal surface filter, of class SurfaceFilter or one of
its descendant classes.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TEXT
static const char *PACKET_TEXT = R"doc(Represents a text packet, of class Text.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION2
static const char *PACKET_TRIANGULATION2 = R"doc(Represents a 2-dimensional triangulation, of class Triangulation<2>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION3
static const char *PACKET_TRIANGULATION3 = R"doc(Represents a 3-dimensional triangulation, of class Triangulation<3>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION4
static const char *PACKET_TRIANGULATION4 = R"doc(Represents a 4-dimensional triangulation, of class Triangulation<4>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION5
static const char *PACKET_TRIANGULATION5 = R"doc(Represents a 5-dimensional triangulation, of class Triangulation<5>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION6
static const char *PACKET_TRIANGULATION6 = R"doc(Represents a 6-dimensional triangulation, of class Triangulation<6>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION7
static const char *PACKET_TRIANGULATION7 = R"doc(Represents a 7-dimensional triangulation, of class Triangulation<7>.)doc";

// Docstring regina::python::doc::PacketType_::PACKET_TRIANGULATION8
static const char *PACKET_TRIANGULATION8 = R"doc(Represents a 8-dimensional triangulation, of class Triangulation<8>.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

