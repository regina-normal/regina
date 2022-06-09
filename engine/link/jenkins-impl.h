
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

#ifndef __REGINA_JENKINS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_JENKINS_IMPL_H
#endif

namespace regina {

template <typename Iterator>
Link Link::fromJenkins(Iterator begin, Iterator end) {
    if (begin == end)
        throw InvalidArgument("fromJenkins(): missing number of components");

    using InputInt = std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>;
    static_assert(std::is_integral_v<InputInt> &&
        ! std::is_unsigned_v<InputInt>, "fromJenkins(): the iterator type "
        "needs to dereference to give a native signed C++ integer type.");

    InputInt val = *begin++;
    if (val < 0)
        throw InvalidArgument("fromJenkins(): invalid number of components");
    if constexpr (sizeof(InputInt) > sizeof(size_t))
        if (val > static_cast<InputInt>(SIZE_MAX))
            throw InvalidArgument("fromJenkins(): too many components");
    auto nComp = static_cast<size_t>(val);

    if (nComp == 0)
        return Link();

    Link ans;
    ans.components_.resize(nComp);

    // We need to allocate the crossings first.
    // Alas, these appear last in the input.
    // Skip through (and remember) the link components, which appear first.
    using CompStep = std::pair<size_t, int>; // (crossing, side)
    auto* compLen = new size_t[nComp];
    auto* compInput = new CompStep*[nComp];
    size_t foundCrossings = 0;

    size_t c;
    try {
        for (c = 0; c < nComp; ++c) {
            if (begin == end)
                throw InvalidArgument(
                    "fromJenkins(): missing length of component");
            val = *begin++;
            if (val < 0)
                throw InvalidArgument(
                    "fromJenkins(): invalid length of component");
            if constexpr (sizeof(InputInt) > sizeof(size_t))
                if (val > static_cast<InputInt>(SIZE_MAX))
                    throw InvalidArgument(
                        "fromJenkins(): component too large");
            compLen[c] = static_cast<size_t>(val);

            compInput[c] = new CompStep[compLen[c]];
            for (size_t i = 0; i < compLen[c]; ++i) {
                if (begin == end) {
                    ++c; // to ensure compInput[c] will be deleted also
                    throw InvalidArgument(
                        "fromJenkins(): incomplete component");
                }
                val = *begin++;
                if (val < 0) {
                    ++c; // to ensure compInput[c] will be deleted also
                    throw InvalidArgument(
                        "fromJenkins(): invalid crossing in component");
                }
                if constexpr (sizeof(InputInt) > sizeof(size_t))
                    if (val > static_cast<InputInt>(SIZE_MAX)) {
                        ++c; // to ensure compInput[c] will be deleted also
                        throw InvalidArgument(
                            "fromJenkins(): crossing too large in component");
                    }
                compInput[c][i].first = static_cast<size_t>(val);

                if (begin == end) {
                    ++c; // to ensure compInput[c] will be deleted also
                    throw InvalidArgument(
                        "fromJenkins(): incomplete component");
                }
                val = *begin++;
                if (val != 1 && val != -1) {
                    ++c; // to ensure compInput[c] will be deleted also
                    throw InvalidArgument(
                        "fromJenkins(): invalid side in component");
                }
                compInput[c][i].second = static_cast<int>(val);
            }

            foundCrossings += compLen[c];
        }
    } catch (const InvalidArgument&) {
        for (size_t i = 0; i < c; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        throw;
    }

    // *Now* we have the number of crossings.
    if (foundCrossings % 2 != 0) {
        // Invalid input.
        for (size_t i = 0; i < nComp; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        throw InvalidArgument("fromJenkins(): odd number of total steps");
    }
    size_t nCross = foundCrossings / 2;

    auto* tmpCross = new Crossing*[nCross];
    std::fill(tmpCross, tmpCross + nCross, nullptr);

    try {
        for (size_t i = 0; i < nCross; ++i) {
            if (begin == end)
                throw InvalidArgument(
                    "fromJenkins(): missing crossing label");
            val = *begin++;
            if (val < 0 || static_cast<size_t>(val) >= nCross)
                throw InvalidArgument(
                    "fromJenkins(): invalid crossing label");
            auto label = static_cast<size_t>(val);
            if (tmpCross[label])
                throw InvalidArgument(
                    "fromJenkins(): duplicate crossing label");

            if (begin == end)
                throw InvalidArgument(
                    "fromJenkins(): missing crossing sign");
            val = *begin++;
            if (val != 1 && val != -1)
                throw InvalidArgument(
                    "fromJenkins(): invalid crossing sign");
            int sign = static_cast<int>(val);

            tmpCross[label] = new Crossing(sign);
        }
    } catch (const InvalidArgument&) {
        for (size_t i = 0; i < nCross; ++i)
            delete tmpCross[i];
        delete[] tmpCross;
        for (size_t i = 0; i < nComp; ++i)
            delete[] compInput[i];
        delete[] compInput;
        delete[] compLen;
        throw;
    }

    for (size_t i = 0; i < nCross; ++i)
        ans.crossings_.push_back(tmpCross[i]);
    delete[] tmpCross;

    // Finally, we can connect the crossings together by following the
    // individual link components.
    for (c = 0; c < nComp; ++c) {
        if (compLen[c] == 0) {
            // The StrandRef() constructor already gives us a null reference.
            continue;
        }

        for (size_t i = 0; i < compLen[c]; ++i) {
            size_t startCross = compInput[c][i].first;
            int startStrand = (compInput[c][i].second > 0 ? 1 : 0);
            size_t endCross = compInput[c][i == compLen[c]-1 ? 0 : i+1].first;
            int endStrand =
                (compInput[c][i == compLen[c]-1 ? 0 : i+1].second > 0 ? 1 : 0);

            if (ans.crossings_[startCross]->next_[startStrand]) {
                for (i = 0; i < nComp; ++i)
                    delete[] compInput[i];
                delete[] compInput;
                delete[] compLen;
                throw InvalidArgument(
                    "fromJenkins(): multiple visits to the same strand");
            }
            ans.crossings_[startCross]->next_[startStrand] =
                ans.crossings_[endCross]->strand(endStrand);
        }

        ans.components_[c] = ans.crossings_[compInput[c][0].first]->strand(
            compInput[c][0].second > 0 ? 1 : 0);
    }

    // Start cleaning up.
    for (size_t i = 0; i < nComp; ++i)
        delete[] compInput[i];
    delete[] compInput;
    delete[] compLen;

    // Set up prev links to match next links.
    StrandRef next;
    for (Crossing* cross : ans.crossings_) {
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

