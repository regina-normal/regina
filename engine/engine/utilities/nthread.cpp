
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nthread.h"
#else
    #include "engine/utilities/nthread.h"
#endif

struct NThreadRuntimeArgs {
    NThread* thread;
    void* args;
    bool deleteAfterwards;

    NThreadRuntimeArgs(NThread* newThread, void* newArgs,
            bool newDeleteAfterwards) : thread(newThread), args(newArgs),
            deleteAfterwards(newDeleteAfterwards) {
    }
};

void* NThreadRuntime(void* runtimeArgs) {
    // Get the arguments.
    NThread* thread = ((NThreadRuntimeArgs*)runtimeArgs)->thread;
    void* args = ((NThreadRuntimeArgs*)runtimeArgs)->args;
    bool deleteAfterwards = ((NThreadRuntimeArgs*)runtimeArgs)->
        deleteAfterwards;
    delete (NThreadRuntimeArgs*)runtimeArgs;

    // Do the work.
    void* ans = thread->run(args);
    if (deleteAfterwards)
        delete thread;
    return ans;
}

bool NThread::start(void* args, bool deleteAfterwards) {
    NThreadID tmpID;
    return (pthread_create(&tmpID, 0, NThreadRuntime,
        new NThreadRuntimeArgs(this, args, deleteAfterwards)) == 0);
}

bool NThread::start(void* (*routine)(void*), void* args, NThreadID* id) {
    if (id)
        return (pthread_create(id, 0, routine, args) == 0);
    else {
        NThreadID tmpID;
        return (pthread_create(&tmpID, 0, routine, args) == 0);
    }
}

