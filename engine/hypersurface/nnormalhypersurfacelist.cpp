
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

#include <list>
#include "dim4/dim4triangulation.h"
#include "enumerate/ndoubledescription.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "hypersurface/hsflavourregistry.h"
#include "maths/nmatrixint.h"
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"
#include "utilities/xmlutils.h"

namespace regina {

#define __HSFLAVOUR_REGISTRY_BODY

const int NNormalHypersurfaceList::STANDARD = 0;
const int NNormalHypersurfaceList::EDGE_WEIGHT = 200;

#define REGISTER_HSFLAVOUR(id_name, class, n) \
    case NNormalHypersurfaceList::id_name: \
        return class::makeZeroVector(triangulation);

NNormalHypersurfaceVector* makeZeroVector(
        const Dim4Triangulation* triangulation, int flavour) {
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "hypersurface/hsflavourregistry.h"
    }
    return 0;
}

#undef REGISTER_HSFLAVOUR
#define REGISTER_HSFLAVOUR(id_name, class, n) \
    case NNormalHypersurfaceList::id_name: \
        return class::makeMatchingEquations(triangulation);

NMatrixInt* makeMatchingEquations(Dim4Triangulation* triangulation,
        int flavour) {
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "hypersurface/hsflavourregistry.h"
    }
    return 0;
}

#undef REGISTER_HSFLAVOUR
#define REGISTER_HSFLAVOUR(id_name, cls, n) \
    case NNormalHypersurfaceList::id_name: \
        constraints = cls::makeEmbeddedConstraints(triang_); break;

void* NNormalHypersurfaceList::Enumerator::run(void*) {
    NProgressNumber* progress = 0;
    if (manager_) {
        progress = new NProgressNumber(0, 1);
        manager_->setProgress(progress);
    }

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list_->embedded_)
        switch(list_->flavour_) {
            // Import cases from the flavour registry.
            #include "hypersurface/hsflavourregistry.h"
        }

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->flavour_);
    NNormalHypersurfaceVector* base = makeZeroVector(triang_, list_->flavour_);

    // Find the normal hypersurfaces.
    NDoubleDescription::enumerateExtremalRays(HypersurfaceInserter(*list_,
        triang_), *base, *eqns, constraints, progress);

    delete base;
    delete eqns;
    delete constraints;

    // All done!
    triang_->insertChildLast(list_);

    if (progress) {
        progress->incCompleted();
        progress->setFinished();
    }

    return 0;
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerate(
        Dim4Triangulation* owner, int flavour, bool embeddedOnly,
        NProgressManager* manager) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        flavour, embeddedOnly);
    Enumerator* e = new Enumerator(ans, owner, manager);

    if (manager) {
        if (! e->start(0, true)) {
            delete ans;
            return 0;
        }
        return ans;
    } else {
        e->run(0);
        delete e;
        return ans;
    }
}

Dim4Triangulation* NNormalHypersurfaceList::getTriangulation() const {
    return dynamic_cast<Dim4Triangulation*>(getTreeParent());
}

#undef REGISTER_HSFLAVOUR
#define REGISTER_HSFLAVOUR(id_name, c, name) \
    case id_name: o << name; break;

void NNormalHypersurfaceList::writeTextShort(std::ostream& o) const {
    o << surfaces_.size() << " vertex normal hypersurface";
    if (surfaces_.size() != 1)
        o << 's';
    o << " (";
    switch(flavour_) {
        // Import cases from the flavour registry...
        #include "hypersurface/hsflavourregistry.h"
        default:
            o << "Unknown";
            break;
    }
    o << ')';
}

#undef REGISTER_HSFLAVOUR
#define REGISTER_HSFLAVOUR(id_name, c, name) \
    case id_name: o << name << '\n'; break;

void NNormalHypersurfaceList::writeTextLong(std::ostream& o) const {
    if (embedded_)
        o << "Embedded ";
    else
        o << "Embedded, immersed & singular ";
    o << "vertex normal hypersurfaces\n";
    o << "Coordinates: ";
    switch(flavour_) {
        // Import cases from the flavour registry...
        #include "hypersurface/hsflavourregistry.h"
        default:
            o << "Unknown\n";
            break;
    }

    unsigned long n = getNumberOfHypersurfaces();
    o << "Number of hypersurfaces is " << n << '\n';
    for (unsigned long i = 0; i < n; i++) {
        getHypersurface(i)->writeTextShort(o);
        o << '\n';
    }
}

#undef REGISTER_HSFLAVOUR
#define REGISTER_HSFLAVOUR(id_name, c, name) \
    case id_name: out << regina::xml::xmlEncodeSpecialChars(name); break;

void NNormalHypersurfaceList::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params embedded=\"" << (embedded_ ? 'T' : 'F')
        << "\" flavourid=\"" << flavour_ << "\"\n";
    out << "\tflavour=\"";
    switch (flavour_) {
        // Import cases from the flavour registry...
        #include "hypersurface/hsflavourregistry.h"
        default:
            out << "Unknown";
            break;
    }
    out << "\"/>\n";

    // Write the individual hypersurfaces.
    std::vector<NNormalHypersurface*>::const_iterator it;
    for (it = surfaces_.begin(); it != surfaces_.end(); it++)
        (*it)->writeXMLData(out);
}

NPacket* NNormalHypersurfaceList::internalClonePacket(NPacket* /* parent */)
        const {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList();
    ans->flavour_ = flavour_;
    ans->embedded_ = embedded_;
    transform(surfaces_.begin(), surfaces_.end(), back_inserter(ans->surfaces_),
        FuncNewClonePtr<NNormalHypersurface>());
    return ans;
}

// Tidy up.
#undef REGISTER_HSFLAVOUR
#undef __HSFLAVOUR_REGISTRY_BODY

} // namespace regina

