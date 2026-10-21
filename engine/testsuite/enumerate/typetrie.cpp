
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <array>
#include "enumerate/typetrie.h"

#include "testhelper.h"

using regina::TypeTrie;

class TypeTrieTest : public testing::Test {
    public:
        using Trie = TypeTrie<4>;

    protected:
        void insert(Trie& trie, std::initializer_list<int> types) {
            trie.insert(types.begin(), types.end());
        }

        bool dominates(const Trie& trie, std::initializer_list<int> types) {
            return trie.dominates(types.begin(), types.end());
        }
};

TEST_F(TypeTrieTest, empty) {
    Trie trie;
    EXPECT_FALSE(dominates(trie, {}));
    insert(trie, {});
    EXPECT_TRUE(dominates(trie, {}));
    insert(trie, {});
    EXPECT_TRUE(dominates(trie, {}));
}

TEST_F(TypeTrieTest, basic) {
    Trie trie;
    EXPECT_FALSE(dominates(trie, { 0, 0, 0, 0 }));
    EXPECT_FALSE(dominates(trie, { 2, 2, 1, 1 }));
    insert(trie, { 0, 1, 3, 0 });
    insert(trie, { 2, 0, 1, 0 });
    insert(trie, { 2, 3, 0, 1 });
    EXPECT_FALSE(dominates(trie, { 0, 0, 0, 0 }));
    EXPECT_FALSE(dominates(trie, { 0, 1, 0, 0 }));
    EXPECT_FALSE(dominates(trie, { 0, 0, 1, 0 }));
    EXPECT_FALSE(dominates(trie, { 0, 1, 1, 0 }));
    EXPECT_FALSE(dominates(trie, { 1, 0, 1, 0 }));
    EXPECT_FALSE(dominates(trie, { 1, 1, 1, 1 }));
    EXPECT_FALSE(dominates(trie, { 1, 2, 3, 2 }));
    EXPECT_TRUE(dominates(trie, { 0, 1, 3, 0 }));
    EXPECT_TRUE(dominates(trie, { 1, 1, 3, 3 }));
    EXPECT_TRUE(dominates(trie, { 2, 1, 3, 1 }));
    EXPECT_TRUE(dominates(trie, { 2, 0, 1, 0 }));
    EXPECT_TRUE(dominates(trie, { 2, 2, 1, 0 }));
    EXPECT_TRUE(dominates(trie, { 2, 2, 1, 1 }));
    EXPECT_TRUE(dominates(trie, { 2, 3, 1, 0 }));
    EXPECT_TRUE(dominates(trie, { 2, 3, 0, 1 }));
    EXPECT_TRUE(dominates(trie, { 2, 3, 1, 1 }));
    EXPECT_TRUE(dominates(trie, { 2, 3, 2, 1 }));
    insert(trie, { 1, 2, 3, 2 });
    EXPECT_TRUE(dominates(trie, { 1, 2, 3, 2 }));
    EXPECT_FALSE(dominates(trie, { 3, 2, 3, 1 }));
    insert(trie, { 0, 0, 0, 0 });
    EXPECT_TRUE(dominates(trie, { 0, 0, 0, 0 }));
    EXPECT_TRUE(dominates(trie, { 3, 2, 3, 1 }));
}

