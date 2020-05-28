file(
  GLOB_RECURSE
  sfmNet_files_header
  *.hpp
)
file(
  GLOB_RECURSE
  sfmNet_files_cpp
  *.cpp
)
#file(GLOB_RECURSE REMOVEFILESUNITTEST *_test.cpp)
#Remove the future main files
#list(REMOVE_ITEM sfmNet_files_cpp ${REMOVEFILESUNITTEST})
#Remove the test header main files
#file(GLOB_RECURSE REMOVEFILESUNITTEST *_test.hpp)
#list(REMOVE_ITEM sfmNet_files_header ${REMOVEFILESUNITTEST})

pybind11_add_module(openMVG_sfmNet ${sfmNet_files_header} ${sfmNet_files_cpp})

target_link_libraries(openMVG_sfmNet
  PUBLIC
    openMVG_geometry
    openMVG_features
    openMVG_graph
    openMVG_matching
    openMVG_multiview
    cereal
    ${OPENMVG_LIBRARY_DEPENDENCIES}
)

target_link_libraries(openMVG_sfmNet
  LINK_PRIVATE
    ${CERES_LIBRARIES}
    openMVG_system
    openMVG_image
    openMVG_exif
    ${STLPLUS_LIBRARY}
)


target_include_directories(openMVG_sfmNet
  PUBLIC
    $<BUILD_INTERFACE:${EIGEN_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
if (DEFINED OpenMVG_USE_INTERNAL_EIGEN)
  target_include_directories(openMVG_sfmNet PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/openMVG/third_party/eigen/>)
else()
  target_include_directories(openMVG_sfmNet PUBLIC
    $<INSTALL_INTERFACE:${EIGEN_INCLUDE_DIRS}>)
endif()

#add_library(openMVG_sfmNet MODULE ${sfmNet_files_header} ${sfmNet_files_cpp})

#target_link_libraries(openMVG_sfmNet PRIVATE pybind11::module)
#set_target_properties(openMVG_sfmNet PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}")

set_target_properties(openMVG_sfmNet PROPERTIES SOVERSION ${OPENMVG_VERSION_MAJOR} VERSION "${OPENMVG_VERSION_MAJOR}.${OPENMVG_VERSION_MINOR}")
set_property(TARGET openMVG_sfmNet PROPERTY FOLDER OpenMVG/OpenMVG)
install(TARGETS openMVG_sfmNet DESTINATION lib EXPORT openMVG-targets)

