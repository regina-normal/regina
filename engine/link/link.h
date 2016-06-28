
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

/*! \file link/nlink.h
 *  \brief Deals with knots and links in the 3-sphere.
 */

#ifndef __LINK_H
#ifndef __DOXYGEN
#define __LINK_H
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "packet/npacket.h"
#include "utilities/nmarkedvector.h"

namespace regina {

/**
 * \addtogroup link Knots and Links
 * Knots and links in the 3-sphere
 * @{
 */

class Crossing;
class Link;

/**
 * TODO: Document this class.
 *
 * Note: intended to pass by value.  Invalid means crossing_ is null.
 */
class REGINA_API CrossingStrand {
    private:
        Crossing* crossing_;
        int strand_; // 0,1 = under, over strands

    public:
        /**
         * Initialises this to an invalid crossing strand.
         */
        CrossingStrand();

        /**
         * Initialises this to the given strand of the given crossing.
         *
         * @param crossing the crossing being identified.
         * @param strand 0 to denote the strand running under the crossing, or
         * 1 to denote the strand running over the crossing.
         */
        CrossingStrand(Crossing* crossing, int strand);

        CrossingStrand(const CrossingStrand& rhs);

        Crossing* crossing() const;
        int strand() const;

        bool operator == (const CrossingStrand& rhs) const;
        bool operator != (const CrossingStrand& rhs) const;
        CrossingStrand& operator = (const CrossingStrand& rhs);

        // Python: inc()
        CrossingStrand& operator ++ ();
        // Python: n/a
        CrossingStrand operator ++ (int);
        // Python: dec()
        CrossingStrand& operator -- ();
        // Python: n/a
        CrossingStrand operator -- (int);

        CrossingStrand next() const;
        CrossingStrand prev() const;

        operator bool() const;

    friend class Link;
};

std::ostream& operator << (std::ostream& out, const CrossingStrand& s);

/**
 * TODO: Document this class.
 *
 * Positive crossing: -\-, both pointing right
 *
 * Negative crossing: -/-, both pointing right
 */
class REGINA_API Crossing : public NMarkedElement,
        public Output<Crossing>, public boost::noncopyable {
    private:
        int sign_; // +1 or -1
        CrossingStrand next_[2]; // 0,1 = under, over strands
        CrossingStrand prev_[2]; // 0,1 = under, over strands

    public:
        int index() const;
        int sign() const;
        CrossingStrand next(int strand) const;
        CrossingStrand prev(int strand) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        // next, prev initialised to (0,0).
        // ID and sign are not initialised.
        Crossing();

        // next, prev initialised to (0,0).
        Crossing(int sign);

    friend class Link;
    friend class XMLLinkCrossingsReader;
    friend class XMLLinkConnectionsReader;
};

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_LINK> {
    typedef Link Class;
    inline static const char* name() {
        return "Link";
    }
};
#endif

class REGINA_API Link : public NPacket {
    REGINA_PACKET(Link, PACKET_LINK)

    private:
        NMarkedVector<Crossing> crossings_;
        std::vector<CrossingStrand> components_; // invalid = unknot component

    public:
        /**
         * Constructs a zero-component link.
         */
        Link();
        Link(const Link& cloneMe);

        ~Link();

        /**
         * Returns the number of crossings in this link.
         *
         * @return the number of crossings.
         */
        size_t size() const;

        /**
         * Returns the number of components in this link.
         */
        size_t countComponents() const;

        Crossing* crossing(size_t index);
        const Crossing* crossing(size_t index) const;

        /**
         * Builds a link from the text representation described by
         * Bob Jenkins.
         *
         * The link is given by a sequence of integers (whitespace does
         * not matter).  These integers are:
         *
         * - the number of components in the link;
         *
         * - for each link component:
         *   + how many crossings the component passes through; and
         *   + for each crossing in order along the component, the crossing ID
         *     followed by +1 or -1 for over or under respectively;
         *
         * - for each crossing:
         *   + the crossing ID followed by +1 or -1 for a right or left
         *     crossing respectively.
         *
         * Crossings must be numbered 0,1,...,<i>c</i>-1, where there are \a c
         * crossings in total.
         *
         * For example, two circles placed one upon the other to give
         * four crossings could be represented by:
         *
         *     2
         *     4
         *     0  1   1  1   2  1   3  1
         *     4
         *     0 -1   1 -1   2 -1   3 -1
         *     0 -1
         *     1  1
         *     2 -1
         *     3  1
         *
         * See <http://burtleburtle.net/bob/knot/homfly.html> for the
         * original code in which this format is used.
         */
        static Link* fromJenkins(std::istream& in);

        static Link* fromJenkins(const std::string& str);

        // All crossings change sign.  Strands stay fixed.
        // Corresponds to a 2-D reflection about some axis in the plane.
        void reflect();

        // All strands switch under/over.  Crossing signs stay fixed.
        // Corresponds to a 3-D rotation about some axis in the plane.
        void rotate();

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* xmlReader(NPacket* parent,
            NXMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;

    private:
        /**
         * Translates a crossing strand from some other link to the
         * corresponding strand of this link.
         */
        CrossingStrand translate(const CrossingStrand& other) const;

    friend class XMLLinkCrossingsReader;
    friend class XMLLinkComponentsReader;
};

/*@}*/

// Inline functions for CrossingStrand

inline CrossingStrand::CrossingStrand() : crossing_(0), strand_(0) {
}

inline CrossingStrand::CrossingStrand(Crossing* crossing, int strand) :
        crossing_(crossing), strand_(strand) {
}

inline CrossingStrand::CrossingStrand(const CrossingStrand& rhs) :
        crossing_(rhs.crossing_), strand_(rhs.strand_) {
}

inline Crossing* CrossingStrand::crossing() const {
    return crossing_;
}

inline int CrossingStrand::strand() const {
    return strand_;
}

inline bool CrossingStrand::operator == (const CrossingStrand& rhs) const {
    return (crossing_ == rhs.crossing_ && strand_ == rhs.strand_);
}

inline bool CrossingStrand::operator != (const CrossingStrand& rhs) const {
    return (crossing_ != rhs.crossing_ || strand_ != rhs.strand_);
}

inline CrossingStrand& CrossingStrand::operator = (const CrossingStrand& rhs) {
    crossing_ = rhs.crossing_;
    strand_ = rhs.strand_;
    return *this;
}

inline CrossingStrand& CrossingStrand::operator ++ () {
    return (*this = crossing_->next(strand_));
}

inline CrossingStrand CrossingStrand::operator ++ (int) {
    CrossingStrand old = *this;
    *this = crossing_->next(strand_);
    return old;
}

inline CrossingStrand& CrossingStrand::operator -- () {
    return (*this = crossing_->prev(strand_));
}

inline CrossingStrand CrossingStrand::operator -- (int) {
    CrossingStrand old = *this;
    *this = crossing_->prev(strand_);
    return old;
}

inline CrossingStrand CrossingStrand::next() const {
    return crossing_->next(strand_);
}

inline CrossingStrand CrossingStrand::prev() const {
    return crossing_->prev(strand_);
}

inline CrossingStrand::operator bool() const {
    return (crossing_ != 0);
}

inline std::ostream& operator << (std::ostream& out, const CrossingStrand& s) {
    return out << (s.strand() == 1 ? '^' : '_') << s.crossing()->index();
}

// Inline functions for Crossing

inline int Crossing::index() const {
    return markedIndex();
}

inline int Crossing::sign() const {
    return sign_;
}

inline CrossingStrand Crossing::next(int strand) const {
    return next_[strand];
}

inline CrossingStrand Crossing::prev(int strand) const {
    return prev_[strand];
}

inline void Crossing::writeTextShort(std::ostream& out) const {
    out << "crossing " << index();
    if (sign_ == 1)
        out << " (+)";
    else
        out << " (-)";
}

inline void Crossing::writeTextLong(std::ostream& out) const {
    out << "Crossing " << index();
    if (sign_ == 1)
        out << " (+)";
    else
        out << " (-)";
    out << '\n';

    /*
    if (sign_ == 1) {
        out << "--\ /-->  " << next_[0] << '\n';
        out << "   \\n"
        out << "--/ \-->  " << next_[1] << std::endl;
    } else {
        out << "--\ /-->  " << next_[1] << '\n';
        out << "   /\n"
        out << "--/ \-->  " << next_[0] << std::endl;
    }
    */

    out << prev_[1] << "  ->  over  ->  " << next_[1] << '\n';
    out << prev_[0] << "  ->  under  ->  " << next_[0] << std::endl;
}

inline Crossing::Crossing() {
}

inline Crossing::Crossing(int sign) : sign_(sign) {
}

// Inline functions for Link

inline Link::Link() {
}

inline Link::~Link() {
    for (Crossing* c : crossings_)
        delete c;
}

inline size_t Link::size() const {
    return crossings_.size();
}

inline size_t Link::countComponents() const {
    return components_.size();
}

inline Crossing* Link::crossing(size_t index) {
    return crossings_[index];
}

inline const Crossing* Link::crossing(size_t index) const {
    return crossings_[index];
}

inline bool Link::dependsOnParent() const {
    return false;
}

inline NPacket* Link::internalClonePacket(NPacket*) const {
    return new Link(*this);
}

inline CrossingStrand Link::translate(const CrossingStrand& other) const {
    return CrossingStrand(crossings_[other.crossing()->index()],
        other.strand());
}

} // namespace regina

#endif

