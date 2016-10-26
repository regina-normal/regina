
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <climits>
#include <cstring>
#include <mutex>

#include "maths/matrix.h"
#include "maths/numbertheory.h"
#include "snappea/snappeatriangulation.h"
#include "snappea/kernel/kernel_prototypes.h"
#include "snappea/kernel/triangulation.h"
#include "snappea/kernel/unix_file_io.h"
#include "snappea/snappy/SnapPy.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    /**
     * A mutex to protect kernelMessages.
     */
    static std::mutex snapMutex;
}

std::complex<double> SnapPeaTriangulation::zero_(0, 0);

void Cusp::writeTextShort(std::ostream& out) const {
    if (complete())
        out << "Complete cusp: ";
    else
        out << "Filled cusp: ";

    out << "vertex " << vertex_->markedIndex();

    if (! complete())
        out << ", filling (" << m_ << ", " << l_ << ')';
}

SnapPeaTriangulation::SnapPeaTriangulation(
        const std::string& fileNameOrContents) :
        data_(0), shape_(0), cusp_(0), filledCusps_(0), syncing_(false) {
    try {
        if (startsWith(fileNameOrContents, "% Triangulation"))
            data_ = regina::snappea::read_triangulation_from_string(
                fileNameOrContents.c_str());
        else
            data_ = regina::snappea::read_triangulation(
                fileNameOrContents.c_str());

        if (data_) {
            setLabel(get_triangulation_name(data_));
            sync();
        }
    } catch (regina::SnapPeaFatalError& err) {
        data_ = 0;
    }
    listen(this);
}

SnapPeaTriangulation::SnapPeaTriangulation(const SnapPeaTriangulation& tri) :
        data_(0), shape_(0), cusp_(0), filledCusps_(0), syncing_(false) {
    if (tri.data_) {
        regina::snappea::copy_triangulation(tri.data_, &data_);
        sync();
    }
    listen(this);
}

SnapPeaTriangulation::SnapPeaTriangulation(const Triangulation<3>& tri, bool) :
        data_(0), shape_(0), cusp_(0), filledCusps_(0), syncing_(false) {
    const SnapPeaTriangulation* clone =
        dynamic_cast<const SnapPeaTriangulation*>(&tri);
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
    // own Triangulation<3> data structures.
    //
    // Make sure SnapPea is likely to be comfortable with it.
    if (tri.isEmpty() ||
            tri.hasBoundaryTriangles() ||
            (! tri.isConnected()) ||
            (! tri.isValid()) ||
            (! tri.isStandard())) {
        listen(this);
        return;
    }
    if (tri.size() >= INT_MAX) {
        listen(this);
        return;
    }

    // Looks good; go build the SnapPea triangulation.
    regina::snappea::TriangulationData tData;
    tData.name = strdup(tri.label().c_str());
    tData.num_tetrahedra = static_cast<int>(tri.size());

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
    Triangulation<3>::TetrahedronIterator it = tri.tetrahedra().begin();
    for (tet = 0; tet < tData.num_tetrahedra; tet++) {
        for (face = 0; face < 4; face++) {
            tData.tetrahedron_data[tet].neighbor_index[face] = static_cast<int>(
                (*it)->adjacentTetrahedron(face)->index());
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

    // SnapPea no longer removes finite vertices automatically - we need
    // to do it here ourselves.  Otherwise snappea will crash shortly
    // after when it tries to initialise the gluing equations.
    regina::snappea::remove_finite_vertices(data_);

    // Regina triangulations know nothing about peripheral curves.
    // Install a sensible basis for each cusp, if SnapPea will let us.
    //
    // Since we need a hyperbolic structure before we can install
    // (shortest, second shortest) bases, find one now.
    regina::snappea::find_complete_hyperbolic_structure(data_);

    // I believe there is no need to call do_Dehn_filling() in the case where
    // all cusps are complete, since find_complete_hyperbolic_structure()
    // already does this.
    // However, if we passed a closed manifold then SnapPea will have
    // automatically created a cusp with a filling:
    if (tri.isClosed())
        regina::snappea::do_Dehn_filling(data_);

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

SnapPeaTriangulation::~SnapPeaTriangulation() {
    unlisten(this);
    delete[] shape_;
    delete[] cusp_;
    regina::snappea::free_triangulation(data_);
}

std::string SnapPeaTriangulation::name() const {
    return (data_ ? get_triangulation_name(data_) : "");
}

SnapPeaTriangulation::SolutionType SnapPeaTriangulation::solutionType()
        const {
    if (! data_)
        return SnapPeaTriangulation::not_attempted;
    return static_cast<SolutionType>(
        regina::snappea::get_filled_solution_type(data_));
}

double SnapPeaTriangulation::volume() const {
    if (! data_)
        return 0;
    return regina::snappea::volume(data_, 0);
}

double SnapPeaTriangulation::volume(int& precision) const {
    if (! data_)
        return 0;
    return regina::snappea::volume(data_, &precision);
}

bool SnapPeaTriangulation::volumeZero() const {
    int precision;
    double vol = regina::snappea::volume(data_, &precision);

    // Here come the magic numbers.
    if (precision < 6)
        return false;
    if (fabs(vol) > 1e-7)
        return false;

    // Test whether |vol| < 1e-(precision+1).
    double epsilon = 1;
    for (int i = 0; i < precision + 1; ++i)
        epsilon /= 10;

    return (fabs(vol) < epsilon);
}

double SnapPeaTriangulation::minImaginaryShape() const {
    if (! shape_)
        return 0;

    // Since shape_ is non-zero, there is at least one tetrahedron.
    double ans = shape_[0].imag();
    for (unsigned i = 1; i < size(); ++i)
        if (ans > shape_[i].imag())
            ans = shape_[i].imag();

    return ans;
}

void SnapPeaTriangulation::unfill(unsigned whichCusp) {
    if (! data_)
        return;

    if (cusp_[whichCusp].complete()) {
        // Nothing to do.
        return;
    }

    regina::snappea::set_cusp_info(data_, whichCusp, true, 0, 0);

    // Update and refresh internal caches.
    cusp_[whichCusp].m_ = cusp_[whichCusp].l_ = 0;
    --filledCusps_;

    regina::snappea::do_Dehn_filling(data_);
    syncFillings();
}

bool SnapPeaTriangulation::fill(int m, int l, unsigned whichCusp) {
    if (! data_)
        return false;

    // Are we unfilling?
    if (m == 0 && l == 0) {
        unfill(whichCusp);
        return true;
    }

    // SnapPea expects reals as filling coefficients.
    //
    // Here we use the same test as SnapPea for whether these will be
    // treated as integers:
    regina::snappea::Real mReal = m;
    regina::snappea::Real lReal = l;
    if (m != (int)mReal || l != (int)lReal)
        return false;

    // Enforce other preconditions on the filling coefficients.
    if (cusp_[whichCusp].vertex_->isLinkOrientable()) {
        // Note that the gcd() below is Regina's.
        if (gcd(m, l) != 1)
            return false;
    } else {
        if (! (l == 0 && (m == 1 || m == -1)))
            return false;
    }

    // Are we filling a complete cusp, or changing an existing filling?
    bool wasComplete = cusp_[whichCusp].complete();

    // Do it.
    regina::snappea::set_cusp_info(data_, whichCusp, false, mReal, lReal);

    // Update and refresh internal caches.
    cusp_[whichCusp].m_ = m;
    cusp_[whichCusp].l_ = l;
    if (wasComplete)
        ++filledCusps_;

    regina::snappea::do_Dehn_filling(data_);
    syncFillings();
    return true;
}

Triangulation<3>* SnapPeaTriangulation::filledTriangulation(unsigned whichCusp)
        const {
    if (! data_)
        return 0;
    if (cusp_[whichCusp].complete())
        return 0;

    regina::snappea::Triangulation* t;
    unsigned nCusps = countCusps();
    if (nCusps == 1) {
        t = regina::snappea::fill_cusps(data_, 0, data_->name,
            1 /* fill_all_cusps = TRUE, which is 1 in SnapPea */);

        if (! t)
            return 0;
        Triangulation<3>* ans = new Triangulation<3>();
        fillRegina(t, *ans);
        return ans;
    } else {
        regina::snappea::Boolean* fill_cusp =
            new regina::snappea::Boolean[nCusps];
        std::fill(fill_cusp, fill_cusp + nCusps, 0);
        fill_cusp[whichCusp] = 1; /* TRUE in SnapPea */
        t = regina::snappea::fill_cusps(data_, fill_cusp, data_->name, 0);
        delete[] fill_cusp;

        SnapPeaTriangulation* ans = new SnapPeaTriangulation();
        if (t)
            ans->reset(t);
        return ans;
    }
}

Triangulation<3>* SnapPeaTriangulation::filledTriangulation() const {
    if (! data_)
        return 0;

    unsigned nCusps = countBoundaryComponents();
    regina::snappea::Triangulation* t;
    if (filledCusps_ == 0) {
        // Fill no cusps.
        return new SnapPeaTriangulation(*this);
    } else if (filledCusps_ == nCusps) {
        // Fill all cusps.
        t = regina::snappea::fill_cusps(data_, 0, data_->name,
            1 /* fill_all_cusps = TRUE, which is 1 in SnapPea */);

        if (! t)
            return 0;
        Triangulation<3>* ans = new Triangulation<3>();
        fillRegina(t, *ans);
        return ans;
    } else {
        // Fill some but not all cusps.
        regina::snappea::Boolean* fill_cusp =
            new regina::snappea::Boolean[nCusps];
        for (unsigned i = 0; i < nCusps; ++i)
            fill_cusp[i] = (cusp_[i].complete() ? 0 : 1 /* TRUE in SnapPea */);
        t = regina::snappea::fill_cusps(data_, fill_cusp, data_->name, 0);
        delete[] fill_cusp;

        if (! t)
            return 0;
        SnapPeaTriangulation* ans = new SnapPeaTriangulation();
        ans->reset(t);
        return ans;
    }
}

SnapPeaTriangulation* SnapPeaTriangulation::protoCanonize() const {
    if (! data_)
        return 0;

    regina::snappea::Triangulation* tmp;
    regina::snappea::copy_triangulation(data_, &tmp);

    if (regina::snappea::proto_canonize(tmp) != regina::snappea::func_OK) {
        regina::snappea::free_triangulation(tmp);
        return 0;
    }

    SnapPeaTriangulation* ans = new SnapPeaTriangulation();
    ans->setLabel(get_triangulation_name(data_));
    ans->reset(tmp);
    return ans;
}

Triangulation<3>* SnapPeaTriangulation::canonize() const {
    if (! data_)
        return 0;

    regina::snappea::Triangulation* tmp;
    regina::snappea::copy_triangulation(data_, &tmp);

    if (regina::snappea::canonize(tmp) != regina::snappea::func_OK) {
        regina::snappea::free_triangulation(tmp);
        return 0;
    }

    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel(get_triangulation_name(data_));
    fillRegina(tmp, *ans);
    regina::snappea::free_triangulation(tmp);
    return ans;
}

void SnapPeaTriangulation::randomize() {
    if (! data_)
        return;

    regina::snappea::randomize_triangulation(data_);
    sync();
}

MatrixInt* SnapPeaTriangulation::gluingEquations() const {
    if (! data_)
        return 0;

    MatrixInt* matrix = new MatrixInt(
        countEdges() + data_->num_cusps + countCompleteCusps(),
        3 * size());

    int numRows, numCols;
    int row, j;

    int** edgeEqns = regina::snappea::get_gluing_equations(data_,
        &numRows, &numCols);
    for (row = 0; row < numRows; ++row)
        for (j = 0; j < numCols; ++j)
            matrix->entry(row, j) = edgeEqns[row][j];
    regina::snappea::free_gluing_equations(edgeEqns, numRows);

    int c;
    int* cuspEqn;
    for (c = 0; c < data_->num_cusps; ++c) {
        if (cusp_[c].complete()) {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 1, 0, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix->entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;

            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 0, 1, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix->entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        } else {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, cusp_[c].m_, cusp_[c].l_, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix->entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        }
    }

    return matrix;
}

MatrixInt* SnapPeaTriangulation::gluingEquationsRect() const {
    if (! data_)
        return 0;

    unsigned n = size();

    MatrixInt* matrix = new MatrixInt(
        countEdges() + data_->num_cusps + countCompleteCusps(),
        2 * n + 1);
    // Note: all entries are automatically initialised to zero.

    int numRows, numCols;
    int row, j;
    int parity;

    int** edgeEqns = regina::snappea::get_gluing_equations(data_,
        &numRows, &numCols);
    for (row = 0; row < numRows; ++row) {
        parity = 0;
        for (j = 0; j < n; ++j) {
            matrix->entry(row, j) += edgeEqns[row][3 * j];
            matrix->entry(row, j + n) -= edgeEqns[row][3 * j + 1];
            matrix->entry(row, j) -= edgeEqns[row][3 * j + 2];
            matrix->entry(row, j + n) += edgeEqns[row][3 * j + 2];
            if (edgeEqns[row][3 * j + 2] % 2)
                parity ^= 1;
        }
        matrix->entry(row, 2 * n) = (parity ? -1 : 1);
    }
    regina::snappea::free_gluing_equations(edgeEqns, numRows);

    int c;
    int* cuspEqn;
    for (c = 0; c < data_->num_cusps; ++c) {
        if (cusp_[c].complete()) {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 1, 0, &numCols);
            parity = 0;
            for (j = 0; j < n; ++j) {
                matrix->entry(row, j) += cuspEqn[3 * j];
                matrix->entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix->entry(row, j) -= cuspEqn[3 * j + 2];
                matrix->entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix->entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;

            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 0, 1, &numCols);
            parity = 0;
            for (j = 0; j < n; ++j) {
                matrix->entry(row, j) += cuspEqn[3 * j];
                matrix->entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix->entry(row, j) -= cuspEqn[3 * j + 2];
                matrix->entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix->entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        } else {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, cusp_[c].m_, cusp_[c].l_, &numCols);
            parity = 0;
            for (j = 0; j < n; ++j) {
                matrix->entry(row, j) += cuspEqn[3 * j];
                matrix->entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix->entry(row, j) -= cuspEqn[3 * j + 2];
                matrix->entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix->entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        }
    }

    return matrix;
}

/**
 * Written by William Pettersson, 2011.
 */
MatrixInt* SnapPeaTriangulation::slopeEquations() const {
    if (! data_)
        return 0;

    MatrixInt* matrix =
        new MatrixInt(2*data_->num_cusps, 3*data_->num_tetrahedra);
    int i,j;
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

void SnapPeaTriangulation::writeTextShort(std::ostream& out) const {
    if (data_) {
        out << "SnapPea triangulation with " << data_->num_tetrahedra
            << " tetrahedra";
    } else {
        out << "Null SnapPea triangulation";
    }
}

void SnapPeaTriangulation::writeTextLong(std::ostream& out) const {
    if (! data_) {
        out << "Null SnapPea triangulation" << std::endl;
        return;
    }

    Triangulation<3>::writeTextLong(out);

    unsigned i;
    if (shape_) {
        out << "Tetrahedron shapes:" << std::endl;
        for (i = 0; i < size(); ++i)
            out << "  " << i << ": ( " << shape_[i].real()
                << ", " << shape_[i].imag() << " )" << std::endl;
    } else
        out << "No tetrahedron shapes stored." << std::endl;

    out << std::endl;

    out << "Cusps:" << std::endl;
    for (i = 0; i < countBoundaryComponents(); ++i) {
        out << "  " << i
            << ": Vertex " << cusp_[i].vertex_->markedIndex();
        if (cusp_[i].complete())
            out << ", complete";
        else
            out << ", filled (" << cusp_[i].m_ << ", " << cusp_[i].l_ << ')';
        out << std::endl;
    }
}

bool SnapPeaTriangulation::kernelMessagesEnabled() {
    std::lock_guard<std::mutex> ml(snapMutex);
    return kernelMessages_;
}

void SnapPeaTriangulation::enableKernelMessages(bool enabled) {
    std::lock_guard<std::mutex> ml(snapMutex);
    kernelMessages_ = enabled;
}

void SnapPeaTriangulation::disableKernelMessages() {
    std::lock_guard<std::mutex> ml(snapMutex);
    kernelMessages_ = false;
}

std::string SnapPeaTriangulation::snapPea() const {
    if (! data_)
        return std::string();

    char* file = regina::snappea::string_triangulation(data_);
    std::string ans(file);
    free(file);
    return ans;
}

void SnapPeaTriangulation::snapPea(std::ostream& out) const {
    if (! data_)
        return;

    char* file = regina::snappea::string_triangulation(data_);
    out << file;
    free(file);
}

bool SnapPeaTriangulation::saveSnapPea(const char* filename) const {
    if (! (data_ && filename && *filename))
        return false;
    return regina::snappea::write_triangulation(data_, filename);
}

void SnapPeaTriangulation::writeXMLPacketData(std::ostream& out) const {
    if (! data_)
        return;

    out << "  <snappea>" << regina::xml::xmlEncodeSpecialChars(snapPea())
        << "</snappea>\n";
}

void SnapPeaTriangulation::packetWasChanged(Packet* packet) {
    // If the triangulation is changed "illegitimately", via the
    // inherited Triangulation<3> interface, then convert this to a null
    // triangulation.
    if (packet == this && data_ && ! syncing_)
        reset(0);
}

void SnapPeaTriangulation::sync() {
    // TODO: Check first whether anything has changed, and only resync
    // the Triangulation<3> data if it has.
    syncing_ = true;
    {
        ChangeEventSpan span(this);

        // Deal with the combinatorial data and cusps first.
        if (! isEmpty())
            removeAllTetrahedra();

        delete[] cusp_;
        filledCusps_ = 0;

        if (data_) {
            fillRegina(data_, *this);

            if (regina::snappea::get_filled_solution_type(data_) ==
                    regina::snappea::not_attempted) {
                regina::snappea::find_complete_hyperbolic_structure(data_);
                regina::snappea::do_Dehn_filling(data_);
            }

            unsigned i, j;

            cusp_ = new Cusp[data_->num_cusps];
            regina::snappea::Cusp* c = data_->cusp_list_begin.next;
            for (i = 0; i < data_->num_cusps; ++i) {
                cusp_[c->index].vertex_ = 0;
                if (c->is_complete) {
                    cusp_[c->index].m_ = cusp_[c->index].l_ = 0;
                } else if (c->topology == regina::snappea::Klein_cusp &&
                        ! (regina::snappea::Dehn_coefficients_are_integers(c)
                            && c->l == 0 && (c->m == 1 || c->m == -1))) {
                    // Abort!  Make this a null triangulation.
                    // Note that reset() calls sync() again; this is
                    // harmless as long as we return immediately afterwards.
                    reset(0);
                    syncing_ = false;
                    return;
                } else if (c->topology == regina::snappea::torus_cusp &&
                        ! regina::snappea::Dehn_coefficients_are_relatively_prime_integers(c)) {
                    // Abort, as above.
                    reset(0);
                    syncing_ = false;
                    return;
                } else {
                    cusp_[c->index].m_ = c->m;
                    cusp_[c->index].l_ = c->l;
                    ++filledCusps_;
                }
                c = c->next;
            }
            regina::snappea::Tetrahedron* stet = data_->tet_list_begin.next;
            for (i = 0; i < size(); ++i) {
                for (j = 0; j < 4; ++j) {
                    c = stet->cusp[j];
                    if (cusp_[c->index].vertex_ == 0)
                        cusp_[c->index].vertex_ = tetrahedron(i)->vertex(j);
                }
                stet = stet->next;
            }
        } else {
            cusp_ = 0;
        }

        // Next, update all data that depend on the fillings (if any).
        // Most importantly, this includes the cache of tetrahedron shapes.
        fillingsHaveChanged();

        // The packet change event (which we are listening to) will be
        // fired at this point.
    }
    syncing_ = false;
}

void SnapPeaTriangulation::syncFillings() {
    syncing_ = true;
    {
        ChangeEventSpan span(this);
        fillingsHaveChanged();
        // The packet change event will be fired at this point.
    }
    syncing_ = false;
}

void SnapPeaTriangulation::fillingsHaveChanged() {
    // Clear properties that depend on the fillings.
    fundGroupFilled_.clear();
    h1Filled_.clear();

    delete[] shape_;

    if (data_) {
        // Refresh the array of tetrahedron shapes.
        regina::snappea::Tetrahedron* stet;
        SolutionType soln = static_cast<SolutionType>(
            regina::snappea::get_filled_solution_type(data_));
        if (soln == not_attempted || soln == no_solution) {
            shape_ = 0;
        } else {
            // Fetch the shapes directly from SnapPea's internal
            // data structures, since SnapPea's get_tet_shape()
            // function is linear time (per tetrahedron).
            shape_ = new std::complex<double>[size()];
            stet = data_->tet_list_begin.next;
            regina::snappea::ComplexWithLog* shape;
            for (unsigned i = 0; i < size(); ++i) {
                shape = &stet->shape[regina::snappea::filled]->
                        cwl[regina::snappea::ultimate][0 /* fixed */];
                shape_[i] = std::complex<double>(
                    shape->rect.real, shape->rect.imag);
                stet = stet->next;
            }
        }
    } else {
        shape_ = 0;
    }
}

void SnapPeaTriangulation::fillRegina(regina::snappea::Triangulation* src,
        Triangulation<3>& dest) {
    ChangeEventSpan span(&dest);

    regina::snappea::TriangulationData* tData;
    regina::snappea::triangulation_to_data(src, &tData);

    Tetrahedron<3>** tet = new Tetrahedron<3>*[tData->num_tetrahedra];

    int i, j;
    for (i = 0; i < tData->num_tetrahedra; ++i)
        tet[i] = dest.newTetrahedron();

    for (i = 0; i < tData->num_tetrahedra; ++i)
        for (j = 0; j < 4; ++j)
            if (! tet[i]->adjacentTetrahedron(j))
                tet[i]->join(j,
                    tet[tData->tetrahedron_data[i].neighbor_index[j]],
                    Perm<4>(tData->tetrahedron_data[i].gluing[j]));

    delete[] tet;
    regina::snappea::free_triangulation_data(tData);
}

void SnapPeaTriangulation::reset(regina::snappea::Triangulation* data) {
    if (data_)
        regina::snappea::free_triangulation(data_);
    data_ = data;

    sync();
}

} // namespace regina

