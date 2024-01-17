```markdown
# TCP-ECHO-SERVER

Here's a project of TCP communication for Quick Suite Trading test.

## Table of Contents
- [Introduction](#TCP-ECHO-SERVER)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build and Run](#build-and-run)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Project Structure

```plaintext
your_project_name/
├── CMakeLists.txt
├── include/
│   ├── common.h
│   ├── server_header.h
│   └── client_header.h
├── src/
│   ├── server.cpp
│   └── client.cpp
├── build/
└── README.md
```

- `CMakeLists.txt`: CMake configuration file.
- `include/`: Header files.
- `src/`: Source code files.
- `build/`: Build artifacts directory.

## Prerequisites

- CMake (version 3.15 or higher)
- C++ compiler with C++14 support
- Boost library

## Build and Run

1. **Clone the repository:**

   ```bash
   git clone https://github.com/alyssonfvictoria/TCP-ECHO-SERVER.git
   cd TCP-ECHO-SERVER
   ```

2. **Create build directory:**

   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake:**

   ```bash
   cmake ..
   ```

4. **Build the project:**

   ```bash
   make
   ```

5. **Run the server:**

   ```bash
   ./build/server
   ```

6. **Run the client:**

   ```bash
   ./build/client
   ```

## Usage

Explain how to use your server and client. Include any relevant information regarding input, output, or any additional steps needed.



```
