
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
    return MY_ENGINE_OBJECT->getNumberOfTetrahedra();
}
Regina::Triangulation::NTetrahedron_ptr NTriangulation_i::getTetrahedron(
        CORBA::Long index) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->getTetrahedron(index));
}
CORBA::Long NTriangulation_i::getTetrahedronIndex(
        Regina::Triangulation::NTetrahedron_ptr tetrahedron) {
    return MY_ENGINE_OBJECT->getTetrahedronIndex(
        GET_ENGINE_OBJECT(NTetrahedron, tetrahedron));
}
void NTriangulation_i::addTetrahedron(
        Regina::Triangulation::NTetrahedron_ptr newTet) {
    MY_ENGINE_OBJECT->addTetrahedron(GET_ENGINE_OBJECT(NTetrahedron, newTet));
}
void NTriangulation_i::removeTetrahedronAt(CORBA::Long index) {
    MY_ENGINE_OBJECT->removeTetrahedronAt(index);
}
void NTriangulation_i::removeTetrahedron(
        Regina::Triangulation::NTetrahedron_ptr tet) {
    MY_ENGINE_OBJECT->removeTetrahedron(GET_ENGINE_OBJECT(NTetrahedron, tet));
}
void NTriangulation_i::removeAllTetrahedra() {
    MY_ENGINE_OBJECT->removeAllTetrahedra();
}
void NTriangulation_i::gluingsHaveChanged() {
    MY_ENGINE_OBJECT->gluingsHaveChanged();
}
CORBA::Long NTriangulation_i::getNumberOfComponents() {
    return MY_ENGINE_OBJECT->getNumberOfComponents();
}
Regina::Triangulation::NComponent_ptr NTriangulation_i::getComponent(
        CORBA::Long index) {
    return NComponent_i::newWrapper(MY_ENGINE_OBJECT->getComponents()[index]);
}
CORBA::Long NTriangulation_i::getComponentIndex(
        Regina::Triangulation::NComponent_ptr component) {
    return MY_ENGINE_OBJECT->getComponents().position(
        GET_ENGINE_OBJECT(NComponent, component));
}
CORBA::Long NTriangulation_i::getNumberOfBoundaryComponents() {
    return MY_ENGINE_OBJECT->getNumberOfBoundaryComponents();
}
Regina::Triangulation::NBoundaryComponent_ptr
        NTriangulation_i::getBoundaryComponent(CORBA::Long index) {
    return NBoundaryComponent_i::newWrapper(MY_ENGINE_OBJECT->
        getBoundaryComponents()[index]);
}
CORBA::Long NTriangulation_i::getBoundaryComponentIndex(
        Regina::Triangulation::NBoundaryComponent_ptr bc) {
    return MY_ENGINE_OBJECT->getBoundaryComponents().
        position(GET_ENGINE_OBJECT(NBoundaryComponent, bc));
}
CORBA::Long NTriangulation_i::getNumberOfVertices() {
    return MY_ENGINE_OBJECT->getNumberOfVertices();
}
Regina::Triangulation::NVertex_ptr NTriangulation_i::getVertex(
        CORBA::Long index) {
    return NVertex_i::newWrapper(MY_ENGINE_OBJECT->getVertices()[index]);
}
CORBA::Long NTriangulation_i::getVertexIndex(
        Regina::Triangulation::NVertex_ptr vertex) {
    return MY_ENGINE_OBJECT->getVertices().position(
        GET_ENGINE_OBJECT(NVertex, vertex));
}
CORBA::Long NTriangulation_i::getNumberOfEdges() {
    return MY_ENGINE_OBJECT->getNumberOfEdges();
}
Regina::Triangulation::NEdge_ptr NTriangulation_i::getEdge(
        CORBA::Long index) {
    return NEdge_i::newWrapper(MY_ENGINE_OBJECT->getEdges()[index]);
}
CORBA::Long NTriangulation_i::getEdgeIndex(
        Regina::Triangulation::NEdge_ptr edge) {
    return MY_ENGINE_OBJECT->getEdges().position(
        GET_ENGINE_OBJECT(NEdge, edge));
}
CORBA::Long NTriangulation_i::getNumberOfFaces() {
    return MY_ENGINE_OBJECT->getNumberOfFaces();
}
Regina::Triangulation::NFace_ptr NTriangulation_i::getFace(
        CORBA::Long index) {
    return NFace_i::newWrapper(MY_ENGINE_OBJECT->getFaces()[index]);
}
CORBA::Long NTriangulation_i::getFaceIndex(
        Regina::Triangulation::NFace_ptr face) {
    return MY_ENGINE_OBJECT->getFaces().position(
        GET_ENGINE_OBJECT(NFace, face));
}
CORBA::Long NTriangulation_i::getEulerCharacteristic() {
    return MY_ENGINE_OBJECT->getEulerCharacteristic();
}
Regina::Algebra::NGroupPresentation_ptr
        NTriangulation_i::getFundamentalGroup() {
    return NGroupPresentation_i::newWrapper(&(::NGroupPresentation&)
        (MY_ENGINE_OBJECT->getFundamentalGroup()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (MY_ENGINE_OBJECT->getHomologyH1()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1Rel() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (MY_ENGINE_OBJECT->getHomologyH1Rel()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH1Bdry() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (MY_ENGINE_OBJECT->getHomologyH1Bdry()));
}
Regina::Algebra::NAbelianGroup_ptr NTriangulation_i::getHomologyH2() {
    return NAbelianGroup_i::newWrapper(&(::NAbelianGroup&)
        (MY_ENGINE_OBJECT->getHomologyH2()));
}
CORBA::Long NTriangulation_i::getHomologyH2Z2() {
    return MY_ENGINE_OBJECT->getHomologyH2Z2();
}
CORBA::Boolean NTriangulation_i::isIdeal() {
    return MY_ENGINE_OBJECT->isIdeal();
}
CORBA::Boolean NTriangulation_i::isValid() {
    return MY_ENGINE_OBJECT->isValid();
}
CORBA::Boolean NTriangulation_i::isStandard() {
    return MY_ENGINE_OBJECT->isStandard();
}
CORBA::Boolean NTriangulation_i::isClosed() {
    return MY_ENGINE_OBJECT->isClosed();
}
CORBA::Boolean NTriangulation_i::isOrientable() {
    return MY_ENGINE_OBJECT->isOrientable();
}
CORBA::Boolean NTriangulation_i::isConnected() {
    return MY_ENGINE_OBJECT->isConnected();
}
CORBA::Boolean NTriangulation_i::hasBoundaryFaces() {
    return MY_ENGINE_OBJECT->hasBoundaryFaces();
}
CORBA::Boolean NTriangulation_i::isZeroEfficient() {
    return MY_ENGINE_OBJECT->isZeroEfficient();
}
CORBA::Boolean NTriangulation_i::hasCentralSurface() {
    return MY_ENGINE_OBJECT->hasCentralSurface();
}
CORBA::Boolean NTriangulation_i::crushMaximalForest() {
    return MY_ENGINE_OBJECT->crushMaximalForest();
}
CORBA::Boolean NTriangulation_i::intelligentSimplify() {
    return MY_ENGINE_OBJECT->intelligentSimplify();
}
CORBA::Boolean NTriangulation_i::simplifyToLocalMinimum(
        CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->simplifyToLocalMinimum(perform);
}
CORBA::Boolean NTriangulation_i::threeTwoMove(
        Regina::Triangulation::NEdge_ptr e,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->threeTwoMove(
        GET_ENGINE_OBJECT(NEdge, e), check, perform);
}
CORBA::Boolean NTriangulation_i::twoThreeMove(
        Regina::Triangulation::NFace_ptr f,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->twoThreeMove(
        GET_ENGINE_OBJECT(NFace, f), check, perform);
}
CORBA::Boolean NTriangulation_i::fourFourMove(
        Regina::Triangulation::NEdge_ptr e, CORBA::Long newAxis,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->fourFourMove(
        GET_ENGINE_OBJECT(NEdge, e), newAxis, check, perform);
}
CORBA::Boolean NTriangulation_i::twoZeroMove_NEdge(
        Regina::Triangulation::NEdge_ptr e,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->twoZeroMove(
        GET_ENGINE_OBJECT(NEdge, e), check, perform);
}
CORBA::Boolean NTriangulation_i::twoZeroMove_NVertex(
        Regina::Triangulation::NVertex_ptr v,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->twoZeroMove(
        GET_ENGINE_OBJECT(NVertex, v), check, perform);
}
CORBA::Boolean NTriangulation_i::twoOneMove(
        Regina::Triangulation::NEdge_ptr e, CORBA::Long edgeEnd,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->twoOneMove(
        GET_ENGINE_OBJECT(NEdge, e), edgeEnd, check, perform);
}
CORBA::Boolean NTriangulation_i::openBook(
        Regina::Triangulation::NFace_ptr f,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->openBook(
        GET_ENGINE_OBJECT(NFace, f), check, perform);
}
CORBA::Boolean NTriangulation_i::shellBoundary(
        Regina::Triangulation::NTetrahedron_ptr t,
        CORBA::Boolean check, CORBA::Boolean perform) {
    return MY_ENGINE_OBJECT->shellBoundary(
        GET_ENGINE_OBJECT(NTetrahedron, t), check, perform);
}
CORBA::Boolean NTriangulation_i::idealToFinite(CORBA::Boolean forceDivision) {
    return MY_ENGINE_OBJECT->idealToFinite(forceDivision);
}
void NTriangulation_i::barycentricSubdivision() {
    MY_ENGINE_OBJECT->barycentricSubdivision();
}
Regina::Triangulation::NTetrahedron_ptr
        NTriangulation_i::insertLayeredSolidTorus(
        CORBA::Long cuts0, CORBA::Long cuts1) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->
        insertLayeredSolidTorus(cuts0, cuts1));
}
void NTriangulation_i::insertLensSpace(CORBA::Long p, CORBA::Long q) {
    MY_ENGINE_OBJECT->insertLensSpace(p, q);
}
void NTriangulation_i::insertTriangulation(
        Regina::Triangulation::NTriangulation_ptr source) {
    MY_ENGINE_OBJECT->insertTriangulation(
        *GET_ENGINE_OBJECT(NTriangulation, source));
}
CORBA::Boolean NTriangulation_i::isIsomorphicTo(
        Regina::Triangulation::NTriangulation_ptr other) {
    return MY_ENGINE_OBJECT->isIsomorphicTo(
        *GET_ENGINE_OBJECT(NTriangulation, other));
}

