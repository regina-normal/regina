
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

/*! \file reginaabout.h
 *  \brief Provides general application information.
 */

#ifndef __REGINAABOUT_H
#define __REGINAABOUT_H

// UI includes:
#include <qstring.h>
#include <QDialog>

/**
 * Provides constants offering general information about the application.
 */
class ReginaAbout : public QDialog {
    Q_OBJECT

    public:
        static const QString regCopyright;
            /**< The application's copyright notice. */
        static const QString regDescription;
            /**< A brief description of the application. */
        static const QString regReleased;
            /**< A message describing the current application release date. */
        static const QString regWebsite;
            /**< The home website of the application. */
        static const QString regLicense;
            /**< The licensing agreement for the application. */

        ReginaAbout(QWidget *parent);

    private:
        /**
         * Full details for an author of the application.
         */
        struct AuthorInfo {
            QString name, email, website;

            AuthorInfo(const QString& name_, const QString& email_,
                    const QString& website_) :
                    name(name_), email(email_), website(website_) {
            }
        };

        /**
         * Full details for a bundled library.
         */
        struct SoftwareInfo {
            QString name, version, website;

            SoftwareInfo(const QString& name_, const QString& version_,
                    const QString& website_) :
                    name(name_), version(version_), website(website_) {
            }
        };

        QList<AuthorInfo> authors;
            /**< Stores a list of authors of the application. */
        QList<QString> thanksCode;
            /**< Stores a list of additional people to thank for code. */
        QList<QString> thanksExpertise;
            /**< Stores a list of additional people to thank for expertise. */
        QList<QString> thanksInstitutions;
            /**< Stores a list of institions to thank. */
        QList<SoftwareInfo> bundled;
            /**< Stores a list of bundled software with version details. */

    private slots:
        void showLicense();
            /**< Shows the license text in a new dialog. */
};

#endif

