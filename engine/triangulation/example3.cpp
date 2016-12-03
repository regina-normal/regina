
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

#include "split/signature.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

namespace {
    static const int poincareAdj[5][4] = {
        { 1, 2, 3, 4},
        { 0, 2, 4, 3},
        { 0, 1, 3, 4},
        { 0, 1, 4, 2},
        { 0, 1, 2, 3}
    };

    static const int poincareGluings[5][4][4] = {
        { { 0, 3, 2, 1 }, { 3, 0, 1, 2 }, { 3, 2, 0, 1 }, { 3, 1, 2, 0 } },
        { { 0, 3, 2, 1 }, { 2, 1, 0, 3 }, { 2, 3, 1, 0 }, { 2, 0, 3, 1 } },
        { { 1, 2, 3, 0 }, { 2, 1, 0, 3 }, { 1, 2, 3, 0 }, { 3, 0, 1, 2 } },
        { { 2, 3, 1, 0 }, { 1, 3, 0, 2 }, { 1, 2, 3, 0 }, { 3, 0, 1, 2 } },
        { { 3, 1, 2, 0 }, { 3, 2, 0, 1 }, { 1, 2, 3, 0 }, { 3, 0, 1, 2 } }
    };

    static const int weeksAdj[9][4] = {
        { 0, 0, 1, 2},
        { 0, 3, 4, 5},
        { 0, 3, 4, 6},
        { 1, 2, 5, 7},
        { 1, 2, 7, 8},
        { 1, 3, 6, 8},
        { 2, 5, 8, 7},
        { 3, 4, 8, 6},
        { 4, 5, 6, 7}
    };

    static const int weeksGluings[9][4][4] = {
        { { 1, 2, 3, 0 }, { 3, 0, 1, 2 }, { 3, 2, 0, 1 }, { 2, 3, 1, 0 } },
        { { 2, 3, 1, 0 }, { 1, 0, 2, 3 }, { 1, 3, 0, 2 }, { 2, 3, 1, 0 } },
        { { 3, 2, 0, 1 }, { 0, 1, 3, 2 }, { 0, 2, 1, 3 }, { 1, 2, 3, 0 } },
        { { 1, 0, 2, 3 }, { 0, 1, 3, 2 }, { 2, 3, 1, 0 }, { 2, 3, 1, 0 } },
        { { 2, 0, 3, 1 }, { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 2, 3, 1, 0 } },
        { { 3, 2, 0, 1 }, { 3, 2, 0, 1 }, { 0, 2, 1, 3 }, { 3, 2, 0, 1 } },
        { { 3, 0, 1, 2 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 2, 1, 0, 3 } },
        { { 3, 2, 0, 1 }, { 0, 2, 1, 3 }, { 1, 2, 3, 0 }, { 2, 1, 0, 3 } },
        { { 3, 2, 0, 1 }, { 2, 3, 1, 0 }, { 1, 0, 2, 3 }, { 3, 0, 1, 2 } }
    };

    static const int closedOrHypAdj[9][4] = {
        { 6, 8, 2, 8 },
        { 6, 8, 3, 7 },
        { 7, 0, 3, 4 },
        { 1, 5, 5, 2 },
        { 2, 6, 5, 7 },
        { 3, 8, 3, 4 },
        { 0, 4, 7, 1 },
        { 1, 4, 2, 6 },
        { 1, 0, 5, 0 }
    };

    static const int closedOrHypGluings[9][4][4] = {
        { { 0, 1, 3, 2 }, { 3, 1, 2, 0 }, { 0, 2, 1, 3 }, { 0, 2, 1, 3 } },
        { { 3, 1, 2, 0 }, { 1, 0, 2, 3 }, { 3, 2, 0, 1 }, { 2, 3, 1, 0 } },
        { { 2, 0, 3, 1 }, { 0, 2, 1, 3 }, { 0, 1, 3, 2 }, { 3, 1, 2, 0 } },
        { { 2, 3, 1, 0 }, { 3, 2, 0, 1 }, { 2, 1, 0, 3 }, { 0, 1, 3, 2 } },
        { { 3, 1, 2, 0 }, { 0, 1, 3, 2 }, { 0, 1, 3, 2 }, { 3, 2, 0, 1 } },
        { { 2, 1, 0, 3 }, { 0, 2, 1, 3 }, { 2, 3, 1, 0 }, { 0, 1, 3, 2 } },
        { { 0, 1, 3, 2 }, { 0, 1, 3, 2 }, { 0, 1, 3, 2 }, { 3, 1, 2, 0 } },
        { { 3, 2, 0, 1 }, { 2, 3, 1, 0 }, { 1, 3, 0, 2 }, { 0, 1, 3, 2 } },
        { { 1, 0, 2, 3 }, { 3, 1, 2, 0 }, { 0, 2, 1, 3 }, { 0, 2, 1, 3 } }
    };

    static const int closedNorHypAdj[11][4] = {
        { 8, 2, 8, 2 },
        { 5, 3, 2, 9 },
        { 1, 4, 0, 0 },
        { 6, 1, 4, 6 },
        { 10, 2, 10, 3 },
        { 7, 7, 6, 1 },
        { 8, 3, 3, 5 },
        { 5, 9, 8, 5 },
        { 0, 0, 6, 7 },
        { 10, 10, 1, 7 },
        { 9, 4, 4, 9 }
    };

    static const int closedNorHypGluings[11][4][4] = {
        { { 1, 3, 2, 0 }, { 0, 3, 2, 1 }, { 2, 1, 0, 3 }, { 3, 1, 0, 2 } },
        { { 3, 0, 1, 2 }, { 3, 1, 0, 2 }, { 2, 1, 0, 3 }, { 1, 0, 3, 2 } },
        { { 2, 1, 0, 3 }, { 3, 1, 2, 0 }, { 2, 1, 3, 0 }, { 0, 3, 2, 1 } },
        { { 2, 1, 3, 0 }, { 2, 1, 3, 0 }, { 2, 0, 3, 1 }, { 0, 3, 2, 1 } },
        { { 2, 1, 0, 3 }, { 3, 1, 2, 0 }, { 3, 2, 1, 0 }, { 1, 3, 0, 2 } },
        { { 3, 1, 2, 0 }, { 1, 0, 3, 2 }, { 0, 1, 3, 2 }, { 1, 2, 3, 0 } },
        { { 2, 1, 0, 3 }, { 0, 3, 2, 1 }, { 3, 1, 0, 2 }, { 0, 1, 3, 2 } },
        { { 1, 0, 3, 2 }, { 0, 3, 2, 1 }, { 0, 1, 3, 2 }, { 3, 1, 2, 0 } },
        { { 2, 1, 0, 3 }, { 3, 0, 2, 1 }, { 2, 1, 0, 3 }, { 0, 1, 3, 2 } },
        { { 3, 1, 2, 0 }, { 2, 0, 1, 3 }, { 1, 0, 3, 2 }, { 0, 3, 2, 1 } },
        { { 1, 2, 0, 3 }, { 3, 2, 1, 0 }, { 2, 1, 0, 3 }, { 3, 1, 2, 0 } }
    };

    static const int whiteheadAdj[4][4] = {
        { 3, 2, 1, 3},
        { 3, 2, 2, 0},
        { 1, 3, 0, 1},
        { 2, 0, 0, 1}
    };

    static const int whiteheadGluings[4][4][4] = {
        { { 2, 3, 1, 0 }, { 3, 2, 0, 1 }, { 0, 1, 3, 2 }, { 3, 2, 0, 1 } },
        { { 3, 2, 0, 1 }, { 2, 3, 1, 0 }, { 3, 2, 0, 1 }, { 0, 1, 3, 2 } },
        { { 2, 3, 1, 0 }, { 1, 0, 2, 3 }, { 2, 3, 1, 0 }, { 3, 2, 0, 1 } },
        { { 1, 0, 2, 3 }, { 2, 3, 1, 0 }, { 3, 2, 0, 1 }, { 2, 3, 1, 0 } }
    };
}

namespace regina {

Triangulation<3>* Example<3>::solidKleinBottle() {
    Triangulation<3>* ans = twistedBallBundle();
    ans->setLabel("Solid Klein bottle");
    return ans;
}

Triangulation<3>* Example<3>::threeSphere() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("3-sphere");

    ans->insertLayeredLensSpace(1, 0);

    return ans;
}

Triangulation<3>* Example<3>::bingsHouse() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Bing's house with two rooms");

    Tetrahedron<3>* r = ans->newTetrahedron();
    Tetrahedron<3>* s = ans->newTetrahedron();
    r->join(0, r, Perm<4>(0, 1));
    s->join(0, s, Perm<4>(0, 1));
    r->join(2, s, Perm<4>(3, 1, 2, 0));
    s->join(3, r, Perm<4>(2, 1, 0, 3));

    return ans;
}

Triangulation<3>* Example<3>::rp2xs1() {
    // Section 3.5.1 of Benjamin Burton's PhD thesis describes how to
    // construct RP^2 x S^1 by identifying the boundary triangles of a
    // three-tetrahedron solid Klein bottle.
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("RP2 x S1");

    Tetrahedron<3>* r = ans->newTetrahedron();
    Tetrahedron<3>* s = ans->newTetrahedron();
    Tetrahedron<3>* t = ans->newTetrahedron();
    s->join(0, r, Perm<4>(0, 1, 2, 3));
    s->join(3, r, Perm<4>(3, 0, 1, 2));
    s->join(1, t, Perm<4>(3, 0, 1, 2));
    s->join(2, t, Perm<4>(0, 1, 2, 3));
    r->join(1, t, Perm<4>(2, 3, 0, 1));
    r->join(3, t, Perm<4>(2, 3, 0, 1));

    return ans;
}

Triangulation<3>* Example<3>::rp3rp3() {
    // This can be generated as the enclosing triangulation of a splitting
    // surface, as described in chapter 4 of Benjamin Burton's PhD thesis.
    Signature* sig = Signature::parse("aabccd.b.d");
    Triangulation<3>* ans = sig->triangulate();
    ans->setLabel("RP3 # RP3");
    delete sig;

    return ans;
}

Triangulation<3>* Example<3>::lens(size_t p, size_t q) {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->insertLayeredLensSpace(p, q);

    std::ostringstream s;
    s << "L(" << p << ',' << q << ')';
    ans->setLabel(s.str());

    return ans;
}

Triangulation<3>* Example<3>::poincareHomologySphere() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Poincar\u00E9 homology sphere");

    ans->insertConstruction(5, poincareAdj, poincareGluings);

    return ans;
}

Triangulation<3>* Example<3>::weeks() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Weeks manifold");

    ans->insertConstruction(9, weeksAdj, weeksGluings);

    return ans;
}

Triangulation<3>* Example<3>::weberSeifert() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Weber-Seifert dodecahedral space");

    // Bah.  Dehydration strings are somewhat impenetrable,
    // but the alternative is 23 lines of hard-coded tetrahedron gluings.
    //
    // This triangulation was constructed by building a 60-tetrahedron
    // dodecahedron and identifying opposite pentagonal faces with a 3/10 twist,
    // and then simplifying down to one vertex and 23 tetrahedra.
    ans->insertRehydration(
        "xppphocgaeaaahimmnkontspmuuqrsvuwtvwwxwjjsvvcxxjjqattdwworrko");
    ans->orient();

    return ans;
}

Triangulation<3>* Example<3>::smallClosedOrblHyperbolic() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Closed orientable hyperbolic 3-manifold");

    ans->insertConstruction(9, closedOrHypAdj, closedOrHypGluings);

    return ans;
}

Triangulation<3>* Example<3>::smallClosedNonOrblHyperbolic() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Closed non-orientable hyperbolic 3-manifold");

    ans->insertConstruction(11, closedNorHypAdj, closedNorHypGluings);

    return ans;
}

Triangulation<3>* Example<3>::sphere600() {
    // See 600cell.py for the code that converts the Benedetti-Lutz
    // simplicial complex to a Regina triangulation.
    Triangulation<3>* ans = Triangulation<3>::fromIsoSig("-cyjvvvvvzzzvzzvvwvLLvLvPPvLvPzAPvzAMPzwLvzPvLPzwwMzvPzMzLwzMzMvALMLzvzMPwzwMALMvvMALzMPvAwvzwzMzAwvwwwwMAzMzQLMvzMPAwzzzMLPzvMzMMwwwwMzAwPMzvwzMzAwPMvAzMzAQLLvQzwwzPMvAzQMwwPzwLwPzLMAPPvQAvAMvzQLPwPAwPAMAMvPPzLMQvPzwMAwPAMwALMQwPzwPQwPAMwALMvwzQQAQzALMvMzvAMvwPAMwzvQMQvMzMzQMzzLPQzzMPzPAMMQMLzAwQAPPQPzvLPQzLzPQzLLQMzvMMPQzzPQPvLPzPPQPvzAwAPPQAzPQQLAQQAQPwvLPQPAPwvvzwvvLQMAQLLQQQQLLQQQQQLQQQQQAMMQMQAQialaoamapaqatavawazaPaBaRaCaFaTaHaVa3aIa6aLaXaNaZa7aOa+aebgbSanbWaqbrb0aubybAb2anb5aFbrb9aHbMbObabcbTbfbVbybYbObibkb4bmb6b9bpbGbtbIbecgcwbUbzbWbmcgcCb5bEb7btcycAcXbKbNbEcycZb8bQbSbLc+bPcScPcVc1b-b3bZcac4c7c4c+ccdlcccDcfcfdAcncscicKckclducScpdpcvc1crctdwc7cxdBdzcFcgdCcMcmdVcLdHcNcJcOdOc3c+cTdWdRcWcqdUc0cudKd2cYcRdcd-dbe6c-cyd9cddCdSdedbdYdaedepdLdidndkdteodwdxdTdzeKdvdsdweBd-dIeSdDdAdBeaeKeFdIdQdHdSeJdveVeNdPdTeWd4ebeVdXdWe5ede3d0d7d2dcf4dEeff6d8d0edfkf+dcegfjfiefemehetfNejewfle9eneufBfpemfqexfAfseueUezeJfIeyeAeXeKfKeDeFeFfefTfHeJehfSfMeMfOevf2fQeVfReyf1fVe4eJf5eKfZeifRf1eGfag3e6ebglf8e0fzfNf+eigafdgbfjgCfffkfTfjfSfagbgofxgWfpf4fDfrfhgsfygmgEfwfBf2fAf1figjgxgygNgIfLfOgUfVgPfQgQfWg3fNgOg4gYfUgZf5gZg5fVgWg4g5g7f8fcgPg+fch-fkgXgPgfgkhdhggfhlglhXgkhlhogpgtg0grgthsgzg6g0gvgwhAguhwgnh6gnhCgDgHg8gFgGgEhLgah8gJgMgHhKgFhshahshSgQhehTgghYgRhQhRh2gxh7gvh3gThTh+gbhIh-gGhYhYh9h+h9h+hihyhjhzhmh-h-hphqhJhrhKhbibilimilimininiBhChLhDhMhpipiviwiviwixixiziziOhZhPh0hShdiaiaiVhWh5hXh6hiicicirioioi2h3h7h4h8htiqiqiBiyiyiDiAiAirifigijihikisisiuiBiCiCiEiDiEiGiHiJiIiKiLiLiMiNiNiOiOiQiRiTiSiUiWiViVihjYiXiXiijYi0i1i3i2i4i6i5i5ijj8i7i7ikjnj8ioj+ibj-icjajejdjdjljgjfjfjmjpjgjqjrjsjijkjtjmjujvjojtjqjujwjsjvjwjxjxjxjgovcfbcfboivlbgmvpifdgtoulcepdbifdlcepdbavpaoupdfvlgtalacoigmivnospdvkgwlaojgripdsfgtarncgmwleauckoisidaplsfbgtgrncbgmwouckfoisvplgwqgrxceckoisvfdloshojxhncqbdpvnhvkqhsfxqxccckoisvfflgncqbcpgsfxoxckfoistlojxbdovkqqcncbgmwupgrexdvnhhsfbgtprugdwqeoeshhojwbdovkrqcgresdvnjhgdwneoeskhbcmoigklwxpvnhngijqruoeshsffqgmxigtirmrgmwqiwccxgtqlhppvnhnuoeshsxitgmwqiwqlhpvnjpoeskuagtirmrqahpvkrlgdwntagijqrlaaojwigresmagklwxiaagtcrorqhpggjfrlagkgwciatqhpgvwgntagrosgmauhqluaojgwoiaphxipalagvwgnovggrosgvoggvoojgwovgogovogvcojgwocgfffgcbbojgwocgffgcbojgwovgologvibgrosgvogpgovmdfigvwgnovgugvotecldpbficlpvpoudgtldla");

    ans->orient();
    ans->setLabel("600-cell");
    return ans;
}

Triangulation<3>* Example<3>::lst(size_t a, size_t b) {
    if (a > b)
        std::swap(a, b);

    Triangulation<3>* ans = new Triangulation<3>();
    ans->insertLayeredSolidTorus(a, b);

    std::ostringstream s;
    s << "LST(" << a << ',' << b << ',' << (a + b) << ')';
    ans->setLabel(s.str());

    return ans;
}

Triangulation<3>* Example<3>::figureEight() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Figure eight knot complement");

    // The two-tetrahedron figure eight knot complement is described at
    // the beginning of chapter 8 of Richard Rannard's PhD thesis.
    Tetrahedron<3>* r = ans->newTetrahedron();
    Tetrahedron<3>* s = ans->newTetrahedron();
    r->join(0, s, Perm<4>(1, 3, 0, 2));
    r->join(1, s, Perm<4>(2, 0, 3, 1));
    r->join(2, s, Perm<4>(0, 3, 2, 1));
    r->join(3, s, Perm<4>(2, 1, 0, 3));

    return ans;
}

Triangulation<3>* Example<3>::trefoil() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Trefoil knot complement");

    Tetrahedron<3>* r = ans->newTetrahedron();
    Tetrahedron<3>* s = ans->newTetrahedron();
    r->join(0, s, Perm<4>(2, 3));
    r->join(1, s, Perm<4>(2, 3));
    r->join(2, s, Perm<4>(1, 3));
    r->join(3, s, Perm<4>(0, 2));

    return ans;
}

Triangulation<3>* Example<3>::whiteheadLink() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Whitehead link complement");

    ans->insertConstruction(4, whiteheadAdj, whiteheadGluings);

    return ans;
}

Triangulation<3>* Example<3>::gieseking() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Gieseking manifold");

    Tetrahedron<3>* r = ans->newTetrahedron();
    r->join(0, r, Perm<4>(1, 2, 0, 3));
    r->join(2, r, Perm<4>(0, 2, 3, 1));

    return ans;
}

Triangulation<3>* Example<3>::cuspedGenusTwoTorus() {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->setLabel("Cusped genus two solid torus");

    // We create this by first constructing an ordinary solid genus two
    // torus and then converting the real boundary to an ideal vertex.
    Tetrahedron<3>* r = ans->newTetrahedron();
    Tetrahedron<3>* s = ans->newTetrahedron();
    Tetrahedron<3>* t = ans->newTetrahedron();
    Tetrahedron<3>* u = ans->newTetrahedron();
    r->join(0, s, Perm<4>());
    r->join(1, t, Perm<4>(1, 2, 3, 0));
    r->join(2, u, Perm<4>(1, 0, 3, 2));
    s->join(3, t, Perm<4>());
    t->join(1, u, Perm<4>());
    ans->finiteToIdeal();

    return ans;
}

} // namespace regina

