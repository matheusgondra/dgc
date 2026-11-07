# dgc - C23 Project Generator

`dgc` is a modern, lightweight command-line interface (CLI) tool written in **C (C23)** designed to scaffold new C projects configured with **CMake**, **Ninja**, and **vcpkg**.

## Features

- **Modern C23 Standard**: Built using contemporary C features, including `#embed` to embed text templates directly into the binary at compile time (no runtime file dependencies).
- **Batteries Included**: Generates a complete project layout with source files, headers, presets and ignore rules.
- **CMake Presets**: Standardized build configurations out of the box.
- **vcpkg Integration**: Ready for dependency management using manifest mode (`vcpkg.json`).
- **Clean UI**: Terminal outputs styled with ANSI colors and a clean directory tree preview.

## Prerequisites

Make sure you have the following tools installed on your system:
- **CMake** (version 3.25 or higher)
- **Ninja** (build system)
- **vcpkg** (with the `VCPKG_ROOT` environment variable configured)
- A modern C23 compatible compiler (such as **GCC 14+** or **Clang 18+**)

## Building and Installing

### 1. Clone and Configure
```bash
git clone https://github.com/matheusgondra/dgc.git
cd dgc
cmake --preset default
```

### 2. Build the Binary
```bash
cmake --build --preset default
```

### 3. Install Globally (Optional)
To make `dgc` available globally in your system's `PATH`:
```bash
sudo cmake --install build
```

## Usage

Generate a new C23 project by running:
```bash
dgc <project_name>
```

### Options
- `-h`, `--help`: Display the help message and exit.
- `-v`, `--version`: Output version information and exit.

## License
This project is open-source and available under the MIT License.