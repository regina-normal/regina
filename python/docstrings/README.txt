The pybind11_mkdoc directory contains a checkout of pybind11_mkdoc:

  https://github.com/pybind/pybind11_mkdoc (commit eec43da56)

There are additional local modifications to reflect the way that Regina
uses its own custom doxygen tags.

This tool also requires the python clang bindings (on debian/ubuntu,
install the python3-clang package).

This tool is designed to be used by the Regina developers only; it is not
meant to form part of Regina's usual build process, and it may be tailored to
a setup specific to the developers' machine(s).

----------------------------------------------------------------------------

The pybind11_mkdoc tool is distributed under the following license:

  The MIT License (MIT)

  Copyright (c) 2020 Wenzel Jakob

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.