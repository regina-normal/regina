
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

/*! \file triangulation/generic/triangulation.h
 *  \brief Internal header for working with triangulations of arbitrary
 *  dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __TRIANGULATION_H
#ifndef __DOXYGEN
#define __TRIANGULATION_H
#endif

namespace regina {
    template <int> class XMLTriangulationReader;
}

#include "packet/packet.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include "utilities/xmlutils.h"
#include <type_traits>
#include "triangulation/detail/triangulation.h"

/**
 * \addtogroup generic Generic triangulations
 * Details for implementing triangulations in arbitrary dimensions.
 * @{
 */

namespace regina {

/**
 * A <i>dim</i>-dimensional triangulation, built by gluing together
 * <i>dim</i>-dimensional simplices along their (<i>dim</i>-1)-dimensional
 * facets.  Typically (but not necessarily) such triangulations are used
 * to represent <i>dim</i>-manifolds.
 *
 * Such triangulations are not the same as pure simplicial complexes, for two
 * reasons:
 *
 * - The only identifications that the user can explicitly specify are
 *   gluings between <i>dim</i>-dimensional simplices along their
 *   (<i>dim</i>-1)-dimensional facets.  All other identifications between
 *   <i>k</i>-faces (for any \a k) are simply consequences of these
 *   (<i>dim</i>-1)-dimensional gluings.  In contrast, a simplicial complex
 *   allows explicit gluings between faces of any dimension.
 *
 * - There is no requirement for a <i>k</i>-face to have (<i>k</i>+1) distinct
 *   vertices (so, for example, edges may be loops).  Many distinct
 *   <i>k</i>-faces of a top-dimensional simplex may be identified together
 *   as a consequence of the (<i>dim</i>-1)-dimensional gluings, and indeed
 *   we are even allowed to glue together two distinct factets of the same
 *   <i>dim</i>-simplex.  In contrast, a simplicial complex does not allow
 *   any of these situations.
 *
 * Amongst other things, this definition is general enough to capture
 * any reasonable definition of a <i>dim</i>-manifold triangulation.
 * However, there is no requirement that a triangulation must actually
 * represent a manifold (and indeed, testing this condition is undecidable
 * for sufficiently large \a dim).
 *
 * You can construct a triangulation from scratch using routines such as
 * newSimplex() and Simplex<dim>::join().  There are also routines for
 * importing and exporting triangulations in bulk, such as isoSig() and
 * fromIsoSig() (which uses <em>isomorphism signatures</em>), or
 * insertConstruction() and dumpConstruction() (which exports C++ code).
 *
 * In additional to top-dimensional simplices, this class also tracks:
 *
 * - connected components of the triangulation, as represented by the
 *   class Component<dim>;
 * - boundary components of the triangulation, as represented by the
 *   class BoundaryComponent<dim>;
 * - lower-dimensional faces of the triangulation, as represented by the
 *   classes Face<dim, subdim> for \a subdim = 0,...,(<i>dim</i>-1).
 *
 * Such objects are temporary: whenever the triangulation changes, they will
 * be deleted and rebuilt, and any pointers to them will become invalid.
 * Likewise, if the triangulation is deleted then all component objects
 * will be deleted alongside it.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers \e much more functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers (e.g.,
 * triangulation/dim2.h for \a dim = 2, or triangulation/dim3.h
 * for \a dim = 3).
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Triangulation2 and Triangulation3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim>
class Triangulation :
        public Packet,
        public detail::TriangulationBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Triangulation<dim> "
        "should not be used for Regina's standard dimensions.");

    private:
        /**
         * Provides implementation details for REGINA_PACKET_FROM.
         *
         * We do not provide the specialisation PacketInfo<typeID> in this
         * header, since this would force an instantiation of every class
         * Triangulation<dim> every time this header is included.  Instead,
         * PacketInfo<typeID> is specialised in packet/packetregistry-impl.h.
         *
         * As a result, we cannot use REGINA_PACKET (which requires
         * PacketInfo<typeID> for its implementation details).  Instead we
         * use REGINA_PACKET_FROM, which allows us to pass our own helper class
         * for the implementation.  PacketTypeInfo is that helper class.
         *
         * See the documentation for REGINA_PACKET_FROM for further details.
         */
        struct PacketTypeInfo {
            static constexpr const PacketType typeID = PacketType(100 + dim);
            static std::string name() {
                std::ostringstream s;
                s << dim << "-Manifold Triangulation";
                return s.str();
            }
        };

    REGINA_PACKET_FROM(Triangulation<dim>, Triangulation<dim>::PacketTypeInfo);

    protected:
        using detail::TriangulationBase<dim>::simplices_;

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
        Triangulation();
        /**
         * Creates a new copy of the given triangulation.
         * The packet tree structure and packet label are \e not copied.
         *
         * This will clone any computed properties (such as homology,
         * fundamental group, and so on) of the given triangulation also.
         * If you want a "clean" copy that resets all properties to unknown,
         * you can use the two-argument copy constructor instead.
         *
         * @param copy the triangulation to copy.
         */
        Triangulation(const Triangulation& copy);
        /**
         * Creates a new copy of the given triangulation, with the option
         * of whether or not to clone its computed properties also.
         *
         * @param copy the triangulation to copy.
         * @param cloneProps \c true if this should also clone any computed
         * properties of the given triangulation (such as homology,
         * fundamental group, and so on), or \c false if the new triangulation
         * should have all properties marked as unknown.
         */
        Triangulation(const Triangulation& copy, bool cloneProps);
        /**
         * Destroys this triangulation.
         *
         * The constituent simplices, the cellular structure and all other
         * properties will also be destroyed.
         */
        virtual ~Triangulation();

        /*@}*/
        /**
         * \name Packet Administration
         */
        /*@{*/

        virtual void writeTextShort(std::ostream& out) const override;
        virtual void writeTextLong(std::ostream& out) const override;
        virtual bool dependsOnParent() const override;

        /*@}*/
        /**
         * \name Simplices
         */
        /*@{*/

        /**
         * Swaps the contents of this and the given triangulation.
         *
         * All top-dimensional simplices that belong to this triangulation
         * will be moved to \a other, and all top-dimensional simplices
         * that belong to \a other will be moved to this triangulation.
         * Likewise, all skeletal objects (such as lower-dimensional faces,
         * components, and boundary components) and all cached properties
         * will be swapped.
         *
         * In particular, any pointers or references to Simplex<dim> and/or
         * Face<dim, subdim> objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this triangulation.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        void swap(Triangulation<dim>& other);
        /**
         * Deprecated routine that swaps the contents of this and the
         * given triangulation.
         *
         * \deprecated Use swap() instead.
         *
         * @param other the triangulation whose contents should be
         * swapped with this.
         */
        [[deprecated]] void swapContents(Triangulation<dim>& other);

        /*@}*/

        static XMLPacketReader* xmlReader(Packet* parent,
                XMLTreeResolver& resolver);

    protected:
        virtual Packet* internalClonePacket(Packet* parent) const override;
        virtual void writeXMLPacketData(std::ostream& out) const override;

    private:
        /**
         * Clears any calculated properties, including skeletal data,
         * and declares them all unknown.  This must be called by any
         * internal function that changes the triangulation.
         *
         * In most cases this routine is followed immediately by firing
         * a packet change event.
         */
        void clearAllProperties();

    friend class detail::SimplexBase<dim>;
    friend class detail::TriangulationBase<dim>;
};

/**
 * A function object used for sorting faces of triangulations by
 * increasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * increasing edge degree, you would use DegreeLessThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class DegreeLessThan {
    static_assert(standardDim(dim),
        "DegreeLessThan is only available for Regina's standard dimensions.");

    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeLessThan(const Triangulation<dim>& tri);
        /**
         * Creates a new clone of the given function object.
         */
        DegreeLessThan(const DegreeLessThan&) = default;
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * @param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has smaller degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeLessThan& operator = (const DegreeLessThan&) = delete;
};

/**
 * A function object used for sorting faces of triangulations by
 * decreasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * decreasing edge degree, you would use DegreeGreaterThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of Regina's \ref stddim "standard dimensions".
 * \pre \a subdim is between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class DegreeGreaterThan {
    static_assert(standardDim(dim),
        "DegreeGreaterThan is only available for Regina's standard dimensions.");

    private:
        const Triangulation<dim>& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const Triangulation<dim>& tri);
        /**
         * Creates a new clone of the given function object.
         */
        DegreeGreaterThan(const DegreeGreaterThan&) = default;
        /**
         * Compares the degrees of the <i>subdim</i>-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of <i>subdim</i>-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first <i>subdim</i>-dimensional face
         * within the triangulation.
         * @param b the index of the second <i>subdim</i>-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;

        // Make this class non-assignable, since \a tri_ is a reference.
        DegreeGreaterThan& operator = (const DegreeGreaterThan&) = delete;
};

/*@}*/

// Inline functions for Triangulation

template <int dim>
inline Triangulation<dim>::Triangulation() : detail::TriangulationBase<dim>() {
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy) :
        detail::TriangulationBase<dim>(copy) {
    // All properties to clone are held by TriangulationBase.
}

template <int dim>
inline Triangulation<dim>::Triangulation(const Triangulation& copy,
        bool cloneProps) : detail::TriangulationBase<dim>(copy, cloneProps) {
    // All properties to clone are held by TriangulationBase.
}

template <int dim>
inline Triangulation<dim>::~Triangulation() {
    clearAllProperties();
}

template <int dim>
inline void Triangulation<dim>::clearAllProperties() {
    detail::TriangulationBase<dim>::clearBaseProperties();
}

template <int dim>
void Triangulation<dim>::swap(Triangulation<dim>& other) {
    if (&other == this)
        return;

    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&other);

    this->swapBaseData(other);
}

template <int dim>
inline void Triangulation<dim>::swapContents(Triangulation<dim>& other) {
    swap(other);
}

template <int dim>
inline void Triangulation<dim>::writeTextShort(std::ostream& out) const {
    if (simplices_.size() == 0)
        out << "Empty " << dim << "-dimensional triangulation";
    else
        out << "Triangulation with " << simplices_.size() << ' ' << dim << '-'
            << (simplices_.size() == 1 ? "simplex" : "simplices");
}

template <int dim>
void Triangulation<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << "\n\n";

    out << "f-vector: ";
    int i;
    {
        std::vector<size_t> f = detail::TriangulationBase<dim>::fVector();
        for (i = 0; i < dim; ++i)
            out << f[i] << ", ";
        out << f[dim] << "\n\n";
    }

    Simplex<dim>* simp;
    Simplex<dim>* adj;
    size_t pos;
    int j;
    Perm<dim+1> gluing;

    out << "  Simplex  |  glued to:";
    for (i = dim; i >= 0; --i) {
        out << "     (";
        for (j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    out << "  ---------+-----------";
    for (i = dim; i >= 0; --i)
        for (j = 0; j < 7 + dim; ++j)
            out << '-';
    out << '\n';
    for (pos=0; pos < simplices_.size(); pos++) {
        simp = simplices_[pos];
        out << "     " << std::setw(4) << pos << "  |           ";
        for (i = dim; i >= 0; --i) {
            adj = simp->adjacentSimplex(i);
            if (! adj) {
                for (j = 0; j < dim - 1; ++j)
                    out << ' ';
                out << "boundary";
            } else {
                gluing = simp->adjacentGluing(i);
                out << std::setw(4) << adj->index() << " (";
                for (j = 0; j <= dim; ++j) {
                    if (j != i)
                        out << regina::digit(gluing[j]);
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';
}

template <int dim>
inline bool Triangulation<dim>::dependsOnParent() const {
    return false;
}

template <int dim>
inline Packet* Triangulation<dim>::internalClonePacket(Packet* parent) const {
    return new Triangulation<dim>(*this);
}

template <int dim>
void Triangulation<dim>::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the simplex gluings.
    Simplex<dim>* adj;
    int facet;

    // We will send permutation codes directly to the output stream.
    // This requires them to be numeric types (not character types).
    static_assert(! (
            std::is_same<typename Perm<dim + 1>::Code, char>::value ||
            std::is_same<typename Perm<dim + 1>::Code, unsigned char>::value
        ),
        "The generic implementation of Triangulation<dim>::writeXMLPacketData "
        "requires permutation codes to be numeric types.");

    out << "  <simplices size=\"" << simplices_.size() << "\">\n";
    for (auto s : simplices_) {
        out << "    <simplex desc=\"" <<
            xmlEncodeSpecialChars(s->description()) << "\"> ";
        for (facet = 0; facet <= dim; ++facet) {
            adj = s->adjacentSimplex(facet);
            if (adj) {
                out << adj->index() << ' '
                    << s->adjacentGluing(facet).permCode() << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</simplex>\n";
    }
    out << "  </simplices>\n";

    detail::TriangulationBase<dim>::writeXMLBaseProperties(out);
}

template <int dim>
inline XMLPacketReader* Triangulation<dim>::xmlReader(Packet*,
        XMLTreeResolver& resolver) {
    return new XMLTriangulationReader<dim>(resolver);
}

// Inline functions for DegreeLessThan / DegreeGreaterThan

template <int dim, int subdim>
inline DegreeLessThan<dim, subdim>::DegreeLessThan(
        const Triangulation<dim>& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeLessThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template face<subdim>(a)->degree() <
            tri_.template face<subdim>(b)->degree());
}

template <int dim, int subdim>
inline DegreeGreaterThan<dim, subdim>::DegreeGreaterThan(
        const Triangulation<dim>& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeGreaterThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template face<subdim>(a)->degree() >
            tri_.template face<subdim>(b)->degree());
}

} // namespace regina

#endif
