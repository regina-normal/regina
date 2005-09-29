
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

/*! \file npluggedsfs.h
 *  \brief Deals with triangulations of Seifert fibred spaces formed
 *  from connections between plugs and sockets.
 */

#ifndef __NPLUGGEDSFS_H
#ifndef __DOXYGEN
#define __NPLUGGEDSFS_H
#endif

#include <list>
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"

namespace regina {

class NSFSpace;
class NSFSPlug;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a two-sided annulus formed from a pair of faces in a
 * Seifert fibred space.  This annulus is saturated, i.e., a union of
 * fibres.  More than that, the fibres run parallel to the two boundary
 * edges of the annulus.
 *
 * The annulus is described from one side only.  The description
 * includes the two tetrahedra that provide the faces of the annulus, as
 * well as an additional permutation \a roles describing what roles the
 * individual tetrahedron vertices play.  More specifically, the annulus
 * can be drawn as follows, with the upper edge identified with the lower:
 *
 * <pre>
 *            *--->---*
 *            |0  2 / |
 *     First  |    / 1|  Second
 *     face   |   /   |   face
 *            |1 /    |
 *            | / 2  0|
 *            *--->---*
 * </pre>
 *
 * Suppose that \a tet[0] and \a tet[1] are the tetrahedra providing the
 * first and second faces respectively.  Then the tetrahedron vertices...
 *
 * TODO: Finish this description.
 */
struct NSFSAnnulus {
    NTetrahedron* tet[2];
    NPerm roles[2];

    NSFSAnnulus();
    NSFSAnnulus(const NSFSAnnulus& cloneMe);
    NSFSAnnulus(NTetrahedron* t0, NPerm r0, NTetrahedron* t1, NPerm r1);
    NSFSAnnulus& operator = (const NSFSAnnulus& cloneMe);

    bool meetsBoundary() const;

    /**
     * PRE: Must be something on the other side!
     */
    void switchSides();
    /**
     * PRE: Must be something on the other side!
     */
    NSFSAnnulus otherSide() const;

    void transform(const NTriangulation* originalTri,
            const NIsomorphism* iso, NTriangulation* newTri);
    NSFSAnnulus image(const NTriangulation* originalTri,
            const NIsomorphism* iso, NTriangulation* newTri) const;
};

class NSFSSocketHolder {
    protected:
        unsigned nSockets_;
        NSFSAnnulus* socket_;
        bool *socketOrient_;
            /**< True for non-reflected, false for reflected. */
        NSFSPlug** plug_;
            /** Guaranteed non-zero. */

    public:
        NSFSSocketHolder(const NSFSSocketHolder& cloneMe);

        /**
         * Takes everything else through the isomorphism, but set all
         * plugs to zero.
         */
        NSFSSocketHolder(const NSFSSocketHolder& preImage,
                const NTriangulation* preImageTri, const NIsomorphism* iso,
                NTriangulation* useTri);

        /**
         * Does \e not destroy plugs.  Just the arrays.
         */
        virtual ~NSFSSocketHolder();

        unsigned numberOfSockets() const;
        const NSFSAnnulus& socket(unsigned which) const;
        bool socketOrient(unsigned which) const;
        const NSFSPlug* plug(unsigned which) const;

        /**
         * Returns true if and only if all plugs were filled in.
         */
        bool isFullyPlugged(bool bailOnFailure);

    protected:
        /**
         * Initialises all plugs to zero.
         * Socket arrays are contructed but uninitialised.
         *
         * \a numSockets must be at least one.
         */
        NSFSSocketHolder(unsigned numSockets);

    private:
        /**
         * Do not allow direct assignment.
         *
         * \warning This operator does nothing!  Don't use it.
         */
        NSFSSocketHolder& operator = (const NSFSSocketHolder& cloneMe);
};

class NSFSPlug : public ShareableObject {
    protected:
        NSFSAnnulus toSocket_;

    public:
        /**
         * My side of the socket.
         */
        const NSFSAnnulus& toSocket() const;

        virtual void adjustSFS(NSFSpace& sfs, bool reflect) const = 0;

        virtual std::ostream& writeName(std::ostream& out) const = 0;
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        static NSFSPlug* isPlugged(const NSFSAnnulus& socket);
        static NSFSPlug* isPlugged(const NSFSAnnulus& socket,
            std::list<NTetrahedron*>& avoidTets);

    protected:
        NSFSPlug(const NSFSAnnulus& toSocket);
};

class NSFSRoot : public ShareableObject, public NSFSSocketHolder {
    protected:
        NTriangulation root_;

    public:
        const NTriangulation& root() const;

        virtual NSFSpace* createSFS() const = 0;

        virtual std::ostream& writeName(std::ostream& out) const = 0;
        virtual std::ostream& writeTeXName(std::ostream& out) const = 0;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    protected:
        NSFSRoot(unsigned numSockets);
};

/**
 * No plugs can be zero.
 */
class NSFSTree : public NStandardTriangulation, public NSFSSocketHolder {
    private:
        const NSFSRoot& root_;
        NIsomorphism* rootIso_;
            /** Must be non-zero. */

    public:
        /**
         * Destroys the plugs.
         */
        ~NSFSTree();

        const NSFSRoot& root() const;
        const NIsomorphism* rootIso() const;

        static NSFSTree* isSFSTree(NTriangulation* tri);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        NSFSTree(const NSFSRoot& whichRoot, NIsomorphism* whichRootIso,
                const NSFSSocketHolder& useSockets);

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        static NSFSTree* hunt(NTriangulation* tri, const NSFSRoot& root);
};

/*@}*/

// Inline functions for NSFSAnnulus

inline NSFSAnnulus::NSFSAnnulus() {
    tet[0] = tet[1] = 0;
}

inline NSFSAnnulus::NSFSAnnulus(const NSFSAnnulus& cloneMe) {
    tet[0] = cloneMe.tet[0]; tet[1] = cloneMe.tet[1];
    roles[0] = cloneMe.roles[0]; roles[1] = cloneMe.roles[1];
}

inline NSFSAnnulus::NSFSAnnulus(NTetrahedron* t0, NPerm r0,
        NTetrahedron* t1, NPerm r1) {
    tet[0] = t0; tet[1] = t1;
    roles[0] = r0; roles[1] = r1;
}

inline NSFSAnnulus& NSFSAnnulus::operator = (const NSFSAnnulus& cloneMe) {
    tet[0] = cloneMe.tet[0]; tet[1] = cloneMe.tet[1];
    roles[0] = cloneMe.roles[0]; roles[1] = cloneMe.roles[1];
    return *this;
}

inline NSFSAnnulus NSFSAnnulus::otherSide() const {
    NSFSAnnulus a(*this);
    a.switchSides();
    return a;
}

inline NSFSAnnulus NSFSAnnulus::image(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) const {
    NSFSAnnulus a(*this);
    a.transform(originalTri, iso, newTri);
    return a;
}

// Inline functions for NSFSSocketHolder

inline NSFSSocketHolder& NSFSSocketHolder::operator = (
        const NSFSSocketHolder&) {
    // Vacuous inaccessible assignment operator.
    return *this;
}

inline NSFSSocketHolder::NSFSSocketHolder(unsigned numSockets) :
        nSockets_(numSockets), socket_(new NSFSAnnulus[numSockets]),
        socketOrient_(new bool[numSockets]), plug_(new NSFSPlug*[numSockets]) {
    for (unsigned i = 0; i < numSockets; i++)
        plug_[i] = 0;
}

inline NSFSSocketHolder::~NSFSSocketHolder() {
    delete[] socket_;
    delete[] socketOrient_;
    delete[] plug_;
}

inline unsigned NSFSSocketHolder::numberOfSockets() const {
    return nSockets_;
}

inline const NSFSAnnulus& NSFSSocketHolder::socket(unsigned which) const {
    return socket_[which];
}

inline bool NSFSSocketHolder::socketOrient(unsigned which) const {
    return socketOrient_[which];
}

inline const NSFSPlug* NSFSSocketHolder::plug(unsigned which) const {
    return plug_[which];
}

// Inline functions for NSFSPlug

inline NSFSPlug::NSFSPlug(const NSFSAnnulus& toSocket) :
        toSocket_(toSocket) {
}

inline const NSFSAnnulus& NSFSPlug::toSocket() const {
    return toSocket_;
}

inline void NSFSPlug::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void NSFSPlug::writeTextLong(std::ostream& out) const {
    out << "SFS plug: ";
    writeName(out);
}

// Inline functions for NSFSRoot

inline const NTriangulation& NSFSRoot::root() const {
    return root_;
}

inline void NSFSRoot::writeTextShort(std::ostream& out) const {
    writeName(out);
}

inline void NSFSRoot::writeTextLong(std::ostream& out) const {
    out << "SFS root: ";
    writeName(out);
}

inline NSFSRoot::NSFSRoot(unsigned numSockets) : NSFSSocketHolder(numSockets) {
}

// Inline functions for NSFSTree

inline NSFSTree::NSFSTree(const NSFSRoot& whichRoot,
        NIsomorphism* whichRootIso, const NSFSSocketHolder& useSockets) :
        NSFSSocketHolder(useSockets), root_(whichRoot), rootIso_(whichRootIso) {
}

inline const NSFSRoot& NSFSTree::root() const {
    return root_;
}

inline const NIsomorphism* NSFSTree::rootIso() const {
    return rootIso_;
}

inline std::ostream& NSFSTree::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& NSFSTree::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

inline void NSFSTree::writeTextLong(std::ostream& out) const {
    out << "SFS tree: ";
    writeName(out);
}

} // namespace regina

#endif

