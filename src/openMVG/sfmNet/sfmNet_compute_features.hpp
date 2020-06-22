// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2012, 2013 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// The <cereal/archives> headers are special and must be included first.

/* Assumes only one potential preset for image quality
* Sfm_data is not being until it is absolutely needed as this step only needs VIEWS and INTRINSICS
*
*
*/
#include <cereal/archives/json.hpp>

#include "openMVG/features/akaze/image_describer_akaze_io.hpp"

#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/progress/progress_display.hpp"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include "nonFree/sift/SIFT_describer_io.hpp"

#include <cereal/details/helpers.hpp>

#include <atomic>
#include <cstdlib>
#include <fstream>
#include <string>
#include <tuple>

#ifdef OPENMVG_USE_OPENMP
#include <omp.h>
#endif

using namespace openMVG;
using namespace openMVG::image;
using namespace openMVG::features;
using namespace openMVG::sfm;
using namespace std;

// -------- HEADER_FILE: sfmNet_compute_features.hpp ----------------------------------------------------------------------------------------------------

features::EDESCRIBER_PRESET stringToEnum(const std::string& sPreset)
{
    features::EDESCRIBER_PRESET preset;
    if (sPreset == "NORMAL")
        preset = features::NORMAL_PRESET;
    else
        if (sPreset == "HIGH")
            preset = features::HIGH_PRESET;
        else
            if (sPreset == "ULTRA")
                preset = features::ULTRA_PRESET;
            else
                preset = features::EDESCRIBER_PRESET(-1);
    return preset;
}

// -------- CPP File: sfmNet_compute_features.cpp ----------------------------------------------------------------------------------------------------
string sfmNet_compute_features(std::tuple<std::string, double, double, std::string, double, double> intrinsics) {

    string img_data = get<0>(intrinsics);
    double width = get<1>(intrinsics);
    double height = get<2>(intrinsics);
    string cam_model = get<3>(intrinsics);
    double focal = get<4>(intrinsics);

    if (img_data.empty() || width <= 0 || height <= 0 || cam_model.empty() || focal <= 0)
        throw std::invalid_argument("Error: One of the following is not valid: img_data = {" + img_data + "}\t" + "width = {" + to_string(width) + "}\t" + "height = {" + to_string(height) + "}\t"
            + "cam_model = {" + cam_model + "}\t" + "focal = {" + to_string(focal) + "}\t" + "\n");
    //implement error checking for if the input string does not match the expected input

    bool bUpRight = false;
    std::string sImage_Describer_Method = "SIFT";
    bool bForce = false;
    std::string sFeaturePreset = "";

#ifdef OPENMVG_USE_OPENMP
    int iNumThreads = 0;
#endif

    //---------------------------------------
    // a. Load input scene - parse the string into the views and instrinsics
    //---------------------------------------



    // b. Init the image_describer
    // - retrieve the used one in case of pre-computed features
    // - else create the desired one

    using namespace openMVG::features;
    std::unique_ptr<Image_describer> image_describer;

    const std::string sImage_describer;   //sImage_desciber is going to passed in string to this function
    if (!bForce && sImage_describer.empty()) {
        // Dynamically load the image_describer from the file (will restore old used settings)
        std::ifstream stream(sImage_describer.c_str());
        if (!stream.is_open())
            throw runtime_error("The stream for the image describer is not open.");

        try
        {
            cereal::JSONInputArchive archive(stream);
            archive(cereal::make_nvp("image_describer", image_describer));
        }
        catch (const cereal::Exception& e)
        {
            std::cerr << e.what() << std::endl
                << "Cannot dynamically allocate the Image_describer interface." << std::endl;
            throw runtime_error("Cannot dynamically allocate the Image_describer interface.");
        }
    }
    else
    {
        // Create the desired Image_describer method.
        // Don't use a factory, perform direct allocation
        if (sImage_Describer_Method == "SIFT")
        {
            image_describer.reset(new SIFT_Image_describer
            (SIFT_Image_describer::Params(), !bUpRight));
        }
        else
            if (sImage_Describer_Method == "SIFT_ANATOMY")
            {
                image_describer.reset(
                    new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
            }
            else
                if (sImage_Describer_Method == "AKAZE_FLOAT")
                {
                    image_describer = AKAZE_Image_describer::create
                    (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), !bUpRight);
                }
                else
                    if (sImage_Describer_Method == "AKAZE_MLDB")
                    {
                        image_describer = AKAZE_Image_describer::create
                        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), !bUpRight);
                    }
        if (!image_describer)
        {
            std::cerr << "Cannot create the designed Image_describer:"
                << sImage_Describer_Method << "." << std::endl;
            throw runtime_error("Cannot create the designed Image_describer:");
        }
        else
        {
            if (!sFeaturePreset.empty())
                if (!image_describer->Set_configuration_preset(stringToEnum(sFeaturePreset)))
                {
                    std::cerr << "Preset configuration failed." << std::endl;
                    throw runtime_error("Preset configuration failed.");
                }
        }

        // Export the used Image_describer and region type for:
        // - dynamic future regions computation and/or loading
        {
            std::ofstream stream(sImage_describer.c_str());
            if (!stream.is_open())
                throw runtime_error("sImage_desciber stream is not valid.");

            cereal::JSONOutputArchive archive(stream);
            archive(cereal::make_nvp("image_describer", image_describer));
            auto regionsType = image_describer->Allocate();
            archive(cereal::make_nvp("regions_type", regionsType));
        }
    }







}

