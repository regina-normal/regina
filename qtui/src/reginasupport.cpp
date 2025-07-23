
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "file/globaldirs.h"
#include "packet/packet.h"
#include "reginamain.h"
#include "reginasupport.h"
#include <array>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QPainter>

namespace {
    // These are the possible icon sizes as measured in points (not pixels).
    // We list icon sizes in descending order so that icon-{k}@2x.png
    // takes priority over icon-{2k}.png.
    constexpr std::array iconSizes { 128, 64, 48, 32, 22, 16 };
}

QString ReginaSupport::home_;

void ReginaSupport::info(QWidget* parent, const QString& text,
        const QString& informativeText, const QString& detailedText) {
    // Make sure that the window title is ignorable, since it does not
    // appear on macOS.
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
    // appear on macOS.
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
    // appear on macOS.
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
    // appear on macOS.
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
    // appear on macOS.
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
    // appear on macOS.
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
    for (int size : iconSizes)
        icon.addFile(filename.arg(size));
    return icon;
}

QIcon ReginaSupport::themeIcon(const QString& name) {
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    // Newer versions of Qt provide QIcon::fromTheme(...) for macOS and
    // Windows.  The visual style clashes horribly with the other icons used
    // throughout Regina.  Don't use them.
    QIcon icon;
#else
    QIcon icon = QIcon::fromTheme(name);
    if (! icon.isNull())
        return icon;
#endif

    QString filename = home() + "/icons/system/" + name + "-%1.png";
    for (int size : iconSizes)
        icon.addFile(filename.arg(size));
    return icon;
}

