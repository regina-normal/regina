
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

#include "EngineI.h"

#include <iostream.h>
#include <string.h>

#define HOST_FLAG "-ORBInitialHost"
#define PORT_FLAG "-ORBInitialPort"
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT "8088"

void stringCopy(char** dest, const char* src) {
	if (*dest)
		delete[] (*dest);
	*dest = new char[strlen(src) + 1];
	strcpy(*dest, src);
}

/**
 * Provides a complete argument set including explicit host and port
 * details.
 */
struct ArgSet {
	int argc;
		/**< The number of arguments. */
	char** argv;
		/**< The array of argument strings. */

	char* host;
		/**< The CORBA name service host to use; this information will
		 *   also appear in array \a argv. */
	char* port;
		/**< The CORBA name service port to use; this information will
		 *   also appear in array \a argv. */

	/**
	 * Creates a new argument set based upon the given argument set.
	 * Explicit host and port details will be added if they have not
	 * already been specified.
	 *
	 * If any problems occur during parsing, member \a argv will be set
	 * to 0.
	 *
	 * @param initArgC the number of arguments in the given set.
	 * @param initArgV the array of argument strings in the given set.
	 */
	ArgSet(int initArgC, char* initArgV[]);
	/**
	 * Destroys this argument set and deallocates all correpsonding memory.
	 */
	~ArgSet();
};

ArgSet::ArgSet(int initArgC, char* initArgV[]) {
	argc = 0;
	argv = 0;

	// First hunt for a preexisting host and port.
	host = 0;
	port = 0;

	int i;
	for (i = 0; i < initArgC; i++)
		if (strcmp(initArgV[i], HOST_FLAG) == 0 && i < initArgC - 1) {
			if (host) {
				cerr << "More than one CORBA name service host was\n";
				cerr << "specified on the command line.\n";
				return;
			}
			stringCopy(&host, initArgV[i + 1]);
		} else if (strcmp(initArgV[i], PORT_FLAG) == 0 && i < initArgC - 1) {
			if (port) {
				cerr << "More than one CORBA name service port was\n";
				cerr << "specified on the command line.\n";
				return;
			}
			stringCopy(&port, initArgV[i + 1]);
		}

	// Prepare to create the new argument array.
	argc = initArgC + (host ? 0 : 2) + (port ? 0 : 2);
	argv = new (char*)[argc];

	// Copy in the initial arguments.
	for (i = 0; i < initArgC; i++)
		stringCopy(argv + i, initArgV[i]);
	
	// Add the default host and port if necessary.
	i = initArgC;
	if (! host) {
		stringCopy(&host, DEFAULT_HOST);
		stringCopy(argv + i, HOST_FLAG);
		stringCopy(argv + i + 1, host);
		i += 2;
	}
	if (! port) {
		stringCopy(&port, DEFAULT_PORT);
		stringCopy(argv + i, PORT_FLAG);
		stringCopy(argv + i + 1, port);
		i += 2;
	}
}

ArgSet::~ArgSet() {
	if (host)
		delete[] host;
	if (port)
		delete[] port;
	if (argv) {
		for (int i = 0; i < argc; i++)
			delete[] argv[i];
		delete[] argv;
	}
}

static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb,
		CORBA::Object_ptr obj) {
	CosNaming::NamingContext_var rootContext;

	try {
		CORBA::Object_var initServ =
			orb->resolve_initial_references("NameService");
		rootContext = CosNaming::NamingContext::_narrow(initServ);
		if (CORBA::is_nil(rootContext)) {
			cerr << "Could not narrow the naming context.\n";
			return 0;
		}
	} catch (CORBA::ORB::InvalidName& ex) {
		cerr << "Could not find the requested naming service.\n";
		return 0;
	}

	try {
		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id = (const char*) "regina";
		contextName[0].kind = (const char*) "context";

		CosNaming::NamingContext_var reginaContext;
		try {
			reginaContext = rootContext->bind_new_context(contextName);
		} catch (CosNaming::NamingContext::AlreadyBound& ex) {
			CORBA::Object_var tmpObj = rootContext->resolve(contextName);
			reginaContext = CosNaming::NamingContext::_narrow(tmpObj);
			if (CORBA::is_nil(reginaContext)) {
				cerr << "Could not narrow the naming context.\n";
				return 0;
			}
		}

		CosNaming::Name objectName;
		objectName.length(1);
		objectName[0].id = (const char*) "Engine";
		objectName[0].kind = (const char*) "object";

		try {
			reginaContext->bind(objectName, obj);
		} catch (CosNaming::NamingContext::AlreadyBound& ex) {
			reginaContext->rebind(objectName, obj);
		}
	} catch (CORBA::COMM_FAILURE& ex) {
		cerr << "A communication failure occurred\n";
		cerr << "    whilst attempting to contact the naming service.\n";
		return 0;
	} catch (omniORB::fatalException& ex) {
		throw;
	} catch (...) {
		cerr << "A system exception was caught\n";
		cerr << "    whilst using the naming service.\n";
		return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	// Prepare a new set of arguments including explicit host and port
	// details if they are not already present.
	ArgSet args(argc, argv);
	if (args.argv == 0)
		return 1;
	cerr << "Using host [" << args.host << "], port [" << args.port << "].\n";

	cerr << "Initialising ORB... ";
	CORBA::ORB_ptr orb = CORBA::ORB_init(args.argc, args.argv, "omniORB2");
	if (CORBA::is_nil(orb)) {
		cerr << "Could not initialise.\n";
		return 1;
	}
	cerr << "Done.\n";

	cerr << "Initialising BOA... ";
	CORBA::BOA_ptr boa = orb->BOA_init(args.argc, args.argv, "omniORB2_BOA");
	if (CORBA::is_nil(orb)) {
		cerr << "Could not initialise.\n";
		orb->NP_destroy();
		return 1;
	}
	cerr << "Done.\n";

	cerr << "Creating engine... ";
	Engine_i *engine = new Engine_i();
	engine->_obj_is_ready(boa);
	cerr << "Done.\n";

	cerr << "Binding engine to name... ";
	Regina::Engine_var ref = engine->_this();
	if (! bindObjectToName(orb, ref)) {
		boa->destroy();
		orb->NP_destroy();
		return 1;
	}
	cerr << "Done.\n";

	cerr << "Starting server.\n";
	boa->impl_is_ready();
		/* - blocks indefinitely; pass parameters 0, 1 to avoid blocking. */

	// We never make it to this point unless boa->impl_shutdown() is
	// called from another thread.

	cerr << "Destroying BOA... ";
	boa->destroy();
	cerr << "Done.\n";

	cerr << "Destroying ORB... ";
	orb->NP_destroy();
	cerr << "Done.\n";

	cerr << "Server stopped.\n";

	return 0;
}

