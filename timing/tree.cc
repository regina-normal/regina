
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of splitting surface signatures of given order          *
 *                                                                        *
 *  Copyright (c) 2013-2016, Ben Burton                                   *
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

#include <enumerate/treetraversal.h>
#include <surfaces/normalsurfaces.h>
#include <triangulation/dim3.h>

using namespace regina;

/**
 * Output information on how to use this program, and then terminate the
 * entire program with return value 1.
 *
 * @param program the name of this program (typically argv[0]).
 */
void usage(const char* program) {
    std::cerr << "Usage:\n\n"
        << program << " ( -s | -q | -a | -o | -e | -3 ) [--] <isosig> ...\n\n"
        "    -s : Use standard coordinates (7n dimensions)\n"
        "    -q : Use quadrilateral coordinates (3n dimensions)\n"
        "    -a : Use standard almost normal coordinates (10n dimensions)\n"
        "    -o : Use quadrilateral-octagon coordinates (6n dimensions)\n"
        "    -e : Only detect 0-efficiency (not full vertex enumeration)\n"
        "    -3 : Detect an almost normal sphere (not full vertex enumeration)\n"
        "\n";
    ::exit(1);
}

/**
 * A variant of TreeTraversal::writeTypes() that also performs sanity
 * checking on the normal surface.
 */
template <class LPConstraint, typename BanConstraint>
bool writeTypesAndVerify(
        const TreeEnumeration<LPConstraint, BanConstraint>& tree, void* eqns) {
    /*
    std::cout << "SOLN #" << tree.nSolns() << ": ";
    tree.dumpTypes(std::cout);
    std::cout << std::endl;
    */

    NormalSurface* s = tree.buildSurface();
    if (! tree.verify(s, static_cast<const MatrixInt*>(eqns)))
        std::cout << "ERROR: Verification failed." << std::endl;

    return true;
}

/**
 * The main body of the program.
 *
 * For each isomorphism signature passed on the command line, we build
 * the corresponding triangulation and then run away to perform the tree
 * traversal algorithm.
 */
int main(int argc, char* argv[]) {
    char mode = 0;

    if (argc == 1)
        usage(argv[0]);

    bool found = false;
    bool noMoreSwitches = false;
    for (int i = 1; i < argc; ++i) {
        // Handle command-line arguments:
        if (argv[i][0] == '-' && ! noMoreSwitches) {
            if (found)
                usage(argv[0]);
            switch (argv[i][1]) {
                case '-':
                    noMoreSwitches = true;
                    break;
                case 'q':
                case 's':
                case 'a':
                case 'o':
                case 'e':
                case '3':
                    if (mode && mode != argv[i][1])
                        usage(argv[0]);
                    mode = argv[i][1];
                    break;
                default:
                    usage(argv[0]);
            }
            continue;
        }
        if (! mode)
            usage(argv[0]);

        // Process the next triangulation on the command line:
        Triangulation<3>* t = Triangulation<3>::fromIsoSig(argv[i]);
        if (t) {
            found = true;

            if (t->isOrientable())
                t->orient();

            std::cout << "PROCESSING: " << argv[i] << std::endl;
            if (mode == 's' || mode == 'q' || mode == 'a' || mode == 'o') {
                NormalCoords coords = (mode == 's' ? NS_STANDARD :
                    mode == 'q' ? NS_QUAD :
                    mode == 'a' ? NS_AN_STANDARD :
                    NS_AN_QUAD_OCT);
                TreeEnumeration<> search(t, coords);
                if (search.constraintsBroken())
                    std::cerr << "ERROR: Constraints broken." << std::endl;
                else {
                    // Build the matching equations for sanity checking.
                    MatrixInt* eqns = regina::makeMatchingEquations(t, coords);

                    search.run(&writeTypesAndVerify<LPConstraintNone,
                        BanNone>, eqns);
                    std::cout << "# solutions = " << search.nSolns()
                        << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited() 
                        << std::endl;

                    delete eqns;
                }
            } else {
                TreeSingleSoln<LPConstraintEuler>
                    search(t, mode == '3' ? NS_AN_STANDARD : NS_STANDARD);
                if (search.constraintsBroken())
                    std::cerr << "ERROR: Constraints broken." << std::endl;
                else if (search.find()) {
                    std::cout << "Found non-trivial Euler > 0:" << std::endl;
                    search.dumpTypes(std::cout);
                    std::cout << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited()
                        << std::endl;

                    // Sanity checking:
                    NormalSurface* s = search.buildSurface();
                    if (! search.verify(s))
                        std::cout << "ERROR: Verification failed." << std::endl;
                } else {
                    std::cout << "No non-trivial solution with Euler > 0"
                        << std::endl;
                    std::cout << "# nodes visited = " << search.nVisited()
                        << std::endl;
                }
            }
        } else
            std::cerr << "ERROR: Could not reconstruct " << argv[i]
                << std::endl;
        delete t;
    }

    if (! found)
        usage(argv[0]);

    return 0;
}
