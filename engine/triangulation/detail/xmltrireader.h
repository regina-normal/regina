
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

/*! \file triangulation/detail/xmltrireader.h
 *  \brief Implementation details for parsing XML data for triangulation
 *  packets.
 */

#ifndef __XMLTRIREADER_H_DETAIL
#ifndef __DOXYGEN
#define __XMLTRIREADER_H_DETAIL
#endif

#include "regina-core.h"
#include "algebra/xmlalgebrareader.h"
#include "packet/xmlpacketreader.h"
#include "triangulation/generic/triangulation.h"
#include "utilities/stringutils.h"
#include <vector>

namespace regina {

template <int> class XMLTriangulationReader;

namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * Internal class that indicates the XML tags and attributes used to describe
 * top-dimensional simplices in a <i>dim</i>-dimensional triangulation.
 *
 * \ifacespython Not present.
 *
 * \tparam dim The dimension of the triangulation in question.
 */
template <int dim>
struct XMLTriangulationTags {
    constexpr static const char* simplices();
        /**< The XML tag that stores the set of all top-dimensional
             simplices for a <i>dim</i>-dimensional triangulation. */
    constexpr static const char* simplex();
        /**< The XML tag that stores a single top-dimensional simplex
             in a <i>dim</i>-dimensional triangulation. */
    constexpr static const char* size();
        /**< The XML attribute that stores the number of top-dimensional
             simplices in a <i>dim</i>-dimensional triangulation. */
};

/**
 * Helper class that reads the XML element for a single top-dimensional
 * simplex in a <i>dim</i>-dimensional triangulation.
 * In other words, this reads the contents of a single &lt;simplex&gt;
 * element for dimension \a dim &ge; 5, or a single &lt;triangle&gt;,
 * &lt;tet&gt; or &lt;pent&gt; element for dimension \a dim = 2, 3 or 4.
 *
 * It is assumed that the underlying triangulation and its simplices
 * have already been created.  The task of this reader is to flesh out
 * the "contents" of a single simplex; that is, the description of the
 * simplex and its gluings to adjacent simplices.
 *
 * \ifacespython Not present.
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

    public:
        /**
         * Creates a new simplex element reader.
         *
         * \pre The given triangulation \a tri already contains at least
         * (\a whichSimplex + 1) top-dimensional simplices.
         *
         * @param tri the triangulation containing the simplex being read.
         * @param whichSimplex the index of the simplex being read
         * within the triangulation \a tri.
         */
        XMLSimplexReader(Triangulation<dim>* tri, size_t whichSimplex);

        virtual void startElement(const std::string&,
                const regina::xml::XMLPropertyDict& props, XMLElementReader*);

        virtual void initialChars(const std::string& chars);
};

/**
 * Helper class that reads the XML element for the set of all top-dimensional
 * simplices in a <i>dim</i>-dimensional triangulation.
 * In other words, this reads the contents of a single &lt;simplices&gt;
 * element for dimension \a dim &ge; 5, or a single &lt;triangles&gt;,
 * &lt;tetrahedra&gt; or &lt;pentachora&gt; element for dimension
 * \a dim = 2, 3 or 4.
 *
 * It is assumed that the underlying triangulation has already been created,
 * but its simplices have not.
 *
 * \ifacespython Not present.
 *
 * \tparam dim The dimension of the triangulation being read.
 */
template <int dim>
class XMLSimplicesReader : public XMLElementReader {
    private:
        Triangulation<dim>* tri_;
            /**< The triangulation to contain the simplices being read. */
        size_t readSimplices_;
            /**< The number of simplices in the triangulation, as defined by
                 the \c size attribute of this tag (or, in standard dimensions,
                 the \c ntriangles, \c ntet or \c npent attribute instead). */

    public:
        /**
         * Creates a new simplices element reader.
         *
         * The given triangulation should be empty; its simplices will
         * be created by this reader.
         *
         * @param tri the triangulation being read.
         */
        XMLSimplicesReader(Triangulation<dim>* tri);

        virtual void startElement(const std::string&,
                const regina::xml::XMLPropertyDict& props, XMLElementReader*);

        virtual XMLElementReader* startSubElement(
                const std::string& subTagName,
                const regina::xml::XMLPropertyDict&);
};

/**
 * Helper class that provides core functionality for the XML packet reader
 * that reads a single <i>dim</i>-dimensional triangulation.
 *
 * The XML packet reader itself is provided by the class
 * XMLTriangulationReader<dim>, which uses this as a base class.
 * There should be no need for other classes to refer to
 * XMLTriangulationReaderBase directly.
 *
 * \ifacespython Not present.
 *
 * \tparam dim The dimension of the triangulation being read.
 */
template <int dim>
class XMLTriangulationReaderBase : public XMLPacketReader {
    protected:
        Triangulation<dim>* tri_;
            /**< The triangulation currently being read. */

    public:
        /**
         * Creates a new triangulation reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLTriangulationReaderBase(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;

    protected:
        /**
         * Internal class that reads an abelian group property.
         */
        class AbelianGroupPropertyReader : public XMLElementReader {
            public:
                typedef Property<AbelianGroup, StoreManagedPtr> PropType;
                    /**< The type of the property currently being read. */

            private:
                PropType& prop_;
                    /**< The property currently being read. */

            public:
                /**
                 * Creates a new reader that stores its results in the
                 * given triangulation property.
                 *
                 * @param prop a reference to the triangulation property
                 * in which the data that is read should be stored.
                 */
                AbelianGroupPropertyReader(PropType& prop);

                virtual XMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&);

                virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader);
        };

        /**
         * Internal class that reads a group presentation property.
         */
        class GroupPresentationPropertyReader : public XMLElementReader {
            public:
                typedef Property<GroupPresentation, StoreManagedPtr> PropType;
                    /**< The type of the property currently being read. */

            private:
                PropType& prop_;
                    /**< The property currently being read. */

            public:
                /**
                 * Creates a new reader that stores its results in the
                 * given triangulation property.
                 *
                 * @param prop a reference to the triangulation property
                 * in which the data that is read should be stored.
                 */
                GroupPresentationPropertyReader(PropType& prop);

                virtual XMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&);

                virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader);
        };

        /**
         * Returns a reader for the generic triangulation property with the
         * given subtag name.
         *
         * If \a subTagName refers to a property that is managed by the
         * generic base class TriangulationBase<dim>, then this routine
         * returns an appropriate element reader.  Otherwise this routine
         * returns \c null.
         *
         * @param subTagName the name of the XML subelement opening tag.
         * @param subTagProps the properties associated with the subelement
         * opening tag.
         * @return a newly created element reader if the corresponding
         * triangulation property is managed by TriangulationBase<dim>, or
         * \c null otherwise.
         */
        XMLElementReader* propertyReader(
                const std::string& subTagName,
                const regina::xml::XMLPropertyDict& subTagProps);
};

/*@}*/

// Inline functions for XMLTriangulationTags

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <int dim>
inline constexpr const char* XMLTriangulationTags<dim>::simplices() {
    return "simplices";
}
template <>
inline constexpr const char* XMLTriangulationTags<4>::simplices() {
    return "pentachora";
}
template <>
inline constexpr const char* XMLTriangulationTags<3>::simplices() {
    return "tetrahedra";
}
template <>
inline constexpr const char* XMLTriangulationTags<2>::simplices() {
    return "triangles";
}

template <int dim>
inline constexpr const char* XMLTriangulationTags<dim>::simplex() {
    return "simplex";
}
template <>
inline constexpr const char* XMLTriangulationTags<4>::simplex() {
    return "pent";
}
template <>
inline constexpr const char* XMLTriangulationTags<3>::simplex() {
    return "tet";
}
template <>
inline constexpr const char* XMLTriangulationTags<2>::simplex() {
    return "triangle";
}

template <int dim>
inline constexpr const char* XMLTriangulationTags<dim>::size() {
    return "size";
}
template <>
inline constexpr const char* XMLTriangulationTags<4>::size() {
    return "npent";
}
template <>
inline constexpr const char* XMLTriangulationTags<3>::size() {
    return "ntet";
}
template <>
inline constexpr const char* XMLTriangulationTags<2>::size() {
    return "ntriangles";
}

#endif // __DOXYGEN

// Implementation details for XMLSimplexReader

template <int dim>
inline XMLSimplexReader<dim>::XMLSimplexReader(
        Triangulation<dim>* tri, size_t whichSimplex) :
        tri_(tri), simplex_(tri->simplices()[whichSimplex]) {
}

template <int dim>
inline void XMLSimplexReader<dim>::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    simplex_->setDescription(props.lookup("desc"));
}

template <int dim>
void XMLSimplexReader<dim>::initialChars(const std::string& chars) {
    std::vector<std::string> tokens;
    if (basicTokenise(back_inserter(tokens), chars) != 2 * (dim + 1))
        return;

    long simpIndex;
    typename Perm<dim + 1>::Code permCode;
    Perm<dim + 1> perm;
    Simplex<dim>* adjSimp;
    int adjFacet;
    for (int k = 0; k <= dim; ++k) {
        if (! valueOf(tokens[2 * k], simpIndex))
            continue;
        if (! valueOf(tokens[2 * k + 1], permCode))
            continue;

        if (simpIndex < 0 || simpIndex >= static_cast<long>(tri_->size()))
            continue;
        if (! Perm<dim + 1>::isPermCode(permCode))
            continue;

        perm.setPermCode(permCode);
        adjSimp = tri_->simplices()[simpIndex];
        adjFacet = perm[k];
        if (adjSimp == simplex_ && adjFacet == k)
            continue;
        if (simplex_->adjacentSimplex(k))
            continue;
        if (adjSimp->adjacentSimplex(adjFacet))
            continue;

        simplex_->join(k, adjSimp, perm);
    }
}

// Implementation details for XMLSimplicesReader

template <int dim>
inline XMLSimplicesReader<dim>::XMLSimplicesReader(Triangulation<dim>* tri) :
        tri_(tri), readSimplices_(0) {
}

template <int dim>
void XMLSimplicesReader<dim>::startElement(const std::string& /* tagName */,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    long nSimplices;
    if (valueOf(props.lookup(XMLTriangulationTags<dim>::size()), nSimplices))
        for ( ; nSimplices > 0; --nSimplices)
            tri_->newSimplex();
}

template <int dim>
XMLElementReader* XMLSimplicesReader<dim>::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == XMLTriangulationTags<dim>::simplex()) {
        if (readSimplices_ < tri_->size())
            return new XMLSimplexReader<dim>(tri_, readSimplices_++);
        else
            return new XMLElementReader();
    } else
        return new XMLElementReader();
}

// Inline functions for XMLTriangulationReaderBase

template <int dim>
inline XMLTriangulationReaderBase<dim>::XMLTriangulationReaderBase(
        XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), tri_(new Triangulation<dim>()) {
}

template <int dim>
inline Packet* XMLTriangulationReaderBase<dim>::packet() {
    return tri_;
}

template <int dim>
XMLElementReader* XMLTriangulationReaderBase<dim>::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == XMLTriangulationTags<dim>::simplices())
        return new XMLSimplicesReader<dim>(tri_);
    return static_cast<XMLTriangulationReader<dim>*>(this)->
        startPropertySubElement(subTagName, subTagProps);
}

template <int dim>
inline void XMLTriangulationReaderBase<dim>::endContentSubElement(
        const std::string&, XMLElementReader*) {
}

// Inline functions for XMLTriangulationReaderBase::AbelianGroupPropertyReader

template <int dim>
inline XMLTriangulationReaderBase<dim>::AbelianGroupPropertyReader::
        AbelianGroupPropertyReader(PropType& prop) : prop_(prop) {
}

template <int dim>
inline XMLElementReader* XMLTriangulationReaderBase<dim>::
        AbelianGroupPropertyReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "abeliangroup")
        if (! prop_.known())
            return new XMLAbelianGroupReader();
    return new XMLElementReader();
}

template <int dim>
inline void XMLTriangulationReaderBase<dim>::AbelianGroupPropertyReader::
        endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "abeliangroup") {
        AbelianGroup* ans = dynamic_cast<XMLAbelianGroupReader*>(subReader)->
            group();
        if (ans)
            prop_ = ans;
    }
}

// Inline functions for
// XMLTriangulationReaderBase::GroupPresentationPropertyReader

template <int dim>
inline XMLTriangulationReaderBase<dim>::GroupPresentationPropertyReader::
        GroupPresentationPropertyReader(PropType& prop) : prop_(prop) {
}

template <int dim>
inline XMLElementReader* XMLTriangulationReaderBase<dim>::
        GroupPresentationPropertyReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "group")
        if (! prop_.known())
            return new XMLGroupPresentationReader();
    return new XMLElementReader();
}

template <int dim>
inline void XMLTriangulationReaderBase<dim>::GroupPresentationPropertyReader::
        endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "group") {
        GroupPresentation* ans = dynamic_cast<
            XMLGroupPresentationReader*>(subReader)->group();
        if (ans)
            prop_ = ans;
    }
}

template <int dim>
inline XMLElementReader* XMLTriangulationReaderBase<dim>::propertyReader(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == "fundgroup")
        return new GroupPresentationPropertyReader(tri_->fundGroup_);
    else if (subTagName == "H1")
        return new AbelianGroupPropertyReader(tri_->H1_);
    return 0;
}

} } // namespace regina::detail

#endif

