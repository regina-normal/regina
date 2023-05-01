Regina Performance Tests
------------------------

This directory contains programs that perform intensive operations.

Developers can time these programs to help estimate how well various
tweaks / modifications / environmental changes are working in practice.

Programs must be run manually from the corresponding build directory.

On macOS/Xcode, to trace from the command line:

  xctrace record --output <path/to/output.trace> --template "Time Profiler" --launch <executable>
