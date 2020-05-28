#include "sfmNet.hpp"

#include "openMVG/image/image_io.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/exif/exif_IO.hpp"


#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <tuple>
#include <stdexcept>
#include <sstream>

using namespace openMVG::image;
using namespace openMVG::exif;
using namespace std;

namespace openMVG {
namespace sfmNet {



//std::tuple<std::string, double, double, std::string, double> sfmNet_load_images(const std::string & image_path, bool mask)
//{
//
//  double width = -1.0, height = -1.0, focal = -1.0;
//  std::string sCamModel = "";
//
//  // Test if the image format is supported:
//  if (openMVG::image::GetFormat(image_path.c_str()) == openMVG::image::Unknown)
//    throw std::invalid_argument(image_path + ": Unkown image file format."); 
//  
//
//  Image<unsigned char> image;
//  if (!ReadImage(image_path.c_str(), &image))
//    throw std::invalid_argument("\"" + image_path + "\" is not a valid path in system directory.");
//
//  width = image.Width();
//  height = image.Height();
//  
//  // load focal length and camera model from exif meta data
//  std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
//  exifReader->open(image_path);
//
//  const bool bHaveValidExifMetadata =
//    exifReader->doesHaveExifInfo()
//    && !exifReader->getModel().empty();
//
//  if (bHaveValidExifMetadata)
//  {
//    const std::string sCamModel = exifReader->getModel();
//
//    // throw error if focal length is equal to 0
//    focal = exifReader->getFocal();
//    if (focal == 0.0f)
//    {
//      throw std::invalid_argument(stlplus::basename_part(image_path) + ": Focal length is missing.");
//    }
//  }
//
//  std::ostringstream img_stream;
//
//  img_stream << image;
//
//  std::string img_data = img_stream.str();
//
//
//  return std::tuple<std::string, double, double, std::string, double>(img_data, width, height, sCamModel, focal);
//}

} // namespace sfmNet
} // namespace openMVG