#ifndef Robot_Upgrade_H
#define Robot_Upgrade_H

#include "Battlefield.h"
#include "GenericRobot.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

//class declaration
class HideBot;
class jumpBot;
class trackBot;
class ScoutBot;
class LongShotBot;
class SemiAutoBot;
class ThirtyShotBot;


inline string getUpgradeCategory(GenericRobot* robot) {
    if (dynamic_cast<HideBot*>(robot) || dynamic_cast<jumpBot*>(robot))
        return "Movement";
    if (dynamic_cast<trackBot*>(robot) || dynamic_cast<ScoutBot*>(robot))
        return "Seeing";
    if (dynamic_cast<LongShotBot*>(robot) || dynamic_cast<SemiAutoBot*>(robot) || dynamic_cast<ThirtyShotBot*>(robot))
        return "Shooting";
    return "Unknown";
}

class HideBot : public GenericRobot {

    private:
        int hideCharges;
        bool isHidden;

    public:
        HideBot(string n, int r, int c, char sym, int charges) :
            GenericRobot(n, r, c, sym), hideCharges(charges), isHidden(false) {}

        void activeHide(ostream& out) {
            if (hideCharges > 0 && !isHidden) {
                isHidden = true;
                hideCharges--;
                out << getName() << "is now hidden (" << hideCharges << " charges left)" << endl;
            }   else if (isHidden) {
                out << getName() << "is already hidden" << endl;
            }   else{
                out << getName() << "has no hide charges left" << endl;
            }
        }

        void deactiveHide(ostream& out) {
            if (isHidden) {
                isHidden = false;
                out << getName() << "is now can be seen" << endl;
            }
        }

        bool isCurrentlyHidden() const {
            return isHidden;
        }

        void fire(Battlefield& field, std::vector<GenericRobot*>& robots, int dx, int dy, ostream& out) override {
            if (isHidden){
                out << getName() << "cannot fire while hidden" << endl;
                return;
            }

            int targetR = getRow() + dx;
            int targetC = getCol() +dy;

            for (auto& robot : robots) {
                if (robot->getRow() == targetR && robot->getCol() == targetC) {
                    if (robot->isCurrentlyHidden()) {
                        out << robot->getName() << "is hidden and cannot be hit" << std::endl;
                        return; 
                    } else {
                        out << getName() << "fires at " << robot->getName() << std::endl;
                        robot->takeDamage(1);
                        return;
                    }
                }
            }
            out << getName() << "fires at empty space" << std::endl;
        }

        void takeDamage(int amount) override {
            if (isHidden) {
                std::cout << getName() << "is hidden and cannot take damage" << std::endl;
                return;
            }
            GenericRobot::takeDamage(amount);
        }

};

class jumpBot : public GenericRobot {
    private:
        int jumpCharges = 3;
        bool hasJumpedThisTurn = false;

    public:
        jumpBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

        void jump(Battlefield& field, int newR, int newC, ostream& out) {
            if (!hasThought) {
                out << getName() << "is thinking before jumping" << endl;
                return;
            } if (hasJumpedThisTurn) {
                out << getName() << "has already jumped this turn" << endl;
                return;
            } if (jumpCharges <= 0) {
                out << getName() << "has no jump charges" << endl;
                return;
            }
            if (newR < 0 || newR >= field.getRows() || newC < 0 || newC >= field.getCols()) {
                out << getName() << "cannot jump out of Battlefield" << endl;
                return;
            }

            if (field.getAt(newR, newC) != '.') {
                out << getName() << "the position is occupied" << endl;
                return;
            }

            field.moveRobot(getRow(), getCol(), newR, newC, getSymbol());
            row = newR;
            col = newC;
            jumpCharges--;
            hasJumpedThisTurn = true;

            out << getName() << "jumped to (" << newR << "," << newC << ")" << endl;
            out << jumpCharges << " jump charges left" << endl;
        }

        void endTurn() override {
            GenericRobot::endTurn();
            hasJumpedThisTurn = false;
        }

        int getJumpCharges() const{
            return jumpCharges;
        }

};

class LongShotBot : public GenericRobot {
private:
    const int maxRange = 3;

public:
    LongShotBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void fire(Battlefield& field, std::vector<GenericRobot*>& robots, int dx, int dy, ostream& out) override {
        if(!hasThought) {
            out << getName << "thinking before firing" << endl;
            return;
        }
        int distance = abs(dx) + abs(dy);
        if (distance > maxRange) {
            out << getName << "can't fire beyond range of " << maxRange << endl;
            return;
        }

        int targetR = getRow() +dx;
        int targetC = getCol() + dy;
        for (auto& robot : robots) {
            if (robot->getRow() == targetR && robot->getCol() == targetC) {
                out << robot->getName() << "is hidden and cannot be hit" << endl;
                return;
            } else {
                out << getName() << "fires at" << robot->getName() << endl;
                robot->takeDamage(1);
                return;
            }
        }
        out << getName() << "missed the target" << endl;
    }

};

class SemiAutoBot : public GenericRobot {
    public:
        SemiAutoBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

        void fire(Battlefield& field,std::vector<GenericRobot*>& robots, int dx, int dy, ostream& out) override {
            if (!hasThought) {
                out << getName << "thinking before firring" << endl;
                return;
            }

            int targetR = row +dx;
            int targetC = col + dy;
            out << getName << "firing 3 round burst" << endl;

            for(int i = 0; i < 3; i++) {
                bool hit (rand() % 100 < 70);
                if (hit) {
                    for (auto& robot : robots) {
                        if (robot->getRow() == targetR && robot->getCol() == targetC) {
                            if (robot->isCurrentlyHidden()) {
                                out << robot->getName() << "is hidden and cannot be hit" << endl;
                                break;
                            } else {
                                out << getName() << "Round" << (i + 1) << "hit" << robot->getName() << endl;
                                robot->takeDamage(1);
                                break;
                            }
                        } 
                    }
                } else{
                    out << getName() << "Round" << (i + 1) << "missed the terget" << endl;
                }
            }
        }
};

class ThirtyShotBot : public GenericRobot {
    private:
        int ammo = 30;

    public:
    ThirtyShotBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void fire(Battlefield& field,std::vector<GenericRobot*>& robots, int dx, int dy, ostream& out) override {
        if(!hasThought) {
            out << getName << "thinking before firing" << endl;
            return;
        }
        if (ammo <= 0) {
            out << getName << " has no ammo left" << endl;
            return;
        }
        ammo--;
        GenericRobot::fire(field, robots, dx ,dy, out);
        out << getName << "has" << ammo << "rounds left" << endl;
    }

    void reload() {
        ammo = 30;
        std::cout << getName << "has reload" << endl;
    }
};

class ScoutBot : public GenericRobot {
    private:
        int scanCharges = 3;

    public:
    ScoutBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void scanBattlefield(Battlefield& field, ostream& out) {
        if(!hasThought) {
            out << getName() << "thinking before scanning" << endl;
            return;
        }
        if(scanCharges <= 0) {
            out << getName() << "has no scan charges left" << endl;
            return;
        }
        out << getName() << "scanning battlefield:" << endl;
        for (int r = 0; r < field.getRows(); r++) {
            for (int c = 0; c < field.getCols(); c++) {
                char cell = field.getAt(r , c);
                if (cell != ',') {
                    out << "spotted" << cell << " at (" << r << "," << c << ")" << endl;
                }
            }
        }
        scanCharges--;
    }
};

class trackBot : public GenericRobot {
    private:
        int trackCharges = 3;
        vector<char> trackedTargets;

    public:
        trackBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

        void plantTracker(Battlefield& field, char targetSymbol, ostream& out) {
            if (!hasThought) {
                out << getName() << "thinking before panting tracker" << endl;
                return;
            }
            if (trackCharges <= 0) {
                out << getName() << "has no track charges left" << endl;
            }return;

            bool targetFound  = false;
            for (int r = 0 ; r < field.getRows(); r++ ) {
                for (int c = 0; c < field.getCols(); c++) {
                    if (field.getAt(r, c) == targetSymbol) {
                        targetFound = true;
                        break;
                    }
                }
            }
            if (targetFound) {
                trackedTargets.push_back(targetSymbol);
                trackCharges--;
                out << getName() << "is now tracking" << targetSymbol << endl;
            } else {
                out << "Target" << targetSymbol << "not found" << endl;
            }
        }

        void showTrackedTargets(Battlefield& field, ostream& out) const {
            out << getName() << " is tracking: " <<endl;
            for (char target : trackedTargets) {
                for (int r = 0; r < field.getRows(); r++) {
                    for (int c = 0; c < field.getCols(); c++) {
                        if (field.getAt(r, c) == target) {
                            std::cout << target << "at" << r << "," << c << endl;
                        }
                    }
                }
            }
        }
};

class upgradeManager {
    private:
        unordered_set<string> movementUpgradesTaken;
        unordered_set<string> shootingUpgradesTaken;
        unordered_set<string> seeingUpgradesTaken;
    public:
    void grantUpgrade(GenericRobot* robot, int killCount, ostream& out) {
        if (killCount < 1) return;

        string category = getUpgradeCategory(robot);
        if (category == "Unknown") return;
        out << robot->getName() << "has been granted" << category << "upgrade" << endl;
    }
};

#endif
