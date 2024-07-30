#include "../../include/Button.hpp"

Button::Button() {
	start = {0, 0};
	size = {0, 0};
	state = BTN_UNPRESS;
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
