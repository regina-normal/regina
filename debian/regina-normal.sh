#!/bin/sh
# Wrapper startup script that sets environment variables and calls the
# real startup script.

if [ -z "$REGINA_HOME" ]; then
  export REGINA_HOME=/usr/share/regina-normal
fi
if [ -z "$REGINA_OPTIONS_GLOBAL" ]; then
  export REGINA_OPTIONS_GLOBAL=/etc/regina-normal
fi
if [ -z "$REGINA_BTOOLS" ]; then
  export REGINA_BTOOLS=/usr/share/java/libbtools-java.jar
fi
if [ -z "$REGINA_JYTHON" ]; then
  export REGINA_JYTHON=/usr/share/java/jython.jar
fi
if [ -z "$REGINA_EXTRA_CLASSES" ]; then
  export REGINA_EXTRA_CLASSES=/usr/share/java/libreadline-java.jar
fi
if [ -z "$REGINA_JNIDIR" ]; then
  export REGINA_JNIDIR=/usr/lib/regina-normal/lib:/usr/lib
fi
if [ -z "$REGINA_JARDIR" ]; then
  export REGINA_JARDIR=/usr/share/regina-normal/jar
fi

# Change the default verbosity to quiet.
if [ -z "$REGINA_VERBOSITY" ]; then
  export REGINA_VERBOSITY=0
fi

/usr/share/regina-normal/regina "$@"
