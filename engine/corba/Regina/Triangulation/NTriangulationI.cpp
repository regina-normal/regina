
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

#include "NTriangulationI.h"
#include "NComponentI.h"
#include "NBoundaryComponentI.h"
#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NTetrahedronI.h"
#include "NAbelianGroupI.h"
#include "NGroupPresentationI.h"

CORBA::Long NTriangulation_i::getNumberOfTetrahedra() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getNumberOfTetrahedra();
}
Regina::Triangulation::NTetrahedron_ptr NTriangulation_i::getTetrahedron(
        CORBA::Long index) {
    return NTetrahedron_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        getTetrahedron(index));
}
CORBA::Long NTriangulation_i::getTetrahedronIndex(
        Regina::Triangulation::NTetrahedron_ptr tetrahedron) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getTetrahedronIndex(
        GET_ENGINE_OBJECT(NTetrahedron, tetrahedron));
}
void NTriangulation_i::addTetrahedron(
        Regina::Triangulation::NTetrahedron_ptr newTet) {
    GET_ENGINE_OBJECT(NTriangulation, this)->addTetrahedron(
        GET_ENGINE_OBJECT(NTetrahedron, newTet));
}
void NTriangulation_i::removeTetrahedronAt(CORBA::Long index) {
    GET_ENGINE_OBJECT(NTriangulation, this)->removeTetrahedronAt(index);
}
void NTriangulation_i::removeTetrahedron(
        Regina::Triangulation::NTetrahedron_ptr tet) {
    GET_ENGINE_OBJECT(NTriangulation, this)->removeTetrahedron(
        GET_ENGINE_OBJECT(NTetrahedron, tet));
}
void NTriangulation_i::removeAllTetrahedra() {
    GET_ENGINE_OBJECT(NTriangulation, this)->removeAllTetrahedra();
}
void NTriangulation_i::gluingsHaveChanged() {
    GET_ENGINE_OBJECT(NTriangulation, this)->gluingsHaveChanged();
}
CORBA::Long NTriangulation_i::getNumberOfComponents() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getNumberOfComponents();
}
Regina::Triangulation::NComponent_ptr NTriangulation_i::getComponent(
        CORBA::Long index) {
    return NComponent_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        getComponents()[index]);
}
CORBA::Long NTriangulation_i::getComponentIndex(
        Regina::Triangulation::NComponent_ptr component) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getComponents().position(
        GET_ENGINE_OBJECT(NComponent, component));
}
CORBA::Long NTriangulation_i::getNumberOfBoundaryComponents() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->
        getNumberOfBoundaryComponents();
}
Regina::Triangulation::NBoundaryComponent_ptr
        NTriangulation_i::getBoundaryComponent(CORBA::Long index) {
    return NBoundaryComponent_i::newWrapper(
        GET_ENGINE_OBJECT(NTriangulation, this)->
        getBoundaryComponents()[index]);
}
CORBA::Long NTriangulation_i::getBoundaryComponentIndex(
        Regina::Triangulation::NBoundaryComponent_ptr bc) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getBoundaryComponents().
        position(GET_ENGINE_OBJECT(NBoundaryComponent, bc));
}
CORBA::Long NTriangulation_i::getNumberOfVertices() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getNumberOfVertices();
}
Regina::Triangulation::NVertex_ptr NTriangulation_i::getVertex(
        CORBA::Long index) {
    return NVertex_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        getVertices()[index]);
}
CORBA::Long NTriangulation_i::getVertexIndex(
        Regina::Triangulation::NVertex_ptr vertex) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getVertices().position(
        GET_ENGINE_OBJECT(NVertex, vertex));
}
CORBA::Long NTriangulation_i::getNumberOfEdges() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getNumberOfEdges();
}
Regina::Triangulation::NEdge_ptr NTriangulation_i::getEdge(
        CORBA::Long index) {
    return NEdge_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        getEdges()[index]);
}
CORBA::Long NTriangulation_i::getEdgeIndex(
        Regina::Triangulation::NEdge_ptr edge) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getEdges().position(
        GET_ENGINE_OBJECT(NEdge, edge));
}
CORBA::Long NTriangulation_i::getNumberOfFaces() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getNumberOfFaces();
}
Regina::Triangulation::NFace_ptr NTriangulation_i::getFace(
        CORBA::Long index) {
    return NFace_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        getFaces()[index]);
}
CORBA::Long NTriangulation_i::getFaceIndex(
        Regina::Triangulation::NFace_ptr face) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getFaces().position(
        GET_ENGINE_OBJECT(NFace, face));
}
CORBA::Long NTriangulation_i::getEulerCharacteristic() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getEulerCharacteristic();
}
Regina::Algebra::NGroupPresentation_ptr
        NTriangulation_i::getFundamentalGroup() {
    return NGroupPresentation_i::newWrapper(&(::NGroupPresentation&)
        (GET_ENGINE_OBJECT(NTriangulation, this)->getFundamentalGroup()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (GET_ENGINE_OBJECT(NTriangulation, this)->getHomologyH1()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1Rel() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (GET_ENGINE_OBJECT(NTriangulation, this)->getHomologyH1Rel()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1Bdry() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (GET_ENGINE_OBJECT(NTriangulation, this)->getHomologyH1Bdry()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH2() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (GET_ENGINE_OBJECT(NTriangulation, this)->getHomologyH2()));
}
CORBA::Long NTriangulation_i::getHomologyH2Z2() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->getHomologyH2Z2();
}
CORBA::Boolean NTriangulation_i::isIdeal() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isIdeal();
}
CORBA::Boolean NTriangulation_i::isValid() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isValid();
}
CORBA::Boolean NTriangulation_i::isStandard() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isStandard();
}
CORBA::Boolean NTriangulation_i::isClosed() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isClosed();
}
CORBA::Boolean NTriangulation_i::isOrientable() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isOrientable();
}
CORBA::Boolean NTriangulation_i::isConnected() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isConnected();
}
CORBA::Boolean NTriangulation_i::hasBoundaryFaces() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->hasBoundaryFaces();
}
CORBA::Boolean NTriangulation_i::crushMaximalForest() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->crushMaximalForest();
}
CORBA::Boolean NTriangulation_i::intelligentSimplify() {
    return GET_ENGINE_OBJECT(NTriangulation, this)->intelligentSimplify();
}
CORBA::Boolean NTriangulation_i::simplifyToLocalMinimum(
        CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->
        simplifyToLocalMinimum(perform);
}
CORBA::Boolean NTriangulation_i::threeTwoMove(
        Regina::Triangulation::NEdge_ptr e,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->threeTwoMove(
        GET_ENGINE_OBJECT(NEdge, e), check, perform);
}
CORBA::Boolean NTriangulation_i::twoThreeMove(
        Regina::Triangulation::NFace_ptr f,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->twoThreeMove(
        GET_ENGINE_OBJECT(NFace, f), check, perform);
}
CORBA::Boolean NTriangulation_i::twoZeroMove_NEdge(
        Regina::Triangulation::NEdge_ptr e,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->twoZeroMove(
        GET_ENGINE_OBJECT(NEdge, e), check, perform);
}
CORBA::Boolean NTriangulation_i::twoZeroMove_NVertex(
        Regina::Triangulation::NVertex_ptr v,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->twoZeroMove(
        GET_ENGINE_OBJECT(NVertex, v), check, perform);
}
CORBA::Boolean NTriangulation_i::twoOneMove(
        Regina::Triangulation::NEdge_ptr e, CORBA::Long edgeEnd,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->twoOneMove(
        GET_ENGINE_OBJECT(NEdge, e), edgeEnd, check, perform);
}
CORBA::Boolean NTriangulation_i::openBook(
        Regina::Triangulation::NFace_ptr f,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->openBook(
        GET_ENGINE_OBJECT(NFace, f), check, perform);
}
CORBA::Boolean NTriangulation_i::shellBoundary(
        Regina::Triangulation::NTetrahedron_ptr t,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->shellBoundary(
        GET_ENGINE_OBJECT(NTetrahedron, t), check, perform);
}
CORBA::Boolean NTriangulation_i::idealToFinite(CORBA::Boolean forceDivision) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->
        idealToFinite(forceDivision);
}
void NTriangulation_i::barycentricSubdivision() {
    GET_ENGINE_OBJECT(NTriangulation, this)->barycentricSubdivision();
}
Regina::Triangulation::NTetrahedron_ptr
        NTriangulation_i::insertLayeredSolidTorus(
        CORBA::Long cuts0, CORBA::Long cuts1) {
    return NTetrahedron_i::newWrapper(GET_ENGINE_OBJECT(NTriangulation, this)->
        insertLayeredSolidTorus(cuts0, cuts1));
}
void NTriangulation_i::insertLensSpace(CORBA::Long p, CORBA::Long q) {
    GET_ENGINE_OBJECT(NTriangulation, this)->insertLensSpace(p, q);
}
void NTriangulation_i::insertTriangulation(
        Regina::Triangulation::NTriangulation_ptr source) {
    GET_ENGINE_OBJECT(NTriangulation, this)->insertTriangulation(
        *GET_ENGINE_OBJECT(NTriangulation, source));
}
CORBA::Boolean NTriangulation_i::isIsomorphicTo(
        Regina::Triangulation::NTriangulation_ptr other) {
    return GET_ENGINE_OBJECT(NTriangulation, this)->isIsomorphicTo(
        *GET_ENGINE_OBJECT(NTriangulation, other));
}

