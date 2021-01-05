
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include <cstdlib>
#include <iterator>

namespace regina {

std::string Link::dt(bool alpha) const {
    std::ostringstream out;
    dt(out, alpha);
    return out.str();
}

void Link::dt(std::ostream& out, bool alpha) const {
    if (components_.size() != 1)
        return;
    if (crossings_.empty())
        return;
    if (alpha && size() > 26)
        return;

    // Dowker-Thistlethwaite notation requires us to start on the lower strand.
    StrandRef start = components_.front();
    if (start.strand() == 1)
        start.jump();

    size_t n = size();

    // Odd steps in traversal -> crossing index
    int* oddCrossing = new int[n];

    // Crossing index -> even steps in traversal, negated if passing under
    int* evenStep = new int[n];

    StrandRef s = start;
    unsigned step = 0;
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
    assert(step == 2 * n);

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

    delete[] evenStep;
    delete[] oddCrossing;
}

Link* Link::fromDT(const std::string& s) {
    // Do we have an alphabetical or numerical string?
    auto it = s.begin();
    while (it != s.end() && ::isspace(*it))
        ++it;
    if (it == s.end()) {
        // Empty string, so return the unknot.
        return new Link(1);
    }

    std::vector<int> terms;

    if ((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z')) {
        // We have the alphabetical variant.
        for ( ; it != s.end(); ++it) {
            if (*it >= 'a' && *it <= 'z')
                terms.push_back(2 * (*it - 'a' + 1));
            else if (*it >= 'A' && *it <= 'Z')
                terms.push_back(-2 * (*it - 'A' + 1));
            else if (! ::isspace(*it)) {
                // Invalid character.
                return nullptr;
            }
        }
    } else if ((*it >= '0' && *it <= '9') || *it == '-') {
        // We have the numerical variant.
        std::istringstream in(s);

        int i;
        while (true) {
            in >> i;
            if (! in) {
                if (in.eof())
                    break;
                return nullptr;
            }
            terms.push_back(i);
        }
    } else
        return nullptr;

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
    int     *anInvolution,
    bool    *aRealization,
    int     aNumCrossings)
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

    int N;
    int i,j;
    int *modTWO_N;
    int *seq;
    int *emb;
    int *A,*D;
    int Aempty,Dempty;
    int OkSoFar;
    int *phi;
    int x;

    N = aNumCrossings;

    /*
     *  Allocate local arrays.
     */
    modTWO_N    = new int[4 * N];
    seq         = new int[4 * N];
    emb         = new int[2 * N];
    A           = new int[2 * N];
    D           = new int[2 * N];
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
        emb[i]=A[i]=D[i]=0;
    }
    /*set initial conditions*/
    OkSoFar=A[0]=A[seq[0]]=1;
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
            D[j]=1;
        for(j=seq[i]+1; j<2*N; j++)
            D[j]=1;
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
                        D[x]=D[seq[x]]=0;
                    }
                    else{
                        OkSoFar=0;
                    }
                }
                else{
                    if(emb[x] != 0){/* emb[x] is already defined*/
                        if(phi[x]*phi[seq[x]]*emb[i]==emb[x])
                            D[x]=0;
                        else
                            OkSoFar=0;
                    }
                    else{/* emb[x] is not yet defined. Hence x<>0 and
x<>seq[0]*/
                        emb[x]=phi[x]*phi[seq[x]]*emb[i];
                        emb[seq[x]]=-emb[x];
                        D[x]=0;
                        if( modTWO_N[abs(seq[x]-seq[x-1])]==1){
                            /*nothing*/
                        }
                        else{
                            A[x]=A[seq[x]]=1;
                        }
                    }
                }
            }
            else{/*x>seq[i]*/
                if(seq[x]<i || seq[x]>seq[i]){
                    D[x]=D[seq[x]]=0;
                }
                else{
                    if(emb[x]!=0){
                        D[x]=0;
                    }
                    else{
                        emb[x]=phi[x]*phi[seq[x]]*emb[i];
                        emb[seq[x]]=-emb[x];
                        D[x]=0;
                        if( modTWO_N[abs(seq[x]-seq[x-1])]==1){
                            /*nothing*/
                        }
                        else{
                            A[x]=A[seq[x]]=1;
                        }
                    }
                }
            }
            /* see if D is empty, ie is all zeroes*/
            for(j=0;j<2*N-1 && !D[j];j++)
                ;
            Dempty=!D[j];
        }/*end of while*/
        A[i]=0;
        A[seq[i]]=0;
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

