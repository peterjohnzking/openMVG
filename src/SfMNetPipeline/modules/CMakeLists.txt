
###
# Test file for modules
###
add_executable(openMVG_net_pipeline_test pipeline_test.cpp)
target_link_libraries(openMVG_net_pipeline_test
  PRIVATE 
    openMVG_system
    openMVG_image
    openMVG_features
    openMVG_sfm
    openMVG_exif
    ${STLPLUS_LIBRARY}

)


# Installation rules
set_property(TARGET openMVG_net_pipeline_test PROPERTY FOLDER OpenMVG/SfMNetPipeline)
install(TARGETS openMVG_net_pipeline_test DESTINATION bin/)
