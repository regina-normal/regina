
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <list>
#include "enumerate/ndoubledescriptor.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/flavourregistry.h"
#include "triangulation/ntriangulation.h"
#include "maths/nmatrixint.h"
#include "file/nfile.h"
#include "utilities/xmlutils.h"

namespace regina {

#define __FLAVOUR_REGISTRY_BODY

const int NNormalSurfaceList::STANDARD = 0;
const int NNormalSurfaceList::AN_STANDARD = 100;
const int NNormalSurfaceList::QUAD = 1;
const int NNormalSurfaceList::EDGE_WEIGHT = 200;
const int NNormalSurfaceList::FACE_ARCS = 201;

#define REGISTER_FLAVOUR(id_name, class, n, a, t) \
    case NNormalSurfaceList::id_name: \
        return class::makeMatchingEquations(triangulation);

NMatrixInt* makeMatchingEquations(NTriangulation* triangulation,
        int flavour) {
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
    }
    return 0;
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, cls, n, a, test) \
    case NNormalSurfaceList::id_name: \
        if (! (test)) \
            return; \
        if (embeddedOnly) \
            constraints = cls::makeEmbeddedConstraints(triang); \
        break;

NNormalSurfaceList::NNormalSurfaceList(NTriangulation* triang,
        int newFlavour, bool embeddedOnly) : flavour(newFlavour),
        embedded(embeddedOnly) {
    NNormalSurfaceList::initialiseAllProperties();
    triang->insertChildLast(this);

    // Perform any pre-enumeration tests and fetch any necessary
    // compatibility constraints.
    NCompConstraintSet* constraints = 0;
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
    }

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang, newFlavour);

    std::list<NNormalSurfaceVector*> originalCone;
    std::list<NVector<NLargeInteger>*> faces;
    createNonNegativeCone(triang, newFlavour, back_inserter(originalCone),
        back_inserter(faces));

    // Find the normal surfaces.
    NDoubleDescriptor().enumerateVertices(SurfaceInserter(*this, triang),
        originalCone.begin(), originalCone.end(), faces.begin(), faces.end(),
        *eqns, constraints);

    for_each(originalCone.begin(), originalCone.end(),
        FuncDelete<NNormalSurfaceVector>());
    for_each(faces.begin(), faces.end(),
        FuncDelete<NVector<NLargeInteger> >());
    delete eqns;
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, c, n, almost_normal, t) \
    case id_name: return almost_normal;

bool NNormalSurfaceList::allowsAlmostNormal() const {
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
    }
    return false;
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, c, name, a, t) \
    case id_name: o << name; break;

void NNormalSurfaceList::writeTextShort(std::ostream& o) const {
    o << surfaces.size() << " vertex normal surface";
    if (surfaces.size() != 1)
        o << 's';
    o << " (";
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
        default: o << "Unknown"; break;
    }
    o << ')';
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, c, name, a, t) \
    case id_name: o << name << '\n'; break;

void NNormalSurfaceList::writeTextLong(std::ostream& o) const {
    if (embedded)
        o << "Embedded ";
    else
        o << "Embedded, immersed & singular ";
    o << "vertex normal surfaces\n";
    o << "Coordinates: ";
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
        default: o << "Unknown\n"; break;
    }
    writeAllSurfaces(o);
}

void NNormalSurfaceList::writePacket(NFile& out) const {
    out.writeInt(flavour);
    out.writeBool(embedded);

    out.writeULong(surfaces.size());
    std::vector<NNormalSurface*>::const_iterator it;
    for (it = surfaces.begin(); it != surfaces.end(); it++)
        (*it)->writeToFile(out);

    // Write the properties.
    // At the moment there are no properties!
    writeAllPropertiesFooter(out);
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, c, name, a, t) \
    case id_name: out << regina::xml::xmlEncodeSpecialChars(name); break;

void NNormalSurfaceList::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params embedded=\"" << (embedded ? 'T' : 'F')
        << "\" flavourid=\"" << flavour << "\"\n";
    out << "\tflavour=\"";
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
        default: out << "Unknown"; break;
    }
    out << "\"/>\n";

    // Write the individual surfaces.
    std::vector<NNormalSurface*>::const_iterator it;
    for (it = surfaces.begin(); it != surfaces.end(); it++)
        (*it)->writeXMLData(out);
}

#undef REGISTER_FLAVOUR
#define REGISTER_FLAVOUR(id_name, c, n, a, t) \
    case id_name: break;

NNormalSurfaceList* NNormalSurfaceList::readPacket(NFile& in,
        NPacket* parent) {
    // Check that we recognise the flavour of coordinate system being
    // used.
    int flavour = in.readInt();
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
        default: return 0;
    }

    NNormalSurfaceList* ans = new NNormalSurfaceList();
    ans->flavour = flavour;
    ans->embedded = in.readBool();

    unsigned long nSurfaces = in.readULong();
    for (unsigned long i=0; i<nSurfaces; i++)
        ans->surfaces.push_back(NNormalSurface::readFromFile(in, ans->flavour,
            (NTriangulation*)parent));

    // Read the properties.
    ans->readProperties(in);

    return ans;
}

NPacket* NNormalSurfaceList::internalClonePacket(NPacket* parent) const {
    NNormalSurfaceList* ans = new NNormalSurfaceList();
    ans->flavour = flavour;
    ans->embedded = embedded;
    transform(surfaces.begin(), surfaces.end(), back_inserter(ans->surfaces),
        FuncNewClonePtr<NNormalSurface>());
    return ans;
}

void NNormalSurfaceList::initialiseAllProperties() {
}

void NNormalSurfaceList::readIndividualProperty(NFile& infile,
        unsigned propType) {
}

} // namespace regina

