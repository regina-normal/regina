
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  The implementation of fromDT() is based on the Dowker-Thistlethwaite  *
 *  implementation from the SnapPy/SnapPea kernel.                        *
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

#include "link/link.h"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <iterator>

namespace {
    // Computes abs(a-b) without using negative numbers.
    size_t absdiff(size_t a, size_t b) {
        return (a >= b ? a - b : b - a);
    }
}

namespace regina {

std::string Link::dt(bool alpha) const {
    std::ostringstream out;
    dt(out, alpha);
    return out.str();
}

void Link::dt(std::ostream& out, bool alpha) const {
    if (components_.size() != 1)
        throw NotImplemented("Dowker-Thistlethwaite notation is "
            "only implemented for single-component links");
    if (crossings_.empty())
        return;
    if (alpha && size() > 26)
        throw NotImplemented("Alphabetical Dowker-Thistlethwaite notation is "
            "only implemented for links with at most 26 crossings");
    if (! isClassical())
        throw NotImplemented("Dowker-Thistlethwaite notation is only "
            "implemented for classical link diagrams, not virtual diagrams");

    // Dowker-Thistlethwaite notation requires us to start on the lower strand.
    StrandRef start = components_.front();
    if (start.strand() == 1)
        start.jump();

    size_t n = size();

    // Work with the largest signed integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;

    // Odd steps in traversal -> crossing index
    FixedArray<size_t> oddCrossing(n);

    // Crossing index -> even steps in traversal, negated if passing under
    FixedArray<Int> evenStep(n);

    StrandRef s = start;
    Int step = 0;
    do {
        ++step;
        if (step % 2 == 1) {
            oddCrossing[step >> 1] = s.crossing()->index();
        } else {
            // TODO: It would be kind to do some sanity checking here,
            // to make sure that every crossing is seen here exactly once.
            evenStep[s.crossing()->index()] = (s.strand() == 1 ? step : -step);
        }
        ++s;
    } while (s != start);
    assert(static_cast<size_t>(step) == 2 * n);

    if (alpha) {
        for (size_t i = 0; i < n; ++i)
            out << (evenStep[oddCrossing[i]] > 0 ?
                (char('a' + evenStep[oddCrossing[i]] / 2 - 1)) :
                (char('A' - evenStep[oddCrossing[i]] / 2 - 1)));
    } else {
        for (size_t i = 0; i < n; ++i) {
            if (i > 0)
                out << ' ';
            out << evenStep[oddCrossing[i]];
        }
    }
}

std::vector<int> Link::dtData() const {
    if (components_.size() != 1)
        throw NotImplemented("Dowker-Thistlethwaite notation is "
            "only implemented for single-component links");
    if (crossings_.empty())
        return {};
    if (2 * size() > INT_MAX)
        throw NotImplemented("This Dowker-Thistlethwaite notation has "
            "entries that cannot fit into a C++ int");

    // Dowker-Thistlethwaite notation requires us to start on the lower strand.
    StrandRef start = components_.front();
    if (start.strand() == 1)
        start.jump();

    int n = static_cast<int>(size());

    // Odd steps in traversal -> crossing index
    FixedArray<int> oddCrossing(n);

    // Crossing index -> even steps in traversal, negated if passing under
    FixedArray<int> evenStep(n);

    StrandRef s = start;
    int step = 0;
    do {
        ++step;
        if (step % 2 == 1) {
            oddCrossing[step >> 1] = static_cast<int>(s.crossing()->index());
        } else {
            evenStep[s.crossing()->index()] = (s.strand() == 1 ? step : -step);
        }
        ++s;
    } while (s != start);
    assert(step == 2 * n);

    std::vector<int> ans;
    ans.reserve(n);

    for (int i = 0; i < n; ++i)
        ans.push_back(evenStep[oddCrossing[i]]);

    return ans;
}

Link Link::fromDT(const std::string& s) {
    // Do we have an alphabetical or numerical string?
    auto it = s.begin();
    while (it != s.end() && ::isspace(*it))
        ++it;
    if (it == s.end()) {
        // Empty string, so return the unknot.
        return { 1 };
    }

    // Work with the largest integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;
    std::vector<Int> terms;

    if ((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z')) {
        // We have the alphabetical variant.
        for ( ; it != s.end(); ++it) {
            if (*it >= 'a' && *it <= 'z')
                terms.push_back(2 * (*it - 'a' + 1));
            else if (*it >= 'A' && *it <= 'Z')
                terms.push_back(-2 * (*it - 'A' + 1));
            else if (! ::isspace(*it)) {
                // Invalid character.
                throw InvalidArgument(
                    "fromDT(): invalid alphabetical character");
            }
        }
    } else if ((*it >= '0' && *it <= '9') || *it == '-') {
        // We have the numerical variant.
        std::istringstream in(s);

        Int i;
        while (true) {
            in >> i;
            if (! in) {
                if (in.eof())
                    break;
                throw InvalidArgument("fromDT(): invalid numerical character");
            }
            terms.push_back(i);
        }
    } else
        throw InvalidArgument("fromDT(): invalid character");

    return fromDT(terms.begin(), terms.end());
}

// -----------------------------------------------------------------------
//
// The following code is modified from kernel/unix_kit/decode_new_DT.c
// from the SnapPy/SnapPea kernel.
//
// A full explanation of decoding Dowker-Thistlethwaite codes may be found in
//
//     Dowker and Thistlethwaite, Classification of knot projections,
//     Topology and its Applications 16 (1983) 19-31.
//
// -----------------------------------------------------------------------

bool Link::realizeDT(
    size_t  *anInvolution,
    bool    *aRealization,
    size_t  aNumCrossings)
{
    /*
     *  Returns true iff the sequence was realizable.  If so, then
     *  aRealization will be filled with a realization.  If not, then
     *  the contents of aRealization will be undefined.
     *
     *  I'm hoping to read through -- and understand -- Dowker
     *  and Thistlethwaite's paper.  But for the moment I'll
     *  try to splice in some of Jim's code and hope I'm interpreting
     *  it correctly.
     */

    size_t N;
    size_t i,j;
    size_t *modTWO_N;
    size_t *seq;
    int *emb; // +1 or -1, or 0 for uninitialised
    bool *A,*D;
    bool Aempty,Dempty;
    bool OkSoFar;
    int *phi; // +1 or -1
    size_t x;

    N = aNumCrossings;

    /*
     *  Allocate local arrays.
     */
    modTWO_N    = new size_t[4 * N];
    seq         = new size_t[4 * N];
    emb         = new int[2 * N];
    A           = new bool[2 * N];
    D           = new bool[2 * N];
    phi         = new int[2 * N];

    /*create the modTWO_N array*/
    for(i=0;i<2*N;i++){
        modTWO_N[i]=i;
        modTWO_N[i+2*N]=i;
    }

    /* get seq and height from DT code*/
    /* seq is two copies of full DT involution on crossings numbered 0 to
2N-1 */

    /*
     *  Concatenate two copies of theInvolution[] to obtain seq[].
     */
    for(i = 0; i < 2*N; i++)
    {
        seq[i      ] = anInvolution[i];
        seq[i + 2*N] = anInvolution[i];
    }

    /* begin realizability routine to recover embedding of projection */
    /*zero emb, A, and D. A and D will only contain zeroes and ones*/
    for(i=0;i<2*N;i++){
        emb[i]=0;
        A[i]=D[i]=false;
    }
    /*set initial conditions*/
    OkSoFar=A[0]=A[seq[0]]=true;
    emb[0]=1;
    emb[seq[0]]=-1;

    /* see if A is empty, ie is all zeroes*/
    for(j=0;j<2*N-1 && !A[j];j++)
        /*nothing*/;
    Aempty=!A[j];

    while(!Aempty && OkSoFar){
        /* let i be least member of A*/
        for(i=0; !A[i]; i++)
            /*nothing*/;
        /*determine phi for this value of i*/
        phi[i]=1;
        for(j=i+1;j<i+2*N;j++){
            phi[modTWO_N[j]]=(seq[j]>=i && seq[j]<=seq[i]) ?
-phi[modTWO_N[j-1]]:phi[modTWO_N[j-1]];
        }
        /*establish D*/
        for(j=0; j<i; j++)
            D[j]=true;
        for(j=seq[i]+1; j<2*N; j++)
            D[j]=true;
        /* see if D is empty, ie is all zeroes*/
        for(j=0;j<2*N-1 && !D[j];j++)
            ;
        Dempty=!D[j];
        while(!Dempty && OkSoFar){
            /*let x be least member of D*/
            for(x=0; !D[x]; x++)
                /*nothing*/;
            if(x<i){
                if(seq[x]<i || seq[x]>seq[i]){
                    if(phi[x]*phi[seq[x]]==1){
                        D[x]=D[seq[x]]=false;
                    }
                    else{
                        OkSoFar=false;
                    }
                }
                else{
                    if(emb[x] != 0){/* emb[x] is already defined*/
                        if(phi[x]*phi[seq[x]]*emb[i]==emb[x])
                            D[x]=false;
                        else
                            OkSoFar=false;
                    }
                    else{/* emb[x] is not yet defined. Hence x<>0 and
x<>seq[0]*/
                        emb[x]=phi[x]*phi[seq[x]]*emb[i];
                        emb[seq[x]]=-emb[x];
                        D[x]=false;
                        if( modTWO_N[absdiff(seq[x], seq[x-1])]==1){
                            /*nothing*/
                        }
                        else{
                            A[x]=A[seq[x]]=true;
                        }
                    }
                }
            }
            else{/*x>seq[i]*/
                if(seq[x]<i || seq[x]>seq[i]){
                    D[x]=D[seq[x]]=false;
                }
                else{
                    if(emb[x]!=0){
                        D[x]=false;
                    }
                    else{
                        emb[x]=phi[x]*phi[seq[x]]*emb[i];
                        emb[seq[x]]=-emb[x];
                        D[x]=false;
                        if( modTWO_N[absdiff(seq[x], seq[x-1])]==1){
                            /*nothing*/
                        }
                        else{
                            A[x]=A[seq[x]]=true;
                        }
                    }
                }
            }
            /* see if D is empty, ie is all zeroes*/
            for(j=0;j<2*N-1 && !D[j];j++)
                ;
            Dempty=!D[j];
        }/*end of while*/
        A[i]=false;
        A[seq[i]]=false;
        /* see if A is empty, ie is all zeroes*/
        for(j=0;j<2*N-1 && !A[j];j++)
            /*nothing*/;
        Aempty=!A[j];
    }/*end of while*/

    /*end of realizability routine*/

    /*
     *  Convert emb[] to aRealization[].
     */
    if (OkSoFar)
        for(i = 0; i < 2*N; i++)
            aRealization[i] = (emb[i] == +1);

    /*
     *  Free local arrays.
     */
    delete[] modTWO_N;
    delete[] seq;
    delete[] emb;
    delete[] A;
    delete[] D;
    delete[] phi;

    /*
     *  Return whether the sequence is realizable.
     */
    return OkSoFar;
}

} // namespace regina

