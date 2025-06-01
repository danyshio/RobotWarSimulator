#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Battlefield.h"
#include "GenericRobot.h"
#include "RobotUpgrade.h"
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
    vector<GenericRobot*> robots;

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

        //Create specific robot types
        if (type == "HideBot")
            robots.push_back(new HideBot(name, r, c, symbol, 3));
        else if (type == "jumpBot")
            robots.push_back(new jumpBot(name, r, c, symbol));
        else if (type == "LongShotBot")
            robots.push_back(new LongShotBot(name, r, c, symbol));
        else if (type == "SemiAutoBot")
            robots.push_back(new SemiAutoBot(name, r, c, symbol));
        else if (type == "ThirtyShotBot")
            robots.push_back(new ThirtyShotBot(name, r, c, symbol));
        else if (type == "ScoutBot")
            robots.push_back(new ScoutBot(name, r, c, symbol));
        else if (type == "trackBot")
            robots.push_back(new trackBot(name, r, c, symbol));
        else
            robots.push_back(new GenericRobot(name, r, c, symbol));  
    }

    writeBoth(outfile, "----- Initial Battlefield -----");
    field.display(outFile);

    writeBoth(outfile, "DEBUG: Starting simulation with" + to_string(steps) + " steps. \n");

for (int step = 0; step < steps; ++step) {
    writeBoth(outfile, "\n----- Step " + to_sting (step + 1) + " -----\n");
    
    for (auto* robot : robots) {
        if (!robot->isAlive()) continue;

        writeBoth(outFile, "DEBUG: Robot " + robot->getName() + " starts turn.\n");

        robot->think(outFile);
        robot->look(field, 0, 1, outFile);  // Look right
        robot->fire(field, robots, 0, 1, outFile); // Fire right
        robot->move(field, 0, 1, outFile);  // Move right
        robot->endTurn();

        writeBoth(outFile, "DEBUG: Robot " + robot->getName() + " ends turn.\n");
    }

    // Try to reenter one robot
    for (auto* robot : robots) {
        if (!robot->isAlive() && robot->getLives() > 0 && robot->getReentries() < 3) {
            if (robot->attemptReentry(field, rows, cols, outFile)) {
                writeBoth(outFile, "DEBUG: " + robot->getName() + " reentered the battlefield.\n");
                break;
            }
        }
    }

    writeBoth(outFile, "----- Battlefield After Turn " + to_string(step + 1) + " -----\n");
    field.display(outFile);
    }

    for (auto* robot : robots) {
        delete robot;
    }

    outFile.close(); // Close log file
    return 0;
}
