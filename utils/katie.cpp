/**************************************************************************
 *                                                                        *
 *  Katie - Kirby Diagrams to Graphs and Triangulations                   *
 *  Builds a triangulation of a 3- or 4-manifold                          *
 *  from a decorated link diagram                                         *
 *                                                                        *
 *  Copyright (c) 2021-2025, Rhuaidi Antonio Burke                        *
 *  For further details contact Rhuaidi Burke (r.antonio.burke@gmail.com).*
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <array>
#include <tuple>
#include <set>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <stack>
#include <string>
#include <sstream>
#include <numeric>
#include <unistd.h>
#include <string>

#include <triangulation/dim3.h>
#include <triangulation/dim4.h>
#include <link/link.h>

bool printDebugInfo = false;

typedef std::vector<std::array<int, 4>> pdcode;

std::ostream& operator<<(std::ostream& os, const std::array<int, 4>& arr) {
    os << "(" << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << ")";
    return os;
}

struct node {
    int nodeID = -1;
    int strand = -1; // This comes from the PD code
    int subgraphComponent = -1; // This is a component ID w.r.t to the subgraphs.
};

const node emptyNode = node{-1,-1,-1};

struct edge {
    node n1 {};
    node n2 {};
    int colour {};
};

std::ostream& operator<<(std::ostream& os, const node& n) {
    os << "(" << n.nodeID << ", " << n.subgraphComponent << ")";
    return os;
}

bool operator ==(const node& x, const node& y) {
    return ((x.nodeID == y.nodeID) && (x.strand == y.strand) && (x.subgraphComponent == y.subgraphComponent));
}

bool operator <(const node& x, const node& y) {
    return std::tie(x.nodeID, x.strand, x.subgraphComponent) < std::tie(y.nodeID, y.strand, y.subgraphComponent);
}

template <typename T>
bool contains(std::vector<T> a, std::vector<T> b) {
    for (const auto& a_element : a) {
        if (std::find(b.begin(),b.end(), a_element) == b.end()) {
            return false;
        }
    }
    return true;
}

bool isCurl(const regina::StrandRef &ref) {
    
    bool ans = false;
    
    long refCrossingIndex = ref.crossing()->index();
    long nextRefCrossingIndex = ref.next().crossing()->index();
    long prevRefCrossingIndex = ref.prev().crossing()->index();

    // Doing this very "safely"...
    if (refCrossingIndex == nextRefCrossingIndex) {
        ans = true;
    }
    if (refCrossingIndex == prevRefCrossingIndex) {
        ans = true;
    }
    
    return ans;
}

template <int dim>
class graph {
    std::map<node, std::array<node,dim+1>> adjList;
    
public:
    
    std::map<node, std::array<node,dim+1>> adjacencyList() const {
        return adjList;
    }
    
    void fromAdjacencyList(const std::map<node, std::array<node,dim+1>>& graphData) {
        adjList = graphData;
    }
        
    void addEdge(edge e) {
        adjList[e.n1][e.colour] = e.n2;
        adjList[e.n2][e.colour] = e.n1;
    }
    
    void addEdges(const std::vector<edge>& el) {
        for (const auto& e : el) {
            addEdge(e);
        }
    }
    
    std::vector<node> nodes() const {
        std::vector<node> nodeList;
        
        for (const auto& [key,val] : adjList) {
            nodeList.emplace_back(key);
        }
        
        return nodeList;
    }
    
    std::vector<edge> edges() const {
        std::vector<edge> edgeList;
        
        for (const auto& [node, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((node < nbrs[i]) && (node.nodeID != 0)) {
                    edgeList.push_back({node,nbrs[i],i});
                }
            }
        }
        
        return edgeList;
    }
    
    static int uniqueID() {
        /*
         TODO: Make thread safe?
         Mutex seemed to break things,
         probably just used it wrong.
         But so far, nothing has broken...
         */
        
        static int nextID = 0;
        return nextID++;
    }
    
    void disjoint_union(graph<dim> h) {
        std::vector<edge> hEdges = h.edges();
        
        int currentID = uniqueID();
        
        for (auto e : hEdges) {
            node n1, n2;
            n1 = e.n1;
            n2 = e.n2;
            n1.subgraphComponent = currentID;
            n2.subgraphComponent = currentID;

            int col = e.colour;
            
            adjList[n1][col] = n2;
            adjList[n2][col] = n1;
        }
    }
    
    size_t size() {
        return adjList.size();
    }
    
    void print() {
        for (const auto& [n,nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((n < nbrs[i]) && (n.nodeID != 0)) {
                    std::cout << "[" << n << "," << nbrs[i] << "," << i << "],\n";
                }
            }
        }
    }
    
    void printNodes() {
        for (const auto& [key,val] : adjList) {
            std::cout << key << "\n";
        }
        std::cout << std::endl;
    }
    
    void pdSub(const pdcode& code) {
        /*
         Debating whether to make this a
         standalone function which operates
         on a graph, rather than a method
         of the graph class...
         */
        
        node newNbr;
        
        for (auto [currNode, nbrs] : adjList) {
            for (int i=0; i<4; i++) {
                if (nbrs[i].nodeID != 0) {
                    switch (nbrs[i].strand) {
                        case 0:
                            break;
                        case 1:
                            newNbr = {nbrs[i].nodeID, code[currNode.subgraphComponent][0],nbrs[i].subgraphComponent};
                            adjList.erase(nbrs[i]);
                            adjList[currNode][i] = newNbr;
                            adjList[newNbr][i] = currNode;
                            break;
                        case 2:
                            newNbr = {nbrs[i].nodeID, code[currNode.subgraphComponent][1],nbrs[i].subgraphComponent};
                            adjList.erase(nbrs[i]);
                            adjList[currNode][i] = newNbr;
                            adjList[newNbr][i] = currNode;
                            break;
                        case 3:
                            newNbr = {nbrs[i].nodeID, code[currNode.subgraphComponent][2],nbrs[i].subgraphComponent};
                            adjList.erase(nbrs[i]);
                            adjList[currNode][i] = newNbr;
                            adjList[newNbr][i] = currNode;
                            break;
                        case 4:
                            newNbr = {nbrs[i].nodeID, code[currNode.subgraphComponent][3],nbrs[i].subgraphComponent};
                            adjList.erase(nbrs[i]);
                            adjList[currNode][i] = newNbr;
                            adjList[newNbr][i] = currNode;
                            break;
                    }
                }
            }
        }
    }
    
    std::vector<std::pair<node,node>> fuseList() {
        /*
         Debating whether to make this a
         standalone function which operates
         on a graph, rather than a method
         of the graph class...
         */

        /*
         Let N_i = (c_i, n_i, s_i), V_j = (c_j, n_j, s_j)
         Criteria in the if statement below are as follows:
         1. Avoids duplicate pairs (works because elements are ordered).
         2. Only operate on "outer" nodes ("internal" nodes denoted via s = 0).
         3. c_i ≠ c_j (different "components")
         4. s_i = s_j (same strand/PD element)
         5. n_j mod 4 = (5 - (n_i mod 4)) mod 4.
         */
        
        std::vector<std::pair<node,node>> result;
        
        for (auto const& [n1,nbrs1] : adjList) {
            for (auto const& [n2,nbrs2] : adjList) {
                if (
                    (n1.subgraphComponent < n2.subgraphComponent) &&
                    (n1.strand !=0) && (n2.strand != 0) &&
                    (n1.strand == n2.strand) &&
                    ((n1.nodeID)%4 == (5-((n2.nodeID)%4))%4)
                    ) {
                        result.emplace_back(n1,n2);
                }
            }
        }
        
        return result;
    }
    
    void fuse(node n1, node n2) {
        std::array<node,dim+1> n1nbrs, n2nbrs;
        n1nbrs = adjList[n1];
        n2nbrs = adjList[n2];
        
        adjList.erase(n1);
        adjList.erase(n2);
        
        for (int i=0; i<dim+1; i++) {
            adjList[n1nbrs[i]][i] = n2nbrs[i];
            adjList[n2nbrs[i]][i] = n1nbrs[i];
        }
        
        adjList.erase({0,0,0});
    }
    
    void addQuadriEdges(const std::vector<std::array<node,4>>& quadriVect) {
        for (const auto& quadri : quadriVect) {
            adjList[quadri[0]][4] = quadri[1];
            adjList[quadri[1]][4] = quadri[0];
            
            adjList[quadri[2]][4] = quadri[3];
            adjList[quadri[3]][4] = quadri[2];
            
            node P4 = adjList[quadri[3]][1];
            node P5 = adjList[quadri[0]][1];
            
            adjList[P4][4] = P5;
            adjList[P5][4] = P4;
        }
    }
    
    void addQuadriEdgesRealBdry(const std::vector<std::array<node,4>>& quadriList) {
        long numQuadris = quadriList.size();
        
        for (int i=0; i<numQuadris; i++) {
            std::array<node,4> currQuadri = quadriList[i];
            
            node R1 = {-(i+1),-1,0};
            node R2 = {-(i+1),-2,0};
            node R3 = {-(i+1),-3,0};
            node R1d = {-(i+1),-4,0};
            node R2d = {-(i+1),-5,0};
            node R3d = {-(i+1),-6,0};
            
            adjList[currQuadri[0]][0] = R1d;
            adjList[currQuadri[0]][3] = R3d;
            adjList[currQuadri[1]][0] = R1;
            adjList[currQuadri[1]][1] = R1;
            adjList[currQuadri[2]][1] = R2;
            adjList[currQuadri[2]][2] = R2;
            adjList[currQuadri[3]][2] = R3;
            adjList[currQuadri[3]][3] = R3;

            adjList[R1d][0] = currQuadri[0];
            adjList[R1d][1] = R2d;
            adjList[R1d][2] = R1;
            adjList[R1d][3] = R2d;
            adjList[R1d][4] = R1;

            adjList[R2d][0] = R3d;
            adjList[R2d][1] = R1d;
            adjList[R2d][2] = R3d;
            adjList[R2d][3] = R1d;
            adjList[R2d][4] = R2;

            adjList[R3d][0] = R2d;
            adjList[R3d][1] = R3;
            adjList[R3d][2] = R2d;
            adjList[R3d][3] = currQuadri[0];
            adjList[R3d][4] = R3;
            
            adjList[R1][0] = currQuadri[1];
            adjList[R1][1] = currQuadri[1];
            adjList[R1][2] = R1d;
            adjList[R1][3] = R2;
            adjList[R1][4] = R1d;

            adjList[R2][0] = R3;
            adjList[R2][1] = currQuadri[2];
            adjList[R2][2] = currQuadri[2];
            adjList[R2][3] = R1;
            adjList[R2][4] = R2d;

            adjList[R3][0] = R2;
            adjList[R3][1] = R3d;
            adjList[R3][2] = currQuadri[3];
            adjList[R3][3] = currQuadri[3];
            adjList[R3][4] = R3d;
        }
    }
    
    void addDoubleOneEdges() {
        for (const auto& [key,nbrs] : adjList) {
            if (
                (key < nbrs[1]) &&
                (adjList[key][4] == emptyNode) &&
                (adjList[nbrs[1]][4] == emptyNode)
                ) {
                    edge e = {key,nbrs[1],4};
                    addEdge(e);
            }
        }
    }
    
    void addOneHandleMarkerEdges(std::vector<std::pair<node,node>> markerNodePairs) {
        if (printDebugInfo) {
            std::clog << "Adding 1-handle marked edges..." << std::endl;
        }
        for (const auto& pair : markerNodePairs) {
            edge e = {pair.first,pair.second,4};
            addEdge(e);
        }
        if (printDebugInfo) {
            std::clog << "Successfully added 1-handle marked edges!" << std::endl;
        }
    }
    
    void addHighlightEdges(std::vector<std::vector<regina::StrandRef>> highlightCrossings) {
        if (printDebugInfo) {
            std::clog << "Adding highlight edges..." << std::endl;
        }
        std::vector<node> allNodes = nodes();
        
        std::vector<std::vector<node>> highlightOverNodes, highlightUnderNodes, highlightCurlNodes;
        
        for (const auto& highlightVect : highlightCrossings) {
            for (const auto& ref : highlightVect) {
                std::vector<node> currCrossingNodes;
                for (const auto& n : allNodes) {
                    if (n.subgraphComponent == ref.crossing()->index()) {
                        currCrossingNodes.emplace_back(n);
                    }
                }
                if ((ref.strand() == 0) && !(isCurl(ref))) {
                    highlightUnderNodes.emplace_back(currCrossingNodes);
                }
                else if ((ref.strand() == 1) && !(isCurl(ref))) {
                    highlightOverNodes.emplace_back(currCrossingNodes);
                }
                else if (isCurl(ref)) {
                    highlightCurlNodes.emplace_back(currCrossingNodes);
                }

            }
        }
        
        // Under
        for (const auto& vect : highlightUnderNodes) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if (
                            ((x.nodeID == 1) && (y.nodeID == 6)) ||
                            ((x.nodeID == 2) && (y.nodeID == 5)) ||
                            ((x.nodeID == 3) && (y.nodeID == 4)) ||
                            ((x.nodeID == 7) && (y.nodeID == 8))
                            ) {
                            edge e = {x,y,4};
                            addEdge(e);
                        }
                    }
                }
            }
        }

        // Over
        for (const auto& vect : highlightOverNodes) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if (
                            ((x.nodeID == 1) && (y.nodeID == 2)) ||
                            ((x.nodeID == 5) && (y.nodeID == 6))
                            ) {
                            edge e = {x,y,4};
                            addEdge(e);
                        }
                    }
                }
            }
        }

        // Curl
        for (const auto& vect : highlightCurlNodes) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if ((adjList[x][4] == emptyNode) && (adjList[y][4] == emptyNode)) {
                            if (
                                ((x.nodeID == 1) && (y.nodeID == 4)) ||
                                ((x.nodeID == 2) && (y.nodeID == 3))
                                ) {
                                edge e = {x,y,4};
                                addEdge(e);
                            }
                        }
                    }
                }
            }
        }
        
        if (printDebugInfo) {
            std::clog << "Successfully added highlight edges!" << std::endl;
        }

    }
    
    void addRemainderEdges() {
        if (printDebugInfo) {
            std::clog << "Adding remainder edges..." << std::endl;
        }
        std::vector<node> allNodes = nodes();
        for (const auto& x : allNodes) {
            if (!(x == emptyNode)) {
                if (adjList[x][4] == emptyNode) {
                    node y = x;
                    int i;
                    int j = 0;
                    do {
                        i = 4*(j%2)+(j+1)%2;
                        y = adjList[y][i];
                        j+=1;
                    } while (!(adjList[y][4] == emptyNode));
                    edge e = {x,y,4};
                    addEdge(e);
                }
            }
        }
        if (printDebugInfo) {
            std::clog << "Successfully added remainder edges!" << std::endl;
        }
    }
    
    void DEBUGremainingNoCol4Nodes() {
        std::vector<node> allNodes = nodes();
        int counter = 0;
        for (const auto& n : allNodes) {
            if (adjList[n][4] == emptyNode) {
                counter++;
//                std::clog << n << " has no colour 4 neighbour.\n";
            }
        }
        std::clog << "Remaining nodes without a colour 4 edge: " << counter << std::endl;
    }
        
    void cleanup() {
        adjList.erase(emptyNode);
    }
};

std::vector<std::array<node, 4>> findGraphQuadricolours(const graph<4>& G) {
    std::vector<std::array<node, 4>> ans;
    
    std::map<node, std::array<node, 5>> adjList = G.adjacencyList();
    
    std::array<node, 2> tmp;
    
    for (const auto& [n, nbrs] : adjList) {
        if (!(n == emptyNode)) {
            tmp[0] = adjList[n][0];
            tmp[1] = adjList[n][3];
            if (adjList[tmp[0]][1] == adjList[tmp[1]][2]) {
                std::array<node, 4> currentQuadri;
                currentQuadri[0] = n;
                currentQuadri[1] = tmp[0];
                currentQuadri[2] = adjList[tmp[0]][1];
                currentQuadri[3] = tmp[1];
                ans.emplace_back(currentQuadri);
            }
        }
    }
    
    return ans;
}

void walkAroundLink(regina::Link lnk) {
    std::cout << "Debug link walkaround:" << std::endl;
    for (const auto& comp : lnk.components()) {
        auto ref = comp;
        do {
            std::cout << ref << ", ";
            ref = ref.next();
        } while (ref != comp);
        std::cout << std::endl;
    }
}

std::vector<std::pair<regina::StrandRef,regina::StrandRef>> findLinkQuadriPairs(const regina::StrandRef &twoHandle) {
    std::vector<std::pair<regina::StrandRef,regina::StrandRef>> result;
    auto currentRef = twoHandle;
    do {
        auto next = currentRef.next();
        if (isCurl(currentRef)) {
            // The current crossing is a curl, and the next one is an undercrossing.
            if (!isCurl(next) && (next.strand() == 0)) {
                result.emplace_back(currentRef,next);
            }
            // The current crossing is a curl, and the next one is a curl of the same sign.
            if (isCurl(next)) {
                if (next.crossing()->index() == currentRef.crossing()->index()) {
                    auto next2 = next.next();
                    if ((isCurl(next2)) && (next2.strand() == currentRef.strand())) {
                        result.emplace_back(currentRef,next2);
                    }
                }
            }
        }
        else {
            // The current crossing is an undercrossing and the next one is a curl.
            if ((currentRef.strand() == 0) && isCurl(next)) {
                result.emplace_back(next,currentRef);
            }
        }
        currentRef = currentRef.next();
    } while (currentRef != twoHandle);
    
    return result;
}

std::vector<int> pdCodeXTypes(const pdcode& code) {
    /*
     Assigns an identifier to each crossing of the link
     based on the PD code tuple, distinguishing between
     a "true" crossing and the four different PD code
     tuples that can arise from a curl.
     */
    std::vector<int> result;
    
    for (const auto& x : code) {
        if (x[2]==x[3]) {
            // (a,b,x,x) Positive
            result.emplace_back(1);
        }
        else if (x[0]==x[1]) {
            // (x,x,c,d) Positive
            result.emplace_back(2);
        }
        else if (x[1]==x[2]) {
            // (a,x,x,d) Negative
            result.emplace_back(3);
        }
        else if (x[0]==x[3]) {
            // (x,b,c,x) Negative
            result.emplace_back(4);
        }
        else {
            // regular crossing
            result.emplace_back(0);
        }
    }
    
    return result;
}

std::vector<int> pdCodeOrientations(const pdcode& code) {
    std::array<int, 4> eovInit = {0,0,0,0};
    
    std::array<int, 4> negative = {1,1,-1,-1};
    std::array<int, 4> positive = {1,-1,-1,1};
    
    long pdLength = code.size();

    std::vector<std::array<int, 4>> extendedOrientationVector(pdLength,eovInit);
        
    std::vector<std::vector<bool>> visited(pdLength,std::vector<bool>(4,false));
    std::vector<int> seenStrands;
    
    int i = 0, j = 0;
    int currentStrand = code[i][j];
    int count = 1;
    
    while (!visited[i][j]) {
        
        bool carry = false;
        int carryRow = 0; // Thanks Lucy for pointing out initialisation bug.

        visited[i][j] = true;
        seenStrands.emplace_back(currentStrand);

        if (count%2 == 1) {
            extendedOrientationVector[i][j] = 1;
        }
        else {
            extendedOrientationVector[i][j] = -1;
        }
        count++;
        
        j = (j+2)%4;
        
        currentStrand = code[i][j];
        visited[i][j] = true;
        seenStrands.emplace_back(currentStrand);
        
        if (count%2 == 1) {
            extendedOrientationVector[i][j] = 1;
        }
        else {
            extendedOrientationVector[i][j] = -1;
        }
        count++;
        
        if (std::count(seenStrands.begin(),seenStrands.end(),currentStrand) == 2) {
            carry = true;
            for (int row=0; row<pdLength; row++) {
                if (!visited[row][0]) {
                    currentStrand = code[row][0];
                    carryRow = row;
                    break;
                }
            }
        }
        
        int nextI = -1, nextJ = -1;
        if (!carry) {
            for (int row=0; row<pdLength; row++) {
                for (int col=0; col<4; col++) {
                    if (!visited[row][col] && code[row][col] == currentStrand) {
                        nextI = row;
                        nextJ = col;
                        break;
                    }
                }
                if (nextI != -1) {
                    break;
                }
            }
        }
        else {
            nextI = carryRow;
            nextJ = 0;
        }
        
        i = nextI;
        j = nextJ;
        
        if (nextI == -1 && nextJ == -1) {
            break;
        }
    }
    
    std::vector<int> orientations;

    for (const auto& x : extendedOrientationVector) {
//      DEBUG: Print the current EOV tuple.
//        std::cout << x << std::endl;
        if (x == positive) {
            orientations.push_back(1);
        }
        else if (x == negative) {
            orientations.push_back(-1);
        }
    }

    return orientations;
}

std::vector<std::pair<int,int>> pdCodeXTypeOrientations(const pdcode& code) {
    /*
     Each element in this list is a pair consisting of:
        1.  The crossing type of the current crossing --
            "true" crossing or curl.
        2.  The orientation of the current crossing.
     */
    std::vector<std::pair<int,int>> result;

    std::vector<int> pdcXTypes = pdCodeXTypes(code);
    std::vector<int> pdcOrientations = pdCodeOrientations(code);
    
    for (size_t i=0; i<code.size(); i++) {
        result.emplace_back(pdcXTypes[i],pdcOrientations[i]);
    }
    
    return result;
}

long getIndex(std::vector<node> n, node K) {
    // 2022 Rewrite: (Doc) Given a list of nodes n, and a node K, return the index of K in n.
    long ans = -1;
    
    auto it = std::find_if(n.begin(),n.end(), [&tNode = K](const node& cNode) -> bool {return tNode == cNode;});
    
    if (it != n.end()) {
        long index = it - n.begin();
        ans = index;
    }
    else {
        ans = -1;
    }
    
    return ans;
}

template <int dimP1>
std::vector<std::tuple<int,int,int>> gluingList(graph<dimP1> G) {
    std::vector<std::tuple<int,int,int>> result;
    std::vector<node> nodes = G.nodes();
    std::vector<edge> edges = G.edges();
    result.reserve(edges.size());
    for (const auto& e : edges) {
        result.emplace_back(getIndex(nodes,e.n1),getIndex(nodes,e.n2),e.colour);
    }
    return result;
}

template <int dimP1>
void printGluingList(graph<dimP1> G) {
    std::vector<node> nodes = G.nodes();
    std::vector<edge> edges = G.edges();
    for (const auto& e : edges) {
        // Print commas after everything except the last line :D
        if (&e != &edges.back()) {
            std::cout << "[" << getIndex(nodes,e.n1) << ", " << getIndex(nodes,e.n2) << ", " << e.colour << "],\n";
        }
        else {
            std::cout << "[" << getIndex(nodes,e.n1) << ", " << getIndex(nodes,e.n2) << ", " << e.colour << "]\n";
        }
    }
}

void printDebug(const std::string& msg, bool breakline) {
    if (printDebugInfo) {
        std::clog << msg;
        if (breakline) {
            std::clog << std::endl;
        }
    }
}

void usage(const char* progName, const std::string& error = std::string()) {
    if (!error.empty()) {
        std::cerr << error << "\n\n";
    }
    
    std::cerr << "Usage:" << std::endl;
    std::cerr << "    " << progName << " \"PD Code\" \"Framing Vector\", "
        " { -3, --dim3 | -4, --dim4 } "
//        "[ -g, --graph ] [ -r, --real ] [ -d, --debug ]\n"
    "[ -g, --graph ] [ -V, --verbose ]\n"
        "    " << progName << " [ -v, --version | -?, --help ]\n\n";
    std::cerr << "    -3, --dim3    : Build a 3-manifold via integer "
        "Dehn surgery.\n";
    std::cerr << "    -4, --dim4    : Build a 4-manifold by attaching "
        "1- and 2-handles along a decorated link.\n";
    std::cerr << "                    The PD code must be the first argument and wrapped with quotation marks.\n";
    std::cerr << "                    The framing sequence must be the second argument and wrapped with quotation marks.\n";
    std::cerr << "                    Use 'x' or '.' to denote 1-handles within the framing sequence.\n\n";
    std::cerr << "    -g, --graph   : Output an edge-coloured graph, "
        "not an isomorphism signature.\n";
//    std::cerr << "    -r, --real    : Builds the 4-manifold triangulation with real boundary "
//            "(not ideal or closed).\n";
    std::cerr << "                    This option is incompatible with the --dim3 flag.\n\n";
    std::cerr << "    -V, --verbose : Display information during the construction.\n";
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help    : Display this help\n\n";

    std::cerr << "Example usage:\n";
    std::cerr << "./katie \"PD: [(4,8,1,9),(9,3,10,4),(1,5,2,6),(6,2,7,3),(7,5,8,10)]\" \"x 0\"\n";
    
    exit(1);
}

int main(int argc, char* argv[]) {
    
    int dimFlag = 4; // Default to build a 4-manifold.
    bool outputGraph = false; // Default to ouptut an isomorphism signature.
    // bool realBdry = false; // Default to build closed/ideal triangulation.
    
    // Check for standard arguments:
    for (int i=1; i<argc; ++i) {
        if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
            usage(argv[0]);
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            if (argc != 2)
                usage(argv[0],
                    "Option --version cannot be used with "
                        "any other arguments.");
            std::cout << PACKAGE_BUILD_STRING << std::endl;
            exit(0);
        }
    }
    
    pdcode PDCworking;
        
    /*
     START Process PD Code
     */
    std::string rawPDinput;
    
    if (argc < 3) {
        usage(argv[0], "Please provide a PD code and framing sequence.");
    }
    else {
        rawPDinput = argv[1];
    }
    
    /*
     "Sanitise" the raw input string:
     Blank everything that isn't a digit.
     Use sstream to handle 'multidigt' numbers.
     */
    for (char &c : rawPDinput) {
        if (!isdigit(c)) {
            c = ' ';
        }
    }
    
    std::stringstream ssPDC(rawPDinput);
    std::vector<int> rawPDVect;
    int currPDVal;
    while (ssPDC >> currPDVal) {
        rawPDVect.push_back(currPDVal);
    }
    
    /*
     Check if the input PD code has come from the Snappy console.
     The Snappy console indexes the strands from 0, contrary to every other place.
     If the code has come from the Snappy console, bump everything up by 1.
     */
    bool codeFromSnappy = false;
    if (std::find(rawPDVect.begin(),rawPDVect.end(),0) != rawPDVect.end()) {
        codeFromSnappy = true;
    }

    if (codeFromSnappy) {
        for (size_t i=0; i<rawPDVect.size(); i++) {
            rawPDVect[i]++;
        }
    }
    
    for (size_t i=0; i<rawPDVect.size(); i+=4) {
        std::array<int, 4> currPDTup;
        for (int j=0; j<4; j++) {
            currPDTup[j] = rawPDVect[i+j];
        }
        PDCworking.push_back(currPDTup);
    }
    /*
     END Process PD Code
     */
    
    /*
     START Process Framings
     */
    std::string rawFramingInput = argv[2];
    
    std::vector<int> framingVector, twoHandleFramings;
    std::vector<bool> isOneHandleVector;
    
    std::istringstream iss(rawFramingInput);
    std::string framingToken;
    while (std::getline(iss, framingToken, ' ')) {
        if (framingToken == "x" || framingToken == ".") {
            framingVector.push_back(0);
            isOneHandleVector.push_back(true);
        } else {
            int framingInt = std::stoi(framingToken);
            framingVector.push_back(framingInt);
            twoHandleFramings.push_back(framingInt);
            isOneHandleVector.push_back(false);
        }
    }
    /*
     END Process Framings
     */

    if (3 <= argc && argc < 8) {
        for (int i=3; i<argc; ++i) {
            if (!strcmp(argv[i], "-3") || !strcmp(argv[i], "--dim3")) {
                dimFlag = 3;
            }
            else if (!strcmp(argv[i], "-4") || !strcmp(argv[i], "--dim4")) {
                dimFlag = 4;
            }
            else if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--graph")) {
                outputGraph = true;
            }
//            else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--real")) {
//                realBdry = true;
//            }
            else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--verbose")) {
                printDebugInfo = true;
            }
            else {
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
            }
        }
    }
    
    regina::Link linkObjWorking = regina::Link::fromPD(PDCworking.begin(),PDCworking.end());
    
    size_t numberOfComponents = linkObjWorking.countComponents();
    
    bool existOneHandles = std::any_of(isOneHandleVector.begin(),isOneHandleVector.end(),[](bool isOneHandle){return isOneHandle == true;});
    
    /*
     Dedicated vectors containing references to 1- and 2-handles.
     WARNING/NOTE:  These vectors will be of size "numberOfOneHandles"
                    and "numberOfTwoHandles" respectively.
                    So indexing these vectors is done w.r.t these sizes as well.
                    This could be a potential "vector" for "mismatched index"
                    errors later on down the track, so keep these ones in mind.
     */
    std::vector<regina::StrandRef> oneHandleComponentRefs, twoHandleComponentRefs;
    for (size_t i=0; i<numberOfComponents; i++) {
        if (isOneHandleVector[i]) {
            oneHandleComponentRefs.emplace_back(linkObjWorking.component(i));
        }
        else {
            twoHandleComponentRefs.emplace_back(linkObjWorking.component(i));
        }
    }
    
    size_t numberOfOneHandles, numberOfTwoHandles;
    numberOfOneHandles = oneHandleComponentRefs.size();
    numberOfTwoHandles = twoHandleComponentRefs.size();
    
    /*
     Dedicated vectors containing the crossing indices of the 1- and 2-handles.
     */
    std::vector<std::set<int>> oneHandleCrossingIndices, twoHandleCrossingIndices;
    for (const auto& oneHandleRef : oneHandleComponentRefs) {
        std::set<int> currentOneHandleCrossingIndices;
        auto currentOneHandleRef = oneHandleRef;
        do {
            currentOneHandleCrossingIndices.emplace(currentOneHandleRef.crossing()->index());
            currentOneHandleRef = currentOneHandleRef.next();
        } while (currentOneHandleRef != oneHandleRef);
        oneHandleCrossingIndices.emplace_back(currentOneHandleCrossingIndices);
    }
    for (const auto& twoHandleRef : twoHandleComponentRefs) {
        std::set<int> currentTwoHandleCrossingIndices;
        auto currentTwoHandleRef = twoHandleRef;
        do {
            currentTwoHandleCrossingIndices.emplace(currentTwoHandleRef.crossing()->index());
            currentTwoHandleRef = currentTwoHandleRef.next();
        } while (currentTwoHandleRef != twoHandleRef);
        twoHandleCrossingIndices.emplace_back(currentTwoHandleCrossingIndices);
    }
    
    /*
     Matrix consisting of StrandRefs for crossings of 2-handles
     which "intersect" 1-handles. That is, if we have a crossing like
     
                   | <-- 2-handle
                ---|---*----
                   |  /|\
                       |____ 1-handle
     
     then this matrix contains an entry for that crossing,
     indexed w.r.t the 2-handle(s).
     */
    std::vector<std::vector<regina::StrandRef>> oneTwoCommons;
    for (const auto& twoHandleRef : twoHandleComponentRefs) {
        std::vector<regina::StrandRef> currentCommons;
        auto currentTwoHandleRef = twoHandleRef;
        do {
            for (const auto& oneHandle : oneHandleCrossingIndices) {
                for (const auto& oneHandleCrossingIndx : oneHandle) {
                    if (currentTwoHandleRef.crossing()->index() == oneHandleCrossingIndx) {
                        currentCommons.emplace_back(currentTwoHandleRef);
                    }
                }
            }
            currentTwoHandleRef = currentTwoHandleRef.next();
        } while (currentTwoHandleRef != twoHandleRef);
        oneTwoCommons.emplace_back(currentCommons);
    }
    
    // init debugging
    if (printDebugInfo) {
        for (const auto& x : framingVector) {
            std::clog << x << ", ";
        }
        std::clog << std::endl;
        for (bool x : isOneHandleVector) {
            std::clog << x << ", ";
        }
        std::clog << std::endl;
        
        if (existOneHandles) {
            std::clog << "There are " << numberOfOneHandles << " 1-handles, and " << numberOfTwoHandles << " 2-handles." << std::endl;
        }
        else {
            std::clog << "There are no 1-handles, and " << numberOfTwoHandles << " 2-handles." << std::endl;
        }
        if (existOneHandles) {
            std::clog << "1-handle crossing indices:\n";
            for (const auto& x : oneHandleCrossingIndices) {
                for (const auto& y : x) {
                    std::clog << y << ", ";
                }
                std::clog << std::endl;
            }
        }
        std::clog << "2-handle crossing indices:\n";
        for (const auto& x : twoHandleCrossingIndices) {
            for (const auto& y : x) {
                std::clog << y << ", ";
            }
            std::clog << std::endl;
        }
        
        if (existOneHandles) {
            std::clog << "1/2 Commons:\n";
            for (const auto& x : oneTwoCommons) {
                for (const auto& y : x) {
                    std::clog << y << ", ";
                }
                std::clog << std::endl;
            }
        }
    }
    // end init debugging
    
    /*
     START Framing Procedure
     */
    std::vector<regina::StrandRef> r1FramingSites;
    for (int i=0; i<numberOfTwoHandles; i++) {
        auto currentTwoHandle = twoHandleComponentRefs[i];
        auto currentCommons = oneTwoCommons[i];
        /*
         ^^^ Shouldn't be any "mismatched index" type errors here,
         since twoHandleComponentRefs is obviously indexed w.r.t 2-handles,
         but so is oneTwoCommons. So indexing them like this should be fine.
         */
        bool found = false;
        for (const auto& commonCrossing : currentCommons) {
            /*
             See if this 2-handle intersects any 1-handles.
             If it does, make sure that the next crossing as we
             travel along the 2-handle is the next common 1-handle
             intersection crossing. Stick the R1 curl between these two:
                            
                        | <-- current 2-handle
                 ...----|----*---... <-- 1-handle (under 2-handle strand)
                        |
                        | <-- Stick R1 curls here.
                        |
                 ...---------*---... <-- same 1-handle (over 2-handle strand)
                        |
             
             */
            if (std::find(currentCommons.begin(),currentCommons.end(),commonCrossing.next() ) != currentCommons.end() ) {
                r1FramingSites.emplace_back(commonCrossing);
                found = true;
                break;
            }
        }
        if (!found) {
            /*
             This 2-handle doesn't intersect any 1-handles,
             so we don't care we stick the framing curls,
             so just stick 'em wherever (from start from this ref).
             */
            r1FramingSites.emplace_back(currentTwoHandle);
        }
    }
    
    std::vector<long> oneHandleWrithes, twoHandleWrithes;
    for (const auto& twoHandle : twoHandleComponentRefs) {
        twoHandleWrithes.emplace_back(linkObjWorking.writheOfComponent(twoHandle));
    }
    for (const auto& oneHandle : oneHandleComponentRefs) {
        oneHandleWrithes.emplace_back(linkObjWorking.writheOfComponent(oneHandle));
    }
    /*
     While we're at it, check the writhes of any 1-handles.
     If they aren't 0, then this could indicate that the user
     has drawn the 1-handle in a "non-standard" way (i.e. as
     not a proper unknot), so we should alert the user and bail.
     */
    std::vector<int> badOneHandleComponentIndices;
    for (int i=0; i<numberOfOneHandles; i++) {
        auto currentOneHandleWrithe = oneHandleWrithes[i];
        if (currentOneHandleWrithe != 0) {
            badOneHandleComponentIndices.emplace_back(i);
        }
    }
    if (!badOneHandleComponentIndices.empty()) {
        std::cerr << "Error: Components ";
        for (const auto& x : badOneHandleComponentIndices) {
            std::cerr << x << ", ";
        }
        std::cerr << "are indicated as 1-handles but have non-zero writhes.\n1-handles must be drawn as \"true\" unknots.\nPlease redraw your diagram and try again.\n";
        usage(argv[0], "Bad diagram (1-handles).");
    }
    
    for (int i=0; i<numberOfTwoHandles; i++) {
        long currentWrithe = twoHandleWrithes[i];
        int currentFraming = twoHandleFramings[i];
        regina::StrandRef r1FramingSiteRef = r1FramingSites[i];
        
        if (currentWrithe > currentFraming) {
            if (printDebugInfo) {
                std::clog << "Self-framing 2-handle " << i << " (--)\n";
            }
            do {
                linkObjWorking.r1(r1FramingSiteRef, 0 /* left */, -1);
                --currentWrithe;
            } while (currentWrithe != currentFraming);
        }
        else if (currentWrithe < currentFraming) {
            if (printDebugInfo) {
                std::clog << "Self-framing 2-handle " << i << " (++)\n";
            }
            do {
                linkObjWorking.r1(r1FramingSiteRef, 0 /* left */, 1);
                ++currentWrithe;
            } while (currentWrithe != currentFraming);
        }
        /*
         This next bit doesn't always behave the way it should.
         TODO: Figure out smarter way of checking when to do this.
         The workaround for now is to do a '2nd round' check-and-fix
         which starts below. This is unnecessarily inefficient,
         but will have to do for now.
         */
        else if ((dimFlag == 4) && (currentWrithe == currentFraming)) {
            if (printDebugInfo) {
                std::clog << "Adding additional pair of cancelling curls to 2-handle " << i << " to guarantee existence of a quadricolour...\n";
            }
            linkObjWorking.r1(r1FramingSiteRef, 0, 1);
            linkObjWorking.r1(r1FramingSiteRef, 0, -1);
        }
    }
    
    /*
     Walk around the link once more and check that every 2-handle
     has either a curl-curl pair of the same sign, or a
     curl-undercrossing pair. If not, add in another pair of
     cancelling curls (opposite to the ones above) to that 2-handle.
     This is not inefficient, but will have to do for now.
     */
    for (const auto& twoHandle : twoHandleComponentRefs) {
        std::vector<std::pair<regina::StrandRef,regina::StrandRef>> tempQuadriCheck = findLinkQuadriPairs(twoHandle);
        if (tempQuadriCheck.empty()) {
            std::clog << "Adding another pair of cancelling curls to current component...\n";
            linkObjWorking.r1(twoHandle, 0, -1);
            linkObjWorking.r1(twoHandle, 0, 1);
        }
    }
    
    /*
     END Framing Procedure
     */
    
    // Debugging stage 2
    std::clog << "Writhes:\n";
    for (size_t i=0; i<numberOfComponents; i++) {
        std::clog << "Component " << i << ": ";
        long currentWrithe = linkObjWorking.writheOfComponent(i);
        if (isOneHandleVector[i]) {
            std::clog << "1-handle (" << currentWrithe << ")\n";
        }
        else {
            std::clog << "2-handle, writhe " << currentWrithe;
            if (currentWrithe != framingVector[i]) {
                std::cerr << " ≠ " << framingVector[i] << "! Ruh-roh! Something's gone wrong during the framing process; we shouldn't be here!\n";
                exit(1);
            }
            else {
                std::clog << std::endl;
            }
        }
    }
    // end debugging
    
    /*
     Since the framing procedure changes the link,
     we need to recompute relevant link data so that
     indices, etc. match between the link object and
     graph objects generated later on.
     */
    regina::Link linkObjFramed = regina::Link(linkObjWorking.pd());
    linkObjWorking = linkObjFramed;
    
    // Recompute reference vectors
    oneHandleComponentRefs.clear();
    twoHandleComponentRefs.clear();
    for (size_t i=0; i<numberOfComponents; i++) {
        if (isOneHandleVector[i]) {
            oneHandleComponentRefs.emplace_back(linkObjWorking.component(i));
        }
        else {
            twoHandleComponentRefs.emplace_back(linkObjWorking.component(i));
        }
    }
    
    // Recompute crossing indices
    oneHandleCrossingIndices.clear();
    twoHandleCrossingIndices.clear();
    for (const auto& oneHandleRef : oneHandleComponentRefs) {
        std::set<int> currentOneHandleCrossingIndices;
        auto currentOneHandleRef = oneHandleRef;
        do {
            currentOneHandleCrossingIndices.emplace(currentOneHandleRef.crossing()->index());
            currentOneHandleRef = currentOneHandleRef.next();
        } while (currentOneHandleRef != oneHandleRef);
        oneHandleCrossingIndices.emplace_back(currentOneHandleCrossingIndices);
    }
    for (const auto& twoHandleRef : twoHandleComponentRefs) {
        std::set<int> currentTwoHandleCrossingIndices;
        auto currentTwoHandleRef = twoHandleRef;
        do {
            currentTwoHandleCrossingIndices.emplace(currentTwoHandleRef.crossing()->index());
            currentTwoHandleRef = currentTwoHandleRef.next();
        } while (currentTwoHandleRef != twoHandleRef);
        twoHandleCrossingIndices.emplace_back(currentTwoHandleCrossingIndices);
    }
    
    // Recompute oneTwoCommons
    oneTwoCommons.clear();
    for (const auto& twoHandleRef : twoHandleComponentRefs) {
        std::vector<regina::StrandRef> currentCommons;
        auto currentTwoHandleRef = twoHandleRef;
        do {
            for (const auto& oneHandle : oneHandleCrossingIndices) {
                for (const auto& oneHandleCrossingIndx : oneHandle) {
                    if (currentTwoHandleRef.crossing()->index() == oneHandleCrossingIndx) {
                        currentCommons.emplace_back(currentTwoHandleRef);
                    }
                }
            }
            currentTwoHandleRef = currentTwoHandleRef.next();
        } while (currentTwoHandleRef != twoHandleRef);
        oneTwoCommons.emplace_back(currentCommons);
    }
    
    if (printDebugInfo) {
        std::clog << std::endl;
        std::clog << "Post-recompute walk-around:\n";
        walkAroundLink(linkObjWorking);
    }

    /*
     START 1-Handle Marked Crossings
     */
    /*
     Assuming 1-handle is traversed counter-clockwise:
     ⚪︎.first is the "leftmost" crossing, and
     ⚪︎.second is the "rightmost" crossing.
     
     TODO: Differentiate traversal directions. Until then insist 1-handles are drawn counter-clockwise, else "garbage in = garbage out".
     */
    std::vector<std::pair<regina::StrandRef,regina::StrandRef>> oneHandleMarkedCrossingRefs;
    for (const auto& oneHandle : oneHandleComponentRefs) {
        std::pair<regina::StrandRef,regina::StrandRef> currentPair;
        auto currentRef = oneHandle;
        do {
            if ((currentRef.strand() == 0) && (currentRef.next().strand() == 1)) {
                currentPair.first = currentRef;
            }
            if ((currentRef.strand() == 1) && (currentRef.next().strand() == 0)) {
                currentPair.second = currentRef.next();
            }
            currentRef = currentRef.next();
        } while (currentRef != oneHandle);
        oneHandleMarkedCrossingRefs.emplace_back(currentPair);
    }

    if (printDebugInfo) {
        if (existOneHandles) {
            std::clog << "1-Handle Marked Crossings:\n";
            for (const auto& pair : oneHandleMarkedCrossingRefs) {
                std::clog << pair.first << ", " << pair.second << std::endl;
            }
            std::clog << std::endl;
        }
    }

    /*
     END 1-Handle Marked Crossings
     */
    
    /*
     START Link Quadricolour Search
     */
    // Make a list of all possible quadricolours per 2-handle.
    // (Obviously indexed by/w.r.t 2-handle.)
    // Outer vector: 2-handle
    // Inner vector: List of quadricolours.
    // Pair: The pair of strand references which make up the quadricolour.
    std::vector<std::pair<regina::StrandRef,regina::StrandRef>> quadriPairRefs(numberOfTwoHandles);
    for (size_t i=0; i<numberOfTwoHandles; i++) {
        auto twoHandle = twoHandleComponentRefs[i];
        std::vector<std::pair<regina::StrandRef,regina::StrandRef>> currentQuadricolourList = findLinkQuadriPairs(twoHandle);
//        std::cerr << currentQuadricolourList.size() << std::endl;
        for (const auto& pair : currentQuadricolourList) {
            if (!isCurl(pair.second)) {
                quadriPairRefs[i] = pair;
                break;
            }
            else {
                quadriPairRefs[i] = pair;
                break;
            }
        }
    }
    if (printDebugInfo) {
        std::clog << "Quadricolour references:\n";
        for (const auto& pair : quadriPairRefs) {
            std::clog << pair.first << ", " << pair.second << "\n";
        }
    }
    /*
     END Link Quadricolour Search
     */
    
    /*
     START Highlighting Procedure
     */
    std::vector<std::vector<regina::StrandRef>> highlightCrossings;
    std::vector<bool> walkOppDirVec;
    if (existOneHandles) {
        for (int i=0; i<numberOfTwoHandles; i++) {
            std::vector<regina::StrandRef> currHighlighted;

            bool walkOppositeDirection = false;
            
            auto currentTwoHandle = oneTwoCommons[i];
            if (!(currentTwoHandle.empty())) {

                std::vector<regina::StrandRef> currNeeded = oneTwoCommons[i];
                
                auto currQuadri = quadriPairRefs[i];
                auto initRef = currQuadri.first;
                auto currQuadriX2 = currQuadri.second;

                // currNeeded = oneTwoCommons - startingQuadriCrossings
                for (size_t i=0; i<currNeeded.size(); i++) {
                    if (currNeeded[i] == initRef) {
                        currNeeded.erase(currNeeded.begin()+i);
                    }
                    if (currNeeded[i] == currQuadriX2) {
                        currNeeded.erase(currNeeded.begin()+i);
                    }
                }
                
                if (isCurl(initRef)) {
                    if (initRef.next().next() == currQuadriX2) {
                        walkOppositeDirection = true;
                    }
                }
                if (initRef.next() == currQuadriX2) {
                    walkOppositeDirection = true;
                }
                
                auto walken = initRef;
                if (walkOppositeDirection) {
                    if (walken.prev().crossing()->index() == walken.crossing()->index()) {
                        walken = walken.prev().prev();
                    }
                    else {
                        walken = walken.prev();
                    }
                }
                
                do {
                    if (walkOppositeDirection) {
                        if (isCurl(walken)) {
                            currHighlighted.emplace_back(walken.prev());
                            walken = walken.prev().prev();
                        }
                        else {
                            currHighlighted.emplace_back(walken);
                            walken = walken.prev();
                        }
                    }
                    else {
                        if (isCurl(walken)) {
                            currHighlighted.emplace_back(walken);
                            walken = walken.next().next();
                        }
                        else {
                            currHighlighted.emplace_back(walken);
                            walken = walken.next();
                        }
                    }
                } while (!contains(currNeeded,currHighlighted));
                
            }
            
            highlightCrossings.emplace_back(currHighlighted);
        }
    }
    /*
     END Highlighting Procedure
     */
    if (printDebugInfo && existOneHandles) {
        std::clog << "Highlighted crossings:\n";
        for (const auto& twoHandle : highlightCrossings) {
            for (const auto& ref : twoHandle) {
                std::clog << ref << ", ";
            }
            std::clog << std::endl;
        }
    }
    
    graph<4> posCross, negCross, posCurlA, posCurlB, negCurlA, negCurlB;
    graph<4> boundaryGraph;

    node n1  = { 1,0}, n2  = { 2,0}, n3  = { 3,0}, n4  = { 4,0};
    node n5  = { 5,0}, n6  = { 6,0}, n7  = { 7,0}, n8  = { 8,0};
    node n9  = { 9,1}, n10 = {10,1}, n11 = {11,1}, n12 = {12,1};
    node n13 = {13,2}, n14 = {14,2}, n15 = {15,2}, n16 = {16,2};
    node n17 = {17,3}, n18 = {18,3}, n19 = {19,3}, n20 = {20,3};
    node n21 = {21,4}, n22 = {22,4}, n23 = {23,4}, n24 = {24,4};

    node pca5 = {5,1}, pca6  = { 6,1}, pca7  = { 7,1}, pca8  = { 8,1};
    node pca9 = {9,2}, pca10 = {10,2}, pca11 = {11,2}, pca12 = {12,2};

    node pcb5 = {5,4}, pcb6  = { 6,4}, pcb7  = { 7,4}, pcb8  = { 8,4};
    node pcb9 = {9,3}, pcb10 = {10,3}, pcb11 = {11,3}, pcb12 = {12,3};

    node nca5 = {5,1}, nca6  = { 6,1}, nca7  = { 7,1}, nca8  = { 8,1};
    node nca9 = {9,4}, nca10 = {10,4}, nca11 = {11,4}, nca12 = {12,4};

    node ncb5 = {5,2}, ncb6  = { 6,2}, ncb7  = { 7,2}, ncb8  = { 8,2};
    node ncb9 = {9,3}, ncb10 = {10,3}, ncb11 = {11,3}, ncb12 = {12,3};
    
    std::vector<edge> posCrossEdgeList = {
        {n1, n6, 0},    {n1,n16,1}, {n1,n8,2},  {n1,n2,3},
        {n2, n5, 0},    {n2,n13,1}, {n2,n3,2},
        {n3, n11,0},    {n3,n12,1},             {n3,n8,3},
        {n4, n10,0},    {n4,n9, 1}, {n4,n5,2},  {n4,n7,3},
                        {n5,n24,1},             {n5,n6,3},
                        {n6,n21,1}, {n6,n7,2},
        {n7, n19,0},    {n7,n20,1},
        {n8, n18,0},    {n8,n17,1},
        {n14,n23,0},
        {n15,n22,0}
    };
    
    std::vector<edge> negCrossEdgeList = {
        {n1, n6, 0},    {n1,n24,1}, {n1,n8,2},  {n1,n2,3},
        {n2, n5, 0},    {n2,n21,1}, {n2,n3,2},
        {n3, n19,0},    {n3,n20,1},             {n3,n8,3},
        {n4, n18,0},    {n4,n17,1}, {n4,n5,2},  {n4,n7,3},
                        {n5,n16,1},             {n5,n6,3},
                        {n6,n13,1}, {n6,n7,2},
        {n7, n11,0},    {n7,n12,1},
        {n8, n10,0},    {n8,n9, 1},
        {n14,n23,0},
        {n15,n22,0}
    };
    
    std::vector<edge> posCurlAEdgeList = {
        {n1,pca6, 0},   {n1,pca9, 1},   {n1,n2,2},  {n1,n4,3},
        {n2,pca7, 0},   {n2,pca8, 1},               {n2,n3,3},
        {n3,pca10,0},   {n3,pca5, 1},   {n3,n4,2},
        {n4,pca11,0},   {n4,pca12,1}
    };
    
    std::vector<edge> posCurlBEdgeList = {
        {n1,pcb6, 0},   {n1,pcb9, 1},   {n1,n2,2},  {n1,n4,3},
        {n2,pcb7, 0},   {n2,pcb8, 1},               {n2,n3,3},
        {n3,pcb10,0},   {n3,pcb5, 1},   {n3,n4,2},
        {n4,pcb11,0},   {n4,pcb12,1}
    };
    
    std::vector<edge> negCurlAEdgeList = {
        {n1,nca6, 0},   {n1,nca5, 1},   {n1,n2,2},  {n1,n4,3},
        {n2,nca7, 0},   {n2,nca12,1},               {n2,n3,3},
        {n3,nca10,0},   {n3,nca9, 1},   {n3,n4,2},
        {n4,nca11,0},   {n4,nca8, 1}
    };
    
    std::vector<edge> negCurlBEdgeList = {
        {n1,ncb6, 0},   {n1,ncb5, 1},   {n1,n2,2},  {n1,n4,3},
        {n2,ncb7, 0},   {n2,ncb12,1},               {n2,n3,3},
        {n3,ncb10,0},   {n3,ncb9, 1},   {n3,n4,2},
        {n4,ncb11,0},   {n4,ncb8, 1}
    };
    
    posCross.addEdges(posCrossEdgeList);
    negCross.addEdges(negCrossEdgeList);
    posCurlA.addEdges(posCurlAEdgeList);
    posCurlB.addEdges(posCurlBEdgeList);
    negCurlA.addEdges(negCurlAEdgeList);
    negCurlB.addEdges(negCurlBEdgeList);
    
    pdcode pdCodeMain = linkObjWorking.pdData();
    
    std::vector<std::pair<int,int>> pdcXOTypes = pdCodeXTypeOrientations(pdCodeMain);
    
    long totalCrossingCounter = 1;
    for (const auto& pair : pdcXOTypes) {
        ++totalCrossingCounter;
        if ((pair.first == 0) && (pair.second == 1)) {
            if (printDebugInfo) {
                std::clog << "Building positive crossing graph...\n";
            }
            boundaryGraph.disjoint_union(posCross);
        }
        else if ((pair.first == 0) && (pair.second == -1)) {
            if (printDebugInfo) {
                std::clog << "Building positive negative graph...\n";
            }
            boundaryGraph.disjoint_union(negCross);
        }
        else if (pair.first == 1) {
            if (printDebugInfo) {
                std::clog << "Building positive curl (type A) graph...\n";
            }
            boundaryGraph.disjoint_union(posCurlA);
        }
        else if (pair.first == 2) {
            if (printDebugInfo) {
                std::clog << "Building positive curl (type B) graph...\n";
            }
            boundaryGraph.disjoint_union(posCurlB);
        }
        else if (pair.first == 3) {
            if (printDebugInfo) {
                std::clog << "Building negative curl (type A) graph...\n";
            }
            boundaryGraph.disjoint_union(negCurlA);
        }
        else if (pair.first == 4) {
            if (printDebugInfo) {
                std::clog << "Building negative curl (type B) graph...\n";
            }
            boundaryGraph.disjoint_union(negCurlB);
        }
    }
    
    boundaryGraph.pdSub(pdCodeMain);
    
    std::vector<std::pair<node,node>> bdryGfuseList = boundaryGraph.fuseList();
    for (auto pair : bdryGfuseList) {
        boundaryGraph.fuse(pair.first,pair.second);
    }

    if (dimFlag == 4) {
        std::vector<std::array<node,4>> graphQuadriListAll, graphQuadriListFinal(numberOfTwoHandles);
        graphQuadriListAll = findGraphQuadricolours(boundaryGraph);
        if (printDebugInfo) {
            std::clog << "Graph quadricolours:\n";
            for (const auto& quadri : graphQuadriListAll) {
                std::clog << quadri[0].subgraphComponent << ", " << quadri[1].subgraphComponent << ", " << quadri[2].subgraphComponent << ", " << quadri[3].subgraphComponent << std::endl;
            }
            std::clog << std::endl;
        }
        
        for (size_t i=0; i<numberOfTwoHandles; i++) {
            auto linkQuadri = quadriPairRefs[i];
            std::set<int> linkIndices;
            linkIndices.emplace(linkQuadri.first.crossing()->index());
            linkIndices.emplace(linkQuadri.second.crossing()->index());
            for (const auto& graphQuadri : graphQuadriListAll) {
                std::set<int> graphIndices;
                for (const auto& qn : graphQuadri) {
                    graphIndices.emplace(qn.subgraphComponent);
                }
                if (graphIndices == linkIndices) {
                    graphQuadriListFinal[i] = graphQuadri;
                    break;
                }
            }
        }
        
        if (printDebugInfo) {
            std::clog << "Final graph quadricolours:\n";
            for (const auto& graphQuadri : graphQuadriListFinal) {
                std::clog << graphQuadri[0].subgraphComponent << ", " << graphQuadri[1].subgraphComponent << ", " << graphQuadri[2].subgraphComponent << ", " << graphQuadri[3].subgraphComponent << std::endl;
            }
        }
        
        boundaryGraph.cleanup();
//        boundaryGraph.DEBUGremainingNoCol4Nodes();
        boundaryGraph.addQuadriEdges(graphQuadriListFinal);
        if (existOneHandles) {
            
            std::vector<std::pair<node,node>> oneHandleMarkedNodes;
            for (const auto& pair : oneHandleMarkedCrossingRefs) {
                int leftComp = pair.first.crossing()->index();
                int rightComp = pair.second.crossing()->index();
                std::pair<node,node> currPair;
                
                int leftOrientation = pdcXOTypes[leftComp].second;
                int rightOrientation = pdcXOTypes[rightComp].second;
                
                if (leftOrientation == 1) {
                    currPair.first = {7,0,leftComp};
                }
                if (leftOrientation == -1) {
                    currPair.first = {3,0,leftComp};
                }
                if (rightOrientation == 1) {
                    currPair.second = {4,0,rightComp};
                }
                if (rightOrientation == -1) {
                    currPair.second = {8,0,rightComp};
                }
                oneHandleMarkedNodes.emplace_back(currPair);
            }
//            boundaryGraph.DEBUGremainingNoCol4Nodes();
            boundaryGraph.addOneHandleMarkerEdges(oneHandleMarkedNodes);
//            boundaryGraph.DEBUGremainingNoCol4Nodes();
            boundaryGraph.addHighlightEdges(highlightCrossings);
//            boundaryGraph.DEBUGremainingNoCol4Nodes();
            boundaryGraph.addDoubleOneEdges();
//            boundaryGraph.DEBUGremainingNoCol4Nodes();
            boundaryGraph.addRemainderEdges();
        }
        else {
            boundaryGraph.addDoubleOneEdges();
        }
        
        if (!outputGraph) {
			//if (printDebugInfo) {
			//	std::cerr << "Building Regina triangulation..." << std::endl;
			//}
            std::vector<std::tuple<int,int,int>> boundaryGraphGluingList = gluingList(boundaryGraph);
            regina::Triangulation<4> fourTri;
            regina::Perm<5> perm;
            std::vector<regina::Simplex<4>> tmpTets;
            fourTri.newPentachora(boundaryGraph.size());
            for (const auto& gluingTrip : boundaryGraphGluingList) {
                int fromIndx, toIndx, facet;
                fromIndx = std::get<0>(gluingTrip);
                toIndx = std::get<1>(gluingTrip);
                facet = std::get<2>(gluingTrip);
				//if (printDebugInfo) {
				//	std::cerr << "Joining " << fromIndx << " to " << toIndx << " along facet " << facet << std::endl;
				//}
                fourTri.pentachoron(fromIndx)->join(facet,fourTri.pentachoron(toIndx),perm);
            }
			
            bool allIsWell = fourTri.isValid();
            if (!allIsWell) {
                std::cerr << "ERROR: Something has gone unexpectedly wrong during the construction!\n";
                std::cerr << "Please contact the developer with the PD code and framing vector which led to this output." << std::endl;
                exit(1);
            }
            
            std::clog << "\r " << std::endl;
            std::clog << "\rHere is the isomorphism signature:\n" << std::flush;
            std::cout << fourTri.isoSig() << "\n";
        }
        else {
            std::clog << "\rHere is the edge list of the coloured graph:\n" << std::flush;
            printGluingList(boundaryGraph);
        }
    }
    else if (dimFlag == 3) {
        if (!outputGraph) {
            boundaryGraph.cleanup();

            std::vector<std::tuple<int,int,int>> boundaryGraphGluingList = gluingList(boundaryGraph);
            
            regina::Triangulation<3> threeTri;
            regina::Perm<4> perm;
            std::vector<regina::Simplex<3>> tmpTets;
            threeTri.newTetrahedra(boundaryGraph.size());
            for (const auto& gluingTrip : boundaryGraphGluingList) {
                int fromIndx, toIndx, facet;
                fromIndx = std::get<0>(gluingTrip);
                toIndx = std::get<1>(gluingTrip);
                facet = std::get<2>(gluingTrip);
                threeTri.tetrahedron(fromIndx)->join(facet,threeTri.tetrahedron(toIndx),perm);
            }
            
            std::clog << "\r " << std::endl;
            std::clog << "\rHere is the isomorphism signature:\n" << std::flush;
            std::cout << threeTri.isoSig() << "\n";
        }
        else {
            std::clog << "\rHere is the edge list of the coloured graph:\n" << std::flush;
            printGluingList(boundaryGraph);
        }
    }
    
    return 0;
}
