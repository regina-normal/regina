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

typedef std::vector<std::array<int, 4>> pdcode;
std::ostream& operator<<(std::ostream& os, const std::array<int, 4>& arr) {
    os << "(" << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << ")";
    return os;
}

std::map<int, int> mod4swap = {{1,0},{2,3},{3,2},{0,1}};

struct extCrossing {
    int crossing_type = -1; // Curl (1) or Standard Crossing (0)
    int over_under = -1; // Over = 1; Under = 0
    size_t crossing = -1;
    size_t component = -1;
};

struct oneHandleSideMarker {
    int side; // Left = 0; Right = 1
    extCrossing crossingData;
};

struct vertex {
    int vertexID = -1;
    int strandID = -1;
    int component = -1;
};

vertex emptyVertex = vertex{-1,-1,-1};

struct edge {
    vertex v1 {};
    vertex v2 {};
    int colour {};
};

bool operator <(const vertex& x, const vertex& y) {
    return std::tie(x.vertexID, x.strandID, x.component) < std::tie(y.vertexID, y.strandID, y.component);
}

bool operator ==(const vertex& x, const vertex& y) {
    return ((x.vertexID == y.vertexID) && (x.strandID == y.strandID) && (x.component == y.component));
}

std::ostream& operator<<(std::ostream& os, const vertex& v) {
//    os << "(" << v.vertexID << ", " << v.strandID << ", " << v.component << ")";
    os << "(" << v.vertexID << ", " << v.component << ")";
    return os;
}

template <int dim>
class graph {
    std::map<vertex, std::array<vertex, dim+1>> adjList;
    
public:
    
    static int uniqueID() {
        // TODO: Make thread safe, but mutex seems to break things...
        static int nextID = 0;
        return nextID++;
    }
    
    std::map<vertex, std::array<vertex, dim+1>> adjacencyList() {
        return adjList;
    }
    
    void print_graph() {
        for (const auto& [vert,nbrs] : adjList) {
            std::cout << vert << ": ";
            for (const auto& v : nbrs) {
                std::cout << v << ", ";
            }
            std::cout << std::endl;
        }
    }

    void add_edge(edge e) {
        adjList[e.v1][e.colour] = e.v2;
        adjList[e.v2][e.colour] = e.v1;
    }
    
    void add_edges(const std::vector<edge>& el) {
        for (const auto& e : el) {
            add_edge(e);
        }
    }
    
    std::vector<vertex> vertices() {
        std::vector<vertex> vertList;
        
        for (const auto& [key,val] : adjList) {
            vertList.emplace_back(key);
        }
        
        return vertList;
    }
    
    std::vector<edge> edges() {
        std::vector<edge> edgeList;
        
        for (const auto& [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((vert < nbrs[i]) && (vert.vertexID != 0)) {
                    edgeList.push_back({vert,nbrs[i],i});
                }
            }
        }
        
        return edgeList;
    }
    
    void print_vertices() {
        for (const auto& [key,val] : adjList) {
            std::cout << key << "\n";
        }
        std::cout << std::endl;
    }
    
    void print_edges() {
        for (const auto& [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((vert < nbrs[i]) && (vert.vertexID != 0)) {
                    std::cout << "[" << vert << ", " << nbrs[i] << ", " << i << "],\n";
                }
            }
        }
    }
    
    void disjoint_union(graph<dim> h) {
        std::vector<edge> hEdges = h.edges();
        
        int currentID = uniqueID();
        
        for (auto e : hEdges) {
            vertex v1, v2;
            v1 = e.v1;
            v2 = e.v2;
            v1.component = currentID;
            v2.component = currentID;

            int col = e.colour;
            
            adjList[v1][col] = v2;
            adjList[v2][col] = v1;
        }
    }
    
    void pd_sub(const pdcode& code) {
        vertex newNbr_i;
        for (auto [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if (nbrs[i].vertexID != 0) {
                    switch (nbrs[i].strandID) {
                        case 0:
                            break;
                        case 1:
                            newNbr_i = {nbrs[i].vertexID,code[vert.component][0],nbrs[i].component};
                            adjList.erase(nbrs[i]);
                            adjList[vert][i] = newNbr_i;
                            adjList[newNbr_i][i] = vert;
//                            nbrs[i].strandID = code[vert.component][0];
                            break;
                        case 2:
                            newNbr_i = {nbrs[i].vertexID,code[vert.component][1],nbrs[i].component};
                            adjList.erase(nbrs[i]);
                            adjList[vert][i] = newNbr_i;
                            adjList[newNbr_i][i] = vert;
//                            nbrs[i].strandID = code[vert.component][1];
                            break;
                        case 3:
                            newNbr_i = {nbrs[i].vertexID,code[vert.component][2],nbrs[i].component};
                            adjList.erase(nbrs[i]);
                            adjList[vert][i] = newNbr_i;
                            adjList[newNbr_i][i] = vert;
//                            nbrs[i].strandID = code[vert.component][2];
                            break;
                        case 4:
                            newNbr_i = {nbrs[i].vertexID,code[vert.component][3],nbrs[i].component};
                            adjList.erase(nbrs[i]);
                            adjList[vert][i] = newNbr_i;
                            adjList[newNbr_i][i] = vert;
//                            nbrs[i].strandID = code[vert.component][3];
                            break;
                    }
                }
            }
        }
    }
    
    std::vector<std::pair<vertex, vertex>> fuse_list() {
        
        // Think this should actually be a standalone function which operates on a graph rather than a method of graph class but anyway, put it here for now/debugging...
        // (otherwise really need to do some input checking...)
        
        /*
         Let V_i = (c_i, v_i, s_i), V_j = (c_j, v_j, s_j)
         Criteria in the if statement below are as follows:
         1. Avoids duplicate pairs (works because elements are ordered).
         2. Only operate on "outer" vertices ("internal" vertices denoted via s = 0).
         3. c_i ≠ c_j (different "components")
         4. s_i = s_j (same strand/PD element)
         5. v_j mod 4 = mod4swap(v_i mod 4), where,
            mod4swap(1) = 0,
            mod4swap(2) = 3,
            mod4swap(3) = 2,
            mod4swap(0) = 1,
         */
        
        std::vector<std::pair<vertex, vertex>> ans;
        
        for (auto const& [v1,nbrs1] : adjList) {
            for (auto const& [v2,nbrs2] : adjList) {
                if (
                    (v1.component < v2.component) &&
                    (v1.strandID != 0) && (v2.strandID != 0) &&
                    (v1.component != v2.component) &&
                    (v1.strandID == v2.strandID) &&
                    ((v1.vertexID)%4 == mod4swap[(v2.vertexID)%4])) {
                    ans.push_back(std::make_pair(v1, v2));
                }
            }
        }
        
        return ans;
        
    }
    
    void fuse(vertex v1, vertex v2) {
        
        // TODO: Should sanitise things - e.g. make sure v1 has as many neighbours as v2 and that their colours match up, etc.
        
        std::array<vertex, dim+1> v1nbrs = adjList[v1]; // {v1n1,v1n2,...,v1n(dim+1)}
        std::array<vertex, dim+1> v2nbrs = adjList[v2]; // {v2n1,v2n2,...,v2n(dim+1)}

        adjList.erase(v1);
        adjList.erase(v2);

        for (int i=0; i<dim+1; i++) {
            adjList[v1nbrs[i]][i] = v2nbrs[i];
            adjList[v2nbrs[i]][i] = v1nbrs[i];
        }
        adjList.erase({0,0,0});
    }
    
    std::vector<std::array<vertex, 4>> quadriGraphFind() {
        /*
         A quadricolour is a cyclic subgraph of the form:
         
                    P1 -|-|-|- P2 ~~~~~~~ P3
                     \_                   .
                       \__                .
                          \___            .
                              \____       .
                                   \_____ P0
         
         Where: ----- = 0
                -|-|- = 1
                ~~~~~ = 2
                ..... = 3
         
         In terms of the adjacency list structure, a quadricolour will look like:
         
         P0 : {P1, **, **, P3, **}
         P1 : {P0, P2, **, **, **}
         P2 : {**, P1, P3, **, **}
         P3 : {**, **, P2, P0, **}
         
         Hence we locate a quadricolour as follows.
         Let P0 = vert, then in the adjaceny list we have:
         
         vert : {P0n1, P0n2, P0n3, P0n4, 0}
         
         We then look at P0n1's and P0n3's neighbours.
         If we are in a quadricolour, then P0n1's 2nd neighbour will be the same as P0n3's 3rd neighbour.
         This completely determines the cycle.
         */

        std::vector<std::array<vertex, 4>> ans;
        
        std::array<vertex, 2> tmp;
        
        for (const auto& [vert, nbrs] : adjList) {
            if (!(vert == emptyVertex)) {
                tmp[0] = adjList[vert][0];
                tmp[1] = adjList[vert][3];
                if (adjList[tmp[0]][1] == adjList[tmp[1]][2]) {
                    std::array<vertex, 4> currentQuadri;
                    currentQuadri[0] = vert;
                    currentQuadri[1] = tmp[0];
                    currentQuadri[2] = adjList[tmp[0]][1];
                    currentQuadri[3] = tmp[1];
                    ans.emplace_back(currentQuadri);
                }
            }
        }

        return ans;

    }
    
    size_t size() {
        size_t ans = adjList.size();
        return ans;
    }
    
    void addQuadriEdges(std::vector<std::array<vertex, 4>> quadriVect) {
        
        for (const auto& quadri : quadriVect) {
            adjList[quadri[0]][4] = quadri[1];
            adjList[quadri[1]][4] = quadri[0];
            
            adjList[quadri[2]][4] = quadri[3];
            adjList[quadri[3]][4] = quadri[2];
            
            vertex P4 = adjList[quadri[3]][1];
            vertex P5 = adjList[quadri[0]][1];
            
            adjList[P4][4] = P5;
            adjList[P5][4] = P4;
        }
        
    }
    
    void addOneHandleIdentEdges(std::vector<std::pair<vertex, vertex>> oneHandleMarkedVertPairs) {

        for (const auto& pair : oneHandleMarkedVertPairs) {
            edge e = {pair.first,pair.second,4};
            add_edge(e);
        }

    }
    
    void addHighlightEdges(std::vector<std::vector<std::pair<int, size_t>>> highlightCrossings) {
        std::vector<vertex> verts = vertices();
        
        std::vector<std::vector<vertex>> highlightGraphOverCrossingVertices;
        std::vector<std::vector<vertex>> highlightGraphUnderCrossingVertices;
        std::vector<std::vector<vertex>> highlightGraphCurlCrossingVertices;
        
        for (const auto& highlightVector : highlightCrossings) {
            for (const auto& pair : highlightVector) {
                std::vector<vertex> currentCrossingVertices;
                for (const auto& v : verts) {
                    if (v.component == pair.second) {
                        currentCrossingVertices.emplace_back(v);
                    }
                }
                if (pair.first == 0) {
                    highlightGraphUnderCrossingVertices.emplace_back(currentCrossingVertices);
                }
                else if (pair.first == 1) {
                    highlightGraphOverCrossingVertices.emplace_back(currentCrossingVertices);
                }
                else if (pair.first == 2) {
                    highlightGraphCurlCrossingVertices.emplace_back(currentCrossingVertices);
                }
            }
        }
        
        // Under
        for (const auto& vect : highlightGraphUnderCrossingVertices) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if (
                            ((x.vertexID == 1) && (y.vertexID == 6)) ||
                            ((x.vertexID == 2) && (y.vertexID == 5)) ||
                            ((x.vertexID == 3) && (y.vertexID == 4)) ||
                            ((x.vertexID == 7) && (y.vertexID == 8))
                            ) {
                            edge e = {x,y,4};
                            add_edge(e);
                        }
                    }
                }
            }
        }
        
        // Over
        for (const auto& vect : highlightGraphOverCrossingVertices) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if (
                            ((x.vertexID == 1) && (y.vertexID == 2)) ||
                            ((x.vertexID == 5) && (y.vertexID == 6))
                            ) {
                            edge e = {x,y,4};
                            add_edge(e);
                        }
                    }
                }
            }
        }
        
        // Curl
        for (const auto& vect : highlightGraphCurlCrossingVertices) {
            for (const auto& x : vect) {
                for (const auto& y : vect) {
                    if (x < y) {
                        if ((adjList[x][4] == emptyVertex) && (adjList[y][4] == emptyVertex)) {
                            if (
                                ((x.vertexID == 1) && (y.vertexID == 4)) ||
                                ((x.vertexID == 2) && (y.vertexID == 3))
                                ) {
                                edge e = {x,y,4};
                                add_edge(e);
                            }
                        }
                    }
                }
            }
        }
        
    }
    
    void addDoubleOneEdges() {
        
        for (const auto& [vert,nbrs] : adjList) {
            if (
                (vert < nbrs[1]) &&
                (adjList[vert][4] == emptyVertex) &&
                (adjList[nbrs[1]][4] == emptyVertex)
                ) {
                edge e = {vert,nbrs[1],4};
                add_edge(e);
            }
        }

    }
    
    void addRemainderEdges() {
        std::vector<vertex> verts = vertices();
        for (const auto& x : verts) {
            if (!(x == emptyVertex)) {
                if (adjList[x][4] == emptyVertex) {
                    vertex y=x;
                    int i;
                    int n=0;
                    do {
                        i = 4*(n%2)+(n+1)%2;
                        y = adjList[y][i];
                        n+=1;
                    } while (!(adjList[y][4] == emptyVertex));
                    edge e = {x,y,4};
                    add_edge(e);
                }
            }
        }
    }
    
    void tempBugFix() {
        std::vector<vertex> verts = vertices();
        for (const auto& x : verts) {
            for (const auto& y : verts) {
                if ((adjList[y][4] == x) && (adjList[x][4] == emptyVertex)) {
                    adjList[x][4] = y;
                }
            }
        }
    }
    
    void DEBUGno4ColVerts() {
        std::cout << "These vertices have no 4-coloured edge:\n";
        std::vector<vertex> verts = vertices();
        for (const auto& v : verts) {
            if (adjList[v][4] == emptyVertex) {
                std::cout << v << std::endl;
            }
        }
        std::cout << std::endl;
    }
    
    void cleanup() {
        adjList.erase(emptyVertex);
    }
    
};

long getIndex(std::vector<vertex> v, vertex K) {
    // 2022 Rewrite: (Doc) Given a list of vertices v, and a vertex K, return the index of K in v ?
    
    long ans = -1;
    
//    auto it = std::find(v.begin(), v.end(), K);
    auto it = std::find_if(v.begin(), v.end(), [&cv = K](const vertex& vert) -> bool {return cv == vert;});

    if (it != v.end()) {
        long index = it - v.begin();
        ans = index;
    }
    else {
        ans = -1;
    }
    
    return ans;
}

std::vector<std::tuple<int,int,int>> gluingList(graph<4> G) {
    std::vector<vertex> verts = G.vertices();
    std::vector<edge> edges = G.edges();
    std::vector<std::tuple<int,int,int>> ans;
    ans.reserve(edges.size());
    for (const auto& elem : edges) {
        ans.emplace_back(getIndex(verts, elem.v1),getIndex(verts, elem.v2),elem.colour);
    }
    return ans;
}

std::vector<int> pdc_orientations(pdcode code) {

    // TODO: Swap signs to be consistent with SnapPy (Currently just reflecting.)
    
    std::array<int, 4> negative = {1,-1,-1,1};
    std::array<int, 4> positive = {1,1,-1,-1};
    
    long pdlen = code.size();

    std::vector<std::array<int, 4>> orientations_extended(pdlen,{0,0,0,0});
    
    std::map<std::pair<int, int>, std::pair<int, int>> pairingIndexMap;
    
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> tempPairingList;

    for (int a=0; a<pdlen; a++) {
        for (int b=0; b<4; b++) {
            for (int c=0; c<pdlen; c++) {
                for (int d=0; d<4; d++) {
                    if ((code[c][d] == code[a][b]) && (std::make_pair(a, b) != std::make_pair(c, d))) {
                        pairingIndexMap.emplace(std::make_pair(a, b), std::make_pair(c, d));
                    }
                }
            }
        }
    }
  
    std::pair<int, int> pairingIndex(0,0);

    int ocounter = 0;

    for (int L=0; L<pdlen; L++) {
        if (orientations_extended[L][0] == 0) {
            ocounter = 0;
            pairingIndex = std::make_pair(L, 0);
            
            while (ocounter<2*pdlen) {
                
                orientations_extended[pairingIndex.first][pairingIndex.second] = 1;
                pairingIndex.second = (pairingIndex.second+2)%4;
                orientations_extended[pairingIndex.first][pairingIndex.second] = -1;
                pairingIndex = pairingIndexMap[pairingIndex];

                ++ocounter;

            }
        }
    }

    std::vector<int> orientations;

    for (const auto& x : orientations_extended) {
        if (x == positive) {
            orientations.push_back(1);
        }
        else if (x == negative) {
            orientations.push_back(-1);
        }
    }
    
    return orientations;
}

std::vector<int> pdc_xtype(const pdcode& code) {

    std::vector<int> ans;
    
    for (const auto& x : code) {
        if (x[1]==x[2]) {
            // (x,y,y,w)
            ans.emplace_back(1);
        }
        else if (x[0]==x[3]) {
            // (x,y,z,x)
            ans.emplace_back(2);
        }
        else if (x[2]==x[3]) {
            // (x,y,z,z)
            ans.emplace_back(3);
        }
        else if (x[0]==x[1]) {
            // (x,x,y,z)
            ans.emplace_back(4);
        }
        else {
            // regular crossing
            ans.emplace_back(0);
        }
    }
    
    return ans;
}

std::vector<std::pair<int, int>> pdc_xotype(const pdcode& code) {
    /*
     Probably too python-esque at the moment (running functions which create vectors, etc...
     */
    std::vector<std::pair<int, int>> ans;
    ans.reserve(code.size());

    std::vector<int> pdc_os = pdc_orientations(code);
    std::vector<int> pdc_xs = pdc_xtype(code);
    
    for (size_t i=0; i<code.size(); i++) {
        ans.emplace_back(pdc_xs[i], pdc_os[i]);
    }
    return ans;

}

graph<4> posCross, negCross, posCurlA, posCurlB, negCurlA, negCurlB;

graph<4> pd2dg(const pdcode& code) {
    
    std::vector<std::pair<int, int>> pdc_xot = pdc_xotype(code);
    graph<4> res_graph;
    
    for (auto p : pdc_xot) {
        if ((p.first == 0) && (p.second == 1)) {
            std::cout << "Generating Positive Crossing...\n";
            res_graph.disjoint_union(posCross);
        }
        else if ((p.first == 0) && (p.second == -1)) {
            std::cout << "Generating Negative Crossing...\n";
            res_graph.disjoint_union(negCross);
        }
        else if (p.first == 1) {
            std::cout << "Generating Positive Curl of Type A (x,y,y,w)...\n";
            res_graph.disjoint_union(posCurlA);
        }
        else if (p.first == 2) {
            std::cout << "Generating Positive Curl of Type B (x,y,z,x)...\n";
            res_graph.disjoint_union(posCurlB);
        }
        else if (p.first == 3) {
            std::cout << "Generating Negative Curl of Type A (x,y,z,z)...\n";
            res_graph.disjoint_union(negCurlA);
        }
        else if (p.first == 4) {
            std::cout << "Generating Negative Curl of Type A (x,x,z,w)...\n";
            res_graph.disjoint_union(negCurlB);
        }
    }
    
    res_graph.pd_sub(code);
    std::cout << "Performing PD strand substitution...\n";
    
    return res_graph;
}

std::ostream& operator << (std::ostream& os, const extCrossing& ex)
{
    os << "(" << ex.crossing_type << ", " << ex.over_under << ", " << ex.crossing << ", " << ex.component << ")";
    return os;
}

template <typename T, typename D>
std::ostream& operator<<(std::ostream& os, const std::pair<T, D> &p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T, T, T> &p)
{
    os << "[" << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << "],";
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

template <typename T>
bool isSubset(std::vector<T> A, std::vector<T> B)
{
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    return std::includes(A.begin(), A.end(), B.begin(), B.end());
}

std::vector<std::vector<extCrossing>> extendedCrossings(const regina::Link& lnk) {
    size_t numberOfComponents = lnk.countComponents();
    
    std::vector<std::vector<extCrossing>> ans;
    
    for (int i=0; i<numberOfComponents; i++) {
        std::vector<extCrossing> currentComponentExtendedCrossings;
        std::vector<extCrossing> tmp;
        
        regina::StrandRef currentCompInitRef = lnk.component(i);
        
        regina::StrandRef currentRef = currentCompInitRef;
        extCrossing currentEntry;
        
        currentEntry.component = i;
        
        do {
            size_t currentCrossingIndex = currentRef.crossing()->index();
            int currentCrossingStrand = currentRef.strand();
            currentEntry.crossing = currentCrossingIndex;
            currentEntry.over_under = currentCrossingStrand;

            if ((currentRef.next().crossing()->index() == currentCrossingIndex) || (currentRef.prev().crossing()->index() == currentCrossingIndex)) {
                currentEntry.crossing_type = 1; // Curl
            }
            else {
                currentEntry.crossing_type = 0; // Standard Crossing
            }
            tmp.emplace_back(currentEntry);
            currentRef = currentRef.next();

        } while (currentRef != currentCompInitRef);
        
        for (int i=0; i<tmp.size(); i++) {
            extCrossing a = tmp[i];
            extCrossing b = tmp[(i+1)%tmp.size()];
            if (a.crossing != b.crossing) {
                currentComponentExtendedCrossings.emplace_back(a);
            }
        }
        
        ans.emplace_back(currentComponentExtendedCrossings);
    }

//    std::vector<regina::StrandRef> componentReferences;
//    for (int i=0; i<numberOfComponents; i++) {
//        componentReferences.emplace_back(lnk.component(i));
//    }
//
//    for (int i=0; i<numberOfComponents; i++) {
//        auto currentStrand = componentReferences[i];
//        std::cout << "extendedCrossings - componentRef: " << currentStrand << std::endl;
//        std::vector<extCrossing> currentCompExtCrossings;
//        extCrossing currentEntry;
//        currentEntry.component = i;
//
//        do {
//            size_t currentCrossingNum = currentStrand.crossing()->index();
//            size_t currentStrandNum = currentStrand.strand();
//            currentEntry.crossing = currentCrossingNum;
//            currentEntry.over_under = currentStrandNum;
//            if (currentStrand.crossing()->index() == currentStrand.next().crossing()->index()) {
//                currentEntry.crossing_type = 1; // Curl
//                currentCompExtCrossings.emplace_back(currentEntry);
//                std::cout << "extendedCrossings - currentEntry (Curl): " << currentEntry << std::endl;
//                currentStrand = currentStrand.next().next();
//            }
//            else {
//                currentEntry.crossing_type = 0; // Standard Crossing
//                currentCompExtCrossings.emplace_back(currentEntry);
//                std::cout << "extendedCrossings - currentEntry (SC): " << currentEntry << std::endl;
//                currentStrand = currentStrand.next();
//            }
//        } while (currentStrand != componentReferences[i]);
//        ans.push_back(currentCompExtCrossings);
//    }
    
    return ans;
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
    
    regina::Link tmpLinkObj = regina::Link::fromPD(pdcTmp.begin(), pdcTmp.end());
//    if (! tmpLinkObj) {
//        // TODO: Barf
//        // --- Benjamin Andrew Burton, 2021
//    }
    
    size_t numComps = tmpLinkObj.countComponents();

    std::vector<regina::StrandRef> comps;
    for (int i=0; i<numComps; i++) {
        comps.push_back(tmpLinkObj.component(i));
    }

    /*
     COMPUTE NUMBER OF CROSSINGS IN INDIVIDUAL COMPONENT
     */
    
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
        compWrithes.push_back(tmpLinkObj.writheOfComponent(i));
    }
    
//    std::cout << "Writhe of\n";
//    for (int i=0; i<compWrithes.size(); i++) {
//        std::cout << "Component " << i << ": " << compWrithes[i] << "\n";
//    }
    
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

    std::vector<int> twoHandleFramings;
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
            twoHandleFramings.emplace_back(framings[i]);
        }
    }
    
    std::vector<long> twoHandleCompWrithes;
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
            twoHandleCompWrithes.emplace_back(tmpLinkObj.writheOfComponent(i));
        }
    }
        
    /* Pre-framing computations. */
    std::vector<int> oneHandleLinkIndices;
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            oneHandleLinkIndices.emplace_back(i);
        }
    }
    
    size_t numOneHandles = oneHandleLinkIndices.size();
    
    std::vector<regina::StrandRef> oneHandleComponentRefs;
    for (auto &e : oneHandleLinkIndices) {
        oneHandleComponentRefs.emplace_back(tmpLinkObj.component(e));
    }
        
    std::vector<int> twoHandleLinkIndices;
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
            twoHandleLinkIndices.emplace_back(i);
        }
    }
    
    size_t numTwoHandles = twoHandleLinkIndices.size();
    
    std::vector<regina::StrandRef> twoHandleComponentRefs;
    for (auto &e : twoHandleLinkIndices) {
        twoHandleComponentRefs.emplace_back(tmpLinkObj.component(e));
    }
    
    std::vector<std::vector<extCrossing>> PREFRAMING_extCrossings = extendedCrossings(tmpLinkObj);
    
//    for (const auto& v : PREFRAMING_extCrossings) {
//        for (const auto& el : v) {
//            std::cout << el << ", ";
//        }
//        std::cout << std::endl;
//    }
//
//    std::cout << std::endl;
    
    std::vector<std::vector<extCrossing>> PREFRAMING_XC_twoHandles, PREFRAMING_XC_oneHandles;
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            PREFRAMING_XC_oneHandles.emplace_back(PREFRAMING_extCrossings[i]);
        }
        else {
            PREFRAMING_XC_twoHandles.emplace_back(PREFRAMING_extCrossings[i]);
        }
    }
    
    std::vector<std::vector<extCrossing>> PREFRAMING_XC_oneTwoCommons;
    for (int i=0; i<numTwoHandles; i++) {
        std::vector<extCrossing> currentComp = PREFRAMING_XC_twoHandles[i];
        std::vector<extCrossing> currentCommons;
        for (const auto& a : currentComp) {
            for (const auto& b : PREFRAMING_XC_oneHandles) {
                for (const auto& c : b) {
                    if (a.crossing == c.crossing) {
                        currentCommons.emplace_back(a);
                    }
                }
            }
        }
        
        PREFRAMING_XC_oneTwoCommons.emplace_back(currentCommons);
    }
    
//    std::cout << "Commons:\n";
//    for (const auto& x : PREFRAMING_XC_oneTwoCommons) {
//        for (const auto& y : x) {
//            std::cout << y << ", ";
//        }
//        std::cout << std::endl;
//    }
        
    /*
     Test whether R1 moves can be performed on the pre-framed link.
     If they can be, then the user may have added their own R1 curls,
     in which case don't add extra ones even if the writhe equals the framing.
     Not sure whether this is 100% safe though (could there be links in which
     R1 moves could be performed but which don't come from user drawn curls?).
     */
    // TODO: Just operate on tmpLnkObj once confirmed R1 syntax with Ben (re. check don't perform)
    regina::Link lnkObjCopy_SimpTest = regina::Link(tmpLinkObj);
    bool lnkSimpBool = false;
    for (regina::Crossing* c : lnkObjCopy_SimpTest.crossings()) {
        if (lnkObjCopy_SimpTest.r1(c, true, false)) {
            lnkSimpBool = true;
        }
    }
    
    regina::StrandRef r1Site;
    for (int i=0; i<numTwoHandles; i++)
    {
        long w = twoHandleCompWrithes[i];

        if (!PREFRAMING_XC_oneTwoCommons[i].empty())
        {
            extCrossing r1xc = PREFRAMING_XC_oneTwoCommons[i].front();
            r1Site = tmpLinkObj.crossing(r1xc.crossing)->strand(r1xc.over_under);
        }
        else {
            r1Site = tmpLinkObj.component(twoHandleLinkIndices[i]);
        }

        if (w > twoHandleFramings[i])
        {
            std::cout << "Self-framing 2-handle " << i << "..." << std::endl;
            do {
                tmpLinkObj.r1(r1Site, 0 /* left */, -1, false, true);
                --w;
            } while (w != twoHandleFramings[i]);
        }
        else if (w < twoHandleFramings[i]) {
            std::cout << "Self-framing 2-handle " << i << "..." << std::endl;
            do {
                tmpLinkObj.r1(r1Site, 0 /* left */, 1, false, true);
                ++w;
            } while (w != twoHandleFramings[i]);
        }
        else if ((dim_flag_int == 4) && (w == twoHandleFramings[i])) {
//            std::cout << "Adding additional pair of cancelling curls to 2-handle " << i << " to guarantee existence of quadricolor..." << std::endl;
            tmpLinkObj.r1(r1Site, 0, 1, false, true);
            tmpLinkObj.r1(r1Site, 0, -1, false, true);
        }
    }
    
//    std::cout << "Link should now be self-framed:\n";
//    for (int i=0; i<compWrithes.size(); i++) {
//        std::cout << "Component " << i << ": " << tmpLinkObj.writheOfComponent(i) << "\n";
//    }
//    std::cout << std::endl;
    
    regina::Link framedLnkObj = regina::Link(tmpLinkObj.pd());
    tmpLinkObj = framedLnkObj;
        
    /* Recompute link data after framing. */
    numComps = tmpLinkObj.countComponents();

    comps.clear();
    for (int i=0; i<numComps; i++) {
        comps.emplace_back(tmpLinkObj.component(i));
    }
        
    oneHandleLinkIndices.clear();
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            oneHandleLinkIndices.emplace_back(i);
        }
    }
    
    oneHandleComponentRefs.clear();
    for (auto &e : oneHandleLinkIndices) {
        oneHandleComponentRefs.emplace_back(tmpLinkObj.component(e));
    }

    twoHandleLinkIndices.clear();
    for (int i=0; i<numComps; i++) {
        if (!oneHandleBools[i]) {
            twoHandleLinkIndices.emplace_back(i);
        }
    }
    
//    std::vector<std::vector<std::tuple<int, int,int>>> twoHandleStrandCrossingIndex(numTwoHandles); // Unused???
    twoHandleComponentRefs.clear();
    for (auto &e : twoHandleLinkIndices) {
        twoHandleComponentRefs.emplace_back(tmpLinkObj.component(e));
    }

    std::vector<std::vector<extCrossing>> extCrossings = extendedCrossings(tmpLinkObj);
    std::vector<std::vector<extCrossing>> xc_oneHandles, xc_twoHandles;
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            xc_oneHandles.emplace_back(extCrossings[i]);
        }
        else {
            xc_twoHandles.emplace_back(extCrossings[i]);
        }
    }

    // DEBUGGING
    for (int i=0; i<numComps; i++) {
        if (oneHandleBools[i]) {
            std::cout << "1-handle:\n";
        }
        else {
            std::cout << "2-handle:\n";
        }
        auto currentComp = extCrossings[i];
        for (auto &e : currentComp) {
            std::cout << e << std::endl;
        }
    }
    
    std::vector<std::pair<extCrossing, extCrossing>> oneHandleMarkedCrossings;
    for (auto &i : oneHandleLinkIndices) {
        auto currentComp = extCrossings[i];
        std::vector<extCrossing> tmpUnders;
        std::pair<extCrossing, extCrossing> current;
        for (auto &e : currentComp) {
            if (e.over_under == 0) {
                tmpUnders.emplace_back(e);
            }
        }
        /*
         Assuming one handle is traversed counter-clockwise,
         p.first is the "leftmost" crossing, and
         p.second is the "rightmost" crossing.
         */
        current.first = tmpUnders.back();
        current.second = tmpUnders.front();
        oneHandleMarkedCrossings.emplace_back(current);
    }
    
    // DEBUGGING
    std::cout << "1-handle marked pairs:\n";
    for (const auto &e : oneHandleMarkedCrossings) {
        std::cout << e << std::endl;
    }
    
    std::vector<std::vector<std::pair<extCrossing, extCrossing>>> quadriCandidates(numTwoHandles);
    
    for (int i=0; i<numTwoHandles; i++) {
        std::vector<std::pair<extCrossing, extCrossing>> currentQuadriList;
        
        int currentCompInt = twoHandleLinkIndices[i];
        auto currentComp = extCrossings[currentCompInt];

        for (int j=0; j<currentComp.size(); j++) {
            extCrossing current = currentComp[j];
            extCrossing next = currentComp[(j+1)%currentComp.size()];
            if (
                // The current crossing is an under crossing,
                // and the next crossing is a curl.
                (
                 (current.over_under == 0) &&
                (current.crossing_type == 0) &&
                (next.crossing_type == 1)
                 )
                ) {
                currentQuadriList.emplace_back(next,current);
            }
            else if (
                // The current crossing is a curl,
                // and the next crossing is an undercrossing.
                (
                 (next.over_under == 0) &&
                (next.crossing_type == 0) &&
                (current.crossing_type == 1)
                 ) ||
                // The current crossing is curl,
                // and the next crossing is a curl of the same sign (0).
                (
                 (current.over_under == 0) &&
                (current.crossing_type == 1) &&
                 (next.over_under == 0) &&
                (next.crossing_type == 1)
                 ) ||
                // The current crossing is curl,
                // and the next crossing is a curl of the same sign (1).
                (
                 (current.over_under == 1) &&
                (current.crossing_type == 1) &&
                 (next.over_under == 1) &&
                (next.crossing_type == 1)
                 )
                     ) {
                currentQuadriList.emplace_back(current,next);
            }
        }
        quadriCandidates[i] = currentQuadriList;
    }
    
    // DEBUGGING
    std::cout << "quadriCandidates:" << std::endl;
    for (const auto& v : quadriCandidates) {
        for (const auto& el : v) {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    }
    
//  Begin highlighting procedure.
    std::cout << "--- Highlighting procedure ---" << std::endl;
    
    std::vector<std::vector<extCrossing>> oneTwoCommons;
    for (int i=0; i<numTwoHandles; i++)
    {
        std::vector<extCrossing> currentComp = xc_twoHandles[i];
        std::vector<extCrossing> currentCommons;
        for (const auto& a : currentComp) {
            for (const auto& b : xc_oneHandles) {
                for (const auto& c : b) {
                    if (a.crossing == c.crossing) {
                        currentCommons.emplace_back(a);
                    }
                }
            }
        }
        oneTwoCommons.emplace_back(currentCommons);
    }
    
    // DEBUGGING
    std::cout << "oneTwoCommons:" << std::endl;
    for (const auto& el : oneTwoCommons) {
        for (const auto& x : el) {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    
    extCrossing dummyCrossing = {-1,-1,SIZE_T_MAX,SIZE_T_MAX};
    std::pair<extCrossing, extCrossing> dummyPair = {dummyCrossing,dummyCrossing};
    std::vector<std::pair<extCrossing, extCrossing>> quadriList(numTwoHandles,dummyPair);

    for (int i=0; i<numTwoHandles; i++) {
        std::vector<extCrossing> currentCommons = oneTwoCommons[i];
        std::vector<std::pair<extCrossing, extCrossing>> currentQuadris = quadriCandidates[i];
        
        for (const auto& q : currentQuadris) {
            for (const auto& x : currentCommons) {
                if ((q.first.crossing == x.crossing) || (q.second.crossing == x.crossing)) {
                    quadriList[i] = q;
                }
            }
        }

        if (quadriList[i].first.component == SIZE_T_MAX) {
            quadriList[i] = currentQuadris[0];
        }
    }
    
    // DEBUGGING
    for (const auto& q : quadriList) {
        std::cout << q << std::endl;
    }
    
    std::vector<std::vector<std::pair<int, size_t>>> highlightCrossings;
    for (int i=0; i<numTwoHandles; i++) {
        bool flag = true;
        std::vector<extCrossing> currentCommons = oneTwoCommons[i];
        
        if (currentCommons.empty()) {
            flag = false;
        }
        if (!flag) {
            break;
        }
        
        std::vector<std::pair<int, size_t>> oneTwoCommons_reduced;
        for (const auto& el : currentCommons) {
            int ou = el.over_under;
            size_t ci = el.crossing;
            std::pair<int, size_t> currentpair = std::make_pair(ou, ci);
            oneTwoCommons_reduced.emplace_back(currentpair);
        }
         
        for (const auto& p : oneTwoCommons_reduced) {
            std::cout << "(" << p.first << ", " << p.second << "), ";
        }
        std::cout << std::endl;
        
        std::vector<std::pair<int, size_t>> currentSeen;

        std::pair<extCrossing, extCrossing> currentQuadri = quadriList[i];

        extCrossing currentQuadriFirst = currentQuadri.first;
        extCrossing currentQuadriSecond = currentQuadri.second;
        
        std::pair<int, size_t> cqf_reduced = std::make_pair(currentQuadriFirst.over_under, currentQuadriFirst.crossing);
        std::pair<int, size_t> cqs_reduced = std::make_pair(currentQuadriSecond.over_under, currentQuadriSecond.crossing);
        
        regina::StrandRef highlightRef, oppRef;
        
        if (
            std::find(oneTwoCommons_reduced.begin(), oneTwoCommons_reduced.end(), cqf_reduced) != oneTwoCommons_reduced.end()
            ) {
            currentSeen.emplace_back(cqf_reduced);
            highlightRef = tmpLinkObj.crossing(cqf_reduced.second)->strand(cqf_reduced.first);
            oppRef = tmpLinkObj.crossing(cqs_reduced.second)->strand(cqs_reduced.first);
        }
        else if (std::find(oneTwoCommons_reduced.begin(), oneTwoCommons_reduced.end(), cqs_reduced) != oneTwoCommons_reduced.end()) {
            currentSeen.emplace_back(cqs_reduced);
            highlightRef = tmpLinkObj.crossing(cqs_reduced.second)->strand(cqs_reduced.first);
            oppRef = tmpLinkObj.crossing(cqf_reduced.second)->strand(cqf_reduced.first);
        }

        bool WALK_OPPOSITE_DIRECTION = false;
        if (highlightRef.next() != oppRef) {
            WALK_OPPOSITE_DIRECTION = true;
        }

        bool keepWalking = true;
        do {
            if (WALK_OPPOSITE_DIRECTION) {
                if (highlightRef.crossing()->index() == highlightRef.prev().crossing()->index()) {
                    highlightRef = highlightRef.prev().prev();
                }
                else {
                    highlightRef = highlightRef.prev();
                }
            }
            else {
                if (highlightRef.crossing()->index() == highlightRef.next().crossing()->index()) {
                    highlightRef = highlightRef.next().next();
                }
                else {
                    highlightRef = highlightRef.next();
                }
            }
            bool isCurl = false;
            if (
                (highlightRef.crossing()->index() == highlightRef.next().crossing()->index()) ||
                (highlightRef.crossing()->index() == highlightRef.prev().crossing()->index())
                ) {
                isCurl = true;
            }
            std::pair<int, size_t> currentPair;
            int ou = highlightRef.strand();
            size_t ci = highlightRef.crossing()->index();
            if (!isCurl) {
                currentPair = std::make_pair(ou, ci);
            }
            else {
                currentPair = std::make_pair(2, ci);
            }
            currentSeen.emplace_back(currentPair);
        } while (!isSubset<std::pair<int, size_t>>(currentSeen, oneTwoCommons_reduced));


        currentSeen.pop_back();
        currentSeen.erase(currentSeen.begin());
        
        highlightCrossings.emplace_back(currentSeen);
    }
    
    // DEBUGGING
    std::cout << "Highlighted crossings:" << std::endl;
    for (const auto& a : highlightCrossings) {
        for (const auto& b : a) {
            std::cout << b << ", ";
        }
        std::cout << std::endl;
    }
    
    // Note: The reflection below is just a quick hack to make orientations consistent with SnapPy+Regina.
    tmpLinkObj.reflect();
    
    pdcode pdc = tmpLinkObj.pdData();
    
    graph<4> pdc_g;
    
    vertex v1={1,0},v2={2,0},v3={3,0},v4={4,0},v5={5,0},v6={6,0},v7={7,0},v8={8,0};
    vertex v9={9,4},v10={10,4},v11={11,4},v12={12,4};
    vertex v13={13,1},v14={14,1},v15={15,1},v16={16,1};
    vertex v17={17,2},v18={18,2},v19={19,2},v20={20,2};
    vertex v21={21,3},v22={22,3},v23={23,3},v24={24,3};
    
    vertex pca5={5,4},pca6={6,4},pca7={7,4},pca8={8,4},
    pca9={9,1},pca10={10,1},pca11={11,1},pca12={12,1};
    vertex pcb5={5,3},pcb6={6,3},pcb7={7,3},pcb8={8,3},
    pcb9={9,2},pcb10={10,2},pcb11={11,2},pcb12={12,2};
    
    vertex nca5={5,2},nca6={6,2},nca7={7,2},nca8={8,2},
    nca9={9,1},nca10={10,1},nca11={11,1},nca12={12,1};
    vertex ncb5={5,4},ncb6={6,4},ncb7={7,4},ncb8={8,4},
    ncb9={9,3},ncb10={10,3},ncb11={11,3},ncb12={12,3};
        
    std::vector<edge> posCross_el = {
        {v1,v2,3},{v2,v3,2},{v4,v5,2},{v5,v6,3},{v6,v7,2},{v8,v1,2},
        {v1,v9,1},{v2,v12,1},{v3,v13,1},{v3,v14,0},{v4,v15,0},{v4,v16,1},{v5,v17,1},{v6,v20,1},{v7,v21,1},{v7,v22,0},{v8,v23,0},{v8,v24,1},
        {v1,v6,0},{v2,v5,0},{v3,v8,3},{v4,v7,3},
        {v10,v19,0},{v11,v18,0}
    };
    
    std::vector<edge> negCross_el = {
        {v1,v2,3},{v2,v3,2},{v4,v5,2},{v5,v6,3},{v6,v7,2},{v8,v1,2},
        {v1,v17,1},{v2,v20,1},{v3,v21,1},{v3,v22,0},{v4,v23,0},{v4,v24,1},{v5,v9,1},{v6,v12,1},{v7,v13,1},{v7,v14,0},{v8,v15,0},{v8,v16,1},
        {v1,v6,0},{v2,v5,0},{v3,v8,3},{v4,v7,3},
        {v18,v11,0},{v19,v10,0}
    };
    
    std::vector<edge> posCurlA_el = {
        {v1,v2,2},{v2,v3,3},{v3,v4,2},{v4,v1,3},
        {v1,pca12,1},{v3,pca11,0},{v4,pca10,0},{v4,pca9,1},
        {v2,pca5,1},{v2,pca6,0},{v1,pca7,0},{v3,pca8,1}
    };
    
    std::vector<edge> posCurlB_el = {
        {v1,v2,2},{v2,v3,3},{v3,v4,2},{v4,v1,3},
        {v1,pcb12,1},{v3,pcb11,0},{v4,pcb10,0},{v4,pcb9,1},
        {v2,pcb5,1},{v2,pcb6,0},{v1,pcb7,0},{v3,pcb8,1}
    };
    
    std::vector<edge> negCurlA_el = {
        {v1,v2,2},{v2,v3,3},{v3,v4,2},{v4,v1,3},
        {v1,nca12,1},{v1,nca11,0},{v2,nca10,0},{v4,nca9,1},
        {v2,nca5,1},{v4,nca6,0},{v3,nca7,0},{v3,nca8,1}
    };
    
    std::vector<edge> negCurlB_el = {
        {v1,v2,2},{v2,v3,3},{v3,v4,2},{v4,v1,3},
        {v1,ncb12,1},{v1,ncb11,0},{v2,ncb10,0},{v4,ncb9,1},
        {v2,ncb5,1},{v4,ncb6,0},{v3,ncb7,0},{v3,ncb8,1}
    };
    
    posCross.add_edges(posCross_el);
    negCross.add_edges(negCross_el);
    posCurlA.add_edges(posCurlA_el);
    posCurlB.add_edges(posCurlB_el);
    negCurlA.add_edges(negCurlA_el);
    negCurlB.add_edges(negCurlB_el);
    
    std::vector<std::pair<int, int>> pdc_xot = pdc_xotype(pdc);
    
    long total_crossing_counter = 1;
    for (auto p : pdc_xot) {
//        std::cout << std::left << std::setw(6) << total_crossing_counter;
        ++total_crossing_counter;
        if ((p.first == 0) && (p.second == 1)) {
//            std::cout << "Generating Positive Crossing...\n";
            pdc_g.disjoint_union(posCross);
        }
        else if ((p.first == 0) && (p.second == -1)) {
//            std::cout << "Generating Negative Crossing...\n";
            pdc_g.disjoint_union(negCross);
        }
        else if (p.first == 1) {
//            std::cout << "Generating Positive Curl of Type A (x,y,y,w)...\n";
            pdc_g.disjoint_union(posCurlA);
        }
        else if (p.first == 2) {
//            std::cout << "Generating Positive Curl of Type B (x,y,z,x)...\n";
            pdc_g.disjoint_union(posCurlB);
        }
        else if (p.first == 3) {
//            std::cout << "Generating Negative Curl of Type A (x,y,z,z)...\n";
            pdc_g.disjoint_union(negCurlA);
        }
        else if (p.first == 4) {
//            std::cout << "Generating Negative Curl of Type A (x,x,z,w)...\n";
            pdc_g.disjoint_union(negCurlB);
        }
    }
    
    pdc_g.pd_sub(pdc);
            
    std::vector<std::pair<vertex, vertex>> pdc_fl = pdc_g.fuse_list();
    
    for (auto p : pdc_fl) {
        pdc_g.fuse(p.first, p.second);
    }
    
    std::vector<std::array<vertex, 4>> ql = pdc_g.quadriGraphFind();
        
    std::vector<std::array<vertex, 4>> finalGraphQuadriList;
    
    std::vector<std::set<size_t>> lnkQuadriCrossingsOnly;
    for (const auto& p : quadriList) {
        std::set<size_t> current;
        current.insert(p.first.crossing);
        current.insert(p.second.crossing);
        lnkQuadriCrossingsOnly.emplace_back(current);
    }
    
    std::vector<std::set<size_t>> gqcseen;
    for (int i=0; i<ql.size(); i++) {
        std::array<vertex, 4> currentQuadri = ql[i];
        std::set<size_t> currentCrossings;
        for (const auto& v : currentQuadri) {
            currentCrossings.insert(v.component);
        }
        if (std::find(lnkQuadriCrossingsOnly.begin(), lnkQuadriCrossingsOnly.end(), currentCrossings) != lnkQuadriCrossingsOnly.end()) {
            if (std::find(gqcseen.begin(), gqcseen.end(), currentCrossings) == gqcseen.end()) {
                gqcseen.emplace_back(currentCrossings);
                finalGraphQuadriList.emplace_back(currentQuadri);
            }
        }
    }
    
    // DEBUGGING
    std::cout << "Quadri list:" << std::endl;
    for (const auto& q : finalGraphQuadriList) {
        std::cout << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << std::endl;
    }
    std::cout << std::endl;
    
    std::vector<vertex> verts = pdc_g.vertices();
        
    std::vector<std::pair<vertex, vertex>> oneHandleMarkedVerts;
    for (const auto& p : oneHandleMarkedCrossings) {
        int leftComp = p.first.crossing;
        int rightComp = p.second.crossing;
        std::pair<vertex, vertex> currentPair;
        
        int leftOrientation = pdc_xot[leftComp].second;
        int rightOrientation = pdc_xot[rightComp].second;
        
        if (leftOrientation == 1) {
            currentPair.first = {7,0,leftComp};
        }
        if (leftOrientation == -1) {
            currentPair.first = {3,0,leftComp};
        }
        if (rightOrientation == 1) {
            currentPair.second = {4,0,rightComp};
        }
        if (rightOrientation == -1) {
            currentPair.second = {8,0,rightComp};
        }
        
        oneHandleMarkedVerts.emplace_back(currentPair);
    }
    
//  DEBUGGING
    std::cout << "One-Handle Marked Vertices:" << std::endl;
    for (const auto& p : oneHandleMarkedVerts) {
        std::cout << p << std::endl;
    }

    pdc_g.addQuadriEdges(finalGraphQuadriList);
    if (std::find(oneHandleBools.begin(), oneHandleBools.end(), true) != oneHandleBools.end()) {
        pdc_g.addOneHandleIdentEdges(oneHandleMarkedVerts);
        pdc_g.addHighlightEdges(highlightCrossings);
        pdc_g.addDoubleOneEdges();
        pdc_g.addRemainderEdges();
    }
    else {
        pdc_g.addDoubleOneEdges();
    }
    pdc_g.cleanup();
//    pdc_g.DEBUGno4ColVerts();
//    pdc_g.print_graph();
    
    std::vector<std::tuple<int,int,int>> gl = gluingList(pdc_g);

    regina::Triangulation<4> tmp_t;
    regina::Perm<5> perm;
    std::vector<regina::Simplex<4>> tmp_s;
    tmp_t.newPentachora(pdc_g.size());
    for (const auto& g : gl) {
        tmp_t.pentachoron(std::get<0>(g))->join(std::get<2>(g),tmp_t.pentachoron(std::get<1>(g)),perm);
    }

    std::cout << "\r " << std::endl;
    std::cout << "\rHere is the isomorphism signature:\n" << std::flush;
    std::cout << tmp_t.isoSig() << "\n";

    return 0;
}
