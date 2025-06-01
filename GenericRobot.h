#ifndef GENERICROBOT_H
#define GENERICROBOT_H

#include <string>
#include <vector>
#include "Battlefield.h"
using namespace std;

class GenericRobot {
private:
    std::string name;
    int row, col;
    char symbol;
    bool hasThought;
    bool alive;
    int lives;
    int shells;
    int reentries;

    bool isAdjacent(int dx, int dy) const;
    bool inBounds(int r, int c, const Battlefield& field) const;

public:
    GenericRobot(std::string n, int r, int c, char sym);

    std::string getName() const;
    int getRow() const;
    int getCol() const;
    char getSymbol() const;
    bool isAlive() const;
    int getLives() const;
    int getShells() const;
    int getReentries() const;

    void think(std::ostream& out);
    void look(const Battlefield& field, int dx, int dy, ostream& out) const;
    void move(Battlefield& field, int dx, int dy, ostream& out);
    void fire(Battlefield& field, std::vector<GenericRobot>& robots, int dx, int dy, ostream& out);
    void endTurn();

    void reset(); // reset shell and thought for new re-entry
    bool attemptReentry(Battlefield& field, int rows, int cols, ostream& out);
};

#endif