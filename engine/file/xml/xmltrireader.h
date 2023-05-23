
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/xmltrireader.h
 *  \brief Deals with parsing XML data for triangulation packets.
 */

#ifndef __REGINA_XMLTRIREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLTRIREADER_H
#endif

#include "regina-core.h"
#include "file/xml/xmlalgebrareader.h"
#include "file/xml/xmlpacketreader.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/stringutils.h"
#include <vector>

namespace regina {

/**
 * Internal class that indicates the XML tags and attributes used in the
 * old second-generation file format to describe top-dimensional simplices in a
 * <i>dim</i>-dimensional triangulation.
 *
 * \nopython
 *
 * \tparam dim The dimension of the triangulation in question.
 */
template <int dim>
struct XMLLegacyTriangulationTags {
    static constexpr const char* simplices = "simplices";
        /**< The XML tag that stores the set of all top-dimensional
             simplices for a <i>dim</i>-dimensional triangulation. */
    static constexpr const char* simplex = "simplex";
        /**< The XML tag that stores a single top-dimensional simplex
             in a <i>dim</i>-dimensional triangulation. */
    static constexpr const char* size = "size";
        /**< The XML attribute that stores the number of top-dimensional
             simplices in a <i>dim</i>-dimensional triangulation. */
};

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.
template <>
struct XMLLegacyTriangulationTags<4> {
    static constexpr const char* simplices = "pentachora";
    static constexpr const char* simplex = "pent";
    static constexpr const char* size = "npent";
};

template <>
struct XMLLegacyTriangulationTags<3> {
    static constexpr const char* simplices = "tetrahedra";
    static constexpr const char* simplex = "tet";
    static constexpr const char* size = "ntet";
};

template <>
struct XMLLegacyTriangulationTags<2> {
    static constexpr const char* simplices = "triangles";
    static constexpr const char* simplex = "triangle";
    static constexpr const char* size = "ntriangles";
};
#endif // __DOXYGEN

/**
 * Helper class that reads the XML element for a single top-dimensional
 * simplex in a <i>dim</i>-dimensional triangulation.
 * In other words, this reads the contents of a single `<simplex>`
 * element for dimension \a dim ≥ 5, or a single `<triangle>`,
 * `<tet>` or `<pent>` element for dimension \a dim = 2, 3 or 4.
 *
 * It is assumed that the underlying triangulation and its simplices
 * have already been created.  The task of this reader is to flesh out
 * the "contents" of a single simplex; that is, the description of the
 * simplex and its gluings to adjacent simplices.
 *
 * \nopython
 *
 * \tparam dim The dimension of the triangulation being read.
 */
template <int dim>
class XMLSimplexReader : public XMLElementReader {
    private:
        Triangulation<dim>* tri_;
            /**< The triangulation containing the simplices being read. */
        Simplex<dim>* simplex_;
            /**< The specific simplex being read. */
        bool permIndex_;
            /**< \c true if permutations are stored as indices into Sn, or
                 \c false if they are stored as image packs. */

    public:
        /**
         * Creates a new simplex element reader.
         *
         * \pre The given triangulation \a tri already contains at least
         * (\a whichSimplex + 1) top-dimensional simplices.
         *
         * \param tri the triangulation containing the simplex being read.
         * \param whichSimplex the index of the simplex being read
         * within the triangulation \a tri.
         * \param permIndex \c true if permutations are stored as indices into
         * Sn, or \c false if they are stored as image packs.
         * This must always be specified, regardless of which XML file format
         * we are reading.
         */
        XMLSimplexReader(Triangulation<dim>* tri, size_t whichSimplex,
            bool permIndex);

        void startElement(const std::string&,
                const regina::xml::XMLPropertyDict& props, XMLElementReader*)
                override;

        void initialChars(const std::string& chars) override;
};

/**
 * Helper class that reads the XML element in the old second-generation format
 * for the set of all top-dimensional simplices in a <i>dim</i>-dimensional
 * triangulation.
 * In other words, this reads the contents of a single `<simplices>`
 * element for dimension \a dim ≥ 5, or a single `<triangles>`,
 * `<tetrahedra>` or `<pentachora>` element for dimension
 * \a dim = 2, 3 or 4.
 *
 * It is assumed that the underlying triangulation has already been created,
 * but its simplices have not.
 *
 * \nopython
 *
 * \tparam dim The dimension of the triangulation being read.
 */
template <int dim>
class XMLLegacySimplicesReader : public XMLElementReader {
    private:
        Triangulation<dim>* tri_;
            /**< The triangulation to contain the simplices being read. */
        size_t& readSimplices_;
            /**< Used to update the number of top-dimensional simplices that
                 have been read so far. */

    public:
        /**
         * Creates a new simplices element reader.
         *
         * The given triangulation should be empty; its simplices will
         * be created by this reader.
         *
         * \param tri the triangulation being read.
         * \param readSimplices a reference to the counter that needs to
         * track the number of top-dimensional simplices read so far.
         */
        XMLLegacySimplicesReader(Triangulation<dim>* tri,
            size_t& readSimplices);

        void startElement(const std::string&,
                const regina::xml::XMLPropertyDict& props, XMLElementReader*)
                override;

        XMLElementReader* startSubElement(const std::string& subTagName,
                const regina::xml::XMLPropertyDict&) override;
};

/**
 * An XML packet reader that reads a single <i>dim</i>-dimensional
 * triangulation.
 *
 * This triangulation will be assembled using the "raw" routines
 * newSimplexRaw(), joinRaw(), etc., _without_ the usual protections from
 * a local ChangeAndClearSpan.  The reason we use the "raw" routines
 * is so we can incrementally recreate simplex/facet locks and
 * assemble top-dimensional simplices without having to worry about how these
 * operations interact.  The reason we do not need the usual protections
 * is because this assembly should all take place before the first call to
 * packetToCommit(), when \a tri_ is a freshly created triangulation with no
 * event listeners, no snapshot users, and has not yet been inserted into the
 * packet tree.
 *
 * \nopython
 *
 * \tparam dim The dimension of the triangulation being read.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class XMLTriangulationReader : public XMLPacketReader {
    protected:
        std::shared_ptr<PacketOf<Triangulation<dim>>> tri_;
            /**< The triangulation currently being read. */
        bool permIndex_;
            /**< \c true if permutations are stored as indices into Sn, or
                 \c false if they are stored as image packs. */
        size_t readSimplices_;
            /**< Maintains the number of top-dimensional simplices that
                 have been read so far. */

    public:
        /**
         * Creates a new triangulation reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * \param size the total number of top-dimensional simplices in the
         * triangulation.  This should be 0 if we are reading the
         * second-generation file format.
         * \param permIndex \c true if permutations are stored as indices into
         * Sn, or \c false if they are stored as image packs.
         * This will be ignored when reading the second-generation file format.
         */
        XMLTriangulationReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, size_t size, bool permIndex);

        /**
         * Returns an XML element reader for the given optional property of a
         * <i>dim</i>-dimensional triangulation.
         *
         * If \a subTagName names an XML element that describes an optional
         * property of a triangulation (such as \c H1 or \c fundgroup for
         * 3-manifold triangulations), then this function should return
         * a corresponding element reader.
         *
         * Otherwise this function should return a new XMLElementReader,
         * which will cause the XML element to be ignored.
         *
         * \param subTagName the name of the XML subelement opening tag.
         * \param subTagProps the properties associated with the
         * subelement opening tag.
         * \return a newly created element reader that will be used to
         * parse the subelement.  This class should not take care of the
         * new reader's destruction; that will be done by the parser.
         */
        XMLElementReader* startPropertySubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * Internal class that reads an abelian group property.
 */
class AbelianGroupPropertyReader : public XMLElementReader {
    public:
        using PropType = std::optional<AbelianGroup>;
            /**< The type of the property currently being read. */

    private:
        PropType& prop_;
            /**< The property currently being read. */

    public:
        /**
         * Creates a new reader that stores its results in the
         * given triangulation property.
         *
         * \param prop a reference to the triangulation property
         * in which the data that is read should be stored.
         */
        AbelianGroupPropertyReader(PropType& prop);

        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict&) override;

        void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * Internal class that reads a group presentation property.
 */
class GroupPresentationPropertyReader : public XMLElementReader {
    public:
        using PropType = std::optional<GroupPresentation>;
            /**< The type of the property currently being read. */

    private:
        PropType& prop_;
            /**< The property currently being read. */

    public:
        /**
         * Creates a new reader that stores its results in the
         * given triangulation property.
         *
         * \param prop a reference to the triangulation property
         * in which the data that is read should be stored.
         */
        GroupPresentationPropertyReader(PropType& prop);

        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict&) override;

        void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

// Implementation details for XMLSimplexReader

template <int dim>
inline XMLSimplexReader<dim>::XMLSimplexReader(
        Triangulation<dim>* tri, size_t whichSimplex, bool permIndex) :
        tri_(tri), simplex_(tri->simplices()[whichSimplex]),
        permIndex_(permIndex) {
}

template <int dim>
inline void XMLSimplexReader<dim>::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    auto desc = props.find("desc");
    if (desc != props.end())
        simplex_->setDescription(desc->second);

    auto lock = props.find("lock");
    if (lock != props.end() && ! lock->second.empty()) {
        // Read the lock mask as an unsigned long, since this is guaranteed
        // to be ≥ 32 bits (which means it is large enough for all of Regina's
        // supported dimensions).
        char* endPtr;
        unsigned long mask = strtoul(lock->second.c_str(), &endPtr, 16);
        if ((*endPtr == 0) && (mask >> (dim + 2) == 0)) {
            // This is a valid lock mask.
            simplex_->locks_ = mask;
        }
    }
}

template <int dim>
void XMLSimplexReader<dim>::initialChars(const std::string& chars) {
    std::vector<std::string> tokens = basicTokenise(chars);
    if (tokens.size() != 2 * (dim + 1))
        return;

    long simpIndex;
    Perm<dim + 1> perm;
    Simplex<dim>* adjSimp;
    int adjFacet;
    for (int k = 0; k <= dim; ++k) {
        if (! valueOf(tokens[2 * k], simpIndex))
            continue;
        if (simpIndex < 0 || simpIndex >= static_cast<long>(tri_->size()))
            continue;

        if constexpr (dim == 2) {
            // Image packs are not supported at all by Perm<3>.
            typename Perm<dim + 1>::Index permIndex;
            if (! valueOf(tokens[2 * k + 1], permIndex))
                continue;
            if (permIndex < 0 || permIndex >= Perm<dim + 1>::nPerms)
                continue;
            perm = Perm<dim + 1>::Sn[permIndex];
        } else if (permIndex_) {
            typename Perm<dim + 1>::Index permIndex;
            if (! valueOf(tokens[2 * k + 1], permIndex))
                continue;
            if (permIndex < 0 || permIndex >= Perm<dim + 1>::nPerms)
                continue;
            perm = Perm<dim + 1>::Sn[permIndex];
        } else {
            typename Perm<dim + 1>::ImagePack imagePack;
            if (! valueOf(tokens[2 * k + 1], imagePack))
                continue;
            if (! Perm<dim + 1>::isImagePack(imagePack))
                continue;
            perm = Perm<dim + 1>::fromImagePack(imagePack);
        }

        adjSimp = tri_->simplices()[simpIndex];
        adjFacet = perm[k];
        if (adjSimp == simplex_ && adjFacet == k)
            continue;
        if (simplex_->adjacentSimplex(k))
            continue;
        if (adjSimp->adjacentSimplex(adjFacet))
            continue;

        simplex_->joinRaw(k, adjSimp, perm);
    }
}

// Implementation details for XMLLegacySimplicesReader

template <int dim>
inline XMLLegacySimplicesReader<dim>::XMLLegacySimplicesReader(
        Triangulation<dim>* tri, size_t& readSimplices) :
        tri_(tri), readSimplices_(readSimplices) {
}

template <int dim>
void XMLLegacySimplicesReader<dim>::startElement(
        const std::string& /* tagName */,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    long size;
    if (valueOf(props.lookup(XMLLegacyTriangulationTags<dim>::size), size))
        for ( ; size > 0; --size)
            tri_->newSimplexRaw();
}

template <int dim>
XMLElementReader* XMLLegacySimplicesReader<dim>::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == XMLLegacyTriangulationTags<dim>::simplex) {
        if (readSimplices_ < tri_->size())
            return new XMLSimplexReader<dim>(tri_, readSimplices_++, dim == 2);
        else
            return new XMLElementReader();
    } else
        return new XMLElementReader();
}

// Inline functions for XMLTriangulationReader

template <int dim>
inline XMLTriangulationReader<dim>::XMLTriangulationReader(
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        std::string label, std::string id, size_t size, bool permIndex) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        tri_(make_packet<Triangulation<dim>>()),
        permIndex_(permIndex), readSimplices_(0) {
    for ( ; size > 0; --size)
        tri_->newSimplexRaw();
}

template <int dim>
inline std::shared_ptr<Packet> XMLTriangulationReader<dim>::packetToCommit() {
    // Here is a good time to enforce the consistency of the triangulation:
    // this function would typically only be called once, immediately after
    // the triangulation has been assembled, and before anything else tries
    // to reference it or use it.
    //
    // This consistency checking is a little work, but again this would
    // typically only happen once (and if it packetToCommit() does get
    // called again, the duplicated work is unnecessary but harmless).
    //
    // For now, all we will do is enforce the consistency of facet locks.
    for (auto s : tri_->simplices())
        if (s->locks_)
            for (int facet = 0; facet <= dim; ++facet)
                if (auto adj = s->adjacentSimplex(facet))
                    if (s->isFacetLocked(facet)) {
                        auto adjFacet = s->adjacentFacet(facet);
                        if (! adj->isFacetLocked(adjFacet))
                            adj->lockFacetRaw(adjFacet);
                    }

    return tri_;
}

template <int dim>
XMLElementReader* XMLTriangulationReader<dim>::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == "simplex") {
        if (readSimplices_ < tri_->size())
            return new XMLSimplexReader<dim>(tri_.get(), readSimplices_++,
                permIndex_);
        else
            return new XMLElementReader();
    } else if (subTagName == XMLLegacyTriangulationTags<dim>::simplices)
        return new XMLLegacySimplicesReader<dim>(tri_.get(), readSimplices_);
    else
        return static_cast<XMLTriangulationReader<dim>*>(this)->
            startPropertySubElement(subTagName, subTagProps);
}

template <int dim>
inline void XMLTriangulationReader<dim>::endContentSubElement(
        const std::string&, XMLElementReader*) {
}

template <int dim>
inline XMLElementReader*
        XMLTriangulationReader<dim>::startPropertySubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (subTagName == "fundgroup")
        return new GroupPresentationPropertyReader(tri_->fundGroup_);
    else if (subTagName == "H1")
        return new AbelianGroupPropertyReader(tri_->H1_);

    if constexpr (dim == 3 || dim == 4) {
        if (subTagName == "H2")
            return new AbelianGroupPropertyReader(tri_->prop_.H2_);
    }

    if constexpr (dim == 3) {
        // We don't read boundary component properties since they're stored
        // across multiple property tags and they're easy to calculate anyway.
        if (subTagName == "zeroeff") {
            bool b;
            if (valueOf(props.lookup("value"), b))
                tri_->prop_.zeroEfficient_ = b;
        } else if (subTagName == "splitsfce") {
            bool b;
            if (valueOf(props.lookup("value"), b))
                tri_->prop_.splittingSurface_ = b;
        } else if (subTagName == "threesphere") {
            bool b;
            if (valueOf(props.lookup("value"), b))
                tri_->prop_.threeSphere_ = b;
        } else if (subTagName == "handlebody") {
            ssize_t genus;
            if (valueOf(props.lookup("value"), genus))
                if (genus >= -1)
                    tri_->prop_.handlebody_ = genus;
        } else if (subTagName == "threeball") {
            bool b;
            // If threeball is false, we don't do anything since this
            // might or might not still be a handelbody of some other genus.
            if (valueOf(props.lookup("value"), b))
                if (b)
                    tri_->prop_.handlebody_ = 0;
        } else if (subTagName == "solidtorus") {
            bool b;
            // If solidtorus is false, we don't do anything since this
            // might or might not still be a handelbody of some other genus.
            if (valueOf(props.lookup("value"), b))
                if (b)
                    tri_->prop_.handlebody_ = 1;
        } else if (subTagName == "txi") {
            bool b;
            if (valueOf(props.lookup("value"), b))
                tri_->prop_.TxI_ = b;
        } else if (subTagName == "irreducible") {
            bool b;
            if (valueOf(props.lookup("value"), b))
                tri_->prop_.irreducible_ = b;
        } else if (subTagName == "compressingdisc") {
            bool b;
            if (valueOf(props.lookup("compressingdisc"), b))
                tri_->prop_.compressingDisc_ = b;
        } else if (subTagName == "haken") {
            bool b;
            if (valueOf(props.lookup("haken"), b))
                tri_->prop_.haken_ = b;
        } else if (subTagName == "H1Rel")
            return new AbelianGroupPropertyReader(tri_->prop_.H1Rel_);
        else if (subTagName == "H1Bdry")
            return new AbelianGroupPropertyReader(tri_->prop_.H1Bdry_);
    }

    return new XMLElementReader();
}

// Inline functions for AbelianGroupPropertyReader

inline AbelianGroupPropertyReader::AbelianGroupPropertyReader(PropType& prop) :
        prop_(prop) {
}

inline XMLElementReader* AbelianGroupPropertyReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "abeliangroup")
        if (! prop_.has_value())
            return new XMLAbelianGroupReader();
    return new XMLElementReader();
}

inline void AbelianGroupPropertyReader::endSubElement(
        const std::string& subTagName, XMLElementReader* subReader) {
    if (subTagName == "abeliangroup") {
        auto& ans = static_cast<XMLAbelianGroupReader*>(subReader)->group();
        if (ans)
            prop_ = std::move(*ans);
    }
}

// Inline functions for GroupPresentationPropertyReader

inline GroupPresentationPropertyReader::GroupPresentationPropertyReader(
        PropType& prop) : prop_(prop) {
}

inline XMLElementReader* GroupPresentationPropertyReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "group")
        if (! prop_.has_value())
            return new XMLGroupPresentationReader();
    return new XMLElementReader();
}

inline void GroupPresentationPropertyReader::endSubElement(
        const std::string& subTagName, XMLElementReader* subReader) {
    if (subTagName == "group") {
        auto& ans = static_cast<XMLGroupPresentationReader*>(subReader)->
            group();
        if (ans)
            prop_ = std::move(*ans);
    }
}

} // namespace regina

#endif

