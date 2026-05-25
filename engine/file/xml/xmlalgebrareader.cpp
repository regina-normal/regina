
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "file/xml/xmlalgebrareader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various task-specific packet readers.
 */
namespace {
    /**
     * Reads a single relation in a group presentation.
     */
    class ExpressionReader : public XMLElementReader {
        private:
            std::optional<GroupExpression> exp_;
            size_t nGens_;

        public:
            ExpressionReader(size_t nGens) : exp_(GroupExpression()),
                    nGens_(nGens) {
            }

            std::optional<GroupExpression>& expression() {
                return exp_;
            }

            void initialChars(const std::string& chars) override {
                try {
                    for (const std::string& t : basicTokenise(chars)) {
                        auto split = t.find('^');
                        if (split == t.length()) {
                            exp_.reset();
                            return;
                        }

                        size_t gen = parse<size_t>(t.substr(0, split));
                        long pow = parse<long>(t.substr(split + 1,
                            t.length() - split - 1));

                        // We already have gen >= 0, since size_t is unsigned.
                        if (gen >= nGens_) {
                            exp_.reset();
                            return;
                        }

                        exp_->addTermLast(gen, pow);
                    }
                } catch (const InvalidArgument&) {
                    exp_.reset();
                }
            }
    };
}

void XMLAbelianGroupReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, XMLElementReader*) {
    try {
        group_ = AbelianGroup(parse<size_t>(tagProps.lookup("rank")));
    } catch (const InvalidArgument&) {
        // Just leave group_ unset.
    }
}

void XMLAbelianGroupReader::initialChars(const std::string& chars) {
    if (group_) {
        try {
            for (const std::string& t : basicTokenise(chars))
                group_->addTorsion(Integer(t));
        } catch (const InvalidArgument&) {
            group_.reset();
        }
    }
}

void XMLGroupPresentationReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, XMLElementReader*) {
    try {
        size_t nGen = parse<size_t>(tagProps.lookup("generators"));
        group_ = GroupPresentation();
        if (nGen)
            group_->addGenerator(nGen);
    } catch (const InvalidArgument&) {
        // It must have been parse() that failed - just leave group_ unset.
    }
}

XMLElementReader* XMLGroupPresentationReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& /* subTagProps */) {
    if (group_)
        if (subTagName == "reln")
            return new ExpressionReader(group_->countGenerators());
    return new XMLElementReader();
}

void XMLGroupPresentationReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (group_)
        if (subTagName == "reln") {
            auto& exp = static_cast<ExpressionReader*>(subReader)->expression();
            if (exp)
                group_->addRelation(std::move(*exp));
        }
}

} // namespace regina

