#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"

Button::Button() {
	start = {0, 0};
	size = {0, 0};
	state = BTN_UNPRESS;
	id = 0;
	name = "None";
}

Button::~Button() {
}

Button& Button::operator=(const Button &ref) {
	if (this != &ref) {
		start = ref.start;
		size = ref.size;
		state = ref.state;
	}
	return (*this);
}

Button::Button(const Button &ref) {
	*this = ref;
}

Button::Button(iVec2 startBtn, iVec2 sizeBtn) {
	start = startBtn;
	size = sizeBtn;
	state = BTN_UNPRESS;
}

void Button::drawButton(GraphicLib *lib, void *texture) {
	iVec2	nameStart;

	lib->drawTextureTile(texture, start, size);
	nameStart.x = start.x + (size.x >> 2);
	nameStart.y = start.y - (size.y >> 4);
	lib->writeText(name.c_str(), nameStart, FONT_SIZE, WHITE_RGBA);
}

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