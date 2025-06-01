#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H
#include <ostream>
#include <vector>

using namespace std;

class Battlefield {
private:
    int rows;
    int cols;
    std::vector<std::vector<char>> grid;

public:
    Battlefield(int r, int c);
    bool placeRobot(int r, int c, char symbol);
    bool moveRobot(int oldR, int oldC, int newR, int newC, char symbol);
    char getAt(int r, int c) const;
    bool removeRobot(int r, int c);
    int getRows() const;
    int getCols() const;
    void display(ostream& out) const;
};

#endif