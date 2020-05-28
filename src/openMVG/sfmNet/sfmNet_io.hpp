#ifndef OPENMVG_SFMNET_SFMNET_IO_HPP
#define OPENMVG_SFMNET_SFMNET_IO_HPP

#include "openMVG/image/image_io.hpp"
#include "openMVG/sfm/sfm_data_io_cereal.hpp"

#include "openMVG/cameras/cameras_io.hpp"
#include "openMVG/geometry/pose3_io.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_landmark_io.hpp"
#include "openMVG/sfm/sfm_view_io.hpp"
#include "openMVG/sfm/sfm_view_priors_io.hpp"
#include "openMVG/types.hpp"
#include "openMVG/cameras/Camera_Intrinsics.hpp"

#include <Eigen/Core>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

#include <iostream>

#include <vector>
#include <string>


using namespace Eigen;

// -------- HEADER_FILE: sfmNet_io.hpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace image {

std::ostream & operator<<(std::ostream & out, const Image<unsigned char> & image);
std::istream & operator>>(std::istream & in, Image<unsigned char> & image);

} // namespace image

namespace sfm
{

std::ostream & operator<<(std::ostream & out, const SfM_Data & sfm_data);
std::istream & operator>>(std::istream & in, SfM_Data & sfm_data);

}

namespace cameras
{

std::ostream & operator<<(std::ostream & out, const IntrinsicBase & intrinsic);
std::istream & operator>>(std::istream & in, IntrinsicBase & intrinsic);

} // namespace cameras

} // namespace openMVG


// -------- CPP_FILE: sfmNet_io.cpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace image {

std::ostream & operator<<(std::ostream & out, const Image<unsigned char> & image)
{
  int32_t width = image.Width();
  int32_t height = image.Height();

  cereal::BinaryOutputArchive archive(out);

  archive(width);
  archive(height);

  archive(cereal::binary_data(image.data(), sizeof(unsigned char) * width * height));

  return out;
}

std::istream & operator>>(std::istream & in, Image<unsigned char> & image) 
{
  cereal::BinaryInputArchive archive(in);
  
  int32_t width;
  int32_t height;

  archive(width);
  archive(height);

  image.resize(width, height);

  archive(cereal::binary_data(image.data(), sizeof(unsigned char) * width * height));

  return in;
}


} // namespace image

namespace sfm
{

std::ostream & operator<<(std::ostream & out, const SfM_Data & data)
{

  cereal::BinaryOutputArchive archive(out);
  
  const std::string version = "0.3";
  archive(cereal::make_nvp("sfm_data_version", version));

  archive(cereal::make_nvp("views", data.views));
  archive(cereal::make_nvp("intrinsics", data.intrinsics));
  archive(cereal::make_nvp("extrinsics", data.poses));
  archive(cereal::make_nvp("structure", data.structure));
  archive(cereal::make_nvp("control_points", data.control_points));

  return out;
}

std::istream & operator>>(std::istream& in, SfM_Data & data)
{
  cereal::BinaryInputArchive archive(in);

  std::string version; 
  archive(cereal::make_nvp("sfm_data_version", version));
  archive(cereal::make_nvp("views", data.views));
  archive(cereal::make_nvp("intrinsics", data.intrinsics));
  archive(cereal::make_nvp("extrinsics", data.poses));
  archive(cereal::make_nvp("structure", data.structure));
  archive(cereal::make_nvp("control_points", data.control_points));

  return in;
}

} // namespace sfm

namespace cameras
{

std::ostream & operator<<(std::ostream & out, const IntrinsicBase & intrinsic)
{
  cereal::BinaryOutputArchive archive(out);
  archive(intrinsic);

  return out;
}

std::istream& operator>>(std::istream& in, IntrinsicBase& intrinsic)
{
  cereal::BinaryInputArchive archive(in);
  archive(intrinsic);

  return in;
}

} // namespace cameras

} // namespace openMVG


#endif // OPENMVG_SFMNET_SFMNET_IO_HPP
