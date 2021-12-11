
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "link/link.h"
#include "maths/matrix.h"
#include "maths/numbertheory.h"
#include "snappea/snappeatriangulation.h"
#include "snappea/kernel/kernel_prototypes.h"
#include "snappea/kernel/link_projection.h"
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

    /**
     * The generic name to give SnapPea triangulations when coming from a
     * Triangulation<3> (which, as of Regina 7.0, no longer has a packet label).
     *
     * This is non-const because we store it in TriangulationData::name
     * in-place, without performing deep copies.
     */
    char genericName[] = "Regina";

    /**
     * Converts a single gluing from snappea::TetrahedronData::gluing
     * into one of Regina's permutations.
     */
    Perm<4> snappeaGluing(const int* gluing) {
        return Perm<4>(gluing[0], gluing[1], gluing[2], gluing[3]);
    }

    /**
     * Returns the SnapPea KLPStrandType corresponding to the given strand
     * in one of Regina's native links.
     */
    inline regina::snappea::KLPStrandType klpStrand(const StrandRef& s) {
        if (s.crossing()->sign() > 0)
            return (s.strand() == 0 ?
                regina::snappea::KLPStrandY : regina::snappea::KLPStrandX);
        else
            return (s.strand() == 0 ?
                regina::snappea::KLPStrandX : regina::snappea::KLPStrandY);
    }
}

void Cusp::writeTextShort(std::ostream& out) const {
    if (complete())
        out << "Complete";
    else
        out << '(' << m_ << ',' << l_ << ")-filled";

    out << " cusp at vertex " << vertex_->markedIndex();
}

SnapPeaTriangulation::SnapPeaTriangulation(
        const std::string& fileNameOrContents) :
        data_(nullptr), shape_(nullptr), cusp_(nullptr), filledCusps_(0) {
    bool isContents = startsWith(fileNameOrContents, "% Triangulation");
    try {
        if (isContents)
            data_ = regina::snappea::read_triangulation_from_string(
                fileNameOrContents.c_str());
        else
            data_ = regina::snappea::read_triangulation(
                fileNameOrContents.c_str());
    } catch (regina::SnapPeaFatalError& err) {
        // data_ will be left as null.
    }

    if (! data_) {
        if (isContents)
            throw regina::FileError("The SnapPea kernel could not "
                "parse the given file contents");
        else
            throw regina::FileError("The SnapPea kernel could not open the "
                "given file, and/or could not parse its contents");
    }

    sync();
    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
}

// We don't call the Triangulation<3> copy constructor, since sync()
// will take care of that work for us.
// NOLINTNEXTLINE(bugprone-copy-constructor-init)
SnapPeaTriangulation::SnapPeaTriangulation(const SnapPeaTriangulation& tri) :
        data_(nullptr), shape_(nullptr), cusp_(nullptr), filledCusps_(0) {
    if (tri.data_) {
        regina::snappea::copy_triangulation(tri.data_, &data_);
        sync();
    }
    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;

    // Do not copy reginaChangeEventSpans_; instead use the default
    // initialisation to zero.
}

SnapPeaTriangulation::SnapPeaTriangulation(SnapPeaTriangulation&& src)
        noexcept :
        Triangulation<3>(std::move(src)),
        data_(src.data_),
        shape_(src.shape_),
        cusp_(src.cusp_),
        filledCusps_(src.filledCusps_),
        fundGroupFilled_(std::move(src.fundGroupFilled_)),
        h1Filled_(std::move(src.h1Filled_)) {
    src.data_ = nullptr;
    src.shape_ = nullptr;
    src.cusp_ = nullptr;

    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;

    // Do not copy reginaChangeEventSpans_; instead use the default
    // initialisation to zero.
}

SnapPeaTriangulation& SnapPeaTriangulation::operator = (
        SnapPeaTriangulation&& src) {
    ChangeAndSyncSpan span(*this);

    Triangulation<3>::operator = (std::move(src));

    // We have already moved out of src, but this only touches the
    // Triangulation<3> data.
    // NOLINTNEXTLINE(bugprone-use-after-move)
    std::swap(data_, src.data_);
    std::swap(shape_, src.shape_);
    std::swap(cusp_, src.cusp_);

    filledCusps_ = src.filledCusps_;
    fundGroupFilled_ = std::move(src.fundGroupFilled_);
    h1Filled_ = std::move(src.h1Filled_);

    // The assignment operator should not touch reginaChangeEventSpans_.

    // Let src dispose of the original data_, shape_ and cusp_ in its
    // own destructor.
    return *this;
}

SnapPeaTriangulation& SnapPeaTriangulation::operator = (
        const SnapPeaTriangulation& src) {
    if (std::addressof(src) == this)
        return *this;

    ChangeAndSyncSpan span(*this);

    regina::snappea::free_triangulation(data_);
    if (src.data_)
        regina::snappea::copy_triangulation(src.data_, &data_);
    else
        data_ = nullptr;

    sync(); // fixes everything else

    // The assignment operator should not touch reginaChangeEventSpans_.

    return *this;
}

SnapPeaTriangulation::SnapPeaTriangulation(const Triangulation<3>& tri, bool) :
        data_(nullptr), shape_(nullptr), cusp_(nullptr), filledCusps_(0) {
    if (const SnapPeaTriangulation* clone = tri.isSnapPea()) {
        // We have a full SnapPea triangulation to clone.
        if (clone->data_) {
            regina::snappea::copy_triangulation(clone->data_, &data_);
            sync();
        }
        Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
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
        Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
        return;
    }
    if (tri.size() >= INT_MAX) {
        Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
        return;
    }

    // Looks good; go build the SnapPea triangulation.
    regina::snappea::TriangulationData tData;
    tData.name = genericName;
    tData.num_tetrahedra = static_cast<int>(tri.size());

    // Fields recalculated by SnapPea:
    tData.solution_type = regina::snappea::not_attempted;
    tData.volume = 0;
    tData.orientability = regina::snappea::unknown_orientability;
    tData.CS_value_is_known = false;
    tData.CS_value = 0;
    tData.num_or_cusps = 0;
    tData.num_nonor_cusps = 0;
    tData.cusp_data = nullptr;

    tData.tetrahedron_data = new regina::snappea::TetrahedronData[
        tData.num_tetrahedra];
    int tet, face, i, j, k, l;
    auto it = tri.tetrahedra().begin();
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

        ++it;
    }

    regina::snappea::data_to_triangulation(&tData, &data_);

    delete[] tData.tetrahedron_data;

    if (! data_) {
        Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
        return;
    }

    // SnapPea no longer removes finite vertices automatically - we need
    // to do it here ourselves.  Otherwise snappea will crash shortly
    // after when it tries to initialise the gluing equations.
    if (tri.countVertices() > tri.countBoundaryComponents())
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

    auto soln = static_cast<SolutionType>(
        regina::snappea::get_filled_solution_type(data_));
    if (soln == geometric_solution || soln == nongeometric_solution) {
        try {
            regina::snappea::install_shortest_bases(data_);
        } catch (regina::SnapPeaFatalError& err) {
            // Blurgh.  SnapPea says no.
        }
    }

    sync();

    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
}

SnapPeaTriangulation::SnapPeaTriangulation(const Link& link) :
        data_(nullptr), shape_(nullptr), cusp_(nullptr), filledCusps_(0) {
    if (link.isEmpty())
        throw InvalidArgument("The SnapPeaTriangulation constructor "
            "requires a non-empty link");

    // In SnapPea's notation:
    // - For a positive crossing, the (x,y) strands are (over, under);
    // - For a negative crossing, the (x,y) strands are (under, over).

    regina::snappea::KLPProjection proj;
    proj.num_crossings = link.size();
    proj.num_free_loops = 0; // We will fix this later.
    proj.num_components = link.countComponents();

    proj.crossings = new regina::snappea::KLPCrossing[link.size()];
    for (Crossing* c : link.crossings()) {
        auto& raw = proj.crossings[c->index()];

        using regina::snappea::KLPStrandX;
        using regina::snappea::KLPStrandY;
        using regina::snappea::KLPForward;
        using regina::snappea::KLPBackward;

        if (c->sign() > 0) {
            raw.neighbor[KLPStrandX][KLPForward] =
                proj.crossings + c->next(1).crossing()->index();
            raw.neighbor[KLPStrandX][KLPBackward] =
                proj.crossings + c->prev(1).crossing()->index();

            raw.neighbor[KLPStrandY][KLPForward] =
                proj.crossings + c->next(0).crossing()->index();
            raw.neighbor[KLPStrandY][KLPBackward] =
                proj.crossings + c->prev(0).crossing()->index();

            raw.strand[KLPStrandX][KLPForward] = klpStrand(c->next(1));
            raw.strand[KLPStrandX][KLPBackward] = klpStrand(c->prev(1));

            raw.strand[KLPStrandY][KLPForward] = klpStrand(c->next(0));
            raw.strand[KLPStrandY][KLPBackward] = klpStrand(c->prev(0));

            raw.handedness = regina::snappea::KLPHalfTwistCL;
        } else {
            raw.neighbor[KLPStrandX][KLPForward] =
                proj.crossings + c->next(0).crossing()->index();
            raw.neighbor[KLPStrandX][KLPBackward] =
                proj.crossings + c->prev(0).crossing()->index();

            raw.neighbor[KLPStrandY][KLPForward] =
                proj.crossings + c->next(1).crossing()->index();
            raw.neighbor[KLPStrandY][KLPBackward] =
                proj.crossings + c->prev(1).crossing()->index();

            raw.strand[KLPStrandX][KLPForward] = klpStrand(c->next(0));
            raw.strand[KLPStrandX][KLPBackward] = klpStrand(c->prev(0));

            raw.strand[KLPStrandY][KLPForward] = klpStrand(c->next(1));
            raw.strand[KLPStrandY][KLPBackward] = klpStrand(c->prev(1));

            raw.handedness = regina::snappea::KLPHalfTwistCCL;
        }
    }

    size_t compIndex = 0;
    for (const StrandRef& comp : link.components()) {
        if (comp) {
            StrandRef s = comp;
            do {
                if (s.crossing()->sign() > 0) {
                    if (s.strand() == 0)
                        proj.crossings[s.crossing()->index()].
                            component[regina::snappea::KLPStrandY] = compIndex;
                    else
                        proj.crossings[s.crossing()->index()].
                            component[regina::snappea::KLPStrandX] = compIndex;
                } else {
                    if (s.strand() == 0)
                        proj.crossings[s.crossing()->index()].
                            component[regina::snappea::KLPStrandX] = compIndex;
                    else
                        proj.crossings[s.crossing()->index()].
                            component[regina::snappea::KLPStrandY] = compIndex;
                }
                ++s;
            } while (s != comp);

            ++compIndex;
        } else
            ++proj.num_free_loops;
    }

    data_ = regina::snappea::triangulate_link_complement(std::addressof(proj),
        1 /* remove finite vertices */);
    delete[] proj.crossings;

    if (! data_) {
        Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
        return;
    }

    // We need to give the SnapPea triangulation a name, since
    // triangulate_link_complement() initialises data_->name to null,
    // and this will crash fill_cusps() (which tries to copy the name).
    data_->name = ::strdup("Link");

    // Since the other SnapPeaTriangulation constructors find a hyperbolic
    // structure in order to choose peripheral curves, for consistency we will
    // make this constructor find a hyperbolic structure also (even though
    // our peripheral curves are already determined from the link diagram).
    regina::snappea::find_complete_hyperbolic_structure(data_);

    sync();
    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
}

SnapPeaTriangulation::~SnapPeaTriangulation() {
    delete[] shape_;
    delete[] cusp_;
    regina::snappea::free_triangulation(data_);
}

void SnapPeaTriangulation::swap(SnapPeaTriangulation& other) {
    if (this == &other)
        return;

    ChangeAndSyncSpan span1(*this);
    ChangeAndSyncSpan span2(other);

    Triangulation<3>::swap(other);
    std::swap(data_, other.data_);
    std::swap(shape_, other.shape_);
    std::swap(cusp_, other.cusp_);
    std::swap(filledCusps_, other.filledCusps_);
    fundGroupFilled_.swap(other.fundGroupFilled_);
    h1Filled_.swap(other.h1Filled_);
}

void SnapPeaTriangulation::nullify() {
    if (! data_)
        return;

    ChangeAndSyncSpan span(*this);

    regina::snappea::free_triangulation(data_);
    data_ = nullptr;
    sync();
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
        throw SnapPeaIsNull("SnapPeaTriangulation::volume");
    return regina::snappea::volume(data_, nullptr);
}

std::pair<double, int> SnapPeaTriangulation::volumeWithPrecision() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::volumeWithPrecision");
    int precision;
    double volume = regina::snappea::volume(data_, &precision);
    return { volume, precision };
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

bool SnapPeaTriangulation::operator == (const SnapPeaTriangulation& other)
        const {
    if (! data_)
        return ! other.data_;
    if (! other.data_)
        return false;

    // Neither triangulation is null.
    if (! (Triangulation<3>::operator == (other)))
        return false;

    // This next test *should* be unnecessary.
    return std::equal(cusp_, cusp_ + countCusps(), other.cusp_);
}

void SnapPeaTriangulation::unfill(unsigned whichCusp) {
    if (! data_)
        return;

    if (cusp_[whichCusp].complete()) {
        // Nothing to do.
        return;
    }

    // We change the SnapPea data structures but not the inherited
    // Triangulation<3>, and so a ChangeEventSpan is fine here.
    PacketData<SnapPeaTriangulation>::ChangeEventSpan span(*this);

    regina::snappea::set_cusp_info(data_, whichCusp, true, 0, 0);

    // Update and refresh internal caches.
    cusp_[whichCusp].m_ = cusp_[whichCusp].l_ = 0;
    --filledCusps_;

    regina::snappea::do_Dehn_filling(data_);
    fillingsHaveChanged();
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

    // We change the SnapPea data structures but not the inherited
    // Triangulation<3>, and so a ChangeEventSpan is fine here.
    PacketData<SnapPeaTriangulation>::ChangeEventSpan span(*this);

    regina::snappea::set_cusp_info(data_, whichCusp, false, mReal, lReal);

    // Update and refresh internal caches.
    cusp_[whichCusp].m_ = m;
    cusp_[whichCusp].l_ = l;
    if (wasComplete)
        ++filledCusps_;

    regina::snappea::do_Dehn_filling(data_);
    fillingsHaveChanged();
    return true;
}

SnapPeaTriangulation SnapPeaTriangulation::filledPartial(unsigned whichCusp)
        const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::filledPartial");
    if (cusp_[whichCusp].complete())
        throw FailedPrecondition("SnapPeaTriangulation::filledPartial() "
            "requires the given cusp to have filling coefficients");

    size_t nCusps = countCusps();

    if (nCusps == 1)
        throw FailedPrecondition(
            "SnapPeaTriangulation::filledPartial(unsigned) "
            "requires the manifold to have at least two cusps");

    // Note: fill_cusps never returns null.
    auto* fill_cusp = new regina::snappea::Boolean[nCusps];
    std::fill(fill_cusp, fill_cusp + nCusps, 0);
    fill_cusp[whichCusp] = 1; /* TRUE in SnapPea */
    regina::snappea::Triangulation* t = regina::snappea::fill_cusps(
        data_, fill_cusp, data_->name, 0);
    delete[] fill_cusp;

    return SnapPeaTriangulation(t);
}

SnapPeaTriangulation SnapPeaTriangulation::filledPartial() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::filledPartial");

    size_t nCusps = countBoundaryComponents();

    if (filledCusps_ == nCusps)
        throw FailedPrecondition("SnapPeaTriangulation::filledPartial() "
            "requires at least one cusp to have no filling coefficients");

    if (filledCusps_ == 0)
        return *this;

    // Note: fill_cusps never returns null.
    auto* fill_cusp = new regina::snappea::Boolean[nCusps];
    for (size_t i = 0; i < nCusps; ++i)
        fill_cusp[i] = (cusp_[i].complete() ? 0 : 1 /* TRUE in SnapPea */);
    regina::snappea::Triangulation* t = regina::snappea::fill_cusps(
        data_, fill_cusp, data_->name, 0);
    delete[] fill_cusp;

    return SnapPeaTriangulation(t);
}

Triangulation<3> SnapPeaTriangulation::filledAll() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::filledAll");

    size_t nCusps = countBoundaryComponents();

    if (filledCusps_ < nCusps)
        throw FailedPrecondition("SnapPeaTriangulation::filledAll() "
            "requires all cusps to have filling coefficients");

    // There should be at least one cusp, but just in case:
    if (filledCusps_ == 0)
        return *this;

    // Note: fill_cusps never returns null.
    regina::snappea::Triangulation* t = regina::snappea::fill_cusps(
        data_, nullptr, data_->name, 1 /* fill_all_cusps = TRUE */);

    Triangulation<3> ans;
    fillRegina(t, ans);
    regina::snappea::free_triangulation(t);
    return ans;
}

SnapPeaTriangulation SnapPeaTriangulation::protoCanonise() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::protoCanonise");

    regina::snappea::Triangulation* tmp;
    regina::snappea::copy_triangulation(data_, &tmp);

    if (regina::snappea::proto_canonize(tmp) != regina::snappea::func_OK) {
        regina::snappea::free_triangulation(tmp);
        throw UnsolvedCase("SnapPea was not able to triangulate the "
            "canonical cell decomposition");
    }

    return SnapPeaTriangulation(tmp);
}

Triangulation<3> SnapPeaTriangulation::canonise() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::canonise");

    regina::snappea::Triangulation* tmp;
    regina::snappea::copy_triangulation(data_, &tmp);

    if (regina::snappea::canonize(tmp) != regina::snappea::func_OK) {
        regina::snappea::free_triangulation(tmp);
        throw UnsolvedCase("SnapPea was not able to compute the "
            "canonical cell decomposition");
    }

    Triangulation<3> ans;
    fillRegina(tmp, ans);
    regina::snappea::free_triangulation(tmp);
    return ans;
}

void SnapPeaTriangulation::randomise() {
    if (! data_)
        return;

    ChangeAndSyncSpan span(*this);

    regina::snappea::randomize_triangulation(data_);
    sync();
}

MatrixInt SnapPeaTriangulation::gluingEquations() const {
    if (! data_)
        return MatrixInt(); // Should never happen, due to preconditions

    MatrixInt matrix(countEdges() + data_->num_cusps + countCompleteCusps(),
        3 * size());

    int numRows, numCols;
    int row, j;

    int** edgeEqns = regina::snappea::get_gluing_equations(data_,
        &numRows, &numCols);
    for (row = 0; row < numRows; ++row)
        for (j = 0; j < numCols; ++j)
            matrix.entry(row, j) = edgeEqns[row][j];
    regina::snappea::free_gluing_equations(edgeEqns, numRows);

    int c;
    int* cuspEqn;
    for (c = 0; c < data_->num_cusps; ++c) {
        if (cusp_[c].complete()) {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 1, 0, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix.entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;

            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 0, 1, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix.entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        } else {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, cusp_[c].m_, cusp_[c].l_, &numCols);
            for (j = 0; j < numCols; ++j)
                matrix.entry(row, j) = cuspEqn[j];
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        }
    }

    return matrix;
}

MatrixInt SnapPeaTriangulation::gluingEquationsRect() const {
    if (! data_)
        return MatrixInt(); // Should never happen, due to preconditions

    unsigned n = size();

    MatrixInt matrix(countEdges() + data_->num_cusps + countCompleteCusps(),
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
            matrix.entry(row, j) += edgeEqns[row][3 * j];
            matrix.entry(row, j + n) -= edgeEqns[row][3 * j + 1];
            matrix.entry(row, j) -= edgeEqns[row][3 * j + 2];
            matrix.entry(row, j + n) += edgeEqns[row][3 * j + 2];
            if (edgeEqns[row][3 * j + 2] % 2)
                parity ^= 1;
        }
        matrix.entry(row, 2 * n) = (parity ? -1 : 1);
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
                matrix.entry(row, j) += cuspEqn[3 * j];
                matrix.entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix.entry(row, j) -= cuspEqn[3 * j + 2];
                matrix.entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix.entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;

            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, 0, 1, &numCols);
            parity = 0;
            for (j = 0; j < n; ++j) {
                matrix.entry(row, j) += cuspEqn[3 * j];
                matrix.entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix.entry(row, j) -= cuspEqn[3 * j + 2];
                matrix.entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix.entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        } else {
            cuspEqn = regina::snappea::get_cusp_equation(
                data_, c, cusp_[c].m_, cusp_[c].l_, &numCols);
            parity = 0;
            for (j = 0; j < n; ++j) {
                matrix.entry(row, j) += cuspEqn[3 * j];
                matrix.entry(row, j + n) -= cuspEqn[3 * j + 1];
                matrix.entry(row, j) -= cuspEqn[3 * j + 2];
                matrix.entry(row, j + n) += cuspEqn[3 * j + 2];
                if (cuspEqn[3 * j + 2] % 2)
                    parity ^= 1;
            }
            matrix.entry(row, 2 * n) = (parity ? -1 : 1);
            regina::snappea::free_cusp_equation(cuspEqn);
            ++row;
        }
    }

    return matrix;
}

/**
 * Written by William Pettersson, 2011.
 */
MatrixInt SnapPeaTriangulation::slopeEquations() const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::slopeEquations");

    MatrixInt matrix(2*data_->num_cusps, 3*data_->num_tetrahedra);
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
            matrix.entry(2*i,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix.entry(2*i,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix.entry(2*i,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        regina::snappea::free_cusp_equation(equations);
        equations =
            regina::snappea::get_cusp_equation(data_, i, 0, 1, &numRows);
        for(j=0; j< data_->num_tetrahedra; j++) {
            matrix.entry(2*i+1,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix.entry(2*i+1,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix.entry(2*i+1,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        regina::snappea::free_cusp_equation(equations);
    }
    return matrix;
}

void SnapPeaTriangulation::writeTextShort(std::ostream& out) const {
    if (data_) {
        Triangulation<3>::writeTextShort(out);
        if (countBoundaryComponents() == 0)
            out << ", no cusps";
        else {
            out << ", cusps: [ ";
            bool first = true;
            for (const auto& c : cusps()) {
                if (first)
                    first = false;
                else
                    out << ", ";
                out << "vertex " << c.vertex_->markedIndex();
                if (! c.complete())
                    out << ": (" << c.m_ << ", " << c.l_ << ')';
            }
            out << " ]";
        }
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
        throw SnapPeaIsNull("SnapPeaTriangulation::snapPea");

    char* file = regina::snappea::string_triangulation(data_);
    std::string ans(file);
    free(file);
    return ans;
}

void SnapPeaTriangulation::snapPea(std::ostream& out) const {
    if (! data_)
        throw SnapPeaIsNull("SnapPeaTriangulation::snapPea");

    char* file = regina::snappea::string_triangulation(data_);
    out << file;
    free(file);
}

bool SnapPeaTriangulation::saveSnapPea(const char* filename) const {
    if (! (data_ && filename && *filename))
        return false;
    return regina::snappea::write_triangulation(data_, filename);
}

SnapPeaTriangulation::SnapPeaTriangulation(
        regina::snappea::Triangulation* data) :
        data_(data), shape_(nullptr), cusp_(nullptr), filledCusps_(0) {
    sync();
    Triangulation<3>::heldBy_ = HELD_BY_SNAPPEA;
}

void SnapPeaTriangulation::sync() {
    // TODO: Check first whether anything has changed, and only resync
    // the Triangulation<3> data if it has.
    {
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
                cusp_[c->index].vertex_ = nullptr;
                if (c->is_complete) {
                    cusp_[c->index].m_ = cusp_[c->index].l_ = 0;
                } else if (c->topology == regina::snappea::Klein_cusp &&
                        ! (regina::snappea::Dehn_coefficients_are_integers(c)
                            && c->l == 0 && (c->m == 1 || c->m == -1))) {
                    // Abort!  Make this a null triangulation.
                    // We call sync() again, but this is harmless as long as
                    // we return immediately afterwards.
                    regina::snappea::free_triangulation(data_);
                    data_ = nullptr;
                    sync();
                    return;
                } else if (c->topology == regina::snappea::torus_cusp &&
                        ! regina::snappea::Dehn_coefficients_are_relatively_prime_integers(c)) {
                    // Abort, as above.
                    regina::snappea::free_triangulation(data_);
                    data_ = nullptr;
                    sync();
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
                    if (cusp_[c->index].vertex_ == nullptr)
                        cusp_[c->index].vertex_ = tetrahedron(i)->vertex(j);
                }
                stet = stet->next;
            }
        } else {
            cusp_ = nullptr;
        }

        // Next, update all data that depend on the fillings (if any).
        // Most importantly, this includes the cache of tetrahedron shapes.
        fillingsHaveChanged();
    }
}

void SnapPeaTriangulation::fillingsHaveChanged() {
    // Clear properties that depend on the fillings.
    fundGroupFilled_.reset();
    h1Filled_.reset();

    delete[] shape_;

    if (data_) {
        // Refresh the array of tetrahedron shapes.
        regina::snappea::Tetrahedron* stet;
        auto soln = static_cast<SolutionType>(
            regina::snappea::get_filled_solution_type(data_));
        if (soln == not_attempted || soln == no_solution) {
            shape_ = nullptr;
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
        shape_ = nullptr;
    }
}

void SnapPeaTriangulation::fillRegina(regina::snappea::Triangulation* src,
        Triangulation<3>& dest) {
    // Ensure only one event pair is fired in this sequence of changes.
    Triangulation<3>::ChangeEventSpan span(dest);

    regina::snappea::TriangulationData* tData;
    regina::snappea::triangulation_to_data(src, &tData);

    auto* tet = new Tetrahedron<3>*[tData->num_tetrahedra];

    int i, j;
    for (i = 0; i < tData->num_tetrahedra; ++i)
        tet[i] = dest.newTetrahedron();

    for (i = 0; i < tData->num_tetrahedra; ++i)
        for (j = 0; j < 4; ++j)
            if (! tet[i]->adjacentTetrahedron(j))
                tet[i]->join(j,
                    tet[tData->tetrahedron_data[i].neighbor_index[j]],
                    snappeaGluing(tData->tetrahedron_data[i].gluing[j]));

    delete[] tet;
    regina::snappea::free_triangulation_data(tData);
}

size_t SnapPeaTriangulation::enumerateCoversInternal(int sheets,
        CoverEnumerationType type,
        std::function<void(SnapPeaTriangulation&&, CoverType)>&& action) const {
    if (! data_)
        return 0;

    regina::snappea::RepresentationList* reps =
        regina::snappea::find_representations(data_, sheets,
        static_cast<regina::snappea::PermutationSubgroup>(type));

    size_t ans = 0;
    for (auto rep = reps->list; rep; rep = rep->next) {
        action(SnapPeaTriangulation(regina::snappea::construct_cover(
                data_, rep, sheets)),
            static_cast<CoverType>(rep->covering_type));
        ++ans;
    }

    free_representation_list(reps);
    return ans;
}

} // namespace regina

