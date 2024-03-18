#!/bin/bash

# Function to clone the repository and build the package
clone_and_build() {
    # Check if the p5 directory already exists
    if [ -d "./p5" ]; then
        echo "p5 directory already exists. Skipping cloning."
    else
        # Clone the repository
        echo "Cloning the repository..."
        git clone https://github.com/ArshiaZr/p5.cpp.git ./p5 > /dev/null 2>&1
        echo "Cloned the repository."
    fi

    # Navigate to the repository directory
    cd p5

    # Build the package
    echo "Building the package..."
    make package > /dev/null 2>&1
    echo "Package built."

    # Navigate back to the parent directory
    cd ..

    # Copy include directory from the package if it exists
    if [ -d "./p5/package/include" ]; then
        echo "Copying include directory..."
        cp -r ./p5/package/include .
        echo "Include directory copied."
    else
        echo "Include directory not found in the package. Aborting."
        exit 1
    fi

    # Create a directory for libraries
    mkdir -p lib

    # Copy the static library if it exists
    if [ -f "./p5/package/libp5.a" ]; then
        echo "Copying static library..."
        cp ./p5/package/libp5.a ./lib
        echo "Static library copied."
    else
        echo "Static library not found in the package. Aborting."
        exit 1
    fi

    # Clean up the cloned repository
    echo "Cleaning up..."
    rm -rf p5
}

# Function to create a Makefile
create_makefile() {
    cat <<EOF > Makefile
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
EXECUTABLE := \$(BUILD_DIR)/app_name 
STATIC_LIB := ./lib/libp5.a  # Path to libp5.a

# SFML configuration
SFML_DIR := /path/to/sfml/libary
SFML_INCLUDE := /path/to/sfml/libary/include  # Path to SFML include directory
SFML_LIB := /path/to/sfml/libary/lib  # Path to SFML lib directory
LIBS := -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS := -L\$(SFML_LIB)
CXXFLAGS += -I\$(SFML_INCLUDE) -I\$(INC_DIR) -I\$(INC_DIR)/p5 # Include the SFML include directory

\$(EXECUTABLE): \$(SRC_DIR)/main.cpp \$(STATIC_LIB)
	@mkdir -p \$(BUILD_DIR)
	\$(CXX) \$(CXXFLAGS) \$< -o \$@ -L\$(dir \$(STATIC_LIB)) -lp5 \$(LDFLAGS) \$(LIBS)

clean:
	rm -rf \$(BUILD_DIR)
EOF

    echo "Makefile created successfully."
}

# Clone the repository and build the package
clone_and_build

# Ask the user if they want to create a Makefile
read -p "Would you like to create a Makefile? (yes/no): " choice
case "$choice" in
  yes|YES|Yes ) 
    create_makefile
    echo "change the sfml library path in the Makefile and run 'make' to build the project."
    ;;
  * )
    echo "Exiting without creating a Makefile."
    echo "Add include and lib directories to your project and link the static library to use p5."
    ;;
esac

