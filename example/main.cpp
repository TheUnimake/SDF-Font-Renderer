#include <iostream>
#include <string>
#include "SDFRenderer.h"
#include "SDFFontRenderer.h"

#define __WIN32
#include <png.hpp>

int main(int argc, char* argv[]) {

	unsigned int width  = 1024;
	unsigned int height = 1024;
	const char*  fontFilename = "C:/Users/Timon/Dropbox/Fonts/Bebas Neue/BebasNeue Regular.otf";

	SDFFontRenderer fontRenderer;
	fontRenderer.SetWidth(width);
	fontRenderer.SetHeight(height);
	fontRenderer.SetFalloff(10);
	fontRenderer.LoadFont(48, fontFilename);
	fontRenderer.CreateSDF();

	// write png 
	png::image<png::rgb_pixel> image(width, height);
	for (size_t y = 0; y < image.get_height(); ++y) {
		for (size_t x = 0; x < image.get_width(); ++x) {
			float dist = (fontRenderer.GetSDF()[y * width + x] + 1.0f) / 2.f;
			png::byte value = static_cast<png::byte>(dist * 255.f);

			image[y][x] = png::rgb_pixel(value, value, value);
		}
	}
	image.write("sdf.png");

	for (size_t y = 0; y < image.get_height(); ++y) {
		for (size_t x = 0; x < image.get_width(); ++x) {
			bool value = fontRenderer.GetTexture()[y * width + x];

			if (value) {
				image[y][x] = png::rgb_pixel(0, 0, 0);
			} else {
				image[y][x] = png::rgb_pixel(255, 255, 255);
			}
		}
	}
	image.write("font.png");

	std::cout << "ended" << std::endl;
	std::cin.get();

	return 0;
}
