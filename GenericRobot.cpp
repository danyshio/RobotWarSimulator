#include "GenericRobot.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Utils.h"

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
    writeBoth(out, name + " is thinking...\n");
}

bool GenericRobot::isAdjacent(int dx, int dy) const {
    return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
}

bool GenericRobot::inBounds(int r, int c, const Battlefield& field) const {
    return r >= 0 && r < field.getRows() && c >= 0 && c < field.getCols();
}

void GenericRobot::look(const Battlefield& field, int dx, int dy, ostream& out) const {
    if (!hasThought) {
        writeBoth(out, name + " must think before looking!\n");
        return;
    }
    if (!isAdjacent(dx, dy)) {
        writeBoth(out, name + " can only look at adjacent squares!\n");
        return;
    }

    int r = row + dx;
    int c = col + dy;
    writeBoth(out, name + " looks at (" + std::to_string(r) + ", " + std::to_string(c) + "): ");

    if (!inBounds(r, c, field)) {
        out << "Out of bounds!" << endl;
    } else {
        char cell = field.getAt(r, c);
        if (cell == '.') {
            writeBoth(out, "Empty.\n");
        }
        else {
            writeBoth(out, std::string("Occupied by ") + cell + ".\n");
        }
}
}

void GenericRobot::move(Battlefield& field, int dx, int dy, ostream& out) {
    if (!hasThought) {
        writeBoth(out, name + " must think before moving!\n");
        return;
    }
    if (!isAdjacent(dx, dy)) {
        writeBoth(out, name + " can only move to adjacent squares!\n");
        return;
    }

    int newR = row + dx;
    int newC = col + dy;
    if (field.moveRobot(row, col, newR, newC, symbol)) {
        row = newR;
        col = newC;
        writeBoth(out, name + " moved to (" + std::to_string(row) + ", " + std::to_string(col) + ").\n");
    } else {
        writeBoth(out, name + " failed to move to (" + std::to_string(newR) + ", " + std::to_string(newC) + ").\n");
    }
}

void GenericRobot::fire(Battlefield& field, vector<GenericRobot>& robots, int dx, int dy, ostream& out) {
    if (!hasThought) {
        writeBoth(out, name + " must think before firing!\n");
        return;
    }
    if (!isAdjacent(dx, dy)) {
        writeBoth(out, name + " can only fire at adjacent squares!\n");
        return;
    }

    if (dx == 0 && dy == 0) {
        writeBoth(out, name + " cannot fire at its own location!\n");
        return;
    }

    if (shells <= 0) {
        writeBoth(out, name + " has no shells left and will self-destruct!\n");
        field.removeRobot(row, col);
        alive = false;
        return;
    }

    --shells;

    int targetR = row + dx;
    int targetC = col + dy;

    if (!inBounds(targetR, targetC, field)) {
        writeBoth(out, name + " fired out of bounds.\n");
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

                writeBoth(out, name + " fired and DESTROYED " + r.getName() + 
                " at (" + std::to_string(targetR) + ", " + std::to_string(targetC) + ")!\n");

                return;
            }
        }
        writeBoth(out, name + " fired but target was empty at (" + std::to_string(targetR) + ", " + std::to_string(targetC) + ").\n");
    } else {
        writeBoth(out, name + " MISSED at (" + std::to_string(targetR) + ", " + std::to_string(targetC) + ").\n");
    }

    if (shells <= 0) {
        writeBoth(out, name + " has run out of shells and will self-destruct!\n");
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
            writeBoth(out, name + " has re-entered the battlefield at (" + std::to_string(r) + ", " + std::to_string(c) + ").\n");
            return true;
        }
    }

    return false;
}