
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

#include <iterator>
#include <sstream>
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "maths/numbertheory.h"

namespace regina {

HomGroupPresentation::HomGroupPresentation(
            const GroupPresentation& groupForIdentity) :
        domain_(new GroupPresentation(groupForIdentity)),
        range_(new GroupPresentation(groupForIdentity)),
        map_(groupForIdentity.countGenerators()),
        inv_(new std::vector<GroupExpression*>(
            groupForIdentity.countGenerators())) {
    for (unsigned long i=0; i<map_.size(); i++) {
        map_[i] = new GroupExpression();
        (*inv_)[i] = new GroupExpression();
        map_[i]->addTermFirst(i, 1);
        (*inv_)[i]->addTermFirst(i,1);
    }
}

GroupExpression HomGroupPresentation::evaluate(
                        const GroupExpression &arg) const
 { // evaluate at arg
   GroupExpression retval(arg);
   unsigned long N( range_->countGenerators() );
   for (unsigned long i=0; i<retval.countTerms(); i++)
       retval.term(i).generator += N;
   for (unsigned long i=0; i<map_.size(); i++)
       retval.substitute( N+i, *map_[i] );
   return retval;
 }

GroupExpression HomGroupPresentation::invEvaluate(
                    const GroupExpression &arg) const
{
   GroupExpression retval(arg);
   unsigned long N( domain_->countGenerators() );
   for (unsigned long i=0; i<retval.countTerms(); i++)
       retval.term(i).generator += N;
   for (unsigned long i=0; i<inv_->size(); i++)
       retval.substitute( N+i, *(*inv_)[i] );
   return retval;

}

std::unique_ptr< HomMarkedAbelianGroup >
    HomGroupPresentation::markedAbelianisation() const
{
 std::unique_ptr<MarkedAbelianGroup> DOM( domain_->markedAbelianisation() );
 std::unique_ptr<MarkedAbelianGroup> RAN( range_->markedAbelianisation() );
 MatrixInt ccMat( RAN->rankCC(), DOM->rankCC() );
 for (unsigned long j=0; j<ccMat.columns(); j++)
  {
   GroupExpression COLj( evaluate(j) );
   for (unsigned long i=0; i<COLj.countTerms(); i++)
    ccMat.entry( COLj.generator(i), j ) += COLj.exponent(i);
  }
 return std::unique_ptr<HomMarkedAbelianGroup>(
        new HomMarkedAbelianGroup( *DOM, *RAN, ccMat ) );
}

void HomGroupPresentation::writeTextShort(std::ostream& out) const {
    if (inv_)
     out << "Isomorphism from ";
    else
     out << "Homomorphism from ";
    domain_->writeTextShort(out);
    out << " to ";
    range_->writeTextShort(out);
}

void HomGroupPresentation::writeTextLong(std::ostream& out) const
{
    if (inv_)
     out << "Isomorphism with ";
    else
     out << "Homomorphism with ";
    out<<"domain ";
    domain_->writeTextCompact(out);
    out<<" "; // std::endl;

    out<<"map[";
    for (unsigned long i=0; i<domain_->countGenerators(); i++) {
        if (i!=0) out<<", ";
        if (domain_->countGenerators()<=26) out<<char('a' + i)<<" --> ";
        else  out<<"g"<<i<<" --> ";
        if (range_->countGenerators()<=26) map_[i]->writeText(out, true);
        else map_[i]->writeText(out, false);
    }
    out<<"] ";

    out<<"range ";
    range_->writeTextCompact(out);
    out<<std::endl;
}

bool HomGroupPresentation::smallCancellation()
{
 std::unique_ptr<regina::HomGroupPresentation> rangeMap =
    range_->smallCancellationDetail();
 std::unique_ptr<regina::HomGroupPresentation> domainMap =
    domain_->smallCancellationDetail();
 if (! domainMap.get())
    domainMap.reset(new HomGroupPresentation(*domain_));
 if (! rangeMap.get())
    rangeMap.reset(new HomGroupPresentation(*range_));
 GroupPresentation *oldDom(domainMap->domain_), *oldRan(rangeMap->domain_),
                    *newDom(domain_), *newRan(range_);
 domain_ = oldDom; range_ = oldRan;// we need to call this->evaluate but our map
 bool retval = rangeMap.get() || domainMap.get();
 std::vector< GroupExpression > newMap( newDom->countGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i].addTermsLast( rangeMap->evaluate(
    evaluate( domainMap->invEvaluate(i) ) ) );
 std::vector< GroupExpression > newInvMap;
 if (inv_) {
     newInvMap.resize( newRan->countGenerators() );
     for (unsigned long i=0; i<newInvMap.size(); i++)
       newInvMap[i].addTermsLast( domainMap->evaluate(
         invEvaluate( rangeMap->invEvaluate(i) ) ) );
 }
 domain_ = newDom; range_ = newRan;
 for (unsigned long i=0; i<map_.size(); i++) delete map_[i];
 map_.resize( newMap.size() );
 if (inv_) {
     for (unsigned long i=0; i<inv_->size(); i++) delete (*inv_)[i];
     inv_->resize( newInvMap.size() );
 }

 for (unsigned long i=0; i<map_.size(); i++)
       {
        map_[i] = new GroupExpression(newMap[i]);
        range_->simplifyWord(*map_[i]);
       }
 if (inv_)
     for (unsigned long i=0; i<inv_->size(); i++)
           {
            (*inv_)[i] = new GroupExpression(newInvMap[i]);
            domain_->simplifyWord(*(*inv_)[i]);
           }

 return retval;
}

std::unique_ptr<HomGroupPresentation> HomGroupPresentation::composeWith(
            const HomGroupPresentation& input) const
{
 std::vector<GroupExpression> evalVec(input.domain_->countGenerators());
 for (unsigned long i=0; i<evalVec.size(); i++)
  evalVec[i] = evaluate( input.evaluate(i) );
 if ( (! inv_) || (! input.inv_) )
  return std::unique_ptr<HomGroupPresentation>(new HomGroupPresentation(
    *input.domain_, *range_, evalVec) );
 else
  {
    std::vector<GroupExpression> invVec( range_->countGenerators());
    for (unsigned long i=0; i<invVec.size(); i++)
     invVec[i] = input.invEvaluate( invEvaluate(i) );
    return std::unique_ptr<HomGroupPresentation>(new HomGroupPresentation(
        *input.domain_, *range_, evalVec, invVec ) );
  }
}



bool HomGroupPresentation::intelligentNielsen()
{ // modelled on intelligentSimplify
 std::unique_ptr<regina::HomGroupPresentation> rangeMap =
    range_->intelligentNielsenDetail();
 std::unique_ptr<regina::HomGroupPresentation> domainMap =
    domain_->intelligentNielsenDetail();
 if (! domainMap.get())
    domainMap.reset(new HomGroupPresentation(*domain_));
 if (! rangeMap.get())
    rangeMap.reset(new HomGroupPresentation(*range_));
 GroupPresentation *oldDom(domainMap->domain_), *oldRan(rangeMap->domain_),
                    *newDom(domain_), *newRan(range_);
 domain_ = oldDom; range_ = oldRan;// we need to call this->evaluate but our map
 bool retval = rangeMap.get() || domainMap.get();
 std::vector< GroupExpression > newMap( newDom->countGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i].addTermsLast( rangeMap->evaluate(
     evaluate( domainMap->invEvaluate(i) ) ) );
 std::vector< GroupExpression > newInvMap;
 if (inv_) {
     newInvMap.resize( newRan->countGenerators() );
     for (unsigned long i=0; i<newInvMap.size(); i++)
       newInvMap[i].addTermsLast( domainMap->evaluate( invEvaluate(
          rangeMap->invEvaluate(i) ) ) );
 }
 domain_ = newDom; range_ = newRan;
 for (unsigned long i=0; i<map_.size(); i++) delete map_[i];
 map_.resize( newMap.size() );
 if (inv_) {
     for (unsigned long i=0; i<inv_->size(); i++) delete (*inv_)[i];
     inv_->resize( newInvMap.size() );
 }

 for (unsigned long i=0; i<map_.size(); i++)
       {
        map_[i] = new GroupExpression(newMap[i]);
        range_->simplifyWord(*map_[i]);
       }
 if (inv_)
     for (unsigned long i=0; i<inv_->size(); i++)
           {
            (*inv_)[i] = new GroupExpression(newInvMap[i]);
            domain_->simplifyWord(*(*inv_)[i]);
           }

 return retval;
}

bool HomGroupPresentation::intelligentSimplify()
{
 // step 1: simplify presentation of domain and range
 std::unique_ptr<regina::HomGroupPresentation> rangeMap =
    range_->intelligentSimplifyDetail();
 std::unique_ptr<regina::HomGroupPresentation> domainMap =
    domain_->intelligentSimplifyDetail();
 // build identity maps if either of the above is null.
 if (! domainMap.get())
    domainMap.reset(new HomGroupPresentation(*domain_));
 if (! rangeMap.get())
    rangeMap.reset(new HomGroupPresentation(*range_));

 GroupPresentation *oldDom(domainMap->domain_), *oldRan(rangeMap->domain_),
                    *newDom(domain_), *newRan(range_);

 domain_ = oldDom; range_ = oldRan;// we need to call this->evaluate but our map

 // step 2: compute rangeMap*(*oldthis)*domainMap.inverse()
 //         and replace "map" appropriately.  Simplify the words in the range.
 //         Do the same for the inverse map if we have one.
 bool retval = rangeMap.get() || domainMap.get();
 std::vector< GroupExpression > newMap( newDom->countGenerators() );
 for (unsigned long i=0; i<newMap.size(); i++)
  newMap[i].addTermsLast( rangeMap->evaluate(
     evaluate( domainMap->invEvaluate(i) ) ) );
 std::vector< GroupExpression > newInvMap;
 if (inv_) {
     newInvMap.resize( newRan->countGenerators() );
     for (unsigned long i=0; i<newInvMap.size(); i++)
       newInvMap[i].addTermsLast( domainMap->evaluate(
          invEvaluate( rangeMap->invEvaluate(i) ) ) );
 }
 domain_ = newDom; range_ = newRan;

 // step 3: rewrite this map, and simplify
 for (unsigned long i=0; i<map_.size(); i++) delete map_[i];
 map_.resize( newMap.size() );
 if (inv_) {
     for (unsigned long i=0; i<inv_->size(); i++) delete (*inv_)[i];
     inv_->resize( newInvMap.size() );
 }

 for (unsigned long i=0; i<map_.size(); i++)
       {
        map_[i] = new GroupExpression(newMap[i]);
        range_->simplifyWord(*map_[i]);
       }
 if (inv_)
     for (unsigned long i=0; i<inv_->size(); i++)
           {
            (*inv_)[i] = new GroupExpression(newInvMap[i]);
            domain_->simplifyWord(*(*inv_)[i]);
           }

 return retval;
}

bool HomGroupPresentation::invert()
{
 if (inv_)
  {
   GroupPresentation* temp( domain_ );
   domain_ = range_; range_ = temp;
   map_.swap( *inv_ );
   return true;
  }
 return false;
}

bool HomGroupPresentation::verify() const
{
 for (unsigned long i=0; i<domain_->countRelations(); i++)
  {
   const GroupExpression& reli( domain_->relation(i) );
   GroupExpression imgRel( evaluate(reli) );
   range_->simplifyWord(imgRel);
   if (!imgRel.isTrivial()) return false;
  }
 return true;
}

bool HomGroupPresentation::verifyIsomorphism() const
{
 if (! inv_) return false;

 if (inv_->size() != range_->countGenerators()) return false;
 // for every generator in the domain compute f^-1(f(x))x^-1 and reduce
 for (unsigned long i=0; i<domain_->countGenerators(); i++)
  {
   GroupExpression tempW( invEvaluate(evaluate(i)) );
   tempW.addTermLast( i, -1 );
   domain_->simplifyWord(tempW);
   if (tempW.countTerms()>0) return false;
  }
 // for every generator in the range compute f(f^-1(x))x^-1 and reduce
 for (unsigned long i=0; i<range_->countGenerators(); i++)
  {
   GroupExpression tempW( evaluate(invEvaluate(i)) );
   tempW.addTermLast( i, -1 );
   range_->simplifyWord(tempW);
   if (tempW.countTerms()>0) return false;
  }
 return true;
}



} // namespace regina

