
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#import "PythonInterpreterObjC.h"
#import "packet/script.h"
#import "../python/gui/pythoninterpreter.h"
#import "../python/gui/pythonoutputstream.h"

#pragma mark Log stream

@implementation PythonLogStream

- (void)processOutput:(const char*)data
{
    // We can't use %s with NSLog, since this does not default to UTF-8.
    // Instead we jump through some hoops to explicitly specify that data is UTF-8.
    NSLog(@"%@", [NSString stringWithUTF8String:data]);
}

@end

#pragma mark C++ wrapper

class PythonOutputStreamObjC : public regina::python::PythonOutputStream {
    private:
        void* _object;
            /**< The Objective-C delegate that implements processOutput:. */

    public:
        PythonOutputStreamObjC(void* object) : _object(object) {
        }

        virtual void processOutput(const std::string& data) {
            [(__bridge id)_object processOutput:data.c_str()];
        }
};

#pragma mark Python interpreter

@implementation PythonInterpreterObjC {
    NSObject<PythonOutputStream>* _out;
    NSObject<PythonOutputStream>* _err;

    PythonOutputStreamObjC* _outCpp;
    PythonOutputStreamObjC* _errCpp;

    regina::python::PythonInterpreter* _interpreter;
}

- (id)initWithOut:(id<PythonOutputStream>)out err:(id<PythonOutputStream>)err
{
    self = [super init];
    if (self) {
        if (out)
            _out = out;
        else
            _out = [[PythonLogStream alloc] init];

        if (err)
            _err = err;
        else
            _err = [[PythonLogStream alloc] init];

        _outCpp = new PythonOutputStreamObjC((__bridge void*)_out);
        _errCpp = new PythonOutputStreamObjC((__bridge void*)_err);

        _interpreter = new regina::python::PythonInterpreter(*_outCpp, *_errCpp);
    }
    return self;
}

- (void)dealloc
{
    NSLog(@"Python interpreter being deallocated");

    // Delete C++ resources that are not managed by ARC.
    delete _interpreter;
    delete _outCpp;
    delete _errCpp;
}

- (bool)executeLine:(const char*)command
{
    return _interpreter->executeLine(command);
}

- (bool)importRegina
{
    return _interpreter->importRegina();
}

- (bool)setVar:(const char*)name value:(regina::Packet*)value
{
    return _interpreter->setVar(name, value);
}

- (bool)runCode:(const char*)code
{
    return _interpreter->runCode(code);
}

- (bool)runScript:(regina::Script*)script
{
    bool result = true;
    for (size_t i = 0; i < script->countVariables(); ++i)
        if (! [self setVar:script->variableName(i).c_str() value:script->variableValue(i)])
            result = false;

    if (! [self runCode:(script->text() + "\n\n").c_str()])
        result = false;

    return result;
}

- (void)flush
{
    _outCpp->flush();
    _errCpp->flush();
}

@end

