
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include "file/nglobaldirs.h"
#include "reginamain.h"
#include "reginasupport.h"
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QPainter>

namespace {
    int iconSizes[] = { 8, 16, 22, 32, 48, 64, 128, 0 /* terminator */ };
}

QString ReginaSupport::home_;

void ReginaSupport::info(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.
    QMessageBox msg(QMessageBox::Information,
        ReginaMain::tr("Information"), text, QMessageBox::Ok, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.exec();
}

void ReginaSupport::sorry(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.

    // Use the generic Information icon, since sorry() is not severe
    // enough to warrant a more exciting icon.
    QMessageBox msg(QMessageBox::Information,
        ReginaMain::tr("Sorry"), text, QMessageBox::Ok, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.exec();
}

void ReginaSupport::warn(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.
    QMessageBox msg(QMessageBox::Warning,
        ReginaMain::tr("Warning"), text, QMessageBox::Ok, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.exec();
}

void ReginaSupport::success(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.
    QMessageBox msg(QMessageBox::Information,
        ReginaMain::tr("Success"), text, QMessageBox::Ok, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.exec();
}

void ReginaSupport::failure(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.
    QMessageBox msg(QMessageBox::Warning,
        ReginaMain::tr("Failure"), text, QMessageBox::Ok, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.exec();
}

const QString& ReginaSupport::home() {
    if (home_.isNull())
        home_ = QFile::decodeName(regina::NGlobalDirs::home().c_str());
    return home_;
}

QIcon ReginaSupport::regIcon(const QString& name) {
    QIcon icon;
    QString filename = home() + "/icons/regina/" + name + "-%1.png";
    for (int i = 0; iconSizes[i]; ++i)
        icon.addFile(filename.arg(iconSizes[i]));
    return icon;
}

QIcon ReginaSupport::themeIcon(const QString& name) {
    QIcon icon = QIcon::fromTheme(name);
    if (! icon.isNull())
        return icon;

    QString filename = home() + "/icons/oxygen/" + name + "-%1.png";
    for (int i = 0; iconSizes[i]; ++i)
        icon.addFile(filename.arg(iconSizes[i]));
    return icon;
}

QIcon ReginaSupport::overlayIcon(const QIcon& base, const QIcon& emblem) {
    QIcon icon;
    int overlaySize;
    for (int i = 0; iconSizes[i]; ++i) {
        QPixmap iconPic = base.pixmap(iconSizes[i]);
        if (iconPic.isNull())
            continue;

        overlaySize = (iconSizes[i] <= 22 ? 8 : iconSizes[i] <= 48 ? 16 : 22);
        QPixmap overlayPic = emblem.pixmap(overlaySize);

        QPainter painter(&iconPic);
        painter.drawPixmap(0, iconSizes[i] - overlayPic.height(), overlayPic);

        icon.addPixmap(iconPic);
    }
    return icon;
}

