
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

// Regina core includes:
#include "triangulation/ntriangulation.h"

// UI includes:
#include "snappeacomponents.h"

#include <klocale.h>

NoSnapPea::NoSnapPea(regina::NTriangulation* useTri, bool allowClosed,
        QWidget* parent, const char* name, bool delayedRefresh) :
        QLabel(parent, name), tri(useTri) {
    if (! delayedRefresh)
        refresh(allowClosed);
}

void NoSnapPea::refresh(bool allowClosed) {
    QString msg = i18n("<qt><p>SnapPea calculations are not available "
        "for this triangulation.</p><p>");

    if (tri->getNumberOfTetrahedra() == 0)
        msg += i18n("This is because the triangulation is empty.");
    else if (! tri->isValid())
        msg += i18n("This is because the triangulation is not valid.");
    else if (tri->hasBoundaryFaces())
        msg += i18n("This is because the triangulation has boundary faces.");
    else if (! tri->isConnected())
        msg += i18n("This is because the triangulation is disconnected.");
    else if (! tri->isStandard())
        msg += i18n("This is because the triangulation contains non-standard "
            "vertices (vertices whose links are not spheres, tori or Klein "
            "bottles).");
    else if ((! tri->isIdeal()) && (! allowClosed))
        msg += i18n("This is because the triangulation does not contain any "
            "ideal vertices.");
    else if (tri->isIdeal() &&
            tri->getNumberOfBoundaryComponents() < tri->getNumberOfVertices())
        msg += i18n("This is because the triangulation contains a mix of "
            "finite and ideal vertices.");
    else if ((! tri->isIdeal()) && 1 != tri->getNumberOfVertices())
        msg += i18n("This is because the triangulation is closed but has "
            "more than one vertex.");
    else if (tri->getNumberOfTetrahedra() >= INT_MAX)
        msg += i18n("This is because the triangulation has too many "
            "tetrahedra.");
    else
        msg += i18n("This is because SnapPea was unable to convert the "
            "triangulation into SnapPea's native format.");

    msg += "</p></qt>";
    setText(msg);
}

#include "snappeacomponents.moc"
