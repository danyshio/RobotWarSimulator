#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

// Writes message to both the given output stream AND the terminal (cout)
void writeBoth(std::ostream& out, const std::string& message);

#endif
