
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/**
 * Thanks to Ryan Budney and Damien Heard for providing the bulk of the
 * code in this file.
 *
 * Ryan Budney contributed the Orb / Casson import filters for Regina.
 * From his initial comments:
 *
 *   This file contains the engine routines to read Casson/Orb format
 *   triangulations and import them into Regina.  The main routines are
 *   adapted from Damian Heard's Orb.
 *                                      -Ryan Budney (April 3rd, 2006)
 *
 * For information on which routines are adapted from Orb and how they
 * have been modified, see the comments above each routine.  For information
 * on Orb itself, see http://www.ms.unimelb.edu.au/~snap/orb.html .
 */

#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "foreign/casson.h"
#include "foreign/orb.h"
#include "triangulation/ntriangulation.h"
#include "utilities/stringutils.h"

namespace regina {

// Anonymous namespace for the private routines used only by this file.
namespace {

/**
 * Modified from Orb's cassonToTriangulation() routine.
 *
 * The routine was changed to be compatible with Regina's NTriangulation
 * data structure.
 */
NTriangulation *cassonToNTriangulation( CassonFormat *cf )
 {
 int i;
 NTriangulation *triang = new NTriangulation();
 // since CassonFormat does not allow naming of triangulations,
 //  triang is given a name in the readOrb() function.
 //  I try to mimic NTriangulation::readSnapPea and
 //  Orb::cassonToTriangulation as much as possible.
 NTetrahedron **tet = new NTetrahedron*[cf->num_tet]; // tet corresponds to tet_array in Orb
 for (i=0; i<cf->num_tet; i++)
        tet[i]=new NTetrahedron();
 // now tet is a pointer to an array of NTetrahedrons,
 //  so for each tet[i] we need to run
 //   for (j=0; j<4; j++)
 //     tet[i]->joinTo(j,tet[g[j]],NPerm(p[j][0],p[j][1],p[j][2],p[j][3],p[j][4]))
 //     where g[j] is the tetrahedron adjacent to face j of tet[i]
 //           p[j][k] is the permutation specifying how the faces are glued together.
 EdgeInfo *ei;
 TetEdgeInfo *tei1, *tei2;
 int t1, t2, a1, a2, a3, a4, b1, b2, b3, b4;
 ei = cf->head;

 // this routine goes through the edges of cf, picking off the adjacent
 // tetrahedra and assembled the information into tet. this code is
 // adapted from Orb::cassonToTriangulation in Orb's organizer.cpp
 while (ei!=NULL) // if we have a non-trivial edge, proceed
        {
        tei1 = ei->head;
        while (tei1!=NULL) // now we spin about the tetrahedra adj to ei.
                {
                if (tei1->next==NULL)
                        tei2 = ei->head;
                else    tei2 = tei1->next;

                t1 = tei1->tet_index;
                a1 = tei1->f1;
                a2 = tei1->f2;
                a3 = vertex_at_faces[a1][a2];
                a4 = vertex_at_faces[a2][a1];

                t2 = tei2->tet_index;
                b1 = tei2->f1;
                b2 = tei2->f2;
                b3 = vertex_at_faces[b1][b2];
                b4 = vertex_at_faces[b2][b1];

                tet[t1]->joinTo( tei1->f1 , tet[t2], // 1st entry is the face of tet[t1]
                                NPerm(a1,b2,a2,b1,a3,b3,a4,b4) ); // being attached to tet[t2]

                tet[t2]->joinTo( tei2->f2 , tet[t1],
                                NPerm(b1,a2,b2,a1,b3,a3,b4,a4) );

                tei1 = tei1->next;
                }
        ei = ei->next;
        }

 for (i=0; i<cf->num_tet; i++)
        triang->addTetrahedron(tet[i]);
 delete[] tet;

 return triang;
}


/**
 * A direct copy of Orb's readCassonFormat() routine.
 */
CassonFormat *readCassonFormat( std::istream &ts )
{
        CassonFormat    *cf;
        std::string     line,
                        section;
        EdgeInfo        *nei,
                        *ei;
        TetEdgeInfo     *ntei,
                        *tei;

        cf = new CassonFormat;
        cf->head = NULL;
        cf->num_tet = 0;

        // Find a non-empty line.
        // The code from Orb used QString::skipWhiteSpace(); we do it
        // manually.
        do {
                getline(ts, line);
                stripWhitespace(line);
        } while ((! ts.eof()) && line.empty());

        // Process lines one at a time until we hit an empty line or EOF.
        while ((! ts.eof()) && (! line.empty()) && (line != "% diagram"))
        {
                // The code from Orb used QString's record separation
                // routines.  We don't have that in std::string, so
                // we'll do it all with istringstreams instead.
                std::istringstream tokens(line);

                nei = new EdgeInfo;

                if (cf->head==NULL)
                        cf->head = nei;
                else    ei->next = nei;

                nei->next = NULL;
                nei->head = NULL;

                ei = nei;

                tokens >> ei->index;
                ei->index--;

                // We never use these two values; just suck them in and
                // forget them.
                tokens >> ei->singular_index >> ei->singular_order;

                tokens >> section;
                while (!section.empty())
                {
                        ntei = new TetEdgeInfo;

                        if (ei->head==NULL)
                                ei->head        = ntei;
                        else
                                tei->next       = ntei;

                        ntei->next = NULL;
                        tei = ntei;

                        tei->f1 = LN(section[section.length()-2]);
                        tei->f2 = LN(section[section.length()-1]);
                        section.resize(section.length()-2);
                        tei->tet_index = atoi(section.c_str()) - 1;

                        if (tei->tet_index + 1 > cf->num_tet)
                                cf->num_tet = tei->tet_index + 1;

                        section.clear();
                        tokens >> section;
                }

                getline(ts, line);
        }

        return cf;
}

/**
 * A direct copy of Orb's verifyCassonFormat() routine.
 */
bool verifyCassonFormat( CassonFormat *cf )
{
        int             i,j,k;
        bool            check[4][4];
        EdgeInfo        *ei;
        TetEdgeInfo     *tei;

        for(i=0;i<cf->num_tet;i++)
        {
                for(j=0;j<4;j++)
                        for(k=0;k<4;k++)
                        if (j==k)
                                check[j][k] = true;
                        else    check[j][k] = false;

                ei = cf->head;

                if (ei == NULL)
                        return false;

                while(ei!=NULL)
                {
                        tei = ei->head;
                        if (tei == NULL)
                                return false;
                        while(tei!=NULL)
                        {
                                if (tei->tet_index == i )
                                {
                                        if (check[tei->f1][tei->f2])
                                                return true;
                                        check[tei->f1][tei->f2] = true;
                                        check[tei->f2][tei->f1] = true;
                                }
                                tei = tei->next;
                        }
                        ei = ei->next;
                }

                for(j=0;j<4;j++)
                        for(k=0;k<4;k++)
                        if (check[j][k]==false)
                                return false;
        }
        return true;
}

/**
 * A direct copy of Orb's freeCassonFormat() routine.
 */
void freeCassonFormat( CassonFormat *cf )
{
        EdgeInfo *e1, *e2;
        TetEdgeInfo *t1, *t2;
        e1 = cf->head;

        while (e1!=NULL)
        {
                e2 = e1->next;
                t1 = e1->head;
                while (t1!=NULL)
                {
                        t2 = t1->next;
                        delete t1;
                        t1 = t2;
                }
                delete e1;
                e1 = e2;
        }
        delete cf;
}

/**
 * Modified from Orb's readTriangulation() routine.
 *
 * The routine was changed to be compatible with Regina's NTriangulation
 * data structure.
 */
NTriangulation *readTriangulation( std::istream &ts,  std::string &file_id)
{

        NTriangulation *manifold = 0;
        std::string line;
        getline(ts, line);
        CassonFormat *cf;

        if (line == "% orb")
         {
                 getline(ts, file_id);
         cf=readCassonFormat( ts );

         if (verifyCassonFormat( cf ))
                 manifold = cassonToNTriangulation( cf );
         else
             {
                 std::cout<<"Error (3) reading Orb/Casson file.\n";std::cout.flush();
             }
         }
        else std::cout<<"Error (2) reading Orb/Casson file.\n";std::cout.flush();

        freeCassonFormat( cf );
        return manifold;
}

} // End anonymous namespace

NTriangulation *readOrb(const char *filename)
 {
   std::string file_id;

   std::ifstream file(filename);
   if (! file)
    {
     std::cout<<"Error (1) opening Orb/Casson file.\n";std::cout.flush();
    }
   NTriangulation *triang = readTriangulation(file, file_id);

   if (triang)
     triang->setPacketLabel(file_id);

   return triang;
 }

} // namespace regina
