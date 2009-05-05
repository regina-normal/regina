
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file dim4triangulation.h
 *  \brief Deals with 4-manifold triangulations.
 */

#ifndef __DIM4TRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM4TRIANGULATION_H
#endif

#include <vector>
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4face.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4vertex.h"
#include "dim4/dim4component.h"
#include "dim4/dim4boundarycomponent.h"
#include "packet/npacket.h"
#include "utilities/nmarkedvector.h"
#include "utilities/nproperty.h"

namespace regina {

class NXMLDim4TriangulationReader;
class NXMLPacketReader;

/**
 * \addtogroup dim4 4-Manifold
 * Triangulations of 4-manifolds.
 * @{
 */

/**
 * Stores the triangulation of a 4-manifold along with its
 * various cellular structures and other information.  A 4-manifold
 * triangulation is built from pentachora; a \e pentachoron is a
 * 4-dimensional simplex, with five vertices.
 *
 * When the triangulation is deleted, the corresponding
 * pentachora, the cellular structure and all other properties
 * will be deallocated.
 *
 * Elements of the 3-, 2-, 1- and 0-skeletons (tetrahedra, faces, edges,
 * vertices respectively) are always temporary, as are components and
 * boundary components.  Whenever a change occurs with the triangulation,
 * these objects will all be deleted and a new skeletal structure will be
 * calculated.  The same is true of various other triangulation properties.
 *
 * Whenever the gluings of pentachora have been altered, the routine
 * responsible for changing the gluings \b must call
 * Dim4Triangulation::gluingsHaveChanged() to ensure that relevant
 * properties will be recalculated when necessary.  It is not necessary
 * to call this function when adding or removing pentachora.
 */
class Dim4Triangulation : public NPacket {
    public:
        static const int packetType;

        typedef std::vector<Dim4Pentachoron*>::const_iterator
                PentachoronIterator;
            /**< Used to iterate through pentachora. */
        typedef std::vector<Dim4Tetrahedron*>::const_iterator
                TetrahedronIterator;
            /**< Used to iterate through tetrahedra. */
        typedef std::vector<Dim4Face*>::const_iterator FaceIterator;
            /**< Used to iterate through faces. */
        typedef std::vector<Dim4Edge*>::const_iterator EdgeIterator;
            /**< Used to iterate through edges. */
        typedef std::vector<Dim4Vertex*>::const_iterator VertexIterator;
            /**< Used to iterate through vertices. */
        typedef std::vector<Dim4Component*>::const_iterator ComponentIterator;
            /**< Used to iterate through components. */
        typedef std::vector<Dim4BoundaryComponent*>::const_iterator
                BoundaryComponentIterator;
            /**< Used to iterate through boundary components. */

    private:
        mutable bool calculatedSkeleton_;
            /**< Has the skeleton been calculated? */

        NMarkedVector<Dim4Pentachoron> pentachora_;
            /**< The pentachora that form the triangulation. */
        mutable NMarkedVector<Dim4Tetrahedron> tetrahedra_;
            /**< The tetrahedra in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Face> faces_;
            /**< The faces in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Edge> edges_;
            /**< The edges in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Vertex> vertices_;
            /**< The vertices in the triangulation skeleton. */
        mutable NMarkedVector<Dim4Component> components_;
            /**< The components that form the triangulation. */
        mutable NMarkedVector<Dim4BoundaryComponent> boundaryComponents_;
            /**< The components that form the boundary of the
                 triangulation. */

        mutable bool valid_;
            /**< Is the triangulation valid? */
        mutable bool ideal_;
            /**< Is the triangulation ideal? */
        mutable bool orientable_;
            /**< Is the triangulation orientable? */

        mutable NProperty<NGroupPresentation, StoreManagedPtr> fundGroup_;
            /**< Fundamental group of the triangulation. */
        mutable NProperty<NAbelianGroup, StoreManagedPtr> H1_;
            /**< First homology group of the triangulation. */
        mutable NProperty<NAbelianGroup, StoreManagedPtr> H2_;
            /**< Second homology group of the triangulation. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Default constructor.
         *
         * Creates an empty triangulation.
         */
        Dim4Triangulation();
        /**
         * Copy constructor.
         *
         * Creates a new triangulation identical to the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * @param cloneMe the triangulation to clone.
         */
        Dim4Triangulation(const Dim4Triangulation& cloneMe);
        /**
         * Destroys this triangulation.
         *
         * The constituent pentachora, the cellular structure and all other
         * properties will also be deallocated.
         */
        virtual ~Dim4Triangulation();

        /*@}*/
        /**
         * (end: Constructors and Destructors)
         */

        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;

        virtual void writePacket(NFile& out) const;
        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        virtual bool dependsOnParent() const;

        /*@}*/
        /**
         * (end: Packet Administration)
         */

        /**
         * \name Pentachora
         */
        /*@{*/

        /**
         * Returns the number of pentachora in the triangulation.
         *
         * @return the number of pentachora.
         */
        unsigned long getNumberOfPentachora() const;
        /**
         * Returns all pentachora in the triangulation.
         *
         * The reference returned will remain valid for as long as the
         * triangulation exists, always reflecting the pentachora currently
         * in the triangulation.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all pentachora.
         */
        const std::vector<Dim4Pentachoron*>& getPentachora() const;
        /**
         * Returns the pentachoron with the given index number in the
         * triangulation.  Note that pentachoron indexing may change when
         * a pentachoron is added or removed from the triangulation.
         *
         * @param index specifies which pentachoron to return; this
         * value should be between 0 and getNumberOfPentachora()-1 inclusive.
         * @return the <tt>index</tt>th pentachoron in the triangulation.
         */
        Dim4Pentachoron* getPentachoron(unsigned long index);
        /**
         * Returns the pentachoron with the given index number in the
         * triangulation.  Note that pentachoron indexing may change when
         * a pentachoron is added or removed from the triangulation.
         *
         * @param index specifies which pentachoron to return; this
         * value should be between 0 and getNumberOfPentachora()-1 inclusive.
         * @return the <tt>index</tt>th pentachoron in the triangulation.
         */
        const Dim4Pentachoron* getPentachoron(unsigned long index) const;
        /**
         * Returns the index of the given pentachoron in the triangulation.
         *
         * Note that pentachoron indexing may change when a pentachoron
         * is added or removed from the triangulation.
         *
         * \pre The given pentachoron is contained in this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.  If you are passing the result of some other
         * routine that \e might return null (such as
         * Dim4Pentachoron::adjacentPentachoron), it might be worth explicitly
         * testing for null beforehand.
         *
         * @param pent specifies which pentachoron to find in the triangulation.
         * @return the index of the specified pentachoron, where 0 is
         * the first pentachoron, 1 is the second and so on.
         */
        long pentachoronIndex(const Dim4Pentachoron* pent) const;
        /**
         * Inserts the given pentachoron into the triangulation.
         * No face gluings anywhere will be examined or altered.
         *
         * The new pentachoron will be assigned a higher index in the
         * triangulation than all pentachora already present.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * \ifacespython Since this triangulation takes ownership of the
         * given pentachoron, the python object containing the given
         * pentachoron becomes a null object and should no longer be used.
         *
         * @param pent the pentachoron to insert.
         */
        void addPentachoron(Dim4Pentachoron* pent);
        /**
         * Removes the given pentachoron from the triangulation.
         * All faces glued to this pentachoron will be unglued.
         * The pentachoron will \e not be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * \pre The given pentachoron exists in the triangulation.
         *
         * @param pent the pentachoron to remove.
         * @return the removed pentachoron.
         */
        Dim4Pentachoron* removePentachoron(Dim4Pentachoron* pent);
        /**
         * Removes the pentachoron with the given index number
         * from the triangulation.  Note that pentachoron indexing may
         * change when a pentachoron is added or removed from the
         * triangulation.
         *
         * All faces glued to this pentachoron will be unglued.
         * The pentachoron will \e not be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         *
         * @param index specifies which pentachoron to remove; this
         * should be between 0 and getNumberOfPentachora()-1 inclusive.
         * @return the removed pentachoron.
         */
        Dim4Pentachoron* removePentachoronAt(unsigned long index);
        /**
         * Removes all pentachora from the triangulation.
         * All pentachora will be deallocated.
         *
         * There is no need to call gluingsHaveChanged() after calling
         * this function.
         */
        void removeAllPentachora();
        /**
         * This \b must be called whenever the gluings of pentachora are
         * changed!  Clears appropriate properties and performs other
         * necessary tasks.
         *
         * The responsibility of calling gluingsHaveChanged()
         * falls upon the routine that alters the gluings (such as a
         * component of a triangulation editor, or so on).
         */
        void gluingsHaveChanged();

        /*@}*/
        /**
         * (end: Pentachora)
         */

        /**
         * \name Skeletal Queries
         */
        /*@{*/

        /**
         * Returns the number of boundary components in this triangulation.
         *
         * Note that each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the Dim4BoundaryComponent
         * class notes and Dim4Vertex::isBoundary() for details.
         *
         * @return the number of boundary components.
         */
        unsigned long getNumberOfBoundaryComponents() const;
        /**
         * Returns the number of components in this triangulation.
         *
         * @return the number of components.
         */
        unsigned long getNumberOfComponents() const;
        /**
         * Returns the number of vertices in this triangulation.
         *
         * @return the number of vertices.
         */
        unsigned long getNumberOfVertices() const;
        /**
         * Returns the number of edges in this triangulation.
         *
         * @return the number of edges.
         */
        unsigned long getNumberOfEdges() const;
        /**
         * Returns the number of faces in this triangulation.
         *
         * @return the number of faces.
         */
        unsigned long getNumberOfFaces() const;
        /**
         * Returns the number of tetrahedra in this triangulation.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getNumberOfTetrahedra() const;

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
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all components.
         */
        const std::vector<Dim4Component*>& getComponents() const;
        /**
         * Returns all boundary components of this triangulation.
         *
         * Note that each ideal vertex forms its own boundary component, and
         * some invalid vertices do also.  See the Dim4BoundaryComponent
         * class notes and Dim4Vertex::isBoundary() for details.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all boundary components.
         */
        const std::vector<Dim4BoundaryComponent*>& getBoundaryComponents()
            const;
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
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all vertices.
         */
        const std::vector<Dim4Vertex*>& getVertices() const;
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
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all edges.
         */
        const std::vector<Dim4Edge*>& getEdges() const;
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
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all faces.
         */
        const std::vector<Dim4Face*>& getFaces() const;
        /**
         * Returns all tetrahedra of this triangulation.
         *
         * Bear in mind that each time the triangulation changes, the
         * tetrahedra will be deleted and replaced with new
         * ones.  Thus the objects contained in this list should be
         * considered temporary only.
         *
         * This reference to the list however will remain valid and
         * up-to-date for as long as the triangulation exists.
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all faces.
         */
        const std::vector<Dim4Tetrahedron*>& getTetrahedra() const;
        /**
         * Returns the requested triangulation component.
         *
         * Bear in mind that each time the triangulation changes, the
         * components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired component, ranging from 0
         * to getNumberOfComponents()-1 inclusive.
         * @return the requested component.
         */
        Dim4Component* getComponent(unsigned long index) const;
        /**
         * Returns the requested triangulation boundary component.
         *
         * Bear in mind that each time the triangulation changes, the
         * boundary components will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired boundary component, ranging
         * from 0 to getNumberOfBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
         */
        Dim4BoundaryComponent* getBoundaryComponent(unsigned long index) const;
        /**
         * Returns the requested triangulation vertex.
         *
         * Bear in mind that each time the triangulation changes, the
         * vertices will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired vertex, ranging from 0
         * to getNumberOfVertices()-1 inclusive.
         * @return the requested vertex.
         */
        Dim4Vertex* getVertex(unsigned long index) const;
        /**
         * Returns the requested triangulation edge.
         *
         * Bear in mind that each time the triangulation changes, the
         * edges will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired edge, ranging from 0
         * to getNumberOfEdges()-1 inclusive.
         * @return the requested edge.
         */
        Dim4Edge* getEdge(unsigned long index) const;
        /**
         * Returns the requested triangulation face.
         *
         * Bear in mind that each time the triangulation changes, the
         * faces will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired face, ranging from 0
         * to getNumberOfFaces()-1 inclusive.
         * @return the requested face.
         */
        Dim4Face* getFace(unsigned long index) const;
        /**
         * Returns the requested triangulation tetrahedron.
         *
         * Bear in mind that each time the triangulation changes, the
         * tetrahedra will be deleted and replaced with new
         * ones.  Thus this object should be considered temporary only.
         *
         * @param index the index of the desired tetrahedron, ranging from 0
         * to getNumberOfTetrahedra()-1 inclusive.
         * @return the requested tetrahedron.
         */
        Dim4Tetrahedron* getTetrahedron(unsigned long index) const;
        /**
         * Returns the index of the given component in the triangulation.
         *
         * \pre The given component belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param component specifies which component to find in the
         * triangulation.
         * @return the index of the specified component, where 0 is the first
         * component, 1 is the second and so on.
         */
        long componentIndex(const Dim4Component* component) const;
        /**
         * Returns the index of the given boundary component
         * in the triangulation.
         *
         * \pre The given boundary component belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param bc specifies which boundary component to find in the
         * triangulation.
         * @return the index of the specified boundary component,
         * where 0 is the first boundary component, 1 is the second and so on. 
         */
        long boundaryComponentIndex(const Dim4BoundaryComponent* bc) const;
        /**
         * Returns the index of the given vertex in the triangulation.
         *
         * \pre The given vertex belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param vertex specifies which vertex to find in the triangulation.
         * @return the index of the specified vertex, where 0 is the first
         * vertex, 1 is the second and so on.
         */
        long vertexIndex(const Dim4Vertex* vertex) const;
        /**
         * Returns the index of the given edge in the triangulation.
         *
         * \pre The given edge belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param edge specifies which edge to find in the triangulation.
         * @return the index of the specified edge, where 0 is the first
         * edge, 1 is the second and so on.
         */
        long edgeIndex(const Dim4Edge* edge) const;
        /**
         * Returns the index of the given face in the triangulation.
         *
         * \pre The given face belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param face specifies which face to find in the triangulation.
         * @return the index of the specified face, where 0 is the first
         * face, 1 is the second and so on.
         */
        long faceIndex(const Dim4Face* face) const;
        /**
         * Returns the index of the given tetrahedron in the triangulation.
         *
         * \pre The given tetrahedron belongs to this triangulation.
         *
         * \warning Passing a null pointer to this routine will probably
         * crash your program.
         *
         * @param tet specifies which tetrahedron to find in the triangulation.
         * @return the index of the specified tetrahedron, where 0 is the
         * first tetrahedron, 1 is the second and so on.
         */
        long tetrahedronIndex(const Dim4Tetrahedron* tet) const;

        /*@}*/
        /**
         * (end: Skeletal Queries)
         */

        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Returns the Euler characteristic of this triangulation.
         * This will be evaluated strictly as \a V-E+F-T+P.
         *
         * Note that this routine handles cusps in a non-standard way.
         * Since it computes the Euler characteristic of the
         * triangulation (and not the underlying manifold), this routine
         * will treat each cusp as a single vertex, and \e not as
         * a surface boundary component.
         *
         * For a routine that handles cusps properly (i.e., treats them
         * as 3-manifold boundary components when computing the Euler
         * characteristic), see getEulerCharManifold() instead.
         *
         * @return the Euler characteristic of this triangulation.
         */
        long getEulerCharTri() const;

        /**
         * Returns the Euler characteristic of the corresponding compact
         * manifold.
         *
         * Instead of simply calculating \a V-E+F-T+P, this routine also
         * treats ideal vertices as 3-manifold boundary components (i.e.,
         * effectively truncates them).
         *
         * For ideal triangulations, this routine therefore computes the
         * proper Euler characteristic of the manifold (unlike
         * getEulerCharTri(), which does not).
         *
         * For triangulations whose vertex links are all 3-spheres or
         * 3-balls, this routine and getEulerCharTri() give identical results.
         *
         * This routine does \e not yet handle invalid triangulations
         * correctly.  For this reason, this routine currently insists on
         * a valid triangulation as a precondition.
         *
         * \pre This triangulation is valid.
         *
         * @return the Euler characteristic of the corresponding compact
         * manifold.
         */
        long getEulerCharManifold() const;

        /**
         * Determines if this triangulation is valid.
         *
         * A triangulation is valid unless it contains an invalid
         * vertex, edge or face.
         *
         * - An invalid vertex has a bad vertex link (specifically, the
         *   link is either an invalid 3-manifold triangulation, an ideal
         *   3-manifold triangulation, or bounded but not a 3-ball).
         *
         * - An invalid edge has a bad edge link (neither a 2-sphere nor
         *   a disc), and/or is identified with itself in reverse.
         *
         * - An invalid face is identified with itself using a
         *   non-trivial rotation or reflection.
         *
         * If you wish to find out why a particular triangulation is
         * invalid, see Dim4Vertex::isValid(), Dim4Edge::isValid() and
         * Dim4Face::isValid() respectively.
         *
         * @return \c true if and only if this triangulation is valid.
         */
        bool isValid() const;
        /**
         * Determines if this triangulation is ideal.
         *
         * A triangulation is ideal if and only if (i) the triangulation
         * is valid, and (ii) one of the vertex links is closed but not
         * a 3-sphere.
         *
         * Note that for 4-manifolds, isIdeal() will only return \c true
         * if the triangulation is valid (unlike 3-manifolds, where
         * invalid ideal triangulations are allowed).  This is to avoid
         * situations like 4-manifold vertices whose links are cusped
         * 3-manifolds (a situation that has no analogue in lower dimensions).
         *
         * @return \c true if and only if this triangulation is ideal.
         */
        bool isIdeal() const;
        /**
         * Determines if this triangulation has any boundary tetrahedra.
         *
         * @return \c true if and only if there are boundary tetrahedra.
         */
        bool hasBoundaryTetrahedra() const;
        /**
         * Determines if this triangulation is closed.
         * This is the case if and only if it has no boundary components.
         *
         * Note that ideal triangulations are not closed.  Triangulations
         * with invalid vertices are also considered not closed; see
         * Dim4Vertex::isBoundary() for details.
         *
         * @return \c true if and only if this triangulation is closed.
         */
        bool isClosed() const;
        /**
         * Determines if this triangulation is orientable.
         *
         * @return \c true if and only if this triangulation is orientable.
         */
        bool isOrientable() const;
        /**
         * Determines if this triangulation is connected.
         *
         * @return \c true if and only if this triangulation is connected.
         */
        bool isConnected() const;

        /*@}*/
        /**
         * (end: Basic Properties)
         */

        /**
         * \name Algebraic Properties
         */
        /*@{*/

        /**
         * Returns the fundamental group of this triangulation.
         * If this triangulation contains any ideal vertices, the
         * fundamental group will be calculated as if each such vertex
         * had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * fundamental group will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getFundamentalGroup() should be called again; this will
         * be instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         * \pre This triangulation has at most one component.
         *
         * @return the fundamental group.
         */
        const NGroupPresentation& getFundamentalGroup() const;
        /**
         * Notifies the triangulation that you have simplified the
         * presentation of its fundamental group.  The old group
         * presentation will be destroyed, and this triangulation will take
         * ownership of the new (hopefully simpler) group that is passed.
         *
         * This routine is useful for situations in which some external
         * body (such as GAP) has simplified the group presentation
         * better than Regina can.
         *
         * Regina does \e not verify that the new group presentation is
         * equivalent to the old, since this is - well, hard.
         *
         * If the fundamental group has not yet been calculated for this
         * triangulation, this routine will nevertheless take ownership
         * of the new group, under the assumption that you have worked
         * out the group through some other clever means without ever
         * having needed to call getFundamentalGroup() at all.
         *
         * Note that this routine will not fire a packet change event.
         *
         * @param newGroup a new (and hopefully simpler) presentation of
         * the fundamental group of this triangulation.
         */
        void simplifiedFundamentalGroup(NGroupPresentation* newGroup);

        /**
         * Returns the first homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getHomologyH1() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the first homology group.
         */
        const NAbelianGroup& getHomologyH1() const;

        /**
         * Returns the second homology group for this triangulation.
         * If this triangulation contains any ideal vertices, the homology
         * group will be calculated as if each such vertex had been truncated.
         *
         * Bear in mind that each time the triangulation changes, the
         * homology groups will be deleted.  Thus the reference that is
         * returned from this routine should not be kept for later use.
         * Instead, getHomologyH2() should be called again; this will be
         * instantaneous if the group has already been calculated.
         *
         * \pre This triangulation is valid.
         *
         * @return the second homology group.
         */
        const NAbelianGroup& getHomologyH2() const;

        /*@}*/
        /**
         * (end: Algebraic Properties)
         */

        /**
         * \name Building Triangulations
         */
        /*@{*/

        /**
         * Inserts a copy of the given triangulation into this triangulation.
         *
         * The new pentachora will be inserted into this triangulation
         * in the order in which they appear in the given triangulation,
         * and the numbering of their vertices (0-4) will not change.
         * They will be given the same descriptions as appear in the
         * given triangulation.
         *
         * @param source the triangulation whose copy will be inserted.
         */
        void insertTriangulation(const Dim4Triangulation& source);
        /**
         * Inserts into this triangulation a set of pentachora and their
         * gluings as described by the given integer arrays.
         *
         * This routine is provided to make it easy to hard-code a
         * medium-sized triangulation in a C++ source file.  All of the
         * pertinent data can be hard-coded into a pair of integer arrays at
         * the beginning of the source file, avoiding an otherwise tedious
         * sequence of many joinTo() calls.
         *
         * An additional \a nPentachora pentachora will be inserted into
         * this triangulation.  The relationships between these pentachora
         * should be stored in the two arrays as follows.  Note that the
         * new pentachora are numbered from 0 to (\a nPentachora - 1), and
         * individual pentachoron facets are numbered from 0 to 4.
         *
         * The \a adjacencies array describes which pentachoron facets are
         * joined to which others.  Specifically, <tt>adjacencies[p][f]</tt>
         * should contain the number of the pentachoron joined to facet \a f
         * of pentachoron \a p.  If this facet is to be left as a
         * boundary tetrahedron, <tt>adjacencies[p][f]</tt> should be -1.
         *
         * The \a gluings array describes the particular gluing permutations
         * used when joining these pentachoron facets together.  Specifically,
         * <tt>gluings[p][f][0..4]</tt> should describe the permutation
         * used to join facet \a f of pentachoron \a p to its adjacent
         * pentachoron.  These five integers should be 0, 1, 2, 3 and 4 in some
         * order, so that <tt>gluings[p][f][i]</tt> contains the image of
         * \a i under this permutation.  If facet \a f of pentachoron \a p
         * is to be left as a boundary tetrahedron, <tt>gluings[p][f][0..4]</tt>
         * may contain anything (and will be duly ignored).
         *
         * It is the responsibility of the caller of this routine to
         * ensure that the given arrays are correct and consistent.
         * No error checking will be performed by this routine.
         *
         * Note that, for an existing triangulation, dumpConstruction()
         * will output a pair of C++ arrays that can be copied into a
         * source file and used to reconstruct the triangulation via
         * this routine.
         *
         * \ifacespython Not present.
         *
         * @param nPentachora the number of additional pentachora to insert.
         * @param adjacencies describes which of the new pentachoron
         * facets are to be identified.  This array must have initial
         * dimension at least \a nPentachora.
         * @param gluings describes the specific gluing permutations by
         * which these new pentachoron facets should be identified.  This
         * array must also have initial dimension at least \a nPentachora.
         */
        void insertConstruction(unsigned long nPentachora,
            const int adjacencies[][5], const int gluings[][5][5]);
        /**
         * Returns C++ code that can be used with insertConstruction()
         * to reconstruct this triangulation.
         *
         * The code produced will consist of the following:
         *
         * - the declaration and initialisation of two integer arrays,
         *   describing the pentachoron gluings in this trianguation;
         * - two additional lines that declare a new Dim4Triangulation and
         *   call insertConstruction() to rebuild this triangulation.
         *
         * The main purpose of this routine is to generate the two integer
         * arrays, which can be tedious and error-prone to code up by hand.
         *
         * Note that the number of lines of code produced grows linearly
         * with the number of pentachora.  If this triangulation is very
         * large, the returned string will be very large as well.
         *
         * @return the C++ code that was generated.
         */
        std::string dumpConstruction() const;

        /*@}*/
        /**
         * (end: Building Triangulations)
         */

        static NXMLPacketReader* getXMLReader(NPacket* parent);
        static Dim4Triangulation* readPacket(NFile& in, NPacket* parent);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

        /**
         * Turns this triangulation into a clone of the given triangulation.
         * The tree structure and label of this triangulation are not touched.
         *
         * @param from the triangulation from which this triangulation
         * will be cloned.
         */
        void cloneFrom(const Dim4Triangulation& from);

    private:
        void deletePentachora();
            /**< Deallocates all pentachora and empties the list. */
        void deleteSkeleton();
            /**< Deallocates all skeletal objects and empties all
                 corresponding lists. */

        /**
         * Clears any calculated properties and declares them all
         * unknown.  All dynamic memory used for storing known
         * properties is deallocated.
         *
         * In most cases this routine is called from gluingsHaveChanged(),
         * which also fires a packet change event.  If you plan to call
         * this routine, it is worth examining whether gluingsHaveChanged()
         * is a more appropriate routine to call instead.
         */
        virtual void clearAllProperties();

        /**
         * Recalculates vertices, edges, faces, tetrahedra, components and
         * boundary components, as well as various other skeletal properties
         * such as validity and vertex links.
         * All appropriate lists are filled.
         *
         * \pre All skeletal lists are empty.
         */
        void calculateSkeleton() const;

        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateComponents() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertices() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateEdges() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateFaces() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateBoundary() const;
        /**
         * Internal to calculateSkeleton().  See the comments within
         * calculateSkeleton() for precisely what this routine does.
         */
        void calculateVertexLinks() const;

    friend class regina::NXMLDim4TriangulationReader;
};

/*@}*/

// Inline functions for Dim4Triangulation

inline Dim4Triangulation::Dim4Triangulation() :
        NPacket(), calculatedSkeleton_(false) {
}

inline Dim4Triangulation::Dim4Triangulation(const Dim4Triangulation& cloneMe) :
        NPacket(), calculatedSkeleton_(false) {
    cloneFrom(cloneMe);
}

inline Dim4Triangulation::~Dim4Triangulation() {
    clearAllProperties();
    deletePentachora();
}

inline void Dim4Triangulation::writePacket(NFile&) const {
    // No need to add support for the old pre-2002 binary format.
    // This format is deprecated and about to be removed from Regina entirely.
}

inline void Dim4Triangulation::writeTextShort(std::ostream& out) const {
    out << "Triangulation with " << pentachora_.size() << " pentachora.";
}

inline bool Dim4Triangulation::dependsOnParent() const {
    return false;
}

inline unsigned long Dim4Triangulation::getNumberOfPentachora() const {
    return pentachora_.size();
}

inline const std::vector<Dim4Pentachoron*>& Dim4Triangulation::getPentachora()
        const {
    return (const std::vector<Dim4Pentachoron*>&)(pentachora_);
}

inline Dim4Pentachoron* Dim4Triangulation::getPentachoron(unsigned long index) {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return pentachora_[index];
}

inline const Dim4Pentachoron* Dim4Triangulation::getPentachoron(
        unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return pentachora_[index];
}

inline long Dim4Triangulation::pentachoronIndex(const Dim4Pentachoron* pent)
        const {
    return pent->markedIndex();
}

inline void Dim4Triangulation::addPentachoron(Dim4Pentachoron* pent) {
    pentachora_.push_back(pent);
    gluingsHaveChanged();
}

inline Dim4Pentachoron* Dim4Triangulation::removePentachoron(
        Dim4Pentachoron* pent) {
    pent->isolate();
    pentachora_.erase(pentachora_.begin() + pentachoronIndex(pent));
    gluingsHaveChanged();
    return pent;
}

inline Dim4Pentachoron* Dim4Triangulation::removePentachoronAt(
        unsigned long index) {
    Dim4Pentachoron* ans = pentachora_[index];
    ans->isolate();
    pentachora_.erase(pentachora_.begin() + index);
    gluingsHaveChanged();
    return ans;
}

inline void Dim4Triangulation::removeAllPentachora() {
    deletePentachora();
    gluingsHaveChanged();
}

inline void Dim4Triangulation::gluingsHaveChanged() {
    clearAllProperties();
    fireChangedEvent();
}

inline unsigned long Dim4Triangulation::getNumberOfBoundaryComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_.size();
}

inline unsigned long Dim4Triangulation::getNumberOfComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return components_.size();
}

inline unsigned long Dim4Triangulation::getNumberOfVertices() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return vertices_.size();
}

inline unsigned long Dim4Triangulation::getNumberOfEdges() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return edges_.size();
}

inline unsigned long Dim4Triangulation::getNumberOfFaces() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return faces_.size();
}

inline unsigned long Dim4Triangulation::getNumberOfTetrahedra() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return tetrahedra_.size();
}

inline const std::vector<Dim4Component*>& Dim4Triangulation::getComponents()
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4Component*>&)(components_);
}

inline const std::vector<Dim4BoundaryComponent*>&
        Dim4Triangulation::getBoundaryComponents() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4BoundaryComponent*>&)(boundaryComponents_);
}

inline const std::vector<Dim4Vertex*>& Dim4Triangulation::getVertices() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4Vertex*>&)(vertices_);
}

inline const std::vector<Dim4Edge*>& Dim4Triangulation::getEdges() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4Edge*>&)(edges_);
}

inline const std::vector<Dim4Face*>& Dim4Triangulation::getFaces() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4Face*>&)(faces_);
}

inline const std::vector<Dim4Tetrahedron*>& Dim4Triangulation::getTetrahedra()
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (const std::vector<Dim4Tetrahedron*>&)(tetrahedra_);
}

inline Dim4Component* Dim4Triangulation::getComponent(unsigned long index)
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return components_[index];
}

inline Dim4BoundaryComponent* Dim4Triangulation::getBoundaryComponent(
        unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_[index];
}

inline Dim4Vertex* Dim4Triangulation::getVertex(unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return vertices_[index];
}

inline Dim4Edge* Dim4Triangulation::getEdge(unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return edges_[index];
}

inline Dim4Face* Dim4Triangulation::getFace(unsigned long index) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return faces_[index];
}

inline Dim4Tetrahedron* Dim4Triangulation::getTetrahedron(unsigned long index)
        const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return tetrahedra_[index];
}

inline long Dim4Triangulation::componentIndex(const Dim4Component* component)
        const {
    return component->markedIndex();
}

inline long Dim4Triangulation::boundaryComponentIndex(
        const Dim4BoundaryComponent* boundaryComponent) const {
    return boundaryComponent->markedIndex();
}

inline long Dim4Triangulation::vertexIndex(const Dim4Vertex* vertex) const {
    return vertex->markedIndex();
}

inline long Dim4Triangulation::edgeIndex(const Dim4Edge* edge) const {
    return edge->markedIndex();
}

inline long Dim4Triangulation::faceIndex(const Dim4Face* face) const {
    return face->markedIndex();
}

inline long Dim4Triangulation::tetrahedronIndex(const Dim4Tetrahedron* tet)
        const {
    return tet->markedIndex();
}

inline long Dim4Triangulation::getEulerCharTri() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    // Cast away the unsignedness of std::vector::size().
    return static_cast<long>(vertices_.size())
        - static_cast<long>(edges_.size())
        + static_cast<long>(faces_.size())
        - static_cast<long>(tetrahedra_.size())
        + static_cast<long>(pentachora_.size());
}

inline bool Dim4Triangulation::isValid() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return valid_;
}

inline bool Dim4Triangulation::isIdeal() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return ideal_;
}

inline bool Dim4Triangulation::hasBoundaryTetrahedra() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (2 * tetrahedra_.size() > 5 * pentachora_.size());
}

inline bool Dim4Triangulation::isClosed() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return boundaryComponents_.empty();
}

inline bool Dim4Triangulation::isOrientable() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return orientable_;
}

inline bool Dim4Triangulation::isConnected() const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    return (components_.size() <= 1);
}

inline void Dim4Triangulation::simplifiedFundamentalGroup(
        NGroupPresentation* newGroup) {
    fundGroup_ = newGroup;
}

inline Dim4Triangulation* Dim4Triangulation::readPacket(NFile&, NPacket*) {
    // Don't bother supporting the old pre-2002 binary format.
    return 0;
}

inline NPacket* Dim4Triangulation::internalClonePacket(NPacket*) const {
    return new Dim4Triangulation(*this);
}

} // namespace regina

#endif

