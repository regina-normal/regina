
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "algebra/nxmlalgebrareader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various task-specific packet readers.
 */
namespace {
    /**
     * Reads a single relation in a group presentation.
     */
    class NExpressionReader : public NXMLElementReader {
        private:
            NGroupExpression* exp;
            long nGens;

        public:
            NExpressionReader(long newNGens) : exp(new NGroupExpression()),
                    nGens(newNGens) {
            }

            NGroupExpression* getExpression() {
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

void NXMLAbelianGroupReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, NXMLElementReader*) {
    long rank;
    if (valueOf(tagProps.lookup("rank"), rank))
        if (rank >= 0) {
            group = new NAbelianGroup();
            if (rank)
                group->addRank(rank);
        }
}

void NXMLAbelianGroupReader::initialChars(const std::string& chars) {
    if (group) {
        std::list<std::string> tokens;
        if (basicTokenise(back_inserter(tokens), chars) > 0) {
            std::multiset<NLargeInteger> torsion;
            NLargeInteger val;

            for (std::list<std::string>::const_iterator it = tokens.begin();
                    it != tokens.end(); it++)
                if (valueOf(*it, val))
                    torsion.insert(val);

            if (! torsion.empty())
                group->addTorsionElements(torsion);
        }
    }
}

void NXMLGroupPresentationReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& tagProps, NXMLElementReader*) {
    long nGen;
    if (valueOf(tagProps.lookup("generators"), nGen))
        if (nGen >= 0) {
            group = new NGroupPresentation();
            if (nGen)
                group->addGenerator(nGen);
        }
}

NXMLElementReader* NXMLGroupPresentationReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (group)
        if (subTagName == "reln")
            return new NExpressionReader(group->getNumberOfGenerators());
    return new NXMLElementReader();
}

void NXMLGroupPresentationReader::endSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (group)
        if (subTagName == "reln") {
            NGroupExpression* exp =
                dynamic_cast<NExpressionReader*>(subReader)->getExpression();
            if (exp)
                group->addRelation(exp);
        }
}

} // namespace regina

