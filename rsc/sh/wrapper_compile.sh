#!/bin/bash

# Set up environment
# export LD_LIBRARY_PATH=$(pwd)/rsc/lib/SFML/build/lib:$LD_LIBRARY_PATH

mkdir -p rsc/wrapperlib

# Compile the wrapper
g++ -shared -o rsc/wrapperlib/SFMLWrapper.so -fPIC src/SFML/SF_C_Wrapper.cpp \
	-I./rsc/lib/SFML/include -L./rsc/lib/SFML/build/lib -lsfml-graphics -lsfml-window -lsfml-system \
	-Wl,-rpath,./rsc/lib/SFML/build/lib

# Check for errors
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

# Run the test
./nibbler

