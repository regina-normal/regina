
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file jnisafe.h
 *  \brief Includes the Java JNI headers.
 *
 *  This header is provided to take care of the macros that need to be
 *  defined \e before the Java JNI headers can be read.  This header
 *  makes the appropriate definitions and then includes the system
 *  <tt>jni.h</tt>.
 */

#ifndef __JNISAFE_H
#ifndef __DOXYGEN
#define __JNISAFE_H
#endif

#include <regina-config.h>

#ifndef HAVE___INT64
    #define __int64 long long
#endif

#include <jni.h>

#endif

