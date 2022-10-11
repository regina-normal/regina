#!/bin/bash
set -e

if [ "$#" = 0 ]; then
  dirs="algebra core file foreign maths packet progress python split treewidth utilities"
else
  dirs="$@"
fi

usage() {
  if [ -n "$1" ]; then echo "ERROR: $1"; echo; fi
  echo "Usage: $0 [ <engine_dir> ... ]"
  echo "Example: $0 algebra core packet"
  exit 1
}

if [ ! -e gendoc.sh ]; then
  usage "This script must be run from python/docstrings/ in Regina's source tree."
fi

# For the include path, we grab the pre-rolled macOS config file.
# The choice of config file shouldn't actually affect the docstrings.
export PYTHONPATH=".:$PYTHONPATH"
export CPP_INCLUDE_DIRS="../../engine ../../preconfig/osx-xcode5"

for dir in $dirs; do
  case "$dir" in
    *[/\\:\;]* )
      usage "Argument $dir should be a single directory name, not a full path"
      exit 1
      ;;
    * )
      ;;
  esac
  if [ "$dir" != python -a ! -d ../../engine/"$dir" ]; then
    usage "Argument $dir should be a subdirectory of ../../engine"
    exit 1
  fi
  if [ ! -d "$dir" ]; then
    usage "Argument $dir does not have a docstring output directory"
    exit 1
  fi

  if [ "$dir" = python ]; then
    python3 -m pybind11_mkdoc -std=c++17 \
      -o python/equality.h "../helpers/equality.h"
  else
    if [ "$dir" = core ]; then
      python3 -m pybind11_mkdoc -std=c++17 \
        -o core/regina-core.h "../../engine/regina-core.h"
    elif [ "$dir" = maths ]; then
      for i in ../../engine/"$dir"/spec/*.h; do
        header=`basename "$i"`
        python3 -m pybind11_mkdoc -std=c++17 \
          -o "$dir/$header" ../../engine/"$dir/spec/$header"
      done
    fi
    for i in ../../engine/"$dir"/*.h; do
      header=`basename "$i"`
      case "$dir/$header" in
        *-impl.h ) ;;
        utilities/markedvector.h ) ;;
        utilities/memstream.h ) ;;
        utilities/sequence.h ) ;;
        utilities/shortarray.h ) ;;
        utilities/typeutils.h ) ;;
        utilities/zstr.h ) ;;
        * )
          python3 -m pybind11_mkdoc -std=c++17 \
            -o "$dir/$header" ../../engine/"$dir/$header"
          ;;
      esac
    done
  fi
done

