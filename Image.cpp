#include <cmath>
#include "Image.h"
#include "cs225/PNG.h"

using namespace cs225;
using namespace std;

void Image::lighten() {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & lightenPixel = getPixel(x, y);
            lightenPixel.l += 0.1;
            if(lightenPixel.l > 1.0) lightenPixel.l = 1.0;
        }
}

void Image::lighten(double amount) {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & lightenPixel = getPixel(x, y);
            lightenPixel.l += amount;
            if(lightenPixel.l > 1.0) lightenPixel.l = 1.0;
        }
}

void Image::darken() {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & darkenPixel = getPixel(x, y);
            darkenPixel.l -= 0.1;
            if(darkenPixel.l < 0.0) darkenPixel.l = 0.0;
        }
}

void Image::darken(double amount) {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & darkenPixel = getPixel(x, y);
            darkenPixel.l -= amount;
            if(darkenPixel.l < 0.0) darkenPixel.l = 0.0;
        }
}

void Image::saturate() {
    for(unsigned int x = 0; x < width(); x++) 
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & saturatePixel = getPixel(x, y);
            saturatePixel.s += 0.1;
            if(saturatePixel.s > 1.0) saturatePixel.s = 1.0;
        }
}

void Image::saturate(double amount) {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & saturatePixel = getPixel(x, y);
            saturatePixel.s += amount;
            if(saturatePixel.s > 1.0) saturatePixel.s = 1.0;
        }
}

void Image::desaturate() {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & desaturatePixel = getPixel(x, y);
            desaturatePixel.s -= 0.1;
            if(desaturatePixel.s < 0.0) desaturatePixel.s = 0.0;
        }
}

void Image::desaturate(double amount) {
    for(unsigned int x = 0; x < width(); x++) 
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & desaturatePixel = getPixel(x, y);
            desaturatePixel.s -= amount;
            if(desaturatePixel.s < 0.0) desaturatePixel.s = 0.0;
        }
}

void Image::grayscale() {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & grayPixel = getPixel(x, y);
            grayPixel.s = 0.0;
        }
}

void Image::rotateColor(double degrees) {
    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & rotatePixel = getPixel(x, y);
            if(degrees < 0)
                rotatePixel.h = fmod((rotatePixel.h + (360.0 - degrees)), 360.0);
            else
                rotatePixel.h = fmod((rotatePixel.h + degrees), 360.0);
        }
}

void Image::illinify() {
    int blue = 216;
    int orange = 11;

    for(unsigned int x = 0; x < width(); x++)
        for(unsigned int y = 0; y < height(); y++) {
            HSLAPixel & illinifyPixel= getPixel(x, y);
            illinifyPixel.h = blue;
            if(abs(illinifyPixel.h - orange) >= abs(illinifyPixel.h - blue))
                illinifyPixel.h = orange;
        }
}

void Image::scale(double factor) {
    unsigned int newWidth = width() * factor;
    unsigned int newHeight = height() * factor;

    Image* temp = new Image();
    *temp = *this;
    resize(newWidth, newHeight);

    for(unsigned int x = 0; x < newWidth; x++)
        for(unsigned int y = 0; y < newHeight; y++) {
            HSLAPixel & scalePixel = getPixel(x, y);
            HSLAPixel & newScalePixel = temp->getPixel(x / factor, y / factor);
            scalePixel = newScalePixel;
        }

    delete temp;
}

void Image::scale(unsigned w, unsigned h) {
    double widthFactor = (double)w / (double)width();
    double heightFactor = (double)h / (double)height();

    scale(std::min(widthFactor, heightFactor));
}