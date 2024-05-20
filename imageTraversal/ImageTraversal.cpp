#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  traversal = NULL;
}

ImageTraversal::Iterator::Iterator(PNG png_, Point point, double tolerance_, ImageTraversal* traversal_) {
  png = png_;
  origPoint = point;
  newPoint = point;
  tolerance = tolerance_;
  traversal = traversal_;

  valid.resize(png.width());
  for(unsigned i = 0; i < valid.size(); i++) 
    valid[i].resize(png.height());

  for(unsigned width = 0; width < png.width(); width++)
    for(unsigned height = 0; height < png.height(); height++)
      valid[width][height] = true;
}

bool ImageTraversal::Iterator::isValid(Point point) {
  if(point.x >= png.width() || point.y >= png.height())
    return false;
  if(0 > point.x || 0 > point.y)
    return false;
  
  HSLAPixel& origPixel = png.getPixel(origPoint.x, origPoint.y);
  HSLAPixel& newPixel = png.getPixel(point.x, point.y);
  if(calculateDelta(origPixel, newPixel) >= tolerance)
    return false;
  
  return true;
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  Point curr = traversal->pop();
  valid[curr.x][curr.y] = false;

  Point right = Point(curr.x + 1, curr.y);
  Point below = Point(curr.x, curr.y + 1);
  Point left = Point(curr.x - 1, curr.y);
  Point above = Point(curr.x, curr.y - 1);

  if(isValid(right))
    traversal->add(right);
  if(isValid(below)) 
    traversal->add(below);
  if(isValid(left)) 
    traversal->add(left);
  if(isValid(above)) 
    traversal->add(above);

  while(!traversal->empty() && !valid[traversal->peek().x][traversal->peek().y])
    traversal->pop();
  
  if(traversal->empty()) {
    traversal = NULL;
    return *this;
  }

  newPoint = traversal->peek();
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return newPoint;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return traversal != other.traversal && png != other.png && tolerance != other.tolerance;
}

