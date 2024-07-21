#include "../include/Snake.hpp"
#include "../include/Nibbler.hpp"

Snake::Snake() {
	headX = 0;
	headY = 0;
}

Snake::~Snake() {}

Snake& Snake::operator=(const Snake &ref) {
	headX = ref.headX;
	headY = ref.headY;
	bodyX = ref.bodyX;
	bodyY = ref.bodyY;
	return (*this);
}

Snake::Snake(const Snake &ref) {
	*this = ref;
}

Snake::Snake(Nibbler &ctx, s32 x, s32 y) {
	setHeadX(x);
	setHeadY(y);
	ctx.boardTileSet(getHeadX(), getHeadY(), SNAKE_HEAD);
}

void Snake::SnakeMove(Nibbler &ctx, s32 direction) {
	if (direction == UP) {
		setHeadY(getHeadY() + 1);
	} else if (direction == DOWN) {
		setHeadY(getHeadY() - 1);
	} else if (direction == RIGHT) {
		setHeadX(getHeadX() + 1);
	} else if (direction == LEFT) {
		setHeadX(getHeadX() - 1);
	}
	ctx.boardTileSet(getHeadX(), getHeadY(), SNAKE_HEAD);
}

s32 &Snake::getHeadX() {
	return (headX);
}

s32 &Snake::getHeadY() {
	return (headY);
}

void Snake::setHeadX(s32 x) {
	headX = x;
}

void Snake::setHeadY(s32 y) {
	headY = y;
}

std::vector<s32> &Snake::getBodyX() {
	return (bodyX);
}

std::vector<s32> &Snake::getBodyY() {
	return (bodyY);
}

