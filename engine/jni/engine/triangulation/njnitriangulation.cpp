
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include "triangulation/ntriangulation.h"
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "jnitools.h"
#include "engine/triangulation/NJNITriangulation.h"

using namespace regina;

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_addTetrahedron
        (JNIEnv *env, jobject me, jobject newTet) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->
        addTetrahedron(GET_ENGINE_OBJECT(env, NTetrahedron, newTet));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_barycentricSubdivision
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->barycentricSubdivision();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_crushMaximalForest
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->crushMaximalForest();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_fourFourMove
        (JNIEnv *env, jobject me, jobject e, jint newAxis, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->fourFourMove(
        GET_ENGINE_OBJECT(env, NEdge, e), newAxis, check, perform);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getBoundaryComponent
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getBoundaryComponents()[index],
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getBoundaryComponentIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getBoundaryComponents().
        index(GET_ENGINE_OBJECT(env, NBoundaryComponent, tet));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getComponent
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->getComponents()[index],
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getComponentIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getComponents().
        index(GET_ENGINE_OBJECT(env, NComponent, tet));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getEdge
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->getEdges()[index],
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getEdgeIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getEdges().
        index(GET_ENGINE_OBJECT(env, NEdge, tet));
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getEulerCharacteristic
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getEulerCharacteristic();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getFace
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->getFaces()[index],
        "normal/engine/implementation/jni/triangulation/NJNIFace");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getFaceIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getFaces().
        index(GET_ENGINE_OBJECT(env, NFace, tet));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getFundamentalGroup
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        (NGroupPresentation*)&GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getFundamentalGroup(),
        "normal/engine/implementation/jni/algebra/NJNIGroupPresentation");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getHomologyH1
        (JNIEnv *env, jobject me) {
    // (NAbelianGroup*) casting removes the const because
    //     CREATE_WRAPPER_OBJECT cannot take a const (since Java does not
    //     understand const).
    return CREATE_WRAPPER_OBJECT(env,
        (NAbelianGroup*)&GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getHomologyH1(),
        "normal/engine/implementation/jni/algebra/NJNIAbelianGroup");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getHomologyH1Bdry
        (JNIEnv *env, jobject me) {
    // (NAbelianGroup*) casting removes the const because
    //     CREATE_WRAPPER_OBJECT cannot take a const (since Java does not
    //     understand const).
    return CREATE_WRAPPER_OBJECT(env,
        (NAbelianGroup*)&GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getHomologyH1Bdry(),
        "normal/engine/implementation/jni/algebra/NJNIAbelianGroup");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getHomologyH1Rel
        (JNIEnv *env, jobject me) {
    // (NAbelianGroup*) casting removes the const because
    //     CREATE_WRAPPER_OBJECT cannot take a const (since Java does not
    //     understand const).
    return CREATE_WRAPPER_OBJECT(env,
        (NAbelianGroup*)&GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getHomologyH1Rel(),
        "normal/engine/implementation/jni/algebra/NJNIAbelianGroup");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getHomologyH2
        (JNIEnv *env, jobject me) {
    // (NAbelianGroup*) casting removes the const because
    //     CREATE_WRAPPER_OBJECT cannot take a const (since Java does not
    //     understand const).
    return CREATE_WRAPPER_OBJECT(env,
        (NAbelianGroup*)&GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getHomologyH2(),
        "normal/engine/implementation/jni/algebra/NJNIAbelianGroup");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getHomologyH2Z2
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getHomologyH2Z2();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfBoundaryComponents
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation,
        me)->getNumberOfBoundaryComponents();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfComponents
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getNumberOfComponents();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfEdges
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getNumberOfEdges();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfFaces
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getNumberOfFaces();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfTetrahedra
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getNumberOfTetrahedra();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getNumberOfVertices
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getNumberOfVertices();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getTetrahedron
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->getTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getTetrahedronIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->
        getTetrahedronIndex(GET_ENGINE_OBJECT(env, NTetrahedron, tet));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getVertex
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->getVertices()[index],
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_getVertexIndex
        (JNIEnv *env, jobject me, jobject tet) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->getVertices().
        index(GET_ENGINE_OBJECT(env, NVertex, tet));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_gluingsHaveChanged
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->gluingsHaveChanged();
}
  
JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_hasBoundaryFaces
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->hasBoundaryFaces();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_hasSplittingSurface
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->hasSplittingSurface();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_idealToFinite
        (JNIEnv *env, jobject me, jboolean force) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->idealToFinite(force);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_insertLayeredSolidTorus
        (JNIEnv *env, jobject me, jlong cuts0, jlong cuts1) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriangulation, me)->
        insertLayeredSolidTorus(cuts0, cuts1),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_insertLensSpace
        (JNIEnv *env, jobject me, jlong p, jlong q) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->insertLensSpace(p, q);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_insertRehydration
        (JNIEnv *env, jobject me, jstring str) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->insertRehydration(
        jstringToCString(env, str));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_insertSplittingSurface
        (JNIEnv *env, jobject me, jstring str) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->insertSplittingSurface(
        jstringToCString(env, str));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_insertTriangulation
        (JNIEnv *env, jobject me, jobject source) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->insertTriangulation(
        *(GET_ENGINE_OBJECT(env, NTriangulation, source)));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_intelligentSimplify
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->intelligentSimplify();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isClosed
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isClosed();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isConnected
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isConnected();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isIdeal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isIdeal();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isIsomorphicTo
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isIsomorphicTo(
        *GET_ENGINE_OBJECT(env, NTriangulation, you));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isOrientable();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isStandard
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isStandard();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isValid
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isValid();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_isZeroEfficient
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->isZeroEfficient();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_knowsSplittingSurface
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->knowsSplittingSurface();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_knowsZeroEfficient
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->knowsZeroEfficient();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_makeDoubleCover
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->makeDoubleCover();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_newNTriangulation__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NTriangulation(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_newNTriangulation__Lnormal_engine_triangulation_NTriangulation_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NTriangulation(
        *GET_ENGINE_OBJECT(env, NTriangulation, you)), me);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_openBook
        (JNIEnv *env, jobject me, jobject f, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->openBook(
        GET_ENGINE_OBJECT(env, NFace, f), check, perform);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_removeAllTetrahedra
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTriangulation, me)->removeAllTetrahedra();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_removeTetrahedron
        (JNIEnv *env, jobject me, jobject tet) {
    delete GET_ENGINE_OBJECT(env, NTriangulation, me)->
        removeTetrahedron(GET_ENGINE_OBJECT(env, NTetrahedron, tet));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_removeTetrahedronAt
        (JNIEnv *env, jobject me, jlong index) {
    delete GET_ENGINE_OBJECT(env, NTriangulation, me)->
        removeTetrahedronAt(index);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_shellBoundary
        (JNIEnv *env, jobject me, jobject t, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->shellBoundary(
        GET_ENGINE_OBJECT(env, NTetrahedron, t), check, perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_simplifyToLocalMinimum
        (JNIEnv *env, jobject me, jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->
        simplifyToLocalMinimum(perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_threeTwoMove
        (JNIEnv *env, jobject me, jobject e, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->threeTwoMove(
        GET_ENGINE_OBJECT(env, NEdge, e), check, perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_twoOneMove
        (JNIEnv *env, jobject me, jobject e, jint edgeEnd, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->twoOneMove(
        GET_ENGINE_OBJECT(env, NEdge, e), edgeEnd, check, perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_twoThreeMove
        (JNIEnv *env, jobject me, jobject f, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->twoThreeMove(
        GET_ENGINE_OBJECT(env, NFace, f), check, perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_twoZeroMove__Lnormal_engine_triangulation_NEdge_2ZZ
        (JNIEnv *env, jobject me, jobject e, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->twoZeroMove(
        GET_ENGINE_OBJECT(env, NEdge, e), check, perform);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITriangulation_twoZeroMove__Lnormal_engine_triangulation_NVertex_2ZZ
        (JNIEnv *env, jobject me, jobject v, jboolean check,
        jboolean perform) {
    return GET_ENGINE_OBJECT(env, NTriangulation, me)->twoZeroMove(
        GET_ENGINE_OBJECT(env, NVertex, v), check, perform);
}

