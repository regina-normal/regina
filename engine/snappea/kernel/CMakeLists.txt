# snappea kernel

# Files to compile
SET ( FILES
  Dehn_coefficients.cpp
  Moebius_transformations.cpp
  abelian_group.cpp
  canonize.cpp
  canonize_part_1.cpp
  canonize_part_2.cpp
  canonize_result.cpp
  change_peripheral_curves.cpp
  chern_simons.cpp
  choose_generators.cpp
  close_cusps.cpp
  complex.cpp
  core_geodesics.cpp
  cover.cpp
  current_curve_basis.cpp
  cusp_cross_sections.cpp
  cusp_neighborhoods.cpp
  cusp_shapes.cpp
  cusps.cpp
  edge_classes.cpp
  filling.cpp
  find_cusp.cpp
  finite_vertices.cpp
  fundamental_group.cpp
  gcd.cpp
  gluing_equations.cpp
  holonomy.cpp
  homology.cpp
  hyperbolic_structure.cpp
  identify_solution_type.cpp
  interface.cpp
  intersection_numbers.cpp
  link_complement.cpp
  matrix_conversion.cpp
  matrix_generators.cpp
  my_malloc.cpp
  o31_matrices.cpp
  orient.cpp
  peripheral_curves.cpp
  positioned_tet.cpp
  precision.cpp
  representations.cpp
  shortest_cusp_basis.cpp
  simplify_triangulation.cpp
  sl2c_matrices.cpp
  solve_equations.cpp
  subdivide.cpp
  tables.cpp
  tet_shapes.cpp
  tidy_peripheral_curves.cpp
  transcendentals.cpp
  triangulations.cpp
  unix_file_io.cpp
  update_shapes.cpp
  volume.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/kernel/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# SnapPea headers should not be shipped: these are for internal use only.
