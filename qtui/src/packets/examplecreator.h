
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

/*! \file examplecreator.h
 *  \brief Helps with the creation of ready-made example triangulations and
 *  links.
 */

#ifndef __EXAMPLECREATOR_H
#define __EXAMPLECREATOR_H

#include "reginaqt.h"
#include <QString>

/**
 * A ready-made example triangulation or link that Regina can create.
 *
 * The class \a Object would typically be `Triangulation<dim>` or `Link`.
 */
template <class Object>
class ExampleCreator {
    public:
        using CreatorFunc = Object (*)();

    private:
        QString name_;
        CreatorFunc creator_;

    public:
        ExampleCreator(QString name, CreatorFunc creator) :
                name_(std::move(name)), creator_(creator) {
        }

        const QString& name() const {
            return name_;
        }

        std::shared_ptr<regina::PacketOf<Object>> create() const {
            return regina::make_packet((*creator_)(),
                name_.toUtf8().constData());
        }
};

#endif
