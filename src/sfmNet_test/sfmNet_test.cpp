#include <iostream>
#include <tuple>
#include <string>

#include "openMVG/sfmNet/sfmNet.hpp"
#include "openMVG/sfmNet/sfmNet_compute_features.hpp"




using namespace openMVG::sfmNet;

int main() {
	//STEP 1 -------------------------------------------------------------------------------------
	std::string path = "C:\\Users\\Andre\\Source\\Repos\\openMVG_peter\\src\\openMVG\\exif\\sensor_width_database\\sensor_width_camera_database.txt";
	sfmNet_load_intrinsics(50, 24, "AgfaPhoto Compact 100", 3, path);

	std::string image, model;
	double width, height, focus; 

	std::string image_path;
	std::cout << "Enter image path: ";
	std::cin >> image_path;

	std::tuple<std::string, double, double, std::string, double, double> val = sfmNet_load_images(image_path, false);
	image = std::get<0>(val);
	width = std::get<1>(val);
	height = std::get<2>(val);
	model = std::get<3>(val);
	focus = std::get<4>(val);

	

	std::cout << image << std::endl;

	std::cout << image;
	//STEP 2 ------------------------------------------------------------------------------------

	return 0;


}