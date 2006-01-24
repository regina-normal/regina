
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nprogresstypes.h
 *  \brief Provides specific methods of representing progress reports.
 */

#ifndef __NPROGRESSTYPES_H
#ifndef __DOXYGEN
#define __NPROGRESSTYPES_H
#endif

#include "progress/nprogress.h"

namespace regina {

/**
 * \weakgroup progress
 * @{
 */

/**
 * A progress report in which the current state of progress is stored as a
 * string message.
 *
 * \ifacespython Not present; all progress classes communicate with
 * external interfaces through the NProgress interface.
 */
class NProgressMessage : public NProgress {
    private:
        std::string message;
            /**< The current state of progress. */

    public:
        /**
         * Creates a new progress report with an empty progress message.
         * Note that the internal mutex is not locked during construction.
         */
        NProgressMessage();
        /**
         * Creates a new progress report with the given progress message.
         * Note that the internal mutex is not locked during construction.
         *
         * @param newMessage the current state of progress.
         */
        NProgressMessage(const std::string& newMessage);
        /**
         * Creates a new progress report with the given progress message.
         * Note that the internal mutex is not locked during construction.
         *
         * @param newMessage the current state of progress.
         */
        NProgressMessage(const char* newMessage);

        /**
         * Returns a reference to the current progress message.
         *
         * @return the current progress message.
         */
        std::string getMessage() const;
        /**
         * Sets the current progress message to the given string.
         *
         * @param newMessage the new state of progress.
         */
        void setMessage(const std::string& newMessage);
        /**
         * Sets the current progress message to the given string.
         *
         * @param newMessage the new state of progress.
         */
        void setMessage(const char* newMessage);

    protected:
        virtual std::string internalGetDescription() const;
};

/**
 * A simple structure used for passing around a numeric state of
 * progress.
 */
struct NProgressStateNumeric {
    long completed;
        /**< The number of items that have already been completed. */
    long outOf;
        /**< The expected total number of items, or -1 if this is not known. */

    /**
     * Initialises a new structure using the given values.
     *
     * @param newCompleted the number of items that have already been
     * completed.
     * @param newOutOf the expected total number of items, or -1 if this
     * is not known.
     */
    NProgressStateNumeric(long newCompleted = 0, long newOutOf = -1);
};

/**
 * A progress report in which the current state of progress is stored as
 * a number of items completed.
 * The expected total number of items can be optionally specified.
 *
 * \ifacespython Not present; all progress classes communicate with
 * external interfaces through the NProgress interface.
 */
class NProgressNumber : public NProgress {
    private:
        long completed;
            /**< The number of items completed. */
        long outOf;
            /**< The expected total number of items, or -1 if this
             *   total is not known. */

    public:
        /**
         * Creates a new progress report containing the given details.
         * Note that the internal mutex is not locked during construction.
         *
         * \pre The new number of items completed is non-negative.
         * \pre If the new expected total is non-negative, then the new
         * number of items completed is at most the new expected total.
         *
         * @param newCompleted the number of items completed; this
         * defaults to 0.
         * @param newOutOf the expected total number of items, or -1 if
         * this total is not known (the default).
         */
        NProgressNumber(long newCompleted = 0, long newOutOf = -1);

        /**
         * Returns the number of items completed.
         *
         * @return the number of items completed.
         */
        long getCompleted() const;
        /**
         * Returns the expected total number of items.
         *
         * @return the expected total number of items, or -1 if this
         * total is not known.
         */
        long getOutOf() const;
        /**
         * Returns both the number of items completed and the expected
         * total number of items.
         *
         * @return the current state of progress.
         */
        NProgressStateNumeric getNumericState() const;
        /**
         * Sets the number of items completed.
         *
         * \pre The new number of items completed is non-negative.
         * \pre If the expected total is non-negative, then the new
         * number of items completed is at most the expected total.
         *
         * @param newCompleted the number of items completed.
         */
        void setCompleted(long newCompleted);
        /**
         * Increases the number of items completed by the given amount.
         *
         * \pre If the expected total is non-negative, then the new
         * total number of items completed is at most the expected total.
         *
         * @param extraCompleted the number of items to add to the number of
         * items already completed.
         */
        void incCompleted(unsigned long extraCompleted = 1);
        /**
         * Sets the expected total number of items.
         *
         * \pre If the new expected total is non-negative, then the
         * new expected total is at least the number of items completed.
         *
         * @param newOutOf the expected total number of items, or -1 if
         * this total is not known.
         */
        void setOutOf(long newOutOf);

        virtual bool isPercent() const;

    protected:
        virtual std::string internalGetDescription() const;
        virtual double internalGetPercent() const;
};

/*@}*/

// Inline functions for NProgressMessage

inline NProgressMessage::NProgressMessage() : NProgress() {
}
inline NProgressMessage::NProgressMessage(const std::string& newMessage) :
        NProgress(), message(newMessage) {
}
inline NProgressMessage::NProgressMessage(const char* newMessage) :
        NProgress(), message(newMessage) {
}

inline std::string NProgressMessage::getMessage() const {
    MutexLock(this);
    changed = false;
    return message;
}
inline void NProgressMessage::setMessage(const std::string& newMessage) {
    MutexLock(this);
    message = newMessage;
    changed = true;
}
inline void NProgressMessage::setMessage(const char* newMessage) {
    MutexLock(this);
    message = newMessage;
    changed = true;
}

inline std::string NProgressMessage::internalGetDescription() const {
    MutexLock(this);
    return message;
}

// Inline functions for NProgressStateNumeric

inline NProgressStateNumeric::NProgressStateNumeric(long newCompleted,
        long newOutOf) : completed(newCompleted), outOf(newOutOf) {
}

// Inline functions for NProgressNumber

inline NProgressNumber::NProgressNumber(long newCompleted, long newOutOf) :
        NProgress(), completed(newCompleted), outOf(newOutOf) {
}

inline long NProgressNumber::getCompleted() const {
    MutexLock(this);
    changed = false;
    return completed;
}
inline long NProgressNumber::getOutOf() const {
    MutexLock(this);
    changed = false;
    return outOf;
}
inline NProgressStateNumeric NProgressNumber::getNumericState() const {
    MutexLock(this);
    changed = false;
    return NProgressStateNumeric(completed, outOf);
}
inline void NProgressNumber::setCompleted(long newCompleted) {
    MutexLock(this);
    completed = newCompleted;
    changed = true;
}
inline void NProgressNumber::incCompleted(unsigned long extraCompleted) {
    MutexLock(this);
    completed += extraCompleted;
    changed = true;
}
inline void NProgressNumber::setOutOf(long newOutOf) {
    MutexLock(this);
    outOf = newOutOf;
    changed = true;
}

inline bool NProgressNumber::isPercent() const {
    MutexLock(this);
    return (outOf >= 0);
}

inline double NProgressNumber::internalGetPercent() const {
    MutexLock(this);
    return (outOf > 0 ? double(completed) * 100 / double(outOf) : double(0));
}

} // namespace regina

#endif

