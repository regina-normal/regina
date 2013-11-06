
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file packet/nxmltreeresolver.h
 *  \brief Support for resolving dangling packet references after a
 *  complete packet tree has been read from file.
 */

#ifndef __NXMLTREERESOLVER_H
#ifndef __DOXYGEN
#define __NXMLTREERESOLVER_H
#endif

#include "regina-core.h"
#include <list>
#include <map>

namespace regina {

class NPacket;

/**
 * \weakgroup packet
 * @{
 */

class NXMLTreeResolver;

class REGINA_API NXMLTreeResolutionTask {
    public:
        virtual ~NXMLTreeResolutionTask();
        virtual void resolve(const NXMLTreeResolver& resolver) = 0;
};

class REGINA_API NXMLTreeResolver {
    public:
        typedef std::map<std::string, NPacket*> IDMap;

    private:
        IDMap ids_;
        std::list<NXMLTreeResolutionTask*> tasks_;

    public:
        NXMLTreeResolver();
        ~NXMLTreeResolver();

        void queueTask(NXMLTreeResolutionTask* task);
        void storeID(const std::string& id, NPacket* packet);

        const IDMap& ids() const;

        void resolve();
};

/*@}*/

// Inline functions for NXMLTreeResolutionTask

inline NXMLTreeResolutionTask::~NXMLTreeResolutionTask() {
}

// Inline functions for NXMLTreeResolver

inline NXMLTreeResolver::NXMLTreeResolver() {
}

inline NXMLTreeResolver::~NXMLTreeResolver() {
    for (std::list<NXMLTreeResolutionTask*>::iterator it = tasks_.begin();
            it != tasks_.end(); ++it)
        delete *it;
}

inline void NXMLTreeResolver::queueTask(NXMLTreeResolutionTask* task) {
    tasks_.push_back(task);
}

inline void NXMLTreeResolver::storeID(const std::string& id, NPacket* packet) {
    ids_.insert(std::make_pair(id, packet));
}

inline const NXMLTreeResolver::IDMap& NXMLTreeResolver::ids() const {
    return ids_;
}

inline void NXMLTreeResolver::resolve() {
    for (std::list<NXMLTreeResolutionTask*>::iterator it = tasks_.begin();
            it != tasks_.end(); ++it) {
        (*it)->resolve(*this);
        delete *it;
    }
    tasks_.clear();
}

} // namespace regina

#endif

