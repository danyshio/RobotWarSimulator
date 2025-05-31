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
    srand(time(0));

    ifstream inFile("input.txt");
    if (!inFile) {
        cerr << "Failed to open input.txt" << endl;
        return 1;
    }

    string line;
    int rows = 0, cols = 0, steps = 0, robotCount = 0;

    getline(inFile, line);
    istringstream dimStream(line.substr(line.find(':') + 1));
    dimStream >> rows >> cols;

    getline(inFile, line);
    steps = stoi(line.substr(line.find(':') + 1));

    getline(inFile, line);
    robotCount = stoi(line.substr(line.find(':') + 1));

    Battlefield field(rows, cols);
    vector<GenericRobot> robots;

    for (int i = 0; i < robotCount; ++i) {
        getline(inFile, line);
        istringstream robotStream(line);
        string type, name, rowStr, colStr;
        robotStream >> type >> name >> rowStr >> colStr;

        int r = (rowStr == "random") ? rand() % rows : stoi(rowStr);
        int c = (colStr == "random") ? rand() % cols : stoi(colStr);

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
    field.print();

    cout << "\n----- Robot Turn -----" << endl;
    for (auto& robot : robots) {
        if (!robot.isAlive()) continue;
        robot.think();
        robot.look(field, 0, 1);
        robot.move(field, 0, 1);
        robot.fire(field, robots, 1, 0);
        robot.endTurn();
        cout << endl;
    }

    cout << "----- Battlefield After Turn -----" << endl;
    field.print();

    return 0;
}
