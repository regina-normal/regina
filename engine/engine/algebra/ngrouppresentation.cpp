
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include <strstream.h>
#include <hash_map>
#include "algebra/ngrouppresentation.h"
#include "file/nfile.h"
#include "maths/numbertheory.h"

typedef NDoubleListIterator<NGroupExpressionTerm> TermIterator;
typedef NDynamicArrayIterator<NGroupExpression*> RelIterator;
typedef NDoubleListIterator<NGroupExpression*> TmpRelIterator;
typedef NDoubleList<NGroupExpressionTerm&> TermList;

NGroupExpression::NGroupExpression(const NGroupExpression& cloneMe) {
    for (TermIterator it(cloneMe.terms); ! it.done(); it++)
        terms.addLast(*it);
}

NGroupExpression* NGroupExpression::inverse() const {
    NGroupExpression* ans = new NGroupExpression();
    for (TermIterator it(terms); ! it.done(); it++)
        ans->terms.addFirst(NGroupExpressionTerm((*it).first, -(*it).second));
    return ans;
}

NGroupExpression* NGroupExpression::power(long exponent) const {
    NGroupExpression* ans = new NGroupExpression();
    if (exponent == 0)
        return ans;
    
    long i;
    TermIterator it;
    if (exponent > 0)
        for (i = 0; i < exponent; i++)
            for (it.init(terms); ! it.done(); it++)
                ans->terms.addLast(*it);
    else
        for (i = 0; i > exponent; i--)
            for (it.init(terms); ! it.done(); it++)
                ans->terms.addFirst(
                    NGroupExpressionTerm((*it).first, -(*it).second));
    return ans;
}

bool NGroupExpression::simplify(bool cyclic) {
    bool changed = false;
    TermIterator next(terms);
    TermIterator tmpIt;
    while (! next.done()) {
        // Take a look at merging next forwards.

        if ((*next).second == 0) {
            // Zero exponent.
            // Delete this term and step back to the previous term in
            // case we can now merge the previous and next terms.
            tmpIt = next;
            next--;
            if (next.done()) {
                // Can't step backwards; this was the first term.
                next = tmpIt;
                next++;
            }
            terms.remove(tmpIt);
            changed = true;
            continue;
        }

        tmpIt = next;
        tmpIt++;
        
        // Now tmpIt points to the term after next.
        if (tmpIt.done()) {
            // No term to merge forwards with.
            next++;
        } else if ((*tmpIt).first == (*next).first) {
            // Merge this and the following term.
            (*next).second += (*tmpIt).second;
            terms.remove(tmpIt);
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
    while (terms.size() > 1) {
        next.init(terms);
        tmpIt.initEnd(terms);
        if ((*next).first == (*tmpIt).first) {
            // Merge!
            (*next).second += (*tmpIt).second;
            terms.remove(tmpIt);
            changed = true;

            // Did we create an empty term?
            if ((*next).second == 0)
                terms.remove(next);
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
    long exponent;

    TermIterator current(terms);
    TermIterator tmp;
    long i;
    while (! current.done()) {
        if ((*current).first != generator)
            current++;
        else {
            exponent = (*current).second;
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
                for (i = 0; i < exponent; i++)
                    for (tmp.init(use->terms); ! tmp.done(); tmp++)
                        terms.addBefore(*tmp, current);
            }

            terms.remove(current);
            changed = true;
        }
    }
    if (inverse)
        delete inverse;
    if (changed)
        simplify(cyclic);
    return changed;
}

void NGroupExpression::writeToFile(NFile& out) const {
    out.writeULong(terms.size());
    for (TermIterator it(terms); ! it.done(); it++) {
        out.writeULong((*it).first);
        out.writeLong((*it).second);
    }
}

NGroupExpression* NGroupExpression::readFromFile(NFile& in) {
    NGroupExpression* ans = new NGroupExpression();
    unsigned long nTerms = in.readULong();
    for (unsigned long i = 0; i < nTerms; i++)
        ans->terms.addLast(NGroupExpressionTerm(in.readULong(),
            in.readLong()));
    return ans;
}

void NGroupExpression::writeTextShort(ostream& out) const {
    bool first = true;
    for (TermIterator it(terms); ! it.done(); it++) {
        if (! first)
            out << ' ';
        first = false;
        out << 'g' << (*it).first;
        if ((*it).second != 1)
            out << '^' << (*it).second;
    }
}

NGroupPresentation::NGroupPresentation(const NGroupPresentation& cloneMe) :
        nGenerators(cloneMe.nGenerators) {
    for (RelIterator it(cloneMe.relations); ! it.done(); it++)
        relations.addLast(new NGroupExpression(**it));
}

bool NGroupPresentation::intelligentSimplify() {
    unsigned long oldNGenerators = nGenerators;
    bool changed = false; // Has anything changed at all?
    bool removed = false; // Have we deleted any relations?

    // Store the relations in a temporary linked list for fast insertion
    // and removal.  We'll put the ones we kept back into the original
    // array at the end.
    NDoubleList<NGroupExpression*> tmpRels;
    NGroupExpression* rel;
    for (RelIterator it(relations); ! it.done(); it++) {
        rel = *it;
        // Do an initial simplification on each relation as we go.
        if (rel->simplify(true))
            changed = true;
        if (rel->getNumberOfTerms() == 0) {
            delete rel;
            changed = true;
            removed = true;
        } else
            tmpRels.addLast(rel);
    }

    // At this point all relations are simplified and none are empty.
    // Throughout the remainder of this routine we will attempt to
    // preserve this state of affairs.
    TmpRelIterator it;
    TmpRelIterator it2;
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
    std::hash_map<unsigned long, long> exponents;
    std::hash_map<unsigned long, long>::iterator expIt;
    NGroupExpression* expansion;
    bool doMoreSubsts = true;
    while (doMoreSubsts) {
        doMoreSubsts = false;
        it.init(tmpRels);
        while (! it.done()) {
            // Can we pull a single variable out of this relation?
            rel = *it;
            // How many times does each generator appear in this relation?
            for (tit.init(rel->getTerms()); ! tit.done(); tit++) {
                // Find this generator, or insert it with exponent 0 if
                // it's not already present.
                expIt = exponents.insert(make_pair((*tit).first, 0)).first;
                if ((*tit).second < 0)
                    (*expIt).second -= (*tit).second;
                else
                    (*expIt).second += (*tit).second;
            }
            // Did any generator appear precisely once?
            expIt = find_if(exponents.begin(), exponents.end(), compose1(
                bind2nd(equal_to<long>(), 1),
                select2nd<pair<unsigned long, long> >()));
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
            for (tit.init(rel->getTerms()); (*tit).first != gen; tit++)
                expansion->addTermFirst((*tit).first, -(*tit).second);
            for (tit.initEnd(rel->getTerms()); (*tit).first != gen; tit--)
                expansion->addTermLast((*tit).first, -(*tit).second);
            // Check if we need to invert it.
            if ((*tit).second == -1) {
                rel = expansion;
                expansion = expansion->inverse();
                delete rel;
            }
            // Do the substitution.
            it2.init(tmpRels);
            while (! it2.done())
                if (*it2 != *it) {
                    (*it2)->substitute(gen, *expansion, true);
                    if ((*it2)->getNumberOfTerms() == 0)
                        delete tmpRels.remove(it2);
                    else
                        it2++;
                } else
                    it2++;

            // Note that we are removing a generator.
            genMap[gen] = -1;
            nGenerators--;

            // Remove the now useless relation and tidy up.
            delete tmpRels.remove(it);
            delete expansion;
            changed = true;
            removed = true;
            doMoreSubsts = true;
        }
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
        for (it.init(tmpRels); ! it.done(); it++)
            for (tit.init((*it)->getTerms()); ! tit.done(); tit++)
                (*tit).first = genMap[(*tit).first];
    }

    // Refill the original array if necessary.
    if (removed) {
        relations.flush();
        for (it.init(tmpRels); ! it.done(); it++)
            relations.addLast(*it);
    }

    // Done!
    return changed;
}

NString NGroupPresentation::recogniseGroup() {
    ostrstream out;
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
    } else if (nRels == 0)
        out << "Free group on " << nGenerators << " generators";
    else if  (nGenerators == 2 && nRels == 2) {
        // See if it's the quaternions.
        
    } else {
        // nGenerators >= 2 and nRels >= 2.
        // Don't have anything intelligent to say at this point.
    }
    out.put(0);
    NString ans(out.str());
    out.freeze(0);
    return ans;
}

void NGroupPresentation::writeToFile(NFile& out) const {
    out.writeULong(nGenerators);
    out.writeULong(relations.size());
    for (RelIterator it(relations); ! it.done(); it++)
        (*it)->writeToFile(out);
    
    // Write properties.
    writeAllPropertiesFooter(out);
}

NGroupPresentation* NGroupPresentation::readFromFile(NFile& in) {
    NGroupPresentation* ans = new NGroupPresentation();
    ans->nGenerators = in.readULong();
    unsigned long nRels = in.readULong();
    for (unsigned long i = 0; i < nRels; i++)
        ans->relations.addLast(NGroupExpression::readFromFile(in));

    // Read properties.
    ans->readProperties(in);

    return ans;
}

void NGroupPresentation::writeTextLong(ostream& out) const {
    out << "Generators: ";
    if (nGenerators == 0)
        out << "(none)";
    else if (nGenerators == 1)
        out << "g0";
    else if (nGenerators == 2)
        out << "g0, g1";
    else
        out << "g0 .. g" << (nGenerators - 1);
    out << endl;

    out << "Relations:\n";
    if (relations.size() == 0)
        out << "    (none)\n";
    else
        for (RelIterator it(relations); ! it.done(); it++) {
            out << "    ";
            (*it)->writeTextShort(out);
            out << endl;
        }
}

