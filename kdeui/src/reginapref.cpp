
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include <kiconloader.h>
#include <klocale.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>

ReginaPreferences::ReginaPreferences(ReginaMain* parent) :
        KDialogBase(IconList, i18n("Regina Preferences"),
        Help|Ok|Apply|Cancel, Ok), mainWindow(parent) {
    // Construct the individual preferences pages.
    QVBox* frame = addVBoxPage(i18n("Display"), i18n("Display Options"),
        BarIcon("viewmag", KIcon::SizeMedium));
    displayPrefs = new ReginaPrefDisplay(frame);

    frame = addVBoxPage(i18n("File"), i18n("File Options"),
        BarIcon("fileopen", KIcon::SizeMedium));
    filePrefs = new ReginaPrefFile(frame);

    frame = addVBoxPage(i18n("Python"), i18n("Python Options"),
        BarIcon("source_py", KIcon::SizeMedium));
    pythonPrefs = new ReginaPrefPython(frame);

    // Read the current preferences from the main window.
    displayPrefs->cbAutoDock->setChecked(mainWindow->getAutoDock());
    displayPrefs->cbDisplayIcon->setChecked(mainWindow->getDisplayIcon());
    filePrefs->cbAutoFileExtension->setChecked(
        mainWindow->getAutoFileExtension());
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
    mainWindow->setAutoDock(displayPrefs->cbAutoDock->isChecked());
    mainWindow->setDisplayIcon(displayPrefs->cbDisplayIcon->isChecked());
    mainWindow->setAutoFileExtension(
        filePrefs->cbAutoFileExtension->isChecked());

    // Save these preferences to the global configuration.
    mainWindow->saveOptions();
}

ReginaPrefDisplay::ReginaPrefDisplay(QWidget* parent) : QVBox(parent) {
    cbAutoDock = new QCheckBox(i18n("Automatic packet docking"), this);
    QWhatsThis::add(cbAutoDock, i18n("Try to dock new packet viewers into "
        "the main window instead of opening them in new windows."));

    cbDisplayIcon = new QCheckBox(i18n("Display icon"), this);
    QWhatsThis::add(cbDisplayIcon, i18n("Display the large Regina icon "
        "beneath the packet tree."));

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefFile::ReginaPrefFile(QWidget* parent) : QVBox(parent) {
    cbAutoFileExtension = new QCheckBox(i18n("Automatic File Extension"), this);
    QWhatsThis::add(cbAutoFileExtension, i18n("Append the default extension "
        "to filenames when saving if no extension is already given."));

    // Add some space at the end.
    setStretchFactor(new QWidget(this), 1);
}

ReginaPrefPython::ReginaPrefPython(QWidget* parent) : QVBox(parent) {
    new QLabel(i18n("Python scripting is not yet implemented."), this);
}

#include "reginapref.moc"
