
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file ndisc.h
 *  \brief Deals with individual normal discs and sets of normal discs
 *  in a normal surface.
 */

#ifndef __NDISC_H
#ifndef __DOXYGEN
#define __NDISC_H
#endif

#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Specifies a single normal disc in a normal surface.
 *
 * There are 10 disc types.  Types 0-3 represent triangles 0-3,
 * types 4-6 represent quads 0-2 and types 7-9 represent
 * octahedra 0-2.
 *
 * Discs of a specific type are assigned numbers from 0 upwards.
 * Triangular discs are numbered outwards from the vertex they surround.
 * Quad discs and octahedral discs are numbered outwards away from vertex 0
 * of the tetrahedron.
 *
 * \warning This class converts the indices of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded
 * normal surfaces.
 */
struct NDiscSpec {
    unsigned long tetIndex;
        /**< The index in the triangulation of the tetrahedron
             containing the disc. */
    int type;
        /**< The disc type; this is between 0 and 9 inclusive, as described
             in the \a NDiscSpec class notes. */
    unsigned long number;
        /**< Specifies which disc of the particular type in the
             particular tetrahedron is being referred to; discs
             are numbered as described in the \a NDiscSpec class notes. */

    /**
     * Creates a new uninitialised disc specifier.
     */
    NDiscSpec();
    /**
     * Creates a new disc specifier containing the given values.
     *
     * @param newTetIndex the index in the triangulation of the tetrahedron
     * containing the disc.
     * @param newType the disc type; this is between 0 and 9 inclusive,
     * as described in the \a NDiscSpec class notes.
     * @param newNumber specifies which disc of the particular type in the
     * particular tetrahedron is being referred to; discs are numbered
     * as described in the \a NDiscSpec class notes.
     */
    NDiscSpec(unsigned long newTetIndex, int newType, unsigned long newNumber);
    /**
     * Creates a new disc specifier that is a clone of the given specifier.
     *
     * @param cloneMe the disc specifier to clone.
     */
    NDiscSpec(const NDiscSpec& cloneMe);

    /**
     * Copies the values from the given disc specifier into this specifier.
     *
     * @param cloneMe the disc specifier whose values should be copied.
     * @return a reference to this disc specifier.
     */
    NDiscSpec& operator = (const NDiscSpec& cloneMe);
    /**
     * Determines if this and the given disc specifier contain identical
     * information.
     *
     * @param other the disc specifier to compare with this.
     * @return \c true if and only if this and the given disc specifier
     * contain identical information.
     */
    bool operator == (const NDiscSpec& other) const;

    friend std::ostream& operator << (std::ostream& out, const NDiscSpec& spec);
};

/**
 * Writes the given disc specifier to the given output stream.
 * The disc specifier will be written as a triple
 * <tt>(tetIndex, type, number)</tt>.
 *
 * @param out the output stream to which to write.
 * @param spec the disc specifier to write.
 * @return a reference to \a out.
 */
std::ostream& operator << (std::ostream& out, const NDiscSpec& spec);

/**
 * Determines whether or not normal discs of the given type are
 * numbered away from the given vertex.
 *
 * @param discType the normal disc type under consideration; this
 * should be between 0 and 9 inclusive, as described by the NDiscSpec
 * class notes.
 * @param vertex the vertex under consideration; this should be
 * between 0 and 3 inclusive.
 * @return \c true if normal discs of the given type are
 * numbered away from the given vertex, or \c false if they are
 * numbered towards the given vertex.
 */
bool numberDiscsAwayFromVertex(int discType, int vertex);

/**
 * Determines whether or not the natural boundary orientation of a normal
 * disc of the given type follows the given directed normal arc.
 * Natural boundary orientation is defined by arrays ::triDiscArcs,
 * ::quadDiscArcs and ::octDiscArcs.
 *
 * \pre The given normal arc lies on a normal disc of the given type.
 *
 * @param discType the normal disc type under consideration; this should
 * be between 0 and 9 inclusive, as described by the NDiscSpec class
 * notes.
 * @param vertex the vertex about which the normal arc runs.
 * @param edgeStart the start vertex of the edge to which the normal arc
 * is parallel.
 * @param edgeEnd the end vertex of the edge to which the normal arc is
 * parallel.
 */
bool discOrientationFollowsEdge(int discType, int vertex, int edgeStart,
        int edgeEnd);

/**
 * Represents a set of normal discs inside a single tetrahedron.
 * The numbers of discs of each type are stored in this structure, so
 * querying them is fast regardless of the underlying normal surface
 * coordinate system used.
 *
 * \warning This class converts the number of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded normal surfaces.
 *
 * \todo \problong Have some error flag so we can barf politely if the number
 * of normal discs of a given type does not fit into an <tt>unsigned
 * long</tt>.  See how this affects NDiscSetTetData also.
 */
class NDiscSetTet {
    protected:
        unsigned long internalNDiscs[10];
            /**< The number of discs of each type. */

    public:
        /**
         * Creates a new set of normal discs corresponding to the discs
         * of the given normal surface that lie within the given
         * tetrahedron.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.getNumberOfTetrahedra()-1</tt> inclusive, where
         * <tt>tri</tt> is the triangulation containing the given normal
         * surface.
         */
        NDiscSetTet(const NNormalSurface& surface, unsigned long tetIndex);
        /**
         * Destroys this disc set.
         */
        virtual ~NDiscSetTet();

        /**
         * Determines the number of discs of the given type inside this
         * tetrahedron.
         *
         * @param type the disc type to examine; this should be between
         * 0 and 9 inclusive.  Disc types are outlined in
         * the NDiscSpec class notes.
         * @return the number of discs of the given type inside this
         * tetrahedron.
         */
        unsigned long nDiscs(int type) const;

        /**
         * Determines which normal arc of a given type on a given face
         * of this tetrahedron corresponds to the given normal disc.
         *
         * \pre The given normal disc actually meets a
         * normal arc of the given type on the given face.
         *
         * @param arcFace the face of this tetrahedron containing the
         * normal arc (between 0 and 3 inclusive).
         * @param arcVertex the vertex of this tetrahedron about which the
         * normal arc runs (between 0 and 3 inclusive); \a arcFace and
         * \a arcVertex should not be the same.
         * @param discType the disc type of the given normal disc;
         * this should be between 0 and 9 inclusive, as described in the
         * NDiscSpec class notes.
         * @param discNumber indicates which normal disc of the given disc
         * type is referred to (between 0 and <tt>nDiscs(discType)-1</tt>
         * inclusive).
         * @return the number of the normal arc of the given type that belongs
         * to the given normal disc.
         * Arcs of a given type (where \a arcFace and \a arcVertex
         * together define the arc type) are numbered starting at 0 from the
         * tetrahedron vertex outwards.
         */
        unsigned long arcFromDisc(int arcFace, int arcVertex,
            int discType, unsigned long discNumber) const;
        /**
         * Determines which normal disc in this tetrahedron meets 
         * the given normal arc on the given face.
         *
         * \pre The given normal arc
         * actually exists in the normal surface with which this
         * \a NDiscSetTet object was created.
         *
         * @param arcFace the face of this tetrahedron containing the
         * normal arc (between 0 and 3 inclusive).
         * @param arcVertex the vertex of this tetrahedron about which the
         * normal arc runs (between 0 and 3 inclusive); \a arcFace and
         * \a arcVertex should not be the same.
         * @param arcNumber indicates which normal arc of the given type
         * is referred to.
         * Arcs of a given type (where \a arcFace and \a arcVertex
         * together define the arc type) are numbered starting at 0 from the
         * tetrahedron vertex outwards.
         * @param discType returns the disc type of the normal disc that
         * meets the given normal arc; this will be between 0 and 9
         * inclusive, as described in the NDiscSpec class notes.
         * Any value may be initially passed.
         * @param discNumber returns a number that indicates which
         * normal disc of the returned disc type (<tt>discType</tt>)
         * meets the given normal arc; this will be between 0 and
         * <tt>nDiscs(discType)-1</tt> inclusive.  Any value may be
         * initially passed.
         */
        void discFromArc(int arcFace, int arcVertex, unsigned long arcNumber,
            int& discType, unsigned long& discNumber) const;
};

/**
 * Stores data of type \c T for every normal disc inside a single
 * tetrahedron.
 *
 * \warning This class converts the number of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded
 * normal surfaces.
 * \pre Type T has a default constructor and an
 * assignment operator.  That is, if \c a and \c b are of type T, then
 * \c a can be declared with no parameters and can then receive the
 * value of \c b using <tt>a=b</tt>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NDiscSetTetData : public NDiscSetTet {
    public:
        typedef T* DataPtr;
            /**< A type that is a pointer to the data stored with each
             *   disc. */

    protected:
        DataPtr internalData[10];
            /**< Stores the data corresponding to each normal disc. */

    public:
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface that lie within the given tetrahedron.
         * The data for each disc will remain uninitialised.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.getNumberOfTetrahedra()-1</tt> inclusive, where
         * <tt>tri</tt> is the triangulation containing the given normal
         * surface.
         */
        NDiscSetTetData(const NNormalSurface& surface,
                unsigned long tetIndex) : NDiscSetTet(surface, tetIndex) {
            for (int i=0; i<10; i++)
                if (internalNDiscs[i])
                    internalData[i] = new T[internalNDiscs[i]];
                else
                    internalData[i] = 0;
        }
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface that lie within the given tetrahedron.
         * The data for each disc will be initialised to the given
         * value.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param tetIndex the index in the triangulation of the
         * tetrahedron that our discs must lie in; this must be between
         * 0 and <tt>tri.getNumberOfTetrahedra()-1</tt> inclusive, where
         * <tt>tri</tt> is the triangulation containing the given normal
         * surface.
         * @param initValue the value with which to initialise the data
         * corresponding to each disc.
         */
        NDiscSetTetData(const NNormalSurface& surface,
                unsigned long tetIndex, const T& initValue) :
                NDiscSetTet(surface, tetIndex) {
            unsigned long disc;
            for (int i=0; i<10; i++)
                if (internalNDiscs[i]) {
                    internalData[i] = new T[internalNDiscs[i]];
                    for (disc = 0; disc < internalNDiscs[i]; disc++)
                        internalData[i][disc] = initValue;
                } else
                    internalData[i] = 0;
        }
        /**
         * Destroys this disc set and deallocates all data arrays.
         * Note that no assumption is made about type \c T, so if data
         * elements are pointers to dynamically allocated objects, these
         * will not be destroyed.
         */
        virtual ~NDiscSetTetData() {
            for (int i=0; i<10; i++)
                if (internalData[i])
                    delete[] internalData[i];
        }

        /**
         * Retrieves a reference to the data corresponding to the given
         * normal disc.
         *
         * @param discType the disc type of the given normal disc;
         * this should be between 0 and 9 inclusive, as described in the
         * NDiscSpec class notes.
         * @param discNumber indicates which normal disc of the given disc
         * type is referred to; this should be between 0 and
         * <tt>nDiscs(discType)-1</tt> inclusive.
         * @return a reference to the data corresponding to the given
         * normal disc.
         */
        T& data(int discType, unsigned long discNumber) {
            return internalData[discType][discNumber];
        }
};

/**
 * Represents the set of all normal discs forming a normal surface.
 * These are stored as an array of NDiscSetTet objects, one for each
 * tetrahedron.
 *
 * \warning This class converts the number of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded
 * normal surfaces.
 */
class NDiscSetSurface {
    protected:
        NDiscSetTet** discSets;
            /**< The disc sets corresponding to each tetrahedron. */
        NTriangulation* triangulation;
            /**< The triangulation in which the normal surface lives. */

    protected:
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.  The array of tetrahedron disc set
         * pointers will be created but the NDiscSetTet objects themselves
         * will <b>not</b> be created.
         *
         * This constructor should be called from constructors of subclasses
         * who wish to use objects of a subclass of NDiscSetTet, which
         * this constructor allows them to create for themselves.
         *
         * \warning After calling this constructor, each
         * NDiscSetTet object
         * in the \a discSets array <b>must</b> be created, since the
         * \a NDiscSetSurface destructor will attempt to destroy them!
         * The \a discSets array will have size
         * <tt>surface.getTriangulation()->getNumberOfTetrahedra()</tt>.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param b this parameter is ignored.
         */
        NDiscSetSurface(const NNormalSurface& surface, bool b);

    public:
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.
         *
         * @param surface the normal surface whose discs we shall use.
         */
        NDiscSetSurface(const NNormalSurface& surface);
        /**
         * Destroys this set of discs and deallocates all associated memory.
         */
        virtual ~NDiscSetSurface();

        /**
         * Returns the number of tetrahedra in the underlying
         * triangulation.
         *
         * @return the number of tetrahedra.
         */
        unsigned long nTets() const;
        /**
         * Determines the number of discs of the given type inside the
         * given tetrahedron.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron to examine.
         * @param type the disc type to examine; this should be between
         * 0 and 9 inclusive.  Disc types are outlined in
         * the NDiscSpec class notes.
         * @return the number of discs of the given type inside the
         * given tetrahedron.
         */
        unsigned long nDiscs(unsigned long tetIndex, int type) const;
        /**
         * Returns the specific set of discs living inside the given
         * tetrahedron.
         *
         * @param tetIndex the index in the triangulation of the given
         * tetrahedron.
         * @return the set of discs inside the given tetrahedron.
         */
        NDiscSetTet& tetDiscs(unsigned long tetIndex) const;

        /**
         * Determines which normal disc is adjacent to the given normal disc
         * along the given directed normal arc in the surface described by
         * this disc set.
         *
         * A directed normal arc will be specified by a permutation
         * <i>p</i>, where the arc runs around vertex <tt>p[0]</tt>
         * parallel to the directed edge from vertex <tt>p[1]</tt> to
         * <tt>p[2]</tt>.
         *
         * @param disc the given normal disc; this must be a disc in this
         * disc set.
         * @param arc the given normal arc; this must actually be an arc
         * on the boundary of the given normal disc (although it may run
         * in either direction).
         * @param adjArc returns the same directed normal arc that was
         * passed, but expressed in terms of the vertices of the
         * adjacent tetrahedron.  Any value may be initially passed.  If
         * there is no adjacent disc/tetrahedron, this permutation will
         * remain unchanged.
         * @return the normal disc adjacent to the given disc along the
         * given arc, or 0 if there is no adjacent disc.  This disc
         * specifier will be newly created, and it is up to the caller
         * of this routine to dispose of it.
         */
        NDiscSpec* adjacentDisc(const NDiscSpec& disc, NPerm arc,
                NPerm& adjArc) const;
};

/**
 * Stores data of type \c T for every normal disc within a particular
 * normal surface.
 * This data is stored using an array of NDiscSetTetData<T> objects,
 * one for each tetrahedron (thus the inherited member function
 * tetDiscs() will return an object of class NDiscSetTetData<T>).
 *
 * \warning This class converts the number of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 * \pre This class should only be used with \a embedded normal surfaces.
 * \pre Type T has a default constructor and an
 * assignment operator.  That is, if \c a and \c b are of type T, then
 * \c a can be declared with no parameters and can then receive the
 * value of \c b using <tt>a=b</tt>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NDiscSetSurfaceData : public NDiscSetSurface {
    public:
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.
         * The data for each disc will remain uninitialised.
         *
         * @param surface the normal surface whose discs we shall use.
         */
        NDiscSetSurfaceData(const NNormalSurface& surface) :
                NDiscSetSurface(surface, true) {
            unsigned long tot = triangulation->getNumberOfTetrahedra();
            if (tot)
                for (unsigned long index = 0; index < tot; index++)
                    discSets[index] = new NDiscSetTetData<T>(surface, index);
        }
        /**
         * Creates a new disc set corresponding to the discs of the
         * given normal surface.
         * The data for each disc will be initialised to the given
         * value.
         *
         * @param surface the normal surface whose discs we shall use.
         * @param initValue the value with which to initialise the data
         * corresponding to each disc.
         */
        NDiscSetSurfaceData(const NNormalSurface& surface, const T& initValue) :
                NDiscSetSurface(surface, true) {
            unsigned long tot = triangulation->getNumberOfTetrahedra();
            if (tot)
                for (unsigned long index = 0; index < tot; index++)
                    discSets[index] = new NDiscSetTetData<T>(surface, index,
                        initValue);
        }

        /**
         * Retrieves a reference to the data corresponding to the given
         * normal disc.
         *
         * @param disc the disc whose data we require; this must refer
         * to a disc within this disc set.
         * @return a reference to the data corresponding to the given
         * normal disc.
         */
        T& data(const NDiscSpec& disc) {
            return dynamic_cast<NDiscSetTetData<T>*>(discSets[disc.tetIndex])->
                data(disc.type, disc.number);
        }
};

/**
 * An iterator used for running through all normal discs in a normal
 * surface.
 *
 * \warning This class converts the indices of normal discs of a
 * given type from NLargeInteger to <tt>unsigned long</tt>.  See the
 * precondition below.
 *
 * \pre The number of normal discs of a particular type
 * in a particular tetrahedron can be represented by a long integer.
 */
class NDiscSpecIterator {
    protected:
        const NDiscSetSurface* internalDiscSet;
            /**< The disc set through which we are iterating. */
        NDiscSpec current;
            /**< The disc currently pointed to. */

    public:
        /**
         * Creates a new uninitialised iterator.
         * This iterator cannot be used or queried until init() is called.
         */
        NDiscSpecIterator();
        /**
         * Creates a new iterator pointing to the first disc in the
         * given disc set.
         *
         * @param discSet the disc set used to initialise this iterator.
         */
        NDiscSpecIterator(const NDiscSetSurface& discSet);
        /**
         * Points this iterator to the first disc in the given disc set.
         *
         * @param discSet the disc set used to reinitialise this iterator.
         */
        void init(const NDiscSetSurface& discSet);

        /**
         * Points this iterator to the next disc, or makes it
         * past-the-end if there is no next disc.
         *
         * \pre This iterator is not past-the-end.
         *
         * \ifacespython This routine is called inc(), since Python does
         * not support the increment operator.
         */
        void operator++(int);
        /**
         * Returns a reference to the disc pointed to by this iterator.
         *
         * \pre This iterator is not past-the-end.
         *
         * \ifacespython This routine is called deref(), since Python does
         * not support the dereference operator.
         *
         * @return a reference to the disc pointed to by this iterator.
         */
        const NDiscSpec& operator *() const;
        /**
         * Determines if this iterator is past-the-end.
         *
         * @return \c true if and only if this iterator is past-the-end.
         */
        bool done() const;

    private:
        /**
         * Ensures the data member \a current points to a real disc and
         * not a virtual disc (in which the disc number exceeds the
         * number of discs of the corresponding type).
         *
         * \pre This iterator is not yet past-the-end
         * (although it may be in the middle of an increment operation
         * that will put it past-the-end).
         */
        void makeValid();
};

/*@}*/

// Inline functions for NDiscSpec

inline NDiscSpec::NDiscSpec() {
}
inline NDiscSpec::NDiscSpec(unsigned long newTetIndex, int newType,
        unsigned long newNumber) : tetIndex(newTetIndex), type(newType),
        number(newNumber) {
}
inline NDiscSpec::NDiscSpec(const NDiscSpec& cloneMe) :
        tetIndex(cloneMe.tetIndex), type(cloneMe.type),
        number(cloneMe.number) {
}

inline NDiscSpec& NDiscSpec::operator = (const NDiscSpec& cloneMe) {
    tetIndex = cloneMe.tetIndex;
    type = cloneMe.type;
    number = cloneMe.number;
    return *this;
}
inline bool NDiscSpec::operator == (const NDiscSpec& other) const {
    return (tetIndex == other.tetIndex && type == other.type &&
        number == other.number);
}

// Inline functions for NDiscSetTet

inline NDiscSetTet::~NDiscSetTet() {
}

inline unsigned long NDiscSetTet::nDiscs(int type) const {
    return internalNDiscs[type];
}

// Inline functions for NDiscSetSurface

inline unsigned long NDiscSetSurface::nTets() const {
    return triangulation->getNumberOfTetrahedra();
}

inline unsigned long NDiscSetSurface::nDiscs(unsigned long tetIndex,
        int type) const {
    return discSets[tetIndex]->nDiscs(type);
}

inline NDiscSetTet& NDiscSetSurface::tetDiscs(unsigned long tetIndex) const {
    return *(discSets[tetIndex]);
}

// Inline functions for NDiscSpecIterator

inline NDiscSpecIterator::NDiscSpecIterator() {
}
inline NDiscSpecIterator::NDiscSpecIterator(const NDiscSetSurface& discSet) :
        internalDiscSet(&discSet), current(0, 0, 0) {
    makeValid();
}
inline void NDiscSpecIterator::init(const NDiscSetSurface& discSet) {
    internalDiscSet = &discSet;
    current.tetIndex = 0;
    current.type = 0;
    current.number = 0;
    makeValid();
}

inline void NDiscSpecIterator::operator++(int) {
    current.number++;
    makeValid();
}
inline const NDiscSpec& NDiscSpecIterator::operator *() const {
    return current;
}
inline bool NDiscSpecIterator::done() const {
    return (current.tetIndex == internalDiscSet->nTets());
}

} // namespace regina

#endif

