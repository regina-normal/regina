
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// UI includes:
#include "../pythonmanager.h"
#include "pythonconsole.h"

#include <khtml_part.h>
#include <khtmlview.h>
#include <klineedit.h>
#include <klocale.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qvbox.h>

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager,
        regina::NPacket* tree, regina::NPacket* selectedPacket) :
        KMainWindow(parent, "PythonConsole#"), manager(useManager) {
    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(500, 400);

    // Set up the widgets.
    QVBox* box = new QVBox(this);

    session = new KHTMLPart(box);
    box->setStretchFactor(session->view(), 1);

    QHBox* inputArea = new QHBox(box);
    inputArea->setMargin(5);
    inputArea->setSpacing(5);
    new QLabel(i18n("Command:"), inputArea);

    input = new KLineEdit(inputArea);
    input->setFocus();

    setCentralWidget(box);
    box->show();

    // Finish off.
    if (manager)
        manager->registerConsole(this);
}

PythonConsole::~PythonConsole() {
    if (manager)
        manager->deregisterConsole(this);
}

#include "pythonconsole.moc"
