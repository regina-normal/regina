#!/bin/sh
# Wrapper startup script that sets environment variables and calls the
# real startup script.

export REGINA_HOME=/usr/share/regina-normal
export REGINA_OPTIONS_GLOBAL=/etc/regina-normal
export REGINA_BTOOLS=/usr/share/java/libbtools-java.jar
export REGINA_JYTHON=/usr/share/java/jython.jar
export REGINA_JNIDIR=/usr/lib/regina-normal/lib
export REGINA_JARDIR=/usr/share/regina-normal/jar

/usr/share/regina-normal/regina "$@"
