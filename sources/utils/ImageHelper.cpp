#include "utils/ImageHelper.hpp"
#include <fstream>
#include <iostream>

Raytracer::Utils::ImageHelper::ImageHelper(const char *filename)
{
    load(filename);
}

bool Raytracer::Utils::ImageHelper::load(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open image file '" << filename
                  << "'.\n";
        return false;
    }

    std::string magic;
    int width, height, maxval;
    file >> magic >> width >> height >> maxval;

    if (magic != "P6") {
        std::cerr << "ERROR: Unsupported image format. Only PPM images "
                     "(P6) are supported.\n";
        return false;
    }

    if (maxval != 255) {
        std::cerr << "ERROR: Unsupported maximum color value. Only 8-bit "
                     "PPM images are supported.\n";
        return false;
    }

    _width = width;
    _height = height;

    data.resize(width * height * 3);
    file.ignore();
    file.read(reinterpret_cast<char *>(&data[0]), data.size());

    return true;
}

const unsigned char *Raytracer::Utils::ImageHelper::pixelData(
    int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        static unsigned char magenta[] = {255, 0, 255};
        return magenta;
    }

    return &data[(y * _width + x) * 3];
}
