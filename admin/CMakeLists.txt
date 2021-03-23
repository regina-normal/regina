
INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})

# Give a way to print out information on the compilers and environment.
# A warning: the cmake variable substitutions in env will break if they
# contain single quotes (but these variables are coming from the
# installed compilers/tools/etc., so in theory they shouldn't).
ADD_EXECUTABLE(arch arch)
CONFIGURE_FILE(env.in "${CMAKE_CURRENT_BINARY_DIR}/env")
add_custom_target(env "${CMAKE_CURRENT_BINARY_DIR}/env"
  DEPENDS arch env
)
