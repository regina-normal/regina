
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

// Regina core includes:
#include "angle/nanglestructure.h"
#include "angle/nanglestructurelist.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "nanglestructureui.h"

#include <klocale.h>
#include <qlabel.h>
#include <qtable.h>
#include <qvbox.h>

using regina::NAngleStructureList;
using regina::NPacket;

NAngleStructureUI::NAngleStructureUI(NAngleStructureList* packet,
        PacketPane* enclosingPane) : PacketReadOnlyUI(enclosingPane),
        structures(packet) {
    ui = new QVBox();

    stats = new QLabel(ui);
    stats->setAlignment(Qt::AlignCenter);

    table = new QTable(ui);
    table->setReadOnly(true);
    ui->setStretchFactor(table, 1);

    QHeader* hdr = table->verticalHeader();
    hdr->hide();
    table->setLeftMargin(0);

    refresh();

    // Final tidying up for the table now that it is full of data.
    for (int i = 0; i < table->numCols(); i++)
        table->adjustColumn(i);
    headerTips = new AngleHeaderToolTip(table->horizontalHeader());
}

NAngleStructureUI::~NAngleStructureUI() {
    delete headerTips;
}

NPacket* NAngleStructureUI::getPacket() {
    return structures;
}

QWidget* NAngleStructureUI::getInterface() {
    return ui;
}

void NAngleStructureUI::refresh() {
    QString statStr;

    // Update the general statistics.
    unsigned long nStructs = structures->getNumberOfStructures();
    if (nStructs == 0)
        statStr = i18n("No vertex angle structures\n");
    else if (nStructs == 1)
        statStr = i18n("1 vertex angle structure\n");
    else
        statStr = i18n("%1 vertex angle structures\n").arg(nStructs);

    statStr.append(i18n("Span includes: "));
    if (structures->allowsStrict())
        statStr.append(i18n("Strict, "));
    else
        statStr.append(i18n("NO Strict, "));
    if (structures->allowsTaut())
        statStr.append(i18n("Taut"));
    else
        statStr.append(i18n("NO Taut"));

    stats->setText(statStr);

    // Empty the table.
    table->setNumRows(0);

    // Update the table.
    unsigned long nTets =
        structures->getTriangulation()->getNumberOfTetrahedra();

    table->setNumRows(nStructs);
    table->setNumCols(3 * nTets + 1);

    QHeader* hdr = table->horizontalHeader();
    hdr->setLabel(0, i18n("Type"));
    unsigned long i, j, k;
    for (i = 0; i < nTets; i++)
        for (j = 0; j < 3; j++)
            hdr->setLabel(3 * i + j + 1, QString::number(i) + ": " +
                regina::vertexSplitString[j]);

    const regina::NAngleStructure* s;
    regina::NRational angle;
    QString pi = i18n("Pi");
    for (i = 0; i < nStructs; i++) {
        s = structures->getStructure(i);

        if (s->isStrict())
            table->setText(i, 0, i18n("Strict"));
        else if (s->isTaut())
            table->setText(i, 0, i18n("Taut"));

        for (j = 0; j < nTets; j++)
            for (k = 0; k < 3; k++) {
                angle = s->getAngle(j, k);
                if (angle != 0)
                    table->setText(i, 3 * j + k + 1, angleToString(angle));
            }
    }

    setDirty(false);
}

QString NAngleStructureUI::angleToString(regina::NRational angle) {
    if (angle == 0)
        return "0";

    static const QString pi(i18n("Pi"));

    if (angle == 1)
        return pi;
    else if (angle.getDenominator() == 1)
        return QString(angle.getNumerator().stringValue().c_str()) + ' ' + pi;
    else if (angle.getNumerator() == 1)
        return pi + " / " + angle.getDenominator().stringValue().c_str();
    else
        return QString(angle.getNumerator().stringValue().c_str()) + ' ' + pi
            + " / " + angle.getDenominator().stringValue().c_str();
}

AngleHeaderToolTip::AngleHeaderToolTip(QHeader *header,
        QToolTipGroup *group) : QToolTip(header, group) {
}

void AngleHeaderToolTip::maybeTip(const QPoint& p) {
    QHeader *header = dynamic_cast<QHeader*>(parentWidget());
    int section = header->sectionAt(p.x());

    QString tipString;
    if (section == 0)
        tipString = i18n("Taut or strict?");
    else
        tipString = i18n("Tetrahedron %1, edges %2").arg((section - 1) / 3).
            arg(regina::vertexSplitString[(section - 1) % 3]);

    tip(header->sectionRect(section), tipString);
}

