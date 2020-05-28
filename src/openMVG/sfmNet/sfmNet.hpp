#ifndef OPENMVG_SFMNET_SFMNET_HPP
#define OPENMVG_SFMNET_SFMNET_HPP

#include "sfmNet_io.hpp"

#include "openMVG/image/image_io.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/exif/exif_IO.hpp"
#include "openMVG/exif/sensor_width_database/ParseDatabase.hpp"
#include "openMVG/system/timer.hpp"
#include "openMVG/cameras/cameras.hpp"
#include "openMVG/cameras/Camera_Common.hpp"
#include "openMVG/stl/split.hpp"

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
using namespace cameras;
using namespace std;





// -------- HEADER_FILE: sfmNet.hpp ----------------------------------------------------------------------------------------------------

//--------------------------------------
// SfM Network Pipeline (cpp functions)
//--------------------------------------

namespace openMVG {
namespace sfmNet {

std::tuple<std::string, double, double, std::string, double, double>
sfmNet_load_images(const std::string & image_path, bool mask = false);
std::tuple<std::string, double>
sfmNet_load_intrinsics
(double width, double height, std::string sCameraModel, double cam_focal, std::string sfileDatabase, int i_User_camera_model = PINHOLE_CAMERA_RADIAL3, std::string sKmatrix = "");

} // namespace sfmNet
} // namespace openMVG


// -------- CPP_FILE: sfmNet_load_images.cpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace sfmNet {

void print(Image<unsigned char> image)
{
  for (int i = 0; i < image.Width() * image.Height(); ++i) {
    std::cout << image[i] << " ";
  }
  std::cout << std::endl;
}
void test()
{
  int width = 5;
  int height = 4;
  std::vector<unsigned char> data(width*height);
  std::generate(data.begin(), data.end(), std::rand);

  Image<unsigned char> image;
  image = Eigen::Map<Image<unsigned char>::Base>(&data[0], height, width);
  
  std::string sep = "\n--------------------------------------------------------------------\n";
  
  system::Timer timer;
  
  print(image);
  timer.reset();

  // outputs image pixels to string 
  std::ostringstream img_stream;

  img_stream << image;

  std::string img_data = img_stream.str();
  std::cout << "Time for output image (outstream) : " << timer.elapsedMs() << std::endl;
  std::cout << sep;

  Image<unsigned char> image2;
  

  
  timer.reset();

  // outputs image pixels to string 
  std::istringstream img_stream2(img_data);

  img_stream2 >> image2;

  double time = timer.elapsedMs();
  print(image2);

  std::cout << "Time for output image (outstream) : " <<  time << std::endl;
  std::cout << sep;


}

std::tuple<std::string, double, double, std::string, double, double> 
sfmNet_load_images(const std::string & image_path, bool mask)
{
  
  system::Timer timer;

  double width = -1.0, height = -1.0, focal = -1.0;
  std::string sCamModel = "";
  
 

  // Test if the image format is supported:
  if (openMVG::image::GetFormat(image_path.c_str()) == openMVG::image::Unknown)
    throw std::invalid_argument(image_path + ": Unkown image file format."); 
    
  
  Image<unsigned char> image;

  try {
    if (!ReadImage(image_path.c_str(), &image))
      throw std::invalid_argument("\"" + image_path + "\" is not a valid path in system directory.");
  }
  catch (...) {
    throw std::invalid_argument("\"" + image_path + "\" is not a valid path in system directory.");
  }

  
  width = image.Width();
  height = image.Height();
  
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
  
  // outputs image pixels to string 
  std::ostringstream img_stream;
  
  img_stream << image;
  
  std::string img_data = img_stream.str();
  
  
  return std::tuple<std::string, double, double, std::string, double, double>(img_data, width, height, sCamModel, focal, timer.elapsedMs());
}




} // namespace sfmNet
} // namespace openMVG


// -------- CPP_FILE: sfmNet_load_sfm_data.cpp ----------------------------------------------------------------------------------------------------

bool checkIntrinsicStringValidity(const std::string& Kmatrix, double& focal, double& ppx, double& ppy); 

namespace openMVG {
namespace sfmNet {

std::tuple<std::string, double>
sfmNet_load_intrinsics
(double width, double height, std::string sCameraModel, double cam_focal, std::string sfileDatabase, int i_User_camera_model, std::string sKmatrix)
{

  system::Timer timer;

  double ppx = -1.0, ppy = -1.0, focal = -1.0;

  const EINTRINSIC e_User_camera_model = EINTRINSIC(i_User_camera_model);

  if (!sKmatrix.empty() && !checkIntrinsicStringValidity(sKmatrix, focal, ppx, ppy))
  {
    std::cerr << "\nInvalid K matrix input" << std::endl;
  }


  std::vector<Datasheet> vec_database;
  if (!sfileDatabase.empty())
  {
    if (!parseDatabase(sfileDatabase, vec_database))
    {
      throw std::invalid_argument(sfileDatabase + ": Invalid file database. Please specify a valid file");
    }
  }

  ppx = width / 2;
  ppy = height / 2;

  if (!sKmatrix.empty()) // Known user calibration K matrix
  {
    if (!checkIntrinsicStringValidity(sKmatrix, focal, ppx, ppy))
      focal = -1.0;
  }
  
  if (focal == -1.0)
  {
    Datasheet datasheet;
    if (getInfo(sCameraModel, vec_database, datasheet))
    {
      // The camera model was found in the database so we can compute it's approximated focal length
      const double ccdw = datasheet.sensorSize_;
      focal = std::max(width, height) * cam_focal / ccdw;
    }
    else
    {
      throw std::runtime_error("\" model \"" + sCameraModel + "\" doesn't exist in the database" + "\n"
        + "Please consider add your camera model and sensor width in the database." + "\n");
    }
  }

  std::shared_ptr<IntrinsicBase> intrinsic;

  if (focal > 0 && ppx > 0 && ppy > 0 && width > 0 && height > 0)
  {
    // Create the desired camera type
    switch (e_User_camera_model)
    {
    case PINHOLE_CAMERA:
      intrinsic = std::make_shared<Pinhole_Intrinsic>
        (width, height, focal, ppx, ppy);
      break;
    case PINHOLE_CAMERA_RADIAL1:
      intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K1>
        (width, height, focal, ppx, ppy, 0.0); // setup no distortion as initial guess
      break;
    case PINHOLE_CAMERA_RADIAL3:
      intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K3>
        (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0);  // setup no distortion as initial guess
      break;
    case PINHOLE_CAMERA_BROWN:
      intrinsic = std::make_shared<Pinhole_Intrinsic_Brown_T2>
        (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0, 0.0); // setup no distortion as initial guess
      break;
    case PINHOLE_CAMERA_FISHEYE:
      intrinsic = std::make_shared<Pinhole_Intrinsic_Fisheye>
        (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0); // setup no distortion as initial guess
      break;
    case CAMERA_SPHERICAL:
      intrinsic = std::make_shared<Intrinsic_Spherical>
        (width, height);
      break;
    default:
      throw std::invalid_argument("Error: unknown camera model: " + std::to_string((int)e_User_camera_model) + "\n");
    }
  }

  // outputs image pixels to string 
  std::ostringstream int_stream;

  int_stream << intrinsic;

  std::string int_data = int_stream.str();

  return std::tuple<std::string, double>(int_data, timer.elapsedMs());

}

} // namespace sfmNet
} // namespace openMVG

// Check that Kmatrix is a string like "f;0;ppx;0;f;ppy;0;0;1"
// With f,ppx,ppy as valid numerical value
bool checkIntrinsicStringValidity(const std::string& Kmatrix, double& focal, double& ppx, double& ppy)
{
  std::vector<std::string> vec_str;
  stl::split(Kmatrix, ';', vec_str);
  if (vec_str.size() != 9) {
    std::cerr << "\n Missing ';' character" << std::endl;
    return false;
  }
  // Check that all K matrix value are valid numbers
  for (size_t i = 0; i < vec_str.size(); ++i) {
    double readvalue = 0.0;
    std::stringstream ss;
    ss.str(vec_str[i]);
    if (!(ss >> readvalue)) {
      std::cerr << "\n Used an invalid not a number character" << std::endl;
      return false;
    }
    if (i == 0) focal = readvalue;
    if (i == 2) ppx = readvalue;
    if (i == 5) ppy = readvalue;
  }
  return true;
}

#endif // OPENMVG_SFMNET_SFMNET_HPP

