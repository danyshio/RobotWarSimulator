#include "GenericRobot.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

GenericRobot::GenericRobot(string n, int r, int c, char sym)
    : name(n), row(r), col(c), symbol(sym), hasThought(false), alive(true),
      lives(3), shells(10), reentries(0) {}

string GenericRobot::getName() const { return name; }
int GenericRobot::getRow() const { return row; }
int GenericRobot::getCol() const { return col; }
char GenericRobot::getSymbol() const { return symbol; }
bool GenericRobot::isAlive() const { return alive; }
int GenericRobot::getLives() const { return lives; }
int GenericRobot::getShells() const { return shells; }
int GenericRobot::getReentries() const { return reentries; }

void GenericRobot::think(ostream& out) {
    hasThought = true;
    out << name << " is thinking..." << endl;
}

bool GenericRobot::isAdjacent(int dx, int dy) const {
    return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
}

bool GenericRobot::inBounds(int r, int c, const Battlefield& field) const {
    return r >= 0 && r < field.getRows() && c >= 0 && c < field.getCols();
}

void GenericRobot::look(const Battlefield& field, int dx, int dy, ostream& out) const {
    if (!hasThought) {
        out << name << " must think before looking!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        out << name << " can only look at adjacent squares!" << endl;
        return;
    }

    int r = row + dx;
    int c = col + dy;
    cout << name << " looks at (" << r << ", " << c << "): ";

    if (!inBounds(r, c, field)) {
        out << "Out of bounds!" << endl;
    } else {
        char cell = field.getAt(r, c);
        if (cell == '.') cout << "Empty." << endl;
        else out << "Occupied by " << cell << "." << endl;
    }
}

void GenericRobot::move(Battlefield& field, int dx, int dy, ostream& out) {
    if (!hasThought) {
        out << name << " must think before moving!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        out << name << " can only move to adjacent squares!" << endl;
        return;
    }

    int newR = row + dx;
    int newC = col + dy;
    if (field.moveRobot(row, col, newR, newC, symbol)) {
        row = newR;
        col = newC;
        out << name << " moved to (" << row << ", " << col << ")." << endl;
    } else {
        out << name << " failed to move to (" << newR << ", " << newC << ")." << endl;
    }
}

void GenericRobot::fire(Battlefield& field, vector<GenericRobot>& robots, int dx, int dy, ostream& out) {
    if (!hasThought) {
        out << name << " must think before firing!" << endl;
        return;
    }
    if (!isAdjacent(dx, dy)) {
        out << name << " can only fire at adjacent squares!" << endl;
        return;
    }

    if (dx == 0 && dy == 0) {
        out << name << " cannot fire at its own location!" << endl;
        return;
    }

    if (shells <= 0) {
        out << name << " has no shells left and will self-destruct!" << endl;
        field.removeRobot(row, col);
        alive = false;
        return;
    }

    --shells;

    int targetR = row + dx;
    int targetC = col + dy;

    if (!inBounds(targetR, targetC, field)) {
        out << name << " fired out of bounds." << endl;
        return;
    }

    // 70% chance to hit
    bool hit = (rand() % 100) < 70;

    if (hit) {
        for (auto& r : robots) {
            if (r.isAlive() && r.getRow() == targetR && r.getCol() == targetC) {
                field.removeRobot(targetR, targetC);
                r.alive = false;
                r.lives--;

                out << name << " fired and DESTROYED " << r.getName()
                     << " at (" << targetR << ", " << targetC << ")!" << endl;
                return;
            }
        }
        out << name << " fired but target was empty at (" << targetR << ", " << targetC << ")." << endl;
    } else {
        out << name << " MISSED at (" << targetR << ", " << targetC << ")." << endl;
    }

    if (shells <= 0) {
        out << name << " has run out of shells and will self-destruct!" << endl;
        field.removeRobot(row, col);
        alive = false;
    }
}

void GenericRobot::endTurn() {
    hasThought = false;
}

void GenericRobot::reset() {
    alive = true;
    shells = 10;
    hasThought = false;
}

bool GenericRobot::attemptReentry(Battlefield& field, int rows, int cols, ostream& out) {
    if (alive || lives <= 0 || reentries >= 3) return false;

    for (int attempt = 0; attempt < 100; ++attempt) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (field.placeRobot(r, c, symbol)) {
            row = r;
            col = c;
            reset();
            ++reentries;
            out << name << " has re-entered the battlefield at (" << r << ", " << c << ")." << endl;
            return true;
        }
    }

    return false;
}
