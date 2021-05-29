
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

/*! \file link/jenkins-impl.h
 *  \brief Contains implementation details for parsing Jenkins' format
 *  for links.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __JENKINS_IMPL_H
#ifndef __DOXYGEN
#define __JENKINS_IMPL_H
#endif

namespace regina {

template <typename Iterator>
Link* Link::fromJenkins(Iterator begin, Iterator end) {
    if (begin == end) {
        std::cerr << "fromJenkins(): could not read #components" << std::endl;
        return nullptr;
    }
    int nComp = *begin++;

    if (nComp == 0)
        return new Link;

    Link* ans = new Link;
    ans->components_.resize(nComp);

    // We need to allocate the crossings first.
    // Alas, these appear last in the input.
    // Skip through (and remember) the link components, which appear first.
    int* compLen = new int[nComp];
    int** compInput = new int*[nComp];
    unsigned c, i;
    unsigned foundCrossings = 0;

    bool broken = false;
    for (c = 0; c < nComp; ++c) {
        if (begin == end) {
            std::cerr << "fromJenkins(): could not read length of component "
                << c << std::endl;
            broken = true;
            break;
        }
        compLen[c] = *begin++;
        if (compLen[c] < 0) {
            std::cerr << "fromJenkins(): invalid length of component " << c
                << std::endl;
            broken = true;
            break;
        }

        compInput[c] = new int[2 * compLen[c]];
        for (i = 0; i < compLen[c]; ++i) {
            if (begin == end) {
                std::cerr << "fromJenkins(): could not read step " << i
                    << " of component " << c << std::endl;
                broken = true;
                break;
            }
            compInput[c][2 * i] = *begin++;
            if (begin == end) {
                std::cerr << "fromJenkins(): could not read step " << i
                    << " of component " << c << std::endl;
                broken = true;
                break;
            }
            compInput[c][2 * i + 1] = *begin++;
            if (compInput[c][2 * i] < 0 ||
                    (compInput[c][2 * i + 1] != 1 &&
                     compInput[c][2 * i + 1] != -1)) {
                std::cerr << "fromJenkins(): invalid step " << i
                    << " of component " << c << std::endl;
                broken = true;
                break;
            }
        }
        if (broken) {
            ++c; // to ensure compInput[c] is deleted also
            break;
        }

        foundCrossings += compLen[c];
    }

    if (broken) {
        for (i = 0; i < c; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        delete ans;
        return nullptr;
    }

    // *Now* we have the number of crossings.
    if (foundCrossings % 2 != 0) {
        // Invalid input.
        for (i = 0; i < nComp; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        delete ans;
        std::cerr << "fromJenkins(): odd number of total steps" << std::endl;
        return nullptr;
    }
    unsigned nCross = foundCrossings / 2;

    Crossing** tmpCross = new Crossing*[nCross];
    std::fill(tmpCross, tmpCross + nCross, nullptr);

    int label, sign;
    for (i = 0; i < nCross; ++i) {
        if (begin == end) {
            std::cerr << "fromJenkins(): could not read label for crossing "
                << i << std::endl;
            broken = true;
            break;
        }
        label = *begin++;
        if (begin == end) {
            std::cerr << "fromJenkins(): could not read sign of crossing "
                << i << std::endl;
            broken = true;
            break;
        }
        sign = *begin++;
        if ((sign != 1 && sign != -1) ||
                label < 0 || label >= nCross || tmpCross[label]) {
            std::cerr << "fromJenkins(): invalid data for crossing " << i
                << std::endl;
            broken = true;
            break;
        }

        tmpCross[label] = new Crossing(sign);
    }

    if (broken) {
        for (i = 0; i < nCross; ++i)
            delete tmpCross[i];
        delete[] tmpCross;
        for (i = 0; i < nComp; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        delete ans;
        return nullptr;
    }

    for (i = 0; i < nCross; ++i)
        ans->crossings_.push_back(tmpCross[i]);
    delete[] tmpCross;

    // Finally, we can connect the crossings together by following the
    // individual link components.
    unsigned startCross, endCross;
    unsigned startStrand, endStrand;
    for (c = 0; c < nComp && ! broken; ++c) {
        if (compLen[c] == 0) {
            // The StrandRef() constructor already gives us a null reference.
            continue;
        }

        for (i = 0; i < compLen[c]; ++i) {
            startCross = compInput[c][2 * i];
            startStrand = (compInput[c][2 * i + 1] > 0 ? 1 : 0);
            endCross = compInput[c][i == compLen[c] - 1 ? 0 : 2 * i + 2];
            endStrand = (compInput[c][i == compLen[c] - 1 ? 1 : 2 * i + 3] > 0 ?
                1 : 0);

            if (ans->crossings_[startCross]->next_[startStrand]) {
                broken = true;
                std::cerr << "fromJenkins(): multiple visits to "
                    << (startStrand == 1 ? "upper" : "lower")
                    << " strand of crossing " << startCross << std::endl;
                break;
            }
            ans->crossings_[startCross]->next_[startStrand] =
                ans->crossings_[endCross]->strand(endStrand);
        }

        ans->components_[c] = ans->crossings_[compInput[c][0]]->strand(
            compInput[c][1] > 0 ? 1 : 0);
    }

    // Start cleaning up.
    for (i = 0; i < nComp; ++i)
        delete[] compInput[i];
    delete[] compInput;
    delete[] compLen;

    if (broken) {
        // Invalid input.
        delete ans;
        return nullptr;
    }

    // Set up prev links to match next links.
    StrandRef next;
    for (Crossing* cross : ans->crossings_) {
        next = cross->next_[0];
        next.crossing()->prev_[next.strand()] = cross->lower();

        next = cross->next_[1];
        next.crossing()->prev_[next.strand()] = cross->upper();
    }

    // All done!
    return ans;
}

} // namespace regina

#endif

