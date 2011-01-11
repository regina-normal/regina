# snappea kernel

# Files to compile
SET ( FILES Dehn_coefficients canonize_part_1 change_peripheral_curves chern_simons choose_generators close_cusps complex core_geodesics current_curve_basis cusp_cross_sections cusp_neighborhoods cusp_shapes cusps edge_classes filling find_cusp finite_vertices gcd gluing_equations holonomy hyperbolic_structure identify_solution_type interface intersection_numbers my_malloc o31_matrices orient peripheral_curves positioned_tet precision shortest_cusp_basis simplify_triangulation solve_equations subdivide tables tet_shapes tidy_peripheral_curves transcendentals triangulations unix_file_io update_shapes volume )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/kernel/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES SnapPea.h canonize.h covers.h dual_one_skeleton_curve.h isometry.h kernel.h kernel_prototypes.h kernel_typedefs.h link_projection.h positioned_tet.h symmetry_group.h tables.h terse_triangulation.h tersest_triangulation.h triangulation.h triangulation_io.h unix_file_io.h winged_edge.h DESTINATION ${INCLUDEDIR}/snappea/kernel)
EXTRA_DIST(FILES README.txt)
