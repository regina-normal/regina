
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

#include "file/globaldirs.h"
#include "packet/packet.h"
#include "reginamain.h"
#include "reginasupport.h"
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QPainter>

namespace {
    int iconSizes[] = { 16, 22, 32, 48, 64, 128, 0 /* terminator */ };
    int iconSizesRaw[] = { 16, 22, 32, 44, 48, 64, 96, 128, 256, 0 };
        /**< The union of (iconSizes) and (2 * iconSizes). */
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

bool ReginaSupport::warnYesNo(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on MacOSX.
    // The tr() call needs a QObject subclass as context; here we use
    // ReginaMain.
    QMessageBox msg(QMessageBox::Information,
        ReginaMain::tr("Warning"), text,
        QMessageBox::Yes | QMessageBox::Cancel, parent);
    if (! informativeText.isNull())
        msg.setInformativeText(informativeText);
    if (! detailedText.isNull())
        msg.setDetailedText(detailedText);
    msg.setDefaultButton(QMessageBox::Cancel);
    return (msg.exec() == QMessageBox::Yes);
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
        home_ = QFile::decodeName(regina::GlobalDirs::home().c_str());
    return home_;
}

QIcon ReginaSupport::regIcon(const QString& name) {
    QIcon icon;
    QString filename = home() + "/icons/" + name + "-%1.png";
    QString filename2x = home() + "/icons/" + name + "-%1@2x.png";
    for (int i = 0; iconSizes[i]; ++i) {
        icon.addFile(filename.arg(iconSizes[i]));
        icon.addFile(filename2x.arg(iconSizes[i]));
    }
    return icon;
}

QIcon ReginaSupport::themeIcon(const QString& name) {
    QIcon icon = QIcon::fromTheme(name);
    if (! icon.isNull())
        return icon;

    QString filename = home() + "/icons/oxygen/" + name + "-%1.png";
    QString filename2x = home() + "/icons/oxygen/" + name + "-%1@2x.png";
    for (int i = 0; iconSizes[i]; ++i) {
        icon.addFile(filename.arg(iconSizes[i]));
        icon.addFile(filename2x.arg(iconSizes[i]));
    }
    return icon;
}

QIcon ReginaSupport::overlayIcon(const QIcon& base, const QIcon& emblem) {
    QIcon icon;
    for (int i = 0; iconSizesRaw[i]; ++i) {
        QPixmap iconPic = base.pixmap(iconSizesRaw[i]);
        if (iconPic.isNull())
            continue;

        QPainter painter(&iconPic);
        painter.drawPixmap(0, 0, emblem.pixmap(iconSizesRaw[i]));
        icon.addPixmap(iconPic);
    }
    return icon;
}

