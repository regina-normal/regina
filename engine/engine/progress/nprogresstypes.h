
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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
 * \ifaces Not present.
 */
class NProgressMessage : public NProgress {
    private:
        std::string message;
            /**< The current state of progress. */

    public:
        /**
         * Creates a new progress report with an empty progress message.
         * Note that the internal mutex is not locked during construction.
         *
         * @param newCancellable \c true if and only if this operation
         * allows itself to be cancelled by an external interface.
         */
        NProgressMessage(bool newCancellable = false);
        /**
         * Creates a new progress report with the given progress message.
         * Note that the internal mutex is not locked during construction.
         *
         * @param newMessage the current state of progress.
         * @param newCancellable \c true if and only if this operation
         * allows itself to be cancelled by an external interface.
         */
        NProgressMessage(const std::string& newMessage,
                bool newCancellable = false);
        /**
         * Creates a new progress report with the given progress message.
         * Note that the internal mutex is not locked during construction.
         *
         * @param newMessage the current state of progress.
         * @param newCancellable \c true if and only if this operation
         * allows itself to be cancelled by an external interface.
         */
        NProgressMessage(const char* newMessage, bool newCancellable = false);

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
 * A progress report in which the current state of progress is stored as
 * a number of items completed.
 * The expected total number of items can be optionally specified.
 *
 * \ifaces Not present.
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
         * @param newCancellable \c true if and only if this operation
         * allows itself to be cancelled by an external interface.
         */
        NProgressNumber(long newCompleted = 0, long newOutOf = -1,
                bool newCancellable = false);

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

inline NProgressMessage::NProgressMessage(bool newCancellable) :
        NProgress(newCancellable) {
}
inline NProgressMessage::NProgressMessage(const std::string& newMessage,
        bool newCancellable) : NProgress(newCancellable),
        message(newMessage) {
}
inline NProgressMessage::NProgressMessage(const char* newMessage,
        bool newCancellable) : NProgress(newCancellable),
        message(newMessage) {
}

inline std::string NProgressMessage::getMessage() const {
    mutexLock();
    std::string ans = message;
    mutexUnlock();
    return ans;
}
inline void NProgressMessage::setMessage(const std::string& newMessage) {
    mutexLock();
    message = newMessage;
    setChanged();
    mutexUnlock();
}
inline void NProgressMessage::setMessage(const char* newMessage) {
    mutexLock();
    message = newMessage;
    setChanged();
    mutexUnlock();
}

inline std::string NProgressMessage::internalGetDescription() const {
    mutexLock();
    std::string ans = message;
    mutexUnlock();
    return ans;
}

// Inline functions for NProgress

inline NProgressNumber::NProgressNumber(long newCompleted, long newOutOf,
        bool newCancellable) : NProgress(newCancellable),
        completed(newCompleted), outOf(newOutOf) {
}

inline long NProgressNumber::getCompleted() const {
    mutexLock();
    long ans = completed;
    mutexUnlock();
    return ans;
}
inline long NProgressNumber::getOutOf() const {
    mutexLock();
    long ans = outOf;
    mutexUnlock();
    return ans;
}
inline void NProgressNumber::setCompleted(long newCompleted) {
    mutexLock();
    completed = newCompleted;
    setChanged();
    mutexUnlock();
}
inline void NProgressNumber::setOutOf(long newOutOf) {
    mutexLock();
    outOf = newOutOf;
    setChanged();
    mutexUnlock();
}

inline bool NProgressNumber::isPercent() const {
    mutexLock();
    bool ans = (outOf >= 0);
    mutexUnlock();
    return ans;
}

inline double NProgressNumber::internalGetPercent() const {
    mutexLock();
    double ans = double(completed) * 100 / double(outOf);
    mutexUnlock();
    return ans;
}

} // namespace regina

#endif

