# dgc - C23 Project Generator

[Português (PT-BR)](README-pt-br.md) | **English**

`dgc` is a modern, lightweight command-line interface (CLI) tool written in **C (C23)** designed to scaffold new C projects configured with **CMake**, **Ninja**, and **vcpkg**.

## Features

- **Modern C23 Standard**: Built using contemporary C features, including `#embed` to embed text templates directly into the binary at compile time (no runtime file dependencies).
- **Modular Command Architecture**: Centralized command registry and dedicated option parsing to make adding new commands and flags straightforward.
- **Batteries Included**: Generates a complete project layout with source files, headers, presets, and ignore rules.
- **CMake Presets**: Standardized build configurations out of the box.
- **vcpkg Integration**: Ready for dependency management using manifest mode (`vcpkg.json`).
- **Clean UI**: Terminal outputs styled with ANSI colors and a clean directory tree preview.

## Prerequisites

Make sure you have the following tools installed on your system:
- **CMake** (version 3.20 or higher)
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

### Available CMake Presets

The project provides presets for the default configuration and explicit GCC or Clang builds:

| Preset | Compiler | Configure command | Build command |
| --- | --- | --- | --- |
| `default` | GCC | `cmake --preset default` | `cmake --build --preset default` |
| `gcc` | GCC | `cmake --preset gcc` | `cmake --build --preset gcc` |
| `clang` | Clang | `cmake --preset clang` | `cmake --build --preset clang` |

### 3. Install Globally (Optional)
To make `dgc` available globally in your system's `PATH`:
```bash
sudo cmake --install build
```

## Usage

Generate a new C23 project using the `new` subcommand:
```bash
dgc new [OPTIONS] <project_name>
```

Example:
```bash
dgc new my_awesome_project
```

### Commands
- `new [OPTIONS] <project_name>`: Create a new C23 project.

### `new` Command Options
- `-f`, `--force`: Allow an existing project directory.
- `-h`, `--help`: Display help for the `new` command and exit.

### Global Options
- `-h`, `--help`: Display global help and exit.
- `-v`, `--version`: Output version information and exit.

## License
This project is open-source and available under the MIT License.