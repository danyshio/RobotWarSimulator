#include "Utils.h"
#include <fstream>
#include <iostream>

void writeBoth(std::ostream& out, const std::string& message) {
    out << message;
    std::cout << message;
}
