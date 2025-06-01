#include "RobotUpgrade.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

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

    vector<GenericRobot*> robots;

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

     for (auto robot : robots) {
        delete robot;
     }   

    cout << "----- Initial Battlefield -----" << endl;
    field.print();

    // Simulate one step
    cout << "\n----- Robot Turn -----" << endl;
    for (auto& robot : robots) {
        robot->think();
        robot->look(field, 0, 1);  // Look right
        robot->move(field, 0, 1);  // Try to move right
        robot->fire(field, 1, 0);  // Try to fire downward
        robot->endTurn();
        cout << endl;
    }

    cout << "----- Battlefield After Turn -----" << endl;
    field.print();

    return 0;
}