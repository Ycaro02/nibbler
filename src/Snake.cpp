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
	bodyX = std::vector<s32>();
	bodyY = std::vector<s32>();

}

/**
 * @brief Detect where to add a new body part
 * @ctx Nibbler context
 * @currX current X position
 * @currY current Y position
 * @maxX board max X position
 * @maxY board max Y position
*/
void Snake::brutForceBodySpace(Nibbler &ctx, s32 currX, s32 currY, s32 maxX, s32 maxY) {
	if (currY + 1 < maxY && ctx.boardTileGet(currX, currY + 1) == EMPTY){
		bodyX.push_back(currX);
		bodyY.push_back(currY + 1);
	} else if (currX + 1 < maxX && ctx.boardTileGet(currX + 1, currY) == EMPTY){
		bodyX.push_back(currX + 1);
		bodyY.push_back(currY);
	} else if (currY - 1 >= 0 && ctx.boardTileGet(currX, currY - 1) == EMPTY){
		bodyX.push_back(currX);
		bodyY.push_back(currY - 1);
	} else if (currX - 1 >= 0 && ctx.boardTileGet(currX - 1, currY) == EMPTY){
		bodyX.push_back(currX - 1);
		bodyY.push_back(currY);
	} else {
		/* No more space to add a new body part */
		std::cout << "No more space to add a new body part" << std::endl;
		return ;
	}
	ctx.boardTileSet(bodyX[bodyX.size() - 1], bodyY[bodyY.size() - 1], SNAKE_BODY);
}

/**
 * @brief Detect where to add a new body part
 * @ctx Nibbler context
*/
void Snake::detectSnakeExpension(Nibbler &ctx) {
	s32 tmpX = -1, tmpY = -1;
	
	/* Detect snake last tile */
	if (bodyX.size() == 0) {
		tmpX = getHeadX();
		tmpY = getHeadY();
	} else {
		tmpX = bodyX[bodyX.size() - 1];
		tmpY = bodyY[bodyY.size() - 1];
	}
	/*	
		Detect with brut force where to add the new body part in range of the board 
		Start with BOTTOM, RIGHT, TOP, LEFT
	*/
	s32 maxX = ctx.getWidth();
	s32 maxY = ctx.getHeight();

	brutForceBodySpace(ctx, tmpX, tmpY, maxX, maxY);
}

void Snake::SnakeEat(Nibbler &ctx) {
	detectSnakeExpension(ctx);
}

/**
 * @brief Update the body position to follow the head
 * @ctx Nibbler context
 * @oldX old head X position
 * @oldY old head Y position
 */
void Snake::bodyFollowHead(Nibbler &ctx, s32 oldX, s32 oldY) {
    if (bodyX.empty() || bodyY.empty()) return; // S'assurer que le corps n'est pas vide

    s32 tmpX, tmpY;

    for (u64 i = 0; i < bodyX.size(); ++i) {
		/* Save current body position */
        tmpX = bodyX[i];
        tmpY = bodyY[i];

		/* Update body position to old pos */
        bodyX[i] = oldX;
        bodyY[i] = oldY;

		/* Update board */
        ctx.boardTileSet(oldX, oldY, SNAKE_BODY);

		/* Update old pos */
        oldX = tmpX;
        oldY = tmpY;
    }

	/* Update board (old last position become empty )*/
	if (bodyX.size() != 0) {
		ctx.boardTileSet(oldX, oldY, EMPTY);
	}
}

void Snake::SnakeMove(Nibbler &ctx, s32 direction) {
	s32 oldX = getHeadX();
	s32 oldY = getHeadY();
	
	s32 newX = oldX;
	s32 newY = oldY;

	if (direction == UP) {
		newY -= 1;
	} else if (direction == DOWN) {
		newY += 1;
	} else if (direction == RIGHT) {
		newX += 1;
	} else if (direction == LEFT) {
		newX -= 1;
	} 

	/* Guard for board */
	if (newX < 0 || newX >= ctx.getWidth()) { return; }
	if (newY < 0 || newY >= ctx.getHeight()) { return; }


	/* Food boolean */
	u8 wasFood = ctx.boardTileGet(newX, newY) == FOOD;

	setHeadX(newX);
	setHeadY(newY);
	ctx.boardTileSet(newX, newY, SNAKE_HEAD);
	/* Mark old head position empty */
	ctx.boardTileSet(oldX, oldY, EMPTY);

	bodyFollowHead(ctx, oldX, oldY);
	
	if (wasFood) {
		ctx.setNbFood(ctx.getNbFood() - 1);
		SnakeEat(ctx);
		std::cout << "Snake eat body size : " << bodyX.size() << std::endl;
		if (ctx.getNbFood() == 0) {
			ctx.foodAdd();
		}
	}


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

