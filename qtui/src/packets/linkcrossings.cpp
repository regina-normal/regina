
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

// Regina core includes:
#include "link/link.h"

// UI includes:
#include "linkcrossings.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QAction>
#include <QLabel>
#include <QMessageBox>
#include <QToolBar>

using regina::Link;
using regina::Packet;

LinkCrossingsUI::LinkCrossingsUI(regina::Link* packet,
        PacketTabbedUI* useParentUI, bool readWrite) :
        PacketEditorTab(useParentUI), link(packet) {
    ui = new QWidget();
    // TODO: Ensure there is a way to switch crossings.

    actSimplify = new QAction(this);
    actSimplify->setText(tr("&Simplify"));
    actSimplify->setIcon(ReginaSupport::regIcon("simplify-link"));
    actSimplify->setToolTip(tr(
        "Simplify the knot or link as far as possible"));
    actSimplify->setEnabled(readWrite);
    actSimplify->setWhatsThis(tr("Simplify this link to use fewer "
        "crossings.  This link will be modified directly.<p>"
        "This procedure uses only fast heuristics based on Reidemeister "
        "moves, and so there is no guarantee that the smallest possible "
        "number of crossings will be achieved."));
    enableWhenWritable.append(actSimplify);
    actionList.append(actSimplify);
    connect(actSimplify, SIGNAL(triggered()), this, SLOT(simplify()));

    QAction* actMoves = new QAction(this);
    actMoves->setText(tr("Reidemeister &Moves..."));
    actMoves->setToolTip(tr(
        "Modify the link diagram using Reidemeister moves"));
    actMoves->setEnabled(readWrite);
    actMoves->setWhatsThis(tr("Perform Reidemeister moves upon this "
        "link diagram.  <i>Reidemeister moves</i> are modifications "
        "local to a small number of crossings that do not change "
        "the underlying link.<p>"
        "A dialog will be presented for you to select which "
        "Reidemeister moves to apply."));
    enableWhenWritable.append(actMoves);
    actionList.append(actMoves);
    connect(actMoves, SIGNAL(triggered()), this, SLOT(moves()));

    QAction* sep = new QAction(this);
    sep->setSeparator(true);
    actionList.append(sep);

    // Set up the link actions.
    actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    actReflect->setIcon(ReginaSupport::regIcon("reflect"));
    actReflect->setToolTip(tr("Reflect this link"));
    actReflect->setEnabled(readWrite);
    actReflect->setWhatsThis(tr("Reflect this link about some axis in "
        "the plane.  Every crossing will change sign, but its upper "
        "and lower strands will remain the same."));
    actionList.append(actReflect);
    enableWhenWritable.append(actReflect);
    connect(actReflect, SIGNAL(triggered()), this, SLOT(reflect()));

    QAction* actRotate = new QAction(this);
    actRotate->setText(tr("&Rotate"));
    actRotate->setIcon(ReginaSupport::regIcon("rotate"));
    actRotate->setToolTip(tr("Rotate this link"));
    actRotate->setEnabled(readWrite);
    actRotate->setWhatsThis(tr("Rotate this link about some axis in "
        "the plane.  Every crossing will keep the same sign, but its upper "
        "and lower strands will be switched.<p>"
        "This operation simply produces a different diagram of the "
        "same link."));
    actionList.append(actRotate);
    enableWhenWritable.append(actRotate);
    connect(actRotate, SIGNAL(triggered()), this, SLOT(rotate()));

    sep = new QAction(this);
    sep->setSeparator(true);
    actionList.append(sep);

    actComplement = new QAction(this);
    actComplement->setText(tr("&Complement"));
    actComplement->setIcon(ReginaSupport::regIcon("packet_triangulation3"));
    actComplement->setToolTip(tr("Triangulate the complement of this link."));
    actComplement->setWhatsThis(tr("Construct the complement of this "
        "knot or link as an ideal 3-manifold triangulation."));
    actionList.append(actComplement);
    connect(actComplement, SIGNAL(triggered()), this, SLOT(complement()));

    // Tidy up.

    refresh();
}

void LinkCrossingsUI::fillToolBar(QToolBar* bar) {
    bar->addAction(actReflect);
    bar->addAction(actSimplify);
    bar->addSeparator();
    bar->addAction(actComplement);
}

regina::Packet* LinkCrossingsUI::getPacket() {
    return link;
}

QWidget* LinkCrossingsUI::getInterface() {
    return ui;
}

const QLinkedList<QAction*>& LinkCrossingsUI::getPacketTypeActions() {
    return actionList;
}

void LinkCrossingsUI::setReadWrite(bool readWrite) {
    QLinkedListIterator<QAction*> it(enableWhenWritable);
    while (it.hasNext())
        (it.next())->setEnabled(readWrite);
}

void LinkCrossingsUI::refresh() {
    // TODO.
}

void LinkCrossingsUI::simplify() {
    if (! link->intelligentSimplify()) {
        QMessageBox msgBox(ui);
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
        if (link->countComponents() > 1) {
            msgBox.setText(tr("I could not simplify the link."));
            msgBox.setInformativeText(tr("I have only tried fast heuristics."));
        } else if (link->countComponents() == 1) {
            msgBox.setText(tr("I could not simplify the knot."));
            msgBox.setInformativeText(tr("I have only tried fast heuristics."));
        } else {
            msgBox.setText(tr("This link is empty."));
        }
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
    }
}

void LinkCrossingsUI::reflect() {
    link->reflect();
}

void LinkCrossingsUI::rotate() {
    link->rotate();
}

void LinkCrossingsUI::moves() {
    // TODO.
}

void LinkCrossingsUI::complement() {
    // TODO.
}

