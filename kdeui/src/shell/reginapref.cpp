
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

#include "reginamain.h"
#include "reginapref.h"

#include <kcombobox.h>
#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvalidator.h>
#include <qwhatsthis.h>

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        KDialogBase(IconList, i18n("Regina Preferences"),
        Help|Ok|Apply|Cancel, Ok), mainWindow(parent),
        prefSet(mainWindow->getPreferences()) {
    // Construct the individual preferences pages.
    QVBox* frame = addVBoxPage(i18n("General"), i18n("General Options"),
        BarIcon("regina", KIcon::SizeMedium));
    generalPrefs = new ReginaPrefGeneral(frame);

    frame = addVBoxPage(i18n("Triangulation"), i18n("Triangulation Options"),
        BarIcon("packet_triangulation", KIcon::SizeMedium));
    triPrefs = new ReginaPrefTri(frame);

    frame = addVBoxPage(i18n("Python"), i18n("Python Options"),
        BarIcon("openterm", KIcon::SizeMedium));
    pythonPrefs = new ReginaPrefPython(frame);

    // Read the current preferences from the main window.
    generalPrefs->cbAutoDock->setChecked(prefSet.autoDock);
    generalPrefs->cbAutoFileExtension->setChecked(prefSet.autoFileExtension);
    generalPrefs->cbDisplayIcon->setChecked(prefSet.displayIcon);
    generalPrefs->cbDisplayTagsInTree->setChecked(prefSet.displayTagsInTree);
    generalPrefs->editTreeJumpSize->setText(
        QString::number(prefSet.treeJumpSize));

    triPrefs->comboEditMode->setCurrentItem(
        prefSet.triEditMode == ReginaPrefSet::DirectEdit ? 0 : 1);
    triPrefs->editSurfacePropsThreshold->setText(
        QString::number(prefSet.triSurfacePropsThreshold));
}

int ReginaPreferences::exec() {
    // Apply changes if OK was pressed.
    int ret = KDialogBase::exec();
    if (ret)
        slotApply();
    return ret;
}

void ReginaPreferences::slotApply() {
    // Propagate changes to the main window.
    bool ok;
    unsigned uintVal;

    prefSet.autoDock = generalPrefs->cbAutoDock->isChecked();
    prefSet.autoFileExtension = generalPrefs->cbAutoFileExtension->isChecked();
    prefSet.displayIcon = generalPrefs->cbDisplayIcon->isChecked();
    prefSet.displayTagsInTree = generalPrefs->cbDisplayTagsInTree->isChecked();

    uintVal = generalPrefs->editTreeJumpSize->text().toUInt(&ok);
    if (ok && uintVal > 0)
        prefSet.treeJumpSize = uintVal;
    else {
        KMessageBox::error(this, i18n("The packet tree jump size "
            "must be a positive integer.  This is the number of steps "
            "that a packet moves when Jump Up or Jump Down is selected."));
        generalPrefs->editTreeJumpSize->setText(
            QString::number(prefSet.treeJumpSize));
    }

    prefSet.triEditMode = (triPrefs->comboEditMode->currentItem() == 0 ?
        ReginaPrefSet::DirectEdit : ReginaPrefSet::Dialog);

    uintVal = triPrefs->editSurfacePropsThreshold->text().toUInt(&ok);
    if (ok)
        prefSet.triSurfacePropsThreshold = uintVal;
    else {
        KMessageBox::error(this, i18n("The surface calculation "
            "threshold must be a non-negative integer.  "
            "This is the maximum number of tetrahedra for which normal "
            "surface properties will be calculated automatically."));
        triPrefs->editSurfacePropsThreshold->setText(
            QString::number(prefSet.triSurfacePropsThreshold));
    }

    // Save these preferences to the global configuration.
    mainWindow->setPreferences(prefSet);
    mainWindow->saveOptions();
}

ReginaPrefGeneral::ReginaPrefGeneral(QWidget* parent) : QVBox(parent) {
    cbAutoFileExtension = new QCheckBox(i18n("Automatic file extension"), this);
    QWhatsThis::add(cbAutoFileExtension, i18n("Append the default extension "
        "to filenames when saving if no extension is already given."));

    cbAutoDock = new QCheckBox(i18n("Automatic packet docking"), this);
    QWhatsThis::add(cbAutoDock, i18n("Try to dock new packet viewers into "
        "the main window instead of opening them in new windows."));

    cbDisplayIcon = new QCheckBox(i18n("Display icon"), this);
    QWhatsThis::add(cbDisplayIcon, i18n("Display the large Regina icon "
        "beneath the packet tree."));

    cbDisplayTagsInTree = new QCheckBox(i18n("Display tags in packet tree"),
        this);
    cbDisplayTagsInTree->setEnabled(false);
    QWhatsThis::add(cbDisplayTagsInTree, i18n("Show full details of any "
        "packet tags directly within the packet tree."));

    // Set up the tree jump size.
    QHBox* box = new QHBox(this);
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Packet tree jump size:"), box);
    editTreeJumpSize = new KLineEdit(box);
    editTreeJumpSize->setMaxLength(
         10 /* ridiculously high number of digits */);
    QIntValidator* val = new QIntValidator(box);
    val->setBottom(1);
    editTreeJumpSize->setValidator(val);
    QString msg = i18n("The number of steps that a packet moves when Jump Up "
        "or Jump Down is selected.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(editTreeJumpSize, msg);

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefTri::ReginaPrefTri(QWidget* parent) : QVBox(parent) {
    setSpacing(5);

    // Set up the edit mode.
    // Note that any change of order in the combo box must be reflected
    // in the ReginaPreferences methods as well.
    QHBox* box = new QHBox(this);
    box->setSpacing(5);

    QLabel* label = new QLabel(i18n("Edit mode:"), box);
    comboEditMode = new KComboBox(box);
    comboEditMode->insertItem(SmallIcon("editclear"), i18n("Direct edit"));
    comboEditMode->insertItem(SmallIcon("view_text"), i18n("Pop-up dialog"));
    QString msg = i18n("Specifies the way in which face gluings are edited.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(comboEditMode, msg);

    // Set up the surface properties threshold.
    box = new QHBox(this);
    box->setSpacing(5);

    label = new QLabel(i18n("Surface calculation threshold:"), box);
    editSurfacePropsThreshold = new KLineEdit(box);
    editSurfacePropsThreshold->setMaxLength(
         3 /* ridiculously high number of digits */);
    editSurfacePropsThreshold->setValidator(new QIntValidator(0,
         999 /* ridiculously high */, box));
    msg = i18n("The maximum number of tetrahedra for which normal "
        "surface properties will be calculated automatically.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(editSurfacePropsThreshold, msg);

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefPython::ReginaPrefPython(QWidget* parent) : QVBox(parent) {
    new QLabel(i18n("Python scripting is not yet implemented."), this);

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

#include "reginapref.moc"
