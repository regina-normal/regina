#!/bin/bash
set -e

# For the include path, we grab the pre-rolled macOS config file.
# The choice of config file shouldn't actually affect the docstrings.
export PYTHONPATH=".:$PYTHONPATH"
export CPP_INCLUDE_DIRS="../../engine ../../preconfig/osx-xcode5"

dirs="file"

for dir in $dirs; do
  for i in ../../engine/"$dir"/*.h; do
    header=`basename "$i"`
    case "$header" in
      *-impl.h )
        # Skip these.
        ;;
      * )
        python3 -m pybind11_mkdoc -std=c++17 \
          -o "$dir/$header" ../../engine/"$dir/$header"
        ;;
    esac
  done
done

