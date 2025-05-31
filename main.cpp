#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Battlefield.h"
#include "GenericRobot.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));

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

    Battlefield field(rows, cols);
    vector<GenericRobot> robots;

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

        char symbol = toupper(name[0]);

        int attempts = 0;
        while (!field.placeRobot(r, c, symbol) && attempts < 100) {
            r = rand() % rows;
            c = rand() % cols;
            ++attempts;
        }

        robots.emplace_back(name, r, c, symbol);
    }

    cout << "----- Initial Battlefield -----" << endl;
    field.display();

    cout << "DEBUG: Starting simulation with " << steps << " steps." << endl;

for (int step = 0; step < steps; ++step) {
    cout << "\n----- Step " << (step + 1) << " -----" << endl;

    for (auto& robot : robots) {
        if (!robot.isAlive()) continue;

        cout << "DEBUG: Robot " << robot.getName() << " starts turn." << endl;

        robot.think();
        robot.look(field, 0, 1);  // Look right
        robot.fire(field, robots, 0, 1); // Fire right
        robot.move(field, 0, 1);  // Move right
        robot.endTurn();

        cout << "DEBUG: Robot " << robot.getName() << " ends turn." << endl;
    }

    // Try to reenter one robot
    for (auto& robot : robots) {
        if (!robot.isAlive() && robot.getLives() > 0 && robot.getReentries() < 3) {
            if (robot.attemptReentry(field, rows, cols)) {
                cout << "DEBUG: " << robot.getName() << " reentered the battlefield." << endl;
                break;
            }
        }
    }

    cout << "----- Battlefield After Turn " << (step + 1) << " -----" << endl;
    field.display();

    cout.flush(); // Make sure everything gets printed immediately
}

}