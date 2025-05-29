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

    bool moveRobot(int oldR, int oldC, int newR, int newC, char symbol) {
        if (newR >= 0 && newR < rows && newC >= 0 && newC < cols && grid[newR][newC] == '.') {
            grid[oldR][oldC] = '.';
            grid[newR][newC] = symbol;
            return true;
        }
        return false;
    }

    char getAt(int r, int c) const {
        if (r >= 0 && rows && c >= 0 && c < cols)
            return grid[r][c];
        return '?'; // invalid access
    }

     bool removeRobot(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] != '.') {
            grid[r][c] = '.';
            return true;
        }
        return false;
    }

    int getRows() const {return rows;}
    int getCols() const {return cols;}

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
        char symbol;
        bool hasThought;
        bool alive;

        bool isAdjacent(int dx, int dy) const {
        return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
    }

    public:
        GenericRobot(string n, int r, int c, char sym) : name(n), row(r), col(c), symbol(sym), hasThought(false), alive(true) {}

        string getName() const {return name;}
        int getRow() const {return row;}
        int getCol() const {return col;}
        char getSymbol() const {return symbol;}
        bool isAlive() const {return alive;}

        void think() {
            hasThought = true;
            cout << name << " is thinking..." << endl;
        }
    
        void look(const Battlefield& field, int dx, int dy) const {
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

        void move(Battlefield& field, int dx, int dy) {
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

        void fire(Battlefield& field, vector<GenericRobot>& robots, int dx, int dy) {
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
            if(r.isAlive() && r.getRow() == targetR && r.getCol() == targetC){
                field.removeRobot(targetR, targetC);
                r.alive = false;
                cout << " fired and DESTROYED " << r.getName() << " at (" << targetC << ")!" << endl;
                return;
            }
        }
        cout << name << " fired and MISSED at (" << targetR << ", " << targetC << ")." << endl;
    }

        void endTurn() {
        hasThought = false;
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
        int attempts = 0;

        while (!field.placeRobot(r, c, symbol) && attempts < 100) {
            r = rand() % rows;
            c = rand() % cols;
            ++attempts; 
        }
        robots.emplace_back(name, r, c, symbol);
    }

    cout << "----- Initial Battlefield -----" << endl;
    field.print();

    // Simulate one step
    cout << "\n----- Robot Turn -----" << endl;
    for (auto& robot : robots) {
        if (!robot.isAlive()) continue;
        robot.think();
        robot.look(field, 0, 1);  // Look right
        robot.move(field, 0, 1);  // Try to move right
        robot.fire(field, robots, 1, 0); 
        robot.endTurn();
        cout << endl;
    }

    cout << "----- Battlefield After Turn -----" << endl;
    field.print();

    return 0;
}