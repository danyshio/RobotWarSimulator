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
#include <cstdlib>
#include <ctime>
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

    bool placeRobot(int r, int c, char symbol) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == '.') {
            grid[r][c] = symbol;
            return true;
        }
        return false;
    }

    bool isOccupied(int r, int c) const{
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            return grid[r][c] != '.';
        }
        return false; // treated as not occupied
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

class GenericRobot{
    private:
        string name;
        int row, col;
    public:
        GenericRobot(string n, int r, int c) : name(n), row(r), col(c) {}

        string getName() const {
        return name;
    }

    int getRow() const {
        return row;
    }

    int getCol() const {
        return col;
    }

    void setPosition(int r, int c) {
        row = r;
        col = c;
    }

    bool look(const Battlefield& field, int r, int c) const {
        return field.isOccupied(r, c);
    }

};
int main() {
    srand(time(0)); // seed for randomness

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

    // Create battlefield
    Battlefield field(rows, cols);

    vector<GenericRobot> robots;

    // Read robot data
    for (int i = 0; i < robotCount; ++i) {
        getline(inFile, line);
        istringstream robotStream(line);
        string type, name, rowStr, colStr;
        robotStream >> type >> name >> rowStr >> colStr;

        int r, c;

        if (rowStr == "random")
            r = rand() % rows;
        else
            r = stoi(rowStr);

        if (colStr == "random")
            c = rand() % cols;
        else
            c = stoi(colStr);

        // Use first letter of robot name as symbol (uppercase)
        char symbol = toupper(name[0]);

        // Try placing robot, retry random if space is taken
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 100) {
            if (field.placeRobot(r, c, symbol)) {
                robots.emplace_back(name, r, c);
                placed = true;
            } else {
                r = rand() % rows;
                c = rand() % cols;
                ++attempts;
            }
            
            
        }
    }

    // ignore first, this is just for reference
    cout << "----- Battlefield Layout from input.txt -----" << endl;
    cout << "Rows: " << rows << ", Columns: " << cols << endl;
    cout << "Steps: " << steps << endl;
    cout << "Robots: " << robotCount << endl << endl;

    cout << "----- Battlefield Layout -----" << endl;
    field.print();

    // Test look function
    cout << "\n----- Robot Look Test -----" << endl;
    for (const auto& robot : robots) {
        int testRow = robot.getRow();
        int testCol = robot.getCol();
        cout << robot.getName() << " looks at (" << testRow << ", " << testCol << "): "
             << (robot.look(field, testRow, testCol) ? "Occupied" : "Empty") << endl;
    }

    return 0;
}