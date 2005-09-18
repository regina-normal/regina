
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*********************
 *
 *  Packet Registry
 *  ---------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A NEW PACKET TYPE IS CREATED!
 *
 *    For each packet type there should be a line of the form:
 *
 *        REGISTER_PACKET(class, type, name)
 *
 *    where:
 *        class = the C++ class representing this packet type;
 *        type = the integer ID of the new packet type;
 *        name = the string (English) name of the new packet type.
 *
 *    The appropriate include files should also be placed with full path
 *        and without full path in the appropriate include sections below.
 *
 *    To use the packet registry, simply #include this file.  If you have
 *        not defined __PACKET_REGISTRY_BODY, only the include sections will
 *        be brought in.  If you have defined __PACKET_REGISTRY_BODY, the
 *        include sections will be skipped and the REGISTER_PACKET lines
 *        will be brought in instead.  By suitably defining the macro
 *        REGISTER_PACKET before including this file, you can have these
 *        lines do whatever you wish.
 *
 *    If you do #include this file, be sure to #undef both
 *        REGISTER_PACKET and __PACKET_REGISTRY_BODY when you are
 *        finished, so that if --enable-final is being used then these
 *        macros are left undefined for the following files.
 *
 *    Packet Types:
 *    ------------
 *    When selecting an integer ID for your new packet type, the following
 *        guidelines should be adhered to:
 *
 *    1-999:      Reserved for use with the official program distribution.
 *    1000-9999:  Reserved for future use.
 *    10000-:     Unreserved.
 */

#ifndef __PACKET_REGISTRY_BODY
    #include "packet/ncontainer.h"
    #include "packet/ntext.h"
    #include "triangulation/ntriangulation.h"
    #include "surfaces/nnormalsurfacelist.h"
    #include "packet/nscript.h"
    #include "surfaces/nsurfacefilter.h"
    #include "angle/nanglestructurelist.h"
#else
    REGISTER_PACKET(NContainer, 1, "Container")
    REGISTER_PACKET(NText, 2, "Text")
    REGISTER_PACKET(NTriangulation, 3, "Triangulation")
    REGISTER_PACKET(NNormalSurfaceList, 6, "Normal Surface List")
    REGISTER_PACKET(NScript, 7, "Script")
    REGISTER_PACKET(NSurfaceFilter, 8, "Surface Filter")
    REGISTER_PACKET(NAngleStructureList, 9, "Angle Structure List")
#endif

/*! \file packetregistry.h
 *  \brief Contains a registry of packet types known to the engine.
 *
 *  Each time a new packet type is created, this packet registry should be
 *  updated.  Instructions regarding how to do this are included in
 *  <i>packetregistry.h</i>, which also contains instructions regarding
 *  how to actually use the packet registry.
 *
 *  See NPacket for further details.
 */

