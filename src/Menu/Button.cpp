#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"

Button::Button() {
	start = {0, 0};
	size = {0, 0};
	nameStart = {0, 0};
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

Button::Button(iVec2 startBtn, iVec2 sizeBtn, std::string nameBtn, u8 idBtn) {
	start = startBtn;
	size = sizeBtn;
	state = BTN_UNPRESS;
	id = idBtn;
	name = nameBtn;
	nameStart.x = start.x + (size.x >> 2);
	nameStart.y = start.y;
	std::cout << "Btn start text Y: " << nameStart.y << " x: "  << nameStart.x << std::endl;
}

void Button::drawButton(GraphicLib *lib, void *texture) const {
	lib->drawTextureTile(texture, start, size);
	lib->writeText(name.c_str(), nameStart, FONT_SIZE, WHITE_RGBA);
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

void Button::setState(u8 newState) {
	state = newState;
}

void Button::setStart(iVec2 newStart) {
	start = newStart;
}

void Button::setSize(iVec2 newSize) {
	size = newSize;
}
