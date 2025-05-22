 /**********|**********|**********|
 Program: YOUR_FILENAME.cpp / YOUR_FILENAME.h 
Course: Data Structures and Algorithms
 Trimester: 2410
 Name: Frank Carrano
 ID: 1071001234
 Lecture Section: TC3L
 Tutorial Section: T11L
 Email: abc123@yourmail.com
 Phone: 010-5324386
 **********|**********|**********/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Battlefield {
private:
    int rows;
    int cols;
    vector<vector<char>> grid;

public:
    Battlefield(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<char>(cols, '.'));
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << grid[i][j] << ' ';
            }
            cout << endl;
        }
    }
};

int main() {
    ifstream inFile("input.txt");
    if (!inFile) {
        cerr << "Failed to open input.txt" << endl;
        return 1;
    }

    string line;
    int rows = 0, cols = 0, steps = 0, robotCount = 0;

    // Read "M by N : rows cols"
    getline(inFile, line);
    istringstream dimStream(line.substr(line.find(':') + 1));
    dimStream >> rows >> cols;

    // Read "steps: xxx"
    getline(inFile, line);
    steps = stoi(line.substr(line.find(':') + 1));

    // Read "robots: xxx"
    getline(inFile, line);
    robotCount = stoi(line.substr(line.find(':') + 1));

    // Print the battlefield
    Battlefield field(rows, cols);
    field.print();

    return 0;
}
