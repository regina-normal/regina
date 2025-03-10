
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "algebra/markedabeliangroup.h"
#include "maths/matrixops.h"
#include "utilities/stringutils.h"
#include <iostream>

namespace regina {

MarkedAbelianGroup::MarkedAbelianGroup(size_t rk, const Integer &p) :
        M_(rk, rk), N_(rk,rk), MR_(rk,rk), MRi_(rk, rk),
        rankM_(0),
        presR_(rk, rk), presC_(rk, rk), presCi_(rk, rk),
        snfFreeRank_(0), snfFreeIndex_(0), ifLoc_(0),
        coeff_(0), tensorIfLoc_(0) {
    // special case p==1 trivial group
    for (size_t i=0; i<rk; i++)
        N_.entry(i,i) = p;
    // everything is already in SNF, so these are identity matrices
    MR_.makeIdentity();
    MRi_.makeIdentity();
    presR_.makeIdentity();
    presC_.makeIdentity(); presCi_.makeIdentity();
    if (p == 0) {
        snfFreeRank_ = rk;
    } else if (p != 1 && rk != 0) {
        invFac_.resize(rk);
        for (auto& inv : invFac_)
            inv = p;
    }
}

MarkedAbelianGroup::MarkedAbelianGroup(MatrixInt tmpM, MatrixInt tmpN) :
        M_(std::move(tmpM)), N_(std::move(tmpN)),
        rankM_(0), snfFreeRank_(0), snfFreeIndex_(0), ifLoc_(0),
        coeff_(0), tensorIfLoc_(0) {
    if (M_.columns() != N_.rows())
        throw InvalidArgument("The chain complex constructor requires "
            "M.columns() == N.rows()");

    MatrixInt tM = M_;
    MatrixInt MC, MCi;
    metricalSmithNormalForm(tM, MR_, MRi_, MC, MCi);

    for (size_t i=0; (i<tM.rows()) && (i<tM.columns()); i++)
        if (tM.entry(i,i) != 0)
            ++rankM_;

    // construct the internal presentation matrix.
    MatrixInt prod = MRi_ * N_;
    MatrixInt pres(N_.rows()-rankM_, N_.columns());

    for (size_t i=0;i<pres.rows();i++)
        for (size_t j=0;j<pres.columns();j++)
            pres.entry(i,j) = prod.entry(i+rankM_,j);

    // put the presentation matrix in Smith normal form, and
    // build the list of invariant factors and their row indexes
    // now compute the rank and column indexes ...

    MatrixInt presRi;
    metricalSmithNormalForm(pres, presR_, presRi, presC_, presCi_);

    for (size_t i=0; ( (i<pres.rows()) && (i<pres.columns()) ); i++) {
        if (pres.entry(i,i)==1)
            ifLoc_++;
        else if (pres.entry(i,i)>1)
            invFac_.push_back(pres.entry(i,i));
    }

    snfFreeIndex_ = ifLoc_ + invFac_.size();
    snfFreeRank_ = pres.rows() - snfFreeIndex_;
}

// We'll store H_k(M;Z_p) internally in two different ways.  The reason boils
// down to the pleasant version of the universal coefficient theorem that you
// see using Smith normal forms. Summary:  if Z^a --N--> Z^b --M--> Z^c is a
// chain complex for H_k(M;Z) and p>0 an integer, put M into SNF(M), this gives
// SNF(M) == MC*M*MR_. Let SNF(M) == diag[s_0, ..., s_{k-1}, 0, ... 0] and
// suppose only entries s_i, ..., s_{k-1} share common factors with p.   Then
// you immediately get the presentation matrix for H_k(M;Z_p) which is a
// product of the two matrices: [ trunc_top_k_rows[MRi_*N], diag(p,p,...,p) ]
//  \times [ diag(gcd(s_i,p), ..., gcd(s_{k-1},p) ] here trunc_top_k_rows means
// remove the first k rows from [MRi_*N].  The left matrix is basically by
// design the presentation matrix for H_k(M;Z)\times Z_p, and the right matrix
// the presentation matrix for TOR(H_{k-1}(M;Z), Z_p).   The 2nd matrix is
// already in SNF.  We apply SNF to the first, and store the change-of-basis
// matrices in tensorR_, tensorC_, tensorRi, tensorCi_.
// We then concatenate these two diagonal
// matrices and apply SNF to them to get a situation MarkedAbelianGroup will
// be happy with. This has the added advantage of us being able to later easily
// implement the HomMarkedAbelianGroup maps for UCT later when we're interested
// in that kind of stuff.
MarkedAbelianGroup::MarkedAbelianGroup(MatrixInt tmpM, MatrixInt tmpN,
        Integer pcoeff):
        M_(std::move(tmpM)), N_(std::move(tmpN)),
        rankM_(0), snfFreeRank_(0), snfFreeIndex_(0),
        ifLoc_(0), coeff_(std::move(pcoeff)),
        tensorIfLoc_(0) {
    if (M_.columns() != N_.rows())
        throw InvalidArgument("The chain complex constructor requires "
            "M.columns() == N.rows()");

    // find SNF(M).
    MatrixInt tM = M_;
    MatrixInt MC, MCi;
    metricalSmithNormalForm(tM, MR_, MRi_, MC, MCi);

    for (size_t i=0; ( (i<tM.rows()) && (i<tM.columns()) ); i++)
        if (tM.entry(i,i) != 0) rankM_++;

    // in the case coeff_ > 0 we need to consider the TOR part of homology.

    if (coeff_ > 0)
        for (size_t i=0; i<rankM_; i++)
            if (tM.entry(i,i).gcd(coeff_) > 1)
                TORVec_.push_back(tM.entry(i,i));

    // okay, lets get a presentation matrix for H_*(M;Z) \otimes Z_p
    // starting by computing the trunc[MRi_*N] matrix and padding with
    // a diagonal p matrix

    MatrixInt MRi_N = MRi_ * N_;

    // hmm, if we're using p == 0 coefficients, lets keep it simple
    if (coeff_ > 0) {
        MatrixInt tensorPres( MRi_N.rows() - rankM_,
                   MRi_N.columns() + MRi_N.rows() - rankM_ );
        for (size_t i=0; i<tensorPres.rows(); i++)
            for (size_t j=0; j<MRi_N.columns(); j++)
                 tensorPres.entry(i,j) = MRi_N.entry(i+rankM_, j);
        for (size_t i=0; i< MRi_N.rows() - rankM_; i++)
             tensorPres.entry(i, MRi_N.columns() + i) = coeff_;

        MatrixInt tensorRi;
        metricalSmithNormalForm(tensorPres, tensorR_, tensorRi,
            tensorC_, tensorCi_);

        // this group is a direct sum of groups of the form Z_q where q =
        // gcd(p, TORVec_[i]), and groups Z_q where q is on the diagonal of
        // tensorPres, and either q=0 or q>1. unfortunately in rare occurances
        // these are not the invariant factors of the group, so we assemble
        // these numbers into a diagonal presentation matrix and apply SNF!
        // Determine the size of the matrix we'll need.
        for (size_t i=0; i<tensorPres.rows() && i<tensorPres.columns(); ++i) {
            if (tensorPres.entry(i,i) == 1)
                tensorIfLoc_++;
            else if (tensorPres.entry(i,i) > 1)
                tensorInvFac_.push_back(tensorPres.entry(i,i));
            else if (tensorPres.entry(i,i) == 0)
                snfFreeRank_++; // should always be zero.
        }
        size_t tensorIfNum = tensorInvFac_.size();

        MatrixInt diagPres( TORVec_.size() + tensorIfNum + snfFreeRank_,
                TORVec_.size() + tensorIfNum + snfFreeRank_);
        for (size_t i=0; i<diagPres.rows(); i++) {
            if (i<TORVec_.size())
                diagPres.entry(i,i) = TORVec_[i].gcd(coeff_);
            else
                diagPres.entry(i,i) = tensorPres.entry(
                    i - TORVec_.size() + tensorIfLoc_,
                    i - TORVec_.size() + tensorIfLoc_);
        }

        MatrixInt presRi;
        metricalSmithNormalForm(diagPres, presR_, presRi, presC_, presCi_);
        for (size_t i=0; i<diagPres.rows(); i++) {
            // should only have terms > 1 or == 0.
            if (diagPres.entry(i,i) > 1)
                invFac_.push_back(diagPres.entry(i,i));
        }
        snfFreeIndex_ = invFac_.size();
        ifLoc_ = diagPres.rows() - invFac_.size();
    } else {
        // coeff_ == p == 0 case
        MatrixInt tensorPres( MRi_N.rows() - rankM_, MRi_N.columns() );
        for (size_t i=0; i<tensorPres.rows(); i++)
            for (size_t j=0; j<MRi_N.columns(); j++)
                tensorPres.entry(i,j) = MRi_N.entry(i+rankM_, j);

        MatrixInt presRi;
        metricalSmithNormalForm(tensorPres, presR_, presRi, presC_, presCi_);

        for (size_t i=0; i<tensorPres.rows() && i<tensorPres.columns();
                ++i) {
            if (tensorPres.entry(i,i) == 1)
                ifLoc_++;
            else if (tensorPres.entry(i,i) > 1)
                invFac_.push_back(tensorPres.entry(i,i));
        }
        snfFreeIndex_ = ifLoc_ + invFac_.size();
        snfFreeRank_ = tensorPres.rows() - snfFreeIndex_;
    }
}

void MarkedAbelianGroup::swap(MarkedAbelianGroup& other) noexcept {
    // Woof.
    M_.swap(other.M_);
    N_.swap(other.N_);
    MR_.swap(other.MR_);
    MRi_.swap(other.MRi_);
    std::swap(rankM_, other.rankM_);
    presR_.swap(other.presR_);
    presC_.swap(other.presC_);
    presCi_.swap(other.presCi_);
    tensorR_.swap(other.tensorR_);
    tensorC_.swap(other.tensorC_);
    tensorCi_.swap(other.tensorCi_);
    invFac_.swap(other.invFac_);
    std::swap(snfFreeRank_, other.snfFreeRank_);
    std::swap(snfFreeIndex_, other.snfFreeIndex_);
    std::swap(ifLoc_, other.ifLoc_);
    coeff_.swap(other.coeff_);
    TORVec_.swap(other.TORVec_);
    std::swap(tensorIfLoc_, other.tensorIfLoc_);
    tensorInvFac_.swap(other.tensorInvFac_);
}

size_t MarkedAbelianGroup::torsionRank(const Integer& degree) const {
    size_t ans = 0;
    for (const auto& inv : invFac_) {
        if (inv % degree == 0)
            ++ans;
    }
    return ans;
}


void MarkedAbelianGroup::writeTextShort(std::ostream& out, bool utf8) const {
    bool writtenSomething = false;

    if (snfFreeRank_ > 0) {
        if (snfFreeRank_ > 1)
            out << snfFreeRank_ << ' ';
        if (utf8)
            out << "\u2124";
        else
            out << 'Z';
        writtenSomething = true;
    }

    auto it = invFac_.begin();
    Integer currDegree;
    size_t currMult = 0;
    while(true) {
        if (it != invFac_.end()) {
            if ((*it) == currDegree) {
                currMult++;
                it++;
                continue;
            }
        }
        if (currMult > 0) {
            if (writtenSomething)
                out << " + ";
            if (currMult > 1)
                out << currMult << ' ';
            if (utf8)
                out << "\u2124" << regina::subscript(currDegree);
            else
                out << "Z_" << currDegree.stringValue();
            writtenSomething = true;
        }
        if (it == invFac_.end())
            break;
        currDegree = *it;
        currMult = 1;
        it++;
    }

    if (! writtenSomething)
        out << '0';

    if (utf8) {
        out << " (\u2124" << superscript(N_.columns())
            << " \u2192 \u2124" << superscript(N_.rows())
            << " \u2192 \u2124" << superscript(M_.rows());
        if (coeff_ != 0)
            out << " with \u2124" << subscript(coeff_) << " coefficients";
        out << ')';
    } else {
        out << " (Z^" << N_.columns()
            << " -> Z^" << N_.rows()
            << " -> Z^" << M_.rows();
        if (coeff_ != 0)
            out << " with Z_" << coeff_ << " coefficients";
        out << ')';
    }
}

/*
 * The marked abelian group was defined by matrices M and N
 * with M×N==0.  Think of M as m by l and N as l by n.  Then
 * this routine returns the index-th free generator of the
 * ker(M)/img(N) in Z^l.
 */
Vector<Integer> MarkedAbelianGroup::freeRep(size_t index) const {
    if (index >= snfFreeRank_)
        throw InvalidArgument("freeRep(): invalid index");

    Vector<Integer> retval(M_.columns());
    // index corresponds to the (index+snfFreeIndex_)-th column of presCi_
    // we then pad this vector (at the front) with rankM_ 0's
    // and apply MR_ to it.

    Vector<Integer> temp(presCi_.rows()+rankM_);
    for (size_t i=0;i<presCi_.rows();i++)
        temp[i+rankM_]=presCi_.entry(i,index+snfFreeIndex_);
    // the above line takes the index+snfFreeIndex_-th column of presCi_ and
    // pads it.
    return MR_ * temp;
}

/*
 * The marked abelian group was defined by matrices M and N
 * with M×N==0.  Think of M as m by l and N as l by n.  Then
 * this routine returns the index-th torsion generator of the
 * ker(M)/img(N) in Z^l.
 */
Vector<Integer> MarkedAbelianGroup::torsionRep(size_t index) const {
    if (index >= invFac_.size())
        throw InvalidArgument("torsionRep(): invalid index");

    if (coeff_ == 0) {
        Vector<Integer> temp(presCi_.rows()+rankM_);
        for (size_t i=0;i<presCi_.rows();i++)
            temp[i+rankM_] = presCi_.entry(i,ifLoc_+index);
        // the above line takes the index+snfFreeIndex_-th column of presCi_ and
        // pads it suitably
        return MR_ * temp;
    } else {
        // coeff_ > 0 with coefficients there's the extra step of dealing with
        // the UCT splitting start with column column index + ifLoc_ of matrix
        // presCi_, split into two vectors
        // 1) first TORVec_.size() entries and 2) remaining entries.
        Vector<Integer> retval(M_.columns());
        Vector<Integer> firstV(TORVec_.size());
        Vector<Integer> secondV(presC_.rows()-TORVec_.size());
        for (size_t i=0; i<firstV.size(); i++)
            firstV[i] = presCi_.entry( i, index + ifLoc_ );
        for (size_t i=0; i<secondV.size(); i++)
            secondV[i] = presCi_.entry( i + firstV.size(), index + ifLoc_ );
        // 1st vec needs coords scaled appropriately by p/gcd(p,q) and
        //  multiplied by appropriate MR_ columns
        for (size_t i=0; i<firstV.size(); i++)
            firstV[i] *= coeff_.divExact( TORVec_[i].gcd(coeff_) );
        Vector<Integer> tCiSecondV(tensorCi_.rows());
        for (size_t i=0; i<tensorCi_.rows(); i++)
            for (size_t j=tensorIfLoc_; j<tensorCi_.columns(); j++)
                tCiSecondV[i] += tensorCi_.entry(i,j) * secondV[j-tensorIfLoc_];
        // 2nd vec needs be multiplied by tensorCi_, padded, then apply MR_.
        size_t TORLoc = rankM_ - TORVec_.size();
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<firstV.size(); j++)
                retval[i] += MR_.entry(i, TORLoc + j)*firstV[j];
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<tCiSecondV.size(); j++)
                retval[i] += MR_.entry(i, rankM_+j) * tCiSecondV[j];
        // add answers together.
        return retval;
    }
}


Vector<Integer> MarkedAbelianGroup::ccRep(const Vector<Integer>& SNFRep) const {
    if (SNFRep.size() != snfFreeRank_ + invFac_.size())
        throw InvalidArgument(
            "The argument to ccRep() is a vector of the wrong size");

    if (coeff_ == 0) {
        Vector<Integer> temp(presCi_.rows()+rankM_);
        for (size_t j=0; j<invFac_.size()+snfFreeRank_; j++)
            for (size_t i=0; i<presCi_.rows(); i++)
                temp[i+rankM_] += presCi_.entry(i,ifLoc_+j) * SNFRep[j];
        return MR_ * temp;
    } else {
        // coeff_ > 0
        Vector<Integer> retval(M_.columns());
        Vector<Integer> firstV(TORVec_.size());
        Vector<Integer> secondV(presC_.rows()-TORVec_.size());
        for (size_t i=0; i<firstV.size(); i++)
            for (size_t j=0; j<SNFRep.size(); j++)
                firstV[i] += presCi_.entry( i, j + ifLoc_ ) * SNFRep[j];
        for (size_t i=0; i<secondV.size(); i++)
            for (size_t j=0; j<SNFRep.size(); j++)
                secondV[i] += presCi_.entry( i + firstV.size(), j + ifLoc_ ) * SNFRep[j];
        // 1st vec needs coords scaled appropriately by p/gcd(p,q) and
        //  multiplied by appropriate MR_ columns
        for (size_t i=0; i<firstV.size(); i++)
            firstV[i] *= coeff_.divExact( TORVec_[i].gcd(coeff_) );
        Vector<Integer> tCiSecondV(tensorCi_.rows());
        for (size_t i=0; i<tensorCi_.rows(); i++)
            for (size_t j=tensorIfLoc_; j<tensorCi_.columns(); j++)
                tCiSecondV[i] += tensorCi_.entry(i,j) * secondV[j-tensorIfLoc_];
        // 2nd vec needs be multiplied by tensorCi_, padded, then apply MR_.
        size_t TORLoc = rankM_ - TORVec_.size();
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<firstV.size(); j++)
                retval[i] += MR_.entry(i, TORLoc + j)*firstV[j];
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<tCiSecondV.size(); j++)
                retval[i] += MR_.entry(i, rankM_+j) * tCiSecondV[j];
        return retval;
    }
}

Vector<Integer> MarkedAbelianGroup::ccRep(size_t SNFRep) const {
    if (SNFRep >= snfFreeRank_ + invFac_.size())
        throw InvalidArgument("The argument to ccRep() must be "
            "less than the number of SNF generators");

    if (coeff_ == 0) {
        Vector<Integer> temp(presCi_.rows()+rankM_);
        for (size_t i=0; i<presCi_.rows(); i++)
            temp[i+rankM_] = presCi_.entry(i,ifLoc_+SNFRep);
        return MR_ * temp;
    } else {
        // coeff_ > 0
        Vector<Integer> retval(M_.columns());
        Vector<Integer> firstV(TORVec_.size());
        Vector<Integer> secondV(presC_.rows()-TORVec_.size());
        for (size_t i=0; i<firstV.size(); i++)
            firstV[i] = presCi_.entry( i, SNFRep + ifLoc_ );
        for (size_t i=0; i<secondV.size(); i++)
            secondV[i] = presCi_.entry( i + firstV.size(), SNFRep + ifLoc_ );
        // 1st vec needs coords scaled appropriately by p/gcd(p,q)
        //  and multiplied by appropriate MR_ columns
        for (size_t i=0; i<firstV.size(); i++)
            firstV[i] *= coeff_.divExact( TORVec_[i].gcd(coeff_) );
        Vector<Integer> tCiSecondV(tensorCi_.rows());
        for (size_t i=0; i<tensorCi_.rows(); i++)
            for (size_t j=tensorIfLoc_; j<tensorCi_.columns(); j++)
                tCiSecondV[i] += tensorCi_.entry(i,j) * secondV[j-tensorIfLoc_];
        // 2nd vec needs be multiplied by tensorCi_, padded, then apply MR_.
        size_t TORLoc = rankM_ - TORVec_.size();
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<firstV.size(); j++)
                retval[i] += MR_.entry(i, TORLoc + j)*firstV[j];
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<tCiSecondV.size(); j++)
                retval[i] += MR_.entry(i, rankM_+j) * tCiSecondV[j];
        return retval;
    }
}


/*
 * The marked abelian group was defined by matrices M and N
 * with M×N==0.  Think of M as m by l and N as l by n.
 * When the group was initialized, it was computed to be isomorphic
 * to some Z_{d1} + ... + Z_{dk} + Z^d where d1 | d2 | ... | dk
 * this routine assumes element is in Z^l, and it returns a vector
 * of length d+k where the last d elements represent which class the
 * vector projects to in Z^d, and the first k elements represent the
 * projections to Z_{d1} + ... + Z_{dk}. Of these last elements, they
 * will be returned mod di respectively. Returns an empty vector if
 * element is not in the kernel of M. element is assumed to have
 * M_.columns()==N_.rows() entries.
 */
Vector<Integer> MarkedAbelianGroup::snfRep(const Vector<Integer>& element)
        const {
    // apply MRi_, crop, then apply presC_, tidy up and return.
    // first, does element have the right dimensions? if not, abort.
    if (element.size() != M_.columns())
        throw InvalidArgument(
            "The argument to snfRep() is a vector of the wrong size");

    // this holds MRi_ * element which we will use to check first if
    // element is in the kernel, and then to construct its SNF rep.
    Vector<Integer> temp = MRi_ * element;

    // make judgement on if in kernel.
    // needs to be tweaked for mod p coefficients.
    if (coeff_ == 0) {
        for (size_t i=0;i<rankM_;i++)
            if (temp[i] != 0)
                throw InvalidArgument(
                    "The argument to snfRep() is not in the kernel");
    } else {
        size_t TORLoc = rankM_ - TORVec_.size();

        // the first TORLoc-1 terms of tM were units mod p so we need only
        // check divisibility by p for temp[i] the remaining terms of tM were
        // given by TORVec_[i-TORLoc] and share a common factor with p==coeff_.
        // for element to be in ker(M), we need
        // temp[i]*TORVec_[i-TORLoc] % p == 0
        for (size_t i=0; i<rankM_; i++) {
            if (i<TORLoc) {
                if (temp[i] % coeff_ != 0)
                    throw InvalidArgument(
                        "The argument to snfRep() is not in the kernel");
            } else {
                if ((temp[i]*TORVec_[i-TORLoc]) % coeff_ != 0)
                    throw InvalidArgument(
                        "The argument to snfRep() is not in the kernel");
                temp[i] = temp[i].divExact(coeff_.divExact(
                    coeff_.gcd(TORVec_[i-TORLoc])));
            }
        }
    }

    // The given vector is in the kernel.
    Vector<Integer> retval(snfFreeRank_+invFac_.size());

    if (coeff_ == 0) {
        for (size_t i=0;i<snfFreeRank_;i++)
            for (size_t j=rankM_;j<N_.rows();j++)
                retval[i+invFac_.size()] +=
                    presC_.entry(i+snfFreeIndex_,j-rankM_)*temp[j];
        for (size_t i=0;i<invFac_.size();i++)
            for (size_t j=rankM_;j<N_.rows();j++)
                retval[i] += presC_.entry(i+ifLoc_,j-rankM_)*temp[j];
        // redundant for loops
    } else {
        size_t TORLoc = rankM_ - TORVec_.size();
        Vector<Integer> diagPresV(presC_.rows());
        for (size_t i=0; i<diagPresV.size(); i++) {
            // TOR part
            if (i < TORVec_.size())
                diagPresV[i] = temp[ i + TORLoc ];
            else // tensor part
                for (size_t j=0; j<tensorC_.columns(); j++)
                    diagPresV[i] += tensorC_.entry( i - TORVec_.size() +
                        tensorIfLoc_, j) * temp[ j + rankM_ ];
        }
        // assemble to a diagPres vector, apply presC_
        retval = presC_ * diagPresV;
    }
    // do modular reduction to make things look nice...
    for (size_t i=0; i<invFac_.size(); i++) {
        retval[i] %= invFac_[i];
        if (retval[i] < 0)
            retval[i] += invFac_[i];
    }
    return retval;
}

bool MarkedAbelianGroup::isCycle(const Vector<Integer> &input) const {
    if (input.size() != M_.columns())
        return false;
    for (size_t i=0; i<M_.rows(); i++) {
        Integer T; // zero
        for (size_t j=0; j<M_.columns(); j++)
            T += input[j]*M_.entry(i,j);
        if (coeff_ == 0) {
            if (T != 0)
                return false;
        } else if ( (T % coeff_) != 0 )
            return false;
    }
    return true;
}

bool MarkedAbelianGroup::isBoundary(const Vector<Integer> &input) const {
    try {
        return snfRep(input).isZero();
    } catch (const InvalidArgument&) {
        // The input vector was the wrong size, or was not in the kernel.
        return false;
    }
}

Vector<Integer> MarkedAbelianGroup::boundaryOf(const Vector<Integer>& CCrep)
        const {
    if (CCrep.size() != M_.columns())
        throw InvalidArgument(
            "The argument to boundaryOf() is a vector of the wrong size");

    Vector<Integer> retval = M_ * CCrep;

    if (coeff_ > 0) {
        for (size_t i=0; i<M_.rows(); i++) {
            retval[i] %= coeff_;
            if (retval[i] < 0)
                retval[i] += coeff_;
        }
    }
    return retval;
}

// routine checks to see if an object in the CC coords for our group is a
// boundary, and if so it returns CC coords of what an object that it is a
// boundary of.  Null vector is returned if not boundary.
Vector<Integer> MarkedAbelianGroup::asBoundary(const Vector<Integer>& input)
        const {
    if ( !isCycle(input) )
        throw InvalidArgument("The argument to asBoundary() "
            "is not a cycle");
    // okay, it's a cycle so lets determine whether or not it is a boundary.
    Vector<Integer> temp = MRi_ * input;
    for (size_t i=0; i<TORVec_.size(); i++)
        if (temp[rankM_ - TORVec_.size() + i] % coeff_ != 0)
            throw InvalidArgument("The argument to asBoundary() "
                "is not a boundary");
    // now we know we're dealing with a cycle with zero TOR part (if coeff != 0)
    // convert into the diagPres coordinates / standard snfcoords if p==0.
    Vector<Integer> retval(N_.columns());
    if (coeff_ == 0) {
        Vector<Integer> snfV(presC_.rows());
        for (size_t i=0;i<presC_.rows();i++)
            for (size_t j=0;j<presC_.columns();j++)
                snfV[i] += presC_.entry(i,j)*temp[j+rankM_];
        // check divisibility in the invFac coords
        for (size_t i=0; i<invFac_.size(); i++) {
            if (snfV[i+ifLoc_] % invFac_[i] != 0)
                throw InvalidArgument("The argument to asBoundary() "
                    "is not a boundary");
            snfV[i+ifLoc_] /= invFac_[i];
        }
        // check that it's zero on coords missed by N...
        for (size_t i=0; i<snfFreeRank_; i++)
            if (snfV[i+snfFreeIndex_] != 0)
                throw InvalidArgument("The argument to asBoundary() "
                    "is not a boundary");
        // we know it's in the image now.
        for (size_t i=0; i<presR_.rows(); i++)
            for (size_t j=0; j<snfFreeIndex_; j++)
                retval[i] += presR_.entry(i, j) * snfV[j];
    } else {
        // find tensorV -- apply tensorC_.
        Vector<Integer> tensorV( tensorC_.rows());
        for (size_t i=0; i<tensorC_.rows(); i++)
            for (size_t j=0; j<tensorC_.columns(); j++)
                tensorV[i] += tensorC_.entry(i, j) * temp[ j + rankM_ ];
        for (size_t i=0; i<tensorInvFac_.size(); i++) {
            if (tensorV[i+tensorIfLoc_] % tensorInvFac_[i] != 0)
                throw InvalidArgument("The argument to asBoundary() "
                    "is not a boundary");
            tensorV[i+tensorIfLoc_] /= tensorInvFac_[i];
        }
        // so we know it's where it comes from now...
        for (size_t i=0; i<retval.size(); i++)
            for (size_t j=0; j<tensorV.size(); j++)
                retval[i] += tensorR_.entry(i,j) * tensorV[j];
        // ah! the other coefficients of tensorR_ gives the relevant congruence.
    }
    return retval;
}

// returns the j+TORLoc -th column of the matrix MR_, rescaled appropriately
//  if it corresponds to a TOR vector.
Vector<Integer> MarkedAbelianGroup::cycleGen(size_t j) const {
    if (j >= cycleRank())
        throw InvalidArgument("The argument to cycleGen() must be "
            "less than cycleRank()");
    Vector<Integer> retval(M_.columns());
    size_t TORLoc = rankM_ - TORVec_.size();
    for (size_t i=0; i<retval.size(); i++)
        retval[i] = MR_.entry(i, j+TORLoc);
    // if j < TORVec_.size() rescale by coeff_ / gcd(coeff_, TORVec_[j]
    if (j < TORVec_.size())
        for (auto& r : retval)
            r *= coeff_.divExact(coeff_.gcd(TORVec_[j]));
    return retval;
}

Vector<Integer> MarkedAbelianGroup::cycleProjection(
        const Vector<Integer>& ccelt) const {
    // multiply by MRi_, truncate, multiply by MR_
    if (ccelt.size() != MRi_.columns())
        throw InvalidArgument(
            "The argument to cycleProjection() is a vector of the wrong size");
    Vector<Integer> retval(MRi_.columns());
    for (size_t i=0; i<retval.size(); i++) {
        for (size_t j=rankM_; j<MRi_.rows(); j++)
            for (size_t k=0; k<ccelt.size(); k++)
                retval[i] += MR_.entry(i,j) * MRi_.entry(j,k) * ccelt[k];
    }
    return retval;
}

Vector<Integer> MarkedAbelianGroup::cycleProjection(size_t ccindx)
        const {
    // truncate column cyclenum of MRi_, multiply by MR_
    if (ccindx >= MRi_.columns())
        throw InvalidArgument("The argument to cycleProjection() must be "
            "less than the number of chain complex coordinates");
    Vector<Integer> retval(MRi_.columns());
    for (size_t i=0; i<retval.size(); i++)
        for (size_t j=rankM_; j<MRi_.rows(); j++)
            retval[i] += MR_.entry(i,j) * MRi_.entry(j,ccindx);
    return retval;
}

// the trivially presented torsion subgroup
MarkedAbelianGroup MarkedAbelianGroup::torsionSubgroup() const {
    MatrixInt dM(1, countInvariantFactors() );
    MatrixInt dN(countInvariantFactors(), countInvariantFactors() );
    for (size_t i=0; i<countInvariantFactors(); i++)
        dN.entry(i,i) = invariantFactor(i);
    return MarkedAbelianGroup(std::move(dM), std::move(dN));
}

// and its canonical inclusion map
HomMarkedAbelianGroup MarkedAbelianGroup::torsionInclusion() const {
    MatrixInt iM( ccRank(), countInvariantFactors() );
    for (size_t j=0; j<iM.columns(); j++) {
        Vector<Integer> jtor = torsionRep(j);
        for (size_t i=0; i<iM.rows(); i++)
            iM.entry(i,j) = jtor[i];
    }
    return HomMarkedAbelianGroup(torsionSubgroup(), *this, std::move(iM));
}


HomMarkedAbelianGroup::HomMarkedAbelianGroup(MatrixInt tmpRedMat,
        MarkedAbelianGroup tmpDom, MarkedAbelianGroup tmpRan) :
        domain_(std::move(tmpDom)), codomain_(std::move(tmpRan)),
        matrix(codomain_.M_.columns(), domain_.M_.columns()),
        reducedMatrix_(std::move(tmpRedMat)) {
    // If using mod p coeff, p != 0:
    //
    // we build up the CC map in reverse from the way we computed
    //  the structure of the domain/codomain groups.
    //  which was: 3) SNF(M,M'), truncate off first TORLoc coords.
    //             2) SNF the tensorPres matrix, TOR coords fixed.
    //                Truncate off first tensorIfLoc_ terms.
    //             1) SNF the combined matrix, truncate off ifLoc_ terms.
    //
    // Step 1: ran.presCi_*[incl redMat]*[trunc dom.presC_] puts us in
    //         diagPres coords ran.presCi_.rows()-by-dom.presC_.rows()
    // Step 2: ran.tensorCi_*(step 1)*[trunc dom.tensorC_] puts us in
    //         trunc(SNF(M,M')) coords
    // Step 3: MR_*(step 2)*[trunc MRi_]

    // If using integer coefficients:
    //
    // we build up the CC map in reverse of the process for which we found
    //  the structure of the domain/codomain
    // groups, which was:  2) SNF(M,M'), truncate off the first
    //                        rankM_==TORLoc coords
    //                     1) SNF(N,N'), truncate off the first ifLoc_ terms.
    //
    // Step 1: ran.presCi_*[incl redMat]*[trunc dom.presC_] puts us
    //         in trunc(SNF(M,M')) coords
    // Step 2: --void--
    // Step 3: MR_*(step 1)*[trunc MRi_]
    // so we have a common Step 1.
    MatrixInt step1Mat(codomain_.presCi_.rows(), domain_.presC_.rows());
    for (size_t i=0; i<step1Mat.rows(); i++)
        for (size_t j=0; j<step1Mat.columns(); j++) {
            // ran->presCi_.entry(i, k)*reducedMatrix_.entry(k, l)*dom->presC_.entry(l, j)
            for (size_t k=0; k<reducedMatrix_->rows(); k++)
                for (size_t l=0;l<reducedMatrix_->columns(); l++)
                    step1Mat.entry(i,j) += codomain_.presCi_.entry(
                        i,k+codomain_.ifLoc_)*reducedMatrix_->entry(k,l) *
                        domain_.presC_.entry(l+domain_.ifLoc_,j);
        }
    // with mod p coefficients we have this fiddly middle step 2.

    MatrixInt step2Mat( step1Mat.rows()+codomain_.tensorIfLoc_,
                         step1Mat.columns()+domain_.tensorIfLoc_ );
    // if coeff_==0, we'll just copy the step1Mat, if coeff_>0 we multiply
    // the tensor part by ran.tensorCi_, dom.tensorC_ resp.
    if (domain_.coeff_ == 0)
        for (size_t i=0; i<step2Mat.rows(); i++)
            for (size_t j=0; j<step2Mat.columns(); j++)
                step2Mat.entry(i,j) = step1Mat.entry(i,j);
    else
        for (size_t i=0; i<step2Mat.rows(); i++)
            for (size_t j=0; j<step2Mat.columns(); j++) {
                // (ID_TOR xran->tensorCi_.entry(i,k)*incl_tensorIfLoc_)*step1Mat.entry(k,l)*
                // ID_TOR x trunc_tensorIfLoc_*dom->tensorC_.entry(l, j)) app shifted...
                if (i < codomain_.TORVec_.size()) {
                    if (j < domain_.TORVec_.size()) {
                        step2Mat.entry(i,j) = step1Mat.entry(i,j);
                    } else {
                        // [step1 UR corner] * [dom->tensorC_ first tensorIfLoc_ rows cropped]
                        for (size_t k=domain_.tensorIfLoc_; k<domain_.tensorC_.rows(); k++)
                            step2Mat.entry(i,j) += step1Mat.entry(i,k-domain_.tensorIfLoc_+domain_.TORVec_.size())*
                                domain_.tensorC_.entry(k,j-domain_.TORVec_.size());
                    }
                } else if (j < domain_.TORVec_.size()) {
                    for (size_t k=codomain_.tensorIfLoc_; k<codomain_.tensorCi_.columns(); k++)
                        step2Mat.entry(i,j) += codomain_.tensorCi_.entry(i-codomain_.TORVec_.size(),k)*
                            step1Mat.entry(k-codomain_.tensorIfLoc_+codomain_.TORVec_.size(),j);
                } else {
                    for (size_t k=codomain_.tensorIfLoc_; k<codomain_.tensorCi_.rows(); k++)
                        for (size_t l=domain_.tensorIfLoc_; l<domain_.tensorC_.rows(); l++)
                            step2Mat.entry(i,j) += codomain_.tensorCi_.entry(i-codomain_.TORVec_.size(),k)*
                                step1Mat.entry(k-codomain_.tensorIfLoc_+codomain_.TORVec_.size(),
                                l-domain_.tensorIfLoc_+domain_.TORVec_.size())*
                                domain_.tensorC_.entry(l,j-domain_.TORVec_.size());
                }
            }
    // now we rescale the TOR components appropriately, various row/column
    // mult and divisions. multiply first ran.TORLoc rows by p/gcd(p,q)
    // divide first dom.TORLoc rows by p/gcd(p,q)

    for (size_t i=0; i<codomain_.TORVec_.size(); i++)
        for (size_t j=0; j<step2Mat.columns(); j++)
            step2Mat.entry(i,j) *=
                codomain_.coeff_.divExact(codomain_.coeff_.gcd(codomain_.TORVec_[i]));
    for (size_t i=0; i<step2Mat.rows(); i++)
        for (size_t j=0; j<domain_.TORVec_.size(); j++)
            step2Mat.entry(i,j) /=
                domain_.coeff_.divExact(domain_.coeff_.gcd(domain_.TORVec_[j]));
    // previous line of code divisibility is good thing to check when debugging.
    // step 3, move it all up to the CC coordinates.
    // ran.MR_ * incl_ifLoc_ * step2Mat * proj_ifLoc_ * dom.MRi_
    size_t domTORLoc = domain_.rankM_ - domain_.TORVec_.size();
    size_t codomTORLoc = codomain_.rankM_ - codomain_.TORVec_.size();
    for (size_t i=0; i<matrix.rows(); i++)
        for (size_t j=0; j<matrix.columns(); j++) {
            for (size_t k=codomTORLoc; k<codomain_.MR_.columns(); k++)
                for (size_t l=domTORLoc;l<domain_.MRi_.rows(); l++)
                    matrix.entry(i,j) += codomain_.MR_.entry(i,k) *
                        step2Mat.entry(k-codomTORLoc,l-domTORLoc) *
                        domain_.MRi_.entry(l,j);
        }
}

void HomMarkedAbelianGroup::swap(HomMarkedAbelianGroup& other) noexcept {
    domain_.swap(other.domain_);
    codomain_.swap(other.codomain_);
    matrix.swap(other.matrix);
    reducedMatrix_.swap(other.reducedMatrix_);
    kernel_.swap(other.kernel_);
    cokernel_.swap(other.cokernel_);
    image_.swap(other.image_);
    reducedKernelLattice_.swap(other.reducedKernelLattice_);
}

void HomMarkedAbelianGroup::computeReducedMatrix() {
    if (!reducedMatrix_) {
        reducedMatrix_ = MatrixInt( codomain_.snfRank(), domain_.snfRank() );

        for (size_t j=0; j<reducedMatrix_->columns(); j++) {
            Vector<Integer> colV = (j<domain_.countInvariantFactors()) ?
                domain_.torsionRep(j) :
                domain_.freeRep(j-domain_.countInvariantFactors());
            Vector<Integer> midge = codomain_.snfRep(matrix * colV);
            for (size_t i=0; i<midge.size(); i++)
                reducedMatrix_->entry(i,j) = midge[i];
        }
    }
}

void HomMarkedAbelianGroup::computeReducedKernelLattice() {
    if (!reducedKernelLattice_) {
        computeReducedMatrix();
        const MatrixInt& redMatrix(*reducedMatrix_);

        std::vector<Integer> dcL(codomain_.rank() +
            codomain_.countInvariantFactors() );
        for (size_t i=0; i<dcL.size(); i++)
            if (i<codomain_.countInvariantFactors())
                dcL[i]=codomain_.invariantFactor(i);
            else
                dcL[i]=0;

        reducedKernelLattice_ = MatrixInt(preImageOfLattice(redMatrix, dcL));
    }
}

void HomMarkedAbelianGroup::computeKernel() {
    if (!kernel_) {
        computeReducedKernelLattice();
        MatrixInt dcLpreimage( *reducedKernelLattice_ );

        MatrixInt R, Ri, C, Ci;
        metricalSmithNormalForm( dcLpreimage, R, Ri, C, Ci );

        // the matrix representing the domain lattice in dcLpreimage
        // coordinates is given by domainLattice * R * (dcLpreimage inverse) * C

        MatrixInt relators(domain_.countInvariantFactors(),
            dcLpreimage.columns());

        for (size_t j = 0; j < relators.rows(); j++)
            for (size_t i = 0; i < relators.columns(); i++)
                for (size_t k = 0; k < R.columns(); k++)
                    relators.entry(j, i) += (domain_.invariantFactor(j) *
                        R.entry(i, k) * C.entry(k, j)) / dcLpreimage.entry(k,k);

        kernel_ = AbelianGroup(std::move(relators));
    }
}



void HomMarkedAbelianGroup::computeCokernel() {
    if (!cokernel_) {
        computeReducedMatrix();

        MatrixInt relators(
            reducedMatrix_->columns() + codomain_.countInvariantFactors(),
            reducedMatrix_->rows());

        for (size_t j = 0; j < reducedMatrix_->columns(); j++)
            for (size_t i = 0; i < reducedMatrix_->rows(); i++)
                relators.entry(j, i) = reducedMatrix_->entry(i, j);
        for (size_t i = 0; i < codomain_.countInvariantFactors(); i++)
            relators.entry(i + reducedMatrix_->columns(), i)=
                codomain_.invariantFactor(i);

        cokernel_ = AbelianGroup(std::move(relators));
    }
}


void HomMarkedAbelianGroup::computeImage() {
    if (!image_) {
        computeReducedKernelLattice();

        MatrixInt relators(
            reducedKernelLattice_->columns() + domain_.countInvariantFactors(),
            reducedKernelLattice_->rows());

        for (size_t i = 0; i < domain_.countInvariantFactors(); i++)
            relators.entry(i, i) = domain_.invariantFactor(i);

        for (size_t j = 0; j < reducedKernelLattice_->columns(); j++)
            for (size_t i = 0; i < relators.columns(); i++)
                relators.entry(j + domain_.countInvariantFactors(), i) =
                    reducedKernelLattice_->entry(i, j);

        image_ = AbelianGroup(std::move(relators));
    }
}

HomMarkedAbelianGroup HomMarkedAbelianGroup::operator * (
        const HomMarkedAbelianGroup& X) const {
    // Induces a deep copy of X.domain_ and this->codomain_.
    return HomMarkedAbelianGroup(X.domain_, codomain_,
        matrix * X.matrix);
}

HomMarkedAbelianGroup HomMarkedAbelianGroup::operator * (
        HomMarkedAbelianGroup&& X) const {
    // Induces a deep copy of this->codomain_.
    return HomMarkedAbelianGroup(std::move(X.domain_), codomain_,
        matrix * X.matrix);
}

Vector<Integer> HomMarkedAbelianGroup::evalSNF(const Vector<Integer>& input)
        const {
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    if (input.size() != domain_.snfRank() )
        throw InvalidArgument(
            "The argument to evalSNF() is a vector of the wrong size");

    Vector<Integer> retval = reducedMatrix() * input;

    for (size_t i=0; i<codomain_.countInvariantFactors(); ++i) {
        retval[i] %= codomain_.invariantFactor(i);
        if (retval[i]<0)
            retval[i] += codomain_.invariantFactor(i);
    }
    return retval;
}


std::string HomMarkedAbelianGroup::summary() const {
    std::ostringstream out;
    summary(out);
    return out.str();
}

void HomMarkedAbelianGroup::summary(std::ostream& out) const {
    if (isIsomorphism())
        out << "Isomorphism";
    else if (isZero())
        out << "Zero map";
    else if (isMonic()) { // monic not epic
        out << "Monic (cokernel ";
        cokernel().writeTextShort(out);
        out << ')';
    } else if (isEpic()) { // epic not monic
        out << "Epic (kernel ";
        kernel().writeTextShort(out);
        out << ')';
    } else { // nontrivial not epic, not monic
        out << "Map (kernel ";
        kernel().writeTextShort(out);
        out << " | cokernel ";
        cokernel().writeTextShort(out);
        out << " | image ";
        image().writeTextShort(out);
        out << ')';
    }
}

void HomMarkedAbelianGroup::writeTextShort(std::ostream& out) const {
    summary(out);
    out << ": ";

    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();

    reducedMatrix_->writeTextShort(out);
}

void HomMarkedAbelianGroup::writeTextLong(std::ostream& out) const {
    out << "Hom: ";
    domain_.writeTextShort(out);
    out << " --> ";
    codomain_.writeTextShort(out);
    out << std::endl;

    if (isIsomorphism())
        out << "Isomorphism" << std::endl;
    else if (isZero())
        out << "Zero map" << std::endl;
    else if (isMonic()) {
        // monic not epic
        out << "Monic, cokernel: ";
        cokernel().writeTextShort(out);
        out << std::endl;
    } else if (isEpic()) {
        // epic not monic
        out << "Epic, kernel: ";
        kernel().writeTextShort(out);
        out << std::endl;
    } else {
        // nontrivial not epic, not monic
        out << "Kernel: ";
        kernel().writeTextShort(out);
        out << std::endl;
        out << "Cokernel: ";
        cokernel().writeTextShort(out);
        out << std::endl;
        out << "Image: ";
        image().writeTextShort(out);
        out << std::endl;
    }

    // Cast away const to compute the reduced matrix -- the only reason we're
    // changing data members now is because we delayed calculations
    // until they were really required.
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();

    out << "Reduced Matrix (torsion coordinates appear first):" << std::endl;
    reducedMatrix_->writeTextLong(out);
}


bool HomMarkedAbelianGroup::isIdentity() const {
    if (domain_ != codomain_)
        return false;
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    return reducedMatrix_->isIdentity();
}

bool HomMarkedAbelianGroup::isCycleMap() const {
    for (size_t j=0; j<domain_.cycleRank(); j++)
        if (! codomain_.isCycle(matrix * domain_.cycleGen(j)))
            return false;
    return true;
}

//  Returns a HomMarkedAbelianGroup representing the induced map
//  on the torsion subgroups.
HomMarkedAbelianGroup HomMarkedAbelianGroup::torsionSubgroup() const {
    MarkedAbelianGroup dom = domain_.torsionSubgroup();
    MarkedAbelianGroup ran = codomain_.torsionSubgroup();

    MatrixInt mat(codomain_.countInvariantFactors(),
        domain_.countInvariantFactors() );
    for (size_t j=0; j<domain_.countInvariantFactors(); j++) {
        // vector in codomain's snfRep coords
        Vector<Integer> temp(codomain_.snfRep(evalCC(domain_.torsionRep(j))));
        for (size_t i=0; i<codomain_.countInvariantFactors(); i++)
            mat.entry(i,j) = temp[i];
    }

    return HomMarkedAbelianGroup(std::move(dom), std::move(ran),
        std::move(mat));
}


/**
 * Given two HomMarkedAbelianGroups, you have two diagrams:
 * Z^a --N1--> Z^b --M1--> Z^c   Z^g --N3--> Z^h --M3--> Z^i
 *                   ^                             ^
 *                   |  this                       | other
 * Z^d --N2--> Z^e --M2--> Z^f   Z^j --N4--> Z^k --M4--> Z^l
 * \return true if and only if M1 == N3, M2 == N4 and diagram commutes
 */
bool HomMarkedAbelianGroup::isChainMap(const HomMarkedAbelianGroup &other)
        const {
    if ( (codomain_.M_.rows() != other.codomain_.N_.rows()) ||
            (codomain_.M_.columns() != other.codomain_.N_.columns()) ||
            (domain_.M_.rows() != other.domain_.N_.rows()) ||
            (domain_.M_.columns() != other.domain_.N_.columns()))
        return false;
    if (codomain_.M_ != other.codomain_.N_ || domain_.M_ != other.domain_.N_)
        return false;
    MatrixInt prodLU = codomain_.M_ * definingMatrix();
    MatrixInt prodBR = other.definingMatrix() * domain_.M_;
    if ( prodLU != prodBR )
        return false;
    return true;
}


// Start with the reduced matrix which is a 2x2 block matrix:
//
//  [A|B]
//  [---]  where D is an inveritble square matrix, 0 is a zero matrix,
//  [0|D]  A a square matrix and B maybe not square.
//         the columns of D represent the free factors of the domain,
//      the rows of D the free factors of the codomain,
//     the columns/rows of A represent the torsion factors of the domain/codomain
//     respectively.  So the inverse matrix must have the form
//  [A'|B']
//  [-----]
//  [0 |D']  where D' is the inverse of D, A' represents the inverse
//           automorphism of
//  Z_p1 + Z_p2 + ... Z_pk, etc.  And so B' must equal -A'BD', which is readily
//computable.  So it all boils down to computing A'.  So we need a routine which
//makes a matrix A representing an automorphism of Z_p1 + ... Z_pk and then
// computes the matrix representing the inverse automorphism.
// So to do this we'll need a new matrixops.cpp command -- call it
// torsionAutInverse.
HomMarkedAbelianGroup HomMarkedAbelianGroup::inverseHom() const {
    const_cast<HomMarkedAbelianGroup*>(this)->computeReducedMatrix();
    MatrixInt invMat( reducedMatrix_->columns(), reducedMatrix_->rows() );
    if (!isIsomorphism())
        return HomMarkedAbelianGroup( std::move(invMat), codomain_, domain_ );
    // get A, B, D from reducedMatrix
    // A must be square with domain/codomain_.countInvariantFactors() columns
    // D must be square with domain/codomain_.rank() columns
    // B may not be square with domain_.rank() columns and
    //  codomain_.countInvariantFactors() rows.
    MatrixInt A(codomain_.countInvariantFactors(),
        domain_.countInvariantFactors());
    MatrixInt B(codomain_.countInvariantFactors(), domain_.rank());
    MatrixInt D(codomain_.rank(), domain_.rank());
    for (size_t i=0; i<A.rows(); i++)
        for (size_t j=0; j<A.columns(); j++)
            A.entry(i,j) = reducedMatrix_->entry(i,j);
    for (size_t i=0; i<B.rows(); i++)
        for (size_t j=0; j<B.columns(); j++)
            B.entry(i,j) = reducedMatrix_->entry(i, j + A.columns());
    for (size_t i=0; i<D.rows(); i++)
        for (size_t j=0; j<D.columns(); j++)
            D.entry(i,j) = reducedMatrix_->entry(i + A.rows(), j + A.columns());
    // compute A', B', D'
    // use void columnEchelonForm(MatrixInt &M, MatrixInt &R, MatrixInt &Ri,
    // const std::vector<size_t> &rowList);
    //  from matrixOps to compute the inverse of D.
    MatrixInt Di(D.rows(), D.columns()); Di.makeIdentity();
    MatrixInt Dold(D.rows(), D.columns()); Dold.makeIdentity();
    std::vector<size_t> rowList(D.rows());
    for (size_t i=0; i<rowList.size(); i++)
        rowList[i]=i;
    columnEchelonForm(D, Di, Dold, rowList);
    // now Di is the inverse of the old D, and D is the identity,
    // and Dold is the old D.
    std::vector<Integer> invF(domain_.countInvariantFactors());
    for (size_t i=0; i<invF.size(); i++)
        invF[i] = domain_.invariantFactor(i);
    MatrixInt Ai = torsionAutInverse( A, invF);
    // then Bi is given by Bi = -AiBDi
    MatrixInt Bi(codomain_.countInvariantFactors(), domain_.rank());
    MatrixInt Btemp(codomain_.countInvariantFactors(), domain_.rank());
    // Btemp will give -BDi
    // Bi will be AiBtemp
    for (size_t i=0; i<Btemp.rows(); i++)
        for (size_t j=0; j<Btemp.columns(); j++)
            for (size_t k=0; k<Btemp.columns(); k++)
                Btemp.entry(i,j) -= B.entry(i,k)*Di.entry(k,j);
    for (size_t i=0; i<Bi.rows(); i++)
        for (size_t j=0; j<Bi.columns(); j++)
            for (size_t k=0; k<Ai.columns(); k++)
                Bi.entry(i,j) += Ai.entry(i,k)*Btemp.entry(k,j);
    // reduce Ai and Bi respectively.
    for (size_t i=0; i<Ai.rows(); i++) {
        for (size_t j=0; j<Ai.columns(); j++) {
            Ai.entry(i,j) %= domain_.invariantFactor(i);
            if (Ai.entry(i,j) < 0)
                Ai.entry(i,j) += domain_.invariantFactor(i);
        }
        for (size_t j=0; j<Bi.columns(); j++) {
            Bi.entry(i,j) %= domain_.invariantFactor(i);
            if (Bi.entry(i,j) < 0)
                Bi.entry(i,j) += domain_.invariantFactor(i);
        }
    }
    // compute the coefficients of invMat.  We're in the funny situation where
    // we know invMat's reduced matrix, not its chain-complex level defining
    // matrix.  So we use the alternative HomMarkedAbelianGroup constructor
    // designed specifically for this situation.
    // assemble into invMat  [A'|B']
    //                       [-----]
    //                       [0 |D']

    for (size_t i=0; i<Ai.rows(); i++)
        for (size_t j=0; j<Ai.columns(); j++)
            invMat.entry(i,j) = Ai.entry(i,j);
    for (size_t i=0; i<Di.rows(); i++)
        for (size_t j=0; j<Di.columns(); j++)
            invMat.entry(i+Ai.rows(),j+Ai.columns()) = Di.entry(i,j);
    for (size_t i=0; i<Bi.rows(); i++)
        for (size_t j=0; j<Bi.columns(); j++)
            invMat.entry(i,j+Ai.columns()) = Bi.entry(i,j);
    return HomMarkedAbelianGroup(std::move(invMat), codomain_, domain_);
}

} // namespace regina

