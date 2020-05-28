add_executable(openMVG_sfmNet_test sfmNet_test.cpp)
target_link_libraries(openMVG_sfmNet_test
  INTERFACE 
    pybind11::module
    openMVG_sfmNet
  PRIVATE
    openMVG_system
    openMVG_image
    openMVG_features
    openMVG_sfm
    openMVG_exif
    ${STLPLUS_LIBRARY}
)

# Installation rules
set_property(TARGET openMVG_sfmNet_test PROPERTY FOLDER OpenMVG/sfmNet_test)
install(TARGETS openMVG_sfmNet_test DESTINATION bin/)