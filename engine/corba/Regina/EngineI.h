
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef __ENGINEI_H
#define __ENGINEI_H

#include "EngineIDL.h"

class Engine_i : public virtual Regina::_sk_Engine {
    public:
        Engine_i() {
        }
        virtual ~Engine_i() {
        }

        virtual Regina::Algebra::NAbelianGroup_ptr newNAbelianGroup_();
        virtual Regina::Algebra::NAbelianGroup_ptr
            newNAbelianGroup_NAbelianGroup(
            Regina::Algebra::NAbelianGroup_ptr cloneMe);

        virtual Regina::File::NFile_ptr newNFile();

        virtual Regina::Maths::NMatrixInt_ptr newNMatrixInt_long_long(
            CORBA::Long rows, CORBA::Long columns);
        virtual Regina::Maths::NMatrixInt_ptr newNMatrixInt_NMatrixInt(
            Regina::Maths::NMatrixInt_ptr cloneMe);

        virtual Regina::Packet::NContainer_ptr newNContainer();
        virtual Regina::Packet::NScript_ptr newNScript();
        virtual Regina::Packet::NText_ptr newNText_();
        virtual Regina::Packet::NText_ptr newNText_string(const char* text);

        virtual Regina::Progress::NProgressManager_ptr newNProgressManager();

        virtual Regina::Surfaces::NNormalSurfaceList_ptr newNNormalSurfaceList(
            Regina::Triangulation::NTriangulation_ptr owner,
            CORBA::Long flavour, CORBA::Boolean isEmbeddedOnly);
        virtual Regina::Surfaces::NSurfaceFilter_ptr newNSurfaceFilter_();
        virtual Regina::Surfaces::NSurfaceFilter_ptr
            newNSurfaceFilter_NSurfaceFilter(
            Regina::Surfaces::NSurfaceFilter_ptr cloneMe);
        virtual Regina::Surfaces::NSurfaceFilterCombination_ptr
            newNSurfaceFilterCombination_();
        virtual Regina::Surfaces::NSurfaceFilterCombination_ptr
            newNSurfaceFilterCombination_NSurfaceFilterCombination(
            Regina::Surfaces::NSurfaceFilterCombination_ptr cloneMe);
        virtual Regina::Surfaces::NSurfaceFilterProperties_ptr
            newNSurfaceFilterProperties_();
        virtual Regina::Surfaces::NSurfaceFilterProperties_ptr
            newNSurfaceFilterProperties_NSurfaceFilterProperties(
            Regina::Surfaces::NSurfaceFilterProperties_ptr cloneMe);
        virtual Regina::Surfaces::NSurfaceSubset_ptr newNSurfaceSubset(
            Regina::Surfaces::NSurfaceSet_ptr set,
            Regina::Surfaces::NSurfaceFilter_ptr filter);

        virtual Regina::Triangulation::NTetrahedron_ptr newNTetrahedron_();
        virtual Regina::Triangulation::NTetrahedron_ptr newNTetrahedron_string(
            const char* desc);
        virtual Regina::Triangulation::NTriangulation_ptr newNTriangulation_();
        virtual Regina::Triangulation::NTriangulation_ptr
            newNTriangulation_NTriangulation(
            Regina::Triangulation::NTriangulation_ptr cloneMe);

        virtual CORBA::Long formCensus(Regina::Packet::NPacket_ptr parent,
            CORBA::Long nTetrahedra, CORBA::Char finiteness,
            CORBA::Char orientability, CORBA::Char boundary,
            CORBA::Long nBdryFaces,
            Regina::Progress::NProgressManager_ptr manager);
        virtual CORBA::Long getVersionMajor();
        virtual CORBA::Long getVersionMinor();
        virtual char* getVersionString();
        virtual Regina::Maths::NMatrixInt_ptr makeMatchingEquations(
            Regina::Triangulation::NTriangulation_ptr triangulation,
            CORBA::Long flavour);
        virtual Regina::Packet::NPacket_ptr readFromFile(
            const char* fileName);
        virtual Regina::Triangulation::NTriangulation_ptr readSnapPea(
            const char* fileName);
        virtual void smithNormalForm(Regina::Maths::NMatrixInt_ptr matrix);
        virtual CORBA::Long testEngine(CORBA::Long value);
        virtual CORBA::Boolean writeToFile(const char* fileName,
            Regina::Packet::NPacket_ptr packet);
};

#endif

