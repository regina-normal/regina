#!/bin/sh
#
# Establishes which flavour of j2sdk1.3 is installed.
# This will determine the locations of the j2sdk1.3 files.
# Output will be either "old" or "new".

set -e

if [ -f /usr/lib/j2re1.3/lib/rt.jar ]; then
	echo old
elif [ -f /usr/lib/j2se/1.3/jre/lib/rt.jar ]; then
	echo new
else
	echo "No j2sdk1.3 files found." 1>&2
	exit 1
fi
