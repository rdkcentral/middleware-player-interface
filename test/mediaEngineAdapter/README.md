# API Middleware Generator

**Automatic HTTP API Layer Generator for C++ Business Logic**

## Project Structure

```
api_generator/
├── api_interface.h       # Developer writes this (public APIs)
├── business_logic.cpp    # Developer writes this (pure C++ logic)
├── api_generator.py      # Auto-generates HTTP layer
├── api_server.cpp        # AUTO-GENERATED - Don't edit!
└── CMakeLists.txt        # build system
```

## Quick Start

### Step 1: Define Your API Interface

```cpp
// api_interface.h
#include <string>

#define MIDDLEWARE_API

class BusinessLogic {
public:
    MIDDLEWARE_API std::string hello(std::string name);
    MIDDLEWARE_API int calculate(int a, int b);
};
```

### Step 2: Implement Business Logic

```cpp
// business_logic.cpp
#include "api_interface.h"

std::string BusinessLogic::hello(std::string name) {
    return "Hello " + name + "!";
}

int BusinessLogic::calculate(int a, int b) {
    return a + b;
}
```

### Step 3: Build & Run

```bash
# Generate + Compile
make

# Run server
./server

# Or in one command
make run
```

### Step 4: Test APIs (from any UI)

**cURL:**
```bash
# Test all endpoints quickly
curl -X POST http://localhost:8080/hello -H "Content-Type: application/json" -d '{"name":"thisisparam"}'
curl -X POST http://localhost:8080/calculate -H "Content-Type: application/json" -d '{"a":10,"b":20}'
curl -X POST http://localhost:8080/bye -H "Content-Type: application/json" -d '{"name":"ThisIsParam"}'
```

**JavaScript:**
```javascript
fetch('http://localhost:8080/hello', {
    method: 'POST',
    body: JSON.stringify({name: 'parameter'})
}).then(r => r.json()).then(console.log);
```

**Flutter:**
```dart
final response = await http.post(
  Uri.parse('http://localhost:8080/hello'),
  body: jsonEncode({'name': 'parameter'}),
);
```

## Supported Types

- `int`, `float`, `double`, `bool`
- `std::string`, `string`
- Multiple parameters
- `void` return type

##  How It Works

```
1. Developer: Write api_interface.h (mark functions with MIDDLEWARE_API)
2. Developer: Write business_logic.cpp (pure C++ implementation)
3. Script: Parse interface → Auto-generate api_server.cpp
4. Build: Compile everything → Ready to use!
```

## Dependencies

```bash
# Install Crow (header-only HTTP library)
sudo apt-get install libcrow-dev

# Or download manually
git clone https://github.com/CrowCpp/Crow.git
```

## Example Output

```json
// Success response
{
  "status": "success",
  "message": "Hello Tanuj!"
}

// Error response
{
  "status": "error",
  "message": "Parameter 'name' is missing"
}
```

## Workflow

```
Developer adds new API:
  1. Add function to api_interface.h with MIDDLEWARE_API
  2. Implement in business_logic.cpp
  3. Run: make
  4. Done! New endpoint automatically available
```

## Debugging

```bash
# Clean build
make clean

# Regenerate only
make generate

# View generated code
cat api_server.cpp
```

## Notes

- Only functions marked with `MIDDLEWARE_API` are exposed
- Private functions remain private (not exposed as APIs)
- Generated file has "DO NOT EDIT" warning
- All APIs are POST endpoints on port 8080

