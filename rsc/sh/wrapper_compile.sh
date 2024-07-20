#!/bin/bash


# Compile the wrapper lib and run the test

PWD=$(pwd)

source ${PWD}/rsc/sh/color.sh
# Set up environment not mandatory if we use -rpath in the compilation
# export LD_LIBRARY_PATH=$(pwd)/rsc/lib/SFML/build/lib:$LD_LIBRARY_PATH

display_color_msg ${YELLOW} "Compiling the wrapper lib in rsc/wrapperlib"
mkdir -p rsc/wrapperlib

# Check for errors
function check_compile_success {
	local name="${1}"
	if [ $? -ne 0 ]; then
		display_color_msg ${RED} "Compilation ${name} failed"
		exit 1
	else
		display_color_msg ${GREEN} "Compilate ${name} successful"
	fi
}

# Compile the wrapper lib
function wrapper_lib_compile {
	local lib_name="${1}"
	local wrapper_path="${2}"
	local lib_deps="${3}"

	g++ -shared -o rsc/wrapperlib/${lib_name} -fPIC ${wrapper_path} \
		-I./rsc/lib/install/include -L./rsc/lib/install/lib ${lib_deps} \
		-Wl,-rpath,./rsc/lib/install/lib 

	check_compile_success "${lib_name}"
}


wrapper_lib_compile "SFMLWrapper.so" "src/SFML/SF_C_Wrapper.cpp" "-lsfml-graphics -lsfml-window -lsfml-system"
wrapper_lib_compile "SDL2Wrapper.so" "src/SDL2/SDL_C_Wrapper.cpp" "-lSDL2"

# Run the test
./nibbler



# # Compile the wrapper
# g++ -shared -o rsc/wrapperlib/SFMLWrapper.so -fPIC src/SFML/SF_C_Wrapper.cpp \
# 	-I./rsc/lib/SFML/include -L./rsc/lib/SFML/build/lib -lsfml-graphics -lsfml-window -lsfml-system \
# 	-Wl,-rpath,./rsc/lib/SFML/build/lib

# g++ -shared -o rsc/wrapperlib/SDL2Wrapper.so -fPIC src/SDL2/SDL_C_Wrapper.cpp \
# 	-I./rsc/lib/install/include -L./rsc/lib/install/lib -lSDL2 \
# 	-Wl,-rpath,./rsc/lib/install/lib 
