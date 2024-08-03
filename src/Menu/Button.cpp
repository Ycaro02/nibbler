#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"

/* Default constructor */
Button::Button() {
	start = {0, 0};
	size = {0, 0};
	state = BTN_UNPRESS;
	id = 0;
	name = "None";
}

/* Destructor */
Button::~Button() {
}

/* Assignment operator */
Button& Button::operator=(const Button &ref) {
	if (this != &ref) {
		start = ref.start;
		size = ref.size;
		state = ref.state;
		id = ref.id;
		name = ref.name;
	}
	return (*this);
}

/* Copy constructor */
Button::Button(const Button &ref) {
	*this = ref;
}

/**
 * @brief Constructor
 * @startBtn Start position of the button
 * @sizeBtn Size of the button
 */
Button::Button(iVec2 startBtn, iVec2 sizeBtn) {
	start = startBtn;
	size = sizeBtn;
	state = BTN_UNPRESS;
	id = 0;
	name = "None";
}

/**
 * @brief Draw the button
 * @lib GraphicLib pointer
 * @texture Texture of the button
*/
void Button::drawButton(GraphicLib *lib, void *texture, u32 color) {
	iVec2	nameStart;
	s32 	fontMult = lib->getFontMult();
	s32 	fontSize = lib->getFontSize();

	lib->drawTextureTile(texture, start, size);
	nameStart.x = start.x + (size.x >> 2);
	nameStart.y = start.y;
	if (fontSize + fontMult < size.y) {
		nameStart.y += (size.y >> 3); // / 8
	}
	lib->writeText(name.c_str(), nameStart, lib->getFontSize(), color);
}

/* Setter */
void Button::setState(u8 newState) {
	state = newState;
}

void Button::setStart(iVec2 newStart) {
	start = newStart;
}

void Button::setSize(iVec2 newSize) {
	size = newSize;
}

void Button::setName(std::string newName) {
	name = newName;
}
void Button::setId(u8 newId) {
	id = newId;
}

/* Getter */
u8 Button::getId() const {
	return (id);
}

std::string Button::getName() const {
	return (name);
}

u8 Button::getState() const {
	return (state);
}


iVec2 Button::getStart() const {
	return (start);
}

iVec2 Button::getSize() const {
	return (size);
}