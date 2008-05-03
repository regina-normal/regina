#!/bin/false
#
# Regina - A Normal Surface Theory Calculator
# Python Helper Routines
#
# Copyright (c) 2002-2008, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Usage:
#
#     The helper routines in this file offer a quick way of telling whether
#     a given packet is of a particular type.
#
#     To automatically load these routines into every new Regina python
#     session, add this file as a new library in Regina's python settings.
#     Alternatively, you can add it directly as a new line to ~/.regina-libs .
#
#     Each routine is documented using standard python docstrings, which
#     means you can access this documentation from within python at runtime:
#
#         >>> from pydoc import help
#         >>> help(isTriangulation)
#
#         Help on function isTriangulation in module __main__:
#
#         isTriangulation(packet)
#             Is the given packet a 3-manifold triangulation?
#             ...
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
# MA 02110-1301, USA.

def isAngleStructureList(packet):
    """
    Is the given packet an angle structure list?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NAngleStructureList.packetType)

def isContainer(packet):
    """
    Is the given packet a container?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NContainer.packetType)

def isScript(packet):
    """
    Is the given packet a python script?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NScript.packetType)

def isSurfaceFilter(packet):
    """
    Is the given packet a normal surface filter?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NSurfaceFilter.packetType)

def isSurfaceList(packet):
    """
    Is the given packet a list of normal surfaces?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NNormalSurfaceList.packetType)

def isText(packet):
    """
    Is the given packet a text packet?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NText.packetType)

def isTriangulation(packet):
    """
    Is the given packet a 3-manifold triangulation?
    Returns 1 for true, or 0 for false.
    """
    return (packet.getPacketType() == regina.NTriangulation.packetType)

