
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

/*! \file howto.h
 *  \brief Contains the Engine Extension HOWTO.
 */

/*! \page HowTo Engine Extension HOWTO
 * 
 *  Below are lists of tasks required when performing certain standard
 *  extensions to the calculation engine.
 *  <ul>
 *    <li>\ref howToNewConstant "Adding a new public constant to a class"</li>
 *    <li>\ref howToNewRoutine "Adding a new public routine to a class"</li>
 *    <li>\ref howToNewClass "Adding a new class"</li>
 *    <li>\ref howToNewNPacket "Adding a new packet type"</li>
 *    <li>\ref howToNewNNormalSurfaceVector "Adding a new normal surface coordinate system"</li>
 *    <li>\ref howToNewNSurfaceFilter "Adding a new type of normal surface filter"</li>
 *    <li>\ref howToNewNSurfaceSet "Adding a new type of normal surface set"</li>
 *  </ul>
 *
 *  <b>Location of sources:</b>
 *
 *  All paths are relative to the top-level source directory.
 *  <ul>
 *    <li>Engine sources: directory <tt>engine</tt>
 *      <ul>
 *        <li>Native engine classes: directory
 *        <tt>engine/engine</tt></li>
 *        <li>JNI native method implementations: directory
 *        <tt>engine/jni</tt></li>
 *      </ul></li>
 *    <li>Java user interface sources: directory <tt>javaui</tt></li>
 *    <li>RegConf configuration utility: directory
 *    <tt>configuration</tt></li>
 *  </ul>
 *
 *  \anchor howToNewConstant
 *  <b>Adding a new public constant to a class:</b>
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>The constant should appear in the class as a public
 *        static member.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>The same constant should appear in the Java wrapper
 *        interface (beneath package <tt>normal.engine</tt>) as a public
 *        static final member.</li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewRoutine
 *  <b>Adding a new public routine to a class:</b>
 *
 *  Note when mirroring routines in external interfaces that
 *  any public constructor <tt>ClassName::ClassName()</tt> becomes
 *  public routine <tt>Engine::newClassName()</tt>.
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>The routine should be fully documented using C++ comments in
 *        Doxygen-compatible format.  The documentation should be placed
 *        in the appropriate header file immediately above the function
 *        declaration.
 *        Examples can be found by looking through the various header
 *        files beneath directory <tt>engine</tt> in the engine sources.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>The routine should be declared in the Java wrapper
 *        interface (beneath package <tt>normal.engine</tt>).</li>
 *        <li>The routine should be implemented in the Java JNI wrapper
 *        class (beneath <tt>normal.engine.implementation.jni</tt>) as a
 *        native method.</li>
 *        <li><tt>make prepjni</tt> should be run in the top-level
 *        source directory.  This will update all the auto-generated
 *        JNI include files in the engine sources.</li>
 *        <li>The JNI native method should be implemented in the engine
 *        sources as a call to the original routine in the calculation
 *        engine.  The JNI native function declaration is in file
 *        <tt>JNIWrapperClassName.h</tt> beneath <tt>jni/engine</tt> in
 *        the engine sources (this file must not be edited).  The
 *        implementations (your task) are generally placed in file
 *        <tt>jniwrapperclassname.cpp</tt> in the same directory.
 *        Note that whenever a packet is returned, routine
 *        makeJNPacket() should be used.</li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewClass
 *  <b>Adding a new class:</b>
 *  
 *  For each class,
 *  define the <b>class location</b> to be the directory beneath
 *  <tt>engine</tt> in the engine sources in which the class is defined
 *  (note that <tt>engine</tt> in the engine sources is
 *  <tt>engine/engine</tt> relative to the top-level source directory).
 *  Let <b>ClassLoc</b> and
 *  <b>classloc</b> represent the capitalised and lowercase
 *  forms of this directory respectively.  For instance, class NFile is
 *  declared in engine/file/nfile.h in the engine sources; thus for
 *  NFile, <tt>ClassLoc</tt> is <tt>File</tt> and <tt>classloc</tt> is
 *  <tt>file</tt>.
 *
 *  Similarly, let <b>ClassName</b> and <b>classname</b> be the
 *  capitalised and lowercase class name respectively.  Let
 *  <b>FooClassName</b> be the class name with a prepended <tt>Foo</tt>,
 *  with the exception that if <tt>ClassName</tt> begins with a leading
 *  <tt>N</tt> then this <tt>N</tt> remains at the very front.  Let
 *  <b>fooclassname</b> be the lowercase version of this.
 *  For instance, for class NFile, <tt>ClassName</tt>,
 *  <tt>classname</tt>, <tt>JNIClassName</tt> and
 *  <tt>jniclassname</tt> are NFile, <tt>nfile</tt>,
 *  <tt>NJNIFile</tt> and <tt>njnifile</tt> respectively.
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>The class header file should reside in directory
 *        <tt>engine/classloc</tt> in the engine sources.</li>
 *        <li>If you have created a new subdirectory in the engine
 *        sources, that subdirectory should be listed in
 *        <tt>Makefile.sources</tt> in the engine sources.</li>
 *        <li>The class should be fully documented using C++ comments in
 *        Doxygen-compatible format.  The documentation should be placed
 *        in the header file in which the class is defined.  There
 *        should be general class notes immediately above the class
 *        declaration, and every routine and data member, public or
 *        private, should be documented where it is declared.
 *        Examples can be found by looking through the various header
 *        files beneath directory <tt>engine</tt> in the engine sources.</li>
 *        <li>If the class is to be mirrored in the external
 *        interfaces, it must be descended from ShareableObject.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>The Java wrapper interface
 *        <tt>normal.engine.classloc.ClassName</tt> must be created.
 *        This will be the interface with which the generic Java UI
 *        components interact.
 *        Note that the names and parameters of some
 *        routines may need to be altered according to the specifications
 *        on the \ref JavaNotes page.</li>
 *        <li>The Java JNI wrapper class
 *        <tt>normal.engine.implementation.jni.classloc.JNIClassName</tt>
 *        must be created.  It
 *        must descend from <tt>JNIShareableObject</tt> and implement
 *        the wrapper interface <tt>ClassName</tt> defined in the previous
 *        step.  It should contain public constructor
 *        <tt>ClassName(normal.engine.implementation.jni.Sentry)</tt>
 *        that does nothing.  All constructors must call the parent
 *        constructor <tt>ParentClass(Sentry)</tt>;
 *        <tt>Sentry.instance</tt> can always be used if a
 *        <tt>Sentry</tt> is needed.
 *        See the HOWTO for \ref howToNewRoutine "adding a new routine"
 *        for details regarding implementing individual routines in the
 *        wrapper class.</li>
 *        <li>Any new packages created must be included in
 *        <tt>Makefile.sources</tt> in the Java UI sources.  This could
 *        include <tt>normal.engine.classloc</tt> and
 *        <tt>normal.engine.implementation.jni.classloc</tt>.
 *        <li><tt>make prepjni</tt> should be run in the top-level
 *        source directory.  This will create all the auto-generated
 *        JNI include files in the engine sources.</li>
 *        <li>The JNI native methods for the class should be implemented
 *        in the engine sources, as described in the HOWTO for
 *        \ref howToNewRoutine "adding a new routine".
 *        The JNI native function declarations are all in file
 *        <tt>jni/engine/classloc/JNIClassName.h</tt> in the engine
 *        sources.  If this is a new source directory it should be
 *        included in <tt>Makefile.sources</tt> in the engine sources.</li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewNPacket
 *  <b>Adding a new packet type (subclassing NPacket):</b>
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>Make the required declarations and implementations as
 *        described in the NPacket class notes.</li>
 *        <li>engine/packet/packetregistry.h should be updated to
 *        register the new packet class.  There are instructions in
 *        packetregistry.h explaining how to do this.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>jni/registry/javapacketregistry.h in the engine sources
 *        should be updated to register the new packet class.  Again the
 *        file itself contains instructions.</li>
 *        <li>The Java wrapper interface (beneath package
 *        <tt>normal.engine</tt>) for the new packet type should have a
 *        member <tt>public static final int packetType</tt> storing the
 *        same packet type ID as is recorded in
 *        engine/packet/packetregistry.h.</li>
 *        <li>Java class <tt>normal.packetui.PacketUIManager</tt> should
 *        be updated to include any UI components that will interact with
 *        the new packet type.</li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewNNormalSurfaceVector
 *  <b>Adding a new normal surface coordinate system:</b>
 *  <ul>
 *    <li><b>For surface viewing:</b>
 *      <ul>
 *        <li><i>Engine:</i>
 *          <ul>
 *            <li>A unique constant (<tt>static const int</tt>) must
 *            be added to class NNormalSurfaceList to represent the new
 *            flavour of coordinate system.</li>
 *          </ul></li>
 *        <li><i>Java User Interface:</i>
 *          <ul>
 *            <li>The same flavour constant needs to be added to Java
 *            interface
 *            <tt>normal.engine.surfaces.NNormalSurfaceList</tt>.</li>
 *            <li>The routines in Java class
 *            <tt>normal.packetui.surfaces.Coordinates</tt> need to be
 *            updated to include the new flavour of coordinate system.</li>
 *          </ul>
 *      </ul></li>
 *    <li><b>For surface creation (subclassing NNormalSurfaceVector):</b>
 *      <ul>
 *        <li>The tasks described above for a surface viewing coordinate
 *        system must be completed.  Additionally:</li>
 *        <li><i>Engine:</i>
 *          <ul>
 *            <li>A new subclass of NNormalSurfaceVector must be created
 *            to represent the new flavour of coordinate system.  A list
 *            of required declarations and implementations is given in
 *            the NNormalSurfaceVector class notes.</li>
 *            <li>engine/surfaces/flavourregistry.h should be updated to
 *            register the new flavour of coordinate system.  There are
 *            instructions in flavourregistry.h explaining how to do
 *            this.</li>
 *          </ul></li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewNSurfaceFilter
 *  <b>Adding a new type of normal surface filter (subclassing
 *  NSurfaceFilter):</b>
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>Make the required declarations and implementations as
 *        described in the NSurfaceFilter class notes.</li>
 *        <li>engine/surfaces/filterregistry.h should be updated to
 *        register the new filtering class.  There are instructions in
 *        filterregistry.h explaining how to do this.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>jni/registry/javafilterregistry.h in the engine sources
 *        should be updated to register the new filtering class.  Again the
 *        file itself contains instructions.</li>
 *        <li>The Java wrapper interface (beneath package
 *        <tt>normal.engine</tt>) for the new type of surface filter
 *        should have a
 *        member <tt>public static final int filterID</tt> storing the same
 *        filter ID as is recorded in engine/surfaces/filterregistry.h.</li>
 *        <li>Routine <tt>packetIcon()</tt> in Java class
 *        <tt>normal.packetui.PacketUIManager</tt> should
 *        be updated to return an icon for the new type of surface
 *        filter.</li>
 *        <li>Java class
 *        <tt>normal.packetui.surfaces.NSurfaceFilterCreator</tt> should
 *        be updated to allow creation of the new type of surface filter.</li>
 *        <li>If a new UI component has been created for interaction
 *        with the new type of surface filter, routine
 *        <tt>fillWithInterfaces()</tt> in Java class
 *        <tt>normal.packetui.surfaces.NSurfaceFilterEditor</tt> should
 *        be updated to include this new UI component.</li>
 *      </ul></li>
 *  </ul>
 *
 *  \anchor howToNewNSurfaceSet
 *  <b>Adding a new type of normal surface set (subclassing NSurfaceSet):</b>
 *  <ul>
 *    <li><i>Engine:</i>
 *      <ul>
 *        <li>Your new class should be a descendant of both
 *        ShareableObject and NSurfaceSet; this may require multiple
 *        inheritance.</li>
 *        <li>Make the required declarations and implementations as
 *        described in the NSurfaceSet class notes.</li>
 *      </ul></li>
 *    <li><i>Java User Interface:</i>
 *      <ul>
 *        <li>Your Java wrapper interface (beneath package
 *        <tt>normal.engine</tt>) should be a descendant of both
 *        ShareableObject and NSurfaceSet.</li>
 *        <li>Your Java JNI wrapper class (beneath package
 *        <tt>normal.engine.implementation.jni</tt>) should descend from
 *        <tt>JNIShareableObject</tt> and simply implement NSurfaceSet.</li>
 *        <li>jni/registry/javasurfacesetregistry.h in the engine sources
 *        should be updated to register the new surface set type.  There
 *        are instructions in javasurfacesetregistry.h explaining how to
 *        do this.</li>
 *      </ul></li>
 *  </ul>
 */

