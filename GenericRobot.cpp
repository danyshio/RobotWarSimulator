#include "GenericRobot.h"
#include <iostream>
#include <cmath>
using namespace std;

GenericRobot::GenericRobot(string n, int r, int c, char sym)
    : name(n), row(r), col(c), symbol(sym), hasThought(false), alive(true) {}

string GenericRobot::getName() const { return name; }
int GenericRobot::getRow() const { return row; }
int GenericRobot::getCol() const { return col; }
char GenericRobot::getSymbol() const { return symbol; }
bool GenericRobot::isAlive() const { return alive; }

bool GenericRobot::isAdjacent(int dx, int dy) const {
    return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
}

void GenericRobot::think() {
    hasThought = true;
    cout << name << " is thinking..." << endl;
}

void GenericRobot::look(const Battlefield& field, int dx, int dy) const {
    if (!hasThought) {
        cout << name << " must think before looking!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        cout << name << " can only look at adjacent squares!" << endl;
        return;
    }

    int r = row + dx;
    int c = col + dy;
    cout << name << " looks at (" << r << ", " << c << "): ";

    if (r < 0 || r >= field.getRows() || c < 0 || c >= field.getCols()) {
        cout << "Out of bounds!" << endl;
    } else {
        char cell = field.getAt(r, c);
        if (cell == '.') cout << "Empty." << endl;
        else cout << "Occupied by " << cell << "." << endl;
    }
}

void GenericRobot::move(Battlefield& field, int dx, int dy) {
    if (!hasThought) {
        cout << name << " must think before moving!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        cout << name << " can only move to adjacent squares!" << endl;
        return;
    }

    int newR = row + dx;
    int newC = col + dy;
    if (field.moveRobot(row, col, newR, newC, symbol)) {
        row = newR;
        col = newC;
        cout << name << " moved to (" << row << ", " << col << ")." << endl;
    } else {
        cout << name << " failed to move to (" << newR << ", " << newC << ")." << endl;
    }
}

void GenericRobot::fire(Battlefield& field, vector<GenericRobot>& robots, int dx, int dy) {
    if (!hasThought) {
        cout << name << " must think before firing!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        cout << name << " can only fire at adjacent squares!" << endl;
        return;
    }

    int targetR = row + dx;
    int targetC = col + dy;
    for (auto& r : robots) {
        if (r.isAlive() && r.getRow() == targetR && r.getCol() == targetC) {
            field.removeRobot(targetR, targetC);
            r.alive = false;
            cout << name << " fired and DESTROYED " << r.getName() << " at (" << targetR << ", " << targetC << ")!" << endl;
            return;
        }
    }
    cout << name << " fired and MISSED at (" << targetR << ", " << targetC << ")." << endl;
}

void GenericRobot::endTurn() {
    hasThought = false;
}
