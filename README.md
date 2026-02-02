## Dependencies

This project uses CMake, as well as a working ARM toolchain.

Ensure you have the following installed before proceeding:

- Git
- CMake with version 3.27 or higher
- A working ARM GCC compiler
- The ARM newlib library for the installed ARM GCC compiler

On Arch Linux, this corresponds to the `git`, `cmake`, `arm-none-eabi-gcc`, and `arm-none-eabi-newlib` packages.

On other platforms, see the [official Pico SDK documentation](https://pip-assets.raspberrypi.com/categories/610-raspberry-pi-pico/documents/RP-008276-DS-1-getting-started-with-pico.pdf) for instructions on preparing a build environment.

## Setup

Before building the project, it is required to initialise the submodules.

To do this, simply run the following git command in the repository root.

```sh
git submodule update --init --recursive
```

After having successfully initialised the submodules, the project is ready for building.

## Build

To generate the files needed to start a build, run the following command.

```sh
cmake -B build
```

These generated files can then be used to build the project.

```sh
cmake --build build -j16
```

Finally, flash the PI PICO by mounting it in BOOTSEL mode and copying the file located at `build/main.uf2`.
