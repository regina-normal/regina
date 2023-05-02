//
//  up-down-simplify.cpp
//  Engine
//
//  Created by Antonio Burke on 24/8/21.
//  Copyright © 2021 Regina Development Team. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <triangulation/dim4.h>

int main(int argc, char* argv[]){
    
    int twoFourCap,threeThreeCap;
    
    std::cout << "Max. number of 2-4 moves: ";
    std::cin >> twoFourCap;

    std::cout << "Max. number of 3-3 moves: ";
    std::cin >> threeThreeCap;

    int numberOfEpochs;
    std::cout << "Number of epochs: ";
    std::cin >> numberOfEpochs;

    std::string initIsoSig;
    std::cout << "Iso Sig: ";
    std::cin >> initIsoSig;
    
    std::string isoSigFile = "nilSigsFile.tmp";
    std::string simplificationDataFile = "nilDataFile.tmp";
    std::ofstream isoSigs, simplificationData;
    
    bool verbose = true;

    if (argc > 0) {
        isoSigFile = argv[1];
        std::string argString = std::string(argv[1]);
        simplificationDataFile = "Simplification Data - " + argString;
    }

    isoSigs.open(isoSigFile);
    simplificationData.open(simplificationDataFile);
    
    int currentEpoch = 1;
    
    while (currentEpoch <= numberOfEpochs) {
        regina::Triangulation<4> init_mfld = regina::Triangulation<4>::fromIsoSig(initIsoSig);
        
        size_t ogEdges, numEdges;
        ogEdges = numEdges = init_mfld.countEdges();
        auto ogIsoSig = init_mfld.isoSig();
    
        size_t runOrigEdges = ogEdges;
        
        clock_t time = clock();
        size_t edgeLowerbound = ogEdges - 1;
        while ((numEdges > edgeLowerbound)) {

            bool doIT = true;

            int attempts = 1;
        
            // Hardcoded stop if we hit 2 pentachora.
            if (init_mfld.size() == 2) {
                doIT = false;
                ogEdges = numEdges = edgeLowerbound;
                break;
            }
        
            while (doIT) {
                
                for (int i=0; i<attempts; i++) {
                    for (regina::Tetrahedron<4>* tet : init_mfld.tetrahedra()) {
                        if (init_mfld.pachner(tet,true,true)) {
                            break;
                        }
                    }
                }
        
                init_mfld.intelligentSimplify();
//                if (init_mfld.isoSig() != "eAMPcaabcddd+aoa+aAa8aQara") {
//                    isoSigs << init_mfld.isoSig() << std::endl;
//                }
                numEdges = init_mfld.countEdges();
        
                if (verbose) {
                    std::cout << "\r" << std::setw(3) << attempts << ", " << std::setw(3) << numEdges << std::flush;
                }
                
//                attempts+=1;

                if (numEdges<ogEdges) {
                    doIT = false;
                    ogEdges = numEdges;
                    break;
                }
            
                for (int i=0; i<threeThreeCap; i++) {
                    for (regina::Triangle<4>* tri : init_mfld.triangles()) {
                        if (init_mfld.pachner(tri,true,true)) {
                            break;
                        }
                    }
                }
                
                attempts+=1;
                
                if (attempts > twoFourCap) {
                    doIT = false;
                    break;
                }
                
            }

        }
        
        time = clock() - time;
        double time_taken = ((double)time)/CLOCKS_PER_SEC;
    
        std::cout << std::endl;
        std::cout << init_mfld.isoSig() << "\n";
        std::cout <<
        currentEpoch << " | " << init_mfld.countVertices() << ", " << runOrigEdges << " -> " << init_mfld.countEdges() << ", " << init_mfld.countPentachora() << " | " << time_taken << "\n";
        
        if (argc == 2) {
            simplificationData << currentEpoch << "," << init_mfld.countVertices() << "," << runOrigEdges << "," << init_mfld.countEdges() << "," << init_mfld.countPentachora() << "," << time_taken << std::endl;
        }
        
        initIsoSig = init_mfld.isoSig();
        
        currentEpoch+=1;
    }
    
    std::cout << "\007";
    
    isoSigs.close();
    simplificationData.close();
    
    if (argc == 1) {
        remove("nilSigsFile.tmp");
        remove("nilDataFile.tmp");
    }
    
    return 0;
}
