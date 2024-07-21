#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "AGraphicLib.hpp"

typedef unsigned char u8;

class Nibbler {
	public:
	/* Canonical form */
	Nibbler();
	~Nibbler();
	Nibbler& operator=(const Nibbler &ref);
	Nibbler(const Nibbler &ref);

	private:

	AGraphicLib			*libs[2];
	int					width;
	int					height;
	u8					**board;	
};

#endif /* NIBBLER_HPP */