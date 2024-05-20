/* Your code here! */
#pragma once
#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <vector>

using namespace std;
using namespace cs225;

class SquareMaze {
    private:
        vector<pair<bool, bool>> allWalls;
        int height;
        int width;
        DisjointSets set;
    public:
        SquareMaze();
        void makeMaze(int width, int height);
        bool canTravel (int x, int y, int dir) const;
        void setWall(int x, int y, int dir, bool exists);
        vector<int> solveMaze();
        PNG* drawMaze() const;
        PNG* drawMazeWithSolution();
};