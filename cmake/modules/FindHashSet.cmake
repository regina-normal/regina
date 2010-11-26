
# Modules needed 
include( CheckIncludeFileCXX)
include( CheckCXXSourceCompiles)

# Find <hash_set> or <ext/hash_set>
check_include_file_cxx(hash_set HAVE_HASH_SET)
if(NOT HAVE_HASH_SET)
  check_include_file_cxx(ext/hash_set HAVE_EXT_HASH_SET)
endif(NOT HAVE_HASH_SET)

# Set the _FOUND variable
if(HAVE_EXT_HASH_SET OR HAVE_HASH_SET)
  set(HashSet_FOUND TRUE)
endif(HAVE_EXT_HASH_SET OR HAVE_HASH_SET)


# Header prefix for below tests
if(HAVE_EXT_HASH_SET)
  set(HASH_HEADER_PREFIX "ext/")
endif(HAVE_EXT_HASH_SET)


if(HashSet_FOUND)
  # Find namespace
  foreach(NAMESPACE std __gnu_cxx)
    check_cxx_source_compiles("
    #include <${HASH_HEADER_PREFIX}hash_set>
    ${NAMESPACE}::hash_set<int> s;int main(){return 0;}"
    NAMESPACE_${NAMESPACE})
    # Note that the above variables change depending on the namespace
    # variable, to avoid caching issues

    # Then store the result
    if(NAMESPACE_${NAMESPACE})
      set(__HASH_NAMESPACE ${NAMESPACE})
    endif(NAMESPACE_${NAMESPACE})
  endforeach(NAMESPACE)

  # CHeck to see if we can alias a namespace
  check_cxx_source_compiles("
  #include <${HASH_HEADER_PREFIX}hash_set>
  namespace stdalias = ${__HASH_NAMESPACE};
  stdalias::hash_set<int> s;int main(){return 0;}"
  ALIAS)
  if (NOT ALIAS)
    set(__NO_NAMESPACE_ALIASES)
  endif (NOT ALIAS)
else(HashSet_FOUND)
  if(HashSet_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find <hash_set.h> nor <ext/hash_set.h>")
  endif(HashSet_FIND_REQUIRED)
endif(HashSet_FOUND)
