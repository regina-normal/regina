/****************************************************************************
**
** DCOP Skeleton created by dcopidl2cpp from reginaiface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "./reginaiface.h"

#include <kdatastream.h>


static const char* const ReginaMainInterface_ftable[7][3] = {
    { "void", "newTopology()", "newTopology()" },
    { "void", "newPython()", "newPython()" },
    { "bool", "openURL(QString)", "openURL(QString url)" },
    { "void", "pythonConsole()", "pythonConsole()" },
    { "void", "close()", "close()" },
    { "void", "quit()", "quit()" },
    { 0, 0, 0 }
};

bool ReginaMainInterface::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
    if ( fun == ReginaMainInterface_ftable[0][1] ) { // void newTopology()
	replyType = ReginaMainInterface_ftable[0][0]; 
	newTopology( );
    } else if ( fun == ReginaMainInterface_ftable[1][1] ) { // void newPython()
	replyType = ReginaMainInterface_ftable[1][0]; 
	newPython( );
    } else if ( fun == ReginaMainInterface_ftable[2][1] ) { // bool openURL(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = ReginaMainInterface_ftable[2][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << openURL(arg0 );
    } else if ( fun == ReginaMainInterface_ftable[3][1] ) { // void pythonConsole()
	replyType = ReginaMainInterface_ftable[3][0]; 
	pythonConsole( );
    } else if ( fun == ReginaMainInterface_ftable[4][1] ) { // void close()
	replyType = ReginaMainInterface_ftable[4][0]; 
	close( );
    } else if ( fun == ReginaMainInterface_ftable[5][1] ) { // void quit()
	replyType = ReginaMainInterface_ftable[5][0]; 
	quit( );
    } else {
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return TRUE;
}

QCStringList ReginaMainInterface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "ReginaMainInterface";
    return ifaces;
}

QCStringList ReginaMainInterface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; ReginaMainInterface_ftable[i][2]; i++ ) {
	QCString func = ReginaMainInterface_ftable[i][0];
	func += ' ';
	func += ReginaMainInterface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


