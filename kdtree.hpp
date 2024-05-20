/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    
    // return the smaller value of the corresponding dimension
    if(first[curDim] != second[curDim])
      return first[curDim] < second[curDim];

    return first < second;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double currentSum = 0;
    double potentialSum =0;

    // calculate the sum of the square of the differences
    for(int i = 0; i < Dim; i++) {
      currentSum += pow(currentBest[i] - target[i], 2);
      potentialSum += pow(potential[i] - target[i], 2);
    }

    // return the shorter distance
    if(currentSum != potentialSum)
      return currentSum > potentialSum;
    
    return currentBest > potential;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */

    // initialize private variables
    size = 0;
    if(newPoints.empty())
      return;

    vector<Point<Dim>> temp = newPoints; // due to const label in the parameter
    root = helpConstruct(temp, 0, newPoints.size() - 1, 0); // call recursive helper function
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::helpConstruct(vector<Point<Dim>>& newPoints, int left, int right, int d) {
    // if the indices cross, the traversal reaches a leaf
    if(right < left)
      return NULL;

    size++; // increase the size of the tree
    // calculate the median index and Point using quick select algo
    size_t median = (left + right) / 2; 
    Point<Dim> medianPoint = quickselect(newPoints, left, right, median, d);

    // create the new node and recursively create tree
    KDTreeNode* node = new KDTreeNode(medianPoint);
    node->left = helpConstruct(newPoints, left, median - 1, (d + 1) % Dim);
    node->right = helpConstruct(newPoints, median + 1, right, (d + 1) % Dim);

    return node;
}

template <int Dim>
Point<Dim> KDTree<Dim>::quickselect(vector<Point<Dim>>& newPoints, int left, int right, int k, int dim) {
  if(left == right)
    return newPoints[left];
  
  // find the pivot index from partition
  int pivotIndex = k;
  pivotIndex = partition(newPoints, left, right, pivotIndex, dim);
  
  // return or recursively call
  if(k == pivotIndex) 
    return newPoints[k]; 
  else if(k < pivotIndex)
    return quickselect(newPoints, left, pivotIndex - 1, k, dim);
  else 
    return quickselect(newPoints, pivotIndex + 1, right, k, dim);
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& newPoints, int left, int right, int pivotIndex, int dim){
  Point<Dim> pivotPoint = newPoints[pivotIndex];
  swap(newPoints[pivotIndex], newPoints[right]); // move pivot to end

  // move smaller values to the left
  size_t temp = left;
  for(int i = left; i < right; i++)
    if(smallerDimVal(newPoints[i], pivotPoint, dim)) {
      swap(newPoints[temp], newPoints[i]);
      temp++;
    }

  swap(newPoints[right], newPoints[temp]); // move pivot to its final place
  return temp;
}

// copy each element of the tree
template <int Dim>
void KDTree<Dim>::copy(KDTreeNode* node, KDTreeNode* otherNode) {
  node = new KDTreeNode();
  node->point = otherNode->point;
  copy(node->left, otherNode->left);
  copy(node->right, otherNode->right);
}

// delete each element of the tree
template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode* node) {
  if(!node)
    return;

  if(node->left != NULL)
    destroy(node->left);
  if(node->right != NULL)
    destroy(node->right); 

  delete node;
  node = NULL;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(root, other.root);
  size = other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if(this != NULL)
    destroy(root);
  copy(root, rhs.root);
  size = rhs.size;

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  destroy(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    
    Point<Dim> nearest = root->point;
    Point<Dim> temp = query;
    findHelp(root, nearest, temp, 0);
    return nearest;
}

template <int Dim>
void KDTree<Dim>::findHelp(KDTreeNode* curRoot, Point<Dim> &currentBest, Point<Dim>& query, int dim) const {
    if(curRoot == NULL)
      return;

    // which side of the current point is the query by dim
    KDTreeNode* nearest;
    if(smallerDimVal(curRoot->point, query, dim))
      nearest = curRoot->right;
    else
      nearest = curRoot->left;
    
    // if already recursed on left/right recurse on right/left
    KDTreeNode* subTree;
    if(nearest == curRoot->left)
      subTree = curRoot->right;
    else 
      subTree = curRoot->left;

    // recurse on the hyperrectangle that contains the query
    findHelp(nearest, currentBest, query, (dim + 1) % Dim);

    // update the nearest if curRoot is closer
    if(shouldReplace(query, currentBest, curRoot->point))
      currentBest = curRoot->point;

    // calculate the radius between query and nearest
    double r = 0.0;
    for(int i = 0; i < Dim; i++)
      r += pow(currentBest[i] - query[i], 2);

    // split distance on place
    double splitDist = pow(curRoot->point[dim] - query[dim], 2);
    if(r >= splitDist)
      findHelp(subTree, currentBest, query, (dim + 1) % Dim);
}

