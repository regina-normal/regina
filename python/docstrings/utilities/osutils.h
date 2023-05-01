/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::resUsage
static const char *resUsage =
R"doc(Returns time and memory usage for the current process, for use on
Linux systems. Information is gathered from the ``/proc`` filesystem;
if this is run on a non-Linux system (or a Linux system where
``/proc`` is not mounted), then this routine will throw an exception.

More precisely, this routine reads information on the running process
from ``/proc/self/stat``, and returns a tuple (*utime*, *stime*,
*vsize*). These three fields reperesent:

* the number jiffies that this process has been scheduled in user mode
  (the *utime* field);

* the number jiffies that this process has been scheduled in kernel
  mode (the *stime* field);

* the the virtual memory size in bytes (the *vsize* field).

The description of these three fields is taken directly from the
``proc``(5) manpage. Note that the length of a jiffy can differ from
system to system; see the ``time``(7) manpage for details.

Exception ``FileError``:
    Either ``/proc/self/stat`` cannot be read, or it contains
    unexpected information.

.. warning::
    Currently this routine allows at most 255 characters for the
    *comm* field in ``/proc/self/stat`` (which stores the executable
    filename along with surrounding parentheses). If the *comm* field
    is too long (i.e., the executable filename is too long), then this
    routine will not be able to parse ``/proc/self/stat``, and will
    throw an exception. If you encounter this problem, you should be
    able to fix it by renaming your executable to something shorter.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

