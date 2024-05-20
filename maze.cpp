/* Your code here! */
#include "maze.h"
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <random>

using namespace std;

SquareMaze::SquareMaze() {}

void SquareMaze::makeMaze(int width, int height) {
    // initialize variables & data structures
    this->width = width;
    this->height = height;
    int len = width * height;
    set.addelements(len);
  
    for(int i = 0; i < len; i++) {
        allWalls.push_back(pair<bool, bool> (true, true)); }

    // randomly choose a wall to erase, if the wall meets requirements, erase the wall and union the sets
    int j = 0;
    while(j < len-1) {
        int row = rand() % height;
        int col = rand() % width;
        bool whichWall = rand() % 2;

        if(whichWall) {
            if(col < width - 1 && allWalls[col + row * width].first && set.find(col + row * width) != set.find(col + row * width + 1)) {
                allWalls[col + row * width].first = false;
                set.setunion(set.find(col + row * width),set.find(col + row * width + 1));
                j++;
            }
        } 
        else {
            if(row < height - 1 && allWalls[col + row * width].second && set.find(col + row * width) != set.find(col + (row + 1) * width)) {
                allWalls[col + row * width].second = false;
                set.setunion(set.find(col + row * width), set.find(col + (row + 1) * width));
                j++;
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if(dir == 0) { // right
        if(x >= width - 1 || allWalls[x + y * width].first) // out of the maze or through a wall
            return false;
    }
    else if(dir == 1) { // down
        if(y >= height - 1 || allWalls[x + y * width].second) // out of the maze or through a wall
            return false;
    }
    else if(dir == 2) { // left
        if(x <= 0 || allWalls[(x - 1) + y * width].first) // out of the maze or through a wall
            return false;
    }
    else { // if dir == 3 -> up
        if(y <= 0 || allWalls[x + (y - 1) * width].second) // out of the maze or through a wall
            return false;
    }

    return true;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    // depending on the direction, assign whether the wall exists
    if(dir == 0) 
        allWalls[x + y * width].first = exists;
    else // if dir == 1
        allWalls[x + y * width].second = exists;
}

vector<int> SquareMaze::solveMaze(){
    // initialize data structures
    vector<bool> visited;
    queue<int> traversal;

    for(int i = 0; i < width * height; i++)
        visited.push_back(false);
    visited[0] = true;
    traversal.push(0);

    // traverse through the maze using the traversal and iterator
    vector<int> bottom;
    map<int, int> iterator;
    while(!traversal.empty()) {
        int current = traversal.front();
        traversal.pop();

        // find the current row/col and store the bottom row
        int row = current / width;
        int col = current % width;
        if(row == height - 1) 
            bottom.push_back(current);

        // travel through the maze by checking if the direction is valid
        if(canTravel(col, row, 0) && !visited[current + 1]) {
            iterator.insert(pair<int, int>(current + 1, current));
            visited[current + 1] = true;
            traversal.push(current + 1);
        }
        if(canTravel(col, row, 1) && !visited[current + width]) {
            iterator.insert(pair<int, int>(current + width, current));
            visited[current + width] = true;
            traversal.push(current + width);
        }

        if(canTravel(col, row, 2) && !visited[current - 1]) {
            iterator.insert(pair<int, int>(current - 1, current));
            visited[current - 1] = true;
            traversal.push(current - 1);
        }

        if(canTravel(col, row, 3) && !visited[current - width]) {
            iterator.insert(pair<int, int>(current - width, current));
            visited[current - width] = true;
            traversal.push(current - width);
        }
    }

    int temp = width - 1;
    while(bottom[temp] == bottom[temp - 1]) 
        temp--;

    int current = bottom[temp];
    stack<int> directions;
    while(current != 0) {
        int previous = iterator[current];
        if(current == previous + 1) 
            directions.push(0);
        else if(current == previous + width) 
            directions.push(1);
        else if(current == previous - 1) 
            directions.push(2);
        else if(current == previous - width) 
            directions.push(3);
        current = previous;
    }

    vector<int> solution;
    int size = directions.size();
    for(int i = 0; i < size; i++) {
        solution.push_back(directions.top());
        directions.pop();
    }

    return solution;
}

PNG* SquareMaze::drawMaze() const{
    PNG* maze = new PNG(1 + width * 10, 1 + height * 10);

    for(int col = 0; col < width; col++) 
        for(int row = 0; row < height; row++) {
            if(!row) 
                for(unsigned i = 0; i < 10; i++) {
                    if(i + col * 10 < 1 || i + col * 10 > 9) {
                        HSLAPixel& curPixel = maze->getPixel(i + col * 10, 0);
                        curPixel.l = 0;
                    }
                }
        
            if(!col) 
                for(unsigned i = 0; i < 10; i++) {
                    HSLAPixel& curPixel = maze->getPixel(0, i + row * 10);
                    curPixel.l = 0;
                }
            
            if(!canTravel(col, row, 0)) 
                for(unsigned i = 0; i <= 10; i++) {
                    HSLAPixel& curPixel = maze->getPixel((col + 1) * 10, i + row * 10);
                    curPixel.l = 0;
                }
            
            if(!canTravel(col, row, 1)) 
                for(unsigned i = 0; i <= 10; i++) {
                    HSLAPixel& curPixel = maze->getPixel(i + col * 10, (row + 1) * 10);
                    curPixel.l = 0;
                }
        }

    return maze;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* maze = drawMaze();
    vector<int> solution = solveMaze();
    int row = 5;
    int col = 5;

    for(unsigned i = 0; i < solution.size(); i++) {
        if(!solution[i])
            for(int j = 0; j < 10; j++) {
                HSLAPixel& curPixel = maze->getPixel(col, row);
                curPixel.l = 0.5;
                curPixel.h = 0;
                curPixel.s = 1;
                col++;
            }
        
        if(solution[i] == 1) 
            for(int j = 0; j < 10; j++) {
                HSLAPixel& curPixel = maze->getPixel(col, row);
                curPixel.l = 0.5;
                curPixel.h = 0;
                curPixel.s = 1;
                row++;
            }
        
        if(solution[i] == 2) 
            for(int j = 0; j < 10; j++) {
                HSLAPixel& curPixel = maze->getPixel(col, row);
                curPixel.l = 0.5;
                curPixel.h = 0;
                curPixel.s = 1;
                col--;
            }

        if(solution[i] == 3) 
            for(int j = 0; j < 10; j++) {
                HSLAPixel& curPixel = maze->getPixel(col, row);
                curPixel.l = 0.5;
                curPixel.h = 0;
                curPixel.s = 1;
                row--;
            }     
    }

    HSLAPixel& pixel = maze->getPixel(col, row);
    pixel.l = 0.5;
    pixel.h = 0;
    pixel.s = 1;
    pixel.a = 1;
    col -= 4;
    row += 5;

    for(int i = 1; i < 10; i++) {
        HSLAPixel& curPixel = maze->getPixel(col, row);
        curPixel.l = 1;
        curPixel.a = 1;
        col++;
    }

    return maze;
}