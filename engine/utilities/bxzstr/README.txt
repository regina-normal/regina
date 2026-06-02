bxzstr C++ Compression Library
------------------------------

This directory contains a copy of bxzstr, a C++ wrapper around various
compression libraries.  This is currently synced with bxzstr version 1.2.3.

Currently this library is not built into Regina at all.  It is only used in
the tool mkcensusdb, which is compiled and run only at build time to repackage
Regina's census databases in an appropriate format for the target system.

The bxzstr library is distributed under the terms of the Mozilla Public License
Version 2.0.  The full text of this license is included in the file LICENSE in
this directory.

The bxzstr library is a fork of the zstr library by Matei David (part of which
_is_ already built into Regina via engine/utilities/zstr.h).  It was written by
Tommi Mäklin, and can be downloaded from https://github.com/tmaklin/bxzstr/ .
