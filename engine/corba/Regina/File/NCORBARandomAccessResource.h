
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#ifndef __NCORBARANDOMACCESSRESOURCE_H
#define __NCORBARANDOMACCESSRESOURCE_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nresources.h"
#else
    #include "engine/file/nresources.h"
#endif

#include "NRandomAccessResourceIDL.h"

/**
 * A resource that is accessed via CORBA.
 */
class NCORBARandomAccessResource : public ::NRandomAccessResource {
    private:
        Regina::File::NRandomAccessResource_var data;
            /**< The CORBA object providing the resource. */

    public:
        /**
         * Create a new resource to be accessed via CORBA.
         *
         * @param newData the CORBA object providing the resource.
         */
        NCORBARandomAccessResource(
                Regina::File::NRandomAccessResource_var newData) :
                data(newData) {
        }

        virtual bool openRead() {
            return data->openRead();
        }
        virtual bool openWrite() {
            return data->openWrite();
        }
        virtual void close() {
            data->close();
        }
        virtual mode getOpenMode() const {
            return (mode)data->getOpenMode();
        }
        virtual char getChar() {
            return data->getChar();
        }
        virtual void putChar(char c) {
            data->putChar(c);
        }
        virtual long getPosition() {
            return data->getPosition();
        }
        virtual void setPosition(long pos) {
            data->setPosition(pos);
        }
};

#endif

