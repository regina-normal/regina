
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
#include "triangulation/generic/triangulation.h"
#include "utilities/stringutils.h"
#include <vector>

namespace regina {

/**
 * \weakgroup triangulation
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
struct XMLTriangulationTags<4> {
    static constexpr const char* simplices = "pentachora";
    static constexpr const char* simplex = "pent";
    static constexpr const char* size = "npent";
};

template <>
struct XMLTriangulationTags<3> {
    static constexpr const char* simplices = "tetrahedra";
    static constexpr const char* simplex = "tet";
    static constexpr const char* size = "ntet";
};

template <>
struct XMLTriangulationTags<2> {
    static constexpr const char* simplices = "triangles";
    static constexpr const char* simplex = "triangle";
    static constexpr const char* size = "ntriangles";
};
#endif // __DOXYGEN

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
                const regina::xml::XMLPropertyDict& props, XMLElementReader*)
                override;

        virtual void initialChars(const std::string& chars) override;
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
                const regina::xml::XMLPropertyDict& props, XMLElementReader*)
                override;

        virtual XMLElementReader* startSubElement(
                const std::string& subTagName,
                const regina::xml::XMLPropertyDict&) override;
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
                typedef std::optional<AbelianGroup> PropType;
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
                    const regina::xml::XMLPropertyDict&) override;

                virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader) override;
        };

        /**
         * Internal class that reads a group presentation property.
         */
        class GroupPresentationPropertyReader : public XMLElementReader {
            public:
                typedef std::optional<GroupPresentation> PropType;
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
                    const regina::xml::XMLPropertyDict&) override;

                virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader) override;
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

/**
 * An XML packet reader that reads a single <i>dim</i>-dimensional
 * triangulation.
 *
 * In some dimensions this template is specialised so that it can read in
 * additional properties of the triangulation.  In order to use these
 * specialised classes, you will need to include the corresponding headers
 * (e.g., triangulation/xmltrireader3.h for \a dim = 3).
 *
 * \ifacespython Not present.
 *
 * \tparam dim The dimension of the triangulation being read.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class XMLTriangulationReader : public XMLTriangulationReaderBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of XMLTriangulationReader<dim> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * Creates a new triangulation reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLTriangulationReader(XMLTreeResolver& resolver);

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
         * @param subTagName the name of the XML subelement opening tag.
         * @param subTagProps the properties associated with the
         * subelement opening tag.
         * @return a newly created element reader that will be used to
         * parse the subelement.  This class should not take care of the
         * new reader's destruction; that will be done by the parser.
         */
        XMLElementReader* startPropertySubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

// Note that some of our classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class XMLTriangulationReader<2>;
template <> class XMLTriangulationReader<3>;
template <> class XMLTriangulationReader<4>;

/*@}*/

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
        if constexpr (dim == 3 || dim == 4) {
            if (! Perm<dim + 1>::isPermCode1(permCode))
                continue;
            perm.setPermCode1(permCode);
        } else {
            if (! Perm<dim + 1>::isPermCode(permCode))
                continue;
            perm.setPermCode(permCode);
        }

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
    if (valueOf(props.lookup(XMLTriangulationTags<dim>::size), nSimplices))
        for ( ; nSimplices > 0; --nSimplices)
            tri_->newSimplex();
}

template <int dim>
XMLElementReader* XMLSimplicesReader<dim>::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == XMLTriangulationTags<dim>::simplex) {
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
    if (subTagName == XMLTriangulationTags<dim>::simplices)
        return new XMLSimplicesReader<dim>(tri_);
    return static_cast<XMLTriangulationReader<dim>*>(this)->
        startPropertySubElement(subTagName, subTagProps);
}

template <int dim>
inline void XMLTriangulationReaderBase<dim>::endContentSubElement(
        const std::string&, XMLElementReader*) {
}

// Inline functions for XMLTriangulationReader

template <int dim>
inline XMLTriangulationReader<dim>::XMLTriangulationReader(
        XMLTreeResolver& resolver) :
        XMLTriangulationReaderBase<dim>(resolver) {
}

template <int dim>
inline XMLElementReader* XMLTriangulationReader<dim>::startPropertySubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new XMLElementReader();
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
        if (! prop_.has_value())
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
        if (ans) {
            prop_ = std::move(*ans);
            delete ans;
        }
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
        if (! prop_.has_value())
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
        if (ans) {
            prop_ = std::move(*ans);
            delete ans;
        }
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

} // namespace regina

#endif

