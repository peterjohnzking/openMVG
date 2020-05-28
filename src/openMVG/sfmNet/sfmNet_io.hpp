#ifndef OPENMVG_SFMNET_SFMNET_IO_HPP
#define OPENMVG_SFMNET_SFMNET_IO_HPP

#include "openMVG/image/image_io.hpp"

#include <Eigen/Core>

#include <iostream>

#include <vector>
#include <string>


using namespace Eigen;

// -------- HEADER_FILE: sfmNet_io.hpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace image {

std::ostream& operator<<(std::ostream& out, const Image<unsigned char>& image);
std::istream& operator>>(std::istream& in, Image<unsigned char>& image);

} // namespace image
} // namespace openMVG


// -------- CPP_FILE: sfmNet_io.cpp ----------------------------------------------------------------------------------------------------

namespace openMVG {
namespace image {

std::ostream& operator<<(std::ostream& out, const Image<unsigned char>& image)
{
  double width = image.Width();
  double height = image.Height();

  //std::cout << "width: " << image.Width() << " height: " << image.Height() << " row: " << image.rows() << " col: " << image.cols() << " entry: " << image.Width() * image.Height() << std::endl; 
  out << width << ' ' << height << ' ';
  const unsigned char* data = image.data();
  double data_length = width * height;
  for (int i = 0; i < data_length; ++i) out << (int) *(data + i) << ' ';

  return out;
}

std::istream& operator>>(std::istream& in, Image<unsigned char>& image) {

 
  double width, height;
  in >> width >> height;

  unsigned int entry;
  std::vector<unsigned char> data;
  while (in >> entry) data.push_back((unsigned char) entry);

  image = Eigen::Map<Image<unsigned char>::Base>(&data[0], height, width);
  

  return in;
}


} // namespace image
} // namespace openMVG
#endif // OPENMVG_SFMNET_SFMNET_IO_HPP
