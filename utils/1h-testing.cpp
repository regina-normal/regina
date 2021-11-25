//
//  main.cpp
//  1h-testing
//
//  Created by Antonio Burke on 13/9/21.
//  Copyright © 2021 Regina Development Team. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <array>
#include <tuple>
#include <set>
#include <algorithm>
#include <cctype>
#include <stack>
#include <string>
#include <sstream>
#include <numeric>
#include <unistd.h>
#include <string>

#include <triangulation/dim3.h>
#include <triangulation/dim4.h>
#include <link/link.h>

/*
 INIT TEST CODES:
 [(2,14,3,13),(14,4,15,3),(12,16,13,15),(16,12,17,11),(1,10,2,11),(9,4,10,5),(6,7,1,8),(18,5,7,6),(17,9,18,8)] (example from M-C paper)
 [(2,14,3,13),(14,4,15,3),(12,16,13,15),(16,12,17,11),(1,10,2,11),(9,4,10,5),(6,7,1,8),(18,5,7,6),(17,9,18,8)] (A-M cork)
 */


typedef std::vector<std::array<int, 4>> pdcode;

template <typename T, typename D>
std::ostream& operator<<(std::ostream& os, const std::pair<T, D> &p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T, T, T> &p)
{
    os << "(" << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << ")";
    return os;
}

bool isNumber(const std::string &str) {
    for (char const &c : str) {
        if ((c != '-') && (std::isdigit(c) == 0)) {
            return false;
        }
    }
    return true;
}

int main() {
    
    int dim_flag_int = 4;
    
    pdcode pdcTmp;
    
    /*
     GET USER INPUT (PD CODE)
     */
    std::string rawPDinput;
    std::cout << "Enter PD Code of Diagram: ";
    std::cin >> rawPDinput;
    for (char &c : rawPDinput) {
        if (!isdigit(c)) {
            c = ' ';
        }
    }
    std::vector<int> tmpRawPDVect;
    std::stringstream ss_pdc(rawPDinput);
    int val;
    while (ss_pdc >> val) {
        tmpRawPDVect.push_back(val);
    }

    for (int i=0; i<tmpRawPDVect.size(); i+=4) {
        std::array<int, 4> tmpa;
        for (int j=0; j<4; j++) {
            tmpa[j] = tmpRawPDVect[i+j];
        }
        pdcTmp.push_back(tmpa);
    }
    /*
     END USER INPUT (PD CODE)
     */
    
    regina::Link* tmpLinkObj = regina::Link::fromPD(pdcTmp.begin(), pdcTmp.end());
    if (! tmpLinkObj) {
        // TODO: Barf
        // --- Benjamin Andrew Burton, 2021
    }
    
    size_t numComps = tmpLinkObj -> countComponents();
    
    /*
     COMPUTE NUMBER OF CROSSINGS IN INDIVIDUAL COMPONENT
     */
    std::vector<regina::StrandRef> comps;
    for (int i=0; i<numComps; i++) {
        comps.push_back(tmpLinkObj->component(i));
    }
    
    /*
     For each component of the link, generate the set of crossing indices encountered as we walk along the component.
     */
    std::vector<std::set<int>> comp_x_indices(numComps);
    for (int i=0; i<numComps; i++) {
        auto current = comps[i];
        do {
            current = current.next();
            comp_x_indices[i].insert(current.crossing()->index());
        } while (current != comps[i]);
    }
    
    /*
    For components L_i and L_j of the link, find the intersection of their respective crossing index sets --- i.e. the intersection of comp_x_indices(i) and comp_x_indices(j).
     The size of the resulting set intersection is then 2 * the linking number of L_i and L_j.
     */
    std::vector<std::vector<long>> tmpIntersectionMatrix;
    std::vector<long> intersectionTotals;
    for (int i=0; i<numComps; i++) {
        tmpIntersectionMatrix.push_back(std::vector<long>());
        for (int j=0; j<numComps; j++) {
            if (i!=j) {
                long s1s = comp_x_indices[i].size();
                long s2s = comp_x_indices[j].size();
                std::set<int> s1 = comp_x_indices[i];
                std::set<int> s2 = comp_x_indices[j];
                
                std::vector<int> tmpIntRes(s1s+s2s);
                std::vector<int>::iterator itr;
                itr = std::set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),tmpIntRes.begin());
                tmpIntRes.resize(itr - tmpIntRes.begin());
                
                tmpIntersectionMatrix[i].push_back(tmpIntRes.size());
          }
        }
        /*
         Sum up the values in each vector to give the total number of intersections from other components.
         */
        long compTotal = std::accumulate(tmpIntersectionMatrix[i].begin(), tmpIntersectionMatrix[i].end(), 0);
        intersectionTotals.push_back(compTotal);
    }
    /*
     Give the crossing number as the size of the crossing set for that component, minus the number of total number of intersections found from other components.
     */
    std::vector<long> compCrossingNums;
    for (int i=0; i<numComps; i++) {
        long compNumCrossings = comp_x_indices[i].size() - intersectionTotals[i];
        compCrossingNums.push_back(compNumCrossings);
    }
    std::cout << std::endl;
    /*
     END
     */
    
    std::vector<long> compWrithes;
    for (int i=0; i<numComps; i++) {
        compWrithes.push_back(tmpLinkObj -> writheOfComponent(i));
    }
    std::cout << "Writhe of\n";
    for (int i=0; i<compWrithes.size(); i++) {
        std::cout << "Component " << i << ": " << compWrithes[i] << "\n";
    }
    
    std::cout << "Framing or 1-Handle Placement: ";
    
    std::vector<int> framings(numComps,INFINITY);
    std::vector<bool> oneHandleBools(numComps,false);
    
    for (int i=0; i<numComps; i++) {
        std::string tmp;
        std::cin >> tmp;
        if (!isNumber(tmp)) {
            oneHandleBools[i] = true;
            framings[i] = 0;
        }
        else {
            framings[i] = std::stoi(tmp);
        }
    }
    
    std::cout << std::endl;
    
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
            std::cout << "Component " << i << ": 2-handle with framing " << framings[i] << std::endl;
        }
        else {
            std::cout << "Component " << i << ": 1-handle" << std::endl;
        }
    }
    
    for (int i=0; i<numComps; i++) {
        long w = compWrithes[i];
        if (w > framings[i]) {
            std::cout << "Self-framing component " << i << "...\n";
            do {
                tmpLinkObj->r1(tmpLinkObj->component(i), 0 /* left */, -1, false, true);
                --w;
            } while (w != framings[i]);
        } else if (w < framings[i]) {
            std::cout << "Self-framing component " << i << "...\n";
            do {
                tmpLinkObj->r1(tmpLinkObj->component(i), 0 /* left */, 1, false, true);
                ++w;
            } while (w != framings[i]);
        }
        // If building a 4-manifold:
        // Check number of crossings in i-th component: if < |framing|+2, suggest adding pair of cancelling twists (in order to guarantee existence of a quadricolour).
        if ((dim_flag_int == 4) && !(oneHandleBools[i]) && (compCrossingNums[i] < abs(framings[i])+2)) {
            std::cout << "Adding additional pair of cancelling curls to component " << i << " to guarantee existence of a quadricolour...\n";
            tmpLinkObj->r1(tmpLinkObj->component(i), 0 /* left */, 1, false, true);
            tmpLinkObj->r1(tmpLinkObj->component(i), 0 /* left */, -1, false, true);
        }
        
    }
    
    std::cout << "Link should now be self-framed:\n";
    for (int i=0; i<compWrithes.size(); i++) {
        std::cout << "Component " << i << ": " << tmpLinkObj->writheOfComponent(i) << "\n";
    }
    
//    std::cout << std::endl;
    
    std::vector<int> oneHandleLinkIndices;
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            oneHandleLinkIndices.push_back(i);
        }
    }
    /*
     debug only
     */
//    for (auto &e : oneHandleLinkIndices) {
//        std::cout << "Component " << e << " of " << numComps << ", is a 1-handle" << std::endl;
//    }
    /*
     end
     */
    
    // 1-handle components:
    std::vector<regina::StrandRef> oneHandleComponentRefs;
    for (auto &e : oneHandleLinkIndices) {
        oneHandleComponentRefs.push_back(tmpLinkObj->component(e));
    }
//    std::cout << tmpLinkObj->brief() << std::endl;
    size_t numOneHandles = oneHandleLinkIndices.size();
    std::vector<std::vector<std::pair<int, int>>> oneHandleCSX(numOneHandles); // CSX: Component Strand Crossing
    for (int i=0; i<numOneHandles; i++) {
        auto currentStrand = oneHandleComponentRefs[i];
        std::cout << "Component " << i << std::endl;
        do {
            int currentStrandCrossing = currentStrand.crossing()->index();
            if (currentStrand.strand() == 0) {
                /*
                 Just for debug purposes
                 */
                std::cout << "U" << currentStrandCrossing << ", ";
                /*
                 End
                 */
                oneHandleCSX[i].emplace_back(0, currentStrandCrossing);
            }
            if (currentStrand.strand() == 1) {
                /*
                 Just for debug purposes
                 */
                std::cout << "O" << currentStrandCrossing << ", ";
                /*
                 End
                 */
                oneHandleCSX[i].emplace_back(1, currentStrandCrossing);
            }
            currentStrand = currentStrand.next();
        } while (currentStrand != oneHandleComponentRefs[i]);
        std::cout << std::endl;
    }
    std::vector<std::vector<std::pair<int, int>>> oneHandleMarkedCrossings(numOneHandles);
    for (int i=0; i<oneHandleCSX.size(); i++) {
        for (int j=0; j<oneHandleCSX[i].size(); j++) {
            if ((oneHandleCSX[i][j].first != oneHandleCSX[i][(j+1)%oneHandleCSX[i].size()].first) || (oneHandleCSX[i][j].first != oneHandleCSX[i][(j-1)%oneHandleCSX[i].size()].first)) {
                oneHandleMarkedCrossings[i].push_back(oneHandleCSX[i][j]);
            }
            else {
                //std::cout << oneHandleCSX[i][j] << std::endl;
            }
        }
    }
    std::cout << std::endl;
    std::cout << "Marked crossings of 1-handles:\n";
    for (int i=0; i<oneHandleMarkedCrossings.size(); i++) {
        std::cout << "1-handle " << i << ":\n";
        for (auto &e : oneHandleMarkedCrossings[i]) {
            std::cout << e << std::endl;
        }
    }
    std::cout << std::endl;
    std::vector<int> twoHandleLinkIndices;
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
//            std::cout << i << std::endl;
            twoHandleLinkIndices.push_back(i);
        }
    }
    size_t numTwoHandles = twoHandleLinkIndices.size();
    std::vector<regina::StrandRef> twoHandleComponentRefs;
    std::vector<std::vector<std::tuple<int, int,int>>> twoHandleStrandCrossingIndex(numTwoHandles);
    for (auto &e : twoHandleLinkIndices) {
        twoHandleComponentRefs.push_back(tmpLinkObj->component(e));
    }
    for (int i=0; i<numTwoHandles; i++) {
        auto currentStrand = twoHandleComponentRefs[i];
        do {
            if (currentStrand.crossing()->index() == currentStrand.next().crossing()->index()) {
                twoHandleStrandCrossingIndex[i].emplace_back(1, currentStrand.strand(),currentStrand.crossing()->index());
                std::cout << "Curl, " << currentStrand.strand() << " " << currentStrand.crossing()->index() << std::endl;
                currentStrand = currentStrand.next().next();
            }
            else {
                twoHandleStrandCrossingIndex[i].emplace_back(0, currentStrand.strand(),currentStrand.crossing()->index());
                std::cout << "Standard crossing, " << currentStrand.strand() << " " << currentStrand.crossing()->index() << std::endl;
                currentStrand = currentStrand.next();
            }
            
        } while (currentStrand != twoHandleComponentRefs[i]);
        std::cout << std::endl;
    }
    
    std::vector<std::vector<std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>>>> quadriCandidates(numTwoHandles);
    
    for (int i=0; i<twoHandleStrandCrossingIndex.size(); i++) {
        size_t currentLen = twoHandleStrandCrossingIndex[i].size();
        for (int j=0; j<currentLen; j++) {
            auto current = twoHandleStrandCrossingIndex[i][j];
//            std::cout << current << std::endl;
            auto next = twoHandleStrandCrossingIndex[i][(j+1)%currentLen];
            if (
                (
                (std::get<0>(current) == 0) &&
                (std::get<1>(current) == 0) &&
                (std::get<0>(next) == 1)
                 ) ||
                (
                (std::get<0>(current) == 1) &&
                (std::get<0>(next) == 0) &&
                (std::get<1>(next) == 0)
                 ) ||
                (
                (std::get<0>(current) == 1) &&
                (std::get<1>(current) == 0) &&
                (std::get<0>(next) == 1) &&
                (std::get<1>(next) == 0)
                 ) ||
                (
                (std::get<0>(current) == 1) &&
                (std::get<1>(current) == 1) &&
                (std::get<0>(next) == 1) &&
                (std::get<1>(next) == 1)
                 ) ||
                (
                (std::get<0>(current) == 1) &&
                (std::get<1>(current) == 0) &&
                (std::get<0>(next) == 1) &&
                (std::get<1>(next) == 1)
                 ) ||
                (
                (std::get<0>(current) == 1) &&
                (std::get<1>(current) == 1) &&
                (std::get<0>(next) == 1) &&
                (std::get<1>(next) == 0)
                 )
                ) {
                quadriCandidates[i].emplace_back(current, next);
//                std::cout << "Quadricolour candidate: " << current << "; " << next << std::endl;
            }
        }
//        std::cout << std::endl;
    }
    for (auto &v : quadriCandidates) {
        for (auto &e : v) {
            std::cout << e.first << "; " << e.second << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Note: The reflection below is just a quick hack to fix orientations (that is, make consistent with SnapPy+Regina).
    tmpLinkObj->reflect();
    
    pdcode pdc = tmpLinkObj->pdData();
    
    delete tmpLinkObj;

    
    return 0;
}
