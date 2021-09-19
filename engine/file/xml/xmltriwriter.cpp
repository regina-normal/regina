
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "triangulation/generic.h"

namespace regina {

template <int dim>
void XMLWriter<Triangulation<dim>>::openPre() {
    if (format_ == REGINA_XML_GEN_2) {
        out_ << "<packet type=\"" << dim
            << "-Manifold Triangulation\" typeid=\"" << (100 + dim) << '\"';
    } else {
        out_ << "<tri dim=\"" << dim
            << "\" size=\"" << data_.size()
            << "\" perm=\"" << (useSnIndex ? "index" : "imagepack") << '\"';
    }
}

template <int dim>
void XMLWriter<Triangulation<dim>>::writeContent() {
    // Write the simplex gluings.
    // We will send permutation codes directly to the output stream.
    // This requires them to be numeric types (not character types).
    static_assert(! (
            std::is_same<typename Perm<dim + 1>::Code, char>::value ||
            std::is_same<typename Perm<dim + 1>::Code, unsigned char>::value
        ),
        "The generic implementation of Triangulation<dim>::writeXMLPacketData "
        "requires permutation codes to be numeric types.");

    if (format_ == REGINA_XML_GEN_2) {
        out_ << "  <simplices size=\"" << data_.size() << "\">\n";
        for (auto s : data_.simplices()) {
            out_ << "    <simplex desc=\"" <<
                regina::xml::xmlEncodeSpecialChars(s->description()) << "\"> ";
            for (int facet = 0; facet <= dim; ++facet) {
                Simplex<dim>* adj = s->adjacentSimplex(facet);
                if (adj) {
                    out_ << adj->index() << ' '
                        << s->adjacentGluing(facet).imagePack() << ' ';
                } else
                    out_ << "-1 -1 ";
            }
            out_ << "</simplex>\n";
        }
        out_ << "  </simplices>\n";
    } else {
        for (auto s : data_.simplices()) {
            if (s->description().empty())
                out_ << "  <simplex> ";
            else
                out_ << "  <simplex desc=\""
                    << regina::xml::xmlEncodeSpecialChars(s->description())
                    << "\"> ";
            for (int facet = 0; facet <= dim; ++facet) {
                Simplex<dim>* adj = s->adjacentSimplex(facet);
                if (adj) {
                    if constexpr (useSnIndex) {
                        out_ << adj->index() << ' '
                            << s->adjacentGluing(facet).SnIndex() << ' ';
                    } else {
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
}

template <int dim>
void XMLWriter<Triangulation<dim>>::close() {
    if (format_ == REGINA_XML_GEN_2)
        out_ << "</packet> <!-- Triangulation -->\n";
    else
        out_ << "</tri>\n";
}

template class XMLWriter<Triangulation<5>>;
template class XMLWriter<Triangulation<6>>;
template class XMLWriter<Triangulation<7>>;
template class XMLWriter<Triangulation<8>>;
template class XMLWriter<Triangulation<9>>;
template class XMLWriter<Triangulation<10>>;
template class XMLWriter<Triangulation<11>>;
template class XMLWriter<Triangulation<12>>;
template class XMLWriter<Triangulation<13>>;
template class XMLWriter<Triangulation<14>>;
template class XMLWriter<Triangulation<15>>;

} // namespace regina

