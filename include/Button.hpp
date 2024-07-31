#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "BasicDefine.hpp"

#define BTN_UNPRESS		0U
#define BTN_PRESSED		1U

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

	/* Getter */
	u8 getState() const;
	iVec2 getStart() const;
	iVec2 getSize() const;

	void drawButton(GraphicLib *lib, void *texture) const;

	/* Setter */
	void setState(u8 newState);
	void setStart(iVec2 newStart);
	void setSize(iVec2 newSize);

	private:

	iVec2	start;
	iVec2	size;	
	u8		state;
};


#endif /* BUTTON_HPP */