# snappea kernel

# Files to compile
SET ( FILES
  Dehn_coefficients.c
  Moebius_transformations.c
  abelian_group.c
  canonize.c
  canonize_part_1.c
  canonize_part_2.c
  canonize_result.c
  change_peripheral_curves.c
  chern_simons.c
  choose_generators.c
  close_cusps.c
  complex.c
  core_geodesics.c
  cover.c
  current_curve_basis.c
  cusp_cross_sections.c
  cusp_neighborhoods.c
  cusp_shapes.c
  cusps.c
  edge_classes.c
  filling.c
  find_cusp.c
  finite_vertices.c
  fundamental_group.c
  gcd.c
  gluing_equations.c
  holonomy.c
  homology.c
  hyperbolic_structure.c
  identify_solution_type.c
  interface.c
  intersection_numbers.c
  link_complement.c
  matrix_conversion.c
  matrix_generators.c
  my_malloc.c
  o31_matrices.c
  orient.c
  peripheral_curves.c
  positioned_tet.c
  precision.c
  representations.c
  shortest_cusp_basis.c
  simplify_triangulation.c
  sl2c_matrices.c
  solve_equations.c
  subdivide.c
  tables.c
  tet_shapes.c
  tidy_peripheral_curves.c
  transcendentals.c
  triangulations.c
  unix_file_io.c
  update_shapes.c
  volume.c
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
