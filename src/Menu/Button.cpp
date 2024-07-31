#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"

Button::Button() {
	start = {0, 0};
	size = {0, 0};
	state = BTN_UNPRESS;
	id = 0;
	name = "None";
	std::cout << "Default btn constructor call\n";
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

Button::Button(iVec2 startBtn, iVec2 sizeBtn, u8 idBtn) {
	start = startBtn;
	size = sizeBtn;
	state = BTN_UNPRESS;
	id = idBtn;
}

void Button::drawButton(GraphicLib *lib, void *texture) {
	iVec2	nameStart;
	// s32		textLen = name.length();

	lib->drawTextureTile(texture, start, size);
	nameStart.x = start.x + (size.x >> 2);
	nameStart.y = start.y;
	lib->writeText(name.c_str(), nameStart, FONT_SIZE, WHITE_RGBA);
	// std::cout << "Btn start text Y: " << nameStart.y << " x: "  << nameStart.x;
	// std::cout << "--> Start Y: " << start.y << " x: "  << start.x << std::endl;

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