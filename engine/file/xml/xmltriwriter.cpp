
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "packet/packet-impl.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"

namespace regina {

template <int dim>
void XMLWriter<Triangulation<dim>>::openPre() {
    if (format_ == FileFormat::XmlGen2) {
        out_ << R"(<packet type=")" << dim
            << R"(-Manifold Triangulation" typeid=")"
            << static_cast<int>(PacketOf<Triangulation<dim>>::typeID) << '"';
    } else {
        out_ << R"(<tri dim=")" << dim << R"(" size=")" << data_.size()
            << R"(" perm=")" << (useSnIndex ? "index" : "imagepack") << '"';
    }
}

template <int dim>
void XMLWriter<Triangulation<dim>>::writeContent() {
    // Write the simplex gluings.
    // We send permutation indices directly to the output stream.
    // This requires them to be numeric types (not character types).
    static_assert(! (
            std::is_same_v<typename Perm<dim + 1>::Index, char> ||
            std::is_same_v<typename Perm<dim + 1>::Index, unsigned char>
        ),
        "XMLWriter<Triangulation<dim>> requires permutation indices to be "
        "numeric types.");

    if (format_ == FileFormat::XmlGen2) {
        if constexpr (dim == 2)
            out_ << "  <triangles ntriangles=\"" << data_.size() << "\">\n";
        else if constexpr (dim == 3)
            out_ << "  <tetrahedra ntet=\"" << data_.size() << "\">\n";
        else if constexpr (dim == 4)
            out_ << "  <pentachora npent=\"" << data_.size() << "\">\n";
        else
            out_ << "  <simplices size=\"" << data_.size() << "\">\n";
        for (auto s : data_.simplices()) {
            if constexpr (dim == 2)
                out_ << "    <triangle desc=\"";
            else if constexpr (dim == 3)
                out_ << "    <tet desc=\"";
            else if constexpr (dim == 4)
                out_ << "    <pent desc=\"";
            else
                out_ << "    <simplex desc=\"";
            out_ << regina::xml::xmlEncodeSpecialChars(s->description())
                << "\"> ";
            for (int facet = 0; facet <= dim; ++facet) {
                Simplex<dim>* adj = s->adjacentSimplex(facet);
                if (adj) {
                    if constexpr (dim == 2) {
                        // We have already asserted that SnIndex() is a
                        // numeric (not character) type.
                        out_ << adj->index() << ' ' << static_cast<int>(
                            s->adjacentGluing(facet).SnIndex()) << ' ';
                    } else if constexpr (sizeof(
                            typename Perm<dim + 1>::ImagePack) == 1) {
                        // Image packs are chars.  We need to make them numeric.
                        out_ << adj->index() << ' ' << static_cast<int>(
                            s->adjacentGluing(facet).imagePack()) << ' ';
                    } else {
                        // Image packs are numeric, but may be larger than int.
                        // Just write them directly.
                        out_ << adj->index() << ' '
                            << s->adjacentGluing(facet).imagePack() << ' ';
                    }
                } else
                    out_ << "-1 -1 ";
            }
            if constexpr (dim == 2)
                out_ << "</triangle>\n";
            else if constexpr (dim == 3)
                out_ << "</tet>\n";
            else if constexpr (dim == 4)
                out_ << "</pent>\n";
            else
                out_ << "</simplex>\n";
        }
        if constexpr (dim == 2)
            out_ << "  </triangles>\n";
        else if constexpr (dim == 3)
            out_ << "  </tetrahedra>\n";
        else if constexpr (dim == 4)
            out_ << "  </pentachora>\n";
        else
            out_ << "  </simplices>\n";
    } else {
        for (auto s : data_.simplices()) {
            out_ << "  <simplex";
            if (auto locks = s->lockMask()) {
                out_ << std::hex;
                out_ << " lock=\"";
                if (sizeof(decltype(locks)) == 1) {
                    // Make sure this comes out as numeric, not a character.
                    out_ << static_cast<int>(locks);
                } else
                    out_ << locks;
                out_ << '\"';
                out_ << std::setbase(0);
            }
            if (! s->description().empty()) {
                out_ << " desc=\""
                    << regina::xml::xmlEncodeSpecialChars(s->description())
                    << '\"';
            }
            out_ << "> ";
            for (int facet = 0; facet <= dim; ++facet) {
                Simplex<dim>* adj = s->adjacentSimplex(facet);
                if (adj) {
                    if constexpr (useSnIndex) {
                        // We have already asserted that SnIndex() is a
                        // numeric (not character) type.
                        out_ << adj->index() << ' '
                            << s->adjacentGluing(facet).SnIndex() << ' ';
                    } else if constexpr (sizeof(
                            typename Perm<dim + 1>::ImagePack) == 1) {
                        // Image packs are chars.  We need to make them numeric.
                        out_ << adj->index() << ' ' << static_cast<int>(
                            s->adjacentGluing(facet).imagePack()) << ' ';
                    } else {
                        // Image packs are numeric, but may be larger than int.
                        // Just write them directly.
                        out_ << adj->index() << ' '
                            << s->adjacentGluing(facet).imagePack() << ' ';
                    }
                } else
                    out_ << "-1 -1 ";
            }
            out_ << "</simplex>\n";
        }
    }

    data_.writeXMLBaseProperties(out_);

    using regina::xml::xmlValueTag;

    if constexpr (dim == 3) {
        if (data_.prop_.H1Rel_.has_value()) {
            out_ << "  <H1Rel>";
            data_.prop_.H1Rel_->writeXMLData(out_);
            out_ << "</H1Rel>\n";
        }
        if (data_.prop_.H1Bdry_.has_value()) {
            out_ << "  <H1Bdry>";
            data_.prop_.H1Bdry_->writeXMLData(out_);
            out_ << "</H1Bdry>\n";
        }
        if (data_.prop_.H2_.has_value()) {
            out_ << "  <H2>";
            data_.prop_.H2_->writeXMLData(out_);
            out_ << "</H2>\n";
        }
        if (data_.prop_.zeroEfficient_.has_value())
            out_ << "  " << xmlValueTag("zeroeff", *data_.prop_.zeroEfficient_)
                << '\n';
        if (data_.prop_.oneEfficient_.has_value())
            out_ << "  " << xmlValueTag("oneeff", *data_.prop_.oneEfficient_)
                << '\n';
        if (data_.prop_.splittingSurface_.has_value())
            out_ << "  " << xmlValueTag("splitsfce",
                *data_.prop_.splittingSurface_) << '\n';
        if (data_.prop_.threeSphere_.has_value())
            out_ << "  " << xmlValueTag("threesphere",
                *data_.prop_.threeSphere_) << '\n';
        if (data_.prop_.handlebody_.has_value())
            out_ << "  " << xmlValueTag("handlebody",
                *data_.prop_.handlebody_) << '\n';
        if (data_.prop_.TxI_.has_value())
            out_ << "  " << xmlValueTag("txi", *data_.prop_.TxI_) << '\n';
        if (data_.prop_.irreducible_.has_value())
            out_ << "  " << xmlValueTag("irreducible",
                *data_.prop_.irreducible_) << '\n';
        if (data_.prop_.compressingDisc_.has_value())
            out_ << "  " << xmlValueTag("compressingdisc",
                *data_.prop_.compressingDisc_) << '\n';
        if (data_.prop_.haken_.has_value())
            out_ << "  " << xmlValueTag("haken", *data_.prop_.haken_) << '\n';
    } else if constexpr (dim == 4) {
        if (data_.prop_.H2_.has_value()) {
            out_ << "  <H2>";
            data_.prop_.H2_->writeXMLData(out_);
            out_ << "</H2>\n";
        }
    }
}

template <int dim>
void XMLWriter<Triangulation<dim>>::close() {
    if (format_ == FileFormat::XmlGen2)
        out_ << "</packet> <!-- Triangulation -->\n";
    else
        out_ << "</tri>\n";
}

template class XMLWriter<Triangulation<2>>;
template class XMLWriter<Triangulation<3>>;
template class XMLWriter<Triangulation<4>>;
template class XMLWriter<Triangulation<5>>;
template class XMLWriter<Triangulation<6>>;
template class XMLWriter<Triangulation<7>>;
template class XMLWriter<Triangulation<8>>;

template std::string PacketData<Triangulation<3>>::anonID() const;
template std::string PacketData<Triangulation<4>>::anonID() const;

template void PacketOf<Triangulation<2>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<3>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<4>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<5>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<6>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<7>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<8>>::addPacketRefs(PacketRefs&) const;

template void PacketOf<Triangulation<2>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<3>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<4>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<5>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<6>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<7>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<8>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;

#ifdef REGINA_HIGHDIM
template class XMLWriter<Triangulation<9>>;
template class XMLWriter<Triangulation<10>>;
template class XMLWriter<Triangulation<11>>;
template class XMLWriter<Triangulation<12>>;
template class XMLWriter<Triangulation<13>>;
template class XMLWriter<Triangulation<14>>;
template class XMLWriter<Triangulation<15>>;

template void PacketOf<Triangulation<9>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<10>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<11>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<12>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<13>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<14>>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Triangulation<15>>::addPacketRefs(PacketRefs&) const;

template void PacketOf<Triangulation<9>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<10>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<11>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<12>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<13>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<14>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
template void PacketOf<Triangulation<15>>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;
#endif /* REGINA_HIGHDIM */

} // namespace regina

