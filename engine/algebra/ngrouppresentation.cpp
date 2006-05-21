
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <iterator>
#include <sstream>
#include "algebra/ngrouppresentation.h"
#include "file/nfile.h"
#include "maths/numbertheory.h"
#include "utilities/boostutils.h"
#include "utilities/hashmap.h"
#include "utilities/hashutils.h"
#include "utilities/stlutils.h"

namespace regina {

typedef std::list<NGroupExpressionTerm>::iterator TermIterator;
typedef std::list<NGroupExpressionTerm>::const_iterator TermIteratorConst;
typedef std::vector<NGroupExpression*>::iterator RelIterator;
typedef std::vector<NGroupExpression*>::const_iterator RelIteratorConst;
typedef std::list<NGroupExpression*>::iterator TmpRelIterator;

NGroupExpressionTerm NGroupExpressionTerm::readFromFile(NFile& in) {
    return NGroupExpressionTerm(in.readULong(), in.readLong());
}

void NGroupExpressionTerm::writeToFile(NFile& out) const {
    out.writeULong(generator);
    out.writeLong(exponent);
}

std::ostream& operator << (std::ostream& out,
        const NGroupExpressionTerm& term) {
    if (term.exponent == 0)
        out << '1';
    else if (term.exponent == 1)
        out << 'g' << term.generator;
    else
        out << 'g' << term.generator << '^' << term.exponent;
    return out;
}

NGroupExpressionTerm& NGroupExpression::getTerm(unsigned long index) {
    TermIterator pos = terms.begin();
    advance(pos, index);
    return *pos;
}

const NGroupExpressionTerm& NGroupExpression::getTerm(
        unsigned long index) const {
    TermIteratorConst pos = terms.begin();
    advance(pos, index);
    return *pos;
}

NGroupExpression* NGroupExpression::inverse() const {
    NGroupExpression* ans = new NGroupExpression();
    transform(terms.begin(), terms.end(), front_inserter(ans->terms),
        std::mem_fun_ref(&NGroupExpressionTerm::inverse));
    return ans;
}

NGroupExpression* NGroupExpression::power(long exponent) const {
    NGroupExpression* ans = new NGroupExpression();
    if (exponent == 0)
        return ans;

    long i;
    if (exponent > 0)
        for (i = 0; i < exponent; i++)
            ans->terms.insert(ans->terms.end(), terms.begin(), terms.end());
    else
        for (i = 0; i > exponent; i--)
            transform(terms.begin(), terms.end(), front_inserter(ans->terms),
                std::mem_fun_ref(&NGroupExpressionTerm::inverse));
    return ans;
}

bool NGroupExpression::simplify(bool cyclic) {
    bool changed = false;
    TermIterator next, tmpIt;
    for (next = terms.begin(); next != terms.end(); ) {
        // Take a look at merging next forwards.
        if ((*next).exponent == 0) {
            // Zero exponent.
            // Delete this term and step back to the previous term in
            // case we can now merge the previous and next terms.
            next = terms.erase(next);
            if (next != terms.begin())
                next--;
            changed = true;
            continue;
        }

        tmpIt = next;
        tmpIt++;

        // Now tmpIt points to the term after next.
        if (tmpIt == terms.end()) {
            // No term to merge forwards with.
            next++;
        } else if ((*tmpIt) += (*next)) {
            // Successfully merged this with the following term.
            next = terms.erase(next);
            changed = true;
            // Look at this term again to see if it can be merged further.
        } else {
            // Different generators; cannot merge.
            next++;
        }
    }

    if (! cyclic)
        return changed;

    // Now trying merging front and back terms.
    // We shall do this by popping terms off the back and merging them
    // with the front term.
    while (terms.begin() != terms.end() &&
            ++terms.begin() != terms.end()) {
        // Thus terms.size() > 1.  The unusual test above is used to
        // avoid calling terms.size() which takes linear time.
        if (terms.front() += terms.back()) {
            // Merged!
            terms.pop_back();
            changed = true;

            // Did we create an empty term?
            if (terms.front().exponent == 0)
                terms.pop_front();
        } else
            break;
    }

    return changed;
}

bool NGroupExpression::substitute(unsigned long generator,
        const NGroupExpression& expansion, bool cyclic) {
    bool changed = false;
    NGroupExpression* inverse = 0;
    const NGroupExpression* use;
    long exponent, i;
    for (TermIterator current = terms.begin(); current != terms.end(); ) {
        if ((*current).generator != generator)
            current++;
        else {
            exponent = (*current).exponent;
            if (exponent != 0) {
                if (exponent > 0)
                    use = &expansion;
                else {
                    if (inverse == 0)
                        inverse = expansion.inverse();
                    use = inverse;
                    exponent = -exponent;
                }

                // Fill in exponent copies of use.
                //
                // Note that the following insertion will invalidate
                // current if the wrong type of data structure is being used!
                for (i = 0; i < exponent; i++)
                    terms.insert(current, use->terms.begin(), use->terms.end());
            }

            current = terms.erase(current);
            changed = true;
        }
    }
    if (inverse)
        delete inverse;
    if (changed)
        simplify(cyclic);
    return changed;
}

void NGroupExpression::writeXMLData(std::ostream& out) const {
    out << "<reln> ";
    for (TermIteratorConst it = terms.begin(); it != terms.end(); it++)
        out << (*it).generator << '^' << (*it).exponent << ' ';
    out << "</reln>";
}

void NGroupExpression::writeToFile(NFile& out) const {
    out.writeULong(terms.size());
    for (TermIteratorConst it = terms.begin(); it != terms.end(); it++)
        (*it).writeToFile(out);
}

NGroupExpression* NGroupExpression::readFromFile(NFile& in) {
    NGroupExpression* ans = new NGroupExpression();
    unsigned long nTerms = in.readULong();
    for (unsigned long i = 0; i < nTerms; i++)
        ans->terms.push_back(NGroupExpressionTerm::readFromFile(in));
    return ans;
}

void NGroupExpression::writeTextShort(std::ostream& out) const {
    if (terms.empty())
        out << '1';
    else {
        TermIteratorConst last = --terms.end();
        copy(terms.begin(), last,
            std::ostream_iterator<NGroupExpressionTerm>(out, " "));
        out << *last;
    }
}

NGroupPresentation::NGroupPresentation(const NGroupPresentation& cloneMe) :
        ShareableObject(), nGenerators(cloneMe.nGenerators) {
    transform(cloneMe.relations.begin(), cloneMe.relations.end(),
        back_inserter(relations), FuncNewCopyPtr<NGroupExpression>());
}

bool NGroupPresentation::intelligentSimplify() {
    unsigned long oldNGenerators = nGenerators;
    bool changed = false; // Has anything changed at all?
    bool removed = false; // Have we deleted any relations?

    // Store the relations in a temporary linked list for fast insertion
    // and removal.  We'll put the ones we kept back into the original
    // array at the end.
    std::list<NGroupExpression*> tmpRels;
    NGroupExpression* rel;
    for (RelIterator it = relations.begin(); it != relations.end(); it++) {
        rel = *it;
        // Do an initial simplification on each relation as we go.
        if (rel->simplify(true))
            changed = true;
        if (rel->getNumberOfTerms() == 0) {
            delete rel;
            changed = true;
            removed = true;
        } else
            tmpRels.push_back(rel);
    }

    // At this point all relations are simplified and none are empty.
    // Throughout the remainder of this routine we will attempt to
    // preserve this state of affairs.
    TmpRelIterator it, it2, it3;
    TermIterator tit;

    // Make a table of generators that have been removed.
    // A value in the array will be set to -1 when the corresponding
    // generator is removed.
    unsigned long gen;
    long* genMap = new long[nGenerators];
    for (gen = 0; gen < nGenerators; gen++)
        genMap[gen] = gen;

    // Run through and look for substitutions we can make.
    // This currently isn't magnificently optimised.
    stdhash::hash_map<unsigned long, long> exponents;
    stdhash::hash_map<unsigned long, long>::iterator expIt;
    NGroupExpression* expansion;
    unsigned long gen1, gen2, genRemove;
    long exp1a, exp1b, exp2a, exp2b;

    bool doMoreSubsts = true;
    while (doMoreSubsts) {
        doMoreSubsts = false;

        // Look for generator substitution.
        it = tmpRels.begin();
        while (it != tmpRels.end()) {
            // Can we pull a single variable out of this relation?
            rel = *it;
            // How many times does each generator appear in this relation?
            for (tit = rel->getTerms().begin();
                    tit != rel->getTerms().end(); tit++) {
                // Find this generator, or insert it with exponent 0 if
                // it's not already present.
                expIt = exponents.insert(
                    std::make_pair((*tit).generator, 0)).first;
                if ((*tit).exponent < 0)
                    (*expIt).second -= (*tit).exponent;
                else
                    (*expIt).second += (*tit).exponent;
            }
            // Did any generator appear precisely once?
            expIt = find_if(exponents.begin(), exponents.end(),
                regina::stl::compose1(bind2nd(std::equal_to<long>(), 1),
                regina::stl::select2nd<std::pair<unsigned long, long> >()));
            if (expIt == exponents.end()) {
                // Can't use this relation.  Move on.
                exponents.clear();
                it++;
                continue;
            }
            gen = (*expIt).first;
            exponents.clear();

            // We are going to replace generator gen.
            // Build up the expansion.
            expansion = new NGroupExpression();
            for (tit = rel->getTerms().begin(); (*tit).generator != gen; tit++)
                expansion->addTermFirst((*tit).inverse());
            for (tit = --(rel->getTerms().end());
                    (*tit).generator != gen; tit--)
                expansion->addTermLast((*tit).inverse());
            // Check if we need to invert it.
            if ((*tit).exponent == -1) {
                rel = expansion;
                expansion = expansion->inverse();
                delete rel;
            }
            // Do the substitution.
            it2 = tmpRels.begin();
            while (it2 != tmpRels.end())
                if (it2 != it) {
                    (*it2)->substitute(gen, *expansion, true);
                    if ((*it2)->getNumberOfTerms() == 0) {
                        delete *it2;
                        it2 = tmpRels.erase(it2);
                    } else
                        it2++;
                } else
                    it2++;

            // Note that we are removing a generator.
            genMap[gen] = -1;
            nGenerators--;

            // Remove the now useless relation and tidy up.
            delete expansion;
            delete *it;
            it = tmpRels.erase(it);
            changed = true;
            removed = true;
            doMoreSubsts = true;
        }

        // Look for pairs of two-generator relations that imply gi == gj^k.
        // As soon as we find such a pair we perform the substitution
        // and break from the loop.
        for (it = tmpRels.begin();
                it != tmpRels.end() && (! doMoreSubsts); it++) {
            if ((*it)->getNumberOfTerms() != 2)
                continue;
            gen1 = (*it)->getGenerator(0);
            gen2 = (*it)->getGenerator(1);
            if (gen1 == gen2)
                continue;
            exp1a = (*it)->getExponent(0);
            exp2a = (*it)->getExponent(1);

            for (it2 = regina::boost::next(it); it2 != tmpRels.end(); it2++) {
                if ((*it2)->getNumberOfTerms() != 2)
                    continue;
                if (gen1 == (*it2)->getGenerator(0) &&
                        gen2 == (*it2)->getGenerator(1)) {
                    exp1b = (*it2)->getExponent(0);
                    exp2b = (*it2)->getExponent(1);
                } else if (gen1 == (*it2)->getGenerator(1) &&
                        gen2 == (*it2)->getGenerator(0)) {
                    exp1b = (*it2)->getExponent(1);
                    exp2b = (*it2)->getExponent(0);
                } else
                    continue;

                // We have two relations of the form
                // (x^a y^b == 1), (x^c y^d == 1).
                NGroupExpression expansion;
                if (exp1b == exp1a + 1) {
                    genRemove = gen1;
                    expansion.addTermLast(gen2, exp2a - exp2b);
                } else if (exp1b == exp1a - 1) {
                    genRemove = gen1;
                    expansion.addTermLast(gen2, -exp2a + exp2b);
                } else if (exp1b == -exp1a + 1) {
                    genRemove = gen1;
                    expansion.addTermLast(gen2, -exp2a - exp2b);
                } else if (exp1b == -exp1a - 1) {
                    genRemove = gen1;
                    expansion.addTermLast(gen2, exp2a + exp2b);
                } else if (exp2b == exp2a + 1) {
                    genRemove = gen2;
                    expansion.addTermLast(gen1, exp1a - exp1b);
                } else if (exp2b == exp2a - 1) {
                    genRemove = gen2;
                    expansion.addTermLast(gen1, -exp1a + exp1b);
                } else if (exp2b == -exp2a + 1) {
                    genRemove = gen2;
                    expansion.addTermLast(gen1, -exp1a - exp1b);
                } else if (exp2b == -exp2a - 1) {
                    genRemove = gen2;
                    expansion.addTermLast(gen1, exp1a + exp1b);
                } else
                    continue;

                // We can now substitute out genRemove.
                it3 = tmpRels.begin();
                while (it3 != tmpRels.end())
                    if (it3 != it2) {
                        (*it3)->substitute(genRemove, expansion, true);
                        if ((*it3)->getNumberOfTerms() == 0) {
                            delete *it3;
                            it3 = tmpRels.erase(it3);
                        } else
                            it3++;
                    } else
                        it3++;

                // Note that we are removing a generator.
                genMap[genRemove] = -1;
                nGenerators--;

                // Remove the now useless relation, tidy up and break
                // from the loops.
                delete *it2;
                tmpRels.erase(it2);
                changed = true;
                removed = true;
                doMoreSubsts = true;
                break;
            }
        }

        // TODO: Look for duplicate relations.
    }

    // Renumber the generators if necessary so we go from 0 to
    // nGenerators with no gaps.
    if (nGenerators < oldNGenerators) {
        // Rebuild the generator mapping table.
        unsigned long newGen = 0;
        for (gen = 0; gen < oldNGenerators; gen++)
            if (genMap[gen] >= 0)
                genMap[gen] = newGen++;

        // Now run through the relations and renumber the generators.
        for (it = tmpRels.begin(); it != tmpRels.end(); it++)
            for (tit = (*it)->getTerms().begin();
                    tit != (*it)->getTerms().end(); tit++)
                (*tit).generator = genMap[(*tit).generator];
    }

    // Refill the original array if necessary.
    if (removed) {
        relations.clear();
        relations.insert(relations.end(), tmpRels.begin(), tmpRels.end());
    }

    // Done!
    return changed;
}

std::string NGroupPresentation::recogniseGroup() const {
    std::ostringstream out;
    unsigned long nRels = relations.size();
    NGroupExpression* rel;
    long exp;

    // Run through cases.
    if (nGenerators == 0)
        out << 0;
    else if (nGenerators == 1) {
        // Each term is of the form g^k=1.  This is Z_d where d is the
        // gcd of the various values of k.
        unsigned long d = 0;
        for (unsigned long i = 0; i < nRels; i++) {
            rel = relations[i];
            if (rel->getNumberOfTerms() > 1)
                rel->simplify();
            // The relation should have at most one term now.
            if (rel->getNumberOfTerms() == 1) {
                exp = rel->getExponent(0);
                if (exp > 0)
                    d = gcd(d, exp);
                else if (exp < 0)
                    d = gcd(d, -exp);
            }
        }
        if (d == 0)
            out << 'Z';
        else if (d == 1)
            out << 0;
        else
            out << "Z_" << d;
    } else if (nRels == 0) {
        out << "Free (" << nGenerators << " generators)";
    } else if (nGenerators == 2 && nRels == 1) {
        // See if it's the abelian Z + Z.
        rel = relations[0];
        rel->simplify(true);

        // Look for a relation (x y x^-1 y^-1).
        if (rel->getNumberOfTerms() == 4) {
            if (rel->getGenerator(0) == rel->getGenerator(2) &&
                    rel->getGenerator(1) == rel->getGenerator(3) &&
                    rel->getGenerator(0) != rel->getGenerator(1) &&
                    abs(rel->getExponent(0)) == 1 &&
                    abs(rel->getExponent(1)) == 1 &&
                    rel->getExponent(0) + rel->getExponent(2) == 0 &&
                    rel->getExponent(1) + rel->getExponent(3) == 0)
                out << "Z + Z (abelian)";
        }
    } else if (nGenerators == 2 && nRels == 2) {
        // TODO: See if it's the quaternions.
    } else {
        // nGenerators >= 2 and nRels >= 2.
        // Don't have anything intelligent to say at this point.
    }
    return out.str();
}

void NGroupPresentation::writeXMLData(std::ostream& out) const {
    out << "<group generators=\"" << nGenerators << "\">\n";
    for (RelIteratorConst it = relations.begin(); it != relations.end(); it++) {
        out << "  ";
        (*it)->writeXMLData(out);
        out << '\n';
    }
    out << "</group>\n";
}

void NGroupPresentation::writeToFile(NFile& out) const {
    out.writeULong(nGenerators);
    out.writeULong(relations.size());
    for (RelIteratorConst it = relations.begin(); it != relations.end(); it++)
        (*it)->writeToFile(out);

    // Write properties.
    out.writeAllPropertiesFooter();
}

NGroupPresentation* NGroupPresentation::readFromFile(NFile& in) {
    NGroupPresentation* ans = new NGroupPresentation();
    ans->nGenerators = in.readULong();
    unsigned long nRels = in.readULong();
    for (unsigned long i = 0; i < nRels; i++)
        ans->relations.push_back(NGroupExpression::readFromFile(in));

    // Read properties.
    in.readProperties(0);

    return ans;
}

void NGroupPresentation::writeTextLong(std::ostream& out) const {
    out << "Generators: ";
    if (nGenerators == 0)
        out << "(none)";
    else if (nGenerators == 1)
        out << "g0";
    else if (nGenerators == 2)
        out << "g0, g1";
    else
        out << "g0 .. g" << (nGenerators - 1);
    out << std::endl;

    out << "Relations:\n";
    if (relations.empty())
        out << "    (none)\n";
    else
        for (RelIteratorConst it = relations.begin();
                it != relations.end(); it++) {
            out << "    ";
            (*it)->writeTextShort(out);
            out << std::endl;
        }
}

} // namespace regina

