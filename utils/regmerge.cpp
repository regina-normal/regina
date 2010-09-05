
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Convert old-style binary data files to new-style XML                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "file/nxmlfile.h"
#include "packet/npacket.h"

#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

/**
 * This program takes an arbitrary collection of regina files and merges them, maintaining
 * what it can of both tree structures.
 */

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName <<
        " [ -o <output-file> ] <data-file> ...\n";
    std::cerr << std::endl;
    std::cerr << "    -o <output-file> : Write to the given data file "
        "(otherwise standard\n";
    std::cerr << "                       output is used)\n";
    exit(1);
}

/**
 * Given a parent packet and another packet toInsert we want to insert a copy of
 * toInsert as a child of parent.  Moreover, we want to preserve as much of the 
 * tree structure of parent as much as possible, so if toInsert is an NContainer, 
 * we insert a copy of it if parent doesn't contain an NContainer with that packet
 * label.  If it's not an NContainer, we re-parent.  We also maintain the relative 
 * order (as much as possible) of the children of parent, so we use a 
 * lexicographical ordering of the packet labels on insertion.
 * Routine returns a pointer to the (possibly pre-existing) "inserted" child of
 * parent.   To get consistent results, 
 * among any group of siblings in the parent packet, no two NContainer siblings should
 * have identical packet labels when calling orderedInsertAsChildOf()
 */
regina::NPacket* orderedInsertAsChildOf( regina::NPacket* toInsert, regina::NPacket* parent)
{
 // now we have to figure out where to put it. Do this with a while loop that iterates through children.
 //         stopping if we either (a) find an exact match                    found in while loop
 //                               (b) find child(n) < toInsert < child(n+1)  found in while loop
 //                               (c) find toInsert < child(1)               found in while loop
 //                               (d) find lastchild < toInsert              found after while loop
 regina::NPacket* packPrev(NULL); regina::NContainer* newContainer(NULL);
 regina::NPacket* packNext( parent->getFirstTreeChild() );
 while (packNext!=NULL)
  {
   // check for (a)
   if ( toInsert->getPacketLabel() == packNext->getPacketLabel() )
    {
      // be careful to avoid creating duplicate containers in ans's tree structure
      if ( (toInsert->getPacketType() != regina::NContainer::packetType ) || 
           (packNext->getPacketType() != regina::NContainer::packetType ) )
       { // we'll put the NContainer first, if there is one.
         if (toInsert->getPacketType() == regina::NContainer::packetType )
          { newContainer=new regina::NContainer; newContainer->setPacketLabel( toInsert->getPacketLabel() );
            parent->insertChildFirst( newContainer ); return newContainer; }
         else 
          { toInsert->makeOrphan(); parent->insertChildAfter( toInsert, packNext ); 
            return toInsert; }
       }
      else { return packNext; } // nothing to do.
    } else
   // check for (c) toInsert before all siblings
   if ( (packPrev==NULL) ? ( toInsert->getPacketLabel() < packNext->getPacketLabel() ) : false )
    { // only move if not an NContainer
      if ( toInsert->getPacketType() != regina::NContainer::packetType )
       { toInsert->makeOrphan(); parent->insertChildFirst( toInsert ); return toInsert; }
      else
       { newContainer=new regina::NContainer; newContainer->setPacketLabel( toInsert->getPacketLabel() );
         parent->insertChildFirst( newContainer ); return newContainer; }
    } else
   // check for (b) toInsert between two siblings packPrev and packNext
   if ( (packPrev!=NULL) ? ( packPrev->getPacketLabel() < toInsert->getPacketLabel() ) &&
                           ( toInsert->getPacketLabel() <  packNext->getPacketLabel() ) : false )
    { 
      if ( toInsert->getPacketType() != regina::NContainer::packetType )
       { toInsert->makeOrphan(); parent->insertChildAfter( toInsert, packPrev ); return toInsert; }
      else
       { newContainer=new regina::NContainer; newContainer->setPacketLabel( toInsert->getPacketLabel() );
         parent->insertChildAfter( newContainer, packPrev ); return newContainer; }
    }
   // prep for next loop
   packPrev = packNext; packNext = packNext->getNextTreeSibling();
  } // end while loop
 // was the list empty? If so just insert. 
 if (packPrev==NULL) 
   {
      if ( toInsert->getPacketType() != regina::NContainer::packetType )
       { toInsert->makeOrphan(); parent->insertChildFirst( toInsert ); return toInsert; }
      else
       { newContainer=new regina::NContainer; newContainer->setPacketLabel( toInsert->getPacketLabel() );
         parent->insertChildFirst( newContainer ); return newContainer; }
   }
 else // else the (d) possibility
   {
      if ( toInsert->getPacketType() != regina::NContainer::packetType )
       { toInsert->makeOrphan(); parent->insertChildLast( toInsert ); return toInsert; }
      else
       { newContainer=new regina::NContainer; newContainer->setPacketLabel( toInsert->getPacketLabel() );
         parent->insertChildLast( newContainer ); return newContainer; }
   }
 // shouldn't get this far!
 std::cout<<"orderedInsertChildOf() should never get this far. \n"; std::cout.flush(); exit(1); // TODO erase
 return NULL;
 // done
}

int main(int argc, char* argv[])
{
    // Parse the command line.
    std::list<std::string> inputfiles;
    std::string outputFile;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            // Option.

            // Is it an argument we don't understand?
            if (strcmp(argv[i], "-o"))
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Are we missing the actual output filename?
            if (i == argc - 1)
                usage(argv[0], "Option -o is missing an output filename.");

            // Have we already specified an output filename?
            if (! outputFile.empty())
                usage(argv[0],
                    "More than one output filename has been supplied.");

            // Suck in the output filename.
            outputFile = argv[++i];
        } else if (*argv[i]) {
            // Just an ordinary filename.
            inputfiles.push_back(argv[i]);
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }
    if (inputfiles.empty())
        usage(argv[0], "No data files have been supplied.");

    // Read the input files one by one.
    regina::NPacket* ans = new regina::NContainer();
    regina::NPacket* ansTreeTop(ans);
    ans->setPacketLabel("Combined Data");

    bool error = false;
    regina::NPacket* data;
    for(std::list<std::string>::const_iterator it = inputfiles.begin();
            it != inputfiles.end(); it++) 
       {
        data = regina::readFileMagic(*it);
        if (!data) 
         {
            std::cerr << "File " << *it << " could not be read.\n";
            error = true;
            continue;
         }
        std::cout<<"Read: "<<(*it)<<" "; std::cout.flush();

        // to ensure data is sorted, we run through all of its NContainers and perform
        // sortChildren() on them. 
        regina::NPacket* dataTreeTop( data ); 
        while (data != NULL)
         { data->sortChildren(); data = data->nextTreePacket("Container"); }
        std::cout<<"sorted "; std::cout.flush(); 

        // let's find the appropriate location in ans to place data.
        if (it == inputfiles.begin() ) 
         {
          data=dataTreeTop->getFirstTreeChild();
          while (data != NULL)
            {
             regina::NPacket* nextKid( data->getNextTreeSibling() );
             data->reparent(ans);
             data=nextKid;
            } 
          delete dataTreeTop;
         }
        else
         { // run through data, and insert appropriately, keeping track of which group of siblings to insert into
           data = dataTreeTop->getFirstTreeChild(); // normalize data
           unsigned long dataDepth( data->levelsUpTo(dataTreeTop) ); //, buildDepth(0);
           ans = ans->getTreeMatriarch(); ans = ans->getFirstTreeChild(); // normalize ans
           while (data!=NULL)
            {
            unsigned long dataUp( data->levelsUpTo( dataTreeTop ) );
             // sketch of procedure:
             // The assumption is that immediately before the latest data increment, both ans and data 
             // were pointing to corresponding objects in their respective tree structures. As data has
             // been incremented, we have to move ans to point to the appropriate group of siblings. 
             if (dataUp < dataDepth) // data has moved shallower, perhaps many levels.
               {
                 for (unsigned long i=0; i<dataDepth-dataUp; i++)
                   ans = ans->getTreeParent();
               } 
             regina::NPacket* nextData(data->nextTreePacket());
             ans = orderedInsertAsChildOf( data, (dataUp>dataDepth) ? ans : ans->getTreeParent() );
             // increment data
             dataDepth=dataUp;
             data = nextData;
            }

           delete dataTreeTop;
         }
      if (it != inputfiles.begin() ) delete data;
      std::cout<<" done merging. \n"; std::cout.flush();
     } // data file loop

    // Tidy up the final data file and write it.
    //ans->makeUniqueLabels(0);
    ans = ans->getTreeMatriarch();
    if (outputFile.empty())
        ans->writeXMLFile(std::cout);
    else if (! regina::writeXMLFile(outputFile.c_str(), ans))
        std::cerr << "File " << outputFile << " could not be written.\n";

    delete ans;
    return (error ? 1 : 0);


}



