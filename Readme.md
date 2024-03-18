# p5.cpp Starter Project

This project serves as a starter template for using the p5.cpp library to create simple graphical sketches in C++.

## Introduction

p5.cpp is a C++ library inspired by the p5.js JavaScript library. It provides a simple and intuitive way to create interactive graphics and animations in C++.

## Installation

To use this project, you need to have the following dependencies installed:

- SFML library
- p5.cpp library

### SFML Library

First, you need to install the SFML library. You can do this by following the installation instructions on the SFML website: [SFML Installation Guide](https://www.sfml-dev.org/tutorials/2.6/start-linux.php).

### p5.cpp Library

After installing SFML, you can install the p5.cpp library:

#### Manual Installation

You can manually clone the p5.cpp library from the GitHub repository:

```bash
git clone https://github.com/ArshiaZr/p5.cpp.git
```

#### Automated Installation

Alternatively, you can use the provided `build_p5.sh` script to automate the installation process. First, make sure the script is executable:

```bash
chmod +x build_p5.sh
```

Then, run the script:

```bash
./build_p5.sh
```

This script will clone the p5.cpp repository, build the package, and copy the necessary files to your project directory.

### Building and Running the Project

Once you have installed the SFML and p5.cpp libraries and made a Makefile, you can build and run the project:

```bash
make
./build/app_name
```

Replace `app_name` with the name of your executable.
