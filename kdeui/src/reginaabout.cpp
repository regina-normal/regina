
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

// Regina core includes:
#include "regina-config.h"

// UI includes:
#include "reginaabout.h"

#include <klocale.h>

const char* ReginaAbout::bugAddress = PACKAGE_BUGREPORT;
const char* ReginaAbout::copyright = "Copyright (c) 1999-2002, Ben Burton";
const QDate ReginaAbout::date(2002, 10, 18);
const char* ReginaAbout::description =
    I18N_NOOP("A normal surface theory calculator");
const char* ReginaAbout::internalName = "regina";
const char* ReginaAbout::name = I18N_NOOP("Regina");
const char* ReginaAbout::version = PACKAGE_VERSION;
const char* ReginaAbout::website = "http://regina.sourceforge.net/";

