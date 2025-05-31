#ifndef Robot_Upgrade_H
#define Robot_Upgrade_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

//class declaration
class Hidebot;
class jumpBot;
class trackBot;
class ScoutBot;
class LongShotbot;
class SemiAutoBot;
class ThirtyShotBot;

class GenericRobot{
    protected:
        string name;
        int row, col;
        char symbol;
        bool hasThought;
        unordered_set<string> upgradesTaken;

        bool isAdjacent(int dx, int dy) const {
        return abs(dx) <= 1 && abs(dy) <= 1 && !(dx == 0 && dy == 0);
    }

    public:
        GenericRobot(string n, int r, int c, char sym) : name(n), row(r), col(c), symbol(sym), hasThought(false) {}

        virtual ~GenericRobot() {}

        string getName() const {return name;}
        int getRow() const {return row;}
        int getCol() const {return symbol;}
        char getSymbol() const {return symbol;}

        //Upgrade tracking
        bool hasUpgrade(const string& category) const {
            return upgradesTaken.count(category) > 0;
        }
        void addUpgrade(const string& category) {
            upgradesTaken.insert(category);
        }

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
        int jumpCharges = 3;
        bool hasJumpedThisTurn = false;

    public:
        jumpBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

        void jump(Battlefield& field, int newR, int newC) {
            if (!hasThought) {
                cout << getName() << "is thinking before jumping" << endl;
                return;
            } if (hasJumpedThisTurn) {
                cout << getName() << "has already jumped this turn" << endl;
                return;
            } if (jumpCharges <= 0) {
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

        int getJumpCharges() const{
            return jumpCharges;
        }

};

class LongShotBot : public GenericRobot {
private:
    const int maxRange = 3;

public:
    LongShotBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void fire(Battlefield& field, int dx, int dy) override {
        if(!hasThought) {
            cout << name << "thinking before firing" << endl;
            return;
        }
        int distance = abs(dx) + abs(dy);
        if (distance > maxRange) {
            cout << name << "can't fire beyond range of " << maxRange << endl;
            return;
        }

        int targetR = row +dx;
        int targetC = col + dy;
        if (field.fireAt(targetR, targetC)) {
            cout << name << "target hit at (" << targetR << "," << targetC << ") from distance" << distance << endl;
        } else {
            cout << name << "missed the target " << endl;
        }
    }

};

class SemiAutoBot : public GenericRobot {
    public:
        SemiAutoBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

        void fire(Battlefield& field, int dx, int dy) override {
            if (!hasThought) {
                cout << name << "thinking before firring" << endl;
                return;
            }

            int targetR = row +dx;
            int targetC = col + dy;
            cout << name << "firing 3 round burst" << endl;

            for(int i = 0; i < 3; i++) {
                if (rand() % 100 < 70) {
                    if (field.fireAt(targetR, targetC)) {
                        cout << name << "Round " << (i + 1) << " hit" << endl;
                    } else {
                        cout << name << "Round " << (i + 1) << " missed target" << endl;
                    } 
                }
            }
        }
};

class ThirtyShotBot : public GenericRobot {
    private:
        int ammo = 30;

    public:
    ThirtyShotBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void fire(Battlefield& field, int dx, int dy) override {
        if(!hasThought) {
            cout << name << "thinking before firing" << endl;
            return;
        }
        if (ammo <= 0) {
            cout << name << " has no ammo left" << endl;
            return;
        }
        ammo--;
        GenericRobot::fire(field, dx ,dy);
        cout << name << "has" << ammo << "rounds left" << endl;
    }

    void reload() {
        ammo = 30;
        cout << name << "has reload" << endl;
    }
};

class ScoutBot : public GenericRobot {
    private:
        int scanCharges = 3;
    
    public:
    ScoutBot(string n, int r, int c, char sym) : GenericRobot(n, r, c, sym) {}

    void scanBattlefield(Battlefield& field) {
        if(!hasThought) {
            cout << name << "thinking before scanning" << endl;
            return;
        }
        if(scanCharges <= 0) {
            cout << name << "has no scan charges left" << endl;
            return;
        }
        cout << name << "scanning battlefield:" << endl;
        for (int r = 0; r < field.getRows(); r++) {
            for (int c = 0; c < field.getCols(); c++) {
                char cell = field.getAt(r , c);
                if (cell != ',') {
                    cout << "spotted" << cell << " at (" << r << "," << c << ")" << endl;
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
        
        void plantTracker(Battlefield& field, char targetSymbol) {
            if (!hasThought) {
                cout << name << "thinking before panting tracker" << endl;
                return;
            }
            if (trackCharges <= 0) {
                cout << name << "has no track charges left" << endl;
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
                cout << name << "is now tracking" << targetSymbol << endl;
            } else {
                cout << "Target" << targetSymbol << "not found" << endl;
            }
        }

        void showTrackedTargets(Battlefield& field) const {
            cout << name << " is tracking: " <<endl;
            for (char target : trackedTargets) {
                for (int r = 0; r < field.getRows(); r++) {
                    for (int c = 0; c < field.getCols(); c++) {
                        if (field.getAt(r, c) == target) {
                            cout << target << "at" << r << "," << c << endl;
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
    void grantUpgrade(GenericRobot* robot, int killCount) {
        if (killCount < 1) return;

        string category = getUpgradeCategory(robot);
        if (category == "Unknown") return;
        if (robot ->hasUpgrade(category)) return;

        robot->addUpgrade(category);
        cout << robot->getName() << "has been granted" << category << "upgrade" << endl;
    }
};

#endif
