
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include <climits>
#include <cstring>

#include "maths/nmatrixint.h"
#include "snappea/nsnappeatriangulation.h"
#include "snappea/kernel/kernel_prototypes.h"
#include "snappea/kernel/triangulation.h"
#include "snappea/kernel/unix_file_io.h"
#include "snappea/snappy/SnapPy.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nthread.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    /**
     * A mutex to protect kernelMessages.
     */
    static NMutex snapMutex;
}

std::complex<double> NSnapPeaTriangulation::zero_(0, 0);

NSnapPeaTriangulation::NSnapPeaTriangulation(
        const std::string& fileNameOrContents) :
        data_(0), shape_(0), cusp_(0), syncing_(false) {
    try {
        if (startsWith(fileNameOrContents, "% Triangulation"))
            data_ = regina::snappea::read_triangulation_from_string(
                fileNameOrContents.c_str());
        else
            data_ = regina::snappea::read_triangulation(
                fileNameOrContents.c_str());

        if (data_) {
            regina::snappea::find_complete_hyperbolic_structure(data_);
            regina::snappea::do_Dehn_filling(data_);
            setPacketLabel(get_triangulation_name(data_));
            sync();
        }
    } catch (regina::SnapPeaFatalError& err) {
        data_ = 0;
    }
    listen(this);
}

NSnapPeaTriangulation::NSnapPeaTriangulation(const NSnapPeaTriangulation& tri) :
        data_(0), shape_(0), cusp_(0), syncing_(false) {
    if (tri.data_) {
        regina::snappea::copy_triangulation(tri.data_, &data_);
        sync();
    }
    listen(this);
}

NSnapPeaTriangulation::NSnapPeaTriangulation(const NTriangulation& tri,
        bool allowClosed) :
        data_(0), shape_(0), cusp_(0), syncing_(false) {
    const NSnapPeaTriangulation* clone =
        dynamic_cast<const NSnapPeaTriangulation*>(&tri);
    if (clone) {
        // We have a full SnapPea triangulation to clone.
        if (clone->data_) {
            regina::snappea::copy_triangulation(clone->data_, &data_);
            sync();
        }
        listen(this);
        return;
    }

    // We are building a SnapPea triangulation from one of Regina's
    // own NTriangulation data structures.
    //
    // Make sure SnapPea is likely to be comfortable with it.
    if (tri.getNumberOfTetrahedra() == 0 ||
            tri.hasBoundaryTriangles() ||
            (! tri.isConnected()) ||
            (! tri.isValid()) ||
            (! tri.isStandard())) {
        listen(this);
        return;
    }
    if (tri.isIdeal()) {
        // If it's ideal, make sure every vertex is ideal.
        if (tri.getNumberOfBoundaryComponents() < tri.getNumberOfVertices()) {
            listen(this);
            return;
        }
    } else {
        // No boundary triangles, not ideal.. must be closed.
        // If closed is okay, at least make sure it's one-vertex.
        if ((! allowClosed) || (1 != tri.getNumberOfVertices())) {
            listen(this);
            return;
        }
    }
    if (tri.getNumberOfTetrahedra() >= INT_MAX) {
        listen(this);
        return;
    }

    // Looks good; go build the SnapPea triangulation.
    regina::snappea::TriangulationData tData;
    tData.name = strdup(tri.getPacketLabel().c_str());
    tData.num_tetrahedra = static_cast<int>(tri.getNumberOfTetrahedra());

    // Fields recalculated by SnapPea:
    tData.solution_type = regina::snappea::not_attempted;
    tData.volume = 0;
    tData.orientability = regina::snappea::unknown_orientability;
    tData.CS_value_is_known = false;
    tData.CS_value = 0;
    tData.num_or_cusps = 0;
    tData.num_nonor_cusps = 0;
    tData.cusp_data = 0;

    tData.tetrahedron_data = new regina::snappea::TetrahedronData[
        tData.num_tetrahedra];
    int tet, face, i, j, k, l;
    NTriangulation::TetrahedronIterator it = tri.getTetrahedra().begin();
    for (tet = 0; tet < tData.num_tetrahedra; tet++) {
        for (face = 0; face < 4; face++) {
            tData.tetrahedron_data[tet].neighbor_index[face] = static_cast<int>(
                tri.tetrahedronIndex((*it)->adjacentTetrahedron(face)));
            for (i = 0; i < 4; i++)
                tData.tetrahedron_data[tet].gluing[face][i] =
                    (*it)->adjacentGluing(face)[i];
        }

        // Other fields are recalculated by SnapPea.
        for (i = 0; i < 4; i++)
            tData.tetrahedron_data[tet].cusp_index[i] = -1;
        for (i = 0; i < 2; i++)
            for (j = 0; j < 2; j++)
                for (k = 0; k < 4; k++)
                    for (l = 0; l < 4; l++)
                        tData.tetrahedron_data[tet].curve[i][j][k][l] = 0;
        tData.tetrahedron_data[tet].filled_shape.real = 0;
        tData.tetrahedron_data[tet].filled_shape.imag = 0;

        it++;
    }

    regina::snappea::data_to_triangulation(&tData, &data_);

    delete[] tData.tetrahedron_data;
    free(tData.name);

    if (! data_) {
        listen(this);
        return;
    }

    // All done.  Recalculate what we need to.
    regina::snappea::find_complete_hyperbolic_structure(data_);
    regina::snappea::do_Dehn_filling(data_);

    // Regina triangulations know nothing about peripheral curves.
    // Install a sensible basis for each cusp, if SnapPea will let us.
    SolutionType soln = static_cast<SolutionType>(
        regina::snappea::get_filled_solution_type(data_));
    if (soln == geometric_solution || soln == nongeometric_solution) {
        try {
            regina::snappea::install_shortest_bases(data_);
        } catch (regina::SnapPeaFatalError& err) {
            // Blurgh.  SnapPea says no.
        }
    }

    sync();
    listen(this);
}

NSnapPeaTriangulation::~NSnapPeaTriangulation() {
    unlisten(this);
    delete[] shape_;
    delete[] cusp_;
    regina::snappea::free_triangulation(data_);
}

std::string NSnapPeaTriangulation::name() const {
    return (data_ ? get_triangulation_name(data_) : "");
}

NSnapPeaTriangulation::SolutionType NSnapPeaTriangulation::solutionType()
        const {
    if (! data_)
        return NSnapPeaTriangulation::not_attempted;
    return static_cast<SolutionType>(
        regina::snappea::get_filled_solution_type(data_));
}

double NSnapPeaTriangulation::volume() const {
    if (! data_)
        return 0;
    return regina::snappea::volume(data_, 0);
}

double NSnapPeaTriangulation::volume(int& precision) const {
    if (! data_)
        return 0;
    return regina::snappea::volume(data_, &precision);
}

NSnapPeaTriangulation* NSnapPeaTriangulation::canonize() const {
    if (! data_)
        return 0;

    regina::snappea::Triangulation* tmp;
    regina::snappea::copy_triangulation(data_, &tmp);

    if (regina::snappea::canonize(tmp) != regina::snappea::func_OK) {
        regina::snappea::free_triangulation(tmp);
        return 0;
    }

    NSnapPeaTriangulation* ans = new NSnapPeaTriangulation();
    ans->setPacketLabel(get_triangulation_name(data_));
    ans->reset(tmp);
    return ans;
}

void NSnapPeaTriangulation::randomize() {
    if (! data_)
        return;

    regina::snappea::randomize_triangulation(data_);
    regina::snappea::find_complete_hyperbolic_structure(data_);
    regina::snappea::do_Dehn_filling(data_);

    sync();
}

/**
 * Written by William Pettersson, 2011.
 */
NMatrixInt* NSnapPeaTriangulation::slopeEquations() const {
    int i,j;
    if (! data_)
        return 0;

    NMatrixInt* matrix =
        new NMatrixInt(2*data_->num_cusps, 3*data_->num_tetrahedra);
    for(i=0; i< data_->num_cusps; i++) {
        int numRows;
        // SnapPea returns "a b c" for each tetrahedron, where the
        // derivative of the holonomy of meridians and longitudes is given as
        //   a log (z_0) + b log ( 1/(1-z_0)) + c log ((z_0 - 1)/z_0) + ... = 0
        //
        // The equation for slopes in terms of quads of types q, q' and q''
        // becomes
        //   nu = (b-c)q + (c-a)q' + (a-b)q''
        //
        // See Lemma 4.2 in "Degenerations of ideal hyperbolic triangulations",
        // Stephan Tillmann, Mathematische Zeitschrift,
        // DOI: 10.1007/s00209-011-0958-8.
        //   
        int *equations =
            regina::snappea::get_cusp_equation(data_, i, 1, 0, &numRows);
        for(j=0; j< data_->num_tetrahedra; j++) {
            matrix->entry(2*i,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix->entry(2*i,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix->entry(2*i,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        regina::snappea::free_cusp_equation(equations);
        equations =
            regina::snappea::get_cusp_equation(data_, i, 0, 1, &numRows);
        for(j=0; j< data_->num_tetrahedra; j++) {
            matrix->entry(2*i+1,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix->entry(2*i+1,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix->entry(2*i+1,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        regina::snappea::free_cusp_equation(equations);
    }
    return matrix;
}

/**
 * Written by William Pettersson, 2011.
 */
bool NSnapPeaTriangulation::verifyTriangulation(const NTriangulation& tri)
        const {
    if (! data_)
        return false;

    regina::snappea::TriangulationData *tData;
    regina::snappea::triangulation_to_data(data_, &tData);

    int tet, face, i;
    if (tData->num_tetrahedra != tri.getNumberOfTetrahedra()) {
        free_triangulation_data(tData);
        return false;
    }

    NTriangulation::TetrahedronIterator it = tri.getTetrahedra().begin();
    for (tet = 0; tet < tData->num_tetrahedra; tet++) {
        for (face = 0; face < 4; face++) {
            if (tData->tetrahedron_data[tet].neighbor_index[face] !=
                    tri.tetrahedronIndex((*it)->adjacentTetrahedron(face))) {
                free_triangulation_data(tData);
                return false;
            }
            for (i = 0; i < 4; i++)
                if (tData->tetrahedron_data[tet].gluing[face][i] !=
                        (*it)->adjacentGluing(face)[i]) {
                    free_triangulation_data(tData);
                    return false;
                }
        }
        it++;
    }

    free_triangulation_data(tData);
    return true;
}

void NSnapPeaTriangulation::writeTextShort(std::ostream& out) const {
    if (data_) {
        out << "SnapPea triangulation with " << data_->num_tetrahedra
            << " tetrahedra";
    } else {
        out << "Null SnapPea triangulation";
    }
}

void NSnapPeaTriangulation::writeTextLong(std::ostream& out) const {
    if (! data_) {
        out << "Null SnapPea triangulation" << std::endl;
        return;
    }

    NTriangulation::writeTextLong(out);

    if (shape_) {
        out << "Tetrahedron shapes:" << std::endl;
        for (unsigned i = 0; i < getNumberOfTetrahedra(); ++i)
            out << "  " << i << ": ( " << shape_[i].real()
                << ", " << shape_[i].imag() << " )" << std::endl;
    } else
        out << "No tetrahedron shapes stored." << std::endl;
}

bool NSnapPeaTriangulation::kernelMessagesEnabled() {
    NMutex::MutexLock ml(snapMutex);
    return kernelMessages_;
}

void NSnapPeaTriangulation::enableKernelMessages(bool enabled) {
    NMutex::MutexLock ml(snapMutex);
    kernelMessages_ = enabled;
}

void NSnapPeaTriangulation::disableKernelMessages() {
    NMutex::MutexLock ml(snapMutex);
    kernelMessages_ = false;
}

std::string NSnapPeaTriangulation::snapPea() const {
    if (! data_)
        return std::string();

    char* file = regina::snappea::string_triangulation(data_);
    std::string ans(file);
    free(file);
    return ans;
}

void NSnapPeaTriangulation::snapPea(std::ostream& out) const {
    if (! data_)
        return;

    char* file = regina::snappea::string_triangulation(data_);
    out << file;
    free(file);
}

bool NSnapPeaTriangulation::saveSnapPea(const char* filename) const {
    if (! (data_ && filename && *filename))
        return false;
    return regina::snappea::write_triangulation(data_, filename);
}

void NSnapPeaTriangulation::dump() const {
    if (! data_)
        return;

    char* file = regina::snappea::string_triangulation(data_);
    printf("%s", file);
    fflush(stdout);
    free(file);
}

void NSnapPeaTriangulation::saveAsSnapPea(const char* filename) const {
    if (data_)
        regina::snappea::write_triangulation(data_, filename);
}

void NSnapPeaTriangulation::writeXMLPacketData(std::ostream& out) const {
    if (! data_)
        return;

    out << "  <snappea>" << regina::xml::xmlEncodeSpecialChars(snapPea())
        << "</snappea>\n";
}

NTriangulation* NSnapPeaTriangulation::toRegina() const {
    if (data_) {
        NTriangulation* ans = new NTriangulation(*this);
        ans->setPacketLabel(get_triangulation_name(data_));
        return ans;
    } else
        return 0;
}

void NSnapPeaTriangulation::packetWasChanged(NPacket* packet) {
    // If the triangulation is changed "illegitimately", via the
    // inherited NTriangulation interface, then convert this to a null
    // triangulation.
    if (packet == this && data_ && ! syncing_)
        reset(0);
}

void NSnapPeaTriangulation::sync() {
    // TODO: Check first whether anything has changed, and only resync
    // the NTriangulation data if it has.
    syncing_ = true;
    {
        ChangeEventSpan span(this);

        if (getNumberOfTetrahedra())
            removeAllTetrahedra();
        delete[] shape_;
        delete[] cusp_;

        if (data_) {
            regina::snappea::TriangulationData* tData;
            regina::snappea::triangulation_to_data(data_, &tData);

            NTetrahedron** tet = new NTetrahedron*[tData->num_tetrahedra];

            int i, j;
            for (i = 0; i < tData->num_tetrahedra; ++i)
                tet[i] = newTetrahedron();

            for (i = 0; i < tData->num_tetrahedra; ++i)
                for (j = 0; j < 4; ++j)
                    if (! tet[i]->adjacentTetrahedron(j))
                        tet[i]->joinTo(j,
                            tet[tData->tetrahedron_data[i].neighbor_index[j]],
                            NPerm4(tData->tetrahedron_data[i].gluing[j]));

            regina::snappea::Tetrahedron* stet;
            if (solutionType() == not_attempted) {
                shape_ = 0;
            } else {
                // Fetch the shapes directly from SnapPea's internal
                // data structures, since SnapPea's get_tet_shape()
                // function is linear time (per tetrahedron).
                shape_ = new std::complex<double>[tData->num_tetrahedra];
                stet = data_->tet_list_begin.next;
                regina::snappea::ComplexWithLog* shape;
                for (i = 0; i < tData->num_tetrahedra; ++i) {
                    shape = &stet->shape[regina::snappea::filled]->
                            cwl[regina::snappea::ultimate][0 /* fixed */];
                    shape_[i] = std::complex<double>(
                        shape->rect.real, shape->rect.imag);
                    stet = stet->next;
                }
            }

            cusp_ = new CuspInfo[data_->num_cusps];
            regina::snappea::Cusp* c = data_->cusp_list_begin.next;
            for (i = 0; i < data_->num_cusps; ++i) {
                cusp_[c->index].complete = c->is_complete;
                cusp_[c->index].vertex = 0;
                c = c->next;
            }
            stet = data_->tet_list_begin.next;
            for (i = 0; i < tData->num_tetrahedra; ++i) {
                for (j = 0; j < 4; ++j) {
                    c = stet->cusp[j];
                    if (cusp_[c->index].vertex == 0)
                        cusp_[c->index].vertex = tet[i]->getVertex(j);
                }
                stet = stet->next;
            }

            delete[] tet;
            regina::snappea::free_triangulation_data(tData);
        } else {
            shape_ = 0;
            cusp_ = 0;
        }

        // The packet change event (which we are listening to) will be
        // fired at this point.
    }
    syncing_ = false;
}

void NSnapPeaTriangulation::reset(regina::snappea::Triangulation* data) {
    if (data_)
        regina::snappea::free_triangulation(data_);
    data_ = data;

    sync();
}

} // namespace regina

