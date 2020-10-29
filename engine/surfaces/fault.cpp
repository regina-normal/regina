
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  This file: Copyright (c) 2020, Robert C. Haraway, III                 *
 *  For further details contact Robert (bobbycyiii@fastmail.com).         *
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


#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"

namespace regina {
    bool NormalSurface::separates() const{
        int tri_cpts = triangulation()->countComponents();
        Triangulation<3>* cut_up = cutAlong();
        cut_up->intelligentSimplify();
        int new_cpts = cut_up->countComponents();
        delete cut_up;
        return tri_cpts < new_cpts;
    }

    bool NormalSurface::isEssentialSphere() const{
        if (!(isConnected()
              && isCompact()
              && eulerChar() == 2))
            return false;

        if (!separates())
            return true;

        Triangulation<3>* cut_up = cutAlong();
        cut_up->intelligentSimplify();

        // Cap sphere boundary components.
        // Since the original triangulation has no sphere boundary components,
        // this caps the sides of this sphere.
        // That is, this undoes a (possibly trivial) connect-sum.
        cut_up->finiteToIdeal();
        cut_up->intelligentSimplify();
        cut_up->idealToFinite();
        cut_up->intelligentSimplify();

        // There are two components,
        // since the original triangulation was connected,
        // and this surface is separating.
        cut_up->splitIntoComponents();
        Triangulation<3>* L = (Triangulation<3>*) cut_up->firstChild();
        Triangulation<3>* R = (Triangulation<3>*) L->nextSibling();
        bool bounds_ball = L->isThreeSphere() || R->isThreeSphere();
        delete L;
        delete R;
        delete cut_up;
        return !bounds_ball;
    }

    bool NormalSurface::isEssentialKleinBottle() const{
        if (!(isConnected()
              && isCompact()
              && !isOrientable()
              && !hasRealBoundary()
              && eulerChar() == 0)){
            return false;
        }

        if (!separates()){
            Triangulation<3>* cut_up = cutAlong();
            bool compressible = cut_up->hasCompressingDisc();
            delete cut_up;
            return !compressible;
        }
        
        /*
        if (!isIncompressible())
            return false;
        */
        // is what we would like to write here,
        // but presently `isIncompressible` requires closed triangulations.
        
        // There are two components,
        // since the original triangulation was connected,
        // and this surface is separating.
        Triangulation<3>* cut_up = cutAlong();
        cut_up->splitIntoComponents();
        Triangulation<3>* L = (Triangulation<3>*) cut_up->firstChild();
        Triangulation<3>* R = (Triangulation<3>*) L->nextSibling();

        bool compressible = true;
        bool boundary_parallel = false;
        compressible = L->hasCompressingDisc() || R->hasCompressingDisc();
        if (!compressible){
            L->makeDoubleCover();
            R->makeDoubleCover();
            boundary_parallel = L->isTorusXInterval() || R->isTorusXInterval();
        }
        delete L;
        delete R;
        delete cut_up;
        return !(compressible || boundary_parallel);
    }

    bool NormalSurface::isEssentialTorus() const{
        if (!(isConnected()
              && isCompact()
              && isOrientable()
              && !hasRealBoundary()
              && eulerChar() == 0))
            return false;

        if (!separates())
            return true;
        /*
          if (!isIncompressible())
          return false;
        */
        // is what we would like to write here,
        // but presently `isIncompressible` requires closed triangulations.
        
        // There are two components,
        // since the original triangulation was connected,
        // and this surface is separating.
        Triangulation<3>* cut_up = cutAlong();
        cut_up->splitIntoComponents();
        Triangulation<3>* L = (Triangulation<3>*) cut_up->firstChild();
        Triangulation<3>* R = (Triangulation<3>*) L->nextSibling();

        bool compressible = true;
        bool boundary_parallel = false;
        compressible = L->hasCompressingDisc() || R->hasCompressingDisc();
        if (!compressible){
            boundary_parallel = L->isTorusXInterval() || R->isTorusXInterval();    
        }
        delete L;
        delete R;
        delete cut_up;
        return !(compressible || boundary_parallel);
    }

    bool NormalSurface::isSolidTorusAnnulus() const{
        if (!(isConnected()
              && isCompact()
              && isOrientable()
              && hasRealBoundary()
              && eulerChar() == 0))
            return false;

        Triangulation<3>* cut_up = cutAlong();
        cut_up->intelligentSimplify();
            
        bool solid_torus;
        if (cut_up->isConnected())
            solid_torus = cut_up->isSolidTorus();
        else {
            cut_up->splitIntoComponents();
            Triangulation<3>* L = (Triangulation<3>*)(cut_up->firstChild());
            Triangulation<3>* R = (Triangulation<3>*)(L->nextSibling());
            solid_torus = L->isSolidTorus() && R->isSolidTorus();
            delete L;
            delete R;
        }
        delete cut_up;
        return solid_torus;
    }

}
