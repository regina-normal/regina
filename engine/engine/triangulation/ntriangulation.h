
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

/*! \file ntriangulation.h
 *  \brief Deals with triangulations.
 */

#ifndef __NTRIANGULATION_H
#ifndef __DOXYGEN
#define __NTRIANGULATION_H
#endif

#include "packet/npacket.h"
#include "property/npropertyholder.h"
#include "utilities/ndynamicarray.h"
#include "utilities/nset.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/nface.h"
#include "triangulation/nedge.h"
#include "triangulation/nvertex.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nboundarycomponent.h"
#include "algebra/nabeliangroup.h"

class NGroupPresentation;

/**
 * Stores the triangulation of a 3-manifold along with its
 * various cellular structures and other information.
 *
 * When the triangulation is deleted, the corresponding
 * tetrahedra, the cellular structure and all other properties
 * will be deallocated.
 * 
 * Faces, edges, vertices and components are always temporary;
 * whenever a change
 * occurs with the triangulation, these will be deleted and a new
 * skeletal structure will be calculated.  The same is true of various
 * other triangulation properties.
 *
 * Whenever the gluings of tetrahedra have been altered, the routine
 * responsible for changing the gluings \b must call
 * NTriangulation::gluingsHaveChanged() to ensure that relevant
 * properties will be recalculated when necessary.  It is not necessary
 * to call this function when adding or removing tetrahedra.
 *
 * \todo \featureurgent Am I obviously a handlebody?  (Simplify and see
 * if there is nothing left).  Am I obviously not a handlebody?
 * (Compare homology with boundary homology).
 * \todo \feature Is the boundary incompressible?
 * \todo \feature Add set of cusps and three corresponding get functions.
 * \todo \feature Make 0-efficient.
 * \todo \feature Insert a SFS with 3 exceptional fibres.
 * \todo \feature Insert a layered loop of index n.
 * \todo \featurelong Am I a sphere?
 * \todo \featurelong Is the triangulation Haken?
 * \todo \featurelong What is the Heegaard genus?
 * \todo \featurelong Have a subcomplex as a child packet of a
 * triangulation.  Include routines to crush a subcomplex or to expand a
 * subcomplex to a normal surface.
 * \todo \featurelong Implement writeTextLong() for skeletal objects.
 * \todo \featurelong Random triangulation with <i>n</i> tetrahedra.
 * \todo \optlong Make a <i>pointer</i>-><i>index</i> lookup class using a
 * hash table and use this to speed up routines that use
 * NDynamicArray::position().
 */
class NTriangulation : public NPacket, NPropertyHolder {
    public:
        static const int packetType;

        typedef NDynamicArrayIterator<NTetrahedron*> TetrahedronIterator;
            /**< Used to iterate through tetrahedra. */
        typedef NDynamicArrayIterator<NFace*> FaceIterator;
            /**< Used to iterate through faces. */
        typedef NDynamicArrayIterator<NEdge*> EdgeIterator;
            /**< Used to iterate through edges. */
        typedef NDynamicArrayIterator<NVertex*> VertexIterator;
            /**< Used to iterate through vertices. */
        typedef NDynamicArrayIterator<NComponent*> ComponentIterator;
            /**< Used to iterate through components. */
        typedef NDynamicArrayIterator<NBoundaryComponent*>
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    private:
        bool calculatedSkeleton;
            /**< Has the skeleton been calculated? */
            
        NDynamicArray<NTetrahedron*> tetrahedra;
            /**< The tetrahedra that form the triangulation. */
        NDynamicArray<NFace*> faces;
            /**< The faces in the triangulation skeleton. */
        NDynamicArray<NEdge*> edges;
            /**< The edges in the triangulation skeleton. */
        NDynamicArray<NVertex*> vertices;
            /**< The vertices in the triangulation skeleton. */
        NDynamicArray<NComponent*> components;
            /**< The components that form the triangulation. */
        NDynamicArray<NBoundaryComponent*> boundaryComponents;
            /**< The components that form the boundary of the
                 triangulation. */
        
        bool valid;
            /**< Is the triangulation valid? */
        bool ideal;
            /**< Is the triangulation ideal? */
        bool standard;
            /**< Is the triangulation standard? */
        bool orientable;
            /**< Is the triangulation orientable? */

        NGroupPresentation* fundamentalGroup;
            /**< Fundamental group of the triangulation. */
        bool calculatedFundamentalGroup;
            /**< Has \a fundamentalGroup been calculated? */
        NAbelianGroup* H1;
            /**< First homology group of the triangulation. */
        bool calculatedH1;
            /**< Has \a H1 been calculated? */
        NAbelianGroup* H1Rel;
            /**< Relative first homology group of the triangulation
             *   with respect to the boundary. */
        bool calculatedH1Rel;
            /**< Has \a H1Rel been calculated? */
        NAbelianGroup* H1Bdry;
            /**< First homology group of the boundary. */
        bool calculatedH1Bdry;
            /**< Has \a H1Bdry been calculated? */
        NAbelianGroup* H2;
            /**< Second homology group of the triangulation. */
        bool calculatedH2;
            /**< Has \a H2 been calculated? */

        bool zeroEfficient;
            /**< Is the triangulation zero-efficient? */
        bool calculatedZeroEfficient;
            /**< Has zero-efficiency been calculated? */
        bool splittingSurface;
            /**< Does the triangulation have a normal splitting surface? */
        bool calculatedSplittingSurface;
            /**< Has the existence of a splitting surface been calculated? */

    public:
        /**
         * Default constructor.
         * Creates an empty triangulation.
         */
        NTriangulation();
        /**
         * Copy constructor.
         * Creates a new triangulation identical to the given
         * triangulation.
         * The packet tree structure and packet label are \e not
         * copied.
         *
         * @param cloneMe the triangulation to clone.
         */
        NTriangulation(const NTriangulation& cloneMe);
        /**
         * Destroys this triangulation.
         * The contained tetrahedra, the cellular
         * structure and all other properties will also be deallocated.
         */
        virtual ~NTriangulation();
        
        virtual int getPacketType() const;
        virtual NString getPacketName() const;
        
        virtual void writePacket(NFile& out) const;
        static NTriangulation* readPacket(NFile& in, NPacket* parent);
        virtual void writeTextShort(ostream& out) const;
        virtual void writeTextLong(ostream& out) const;
        virtual bool dependsOnParent() const;
        virtual void readIndividualProperty(NFile& infile, unsigned propType);

        /**
         * Allows the user to interactively enter a triangulation in
         * plain text.  Prompts will be sent to the given output stream
         * and information will be read from the given input stream.
         *
         * \ifaces This routine is a member of class Engine.
         * It takes no parameters; \a in and \a out are always assumed
         * to be standard input and standard output respectively.
         * If the engine is being accessed through CORBA, this routine
         * returns <tt>null</tt>.
         * \ifacescorba Not present.
         *
         * @param in the input stream from which text will be read.
         * @param out the output stream to which prompts will be
         * written.
         * @return the triangulation entered in by the user.
         */
        static NTriangulation* enterTextTriangulation(istream& in,
                ostream& out);

        /**
         * Returns the Euler characteristic of this triangulation.
         *
         * This will be evaluated strictly as
         * <i>V</i>-<i>E</i>+<i>F</i>-<i>T</i>.  Thus if the manifold
         * contains cusps, the Euler characteristic will almost
         * certainly not be the same as the corresponding 3-manifold
         * with the cusps truncated.
         *
         * @return the Euler characteristic.
         */
        long getEulerCharacteristic();

        /**
         * Returns the fundamental group of this triangulation.
         * If this triangulation contains any ideal or non-standard
         * vertices, the fundamental group will be
         * calculated as if each such vertex had been truncated.
         *
         * If this triangulation contains any invalid edges, the
         * calculations will be performed <b>without</b> any truncation
         * of the corresponding projective plane cusp.  Thus if a
         * barycentric subdivision is performed on the triangulation, the
         * result of getFundamentalGroup() will change.
         *
         * Bear in mind that each time the triangulation changes, the
         * fundamental group will be deleted.
         * Thus the group reference returned should not be kept for
         * later use.  Instead, getFundamentalGroup() should be called again;
         * this will be instantaneous if the group has already been
         * calculated.
         *
         * Note that this triangulation is not required to be valid
         * (see isValid()).
         *
         * \pre This triangulation has at most one component.
         *
         * @return the fundamental group.
         */
        const NGroupPresentation& getFundamentalGroup();
        /**
         * Returns the first homology group for this triangulation.
         * If this triangulation contains any ideal or non-standard
         * vertices, the homology group will be
         * calculated as if each such vertex had been truncated.
         *
         * If this triangulation contains any invalid edges, the
         * calculations will be performed <b>without</b> any truncation
         * of the corresponding projective plane cusp.  Thus if a
         * barycentric subdivision is performed on the triangulation, the
         * result of getHomologyH1() will change.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.
         * Thus the group reference returned should not be kept for
         * later use.  Instead, getHomologyH1() should be called again;
         * this will be instantaneous if the group has already been
         * calculated.
         *
         * Note that this triangulation is not required to be valid
         * (see isValid()).
         *
         * @return the first homology group.
         */
        const NAbelianGroup& getHomologyH1();
        /**
         * Returns the relative first homology group with
         * respect to the boundary for this triangulation.
         * Note that ideal vertices are considered part of the boundary.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.
         * Thus the group reference returned should not be kept for
         * later use.  Instead, getHomologyH1Rel() should be called again;
         * this will be instantaneous if the group has already been
         * calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the relative first homology group with respect to the
         * boundary.
         */
        const NAbelianGroup& getHomologyH1Rel();
        /**
         * Returns the first homology group of the
         * boundary for this triangulation.
         * Note that ideal vertices are considered part of the boundary.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.
         * Thus the group reference returned should not be kept for
         * later use.  Instead, getHomologyH1Bdry() should be called again;
         * this will be instantaneous if the group has already been
         * calculated.
         *
         * This routine is fairly fast, since it deduces the homology of
         * each boundary component through knowing what kind of surface
         * it is.
         *
         * \pre This triangulation is valid.
         *
         * @return the first homology group of the boundary.
         */
        const NAbelianGroup& getHomologyH1Bdry();
        /**
         * Returns the second homology group for this triangulation.
         * If this triangulation contains any ideal vertices,
         * the homology group will be
         * calculated as if each such vertex had been truncated.
         * The algorithm used calculates various first homology groups
         * and uses homology and cohomology theorems to deduce the
         * second homology group.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.
         * Thus the group reference returned should not be kept for
         * later use.  Instead, getHomologyH2() should be called again;
         * this will be instantaneous if the group has already been
         * calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the second homology group.
         */
        const NAbelianGroup& getHomologyH2();
        /**
         * Returns the second homology group with coefficients in Z_2
         * for this triangulation.
         * If this triangulation contains any ideal vertices,
         * the homology group will be
         * calculated as if each such vertex had been truncated.
         * The algorithm used calculates the relative first homology group
         * with respect to the boundary and uses homology and cohomology
         * theorems to deduce the second homology group.
         *
         * This group will simply be the direct sum of several copies of
         * Z_2, so the number of Z_2 terms is returned.
         *
         * \pre This triangulation is valid.
         *
         * @return the number of Z_2 terms in the second homology group
         * with coefficients in Z_2.
         */
        unsigned long getHomologyH2Z2();

        /**
         * Returns the number of tetrahedra in the triangulation.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;
        /**
         * Returns all tetrahedra in the triangulation.
         *
         * The reference returned will remain valid
         * for as long as the triangulation exists,
         * always reflecting the tetrahedra currently in the
         * triangulation.
         *
         * \ifaces Not present.
         *
         * @return the list of all tetrahedra.
         */
        const NDynamicArray<NTetrahedron*>& getTetrahedra() const;
        /**
         * Returns the tetrahedron with the given index number in the
         * triangulation.
         * Note that tetrahedron indexing may change when a tetrahedron
         * is added or removed from the triangulation.
         *
         * This routine will ensure the skeleton is calculated, since
         * other skeleton objects can be accessed from NTetrahedron.
         * 
         * @param index specifies which tetrahedron to return; this
         * value should be between 0 and getNumberOfTetrahedra()-1
         * inclusive.
         * @return the <tt>index</tt>th tetrahedron in the
         * triangulation.
         */
        NTetrahedron* getTetrahedron(unsigned long index);
        /**
         * Returns the index of the given tetrahedron in the
         * triangulation.
         *
         * Note that tetrahedron indexing may change when a tetrahedron
         * is added or removed from the triangulation.
         *
         * @param tet specifies which tetrahedron to find in the
         * triangulation.
         * @return the index of the specified tetrahedron, where 0 is
         * the first tetrahedron, 1 is the second and so on.  If the
         * tetrahedron is not contained in the triangulation, a negative
         * number is returned.
         */
        unsigned long getTetrahedronIndex(const NTetrahedron* tet) const;
        /**
         * Inserts the given tetrahedron into the triangulation.
         * No face gluings anywhere will be examined or altered.
         *
         * The new tetrahedron will be assigned a higher index in the
         * triangulation than all tetrahedra already present.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * @param tet the tetrahedron to insert.
         */
        void addTetrahedron(NTetrahedron* tet);
        /**
         * Removes the given tetrahedron from the triangulation.
         * All faces glued to this tetrahedron will be unglued.
         * The tetrahedron will \e not be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * \pre The given tetrahedron exists in the triangulation.
         *
         * \ifaces This routine returns nothing.  Furthermore,
         * the tetrahedron is not only removed but is also destroyed in
         * the internal C++ engine.
         *
         * @param tet the tetrahedron to remove.
         * @return the removed tetrahedron.
         */
        NTetrahedron* removeTetrahedron(NTetrahedron* tet);
        /**
         * Removes the tetrahedron with the given index number
         * from the triangulation.  Note that tetrahedron indexing may
         * change when a tetrahedron is added or removed from the
         * triangulation.
         *
         * All faces glued to this tetrahedron will be unglued.
         * The tetrahedron will \e not be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * \ifaces This routine returns nothing.  Furthermore,
         * the tetrahedron is not only removed but is also destroyed in
         * the internal C++ engine.
         *
         * @param index specifies which tetrahedron to remove; this
         * should be between 0 and getNumberOfTetrahedra()-1 inclusive.
         * @return the removed tetrahedron.
         */
        NTetrahedron* removeTetrahedronAt(unsigned long index);
        /**
         * Removes all tetrahedra from the triangulation.
         * All tetrahedra will be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         */
        void removeAllTetrahedra();
        /**
         * This \b must be called whenever the gluings of tetrahedra are
         * changed! 
         * Clears appropriate properties and performs other
         * necessary tasks.
         * The responsibility of calling gluingsHaveChanged()
         * falls upon the routine that alters the gluings (such as a
         * component of a triangulation editor, or so on).
         */
        void gluingsHaveChanged();
                
        /**
         * Returns the number of boundary components in this
         * triangulation.  Note that each ideal vertex forms its own
         * boundary component.
         *
         * @return the number of boundary components.
         */
        unsigned long getNumberOfBoundaryComponents();
        /**
         * Returns the number of components in this triangulation.
         *
         * @return the number of components.
         */
        unsigned long getNumberOfComponents();
        /**
         * Returns the number of vertices in this triangulation.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices();
        /**
         * Returns the number of edges in this triangulation.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges();
        /**
         * Returns the number of faces in this triangulation.
         *
         * @return the number of faces.
         */
        unsigned long getNumberOfFaces();

        /**
         * Returns all components of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * components will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifaces Not present.
         *
         * @return the list of all components.
         */
        const NDynamicArray<NComponent*>& getComponents();
        /**
         * Returns all boundary components of this triangulation.
         * Note that each ideal vertex forms its own boundary component.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifaces Not present.
         *
         * @return the list of all boundary components.
         */
        const NDynamicArray<NBoundaryComponent*>& getBoundaryComponents();
        /**
         * Returns all vertices of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifaces Not present.
         *
         * @return the list of all vertices.
         */
        const NDynamicArray<NVertex*>& getVertices();
        /**
         * Returns all edges of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifaces Not present.
         *
         * @return the list of all edges.
         */
        const NDynamicArray<NEdge*>& getEdges();
        /**
         * Returns all faces of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * faces will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifaces Not present.
         *
         * @return the list of all faces.
         */
        const NDynamicArray<NFace*>& getFaces();
        /**
         * Returns the requested triangulation component.
         *
         * Bear in mind that each time the triangulation changes, the
         * components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * \ifacescpp Not present.
         *
         * @param index the index of the desired component, ranging from 0
         * to getNumberOfComponents()-1 inclusive.
         * @return the requested component.
         */
        #ifdef __DOXYGEN
        NComponent* getComponent(unsigned long index);
        #endif
        /**
         * Returns the requested triangulation boundary component.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * \ifacescpp Not present.
         *
         * @param index the index of the desired boundary
         * component, ranging from 0
         * to getNumberOfBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        #ifdef __DOXYGEN
        NBoundaryComponent* getBoundaryComponent(unsigned long index);
        #endif
        /**
         * Returns the requested triangulation vertex.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * \ifacescpp Not present.
         *
         * @param index the index of the desired vertex, ranging from 0
         * to getNumberOfVertices()-1 inclusive.
         * @return the requested vertex.
         */
        #ifdef __DOXYGEN
        NVertex* getVertex(unsigned long index);
        #endif
        /**
         * Returns the requested triangulation edge.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * \ifacescpp Not present.
         *
         * @param index the index of the desired edge, ranging from 0
         * to getNumberOfEdges()-1 inclusive.
         * @return the requested edge.
         */
        #ifdef __DOXYGEN
        NEdge* getEdge(unsigned long index);
        #endif
        /**
         * Returns the requested triangulation face.
         *
         * Bear in mind that each time the triangulation changes, the
         * faces will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * \ifacescpp Not present.
         *
         * @param index the index of the desired face, ranging from 0
         * to getNumberOfFaces()-1 inclusive.
         * @return the requested face.
         */
        #ifdef __DOXYGEN
        NFace* getFace(unsigned long index);
        #endif
        /**
         * Returns the index of the given component in the triangulation.
         *
         * \pre The given component belongs to this triangulation.
         *
         * \ifacescpp Not present.
         *
         * @param component specifies which component to find in the
         * triangulation.
         * @return the index of the specified component, where 0 is the first
         * component, 1 is the second and so on.
         */
        #ifdef __DOXYGEN
        unsigned long getComponentIndex(const NComponent* component);
        #endif
        /**
         * Returns the index of the given boundary component
         * in the triangulation.
         *
         * \pre The given boundary component belongs to this triangulation.
         *
         * \ifacescpp Not present.
         *
         * @param bc specifies which boundary component to find in the
         * triangulation.
         * @return the index of the specified boundary component,
         * where 0 is the first boundary component,
         * 1 is the second and so on.
         */
        #ifdef __DOXYGEN
        unsigned long getBoundaryComponentIndex(const NBoundaryComponent* bc);
        #endif
        /**
         * Returns the index of the given vertex in the triangulation.
         *
         * \pre The given vertex belongs to this triangulation.
         *
         * \ifacescpp Not present.
         *
         * @param vertex specifies which vertex to find in the
         * triangulation.
         * @return the index of the specified vertex, where 0 is the first
         * vertex, 1 is the second and so on.
         */
        #ifdef __DOXYGEN
        unsigned long getVertexIndex(const NVertex* vertex);
        #endif
        /**
         * Returns the index of the given edge in the triangulation.
         *
         * \pre The given edge belongs to this triangulation.
         *
         * \ifacescpp Not present.
         *
         * @param edge specifies which edge to find in the
         * triangulation.
         * @return the index of the specified edge, where 0 is the first
         * edge, 1 is the second and so on.
         */
        #ifdef __DOXYGEN
        unsigned long getEdgeIndex(const NEdge* edge);
        #endif
        /**
         * Returns the index of the given face in the triangulation.
         *
         * \pre The given face belongs to this triangulation.
         *
         * \ifacescpp Not present.
         *
         * @param face specifies which face to find in the
         * triangulation.
         * @return the index of the specified face, where 0 is the first
         * face, 1 is the second and so on.
         */
        #ifdef __DOXYGEN
        unsigned long getFaceIndex(const NFace* face);
        #endif

        /**
         * Determines if this triangulation is valid.
         * A triangulation is valid unless there is some vertex whose
         * link has boundary but is not a disc (i.e., a vertex for which
         * NVertex::getLink() returns NVertex::NON_STANDARD_BDRY),
         * or unless there is some edge glued to itself in reverse
         * (i.e., an edge for which NEdge::isValid() returns \c false).
         *
         * @return \c true if and only if this triangulation is valid.
         */
        bool isValid();
        /**
         * Determines if this triangulation is ideal.
         * This is the case if and only if one of the vertex links
         * is closed and not a 2-sphere.
         * Note that the triangulation is not required to be valid.
         *
         * @return \c true if and only if this triangulation is ideal.
         */
        bool isIdeal();
        /**
         * Determines if this triangulation is standard.
         * This is the case if and only if every vertex is standard.
         * See NVertex::isStandard() for further details.
         *
         * @return \c true if and only if this triangulation is
         * standard.
         */
        bool isStandard();
        /**
         * Determines if this triangulation has any boundary faces.
         *
         * @return \c true if and only if there are boundary faces.
         */
        bool hasBoundaryFaces();
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal triangulations are not closed.
         *
         * @return \c true if and only if this triangulation is closed.
         */
        bool isClosed();
        /**
         * Determines if this triangulation is orientable.
         *
         * @return \c true if and only if this triangulation is
         * orientable.
         */
        bool isOrientable();
        /**
         * Determines if this triangulation is connected.
         *
         * @return \c true if and only if this triangulation is
         * connected.
         */
        bool isConnected();

        /**
         * Determines if this triangulation is 0-efficient.
         * A triangulation is 0-efficient if its only normal spheres and
         * discs are vertex linking, and if it has no 2-sphere boundary
         * components.
         *
         * @return \c true if and only if this triangulation is
         * 0-efficient.
         */
        bool isZeroEfficient();
        /**
         * Is it already known whether or not this triangulation is
         * 0-efficient?
         * See isZeroEfficient() for further details.
         *
         * If this property is already known, future calls to
         * isZeroEfficient() will be very fast (simply returning the
         * precalculated value).
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsZeroEfficient() const;
        /**
         * Determines whether this triangulation has a normal splitting
         * surface.  See NNormalSurface::isSplitting() for details
         * regarding normal splitting surfaces.
         *
         * \pre This triangulation is connected.  If the triangulation
         * is not connected, this routine will still return a result but
         * that result will be unreliable.
         *
         * @return \c true if and only if this triangulation has a
         * normal splitting surface.
         */
        bool hasSplittingSurface();
        /**
         * Is it already known whether or not this triangulation has a
         * splitting surface?
         * See hasSplittingSurface() for further details.
         *
         * If this property is already known, future calls to
         * hasSplittingSurface() will be very fast (simply returning the
         * precalculated value).
         *
         * @return \c true if and only if this property is already known.
         */
        bool knowsSplittingSurface() const;
        
        /**
         * Produces a maximal forest in the 1-skeleton of the
         * triangulation boundary.
         * Both given sets will be emptied and the edges and vertices of
         * the maximal forest will be placed into them.
         * A vertex that forms its own boundary component (such as an
         * ideal vertex) will still be placed in \c vertexSet.
         *
         * Note that the edge and vertex pointers returned will become
         * invalid once the triangulation has changed.
         *
         * \ifaces Not present.
         *
         * @param edgeSet the set to be emptied and into which the edges
         * of the maximal forest will be placed.
         * @param vertexSet the set to be emptied and into which the
         * vertices of the maximal forest will be placed.
         */
        void maximalForestInBoundary(NPointerSet<NEdge>& edgeSet,
                NPointerSet<NVertex>& vertexSet);
        /**
         * Produces a maximal forest in the triangulation's 1-skeleton.
         * The given set will be emptied and will have the edges of the
         * maximal forest placed into it.  It can be specified whether
         * or not different boundary components may be joined by the
         * maximal forest.
         *
         * An edge leading to an ideal vertex is still a
         * candidate for inclusion in the maximal forest.  For the
         * purposes of this algorithm, any ideal vertex will be treated
         * as any other vertex (and will still be considered part of its
         * own boundary component).
         *
         * Note that the edge pointers returned will become
         * invalid once the triangulation has changed.
         *
         * \ifaces Not present.
         *
         * @param edgeSet the set to be emptied and into which the edges
         * of the maximal forest will be placed.
         * @param canJoinBoundaries \c true if and only if different
         * boundary components are allowed to be joined by the maximal
         * forest.
         */
        void maximalForestInSkeleton(NPointerSet<NEdge>& edgeSet,
                bool canJoinBoundaries = true);
        /**
         * Produces a maximal forest in the triangulation's dual
         * 1-skeleton.  The given set will be emptied and will have the
         * faces corresponding to the edges of the maximal forest in the
         * dual 1-skeleton placed into it.
         *
         * Note that the face pointers returned will become invalid once
         * the triangulation has changed.
         *
         * \ifaces Not present.
         *
         * @param faceSet the set to be emptied and into which the faces
         * representing the maximal forest will be placed.
         */
        void maximalForestInDualSkeleton(NPointerSet<NFace>& faceSet);
        /**
         * Attempts to reduce the number of vertices by crushing a
         * maximal forest in the 1-skeleton.
         *
         * \todo \bugurgent This algorithm needs to be changed from the
         * current incorrect algorithm to Dave's algorithm that avoids
         * crisis by using 2-3 moves.
         *
         * @return \c true if and only if the triangulation was changed.
         */
        bool crushMaximalForest();

        /**
         * Attempts to simplify the triangulation as intelligently as
         * possible without further input.
         *
         * Currently this routine does nothing but call
         * simplifyToLocalMinimum(); once a local minimum is reached
         * it will stay there.
         *
         * \warning The specific behaviour of this routine is
         * very likely to change between releases.
         *
         * \todo \opturgent Make this faster and more effective.
         * Include random 4-4 moves and random 2-3 moves to get out of
         * wells.  Unglue faces with three boundary edges and record the
         * corresponding change in topology.  Minimise the amount of
         * skeletal/homological calculation.
         *
         * @return \c true if and only if the triangulation was changed.
         */
        bool intelligentSimplify();
        /**
         * Uses all known simplification moves to reduce the triangulation
         * monotonically to some local minimum number of tetrahedra.
         * Note that this will probably not give a globally minimal
         * triangulation; see intelligentSimplify() for further
         * assistance in achieving this goal.
         *
         * The moves used include crushing maximal forests in the
         * triangulation 1-skeleton and using 3-2, 2-0 (edge and vertex),
         * 2-1, boundary shelling and book opening moves as far as
         * possible.
         *
         * Note that if a book opening move is possible, the
         * triangulation will \b not be considered a local minimum, even
         * though the book opening move alone will not reduce the number of
         * tetrahedra.
         *
         * \warning The specific behaviour of this routine is
         * very likely to change between releases.
         *
         * \ifacescorba All parameters are compulsory.
         *
         * \todo \bugurgent This routine currently does not crush a
         * maximal forest!
         *
         * @param perform \c true if we are to perform the
         * simplifications, or \c false if we are only to investigate
         * whether simplifications are possible (defaults to \c true).
         * @return if \a perform is \c true, this routine returns
         * \c true if and only if the triangulation was changed; if
         * \a perform is \c false, this routine returns \c true if and
         * only if any simplifications are possible.
         */
        bool simplifyToLocalMinimum(bool perform = true);
        
        /**
         * Checks the eligibility of and/or performs a 3-2 move
         * about the given edge.
         * This involves replacing the three tetrahedra joined at that
         * edge with two tetrahedra joined by a face.
         * This can be done iff the edge is non-boundary and the three
         * tetrahedra are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool threeTwoMove(NEdge* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-3 move
         * about the given face.
         * This involves replacing the two tetrahedra joined at that
         * face with three tetrahedra joined by an edge.
         * This can be done iff the two tetrahedra are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param f the face about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoThreeMove(NFace* f, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 4-4 move
         * about the given edge.
         * This involves replacing the four tetrahedra joined at that
         * edge with four tetrahedra joined along a different edge.
         * Consider the octahedron made up of the four original
         * tetrahedra; this has three internal axes.  The initial four
         * tetrahedra meet along the given edge which forms one of these
         * axes; the new tetrahedra will meet along a different axis.
         * This move can be done iff the edge is non-boundary and the four
         * tetrahedra are distinct.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param e the edge about which to perform the move.
         * @param newAxis Specifies which axis of the octahedron the new
         * tetrahedra should meet along; this should be 0 or 1.
         * Consider the four original tetrahedra in the order described
         * by NEdge::getEmbeddings(); call these tetrahedra 0, 1, 2 and
         * 3.  If \a newAxis is 0, the new axis will separate tetrahedra
         * 0 and 1 from 2 and 3.  If \a newAxis is 1, the new axis will
         * separate tetrahedra 1 and 2 from 3 and 0.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool fourFourMove(NEdge* e, int newAxis, bool check = true,
                bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-0 move
         * about the given edge of degree 2.
         * This involves taking the two tetrahedra joined at that edge
         * and squashing them flat.
         * This can be done only if the edge is non-boundary, the two
         * tetrahedra are distinct and the edges opposite \c e in each
         * tetrahedron are distinct and not both boundary.  Furthermore,
         * if faces \e f1 and \e f2 of one tetrahedron are to be
         * flattened onto faces \e g1 and \e g2 of the other
         * respectively, we must
         * have (a) \e f1 and \e g1 distinct, (b) \e f2 and \e g2 distinct,
         * (c) not both <i>f1</i>=<i>g2</i> and <i>g1</i>=<i>f2</i>,
         * (d) not both <i>f1</i>=<i>f2</i> and <i>g1</i>=<i>g2</i> and
         * (e) not two of the faces boundary with the other two identified.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param e the edge about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(NEdge* e, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-0 move
         * about the given vertex of degree 2.
         * This involves taking the two tetrahedra joined at that vertex
         * and squashing them flat.
         * This can be done only if the vertex is non-boundary, the two
         * tetrahedra are distinct, the
         * faces opposite \c v in each tetrahedron are distinct and not
         * both boundary, and the two tetrahedra meet each other
         * on all three faces touching the vertex (as opposed to meeting
         * each other on one face and being glued to themselves along the
         * other two).
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param v the vertex about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoZeroMove(NVertex* v, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a 2-1 move
         * about the given edge.
         * This involves taking an edge meeting only one tetrahedron
         * just once and merging that tetrahedron with one of the
         * tetrahedra joining it.
         *
         * This can be done assuming the following conditions.  The edge
         * must be non-boundary.  The two vertices that are its endpoints
         * cannot both be boundary.  The two remaining faces of the
         * tetrahedron may not be joined.  Furthermore, consider the two
         * edges of the second tetrahedron (to be merged) that run from
         * the (identical) vertices of the original tetrahedron not
         * touching \c e to the vertex of the second tetrahedron not
         * touching the original tetrahedron.  These edges must be
         * distinct and may not both be in the boundary.  Finally (which
         * should follow from the previous conditions), the two faces
         * joining these two edges to the vertex of \c e that is common to
         * both tetrahedra should be distinct.  Phew.  Code documentation
         * could really do with diagrams!
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param e the edge about which to perform the move.
         * @param edgeEnd the end of the edge \e opposite that at
         * which the second tetrahedron (to be merged) is joined.
         * The end is 0 or 1, corresponding to the labelling (0,1) of
         * the vertices of the edge as described in
         * NEdgeEmbedding::getVertices().
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool twoOneMove(NEdge* e, int edgeEnd,
                bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a book opening move
         * about the given face.
         * This involves taking a face meeting the boundary along two
         * edges and ungluing it to create two new boundary faces and
         * thus expose the tetrahedra it initially joined, allowing for
         * potential boundary shelling moves.
         * This move can be done only if the face meets the boundary in
         * precisely two edges (and thus also joins two tetrahedra) and
         * if the vertex between these two edges is a standard boundary
         * vertex (its link is a disc).
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param f the face about which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool openBook(NFace* f, bool check = true, bool perform = true);
        /**
         * Checks the eligibility of and/or performs a boundary shelling
         * move on the given tetrahedron.
         * This involves simply popping off a tetrahedron that touches
         * the boundary.
         * This can be done only if precisely 1, 2 or 3 faces of the
         * tetrahedron lie in the boundary.
         * Furthermore, if 1 face lies in the boundary, the opposite
         * vertex may not lie in the boundary.  If 2 faces lie in the
         * boundary, the remaining edge may not lie in the boundary and
         * the remaining two faces of the tetrahedron may not be glued
         * together.
         *
         * If the routine is asked to both check and perform, the move
         * will only be performed if the check shows it is legal.
         *
         * Note that after performing this move, all skeletal objects
         * (faces, components, etc.) will be invalid.
         *
         * \pre If the move is being performed and no
         * check is being run, it must be known in advance that the move
         * is legal.
         * \pre The skeleton has been calculated.
         * Skeleton calculation can be forced by querying the skeleton,
         * such as calling getNumberOfVertices().
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param t the tetrahedron upon which to perform the move.
         * @param check \c true if we are to check whether the move is
         * allowed (defaults to \c true).
         * @param perform \c true if we are to perform the move
         * (defaults to \c true).
         * @return If \a check is \c true, the function returns \c true
         * if and only if the requested move may be performed
         * without changing the topology of the manifold.  If \a check
         * is \c false, the function simply returns \c true.
         */
        bool shellBoundary(NTetrahedron* t,
                bool check = true, bool perform = true);

        /**
         * Converts an ideal triangulation into a finite triangulation.
         * All ideal or non-standard vertices are truncated and thus
         * converted into
         * real boundary components made from unglued faces of tetrahedra.
         *
         * \warning Currently, this routine subdivides all tetrahedra as
         * if <i>all</i> vertices (not just some) were ideal.
         * This may lead to more tetrahedra than are necessary.
         *
         * \warning Currently, the presence of an invalid edge will force
         * the triangulation to be subdivided regardless of the value of
         * parameter \a forceDivision.  The final triangulation will
         * still have the projective plane cusp caused by the invalid
         * edge.
         *
         * \todo \optlong Have this routine only use as many tetrahedra
         * as are necessary, leaving finite vertices alone.
         *
         * \ifacescorba All parameters are compulsory.
         *
         * @param forceDivision specifies what to do if the triangulation
         * has no ideal or non-standard vertices.
         * If \c true, the triangulation will be
         * subdivided anyway, as if all vertices were ideal.  If
         * \c false (the default), the triangulation will be left alone.
         *
         * @return \c true if and only if the triangulation was changed.
         * @author David Letscher
         */
        bool idealToFinite(bool forceDivision = false);
        /**
         * Does a barycentric subdivision of the triangulation.
         * Each tetrahedron is divided into four tetrahedra by placing
         * an extra vertex at its centre.
         *
         * @author David Letscher
         */
        void barycentricSubdivision();

        /**
         * Inserts a new layered solid torus into the triangulation.
         * The meridinal disc of the layered solid torus will intersect
         * the three edges of the boundary torus in \a cuts0, \a cuts1
         * and (\a cuts0 + \a cuts1) points respectively.
         *
         * The boundary torus will always consist of faces 012 and 013 of the
         * tetrahedron containing this boundary torus (this tetrahedron will be
         * returned).  In face 012, edges 12, 02 and 01 will meet the meridinal
         * disc \a cuts0, \a cuts1 and (\a cuts0 + \a cuts1) times respectively.
         * The only exceptions are if these three intersection numbers are
         * (1,1,2) or (0,1,1), in which case edges 12, 02 and 01 will meet the
         * meridinal disc (1, 2 and 1) or (1, 1 and 0) times respectively.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \pre 0 \<= \a cuts0 \<= \a cuts1;
         * \pre \a cuts1 is non-zero;
         * \pre gcd(\a cuts0, \a cuts1) = 1.
         *
         * \todo \bug This seems to segfault on 10000 tetrahedra.
         *
         * @param cuts0 the smallest of the three desired intersection numbers.
         * @param cuts1 the second smallest of the three desired intersection
         * numbers.
         * @return the tetrahedron containing the boundary torus.
         */
        NTetrahedron* insertLayeredSolidTorus(unsigned long cuts0,
            unsigned long cuts1);
        /**
         * Inserts a new lens space L(p,q) into the triangulation.
         * The lens space will be created by gluing together two layered
         * solid tori.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \pre \a p \> \a q \>= 0;
         * \pre gcd(\a p, \a q) = 1.
         *
         * \todo \bug Do this using the fewest possible tetrahedra.
         *
         * @param p a parameter of the desired lens space.
         * @param q a parameter of the desired lens space.
         */
        void insertLensSpace(unsigned long p, unsigned long q);
        /**
         * Inserts a copy of the given triangulation into this
         * triangulation.
         *
         * The new tetrahedra will be inserted into this triangulation
         * in the order in which they appear in the given triangulation,
         * and the numbering of their vertices (0-3) will not change.
         * They will be given the same descriptions as appear in the
         * given triangulation.
         *
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const NTriangulation& source);

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation (this is the same as
         * simplicially homeomorphic).
         *
         * \todo \opt Improve the complexity by choosing a tetrahedron
         * mapping from each component and following gluings to
         * determine the others.
         *
         * @param other the triangulation to compare with this one.
         * @return \c true if and only if this and the given
         * triangulation are combinatorially isomorphic.
         */
        bool isIsomorphicTo(NTriangulation& other);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void clearAllProperties();
        virtual void initialiseAllProperties();

        /**
         * Turns this triangulation into a clone of the given
         * triangulation.
         * The tree structure and label of this triangulation are not
         * touched.
         *
         * @param from the triangulation from which this triangulation
         * will be cloned.
         */
        void cloneFrom(const NTriangulation& from);
        
    private:
        void deleteTetrahedra();
            /**< Deallocates all tetrahedra and empties the list. */
        void deleteFaces();
            /**< Deallocates all faces and empties the list. */
        void deleteEdges();
            /**< Deallocates all edges and empties the list. */
        void deleteVertices();
            /**< Deallocates all vertices and empties the list. */
        void deleteComponents();
            /**< Deallocates all components and empties the list. */
        void deleteBoundaryComponents();
            /**< Deallocates all boundary components and empties the list. */
        void deleteSkeleton();
            /**< Deallocates all skeletal objects and empties all
                 corresponding lists. */

        /**
         * Recalculates vertices, edges, faces, components and
         * boundary components, as well as various other skeletal
         * properties such as validity and vertex links.
         * All appropriate lists are filled.
         *
         * \pre All skeletal lists are empty.
         */
        void calculateSkeleton();
        /**
         * Calculates the triangulation components and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateComponents();
        void labelComponent(NTetrahedron*, NComponent*, int);
            /**< Internal to calculateComponents(). */
        /**
         * Calculates the triangulation vertices and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateVertices();
        static void labelVertex(NTetrahedron*, int, NVertex*, int);
            /**< Internal to calculateVertices(). */
        /**
         * Calculates the triangulation edges and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateEdges();
        void labelEdge(NTetrahedron*, int, NEdge*, const NPerm&);
            /**< Internal to calculateEdges(). */
        /**
         * Calculates the triangulation faces and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateFaces();
        /**
         * Calculates the triangulation boundary components and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateBoundary();
        void labelBoundaryFace(NFace*, NBoundaryComponent*, int);
            /**< Internal to calculateBoundary(). */
        /**
         * Calculates the triangulation vertex links and associated
         * properties.
         *
         * \warning This should only be called from within
         * calculateSkeleton().
         */
        void calculateVertexLinks();

        /**
         * Calculates all properties that require examination of normal
         * surfaces.
         */
        void calculateSurfaceProperties();

        void stretchBoundaryForestFromVertex(NVertex*,
                NPointerSet<NEdge>&, NPointerSet<NVertex>&);
            /**< Internal to maximalForestInBoundary(). */
        bool stretchForestFromVertex(NVertex*,
                NPointerSet<NEdge>&, NPointerSet<NVertex>&,
                NPointerSet<NVertex>&);
            /**< Internal to maximalForestInSkeleton(). */
        void stretchDualForestFromTet(NTetrahedron*,
                NPointerSet<NFace>&, NPointerSet<NTetrahedron>&);
            /**< Internal to maximalForestInDualSkeleton(). */
};

// Inline functions for NTriangulation

inline NTriangulation::NTriangulation() {
    NTriangulation::initialiseAllProperties();
}

inline NTriangulation::NTriangulation(const NTriangulation& cloneMe) {
    NTriangulation::initialiseAllProperties();
    cloneFrom(cloneMe);
}

inline NTriangulation::~NTriangulation() {
    clearAllProperties();
    deleteTetrahedra();
}

inline NPacket* NTriangulation::internalClonePacket(NPacket*) const {
    return new NTriangulation(*this);
}

inline bool NTriangulation::dependsOnParent() const {
    return false;
}

inline unsigned long NTriangulation::getNumberOfTetrahedra() const {
    return tetrahedra.size();
}

inline NTetrahedron* NTriangulation::getTetrahedron(unsigned long index) {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return tetrahedra[index];
}

inline unsigned long NTriangulation::getTetrahedronIndex(
        const NTetrahedron* tet) const {
    return tetrahedra.position((NTetrahedron*)tet);
}

inline void NTriangulation::addTetrahedron(NTetrahedron* t) {
    clearAllProperties();
    tetrahedra.addLast(t);
}

inline NTetrahedron* NTriangulation::removeTetrahedronAt(unsigned long index) {
    clearAllProperties();
    tetrahedra[index]->isolate();
    return tetrahedra.removeFromPosition(index);
}

inline NTetrahedron* NTriangulation::removeTetrahedron(
        NTetrahedron* tet) {
    clearAllProperties();
    tet->isolate();
    return tetrahedra.remove(tet);
}

inline void NTriangulation::removeAllTetrahedra() {
    clearAllProperties();
    deleteTetrahedra();
}

inline void NTriangulation::gluingsHaveChanged() {
    clearAllProperties();
}

inline unsigned long NTriangulation::getNumberOfBoundaryComponents() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return boundaryComponents.size();
}

inline unsigned long NTriangulation::getNumberOfComponents() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return components.size();
}

inline unsigned long NTriangulation::getNumberOfVertices() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return vertices.size();
}

inline unsigned long NTriangulation::getNumberOfEdges() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return edges.size();
}

inline unsigned long NTriangulation::getNumberOfFaces() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return faces.size();
}

inline long NTriangulation::getEulerCharacteristic() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return long(vertices.size()) - long(edges.size())
        + long(faces.size()) - long(tetrahedra.size());
}

inline const NDynamicArray<NBoundaryComponent*> &
        NTriangulation::getBoundaryComponents() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return boundaryComponents;
}

inline const NDynamicArray<NTetrahedron*>& NTriangulation::getTetrahedra()
        const {
    return tetrahedra;
}

inline const NDynamicArray<NComponent*>& NTriangulation::getComponents() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return components;
}

inline const NDynamicArray<NVertex*>& NTriangulation::getVertices() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return vertices;
}

inline const NDynamicArray<NEdge*>& NTriangulation::getEdges() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return edges;
}

inline const NDynamicArray<NFace*>& NTriangulation::getFaces() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return faces;
}

inline bool NTriangulation::isValid() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return valid;
}

inline bool NTriangulation::isIdeal() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return ideal;
}

inline bool NTriangulation::isStandard() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return standard;
}

inline bool NTriangulation::hasBoundaryFaces() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return (faces.size() > 2 * tetrahedra.size());
}

inline bool NTriangulation::isClosed() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return (boundaryComponents.size() == 0);
}

inline bool NTriangulation::isOrientable() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return orientable;
}

inline bool NTriangulation::isConnected() {
    if (! calculatedSkeleton)
        calculateSkeleton();
    return (components.size() <= 1);
}

inline bool NTriangulation::isZeroEfficient() {
    if (! calculatedZeroEfficient)
        calculateSurfaceProperties();
    return zeroEfficient;
}

inline bool NTriangulation::knowsZeroEfficient() const {
    return calculatedZeroEfficient;
}

inline bool NTriangulation::hasSplittingSurface() {
    if (! calculatedSplittingSurface)
        calculateSurfaceProperties();
    return splittingSurface;
}

inline bool NTriangulation::knowsSplittingSurface() const {
    return calculatedSplittingSurface;
}

inline unsigned long NTriangulation::getHomologyH2Z2() {
    return getHomologyH1Rel().getRank() + getHomologyH1Rel().getTorsionRank(2);
}

inline void NTriangulation::writeTextShort(ostream& out) const {
    out << "Triangulation with " << tetrahedra.size() << " tetrahedra.";
}

#endif

