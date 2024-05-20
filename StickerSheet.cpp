#include "StickerSheet.h"
#include "Image.h"

StickerSheet::StickerSheet(const Image & picture, unsigned max) {
    baseImage = picture;
    maxStickers = max;

    images = new Image*[maxStickers];
    xCoor = new unsigned[maxStickers];
    yCoor = new unsigned[maxStickers];

    for (unsigned i = 0; i < max; i++) 
        images[i] = NULL;
}

StickerSheet::~StickerSheet() {
    Remove();
}

void StickerSheet::Remove() {
    delete[] xCoor;
    delete[] yCoor;
    delete[] images;

    xCoor = NULL;
    yCoor = NULL;
    images = NULL;
}

void StickerSheet::Copy(const StickerSheet &other) {
    baseImage = other.baseImage;
    maxStickers = other.maxStickers;

    images = new Image*[maxStickers];
    xCoor = new unsigned[maxStickers];
    yCoor = new unsigned[maxStickers];

    for (unsigned i = 0; i < maxStickers; i++) {
        if(other.images[i] != NULL) 
            images[i] = other.images[i];
        else 
            images[i] = NULL;

        xCoor[i] = other.xCoor[i];
        yCoor[i] = other.yCoor[i];
    }
}

StickerSheet::StickerSheet(const StickerSheet & other) {
    Copy(other);
}

const StickerSheet & StickerSheet::operator=(const StickerSheet & other) {
    Remove();
    Copy(other);

    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
    if (max == maxStickers) {
        return;
    } 
    else {
        Image **tempImages = new Image*[max];
        unsigned *tempX = new unsigned[max];
        unsigned *tempY = new unsigned[max];

        for (unsigned i = 0; i < max; i++) {
            if(i < maxStickers) {
                tempImages[i] = images[i];
                tempX[i] = xCoor[i];
                tempY[i] = yCoor[i];
            }
        }

        if (max > maxStickers)
            for (unsigned i = maxStickers; i < max; i++) 
                tempImages[i] = NULL;

        Remove();

        images = tempImages;
        xCoor = tempX;
        yCoor = tempY;

        maxStickers = max;
    }
}

int StickerSheet::addSticker(Image & sticker, unsigned x, unsigned y) {
    for (unsigned i = 0; i < maxStickers; i++) 
        if (images[i] == NULL) {
            images[i] = &sticker;
            xCoor[i] = x;
            yCoor[i] = y;
            return i;
        }

  return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if (images[index] == NULL || index >= maxStickers) {
        return false;
    } 
    else {
        xCoor[index] = x;
        yCoor[index] = y;

        return true;
    }
}

void StickerSheet::removeSticker(unsigned index) {
    if (index < maxStickers && images[index] != NULL) {
        images[index] = NULL;
        delete images[index];
    }
}

Image * StickerSheet::getSticker(unsigned index) const {
    if (images[index] == NULL || index > maxStickers) 
        return NULL;
        
    return images[index];
}

Image StickerSheet::render() const {

    Image stickerSheet(baseImage);
    unsigned imageWidth = 0;
    unsigned imageHeight = 0;
    unsigned baseWidth = stickerSheet.width();
    unsigned baseHeight = stickerSheet.height();
    
    for(unsigned i = 0; i < maxStickers; i++)
        if(images[i] != NULL) {
            imageWidth = images[i]->width();
            imageHeight = images[i]->height();
            baseWidth = stickerSheet.width();
            baseHeight = stickerSheet.height();

            if(imageWidth + xCoor[i] > baseWidth) 
                stickerSheet.resize(imageWidth + xCoor[i], baseHeight);
            if(imageHeight + yCoor[i] > baseHeight)
                stickerSheet.resize(baseWidth, imageHeight + yCoor[i]);

            //draw the pixels of the images over the coresponding pixels of the base image
            for(unsigned x = 0; x < imageWidth; x++) 
                for(unsigned y = 0; y < imageHeight; y++) {
                    HSLAPixel & stickerSheetPixel = stickerSheet.getPixel(x + xCoor[i], y + yCoor[i]);
                    HSLAPixel & imagePixel = images[i]->getPixel(x, y);
                    if(imagePixel.a == 0)
                        continue;
                    stickerSheetPixel = imagePixel;
                }
        }

    return stickerSheet;
}