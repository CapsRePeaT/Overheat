# Install script for directory: C:/Users/asavinov/source/repos/cinolib/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/asavinov/source/repos/cinolib/examples/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/imgui/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/STB/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/_deps/tetgen-build/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/_deps/triangle-build/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/shewchuk_predicates/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/_deps/indirect_predicates-build/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/_deps/graph_cuts-build/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/01_trimesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/02_quadmesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/03_polygonmesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/04_tetmesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/05_hexmesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/06_polyhedralmesh_viewer/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/07_texture_loading/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/08_picking/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/09_polyharmonic_functions_srf/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/10_polyharmonic_functions_vol/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/11_sphere_mapping/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/12_polygon_mesh_generation/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/13_polyhedral_mesh_generation/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/14_heat_geodesics/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/16_sphere_sampling/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/17_iso_contours/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/18_iso_surfaces/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/19_harmonic_map/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/20_coarse_quad_layouts/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/21_coarse_hex_layouts/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/22_remesher/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/23_sharp_creases/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/25_surface_painter/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/26_export_surface/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/27_import_surface/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/28_tetrahedralize/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/29_ambient_occlusion/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/30_mesh_format_converter/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/31_LSCM/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/32_homotopy_basis/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/33_edge_collapse/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/34_Hermite_RBF/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/35_Poisson_sampling/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/36_canonical_polygonal_schema/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/37_find_intersections/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/38_octree/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/39_normalize_and_center/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/40_ARAP_UVmap/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/41_ARAP_deformation/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/42_connected_components/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/43_hex2tet/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/44_voxelize_mesh/cmake_install.cmake")
  include("C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/45_voxelize_function/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/asavinov/source/repos/cinolib/examples/out/build/x64-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
