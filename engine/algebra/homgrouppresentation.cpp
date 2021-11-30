
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

#include <iterator>
#include <sstream>
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "maths/numbertheory.h"

namespace regina {

HomGroupPresentation::HomGroupPresentation(
            const GroupPresentation& groupForIdentity) :
        domain_(groupForIdentity), codomain_(groupForIdentity),
        map_(groupForIdentity.countGenerators()),
        inv_(std::in_place, groupForIdentity.countGenerators()) {
    unsigned long i = 0;
    for (GroupExpression& e : map_)
        e.addTermFirst(i++, 1);
    i = 0;
    for (GroupExpression& e : *inv_)
        e.addTermFirst(i++, 1);
}

HomMarkedAbelianGroup HomGroupPresentation::markedAbelianisation() const {
    MarkedAbelianGroup DOM = domain_.markedAbelianisation();
    MarkedAbelianGroup RAN = codomain_.markedAbelianisation();
    MatrixInt ccMat( RAN.ccRank(), DOM.ccRank() );
    for (unsigned long j=0; j<ccMat.columns(); j++) {
        GroupExpression COLj( evaluate(j) );
        for (unsigned long i=0; i<COLj.countTerms(); i++)
            ccMat.entry( COLj.generator(i), j ) += COLj.exponent(i);
    }
    return HomMarkedAbelianGroup(std::move(DOM), std::move(RAN),
        std::move(ccMat));
}

void HomGroupPresentation::writeTextShort(std::ostream& out) const {
    if (inv_)
        out << "Isomorphism from ";
    else
        out << "Homomorphism from ";
    domain_.writeTextShort(out);
    out << " to ";
    codomain_.writeTextShort(out);
}

void HomGroupPresentation::writeTextLong(std::ostream& out) const {
    if (inv_)
        out << "Isomorphism with ";
    else
        out << "Homomorphism with ";
    out<<"domain ";
    domain_.writeTextCompact(out);
    out<<" "; // std::endl;

    out<<"map[";
    for (unsigned long i=0; i<domain_.countGenerators(); i++) {
        if (i!=0)
            out<<", ";
        if (domain_.countGenerators()<=26)
            out<<char('a' + i)<<" --> ";
        else
            out<<"g"<<i<<" --> ";
        map_[i].writeTextShort(out, false, codomain_.countGenerators()<=26);
    }
    out<<"] ";

    out<<"codomain ";
    codomain_.writeTextCompact(out);
    out<<std::endl;
}

bool HomGroupPresentation::smallCancellation() {
    auto codomainMap = codomain_.smallCancellation();
    auto domainMap = domain_.smallCancellation();
    bool retval = codomainMap || domainMap;
    if (! domainMap)
        domainMap = HomGroupPresentation(domain_);
    if (! codomainMap)
        codomainMap = HomGroupPresentation(codomain_);
    std::vector< GroupExpression > newMap( domain_.countGenerators() );
    for (unsigned long i=0; i<newMap.size(); i++)
        newMap[i] = codomainMap->evaluate( evaluate( domainMap->invEvaluate(i) ) );
    std::vector< GroupExpression > newInvMap;
    if (inv_) {
        newInvMap.resize( codomain_.countGenerators() );
        for (unsigned long i=0; i<newInvMap.size(); i++)
            newInvMap[i] = domainMap->evaluate( invEvaluate(
                codomainMap->invEvaluate(i) ) );
    }

    map_ = std::move(newMap);
    for (GroupExpression& e : map_)
        retval |= codomain_.simplifyWord(e);

    if (inv_) {
        *inv_ = std::move(newInvMap);
        for (GroupExpression& e : *inv_)
            retval |= domain_.simplifyWord(e);
    }

    return retval;
}

HomGroupPresentation HomGroupPresentation::operator * (
        const HomGroupPresentation& input) const {
    std::vector<GroupExpression> evalVec(input.domain_.countGenerators());
    for (unsigned long i=0; i<evalVec.size(); i++)
        evalVec[i] = evaluate( input.evaluate(i) );
    if ( (! inv_) || (! input.inv_) ) {
        return HomGroupPresentation(input.domain_, codomain_, evalVec);
    } else {
        std::vector<GroupExpression> invVec( codomain_.countGenerators());
        for (unsigned long i=0; i<invVec.size(); i++)
            invVec[i] = input.invEvaluate( invEvaluate(i) );
        return HomGroupPresentation(input.domain_, codomain_, evalVec, invVec );
    }
}

HomGroupPresentation HomGroupPresentation::operator * (
        HomGroupPresentation&& input) const {
    std::vector<GroupExpression> evalVec(input.domain_.countGenerators());
    for (unsigned long i=0; i<evalVec.size(); i++)
        evalVec[i] = evaluate( input.evaluate(i) );
    if ( (! inv_) || (! input.inv_) ) {
        return HomGroupPresentation(std::move(input.domain_), codomain_, evalVec);
    } else {
        std::vector<GroupExpression> invVec( codomain_.countGenerators());
        for (unsigned long i=0; i<invVec.size(); i++)
            invVec[i] = input.invEvaluate( invEvaluate(i) );
        return HomGroupPresentation(std::move(input.domain_), codomain_,
            evalVec, invVec );
    }
}

bool HomGroupPresentation::intelligentNielsen()
{ // modelled on intelligentSimplify
 auto codomainMap = codomain_.intelligentNielsen();
 auto domainMap = domain_.intelligentNielsen();
 bool retval = codomainMap || domainMap;
 if (! domainMap)
    domainMap = HomGroupPresentation(domain_);
 if (! codomainMap)
    codomainMap = HomGroupPresentation(codomain_);
 std::vector< GroupExpression > newMap( domain_.countGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i] = codomainMap->evaluate( evaluate( domainMap->invEvaluate(i) ) );
 std::vector< GroupExpression > newInvMap;
 if (inv_) {
     newInvMap.resize( codomain_.countGenerators() );
     for (unsigned long i=0; i<newInvMap.size(); i++)
       newInvMap[i] = domainMap->evaluate( invEvaluate(
          codomainMap->invEvaluate(i) ) );
 }

 map_ = std::move(newMap);
 for (GroupExpression& e : map_)
     retval |= codomain_.simplifyWord(e);

 if (inv_) {
     *inv_ = std::move(newInvMap);
     for (GroupExpression& e : *inv_)
         retval |= domain_.simplifyWord(e);
 }

 return retval;
}

bool HomGroupPresentation::intelligentSimplify()
{
 // step 1: simplify presentation of domain and codomain
 auto codomainMap = codomain_.intelligentSimplify();
 auto domainMap = domain_.intelligentSimplify();
 bool retval = codomainMap || domainMap;

 // build identity maps if either of the above is null.
 if (! domainMap)
    domainMap = HomGroupPresentation(domain_);
 if (! codomainMap)
    codomainMap = HomGroupPresentation(codomain_);

 // step 2: compute codomainMap*(*oldthis)*domainMap.inverse()
 //         and replace "map" appropriately.  Simplify the words in the codomain.
 //         Do the same for the inverse map if we have one.
 std::vector< GroupExpression > newMap( domain_.countGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i] = codomainMap->evaluate( evaluate( domainMap->invEvaluate(i) ) );
 std::vector< GroupExpression > newInvMap;
 if (inv_) {
     newInvMap.resize( codomain_.countGenerators() );
     for (unsigned long i=0; i<newInvMap.size(); i++)
       newInvMap[i] = domainMap->evaluate(
          invEvaluate( codomainMap->invEvaluate(i) ) );
 }

 // step 3: rewrite this map, and simplify
 map_ = std::move(newMap);
 for (GroupExpression& e : map_)
     retval |= codomain_.simplifyWord(e);

 if (inv_) {
     *inv_ = std::move(newInvMap);
     for (GroupExpression& e : *inv_)
         retval |= domain_.simplifyWord(e);
 }

 return retval;
}

bool HomGroupPresentation::invert() {
    if (inv_) {
        domain_.swap(codomain_);
        map_.swap(*inv_);
        return true;
    }
    return false;
}

bool HomGroupPresentation::verify() const {
    for (const auto& r : domain_.relations()) {
        GroupExpression imgRel( evaluate(r) );
        codomain_.simplifyWord(imgRel);
        if (!imgRel.isTrivial())
            return false;
    }
    return true;
}

bool HomGroupPresentation::verifyIsomorphism() const
{
 if (! inv_) return false;

 if (inv_->size() != codomain_.countGenerators()) return false;
 // for every generator in the domain compute f^-1(f(x))x^-1 and reduce
 for (unsigned long i=0; i<domain_.countGenerators(); i++)
  {
   GroupExpression tempW( invEvaluate(evaluate(i)) );
   tempW.addTermLast( i, -1 );
   domain_.simplifyWord(tempW);
   if (tempW.countTerms()>0) return false;
  }
 // for every generator in the codomain compute f(f^-1(x))x^-1 and reduce
 for (unsigned long i=0; i<codomain_.countGenerators(); i++)
  {
   GroupExpression tempW( evaluate(invEvaluate(i)) );
   tempW.addTermLast( i, -1 );
   codomain_.simplifyWord(tempW);
   if (tempW.countTerms()>0) return false;
  }
 return true;
}



} // namespace regina

