
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
#include "packet/nscript.h"

// UI includes:
#include "nscriptui.h"

#include <cstring>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>

using regina::NPacket;
using regina::NScript;

NScriptUI::NScriptUI(NScript* packet, PacketPane* enclosingPane,
        KTextEditor::Document* doc, bool readWrite) :
        PacketUI(enclosingPane), script(packet), document(doc),
        isCommitting(false) {
    // Create a view before we do anything else.
    // Otherwise the Vim component crashes.
    view = document->createView(0);
    editInterface = KTextEditor::editInterface(document);

    // Prepare the components.
    document->setReadWrite(readWrite);
    setPythonMode();

    // Fill the components with data.
    refresh();

    // Clear the undo stack.
    // TODO: Can we work around this in a nicer way (Vim component problem)?
    if (strcmp(document->className(), "Vim::Document") == 0)
        std::cerr << "Not flushing the undo list since this has strange "
            "side-effects with the Vim component." << std::endl;
    else
        KTextEditor::undoInterface(document)->clearUndo();

    // Final tidying up.
    connect(document, SIGNAL(textChanged()),
        this, SLOT(notifyTextChanged()));
}

NScriptUI::~NScriptUI() {
    delete document;
}

NPacket* NScriptUI::getPacket() {
    return script;
}

QWidget* NScriptUI::getInterface() {
    return view;
}

KTextEditor::Document* NScriptUI::getTextComponent() {
    return document;
}

void NScriptUI::commit() {
    isCommitting = true;

    // Update the lines.
    script->removeAllLines();
    unsigned nLines = editInterface->numLines();
    for (unsigned i = 0; i < nLines; i++)
        script->addLast(editInterface->textLine(i).ascii());

    // Update the variables.
    // TODO

    isCommitting = false;
    setDirty(false);
}

void NScriptUI::refresh() {
    // Refresh the lines.
    editInterface->clear();
    unsigned long nLines = script->getNumberOfLines();
    for (unsigned long i = 0; i < nLines; i++)
        editInterface->insertLine(editInterface->numLines(),
            script->getLine(i).c_str());

    // Refresh the variables.
    // TODO

    setDirty(false);
}

void NScriptUI::setReadWrite(bool readWrite) {
    document->setReadWrite(readWrite);
}

void NScriptUI::notifyTextChanged() {
    if (! isCommitting)
        setDirty(true);
}

void NScriptUI::setPythonMode() {
    KTextEditor::HighlightingInterface* hi =
        KTextEditor::highlightingInterface(document);

    unsigned nModes = hi->hlModeCount();
    for (unsigned i = 0; i < nModes; i++)
        if (hi->hlModeName(i).lower() == "python") {
            hi->setHlMode(i);
            break;
        }
}

#include "nscriptui.moc"
