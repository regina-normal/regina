
INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})

# Give a way to print out information on the compilers and environment.
# A warning: the cmake variable substitutions in env will break if they
# contain single quotes (but these variables are coming from the
# installed compilers/tools/etc., so in theory they shouldn't).
ADD_EXECUTABLE(arch arch.cpp)
CONFIGURE_FILE(env.in "${CMAKE_CURRENT_BINARY_DIR}/env")
add_custom_target(env "${CMAKE_CURRENT_BINARY_DIR}/env"
  DEPENDS arch env
)

# A make target whose aim is to give us some confidence that all the
# files that *should* have been built have actually been built.
# This is only suitable for use in packaging mode, in that it assumes
# that all optional build dependencies should have been found.
CONFIGURE_FILE(verify_build.in "${CMAKE_CURRENT_BINARY_DIR}/verify_build")
add_custom_target(verify_build "${CMAKE_CURRENT_BINARY_DIR}/verify_build"
  DEPENDS verify_build
)
