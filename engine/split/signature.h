
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

/*! \file split/signature.h
 *  \brief Deals with signatures of splitting surfaces.
 */

#ifndef __SIGNATURE_H
#ifndef __DOXYGEN
#define __SIGNATURE_H
#endif

#include "regina-core.h"
#include "output.h"
#include "triangulation/forward.h"
#include <boost/noncopyable.hpp>

namespace regina {

class SigCensus;
class SigPartialIsomorphism;

/**
 * \addtogroup split Splitting Surfaces
 * Splitting surfaces in triangulations.
 * @{
 */

/**
 * Represents a signature of a splitting surface in a closed 3-manifold
 * triangulation.
 *
 * A <i>splitting surface</i> is (for these purposes) a compact normal surface
 * consisting of precisely one quad per tetrahedron and no other normal
 * (or almost normal) discs.
 *
 * A <i>signature</i> of order <i>n</i> is a string consisting of 2<i>n</i>
 * letters arranged into cycles, where <i>n</i> is the number of quads in the
 * splitting surface.  From a signature, the corresponding splitting
 * surface and then the entire 3-manifold triangulation can be recreated.
 *
 * A signature of order <i>n</i> uses the first <i>n</i> letters of the
 * alphabet, each precisely twice.  Case is important; the meaning of a
 * letter changes according to whether it appears in upper-case or
 * lower-case.
 *
 * Each letter represents an individual quadrilateral (the two
 * occurrences of the letter representing the quadrilateral's two sides).
 * Each cycle represents a chain of quadrilaterals joined together in the
 * splitting surface.  The case of a letter represents in which direction
 * a quadrilateral is traversed within a cycle.
 *
 * Cycles are arranged into <i>cycle groups</i>, where a cycle group
 * consists of a series of consecutive cycles all of the same length.
 *
 * An example of a signature is <tt>(abc)(a)(b)(c)</tt>.  This signature
 * is of order 3 and contains two cycle groups, the first being
 * <tt>(abc)</tt> and the second being <tt>(a)(b)(c)</tt>.
 *
 * A signature cannot represent a splitting surface with more than 26
 * quadrilaterals.
 *
 * For further details on splitting surfaces and their signatures, consult
 * <i>Minimal triangulations and normal surfaces</i>, Burton, PhD thesis,
 * available from the Regina website.
 */
class REGINA_API Signature :
        public ShortOutput<Signature>,
        public boost::noncopyable {
    private:
        unsigned order_;
            /**< The number of quads in this splitting surface. */
        unsigned* label;
            /**< The 2<i>n</i> letters making up this signature from
                 start to finish; letters A,B,... are represented by
                 integers 0,1,... . */
        bool* labelInv;
            /**< <tt>labelInv[i]</tt> stores the case of the letter
                 corresponding to <tt>label[i]</tt>.  In this case
                 \c false represents lower-case and \c true represents
                 upper-case. */
        unsigned nCycles;
            /**< The number of cycles in this signature. */
        unsigned* cycleStart;
            /**< The starting position of each cycle; an additional
                 element is appended to the end of this array storing
                 the length of the entire signature. */
        unsigned nCycleGroups;
            /**< The number of cycle groups in this signature. */
        unsigned* cycleGroupStart;
            /**< The starting cycle for each cycle group; an additional
                 element is appended to the end of this array storing
                 the total number of cycles. */

    public:
        /**
         * Creates a new signature that is a clone of the given signature.
         *
         * @param sig the signature to clone.
         */
        Signature(const Signature& sig);
        /**
         * Destroys this signature.
         */
        ~Signature();

        /**
         * Returns the order of this signature.  The order is the number
         * of quads in the corresponding splitting surface.
         *
         * @return the order of this signature.
         */
        unsigned order() const;

        /**
         * Parses the given signature string.
         *
         * Punctuation characters in the given string will be interpreted
         * as separating cycles.  All whitespace will be ignored.
         * 
         * Examples of valid signatures are <tt>"(ab)(bC)(Ca)"</tt> and
         * <tt>"AAb-bc-C"</tt>.  See the class notes for further details
         * on what constitutes a valid signature.
         *
         * \pre The given string contains at least one letter.
         *
         * @param sig a string representation of a splitting surface
         * signature.
         * @return a corresponding newly created signature, or 0 if the
         * given string was invalid.
         */
        static Signature* parse(const std::string& sig);
        /**
         * Returns a newly created 3-manifold triangulation corresponding to
         * this splitting surface signature.
         *
         * @return the corresponding triangulation.
         */
        Triangulation<3>* triangulate() const;

        /**
         * Lexicographically compares the results of transformations upon
         * two given cycles.  Even if transformations are specified, the
         * underlying signatures will not be changed.
         *
         * This comparison is \e not case-sensitive.
         *
         * \pre The two specified cycles have the same length.
         *
         * \ifacespython Not present.
         *
         * @param sig1 the signature containing the first cycle to examine.
         * @param cycle1 specifies which cycle to examine in signature
         * \a sig1.  This must be less than the total number of cycles in
         * \a sig1.
         * @param start1 allows the first cycle to be transformed by
         * rotation; this parameter is the new starting position of the first
         * cycle.  This must be between 0 and
         * <tt>sig1.getCycleLength(cycle1)-1</tt> inclusive.
         * @param dir1 allows the first cycle to be transformed by
         * reversal; this parameter must be positive to use an unreversed
         * cycle or negative to use a reversed cycle.
         * @param relabel1 allows the first cycle to be transformed by
         * relabelling; this parameter must be an array of size at least
         * <tt>sig1.order()</tt> mapping old labels 0,1,...
         * (representing letters A,B,...) to new labels (which must also be
         * 0,1,..., possibly in a different order).  This parameter may
         * be 0 if no relabelling is to be used.
         *
         * @param sig2 the signature containing the second cycle to examine.
         * @param cycle2 specifies which cycle to examine in signature
         * \a sig2.  This must be less than the total number of cycles in
         * \a sig2.
         * @param start2 allows the second cycle to be transformed by
         * rotation; this parameter is the new starting position of the
         * second cycle.  This must be between 0 and
         * <tt>sig2.getCycleLength(cycle2)-1</tt> inclusive.
         * @param dir2 allows the second cycle to be transformed by
         * reversal; this parameter must be positive to use an unreversed
         * cycle or negative to use a reversed cycle.
         * @param relabel2 allows the second cycle to be transformed by
         * relabelling; this parameter must be an array of size at least
         * <tt>sig2.order()</tt> mapping old labels 0,1,...
         * (representing letters A,B,...) to new labels (which must also be
         * 0,1,..., possibly in a different order).  This parameter may
         * be 0 if no relabelling is to be used.
         *
         * @return -1, 1 or 0 if the transformed first cycle is
         * lexicographically less than, greater than or equal to the
         * transformed second cycle respectively.
         */
        static int cycleCmp(const Signature& sig1, unsigned cycle1,
            unsigned start1, int dir1, unsigned* relabel1,
            const Signature& sig2, unsigned cycle2, unsigned start2,
            int dir2, unsigned* relabel2);

        /**
         * Writes a string representation of this signature to the given
         * output stream.
         *
         * \ifacespython The parameter \a out does not exist; standard
         * output will be used.
         *
         * @param out the output stream to which to write.
         * @param cycleOpen the text to write at the beginning of a cycle
         * (such as <tt>"("</tt>).
         * @param cycleClose the text to write at the end of a cycle
         * (such as <tt>")"</tt>).
         * @param cycleJoin the text to write between two cycles.
         */
        void writeCycles(std::ostream& out, const std::string& cycleOpen,
            const std::string& cycleClose, const std::string& cycleJoin) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new completely uninitialised signature.
         *
         * \warning The internal arrays \e must be created before this
         * signature is destroyed!
         */
        Signature();
        /**
         * Creates a new signature of the given order.  All internal
         * arrays will be created but not initialised.
         *
         * The first elements of the \a cycleStart and \a cycleGroupStart
         * arrays will be set to 0.
         *
         * The newly created signature can be used as a partial
         * signature containing no cycles.
         *
         * @param newOrder the order of the new signature; this must be
         * strictly positive.
         */
        Signature(unsigned newOrder);

    friend class regina::SigPartialIsomorphism;
    friend class regina::SigCensus;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSignature has now been renamed to Signature.
 */
REGINA_DEPRECATED typedef Signature NSignature;

/*@}*/

// Inline functions for Signature

inline Signature::Signature() {
}

inline Signature::Signature(unsigned newOrder) : order_(newOrder),
        label(new unsigned[2 * newOrder]), labelInv(new bool[2 * newOrder]),
        nCycles(0), cycleStart(new unsigned[2 * newOrder + 1]),
        nCycleGroups(0), cycleGroupStart(new unsigned[2 * newOrder + 1]) {
    // Insert sentinels.
    cycleStart[0] = cycleGroupStart[0] = 0;
}

inline Signature::~Signature() {
    delete[] label;
    delete[] labelInv;
    delete[] cycleStart;
    delete[] cycleGroupStart;
}

inline unsigned Signature::order() const {
    return order_;
}

inline void Signature::writeTextShort(std::ostream& out) const {
    writeCycles(out, "(", ")", "");
}

} // namespace regina

#endif

