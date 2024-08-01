#!/bin/bash

# Create a C++ header file with the given name, name_HPP is used as the include guard

if [ $# -eq 0 ]; then
	echo "Usage: $0 <header_name>"
	exit 1
fi

header_name=$1

header_file="${header_name}.hpp"

if [ -f $header_file ]; then
	echo "File $header_file already exists"
	exit 1
fi

# Basic guard
echo "#ifndef ${header_name^^}_HPP" > $header_file
echo "#define ${header_name^^}_HPP" >> $header_file
echo "" >> $header_file

# Class definition
echo "class ${header_name} {" >> $header_file

# Canonical class definition
echo "	public:" >> $header_file
echo "		${header_name}();" >> $header_file
echo "		~${header_name}();" >> $header_file
echo "		${header_name}(const ${header_name}& ref);" >> $header_file
echo "		${header_name}& operator=(const ${header_name}& ref);" >> $header_file
echo "" >> $header_file
echo "};" >> $header_file
echo "" >> $header_file
echo "#endif /* ${header_name^^}_HPP */ " >> $header_file


# Create the source file

cat << EOF > ${header_name}.cpp
#include "../include/${header_name}.hpp"

${header_name}::${header_name}() {
}

${header_name}::~${header_name}() {
}

${header_name}::${header_name}(const ${header_name}& ref) {
}

${header_name}& ${header_name}::operator=(const ${header_name}& ref) {
	return (*this);
}

EOF
