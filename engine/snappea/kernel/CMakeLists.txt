# snappea kernel

# Files to compile
SET ( FILES
  Dehn_coefficients
  Moebius_transformations
  abelian_group
  canonize
  canonize_part_1
  canonize_part_2
  canonize_result
  change_peripheral_curves
  chern_simons
  choose_generators
  close_cusps
  complex
  core_geodesics
  current_curve_basis
  cusp_cross_sections
  cusp_neighborhoods
  cusp_shapes
  cusps
  edge_classes
  filling
  find_cusp
  finite_vertices
  fundamental_group
  gcd
  gluing_equations
  holonomy
  homology
  hyperbolic_structure
  identify_solution_type
  interface
  intersection_numbers
  matrix_conversion
  matrix_generators
  my_malloc
  o31_matrices
  orient
  peripheral_curves
  positioned_tet
  precision
  shortest_cusp_basis
  simplify_triangulation
  sl2c_matrices
  solve_equations
  subdivide
  tables
  tet_shapes
  tidy_peripheral_curves
  transcendentals
  triangulations
  unix_file_io
  update_shapes
  volume
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/kernel/${SOURCE_FILE})
  SET ( SOURCES_C_AS_CPP ${SOURCES_C_AS_CPP} snappea/kernel/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
SET(SOURCES_C_AS_CPP ${SOURCES_C_AS_CPP} PARENT_SCOPE)

# SnapPea headers should not be shipped: these are for internal use only.
