#ifndef GENERICROBOT_H
#define GENERICROBOT_H

#include <string>
#include <vector>
#include "Battlefield.h"

class GenericRobot {
private:
    std::string name;
    int row, col;
    char symbol;
    bool hasThought;
    bool alive;

    bool isAdjacent(int dx, int dy) const;

public:
    GenericRobot(std::string n, int r, int c, char sym);
    std::string getName() const;
    int getRow() const;
    int getCol() const;
    char getSymbol() const;
    bool isAlive() const;

    void think();
    void look(const Battlefield& field, int dx, int dy) const;
    void move(Battlefield& field, int dx, int dy);
    void fire(Battlefield& field, std::vector<GenericRobot>& robots, int dx, int dy);
    void endTurn();
};

#endif
