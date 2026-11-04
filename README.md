## Project README

### Overview
This project contains a custom compiler and related tools designed to handle specific language features, particularly focused on structure handling and data reference management. The core functionality revolves around parsing and interpreting code written in custom languages like `.rex`, `.ll`, and `.omml`.

### Features
- **Structure Handling**: Support for defining and managing structures with custom operators.
- **Data Reference Management (DREF)**: Capabilities to handle and manipulate data references efficiently.
- **Custom Operators**: Define and manage operators for various operations, including initialization, assignment, size queries, and more.

### Project Structure
The project is organized as follows:

#### Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- No additional libraries are needed for this basic setup.

#### Build & Run
To build the project, navigate to the root directory and use the appropriate `Makefile` based on your operating system. For example:

```bash
# For Linux
make -f Makefile.linux all

# For Windows
make -f Makefile.windows all

# For Wine (Linux cross-compile for Windows)
make -f Makefile.wine all

# For WebAssembly (Emscripten or wasmtime)
make -f Makefile.web all
```

To clean and rebuild, use:

```bash
make -f Makefile.(os) clean
make -f Makefile.(os) all
```

#### Build Options
- `make -f Makefile.(os) all`: Builds the project without executing it.
- `make -f Makefile.(os) do`: Builds and executes the project.
- `make -f Makefile.(os) clean`: Removes build artifacts to start fresh.

To execute the built executable:

```bash
make -f Makefile.(os) exe
```

This README provides a structured overview of the project, its features, and how to build and run it.