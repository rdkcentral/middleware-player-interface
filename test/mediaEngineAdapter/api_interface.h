#ifndef API_INTERFACE_H
#define API_INTERFACE_H

#include <string>

// MIDDLEWARE_API macro marks functions to be exposed as HTTP endpoints
#define MIDDLEWARE_API

class BusinessLogic {
public:
    // Developer defines only these public APIs
    MIDDLEWARE_API std::string hello(std::string name);
    MIDDLEWARE_API std::string bye(std::string name);
    MIDDLEWARE_API int calculate(int a, int b);
    MIDDLEWARE_API std::string processUser(std::string username, int age, bool isActive);
    
private:
    // Private functions won't be exposed
    void internalHelper();
};

#endif
