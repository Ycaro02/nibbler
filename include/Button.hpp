#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "BasicDefine.hpp"
#include <string>

/* Forward declaration */
class GraphicLib;

/* Menu class to handle menu */
class Button {
	public:
	/* Canonical form */
	Button();
	~Button();
	Button& operator=(const Button &ref);
	Button(const Button &ref);

	/* Constructor */
	Button(iVec2 start, iVec2 size);
	void drawButton(GraphicLib *lib, void *texture);

	/* Setter */
	void setState(u8 newState);
	void setStart(iVec2 newStart);
	void setSize(iVec2 newSize);
	void setName(std::string newName);
	void setId(u8 newId);

	/* Getter */
	u8 getId() const;
	std::string getName() const;
	iVec2 getStart() const;
	iVec2 getSize() const ;
	u8 getState() const ;


	private:

	iVec2		start;
	iVec2		size;
	u8			id;	
	u8			state;
	std::string name;
};


#endif /* BUTTON_HPP */