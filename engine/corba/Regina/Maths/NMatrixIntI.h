
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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

#ifndef __NMATRIXINTI_H
#define __NMATRIXINTI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
	#include "nmatrixint.h"
	#include "corbatools.h"
#else
	#include "engine/maths/nmatrixint.h"
	#include "corba/corbatools.h"
#endif

#include "NMatrixIntIDL.h"
#include "ShareableObjectI.h"

class NMatrixInt_i : public virtual Regina::Maths::_sk_NMatrixInt,
		public ShareableObject_i {
	protected:
		NMatrixInt_i(::NMatrixInt* newCppPtr) : ShareableObject_i(newCppPtr) {
		}
	public:
		STANDARD_NEW_WRAPPER(NMatrixInt, NMatrixInt_i,
			Regina::Maths::NMatrixInt_ptr)

		virtual void initialise(const char* value);
		virtual void makeIdentity();
		virtual CORBA::Long rows();
		virtual CORBA::Long columns();
		virtual char* getEntry(CORBA::Long row, CORBA::Long column);
		virtual void setEntry(CORBA::Long row, CORBA::Long column,
			const char* value);

		virtual void swapRows(CORBA::Long first, CORBA::Long second);
		virtual void swapColumns(CORBA::Long first, CORBA::Long second);
		virtual void addRow_long_long(CORBA::Long source, CORBA::Long dest);
		virtual void addRow_long_long_bigInt(CORBA::Long source,
			CORBA::Long dest, const char* copies);
		virtual void addCol_long_long(CORBA::Long source, CORBA::Long dest);
		virtual void addCol_long_long_bigInt(CORBA::Long source,
			CORBA::Long dest, const char* copies);
		virtual void multRow(CORBA::Long row, const char* factor);
		virtual void multCol(CORBA::Long col, const char* factor);
};

#endif

