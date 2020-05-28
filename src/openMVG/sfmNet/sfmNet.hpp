#ifndef OPENMVG_SFMNET_SFMNET_HPP
#define OPENMVG_SFMNET_SFMNET_HPP

#include "sfmNet_io.hpp"

#include "openMVG/image/image_io.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/exif/exif_IO.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"
#include <Eigen/Dense>

#include <iostream>
#include <tuple>
#include <stdexcept>
#include <sstream>
#include <string>



using namespace openMVG;
using namespace openMVG::image;
using namespace openMVG::exif;
using namespace std;




// -------- HEADER_FILE: sfmNet.hpp ----------------------------------------------------------------------------------------------------

//--------------------------------------
// SfM Network Pipeline (cpp functions)
//--------------------------------------

namespace openMVG {
namespace sfmNet {

std::tuple<std::string, double, double, std::string, double>
sfmNet_load_images(const std::string & image_path, bool mask = false);

} // namespace sfmNet
} // namespace openMVG


// -------- CPP_FILE: sfmNet_load_images.cpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace sfmNet {

std::tuple<std::string, double, double, std::string, double> 
sfmNet_load_images(const std::string & image_path, bool mask)
{
  

  double width = -1.0, height = -1.0, focal = -1.0;
  std::string sCamModel = "";
  
 

  // Test if the image format is supported:
  if (openMVG::image::GetFormat(image_path.c_str()) == openMVG::image::Unknown)
    throw std::invalid_argument(image_path + ": Unkown image file format."); 
    
  system::Timer timer;
  Image<unsigned char> image;

  try {
    if (!ReadImage(image_path.c_str(), &image))
      throw std::invalid_argument("\"" + image_path + "\" is not a valid path in system directory.");
  }
  catch (...) {
    throw std::invalid_argument("\"" + image_path + "\" is not a valid path in system directory.");
  }

  std::cout << "Time for read image: " << timer.elapsedMs() << std::endl;
  
  width = image.Width();
  height = image.Height();
  
  timer.reset();
  // load focal length and camera model from exif meta data
  std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
  exifReader->open(image_path);
  
  const bool bHaveValidExifMetadata =
    exifReader->doesHaveExifInfo()
    && !exifReader->getModel().empty();
  
  if (bHaveValidExifMetadata)
  {
    const std::string sCamModel = exifReader->getModel();
  
    // throw error if focal length is equal to 0
    focal = exifReader->getFocal();
    if (focal == 0.0f)
    {
      throw std::invalid_argument(stlplus::basename_part(image_path) + ": Focal length is missing.");
    }
  }
  std::cout << "Time for read metadata: " << timer.elapsedMs() << std::endl;

  timer.reset();
  
  // outputs image pixels to string 
  std::ostringstream img_stream;
  
  img_stream << image;
  
  std::string img_data = img_stream.str();
  std::cout << "Time for output image (outstream) : " << timer.elapsedMs() << std::endl;
  
  return std::tuple<std::string, double, double, std::string, double>(img_data, width, height, sCamModel, focal);
    
}


} // namespace sfmNet
} // namespace openMVG

#endif // OPENMVG_SFMNET_SFMNET_HPP

