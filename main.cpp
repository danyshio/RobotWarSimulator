#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Battlefield.h"
#include "GenericRobot.h"
#include "Utils.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));

    ifstream inFile("input.txt");
    if (!inFile) {
        cerr << "Failed to open input.txt" << endl;
        return 1;
    }

    ofstream outFile("game1_log.txt"); // Log output here
    if (!outFile){
        cerr << "Failed to create log file." << endl;
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

    writeBoth(outFile, "----- Initial Battlefield -----\n");
    field.display(outFile); // display handles writeBoth inside now

    writeBoth(outFile, "DEBUG: Starting simulation with " + to_string(steps) + " steps.\n");

    for (int step = 0; step < steps; ++step) {
        writeBoth(outFile, "\n----- Step " + to_string(step + 1) + " -----\n");

        for (auto& robot : robots) {
            if (!robot.isAlive()) continue;

            writeBoth(outFile, "DEBUG: Robot " + robot.getName() + " starts turn.\n");

            robot.think(outFile);
            robot.look(field, 0, 1, outFile);  // Look right
            robot.fire(field, robots, 0, 1, outFile); // Fire right
            robot.move(field, 0, 1, outFile);  // Move right
            robot.endTurn();

            writeBoth(outFile, "DEBUG: Robot " + robot.getName() + " ends turn.\n");
        }

        // Try to reenter one robot
        for (auto& robot : robots) {
            if (!robot.isAlive() && robot.getLives() > 0 && robot.getReentries() < 3) {
                if (robot.attemptReentry(field, rows, cols, outFile)) {
                    writeBoth(outFile, "DEBUG: " + robot.getName() + " reentered the battlefield.\n");
                    break;
                }
            }
        }

        writeBoth(outFile, "----- Battlefield After Turn " + to_string(step + 1) + " -----\n");
        field.display(outFile);
    }

    outFile.close();
    return 0;
}
