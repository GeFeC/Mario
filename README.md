# **Super Mario Boss Worlds**
Fan-made game based on the first Super Mario Bros. game, which extends some mechanics from the original game.
It is only a fan-game! Mario game rights and all the resources, such as textures, used in this project belong to Nintendo! I've made this project only for learning purposes!

## Installation And Building
### Tools needed:
* C++17 Compiler (e.g. g++ or clang)
* [CMake](https://cmake.org/)
* [Git](https://git-scm.com/)

If You're not using an IDE with it's own build system, You will also need some tool to compile Your project (e.g. [GNU Make](https://www.gnu.org/software/make/))<br>
If You are using Linux, most often you can easily install those tools using built-in package manager.

### Building
You can install and build the project using the following commands:
```
git clone --recurse-submodule https://github.com/GeFeC/mario
cd mario
mkdir build
cd build
cmake ..
cmake --build .
```
If CMake doesn't generate the project, you have to specify different [Generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html) after -G flag, so CMake knows which build system it should generate the project for. For example, when You use [GNU Make](https://www.gnu.org/software/make/), the generator You should use is "Unix Makefiles" (on Linux), or "MinGW Makefiles" (on Windows).

And it's done! Have fun playing!
