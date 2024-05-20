/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>  

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> points;
    map<Point<3>, TileImage*> tiles;

    for(unsigned i = 0; i < theTiles.size(); i++) {
        Point<3> point = convertToXYZ(theTiles[i].getAverageColor());
        points.push_back(point);
        tiles[point] = &theTiles[i];
    }

    KDTree<3> tree(points);

    for(int row = 0; row < theSource.getRows(); row++)
        for(int col = 0; col < theSource.getColumns(); col++) {
            Point<3> point = convertToXYZ(theSource.getRegionColor(row, col));
            Point<3> nearest = tree.findNearestNeighbor(point);
            TileImage* tile = tiles[nearest];
            canvas->setTile(row, col, tile);
        }

    return canvas;
}

