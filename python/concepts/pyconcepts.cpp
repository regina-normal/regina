
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "../helpers.h"
#include "../docstrings/concepts/array.h"
#include "../docstrings/concepts/core.h"
#include "../docstrings/concepts/io.h"
#include "../docstrings/concepts/iterator.h"
#include "../docstrings/concepts/maths.h"

namespace pybind11 { class module_; }

void addConceptsClasses(pybind11::module_& m) {
    using regina::python::add_concept;

    RDOC_SCOPE_BEGIN_MAIN

    // From array.h:
    add_concept<rdoc::ArrayOf>(m, "ArrayOf");
    add_concept<rdoc::ConstRefArrayOf>(m, "ConstRefArrayOf");

    // From core.h:
    add_concept<rdoc::AssignableTo>(m, "AssignableTo");
    add_concept<rdoc::CanConstruct>(m, "CanConstruct");
    add_concept<rdoc::SameModCVRef>(m, "SameModCVRef");
    add_concept<rdoc::RingLike>(m, "RingLike");
    add_concept<rdoc::Ring>(m, "Ring");
    add_concept<rdoc::Domain>(m, "Domain");
    add_concept<rdoc::IntegralDomain>(m, "IntegralDomain");
    add_concept<rdoc::Field>(m, "Field");
    add_concept<rdoc::CoefficientDomain>(m, "CoefficientDomain");
    add_concept<rdoc::ReginaBitmask>(m, "ReginaBitmask");
    add_concept<rdoc::ReginaQitmask>(m, "ReginaQitmask");
    add_concept<rdoc::StrictWeakOrder>(m, "StrictWeakOrder");
    add_concept<rdoc::VoidCallback>(m, "VoidCallback");
    add_concept<rdoc::TerminatingCallback>(m, "TerminatingCallback");

    // From io.h:
    add_concept<rdoc::Writeable>(m, "Writeable");
    add_concept<rdoc::Readable>(m, "Readable");
    add_concept<rdoc::Stringifiable>(m, "Stringifiable");
    add_concept<rdoc::RichStringifiable>(m, "RichStringifiable");
    add_concept<rdoc::StandardStringifiable>(m, "StandardStringifiable");
    add_concept<rdoc::InherentlyTightEncodable>(m, "InherentlyTightEncodable");

    // From iterator.h:
    add_concept<rdoc::InputIteratorFor>(m, "InputIteratorFor");
    add_concept<rdoc::ForwardIteratorFor>(m, "ForwardIteratorFor");
    add_concept<rdoc::BidirectionalIteratorFor>(m, "BidirectionalIteratorFor");
    add_concept<rdoc::RandomAccessIteratorFor>(m, "RandomAccessIteratorFor");
    add_concept<rdoc::SelfSentinelInputIterator>(m,
        "SelfSentinelInputIterator");
    add_concept<rdoc::CharIterator>(m, "CharIterator");
    add_concept<rdoc::PacketIterator>(m, "PacketIterator");
    add_concept<rdoc::Iterable>(m, "Iterable");
    add_concept<rdoc::IterableFor>(m, "IterableFor");
    add_concept<rdoc::BidirectionalIterableFor>(m, "BidirectionalIterableFor");
    add_concept<rdoc::RandomAccessIterableFor>(m, "RandomAccessIterableFor");
    add_concept<rdoc::OutputIterator>(m, "OutputIterator");
    add_concept<rdoc::IndexedContainer>(m, "IndexedContainer");

    // From maths.h:
    add_concept<rdoc::ArbitraryPrecisionIntegerVector>(m,
        "ArbitraryPrecisionIntegerVector");
    add_concept<rdoc::IntegerVector>(m, "IntegerVector");
    add_concept<rdoc::SignatureType>(m, "SignatureType");
    add_concept<rdoc::SignatureReconstructible>(m, "SignatureReconstructible");

    RDOC_SCOPE_END
}

