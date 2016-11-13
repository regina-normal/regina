
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/xmlalgebrareader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various task-specific packet readers.
 */
namespace {
    /**
     * Reads a single relation in a group presentation.
     */
    class NExpressionReader : public XMLElementReader {
        private:
            GroupExpression* exp;
            long nGens;

        public:
            NExpressionReader(long newNGens) : exp(new GroupExpression()),
                    nGens(newNGens) {
            }

            GroupExpression* getExpression() {
                return exp;
            }

            virtual void initialChars(const std::string& chars) {
                std::list<std::string> tokens;
                basicTokenise(back_inserter(tokens), chars);

                std::string genStr, powStr;
                long gen, pow;
                std::string::size_type split;
                for (std::list<std::string>::const_iterator it = tokens.begin();
                        it != tokens.end(); it++) {
                    split = (*it).find('^');
                    if (split == (*it).length()) {
                        delete exp;
                        exp = 0;
                        break;
                    }

                    genStr = (*it).substr(0, split);
                    powStr = (*it).substr(split + 1,
                        (*it).length() - split - 1);

                    if ((! valueOf(genStr, gen)) || (! valueOf(powStr, pow))) {
                        delete exp;
                        exp = 0;
                        break;
                    } 

                    if (gen < 0 || gen >= nGens) {
                        delete exp;
                        exp = 0;
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
            group_ = new AbelianGroup();
            if (rank)
                group_->addRank(rank);
        }
}

void XMLAbelianGroupReader::initialChars(const std::string& chars) {
    if (group_) {
        std::list<std::string> tokens;
        if (basicTokenise(back_inserter(tokens), chars) > 0) {
            std::multiset<Integer> torsion;
            Integer val;

            for (std::list<std::string>::const_iterator it = tokens.begin();
                    it != tokens.end(); it++)
                if (valueOf(*it, val))
                    torsion.insert(val);

            if (! torsion.empty())
                group_->addTorsionElements(torsion);
        }
    }
}

void XMLGroupPresentationReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, XMLElementReader*) {
    long nGen;
    if (valueOf(tagProps.lookup("generators"), nGen))
        if (nGen >= 0) {
            group_ = new GroupPresentation();
            if (nGen)
                group_->addGenerator(nGen);
        }
}

XMLElementReader* XMLGroupPresentationReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& /* subTagProps */) {
    if (group_)
        if (subTagName == "reln")
            return new NExpressionReader(group_->countGenerators());
    return new XMLElementReader();
}

void XMLGroupPresentationReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (group_)
        if (subTagName == "reln") {
            GroupExpression* exp =
                dynamic_cast<NExpressionReader*>(subReader)->getExpression();
            if (exp)
                group_->addRelation(exp);
        }
}

} // namespace regina

