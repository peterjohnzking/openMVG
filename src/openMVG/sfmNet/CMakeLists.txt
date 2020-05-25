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

add_library(openMVG_sfmNet ${sfmNet_files_header} ${sfmNet_files_cpp})
target_compile_features(openMVG_sfmNet INTERFACE ${CXX11_FEATURES})

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
    openMVG_graph
    openMVG_image
    openMVG_lInftyComputerVision
    openMVG_system
    ${STLPLUS_LIBRARY}
)

if (MSVC)
  set_target_properties(openMVG_sfmNet PROPERTIES COMPILE_FLAGS "/bigobj")
endif (MSVC)
set_target_properties(openMVG_sfmNet PROPERTIES SOVERSION ${OPENMVG_VERSION_MAJOR} VERSION "${OPENMVG_VERSION_MAJOR}.${OPENMVG_VERSION_MINOR}")
set_property(TARGET openMVG_sfmNet PROPERTY FOLDER OpenMVG/OpenMVG)
install(TARGETS openMVG_sfmNet DESTINATION lib EXPORT openMVG-targets)

