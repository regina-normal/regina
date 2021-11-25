//
//  up-down-simplify.cpp
//  Engine
//
//  Created by Antonio Burke on 24/8/21.
//  Copyright © 2021 Regina Development Team. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <triangulation/dim4.h>

int main(){
    
    int twoFourCap, edgeLowerbound, threeThreeCap;

    std::string initIsoSig;
    std::cout << "Iso Sig: ";
    std::cin >> initIsoSig;
    
    std::cout << "Lower bound goal: ";
    std::cin >> edgeLowerbound;

    std::cout << "Max. number of attempts per run: ";
    std::cin >> twoFourCap;

    std::cout << "Max. number of 3-3 moves: ";
    std::cin >> threeThreeCap;
    
    std::unique_ptr<regina::Triangulation<4>> init_mfld(regina::Triangulation<4>::fromIsoSig(initIsoSig));
    
    size_t ogEdges, numEdges;
    ogEdges = numEdges = init_mfld->countEdges();

    std::cout << "Original number of edges: " << ogEdges << "\n";
    
    bool runaway = false;
    
    int current_smallest = INFINITY;
        
    clock_t time = clock();
    
    while (numEdges > edgeLowerbound) {
        bool doIT = true;

        int attempts = 1;
    
        while (doIT) {

            for (int i=0; i<attempts; i++) {
                for (regina::Tetrahedron<4>* tet : init_mfld->tetrahedra()) {
                    if (init_mfld->pachner(tet,true,true)) {
                        break;
                    }
                }
            }
        
            init_mfld->intelligentSimplify();
            numEdges = init_mfld->countEdges();
        
            std::cout << attempts << ", " << numEdges << std::endl;
            
            attempts+=1;

            if (numEdges<ogEdges) {
                doIT = false;
                ogEdges = numEdges;
                break;
            }
            
                for (int i=0; i<threeThreeCap; i++) {
                    for (regina::Triangle<4>* tri : init_mfld->triangles()) {
                        if (init_mfld->pachner(tri,true,true)) {
                            break;
                        }
                    }
                }
            
            if (attempts > twoFourCap) {
                doIT = false;
//                for (int i=0; i<threeThreeCap; i++) {
//                    //std::cout << "3"<<i<< " Trying some 3-3 moves...\n";
//                    for (regina::Triangle<4>* tri : init_mfld->triangles()) {
//                        if (init_mfld->pachner(tri,true,true)) {
//                            break;
//                        }
//                    }
//                }
//                std::cout << "Perfomed " << threeThreeCap << " 3-3 moves.\n";
                break;
            }

        }
    }

    time = clock() - time;
    double time_taken = ((double)time)/CLOCKS_PER_SEC;
    
    if (!runaway) {
        std::cout << "Resultant isomorphism signature: " << init_mfld->isoSig() << "\n";
    }
    else {
        std::cout << "Original isomorphism signature: " << initIsoSig << "\n";
    }
    
    std::cout << "Time taken: " << time_taken << std::endl;
    
    
    return 0;
}
