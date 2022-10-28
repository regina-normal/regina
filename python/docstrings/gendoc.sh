#!/bin/bash
set -e

if [ "$#" = 0 ]; then
  dirs="algebra angle census core enumerate file foreign hypersurface link manifold maths packet progress python snappea split subcomplex surface treewidth triangulation utilities"
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
export CPP_INCLUDE_DIRS="../../engine ../../preconfig/macos"

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
    python3 ./mkdoc.py -std=c++17 \
      -o python/equality.h "../helpers/equality.h"
  else
    if [ "$dir" = core ]; then
      python3 ./mkdoc.py -std=c++17 \
        -o core/regina-core.h "../../engine/regina-core.h"
    elif [ "$dir" = maths ]; then
      for i in ../../engine/"$dir"/spec/*.h; do
        header=`basename "$i"`
        python3 ./mkdoc.py -std=c++17 \
          -o "$dir/$header" ../../engine/"$dir/spec/$header"
      done
    elif [ "$dir" = triangulation ]; then
      for sub in dim2 dim3 dim4 generic alias detail; do
        for i in ../../engine/"$dir"/"$sub"/*.h; do
          header=`basename "$i"`
          case "$sub/$header" in
            *-impl.h ) ;;
            detail/retriangulate.h ) ;;
            * )
              python3 ./mkdoc.py -std=c++17 \
                -o "$dir/$sub/$header" ../../engine/"$dir/$sub/$header"
              ;;
          esac
        done
      done
    fi
    for i in ../../engine/"$dir"/*.h; do
      header=`basename "$i"`
      case "$dir/$header" in
        *-impl.h ) ;;
        link/graph.h ) ;;
        manifold/notation.h ) ;;
        triangulation/dim?.h ) ;;
        triangulation/facetpairing.h ) ;;
        triangulation/forward.h ) ;;
        triangulation/generic.h ) ;;
        triangulation/graph.h ) ;;
        triangulation/pachner.h ) ;;
        utilities/markedvector.h ) ;;
        utilities/memstream.h ) ;;
        utilities/sequence.h ) ;;
        utilities/shortarray.h ) ;;
        utilities/typeutils.h ) ;;
        utilities/zstr.h ) ;;
        * )
          python3 ./mkdoc.py -std=c++17 \
            -o "$dir/$header" ../../engine/"$dir/$header"
          ;;
      esac
    done
  fi
done

