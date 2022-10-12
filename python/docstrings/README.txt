The file mkdoc.py contains a very heavily modified version of pybind11_mkdoc.
The original was taken from:

  https://github.com/pybind/pybind11_mkdoc (commit eec43da56)

The local modifications reorganise the way docstrings are stored and accessed,
strip out a significant amount of unwanted noise (duplicates and/or methods
that Regina does not bind in python), understand a richer variety of Doxygen
markup commands, and better reflect Regina's own custom Doxygen tags.

This tool requires the python clang bindings (on debian/ubuntu, install the
package python3-clang).

This tool is designed to be used by the Regina developers only; it is not
meant to form part of Regina's usual build process.  To regenerate all
docstrings, run ./gendoc.sh.  To regenerate only some of the docstrings,
you can pass ./gendoc.sh an optional list of engine subdirectories.

Most of the headers define *many* static const strings. Therefore:

- Normally, each header should only be included by _one_ of the compiled
  binding sources (*.cpp).

- The exception is those headers that use inline constexpr strings, which
  can happily be included many times. These are whitelisted in the conversion
  tool (see INLINE_FILES near the top of pybind11_mkdoc/mkdoc_lib.py).
  You should *not* whitelist a header that will produce symbols that will
  be used again elsewhere (e.g., the Perm<n> docstrings, all of which define
  different variants of Perm and Perm_ corresponding to different template
  specialisations).

----------------------------------------------------------------------------

The original pybind11_mkdoc tool is distributed under the following license:

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

All modifications are Copyright (c) 2022 Ben Burton, and released under the
same license as above.
