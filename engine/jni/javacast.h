
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#ifndef __JAVACAST_H
#ifndef __DOXYGEN
#define __JAVACAST_H
#endif

/*! \file javacast.h
 *  \brief Facilitates conversion between C++ pointers and Java longs.
 */

#include <jni.h>

namespace regina {
    class ShareableObject;
}

#ifndef __USE_RAW_CASTING
    /**
     * Casts the given Java long as a C++ pointer.
     * This is used when storing a C++ pointer as a long member of a
     * Java object.  The returned value is recast as a
     * (regina::ShareableObject*).
     *
     * These routines <b>must</b> be used when moving between C++ pointers
     * and Java longs!  They resolve serious complications caused by multiple
     * inheritance.
     *
     * \ifaces Not present.
     *
     * @param i the original Java long.
     * @return the corresponding C++ pointer.
     */
    regina::ShareableObject* javaLongToPtr(jlong i);
    /**
     * Casts the given C++ pointer as a Java long.
     * This is used when storing a C++ pointer as a long member of a
     * Java object.  The pointer will be recast as a
     * (regina::ShareableObject*) before conversion.
     *
     * These routines <b>must</b> be used when moving between C++ pointers
     * and Java longs!  They resolve serious complications caused by multiple
     * inheritance.
     *
     * \ifaces Not present.
     *
     * @param p the original C++ pointer; this must be of or derived
     * from type (regina::ShareableObject*).
     * @return the corresponding Java long.
     */
    jlong javaPtrToLong(regina::ShareableObject* p);
#else
    /**
     * \hideinitializer
     *
     * Casts the given Java long as a C++ pointer.
     * This is used when storing a C++ pointer as a long member of a
     * Java object.  The returned value is recast as a
     * (regina::ShareableObject*).
     *
     * These routines <b>must</b> be used when moving between C++ pointers
     * and Java longs!  They resolve serious complications caused by multiple
     * inheritance.
     *
     * \ifaces Not present.
     *
     * @param i the original Java long.
     * @return the corresponding C++ pointer; this will be of type
     * (regina::ShareableObject*).
     */
    #define javaLongToPtr(i) ((regina::ShareableObject*)((void*)(i)))
    /**
     * \hideinitializer
     *
     * Casts the given C++ pointer as a Java long.
     * This is used when storing a C++ pointer as a long member of a
     * Java object.  The pointer will be recast as a
     * (regina::ShareableObject*) before conversion.
     *
     * These routines <b>must</b> be used when moving between C++ pointers
     * and Java longs!  They resolve serious complications caused by multiple
     * inheritance.
     *
     * \ifaces Not present.
     *
     * @param p the original C++ pointer; this must be of or derived
     * from type (regina::ShareableObject*).
     * @return the corresponding Java long.
     */
    #define javaPtrToLong(p) ((jlong)((regina::ShareableObject*)(p)))
#endif

#endif

