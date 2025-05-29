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

    bool fireAt(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] != '.') {
            grid[r][c] = '.'; // Hit!
            return true;
        }
        return false;
    }

    char getAt(int r, int c) const {
        if (r >= 0 && rows && c >= 0 && c < cols)
            return grid[r][c];
        return '?'; // invalid access
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
    protected:
        string name;
        int row, col;
        char symbol;
        bool hasThought;

        bool isAdjacent(int dx, int dy) const {
        return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
    }

    public:
        GenericRobot(string n, int r, int c, char sym) : name(n), row(r), col(c), symbol(sym), hasThought(false) {}

        string getName() const {return name;}
        int getRow() const {return row;}
        int getCol() const {return symbol;}
        char getSymbol() const {return symbol;}

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

        virtual void fire(Battlefield& field, int dx, int dy) {
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
        if (field.fireAt(targetR, targetC)) {
            cout << name << " fired and HIT at (" << targetR << ", " << targetC << ")!" << endl;
        } else {
            cout << name << " fired and MISSED at (" << targetR << ", " << targetC << ")." << endl;
        }
    }

        virtual void endTurn() {
        hasThought = false;
    }
};

class HideBot : public GenericRobot {
    private:
        int hideCharges;
        bool isHidden;

    public:
        HideBot(string n, int r, int c, char sym, int charges) :
            GenericRobot(n, r, c, sym), hideCharges(charges), isHidden(false) {}

        void activeHide() {
            if (hideCharges > 0 && !isHidden) {
                isHidden = true;
                hideCharges--;
                cout << getName() << "is now hidden (" << hideCharges << " charges left)" << endl;
            }   else if (isHidden) {
                cout << getName() << "is already hidden" << endl;
            }   else{
                cout << getName() << "has no hide charges left" << endl;
            }
        }

        void deactiveHide() {
            if (isHidden) {
                isHidden = false;
                cout << getName() << "is now can be seen" << endl;
            }
        }

        bool isCurrentlyHidden() const {
            return isHidden;
        } 

        void fire(Battlefield& field, int dx, int dy) override {
            GenericRobot::fire(field, dx,dy);
            int targetR = getRow() + dx;
            int targetC = getCol() + dy;
            if (field.getAt(targetR, targetC) == '.') {
                if (hideCharges == 3) {
                    cout << getName() << " is hidden and cannot be targeted" << endl;
                }
            }

        }

};

class jumpBot : public GenericRobot {
    private:
        int jumpCharges;
        const int maxJump = 3;
        bool hasJumpedThisTurn;

    public:
        jumpBot(string n, int r, int c, char sym) :
            GenericRobot(n, r, c, sym), jumpCharges(0), hasJumpedThisTurn(false) {}

        void jump(Battlefield& field, int newR, int newC) {
            if (!hasThought) {
                cout << getName() << "is thinking before jumping" << endl;
                return;
            } if (hasJumpedThisTurn) {
                cout << getName() << "has already jumped this turn" << endl;
                return;
            } if (jumpCharges >= maxJump) {
                cout << getName() << "has no jump charges" << endl;
                return;
            }

            if (newR < 0 || newR >= field.getRows() || newC < 0 || newC >= field.getCols()) {
                cout << getName() << "cannot jump out of Battlefield" << endl;
                return;
            }

            if (field.getAt(newR, newC) != '.') {
                cout << getName() << "the position is occupied" << endl;
                return;
            }

            field.moveRobot(getRow(), getCol(), newR, newC, getSymbol());
            row = newR;
            col = newC;
            jumpCharges++;
            hasJumpedThisTurn = true;

            cout << getName() << "jumped to (" << row << "," << col << ")" << endl;
            cout << jumpCharges << " jump charges left" << endl;
        }

        void endTurn() override {
            GenericRobot::endTurn();
            hasJumpedThisTurn = false;
        }

        int getRemainingJumps() const {
            return maxJump - jumpCharges;
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

        while (field.placeRobot(r, c, symbol) && attempts < 100) {
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
        robot.think();
        robot.look(field, 0, 1);  // Look right
        robot.move(field, 0, 1);  // Try to move right
        robot.fire(field, 1, 0);  // Try to fire downward
        robot.endTurn();
        cout << endl;
    }

    cout << "----- Battlefield After Turn -----" << endl;
    field.print();

    return 0;
}