Regina Python Interface Directory
---------------------------------

This directory tree contains the sources for the calculation engine
Python bindings, which are built into the Python module "regina".

Like the calculation engine itself, these sources are written in C++.
They use the pybind11 header-only library, which is shipped with Regina
in the subdirectories regina/pybind11_v2/ (for use with Python ≤ 3.11)
and regina/pybind11_v3/ (for use with Python ≥ 3.12).
