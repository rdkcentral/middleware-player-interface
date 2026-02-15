#include "api_interface.h"
#include <sstream>

// Pure C++ Business Logic Implementation
// No JSON, no HTTP - just pure logic

std::string BusinessLogic::hello(std::string name) {
    return "Hello " + name + "! Welcome to the system.";
}

std::string BusinessLogic::bye(std::string name) {
    return "Goodbye " + name + "! See you later.";
}

int BusinessLogic::calculate(int a, int b) {
    // Complex business logic here
    return a + b + 10; // Some processing
}

std::string BusinessLogic::processUser(std::string username, int age, bool isActive) {
    std::stringstream ss;
    ss << "Processing user: " << username 
       << ", Age: " << age 
       << ", Status: " << (isActive ? "Active" : "Inactive");
    return ss.str();
}

void BusinessLogic::internalHelper() {
    // This won't be exposed as API
}
