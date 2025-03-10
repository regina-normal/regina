
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include <cstring>
#include <string>
#include "packet/script.h"

#include "gtest/gtest.h"

namespace {
    struct Listener : public regina::PacketListener {
        bool expired_ { false };
        std::weak_ptr<regina::Packet> ptr_;

        void packetBeingDestroyed(regina::PacketShell) override {
            if (ptr_.expired())
                expired_ = true;
        }
    };
}

TEST(ListenersTest, expiration) {
    // This tests the logic used in the Qt user interface, in
    // ScriptUI::packetBeingDestroyed().  See the more extensive
    // comments there.  The short summary: the Qt UI code assumes the
    // behaviour that we are testing here, but I don't think it's
    // actually promised by the C++ standard (hence this test).

    Listener listener;
    {
        auto s = std::make_shared<regina::Script>();
        listener.ptr_ = s;

        s->listen(&listener);

        ASSERT_FALSE(listener.ptr_.expired());
        ASSERT_FALSE(listener.expired_);

        // The weak pointer to the Script managed by s should expire now, since
        // the shared pointer s will go out of scope and destroy the Script that
        // it manages.
        //
        // The behaviour we are testing is whether this expiry happens
        // *before* the Script destructor is called (which means the expiry
        // happens in time for our custom packetBeingDestroyed() to detect it).
    }
    EXPECT_TRUE(listener.expired_);
    EXPECT_TRUE(listener.ptr_.expired());
}
