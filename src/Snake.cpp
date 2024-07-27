#include "../include/Snake.hpp"
#include "../include/Nibbler.hpp"

/* Default constructor */
Snake::Snake() {
	head.x = 0;
	head.y = 0;
	body = std::vector<iVec2>();
	toAdd.x = -1;
	toAdd.y = -1;
	wasFood = false;
	setDirection(UP);
	setNextDirection(UP);
}

/* Destructor */
Snake::~Snake() {}

/* Assignment operator */
Snake& Snake::operator=(const Snake &ref) {
	head.x = ref.head.x;
	head.y = ref.head.y;
	body = ref.body;
	toAdd.x = ref.toAdd.x;
	toAdd.y = ref.toAdd.y;
	return (*this);
}

/* Copy constructor */
Snake::Snake(const Snake &ref) {
	*this = ref;
}

/**
 * @brief Real Constructor
 * @ctx Nibbler context
 * @x X position
 * @y Y position
*/
Snake::Snake(Nibbler &ctx, s32 x, s32 y) {
	toAdd.x = -1;
	toAdd.y = -1;
	wasFood = false;
	setDirection(UP);
	setNextDirection(UP);
	setHeadX(x);
	setHeadY(y);
	ctx.boardTileSet(getHeadX(), getHeadY(), SNAKE_HEAD);
	body = std::vector<iVec2>();
	brutExpension(ctx);
	brutExpension(ctx);
	brutExpension(ctx);
}

/**
 * @brief Reset the snake position
*/
void Snake::resetSnake() {
	head.x = 0;
	head.y = 0;
	body.clear();
	toAdd.x = -1;
	toAdd.y = -1;
	direction = UP;
	nextDirection = UP;
	wasFood = false;
}

/**
 * @brief Detect where to add a new body part (only use for first 4 body part)
 * @ctx Nibbler context
 * @currX current X position
 * @currY current Y position
 * @maxX board max X position
 * @maxY board max Y position
*/
void Snake::brutForceBodySpace(Nibbler &ctx, s32 currX, s32 currY, s32 maxX, s32 maxY) {
	iVec2 tmp = {-1, -1};
	
	if (currY + 1 < maxY && ctx.boardTileGet(currX, currY + 1) == EMPTY){
		tmp.x = currX;
		tmp.y = currY + 1;
	} else if (currX + 1 < maxX && ctx.boardTileGet(currX + 1, currY) == EMPTY){
		tmp.x = currX + 1;
		tmp.y = currY;
	} else if (currY - 1 >= 0 && ctx.boardTileGet(currX, currY - 1) == EMPTY){
		tmp.x = currX;
		tmp.y = currY - 1;
	} else if (currX - 1 >= 0 && ctx.boardTileGet(currX - 1, currY) == EMPTY){
		tmp.x = currX - 1;
		tmp.y = currY;
	} else {
		/* No more space to add a new body part */
		std::cout << "No more space to add a new body part" << std::endl;
		return ;
	}
	body.push_back(tmp);
	ctx.boardTileSet(body[body.size() - 1].x, body[body.size() - 1].y, SNAKE_BODY);
}

/**
 * @brief Detect where to add a new body part
 * @ctx Nibbler context
*/
void Snake::brutExpension(Nibbler &ctx) {
	s32 tmpX = -1, tmpY = -1;
	
	/* Detect snake last tile */
	if (body.size() == 0) {
		tmpX = getHeadX();
		tmpY = getHeadY();
	} else {
		tmpX = body[body.size() - 1].x;
		tmpY = body[body.size() - 1].y;
	}
	/*	
		Detect with brut force where to add the new body part in range of the board 
		Start with BOTTOM, RIGHT, TOP, LEFT
	*/
	s32 maxX = ctx.getWidth();
	s32 maxY = ctx.getHeight();

	brutForceBodySpace(ctx, tmpX, tmpY, maxX, maxY);
}

/**
 * @brief Snake eat (grow), when snake eats food add a new body part
 * @ctx Nibbler context
*/
void Snake::SnakeEat(Nibbler &ctx) {
	iVec2 tmp = {toAdd.x, toAdd.y};
	body.push_back(tmp);
	ctx.boardTileSet(toAdd.x, toAdd.y, SNAKE_BODY);
}

/**
 * @brief Update the body position to follow the head
 * @ctx Nibbler context
 * @oldX old head X position
 * @oldY old head Y position
 */
void Snake::bodyFollowHead(Nibbler &ctx, s32 oldX, s32 oldY) {
    if (body.empty()) return;

    s32 tmpX, tmpY;

    for (u64 i = 0; i < body.size(); ++i) {
		/* Save current body position */
        tmpX = body[i].x;
        tmpY = body[i].y;
		/* Update body position to old pos */
        body[i].x = oldX;
        body[i].y = oldY;
		/* Update board */
        ctx.boardTileSet(oldX, oldY, SNAKE_BODY);
		/* Update old pos */
        oldX = tmpX;
        oldY = tmpY;
    }

	/* Update board (old last position become empty )*/
	ctx.boardTileSet(oldX, oldY, EMPTY);
	toAdd.x = oldX;
	toAdd.y = oldY;
}

/**
 * @brief Move the snake in the direction (move head and body follow him)
 * @ctx Nibbler context
 * @direction direction to move
 */
void Snake::SnakeMove(Nibbler &ctx, s32 direction) {
	iVec2 old = {getHeadX(), getHeadY()};
	iVec2 newHead = {old.x, old.y};

	/* Check if the new head tile was food and grow snake body */
	if (wasFood) {
		SnakeEat(ctx);
		// std::cout << "Snake eat body size : " << body.size() << std::endl;
	}

	/* Update direction */
	setDirection(getNextDirection());

	/* Update head position */
	if (direction == UP) { newHead.y -= 1; }
	else if (direction == DOWN) { newHead.y += 1; }
	else if (direction == RIGHT) { newHead.x += 1; }
	else if (direction == LEFT) { newHead.x -= 1; }

	/* Guard for out of board */
	if ((newHead.x < 0 || newHead.x >= ctx.getWidth()) || (newHead.y < 0 || newHead.y >= ctx.getHeight())) {
		std::cout << "Out of board" << std::endl;
		resetSnake();
		ctx.resetGame();
		return;
	}

	/* Guard for snake collision */
	if (ctx.boardTileGet(newHead.x, newHead.y) == SNAKE_BODY) { 
		std::cout << "Snake collision" << std::endl;
		resetSnake();
		ctx.resetGame();
		return;
	}

	/* set food boolean */
	wasFood = ctx.boardTileGet(newHead.x, newHead.y) == FOOD;

	setHeadX(newHead.x);
	setHeadY(newHead.y);
	ctx.boardTileSet(newHead.x, newHead.y, SNAKE_HEAD);
	/* Mark old head position empty */
	ctx.boardTileSet(old.x, old.y, EMPTY);
	bodyFollowHead(ctx, old.x, old.y);

	/* If new head tile was food, check if we need to add a new food */
	if (wasFood) {
		ctx.setNbFood(ctx.getNbFood() - 1);
		if (ctx.getNbFood() == 0) {
			ctx.foodAdd();
		}
	}

}


s32 Snake::getSnakeNormaliseDir(s32 dir) {
	if (dir == UP || dir == DOWN) {
		return (TOP_BOT_DIR);
	}
	return (LEFT_RIGHT_DIR);
}
/* Handle snake move by key pressed */
void Snake::handleSnakeDir(s32 event) {

	s32 snakeDir = getSnakeNormaliseDir(getDirection());

	if (snakeDir == TOP_BOT_DIR) {
		if (event == NKEY_LEFT) {
			setNextDirection(LEFT);
		} else if (event == NKEY_RIGHT) {
			setNextDirection(RIGHT);
		}
		return ;
	} 
	if (event == NKEY_UP) {
		setNextDirection(UP);
	} else if (event == NKEY_DOWN) {
		setNextDirection(DOWN);
	}
}


/* Getter setter */

s32 &Snake::getHeadX() {
	return (head.x);
}

s32 &Snake::getHeadY() {
	return (head.y);
}

void Snake::setHeadX(s32 x) {
	head.x = x;
}

void Snake::setHeadY(s32 y) {
	head.y = y;
}

std::vector<iVec2> &Snake::getBody() {
	return (body);
}

void Snake::setDirection(s32 dir) {
	direction = dir;
}

s32 &Snake::getDirection() {
	return (direction);
}

s32 &Snake::getNextDirection() {
	return (nextDirection);
}

void Snake::setNextDirection(s32 dir) {
	nextDirection = dir;
}