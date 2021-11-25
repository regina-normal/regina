
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

#include "manifold/sfs.h"
#include "split/signature.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

namespace regina {

Triangulation<3> Example<3>::bingsHouse() {
    Triangulation<3> ans;

    auto [r, s] = ans.newTetrahedra<2>();
    r->join(0, r, Perm<4>(0, 1));
    s->join(0, s, Perm<4>(0, 1));
    r->join(2, s, Perm<4>(3, 1, 2, 0));
    s->join(3, r, Perm<4>(2, 1, 0, 3));

    return ans;
}

Triangulation<3> Example<3>::rp2xs1() {
    // Section 3.5.1 of Benjamin Burton's PhD thesis describes how to
    // construct RP^2 x S^1 by identifying the boundary triangles of a
    // three-tetrahedron solid Klein bottle.
    Triangulation<3> ans;

    auto [r, s, t] = ans.newTetrahedra<3>();
    s->join(0, r, Perm<4>(0, 1, 2, 3));
    s->join(3, r, Perm<4>(3, 0, 1, 2));
    s->join(1, t, Perm<4>(3, 0, 1, 2));
    s->join(2, t, Perm<4>(0, 1, 2, 3));
    r->join(1, t, Perm<4>(2, 3, 0, 1));
    r->join(3, t, Perm<4>(2, 3, 0, 1));

    return ans;
}

Triangulation<3> Example<3>::rp3rp3() {
    // This can be generated as the enclosing triangulation of a splitting
    // surface, as described in chapter 4 of Benjamin Burton's PhD thesis.
    return Signature::parse("aabccd.b.d").triangulate();
}

Triangulation<3> Example<3>::lens(size_t p, size_t q) {
    Triangulation<3> ans;

    Tetrahedron<3>* chain;
    if (p == 0) {
        chain = ans.insertLayeredSolidTorus(1, 1);
        chain->join(3, chain, Perm<4>(3, 0, 1, 2));
    } else if (p == 1) {
        chain = ans.insertLayeredSolidTorus(1, 2);
        chain->join(3, chain, Perm<4>(0, 1, 3, 2));
    } else if (p == 2) {
        chain = ans.insertLayeredSolidTorus(1, 3);
        chain->join(3, chain, Perm<4>(0, 1, 3, 2));
    } else if (p == 3) {
        chain = ans.insertLayeredSolidTorus(1, 1);
        // Either of the following gluings will work.
        chain->join(3, chain, Perm<4>(1, 3, 0, 2));
        // chain->join(3, chain, Perm<4>(0, 1, 3, 2));
    } else {
        if (2 * q > p)
            q = p - q;
        if (3 * q > p) {
            chain = ans.insertLayeredSolidTorus(p - 2 * q, q);
            chain->join(3, chain, Perm<4>(1, 3, 0, 2));
        } else {
            chain = ans.insertLayeredSolidTorus(q, p - 2 * q);
            chain->join(3, chain, Perm<4>(3, 0, 1, 2));
        }
    }

    return ans;
}

Triangulation<3> Example<3>::layeredLoop(size_t length, bool twisted) {
    Triangulation<3> ans;
    if (length == 0)
        return ans;

    // Ensure only one event pair is fired in this sequence of changes.
    Triangulation<3>::ChangeEventSpan span(ans);

    // Insert a layered chain of the given length.
    // We should probably split this out into a separate routine.
    Tetrahedron<3>* base;
    Tetrahedron<3>* curr;
    Tetrahedron<3>* next;

    base = ans.newTetrahedron();
    curr = base;

    for (size_t i = 1; i < length; i++) {
        next = ans.newTetrahedron();
        curr->join(0, next, Perm<4>(1, 0, 2, 3));
        curr->join(3, next, Perm<4>(0, 1, 3, 2));
        curr = next;
    }

    // Join the two ends of the layered chain.
    if (twisted) {
        curr->join(0, base, Perm<4>(2, 3, 1, 0));
        curr->join(3, base, Perm<4>(3, 2, 0, 1));
    } else {
        curr->join(0, base, Perm<4>(1, 0, 2, 3));
        curr->join(3, base, Perm<4>(0, 1, 3, 2));
    }

    return ans;
}

Triangulation<3> Example<3>::augTriSolidTorus(long a1, long b1,
        long a2, long b2, long a3, long b3) {
    Triangulation<3> ans;

    // Ensure only one event pair is fired in this sequence of changes.
    Triangulation<3>::ChangeEventSpan span(ans);

    // Construct the core triangular solid torus.
    auto core = ans.newTetrahedra<3>();
    for (int i = 0; i < 3; i++)
        core[i]->join(0, core[(i + 1) % 3], Perm<4>(3, 0, 1, 2));

    // Attach the external layered solid tori.
    long axis, major, minor;
    unsigned long absAxis, absMajor, absMinor;
    Tetrahedron<3>* lstTop;
    for (int i = 0; i < 3; i++) {
        if (i == 0) axis = a1; else if (i == 1) axis = a2; else axis = a3;
        if (i == 0) major = b1; else if (i == 1) major = b2; else major = b3;
        minor = -(axis + major);

        absAxis = (axis < 0 ? -axis : axis);
        absMajor = (major < 0 ? -major : major);
        absMinor = (minor < 0 ? -minor : minor);

        // Are we simply attaching a mobius band?
        if (absAxis <= 2 && absMajor <= 2 && absMinor <= 2) {
            // We have either (2,1,1) or (1,1,0).
            if (absAxis == 2) {
                core[i]->join(2, core[(i + 1) % 3], Perm<4>(0, 2, 1, 3));
                continue;
            } else if (absMajor == 2) {
                core[i]->join(2, core[(i + 1) % 3], Perm<4>(2, 3, 1, 0));
                continue;
            } else if (absMinor == 2) {
                core[i]->join(2, core[(i + 1) % 3], Perm<4>(3, 0, 1, 2));
                continue;
            }

            // It's (1,1,0).  But this needs to be handled specially anyway.
            lstTop = ans.insertLayeredSolidTorus(0, 1);
            if (absAxis == 0) {
                core[i]->join(2, lstTop, Perm<4>(0, 2, 3, 1));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(0, 2, 3, 1));
            } else if (absMajor == 0) {
                core[i]->join(2, lstTop, Perm<4>(1, 0, 3, 2));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(3, 2, 1, 0));
            } else {
                core[i]->join(2, lstTop, Perm<4>(3, 0, 2, 1));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(0, 3, 1, 2));
            }
            continue;
        }

        if (absAxis >= absMajor && absAxis >= absMinor) {
            // Most cuts on the axis edges.
            if (absMinor <= absMajor) {
                // (minor, major, axis)
                lstTop = ans.insertLayeredSolidTorus(absMinor, absMajor);
                core[i]->join(2, lstTop, Perm<4>(0, 2, 3, 1));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(0, 2, 3, 1));
            } else {
                // (major, minor, axis)
                lstTop = ans.insertLayeredSolidTorus(absMajor, absMinor);
                core[i]->join(2, lstTop, Perm<4>(1, 2, 3, 0));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(1, 2, 3, 0));
            }
        } else if (absMajor >= absMinor) {
            // Most cuts on the major edges.
            if (absMinor <= absAxis) {
                // (minor, axis, major)
                lstTop = ans.insertLayeredSolidTorus(absMinor, absAxis);
                core[i]->join(2, lstTop, Perm<4>(0, 1, 3, 2));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(3, 2, 0, 1));
            } else {
                // (axis, minor, major)
                lstTop = ans.insertLayeredSolidTorus(absAxis, absMinor);
                core[i]->join(2, lstTop, Perm<4>(1, 0, 3, 2));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(3, 2, 1, 0));
            }
        } else {
            // Most cuts on the minor edges.
            if (absAxis <= absMajor) {
                // (axis, major, minor)
                lstTop = ans.insertLayeredSolidTorus(absAxis, absMajor);
                core[i]->join(2, lstTop, Perm<4>(3, 1, 2, 0));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(1, 3, 0, 2));
            } else {
                // (major, axis, minor)
                lstTop = ans.insertLayeredSolidTorus(absMajor, absAxis);
                core[i]->join(2, lstTop, Perm<4>(3, 0, 2, 1));
                core[(i + 1) % 3]->join(1, lstTop, Perm<4>(0, 3, 1, 2));
            }
        }
    }

    return ans;
}

Triangulation<3> Example<3>::poincare() {
    return Triangulation<3>::fromGluings(5, {
        { 0, 0, 1, {0,3,2,1} }, { 0, 1, 2, {3,0,1,2} }, { 0, 2, 3, {3,2,0,1} },
        { 0, 3, 4, {3,1,2,0} }, { 1, 1, 2, {2,1,0,3} }, { 1, 2, 4, {2,3,1,0} },
        { 1, 3, 3, {2,0,3,1} }, { 2, 2, 3, {1,2,3,0} }, { 2, 3, 4, {3,0,1,2} },
        { 3, 2, 4, {1,2,3,0} }
    });
}

Triangulation<3> Example<3>::sfsOverSphere(long a1, long b1, long a2, long b2,
        long a3, long b3) {
    // Use the SFS construction routine, which can handle this type of SFS.

    SFSpace sfs;
    if (a1 < 0)
        sfs.insertFibre(-a1, -b1);
    else
        sfs.insertFibre(a1, b1);
    if (a2 < 0)
        sfs.insertFibre(-a2, -b2);
    else
        sfs.insertFibre(a2, b2);
    if (a3 < 0)
        sfs.insertFibre(-a3, -b3);
    else
        sfs.insertFibre(a3, b3);

    sfs.reduce();
    return sfs.construct();
}

Triangulation<3> Example<3>::weeks() {
    return Triangulation<3>::fromGluings(9, {
        { 0, 0, 0, {1,2,3,0} }, { 0, 2, 1, {3,2,0,1} }, { 0, 3, 2, {2,3,1,0} },
        { 1, 1, 3, {1,0,2,3} }, { 1, 2, 4, {1,3,0,2} }, { 1, 3, 5, {2,3,1,0} },
        { 2, 1, 3, {0,1,3,2} }, { 2, 2, 4, {0,2,1,3} }, { 2, 3, 6, {1,2,3,0} },
        { 3, 2, 5, {2,3,1,0} }, { 3, 3, 7, {2,3,1,0} }, { 4, 2, 7, {0,2,1,3} },
        { 4, 3, 8, {2,3,1,0} }, { 5, 2, 6, {0,2,1,3} }, { 5, 3, 8, {3,2,0,1} },
        { 6, 2, 8, {1,0,2,3} }, { 6, 3, 7, {2,1,0,3} }, { 7, 2, 8, {1,2,3,0} }
    });
}

Triangulation<3> Example<3>::weberSeifert() {
    // Bah.  Dehydration strings are somewhat impenetrable,
    // but the alternative is 23 lines of hard-coded tetrahedron gluings.
    //
    // This triangulation was constructed by building a 60-tetrahedron
    // dodecahedron and identifying opposite pentagonal faces with a 3/10 twist,
    // and then simplifying down to one vertex and 23 tetrahedra.
    Triangulation<3> ans = Triangulation<3>::rehydrate(
        "xppphocgaeaaahimmnkontspmuuqrsvuwtvwwxwjjsvvcxxjjqattdwworrko");
    ans.orient();
    return ans;
}

Triangulation<3> Example<3>::smallClosedOrblHyperbolic() {
    return Triangulation<3>::fromGluings(9, {
        { 0, 0, 6, {0,1,3,2} }, { 0, 1, 8, {3,1,2,0} }, { 0, 2, 2, {0,2,1,3} },
        { 0, 3, 8, {0,2,1,3} }, { 1, 0, 6, {3,1,2,0} }, { 1, 1, 8, {1,0,2,3} },
        { 1, 2, 3, {3,2,0,1} }, { 1, 3, 7, {2,3,1,0} }, { 2, 0, 7, {2,0,3,1} },
        { 2, 2, 3, {0,1,3,2} }, { 2, 3, 4, {3,1,2,0} }, { 3, 1, 5, {3,2,0,1} },
        { 3, 2, 5, {2,1,0,3} }, { 4, 1, 6, {0,1,3,2} }, { 4, 2, 5, {0,1,3,2} },
        { 4, 3, 7, {3,2,0,1} }, { 5, 1, 8, {0,2,1,3} }, { 6, 2, 7, {0,1,3,2} }
    });
}

Triangulation<3> Example<3>::smallClosedNonOrblHyperbolic() {
    return Triangulation<3>::fromGluings(11, {
        { 0, 0, 8, {1,3,2,0} }, { 0, 1, 2, {0,3,2,1} }, { 0, 2, 8, {2,1,0,3} },
        { 0, 3, 2, {3,1,0,2} }, { 1, 0, 5, {3,0,1,2} }, { 1, 1, 3, {3,1,0,2} },
        { 1, 2, 2, {2,1,0,3} }, { 1, 3, 9, {1,0,3,2} }, { 2, 1, 4, {3,1,2,0} },
        { 3, 0, 6, {2,1,3,0} }, { 3, 2, 4, {2,0,3,1} }, { 3, 3, 6, {0,3,2,1} },
        { 4, 0, 10, {2,1,0,3} }, { 4, 2, 10, {3,2,1,0} }, { 5, 0, 7, {3,1,2,0} },
        { 5, 1, 7, {1,0,3,2} }, { 5, 2, 6, {0,1,3,2} }, { 6, 0, 8, {2,1,0,3} },
        { 7, 1, 9, {0,3,2,1} }, { 7, 2, 8, {0,1,3,2} }, { 9, 0, 10, {3,1,2,0} },
        { 9, 1, 10, {2,0,1,3} }
    });
}

Triangulation<3> Example<3>::sphere600() {
    // See 600cell.py for the code that converts the Benedetti-Lutz
    // simplicial complex to a Regina triangulation.
    Triangulation<3> ans = Triangulation<3>::fromIsoSig("-cyjvvvvvzzzvzzvvwvLLvLvPPvLvPzAPvzAMPzwLvzPvLPzwwMzvPzMzLwzMzMvALMLzvzMPwzwMALMvvMALzMPvAwvzwzMzAwvwwwwMAzMzQLMvzMPAwzzzMLPzvMzMMwwwwMzAwPMzvwzMzAwPMvAzMzAQLLvQzwwzPMvAzQMwwPzwLwPzLMAPPvQAvAMvzQLPwPAwPAMAMvPPzLMQvPzwMAwPAMwALMQwPzwPQwPAMwALMvwzQQAQzALMvMzvAMvwPAMwzvQMQvMzMzQMzzLPQzzMPzPAMMQMLzAwQAPPQPzvLPQzLzPQzLLQMzvMMPQzzPQPvLPzPPQPvzAwAPPQAzPQQLAQQAQPwvLPQPAPwvvzwvvLQMAQLLQQQQLLQQQQQLQQQQQAMMQMQAQialaoamapaqatavawazaPaBaRaCaFaTaHaVa3aIa6aLaXaNaZa7aOa+aebgbSanbWaqbrb0aubybAb2anb5aFbrb9aHbMbObabcbTbfbVbybYbObibkb4bmb6b9bpbGbtbIbecgcwbUbzbWbmcgcCb5bEb7btcycAcXbKbNbEcycZb8bQbSbLc+bPcScPcVc1b-b3bZcac4c7c4c+ccdlcccDcfcfdAcncscicKckclducScpdpcvc1crctdwc7cxdBdzcFcgdCcMcmdVcLdHcNcJcOdOc3c+cTdWdRcWcqdUc0cudKd2cYcRdcd-dbe6c-cyd9cddCdSdedbdYdaedepdLdidndkdteodwdxdTdzeKdvdsdweBd-dIeSdDdAdBeaeKeFdIdQdHdSeJdveVeNdPdTeWd4ebeVdXdWe5ede3d0d7d2dcf4dEeff6d8d0edfkf+dcegfjfiefemehetfNejewfle9eneufBfpemfqexfAfseueUezeJfIeyeAeXeKfKeDeFeFfefTfHeJehfSfMeMfOevf2fQeVfReyf1fVe4eJf5eKfZeifRf1eGfag3e6ebglf8e0fzfNf+eigafdgbfjgCfffkfTfjfSfagbgofxgWfpf4fDfrfhgsfygmgEfwfBf2fAf1figjgxgygNgIfLfOgUfVgPfQgQfWg3fNgOg4gYfUgZf5gZg5fVgWg4g5g7f8fcgPg+fch-fkgXgPgfgkhdhggfhlglhXgkhlhogpgtg0grgthsgzg6g0gvgwhAguhwgnh6gnhCgDgHg8gFgGgEhLgah8gJgMgHhKgFhshahshSgQhehTgghYgRhQhRh2gxh7gvh3gThTh+gbhIh-gGhYhYh9h+h9h+hihyhjhzhmh-h-hphqhJhrhKhbibilimilimininiBhChLhDhMhpipiviwiviwixixiziziOhZhPh0hShdiaiaiVhWh5hXh6hiicicirioioi2h3h7h4h8htiqiqiBiyiyiDiAiAirifigijihikisisiuiBiCiCiEiDiEiGiHiJiIiKiLiLiMiNiNiOiOiQiRiTiSiUiWiViVihjYiXiXiijYi0i1i3i2i4i6i5i5ijj8i7i7ikjnj8ioj+ibj-icjajejdjdjljgjfjfjmjpjgjqjrjsjijkjtjmjujvjojtjqjujwjsjvjwjxjxjxjgovcfbcfboivlbgmvpifdgtoulcepdbifdlcepdbavpaoupdfvlgtalacoigmivnospdvkgwlaojgripdsfgtarncgmwleauckoisidaplsfbgtgrncbgmwouckfoisvplgwqgrxceckoisvfdloshojxhncqbdpvnhvkqhsfxqxccckoisvfflgncqbcpgsfxoxckfoistlojxbdovkqqcncbgmwupgrexdvnhhsfbgtprugdwqeoeshhojwbdovkrqcgresdvnjhgdwneoeskhbcmoigklwxpvnhngijqruoeshsffqgmxigtirmrgmwqiwccxgtqlhppvnhnuoeshsxitgmwqiwqlhpvnjpoeskuagtirmrqahpvkrlgdwntagijqrlaaojwigresmagklwxiaagtcrorqhpggjfrlagkgwciatqhpgvwgntagrosgmauhqluaojgwoiaphxipalagvwgnovggrosgvoggvoojgwovgogovogvcojgwocgfffgcbbojgwocgffgcbojgwovgologvibgrosgvogpgovmdfigvwgnovgugvotecldpbficlpvpoudgtldla");
    ans.orient();
    return ans;
}

Triangulation<3> Example<3>::lst(size_t a, size_t b) {
    if (a > b)
        std::swap(a, b);

    Triangulation<3> ans;
    ans.insertLayeredSolidTorus(a, b);
    return ans;
}

Triangulation<3> Example<3>::figureEight() {
    Triangulation<3> ans;

    // The two-tetrahedron figure eight knot complement is described at
    // the beginning of chapter 8 of Richard Rannard's PhD thesis.
    auto [r, s] = ans.newTetrahedra<2>();
    r->join(0, s, Perm<4>(1, 3, 0, 2));
    r->join(1, s, Perm<4>(2, 0, 3, 1));
    r->join(2, s, Perm<4>(0, 3, 2, 1));
    r->join(3, s, Perm<4>(2, 1, 0, 3));

    return ans;
}

Triangulation<3> Example<3>::trefoil() {
    Triangulation<3> ans;

    auto [r, s] = ans.newTetrahedra<2>();
    r->join(0, s, Perm<4>(2, 3));
    r->join(1, s, Perm<4>(2, 3));
    r->join(2, s, Perm<4>(1, 3));
    r->join(3, s, Perm<4>(0, 2));

    return ans;
}

Triangulation<3> Example<3>::whiteheadLink() {
    return Triangulation<3>::fromGluings(4, {
        { 0, 0, 3, {2,3,1,0} }, { 0, 1, 2, {3,2,0,1} }, { 0, 2, 1, {0,1,3,2} },
        { 0, 3, 3, {3,2,0,1} }, { 1, 0, 3, {3,2,0,1} }, { 1, 1, 2, {2,3,1,0} },
        { 1, 2, 2, {3,2,0,1} }, { 2, 1, 3, {1,0,2,3} }
    });
}

Triangulation<3> Example<3>::gieseking() {
    Triangulation<3> ans;

    Tetrahedron<3>* r = ans.newTetrahedron();
    r->join(0, r, Perm<4>(1, 2, 0, 3));
    r->join(2, r, Perm<4>(0, 2, 3, 1));

    return ans;
}

Triangulation<3> Example<3>::cuspedGenusTwoTorus() {
    Triangulation<3> ans;

    // We create this by first constructing an ordinary solid genus two
    // torus and then converting the real boundary to an ideal vertex.
    auto [r, s, t, u] = ans.newTetrahedra<4>();
    r->join(0, s, Perm<4>());
    r->join(1, t, Perm<4>(1, 2, 3, 0));
    r->join(2, u, Perm<4>(1, 0, 3, 2));
    s->join(3, t, Perm<4>());
    t->join(1, u, Perm<4>());
    ans.finiteToIdeal();

    return ans;
}

} // namespace regina

