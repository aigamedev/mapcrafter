/*
 * Copyright 2012-2015 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h> // to be sure M_PI is defined

#include <png.h>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace mapcrafter {
namespace renderer {

typedef uint32_t RGBAPixel;

RGBAPixel rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
uint8_t rgba_red(RGBAPixel value);
uint8_t rgba_green(RGBAPixel value);
uint8_t rgba_blue(RGBAPixel value);
uint8_t rgba_alpha(RGBAPixel value);

RGBAPixel rgba_add_clamp(RGBAPixel value, int r, int g, int b, int a = 0);
RGBAPixel rgba_add_clamp(RGBAPixel value, const std::tuple<int, int, int>& values);
RGBAPixel rgba_multiply(RGBAPixel value, double r, double g, double b, double a = 1);
RGBAPixel rgba_multiply(RGBAPixel value, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

int rgba_distance2(RGBAPixel value1, RGBAPixel value2);

void blend(RGBAPixel& dest, const RGBAPixel& source);

void pngReadData(png_structp pngPtr, png_bytep data, png_size_t length);
void pngWriteData(png_structp pngPtr, png_bytep data, png_size_t length);

template <typename Pixel>
class Image {
public:
	Image(int width = 0, int height = 0);
	~Image();

	int getWidth() const;
	int getHeight() const;

	Pixel getPixel(int x, int y) const;
	void setPixel(int x, int y, Pixel pixel);

	const Pixel& pixel(int x, int y) const;
	Pixel& pixel(int x, int y);

	void setSize(int width, int height);

protected:
	int width;
	int height;

	std::vector<Pixel> data;
};

const int ROTATE_90 = 1;
const int ROTATE_180 = 2;
const int ROTATE_270 = 3;

enum class InterpolationType {
	// nearest-neighbor interpolation, simple one
	NEAREST,
	// bilinear interpolation, fancy one
	BILINEAR,
	// special interpolation tweaked for resizing to (width/2, height/2)
	HALF,
	// automatically choose an interpolation type
	AUTO
};

// TODO better documentation...
class RGBAImage : public Image<RGBAPixel> {
public:
	RGBAImage(int width = 0, int height = 0);
	~RGBAImage();

	/**
	 * Blits one image to another one. Just copies the pixels over without any processing.
	 */
	void simpleBlit(const RGBAImage& image, int x, int y);

	/**
	 * Blits one image to another one. Just copies the pixels over, but skips completely
	 * transparent pixels (alpha(pixel) == 0).
	 */
	void simpleAlphaBlit(const RGBAImage& image, int x, int y);

	/**
	 * Blits one image to another one. Also Alphablends transparent pixels of the source
	 * image with the pixels of the destination image.
	 */
	void alphaBlit(const RGBAImage& image, int x, int y);
	void blendPixel(RGBAPixel color, int x, int y);

	void fill(RGBAPixel color, int x1, int y1, int w, int h);
	void clear();

	RGBAImage clip(int x, int y, int width, int height) const;
	RGBAImage colorize(double r, double g, double b, double a = 1) const;
	RGBAImage colorize(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;
	RGBAImage rotate(int rotation) const;
	RGBAImage flip(bool flip_x, bool flip_y) const;
	RGBAImage move(int x_off, int y_off) const;
	
	void resize(RGBAImage& dest, int width, int height,
			InterpolationType interpolation = InterpolationType::AUTO) const;

	RGBAImage resize(int width, int height,
			InterpolationType interpolation = InterpolationType::AUTO) const;

	/**
	 * (In-place) Shearing along the x-axis by a specific factor.
	 */
	RGBAImage& shearX(double factor);

	/**
	 * (In-place) Shearing along the y-axis by a specific factor.
	 */
	RGBAImage& shearY(double factor);

	/**
	 * (In-place) Rotation by shearing:
	 * https://www.ocf.berkeley.edu/~fricke/projects/israel/paeth/rotation_by_shearing.html
	 */
	RGBAImage& rotateByShear(double degrees);

	/**
	 * Applies a simple blur filter to the image. Uses the specified radius for the
	 * (quadratic) blur effect.
	 */
	void blur(RGBAImage& dest, int radius) const;

	bool readPNG(const std::string& filename);
	bool writePNG(const std::string& filename) const;
	bool writeIndexedPNG(const std::string& filename, int palette_bits = 8, bool dithered = true) const;

	bool readJPEG(const std::string& filename);
	bool writeJPEG(const std::string& filename, int quality,
			RGBAPixel background = rgba(255, 255, 255, 255)) const;
};

template <typename Pixel>
Image<Pixel>::Image(int width, int height)
	:width(width), height(height) {
	data.resize(width * height);
}

template <typename Pixel>
Image<Pixel>::~Image() {
}

template <typename Pixel>
int Image<Pixel>::getWidth() const {
	return width;
}

template <typename Pixel>
int Image<Pixel>::getHeight() const {
	return height;
}

template <typename Pixel>
Pixel Image<Pixel>::getPixel(int x, int y) const {
	if (x >= width || x < 0 || y >= height || y < 0)
		return 0;
	return data[y * width + x];
}

template <typename Pixel>
void Image<Pixel>::setPixel(int x, int y, Pixel pixel) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return;
	data[y * width + x] = pixel;
}

template <typename Pixel>
const Pixel& Image<Pixel>::pixel(int x, int y) const {
	return data[y * width + x];
}

template <typename Pixel>
Pixel& Image<Pixel>::pixel(int x, int y) {
	return data[y * width + x];
}

template <typename Pixel>
void Image<Pixel>::setSize(int width, int height) {
	this->width = width;
	this->height = height;
	data.resize(width * height);
}

}
}

#endif /* IMAGE_H_ */
