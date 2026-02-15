#include "api_interface.h"
#include <fcntl.h>      // For open(), O_WRONLY
#include <unistd.h>     // For dup2(), close()
#include <cstdio>
#include "InterfacePlayerRDK.h"
#include <sstream>

// Pure C++ Business Logic Implementation
// No JSON, no HTTP - just pure logic

BusinessLogic::BusinessLogic()
{
	freopen("/tmp/player_if.log", "w", stdout);
	std:: string debugLevel = "3";
	playerInstance->EnableGstDebugLogging(std::move(debugLevel));
	std::cout<<"\nthis is the log\n";
}
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

void BusinessLogic::SetPlayerName(std::string playerName)
{
	int fd = open("/dev/tty", O_WRONLY);
    const char* test = "[DEBUG] SetPlayerName called\n";
    write(fd, test, strlen(test));
    close(fd);
	playerInstance->SetPlayerName(playerName);
}
void BusinessLogic::internalHelper() {
    // This won't be exposed as API
}
