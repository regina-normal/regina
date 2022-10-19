//
//  main.cpp
//  DGT2: Diagrams to Graphs and Triangulations
//
//  Created by Rhuaidi Antonio Burke on 12/3/21.
//

#include <iostream>
#include <iomanip>
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

#include <triangulation/dim3.h>
#include <triangulation/dim4.h>
#include <link/link.h>

using vertex3 = std::tuple<int, int, int>; // (c_id, v_id, s_id)
using vertex = std::tuple<int, int>; // (v_id, s_id)
using edge3 = std::tuple<vertex3, vertex3, int>; // (v1, v2, col)
using edge = std::tuple<vertex, vertex, int>;

using pdcode = std::vector<std::array<int, 4>>;

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

std::map<int, int> mod4swap = {{1,0},{2,3},{3,2},{0,1}};

template <int dim>
class graph {
    std::map<vertex3, std::array<vertex3, dim+1>> adjList;
    
public:
    
    static int uniqueID() {
        // TODO: Make thread safe but mutex seems to break things...
        static int nextID = 0;
        return nextID++;
    }

    void add_edge(edge3 e) {
        /*
         Edge e = (v1, v2, col)
         adjList[v1][col] = v2
         adjList[v2][col] = v1
         */
        adjList[std::get<0>(e)][std::get<2>(e)] = std::get<1>(e);
        adjList[std::get<1>(e)][std::get<2>(e)] = std::get<0>(e);
        
        // TODO: Sanitise inputs?
    }
    
    void add_edges(const std::vector<edge3>& edgeList){
        for(const auto& e : edgeList){
            add_edge(e);
        }
    }
    
    void from_simple_edges(std::vector<edge> el){
        /*
         Converts a list of "simple" edges of the form {{v1id,v2id,col},...} to
         their "extended" form {{{0,v1id,s1id},{0,v2id,s2id},col},...}
         */

        for (auto & elem : el){
            vertex3 v1 = {0, std::get<0>(std::get<0>(elem)), std::get<1>(std::get<0>(elem))};
            vertex3 v2 = {0, std::get<0>(std::get<1>(elem)), std::get<1>(std::get<1>(elem))};
            int col = std::get<2>(elem);
            
            adjList[v1][col] = v2;
            adjList[v2][col] = v1;
            
        }
    }
    
    
    std::vector<vertex3> vertices(){
        std::vector<vertex3> vertList;
        for (const auto& [key,val] : adjList) {
            vertList.push_back(key);
        }
        return vertList;
    }

    void print_vertices() {
        // TODO: Make const.
        
        for (const auto& [key,val] : adjList) {
            std::cout << key << "\n";
        }
        std::cout << "\n";
    }
    
    std::vector<edge3> edges(){
        std::vector<edge3> edgeList;
        
        for (const auto& [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((vert < nbrs[i]) && (std::get<1>(vert) != 0)) {
                    edgeList.push_back({vert,nbrs[i],i});
                }
            }
        }
        
        return edgeList;
    }
    
    void print_edges() {
        for (const auto& [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if ((vert < nbrs[i]) && (std::get<1>(vert) != 0)) {
                    std::cout << "[" << vert << ", " << nbrs[i] << ", " << i << "],\n";
                }
            }
        }
    }
        
    void disjoint_union(graph<dim> h) {
        
        std::vector<edge3> hEdges = h.edges();
        std::vector<edge> hSimpleEdges;
        
        int currentID = uniqueID();
        
        for (auto elem : hEdges) {
            
            vertex3 v1 = {currentID,std::get<1>(std::get<0>(elem)),std::get<2>(std::get<0>(elem))};
            vertex3 v2 = {currentID,std::get<1>(std::get<1>(elem)),std::get<2>(std::get<1>(elem))};
            int col = std::get<2>(elem);
            
            adjList[v1][col] = v2;
            adjList[v2][col] = v1;
            
        }
    }
    
    int degree(vertex3 vert) {
        int vert_degree = 0;
        for (auto elem : adjList[vert]) {
            if (std::get<1>(elem) != 0) {
                vert_degree++;
            }
        }
        return vert_degree;
    }
    
    void degree_sequence() {
        for (const auto& [key,val] : adjList) {
            std::cout << degree(key) << "\n";
        }
    }
    
    std::vector<vertex3> neighbours(vertex3 vert) {
        std::vector<vertex3> vertNeighbours;
        for (auto elem : adjList[vert]) {
            if (std::get<1>(elem) != 0) {
                vertNeighbours.push_back(elem);
            }
        }
        return vertNeighbours;
    }
    
    std::vector<edge3> edges_incident(vertex3 vert) {
        std::vector<edge3> vertEdgesIncident;
        for (int i=0; i<dim+1; i++) {
            if (std::get<1>(adjList[vert][i]) != 0) {
                vertEdgesIncident.push_back({vert, adjList[vert][i], i});
            }
        }
        return vertEdgesIncident;
    }
    
    void delete_edge(edge3 e) {
        vertex3 v1 = std::get<0>(e);
        vertex3 v2 = std::get<1>(e);
        int col = std::get<2>(e);
        adjList[v1][col] = {std::get<0>(v1),0,0};
        adjList[v2][col] = {std::get<0>(v2),0,0};
        
    }
    
    void delete_vertex(vertex3 v) {
        adjList.erase(v);
    }
    
    void pd_sub(const pdcode& code) {
        vertex3 newNbri;
        for (auto [vert, nbrs] : adjList) {
            for (int i=0; i<dim+1; i++) {
                if (std::get<1>(nbrs[i]) != 0) {
                    switch (std::get<2>(nbrs[i])) {
                        case 0:
                            break;
                        case 1:
                            newNbri = {std::get<0>(nbrs[i]),std::get<1>(nbrs[i]),code[std::get<0>(vert)][0]};
                            adjList.erase(nbrs[i]);

                            adjList[vert][i] = newNbri;
                            adjList[newNbri][i] = vert;
                            break;
                        case 2:
                            newNbri = {std::get<0>(nbrs[i]),std::get<1>(nbrs[i]),code[std::get<0>(vert)][1]};
                            adjList.erase(nbrs[i]);

                            adjList[vert][i] = newNbri;
                            adjList[newNbri][i] = vert;
                            break;
                        case 3:
                            newNbri = {std::get<0>(nbrs[i]),std::get<1>(nbrs[i]),code[std::get<0>(vert)][2]};
                            adjList.erase(nbrs[i]);

                            adjList[vert][i] = newNbri;
                            adjList[newNbri][i] = vert;
                            break;
                        case 4:
                            newNbri = {std::get<0>(nbrs[i]),std::get<1>(nbrs[i]),code[std::get<0>(vert)][3]};
                            adjList.erase(nbrs[i]);

                            adjList[vert][i] = newNbri;
                            adjList[newNbri][i] = vert;
                            break;
                    }

                }
                
            }
        }
    }
    
    std::map<vertex3, std::array<vertex3, dim+1>> data() {
        return adjList;
    }
    
    std::vector<std::pair<vertex3, vertex3>> fuse_list() {
        
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
        
        std::vector<std::pair<vertex3, vertex3>> ans;
        
        for (auto const& [v1,nbrs1] : adjList) {
            for (auto const& [v2,nbrs2] : adjList) {
                if (
                    (std::get<0>(v1) < std::get<0>(v2)) &&
                    (std::get<2>(v1) != 0) && (std::get<2>(v2) != 0) &&
                    (std::get<0>(v1) != std::get<0>(v2)) &&
                    (std::get<2>(v1) == std::get<2>(v2)) &&
                    (std::get<1>(v1)%4 == mod4swap[std::get<1>(v2)%4])) {
                    ans.push_back(std::make_pair(v1, v2));
                }
            }
        }
        
        return ans;
        
    }
    
    void fuse(vertex3 v1, vertex3 v2) {
        
        // TODO: Should sanitise things - e.g. make sure v1 has as many neighbours as v2 and that their colours match up, etc.
        
        std::array<vertex3, dim+1> v1nbrs = adjList[v1]; // {v1n1,v1n2,...,v1n(dim+1)}
        std::array<vertex3, dim+1> v2nbrs = adjList[v2]; // {v2n1,v2n2,...,v2n(dim+1)}

        adjList.erase(v1);
        adjList.erase(v2);

        for (int i=0; i<dim+1; i++) {
            adjList[v1nbrs[i]][i] = v2nbrs[i];
            adjList[v2nbrs[i]][i] = v1nbrs[i];
        }
        adjList.erase({0,0,0});
    }
    
    std::vector<std::array<vertex3, 4>> quadri_find() {
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
        
        std::array<vertex3, 4> quadri;
        std::array<vertex3, 2> tmp;
        std::set<std::array<vertex3, 4>> uniqueQuadris;
    
        for (const auto& [vert, nbrs] : adjList) {
            if (std::get<1>(vert) != 0) {
                tmp[0] = adjList[vert][0];
                tmp[1] = adjList[vert][3];
                if (adjList[tmp[0]][1] == adjList[tmp[1]][2]) {
                    quadri[0] = vert;
                    quadri[1] = tmp[0];
                    quadri[2] = adjList[tmp[0]][1];
                    quadri[3] = tmp[1];
                }
                if (std::get<1>(quadri[0]) != 0) {
                    uniqueQuadris.insert(quadri);
                }
            }
        }
        if (uniqueQuadris.size() < 1) {
            std::cout << "Could not locate any quadricolours!\n";
        }
        
        std::vector<std::array<vertex3, 4>> ans(uniqueQuadris.begin(),uniqueQuadris.end());
        
        return ans;
    }
    
    std::vector<std::array<int, 4>> int_quadris() {
            
        std::vector<std::array<vertex3, 4>> ql = quadri_find();
        std::vector<std::array<int, 4>> ans(ql.size());
        for (int i=0; i<ql.size(); i++) {
            for (int j=0; j<4; j++) {
                ans[i][j] = index_of_vertex(ql[i][j]);
            }
        }
        
        return ans;
    }
    
    int index_of_vertex(vertex3 v) {
        // Probably not the right way to go about this...
        // Ask Ben about structure/placement of this (e.g. not iterating through map everytime function is called)...
        std::map<vertex3, int> vert2int;
        int v2i_val = 0;
        for (const auto& [vert, nbrs] : adjList) {
            vert2int[vert] = v2i_val++;
        }
        
        return vert2int[v];
    }
    
    vertex3 vert_from_index(int i) {
        // Again, bad strucutre/way of doing this but just for now...
        std::vector<vertex3> keys;
        for (const auto& [vert,nbrs] : adjList) {
            keys.push_back(vert);
        }

        return keys[i];
    }
    
    std::vector<std::vector<int>> connected_components() {
        std::vector<std::vector<int>> ans;
        std::vector<int> tmp;

        size_t V = adjList.size();

        bool* visited = new bool[V];
        for (int i=0; i<V; i++) {
            visited[i] = false;
        }
        
        int count = 0;
        
        for (int i=0; i<V; i++) {
            if (visited[i] == false) {
                tmp = dfs(i, visited);
                ans.push_back(tmp);
                count += 1;
            }
        }
        
        delete[] visited;
        
       return ans;
    }
    
    std::vector<int> dfs(int v, bool visited[]) {
        std::set<int> tmpAns;
        std::stack<int> stack;
        stack.push(v);
        while(!stack.empty())
        {
            int top = stack.top();
            visited[top] = true;
            stack.pop();
            
            std::array<int, 4> tmpIntArray;
            for (int i=0; i<4; i++) {
                tmpIntArray[i] = index_of_vertex(adjList[vert_from_index(top)][i]);
            }

            std::array<int, 4>::iterator it;
            for (it=std::begin(tmpIntArray); it != std::end(tmpIntArray); ++it)
            {
                if(!visited[*it])
                    stack.push(*it);
            }
                tmpAns.insert(top);
        }
        std::vector<int> ans(tmpAns.begin(),tmpAns.end());
        
        return ans;
    }
    
    void singularFiveColSub(std::vector<std::array<vertex3, 4>> quadriList) {
        // quadriList should be the result of component_quadris
        
        // Again, this probably shouldn't be a graph member function but anyway, for now...
        std::vector<vertex3> quadriVertsUsed;
        
        long quadriSize = quadriList.size();
        
        // Quadri Subs:
        for (int i=0; i<quadriSize; i++) {

            std::array<vertex3, 4> currentQuadri = quadriList[i];
            
            adjList[currentQuadri[0]][4] = currentQuadri[1];
            adjList[currentQuadri[1]][4] = currentQuadri[0];
            
            adjList[currentQuadri[2]][4] = currentQuadri[3];
            adjList[currentQuadri[3]][4] = currentQuadri[2];
            
            vertex3 P4 = adjList[currentQuadri[3]][1];
            vertex3 P5 = adjList[currentQuadri[0]][1];
            
            adjList[P4][4] = P5;
            adjList[P5][4] = P4;
            
            for (int i=0; i<4; i++) {
                quadriVertsUsed.push_back(currentQuadri[i]);
            }
        }
                
        for (const auto& [vert,nbrs] : adjList) {
            if (
                (std::get<1>(vert) != 0) &&
                (vert < nbrs[1]) &&
                (std::find(quadriVertsUsed.begin(), quadriVertsUsed.end(), vert) == quadriVertsUsed.end()) &&
                (std::find(quadriVertsUsed.begin(), quadriVertsUsed.end(), nbrs[1]) == quadriVertsUsed.end())
                ) {
                adjList[vert][4] = nbrs[1];
                adjList[nbrs[1]][4] = vert;
            }
        }
        adjList.erase({0,0,0});
    }
    
    void realFiveColSub(std::vector<std::array<vertex3, 4>> quadriList) {
            // quadriList should be the result of component_quadris
            
            // Again, this probably shouldn't be a graph member function but anyway, for now...
            
            long quadriSize = quadriList.size();
            
            // Quadri Subs:
            for (int i=0; i<quadriSize; i++) {

                std::array<vertex3, 4> currentQuadri = quadriList[i];
                
                vertex3 R1 = {-(i+1),-1,0};
                vertex3 R2 = {-(i+1),-2,0};
                vertex3 R3 = {-(i+1),-3,0};
                vertex3 R1d = {-(i+1),-4,0};
                vertex3 R2d = {-(i+1),-5,0};
                vertex3 R3d = {-(i+1),-6,0};
                
                adjList[currentQuadri[0]][0] = R1d;
                adjList[currentQuadri[0]][3] = R3d;
                adjList[currentQuadri[1]][0] = R1;
                adjList[currentQuadri[1]][1] = R1;
                adjList[currentQuadri[2]][1] = R2;
                adjList[currentQuadri[2]][2] = R2;
                adjList[currentQuadri[3]][2] = R3;
                adjList[currentQuadri[3]][3] = R3;

                adjList[R1d][0] = currentQuadri[0];
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
                adjList[R3d][3] = currentQuadri[0];
                adjList[R3d][4] = R3;
                
                adjList[R1][0] = currentQuadri[1];
                adjList[R1][1] = currentQuadri[1];
                adjList[R1][2] = R1d;
                adjList[R1][3] = R2;
                adjList[R1][4] = R1d;

                adjList[R2][0] = R3;
                adjList[R2][1] = currentQuadri[2];
                adjList[R2][2] = currentQuadri[2];
                adjList[R2][3] = R1;
                adjList[R2][4] = R2d;

                adjList[R3][0] = R2;
                adjList[R3][1] = R3d;
                adjList[R3][2] = currentQuadri[3];
                adjList[R3][3] = currentQuadri[3];
                adjList[R3][4] = R3d;
                
            }
            adjList.erase({0,0,0});
        }
    
    size_t size() {
        size_t ans = adjList.size();
        return ans;
    }
        
};

std::vector<int> pdc_orientations(pdcode code) {

    // TODO: Swap signs to be consistent with SnapPy (Currently just reflecting)
    
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
            ans.push_back(1);
        }
        else if (x[0]==x[3]) {
            // (x,y,z,x)
            ans.push_back(2);
        }
        else if (x[2]==x[3]) {
            // (x,y,z,z)
            ans.push_back(3);
        }
        else if (x[0]==x[1]) {
            // (x,x,y,z)
            ans.push_back(4);
        }
        else {
            // regular crossing
            ans.push_back(0);
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
    
    for (int i=0; i<code.size(); i++) {
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

long getIndex(std::vector<vertex3> v, vertex3 K) {
    long ans = -1;

    auto it = std::find(v.begin(), v.end(), K);
 
    if (it != v.end()) {
        long index = it - v.begin();
        ans = index;
    }
    else {
        ans = -1;
    }
    return ans;
}

void printGluList(graph<4> G) {
    std::vector<vertex3> verts = G.vertices();
    std::vector<edge3> edges = G.edges();
    std::cout << "[";
    for (const auto& elem : edges) {
        // trick to print commas after everything except the last line :D
        if (&elem != &edges.back()) {
            std::cout << "[" << getIndex(verts, std::get<0>(elem)) << ", " << getIndex(verts, std::get<1>(elem)) <<", " << std::get<2>(elem) << "],\n";
        }
        else {
            std::cout << "[" << getIndex(verts, std::get<0>(elem)) << ", " << getIndex(verts, std::get<1>(elem)) <<", " << std::get<2>(elem) << "]]\n";
        }
    }
}

std::vector<std::tuple<int,int,int>> genGluList(graph<4> G) {
    std::vector<vertex3> verts = G.vertices();
    std::vector<edge3> edges = G.edges();
    std::cout << "[";
    std::vector<std::tuple<int,int,int>> ans;
    ans.reserve(edges.size());
    for (const auto& elem : edges) {
        ans.emplace_back(getIndex(verts, std::get<0>(elem)), getIndex(verts, std::get<1>(elem)), std::get<2>(elem));
    }
    return ans;
}

int writhe(const pdcode& pdc) {
    int ans=0;
    for (const auto& n : pdc_orientations(pdc)) {
        ans += n;
    }
    return ans;
}

std::vector<std::array<vertex3, 4>> find_component_quadris(graph<4> G) {
    
    graph<4> G_no1;
    for (const auto& e : G.edges()) {
        if (std::get<2>(e) != 1) {
            G_no1.add_edge(e);
        }
    }
    
    std::vector<std::vector<int>> no1_cc = G_no1.connected_components();
    std::vector<std::array<vertex3, 4>> quadriList = G.quadri_find();
    
    long cc_size = no1_cc.size();
    long quadri_size = quadriList.size();
    
    std::vector<std::array<vertex3, 4>> ans(cc_size);
        
    std::vector<std::array<int, 4>> test = G.int_quadris();
    
    for (int i=0; i<cc_size; i++) {
        std::vector<int> tmp1 = no1_cc[i];
        std::sort(tmp1.begin(),tmp1.end());
        for (int j=0; j<quadri_size; j++) {
            std::array<int, 4> tmpIntQuadri = test[j];
            std::sort(tmpIntQuadri.begin(),tmpIntQuadri.end());
            if ((std::includes(tmp1.begin(),tmp1.end(),tmpIntQuadri.begin(),tmpIntQuadri.end()))) {
                ans[i] = quadriList[j];
            }
        }
    }
    
    return ans;
}

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:" << std::endl;
    std::cerr << "    " << progName << " { -3, --dim3 | -4, --dim4 } "
        "[ -g, --graph ] [ -r, --real ]\n"
        "    " << progName << " [ -v, --version | -?, --help ]\n\n";
    std::cerr << "    -3, --dim3 : Build a 3-manifold via integer "
        "Dehn surgery\n";
    std::cerr << "    -4, --dim4 : Build a 4-manifold by attaching "
        "2-handles along a framed link\n\n";
    std::cerr << "    -g, --graph : Output an edge-coloured graph, "
        "not an isomorphism signature\n";
    std::cerr << "    -r, --real  : Build a triangulation with real boundary "
        "(not ideal or closed)\n\n";
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help    : Display this help\n";
    exit(1);
}

int main(int argc, char* argv[]) {

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
    
    graph<4> posCross, negCross, posCurlA, posCurlB, negCurlA, negCurlB;
    
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
    
    posCross.from_simple_edges(posCross_el);
    negCross.from_simple_edges(negCross_el);
    posCurlA.from_simple_edges(posCurlA_el);
    posCurlB.from_simple_edges(posCurlB_el);
    negCurlA.from_simple_edges(negCurlA_el);
    negCurlB.from_simple_edges(negCurlB_el);

    // Check for standard arguments:
    for (int i = 1; i < argc; ++i) {
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

    int dim_flag_int = 4; // Default to build a 4-manifold.
    bool output_type = false;
    bool bdy_type = false;
    /*
     output_type:
     ------------
     false  :   isomorphism signature (default)
     true   :   graph edge list
     
     bdy_type:
     ---------
     false  :   non-spherical boundary -> ideal,
                spherical boundary -> capped off (generates closed manifold)
                (default)
     true   :   non-spherical boundary -> real,
                spherical boundary -> real (retains spherical boundary)
     */
    
    if (argc < 2) {
        usage(argv[0], "Please provide a dimension flag (-3 or -4).");
    }
    else if (2 <= argc && argc < 5) {
        for (int i=1; i<argc; ++i) {
            if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--graph")) {
                output_type = true;
            }
            else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--real")) {
                bdy_type = true;
            }
            else if (!strcmp(argv[i], "-3") || !strcmp(argv[i], "--dim3")) {
                dim_flag_int = 3;
            }
            else if (!strcmp(argv[i], "-4") || !strcmp(argv[i], "--dim4")) {
                dim_flag_int = 4;
            }
            else {
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
            }
        }
    }
    
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
    
    regina::Link tmpLinkObj;
    try {
        tmpLinkObj = regina::Link::fromPD(pdcTmp.begin(), pdcTmp.end());
    } catch (const regina::InvalidArgument&) {
        // TODO: Barf
        // --- Benjamin Andrew Burton, 2021
    }
    
    size_t numComps = tmpLinkObj.countComponents();
    
    /*
     COMPUTE NUMBER OF CROSSINGS IN INDIVIDUAL COMPONENT
     */
    std::vector<regina::StrandRef> comps;
    comps.reserve(numComps);
    for (int i=0; i<numComps; i++) {
        comps.push_back(tmpLinkObj.component(i));
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
        tmpIntersectionMatrix.emplace_back();
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
        long compTotal = std::accumulate(tmpIntersectionMatrix[i].begin(), tmpIntersectionMatrix[i].end(), long(0));
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
    compWrithes.reserve(numComps);
    for (int i=0; i<numComps; i++) {
        compWrithes.push_back(tmpLinkObj.writheOfComponent(i));
    }
    std::cout << "Writhe of\n";
    for (int i=0; i<compWrithes.size(); i++) {
        std::cout << "Component " << i << ": " << compWrithes[i] << "\n";
    }
    
    /*
     USER INPUT FRAMINGS
     */
    std::vector<int> inputFramingVect(numComps);
    std::cout << "Enter Integer Framings of Each Component (same order as in PLink/SnapPy):" << "\n";
    for (int i=0; i<numComps; i++) {
        std::cin >> inputFramingVect[i];
    }
    /*
     END USER INPUT FRAMINGS
     */
    
    for (int i=0; i<numComps; i++) {
        long w = compWrithes[i];
        if (w > inputFramingVect[i]) {
            std::cout << "Self-framing component " << i << "...\n";
            do {
                tmpLinkObj.r1(tmpLinkObj.component(i), 0 /* left */, -1, false, true);
                --w;
            } while (w != inputFramingVect[i]);
        } else if (w < inputFramingVect[i]) {
            std::cout << "Self-framing component " << i << "...\n";
            do {
                tmpLinkObj.r1(tmpLinkObj.component(i), 0 /* left */, 1, false, true);
                ++w;
            } while (w != inputFramingVect[i]);
        }
        // If building a 4-manifold:
        // Check number of crossings in i-th component: if < |framing|+2, suggest adding pair of cancelling twists (in order to guarantee existence of a quadricolour).
        if ((dim_flag_int == 4) && (compCrossingNums[i] < abs(inputFramingVect[i])+2)) {
            std::cout << "Adding additional pair of cancelling curls to component " << i << " to guarantee existence of a quadricolour...\n";
            tmpLinkObj.r1(tmpLinkObj.component(i), 0 /* left */, 1, false, true);
            tmpLinkObj.r1(tmpLinkObj.component(i), 0 /* left */, -1, false, true);
        }
        
    }
    
    std::cout << "Link should now be self-framed: writhe(component) = framing(component)...\nWrithe of \n";
    for (int i=0; i<compWrithes.size(); i++) {
        std::cout << "Component " << i << ": " << tmpLinkObj.writheOfComponent(i) << "\n";
    }
    
    std::cout << std::endl;
    
    // Note: The reflection below is just a quick hack to fix orientations (that is, make consistent with SnapPy+Regina).
    tmpLinkObj.reflect();
    pdcode pdc = tmpLinkObj.pdData();
        
    graph<4> pdc_g;

    std::vector<std::pair<int, int>> pdc_xot = pdc_xotype(pdc);

    long total_crossing_counter = 1;
    for (auto p : pdc_xot) {
        std::cout << std::left << std::setw(6) << total_crossing_counter;
        ++total_crossing_counter;
        if ((p.first == 0) && (p.second == 1)) {
            std::cout << "Generating Positive Crossing...\n";
            pdc_g.disjoint_union(posCross);
        }
        else if ((p.first == 0) && (p.second == -1)) {
            std::cout << "Generating Negative Crossing...\n";
            pdc_g.disjoint_union(negCross);
        }
        else if (p.first == 1) {
            std::cout << "Generating Positive Curl of Type A (x,y,y,w)...\n";
            pdc_g.disjoint_union(posCurlA);
        }
        else if (p.first == 2) {
            std::cout << "Generating Positive Curl of Type B (x,y,z,x)...\n";
            pdc_g.disjoint_union(posCurlB);
        }
        else if (p.first == 3) {
            std::cout << "Generating Negative Curl of Type A (x,y,z,z)...\n";
            pdc_g.disjoint_union(negCurlA);
        }
        else if (p.first == 4) {
            std::cout << "Generating Negative Curl of Type A (x,x,z,w)...\n";
            pdc_g.disjoint_union(negCurlB);
        }
    }

    pdc_g.pd_sub(pdc);

    std::vector<std::pair<vertex3, vertex3>> pdc_fl = pdc_g.fuse_list();

    for (auto p : pdc_fl) {
        pdc_g.fuse(p.first, p.second);
    }

//    size_t size = pdc_g.size();
    
    if (dim_flag_int == 4) {
        std::vector<std::array<vertex3, 4>> component_ql = find_component_quadris(pdc_g);
        std::cout << std::endl;
        std::cout << "Performing " << component_ql.size() << " quadricolour substitution";
        if (component_ql.size() == 1) {
            std::cout << "...\n";
        }
        else {
            std::cout << "s...\n";
        }
        std::cout << std::endl;
        
        if (bdy_type == false) {
            std::cout << "If manifold has (non-spherical) boundary, resulting triangulation will have ideal boundary.\nIf manifold has spherical boundary, manifold will be capped off to produce a closed manifold.\n";
            pdc_g.singularFiveColSub(component_ql);
        }
        else if (bdy_type == true) {
            std::cout << "If manifold has boundary, resulting triangulation will have real boundary.\n";
            pdc_g.realFiveColSub(component_ql);
        }
        
        std::cout << std::endl;
        
        size_t size = pdc_g.size();
        if (output_type == false) {
            
            std::vector<std::tuple<int,int,int>> gl = genGluList(pdc_g);

            regina::Triangulation<4> tmp_t;
            regina::Perm<5> perm;
            std::vector<regina::Simplex<4>> tmp_p;
            
            for (int i=0; i<size; i++) {
                tmp_t.newPentachoron();
            }
            for (const auto& g : gl) {
                tmp_t.pentachoron(std::get<0>(g))->join(std::get<2>(g), tmp_t.pentachoron(std::get<1>(g)), perm);
            }
            std::cout << "\rHere is the isomorphism signature:\n" << std::flush;
            std::cout << tmp_t.isoSig() << "\n";
            
        }
        else if (output_type == true) {
            std::cout << "\rHere is the coloured graph edge list/facet identification list:\n" << std::flush;
            printGluList(pdc_g);
        }
    }
    else if (dim_flag_int == 3) {
        if (output_type == false) {
            std::vector<std::tuple<int,int,int>> gl = genGluList(pdc_g);
            regina::Triangulation<3> tmp_t;
            regina::Perm<4> perm;
            std::vector<regina::Simplex<3>> tmp_p;
            tmp_t.newTetrahedra(pdc_g.size());
            for (const auto& g : gl) {
                tmp_t.tetrahedron(std::get<0>(g))->join(std::get<2>(g), tmp_t.tetrahedron(std::get<1>(g)), perm);
            }
            std::cout << "\r " << std::endl;
            std::cout << "\rHere is the isomorphism signature:\n" << std::flush;
            std::cout << tmp_t.isoSig() << "\n";
        }
        else if (output_type == true) {
            //std::cout << std::endl;
            std::cout << "\rHere is the coloured graph edge list/facet identification list:\n" << std::flush;
            printGluList(pdc_g);
        }
    }
    
    return 0;

}
