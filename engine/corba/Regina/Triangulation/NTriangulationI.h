
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#ifndef __NTRIANGULATIONI_H
#define __NTRIANGULATIONI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntriangulation.h"
#else
    #include "engine/triangulation/ntriangulation.h"
#endif

#include "NTriangulationIDL.h"
#include "NPacketI.h"

class NTriangulation_i :
        public virtual POA_Regina::Triangulation::NTriangulation,
        public NPacket_i {
    STANDARD_ENGINE_TYPEDEFS(NTriangulation_i, NTriangulation,
            Regina::Triangulation::NTriangulation)

    protected:
        NTriangulation_i(::NTriangulation* newCppPtr) : NPacket_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Long getNumberOfTetrahedra();
        virtual Regina::Triangulation::NTetrahedron_ptr getTetrahedron(
            CORBA::Long index);
        virtual CORBA::Long getTetrahedronIndex(
            Regina::Triangulation::NTetrahedron_ptr tetrahedron);
        virtual void addTetrahedron(
            Regina::Triangulation::NTetrahedron_ptr newTet);
        virtual void removeTetrahedronAt(CORBA::Long index);
        virtual void removeTetrahedron(
            Regina::Triangulation::NTetrahedron_ptr tet);
        virtual void removeAllTetrahedra();
        virtual void gluingsHaveChanged();
        virtual CORBA::Long getNumberOfComponents();
        virtual Regina::Triangulation::NComponent_ptr getComponent(
            CORBA::Long index);
        virtual CORBA::Long getComponentIndex(
            Regina::Triangulation::NComponent_ptr component);
        virtual CORBA::Long getNumberOfBoundaryComponents();
        virtual Regina::Triangulation::NBoundaryComponent_ptr
            getBoundaryComponent(CORBA::Long index);
        virtual CORBA::Long getBoundaryComponentIndex(
            Regina::Triangulation::NBoundaryComponent_ptr bc);
        virtual CORBA::Long getNumberOfVertices();
        virtual Regina::Triangulation::NVertex_ptr getVertex(CORBA::Long index);
        virtual CORBA::Long getVertexIndex(
            Regina::Triangulation::NVertex_ptr vertex);
        virtual CORBA::Long getNumberOfEdges();
        virtual Regina::Triangulation::NEdge_ptr getEdge(CORBA::Long index);
        virtual CORBA::Long getEdgeIndex(
            Regina::Triangulation::NEdge_ptr edge);
        virtual CORBA::Long getNumberOfFaces();
        virtual Regina::Triangulation::NFace_ptr getFace(CORBA::Long index);
        virtual CORBA::Long getFaceIndex(
            Regina::Triangulation::NFace_ptr face);
        virtual CORBA::Long getEulerCharacteristic();
        virtual Regina::Algebra::NGroupPresentation_ptr getFundamentalGroup();
        virtual Regina::Algebra::NAbelianGroup_ptr getHomologyH1();
        virtual Regina::Algebra::NAbelianGroup_ptr getHomologyH1Rel();
        virtual Regina::Algebra::NAbelianGroup_ptr getHomologyH1Bdry();
        virtual Regina::Algebra::NAbelianGroup_ptr getHomologyH2();
        virtual CORBA::Long getHomologyH2Z2();
        virtual CORBA::Boolean isIdeal();
        virtual CORBA::Boolean isValid();
        virtual CORBA::Boolean isStandard();
        virtual CORBA::Boolean isClosed();
        virtual CORBA::Boolean isOrientable();
        virtual CORBA::Boolean isConnected();
        virtual CORBA::Boolean hasBoundaryFaces();
        virtual CORBA::Boolean isZeroEfficient();
        virtual CORBA::Boolean hasCentralSurface();
        virtual CORBA::Boolean crushMaximalForest();
        virtual CORBA::Boolean intelligentSimplify();
        virtual CORBA::Boolean simplifyToLocalMinimum(CORBA::Boolean perform);
        virtual CORBA::Boolean threeTwoMove(
            Regina::Triangulation::NEdge_ptr e,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean twoThreeMove(
            Regina::Triangulation::NFace_ptr f,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean fourFourMove(
            Regina::Triangulation::NEdge_ptr e, CORBA::Long newAxis,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean twoZeroMove_NEdge(
            Regina::Triangulation::NEdge_ptr e,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean twoZeroMove_NVertex(
            Regina::Triangulation::NVertex_ptr v,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean twoOneMove(
            Regina::Triangulation::NEdge_ptr e, CORBA::Long edgeEnd,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean openBook(
            Regina::Triangulation::NFace_ptr f,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean shellBoundary(
            Regina::Triangulation::NTetrahedron_ptr t,
            CORBA::Boolean check, CORBA::Boolean perform);
        virtual CORBA::Boolean idealToFinite(CORBA::Boolean forceDivision);
        virtual void barycentricSubdivision();
        virtual Regina::Triangulation::NTetrahedron_ptr
            insertLayeredSolidTorus(CORBA::Long cuts0, CORBA::Long cuts1);
        virtual void insertLensSpace(CORBA::Long p, CORBA::Long q);
        virtual void insertTriangulation(
            Regina::Triangulation::NTriangulation_ptr source);
        virtual CORBA::Boolean isIsomorphicTo(
            Regina::Triangulation::NTriangulation_ptr other);
};

#endif

