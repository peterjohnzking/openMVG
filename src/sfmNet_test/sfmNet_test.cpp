#include <iostream>
#include <tuple>
#include <string>

#include "openMVG/sfmNet/sfmNet.hpp"




using namespace openMVG::sfmNet;

int main() {
	std::string path = "D:\\Users\\peter\\Source\\Repos\\openMVG\\src\\openMVG\\exif\\sensor_width_database\\sensor_width_camera_database.txt";
	sfmNet_load_intrinsics(50, 24, "AgfaPhoto Compact 100", 3, path);
	//std::string image, model;
	//double width, height, focus; 

	//std::string image_path;
	//std::cout << "Enter image path: ";
	//std::cin >> image_path;

	//std::tuple<std::string, double, double, std::string, double> val = sfmNet_load_images(image_path, false);
	//image = std::get<0>(val);
	//width = std::get<1>(val);
	//height = std::get<2>(val);
	//model = std::get<3>(val);
	//focus = std::get<4>(val);

	//std::cout << image << std::endl;

	//std::cout << image;

	return 0;


}