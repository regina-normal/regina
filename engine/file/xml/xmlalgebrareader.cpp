
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
            std::optional<GroupExpression> exp;
            long nGens;

        public:
            ExpressionReader(long newNGens) : exp(GroupExpression()),
                    nGens(newNGens) {
            }

            std::optional<GroupExpression>& expression() {
                return exp;
            }

            void initialChars(const std::string& chars) override {
                std::vector<std::string> tokens = basicTokenise(chars);

                std::string genStr, powStr;
                long gen, pow;
                std::string::size_type split;
                for (const std::string& t : tokens) {
                    split = t.find('^');
                    if (split == t.length()) {
                        exp.reset();
                        break;
                    }

                    genStr = t.substr(0, split);
                    powStr = t.substr(split + 1, t.length() - split - 1);

                    if ((! valueOf(genStr, gen)) || (! valueOf(powStr, pow))) {
                        exp.reset();
                        break;
                    } 

                    if (gen < 0 || gen >= nGens) {
                        exp.reset();
                        break;
                    }

                    exp->addTermLast(gen, pow);
                }
            }
    };
}

void XMLAbelianGroupReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, XMLElementReader*) {
    long rank;
    if (valueOf(tagProps.lookup("rank"), rank))
        if (rank >= 0) {
            group_ = AbelianGroup();
            if (rank)
                group_->addRank(rank);
        }
}

void XMLAbelianGroupReader::initialChars(const std::string& chars) {
    if (group_) {
        std::vector<std::string> tokens = basicTokenise(chars);
        try {
            for (const std::string& t : tokens)
                group_->addTorsion(Integer(t));
        } catch (const regina::InvalidArgument&) {
            // Unparseable.
            group_.reset();
        }
    }
}

void XMLGroupPresentationReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, XMLElementReader*) {
    long nGen;
    if (valueOf(tagProps.lookup("generators"), nGen))
        if (nGen >= 0) {
            group_ = GroupPresentation();
            if (nGen)
                group_->addGenerator(nGen);
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

