#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <utility>
#include <stdexcept>

using std::vector;
using std::pair;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;

const std::string userNameColor = "\033[93m";  // Light yellow
const std::string directoryColor = "\033[95m";  // Pink
const std::string branchColor = "\033[92m";  // Light green
const std::string resetColor = "\033[0m";  // Reset color