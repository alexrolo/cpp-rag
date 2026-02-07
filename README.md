# RAG Backend

A C++ backend for a local RAG (Retrieval-Augmented Generation) service using local LLM models, embeddings, and vector storage.

## Prerequisites

- C++ compiler (g++ 7.0 or later)
- CMake 3.18 or later
- Python 3.7+ (for Conan package manager)
- Git

### Installing Dependencies on Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake python3 python3-pip python3-venv git
```

## Setting Up Conan with Virtual Environment

Conan is a C++ package manager that handles downloading and building dependencies. Using a Python virtual environment keeps Conan isolated from your system Python packages.

### Step-by-Step Conan Installation

```bash
# 1. Create a Python virtual environment in your project directory
python3 -m venv .venv

# 2. Activate the virtual environment
source .venv/bin/activate

# 3. Upgrade pip to the latest version
pip install --upgrade pip

# 4. Install Conan package manager
pip install conan

# 5. Verify Conan is installed
conan --version

# 6. Create a default Conan profile (detects your compiler and system)
conan profile detect --force
```

**Note:** You need to activate the virtual environment (`source .venv/bin/activate`) each time you open a new terminal session before using Conan commands.

## Adding Dependencies

Dependencies are managed through two files: `conanfile.txt` and `CMakeLists.txt`.

### Example: Adding a JSON Library (nlohmann_json)

#### Step 1: Add to `conanfile.txt`

Edit `conanfile.txt` and add the package to the `[requires]` section:

```ini
[requires]
libcurl/8.6.0
nlohmann_json/3.11.3

[generators]
CMakeToolchain
CMakeDeps

[layout]
cmake_layout
```

**What each section means:**
- `[requires]`: Lists all C++ libraries your project depends on
- `[generators]`: Tells Conan to generate CMake configuration files
- `[layout]`: Uses Conan's cmake_layout for standardized build directory structure

#### Step 2: Update `CMakeLists.txt`

Add the `find_package()` and link the library:

```cmake
cmake_minimum_required(VERSION 3.18)
project(rag LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)  # Add this line

add_executable(rag_app
  src/main.cpp
  src/util/http_client/HttpClient.cpp
)

target_include_directories(rag_app
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(rag_app
  PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json  # Add this line
)
```

#### Step 3: Reinstall Dependencies

```bash
# Clean previous build
rm -rf build

# Install all dependencies (including the new one)
conan install . --output-folder=build --build=missing

# Reconfigure and build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Step 4: Use in Your Code

```cpp
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Create JSON object
json data = {
  {"name", "example"},
  {"value", 42}
};

// Convert to string
std::string json_str = data.dump();
```

### Finding Available Packages

Search for packages in the Conan Center repository:

```bash
# Search for packages
conan search <package_name> --remote=conancenter

# Example: search for JSON libraries
conan search json --remote=conancenter

# Get information about a specific package
conan inspect nlohmann_json/3.11.3
```

Or browse online: https://conan.io/center/

## Building the Program

Here's a detailed explanation of each build step:

### Complete Build Process

```bash
# Ensure virtual environment is active
source .venv/bin/activate

# Step 1: Install dependencies with Conan
conan install . --output-folder=build --build=missing

# Step 2: Configure the project with CMake
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# Step 3: Build the executable
cmake --build build

# Step 4: Run the program
./build/rag_app
```

### What Each Step Does

#### Step 1: `conan install . --output-folder=build --build=missing`

**Purpose:** Downloads and prepares all C++ dependencies.

- **`conan install .`**: Reads `conanfile.txt` in the current directory (`.`) and resolves all dependencies
- **`--output-folder=build`**: Stores all Conan-generated files in the `build/` directory
- **`--build=missing`**: Compiles dependencies from source if precompiled binaries aren't available for your system

**Output:**
- `build/conan_toolchain.cmake`: CMake toolchain file that tells CMake where to find dependencies
- `build/Find*.cmake`: CMake package configuration files for each dependency
- Downloaded libraries (headers and compiled binaries)

**When to run:** 
- First time building the project
- After modifying `conanfile.txt` (adding/removing/updating dependencies)
- After switching Conan profiles or compilers

#### Step 2: `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`

**Purpose:** Configures the build system and generates Makefiles.

- **`-S .`**: Specifies source directory (current directory containing `CMakeLists.txt`)
- **`-B build`**: Specifies build directory where all build artifacts will be placed
- **`-DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake`**: Points CMake to Conan's toolchain file so it can find all dependencies
- **`-DCMAKE_BUILD_TYPE=Release`**: Builds optimized code (alternatives: `Debug`, `RelWithDebInfo`)

**What happens:**
- CMake reads `CMakeLists.txt`
- Detects your compiler
- Finds all dependencies using Conan's generated `.cmake` files
- Generates build files (Makefiles) in the `build/` directory
- Validates that all required libraries are available

**When to run:**
- First time building
- After modifying `CMakeLists.txt`
- After adding/removing source files
- After running `conan install` with new dependencies

#### Step 3: `cmake --build build`

**Purpose:** Compiles the source code and links the executable.

- **`cmake --build build`**: Tells CMake to build everything in the `build/` directory
- CMake automatically uses the appropriate build tool (Make, Ninja, etc.)

**What happens:**
- Compiles each `.cpp` file into object files (`.o`)
- Links object files with dependency libraries
- Produces the final executable: `build/rag_app`

**When to run:**
- After modifying any source code (`.cpp` or `.hpp` files)
- CMake automatically detects which files changed and only recompiles those (incremental builds)

#### Step 4: `./build/rag_app`

**Purpose:** Runs the compiled executable.

- The program is located at `build/rag_app`
- Executes the `main()` function in `src/main.cpp`

### Incremental Builds

After the initial setup, you typically only need to rebuild when you modify source code:

```bash
# After editing .cpp or .hpp files
cmake --build build && ./build/rag_app
```

**Full rebuild (clean slate):**

```bash
rm -rf build
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Build Types

CMake supports different build configurations:

```bash
# Debug build (includes debugging symbols, no optimizations)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake

# Release build (optimized, no debug symbols)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake

# Release with debug info (optimized + debug symbols)
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
```

## Project Structure

```
.
├── CMakeLists.txt           # Build configuration
├── conanfile.txt            # C++ dependencies
├── docker-compose.yml       # LLM, embeddings, and vector DB services
├── README.md                # This file
├── SETUP.md                 # Docker and model setup guide
├── .venv/                   # Python virtual environment (created by you)
├── build/                   # Build artifacts (generated)
├── data/                    # Runtime data
│   └── qdrant/             # Vector database storage
├── models/                  # GGUF model files
│   ├── llm/                # Language model
│   └── embedding/          # Embedding model
└── src/
    ├── main.cpp            # Application entry point
    └── util/
        └── http_client/
            ├── HttpClient.hpp
            └── HttpClient.cpp
```

## Running the Docker Services

See [SETUP.md](SETUP.md) for detailed instructions on setting up and running the LLM, embedding, and vector database services.

Quick start:

```bash
# Start all services
docker compose up -d

# Check service health
curl http://localhost:8080/health  # LLM service
curl http://localhost:8081/health  # Embedding service
curl http://localhost:6333         # Vector database
```

## Common Tasks

### Adding a New Source File

1. Create the file (e.g., `src/utils/logger.cpp`)
2. Update `CMakeLists.txt`:

```cmake
add_executable(rag_app
  src/main.cpp
  src/util/http_client/HttpClient.cpp
  src/utils/logger.cpp  # Add new file
)
```

3. Reconfigure and rebuild:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Updating a Dependency Version

1. Edit `conanfile.txt` with the new version:

```ini
[requires]
libcurl/8.10.0  # Updated version
```

2. Reinstall and rebuild:

```bash
rm -rf build
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Troubleshooting

### "Could not find toolchain file: build/conan_toolchain.cmake"

**Solution:** Run `conan install` first:

```bash
conan install . --output-folder=build --build=missing
```

### "Package 'XYZ' not found"

**Solution:** The package name or version might be incorrect. Search Conan Center:

```bash
conan search XYZ --remote=conancenter
```

### "CMake Error: CMAKE_CXX_COMPILER not set"

**Solution:** Install build tools:

```bash
sudo apt install build-essential
```

### Conan command not found after installation

**Solution:** Activate the virtual environment:

```bash
source .venv/bin/activate
```

### Build fails with linker errors

**Solution:** Clean rebuild:

```bash
rm -rf build
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
