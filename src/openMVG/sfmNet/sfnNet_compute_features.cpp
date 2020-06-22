//string compute_features(string image_pixels) {
//
//    //read in image pixels 
//    int32_t width;
//    int32_t height;
//    cereal::BinaryInputArchive iarchive(ss); // Create an input archive
//    iarchive(width, height);
//
//  //implement error checking for if the input string does not match the expected input
//
//  bool bUpRight = false;
//  std::string sImage_Describer_Method = "SIFT";
//  bool bForce = false;
//  std::string sFeaturePreset = "";
//
//#ifdef OPENMVG_USE_OPENMP
//  int iNumThreads = 0;
//#endif
//
//  //---------------------------------------
//  // a. Load input scene - parse the string into the views and instrinsics
//  //---------------------------------------
//
//
//
//  // b. Init the image_describer
//  // - retrieve the used one in case of pre-computed features
//  // - else create the desired one
//
//  using namespace openMVG::features;
//  std::unique_ptr<Image_describer> image_describer;
//
//  const std::string sImage_describer;   //sImage_desciber is going to passed in string to this function
//  if(!bForce && sImage_desciber.empty() ) {
//    // Dynamically load the image_describer from the file (will restore old used settings)
//    std::ifstream stream(sImage_describer.c_str());
//    if (!stream.is_open())
//      return EXIT_FAILURE;
//
//      try
//      {
//        cereal::JSONInputArchive archive(stream);
//        archive(cereal::make_nvp("image_describer", image_describer));
//      }
//      catch (const cereal::Exception & e)
//      {
//        std::cerr << e.what() << std::endl
//          << "Cannot dynamically allocate the Image_describer interface." << std::endl;
//        return EXIT_FAILURE;
//      }
//  }
//  else
//  {
//    // Create the desired Image_describer method.
//    // Don't use a factory, perform direct allocation
//    if (sImage_Describer_Method == "SIFT")
//    {
//      image_describer.reset(new SIFT_Image_describer
//        (SIFT_Image_describer::Params(), !bUpRight));
//    }
//    else
//    if (sImage_Describer_Method == "SIFT_ANATOMY")
//    {
//      image_describer.reset(
//        new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
//    }
//    else
//    if (sImage_Describer_Method == "AKAZE_FLOAT")
//    {
//      image_describer = AKAZE_Image_describer::create
//        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), !bUpRight);
//    }
//    else
//    if (sImage_Describer_Method == "AKAZE_MLDB")
//    {
//      image_describer = AKAZE_Image_describer::create
//        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), !bUpRight);
//    }
//    if (!image_describer)
//    {
//      std::cerr << "Cannot create the designed Image_describer:"
//        << sImage_Describer_Method << "." << std::endl;
//      return EXIT_FAILURE;
//    }
//    else
//    {
//      if (!sFeaturePreset.empty())
//      if (!image_describer->Set_configuration_preset(stringToEnum(sFeaturePreset)))
//      {
//        std::cerr << "Preset configuration failed." << std::endl;
//        return EXIT_FAILURE;
//      }
//    }
//
//    // Export the used Image_describer and region type for:
//    // - dynamic future regions computation and/or loading
//    {
//      std::ofstream stream(sImage_describer.c_str());
//      if (!stream.is_open())
//        return EXIT_FAILURE;
//
//      cereal::JSONOutputArchive archive(stream);
//      archive(cereal::make_nvp("image_describer", image_describer));
//      auto regionsType = image_describer->Allocate();
//      archive(cereal::make_nvp("regions_type", regionsType));
//    }
//  }
//
//
//
//
//
//
//
//}
