#include "Battlefield.h"
#include <iostream>
using namespace std;

Battlefield::Battlefield(int r, int c) : rows(r), cols(c) {
    grid.resize(rows, vector<char>(cols, '.'));
}

bool Battlefield::placeRobot(int r, int c, char symbol) {
    if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == '.') {
        grid[r][c] = symbol;
        return true;
    }
    return false;
}

bool Battlefield::moveRobot(int oldR, int oldC, int newR, int newC, char symbol) {
    if (newR >= 0 && newR < rows && newC >= 0 && newC < cols && grid[newR][newC] == '.') {
        grid[oldR][oldC] = '.';
        grid[newR][newC] = symbol;
        return true;
    }
    return false;
}

char Battlefield::getAt(int r, int c) const {
    if (r >= 0 && r < rows && c >= 0 && c < cols)
        return grid[r][c];
    return '?';
}

bool Battlefield::removeRobot(int r, int c) {
    if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] != '.') {
        grid[r][c] = '.';
        return true;
    }
    return false;
}

int Battlefield::getRows() const { return rows; }
int Battlefield::getCols() const { return cols; }

void Battlefield::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
}
