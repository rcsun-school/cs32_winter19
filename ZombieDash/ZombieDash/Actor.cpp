#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth): GraphObject(imageID, startX, startY, dir , depth){
	isAlive = true;
}

Actor::~Actor() {

}

bool Actor::isTangible() const {
	return true;
}

bool Actor::canBeSaved() const {
	return false;
}
bool Actor::isKillable() const {
	return false;
}

bool Actor::zombieFood() const {
	return false;
}

int Actor::die() {
	isAlive = false;
	return 0;
}

int Actor::beSaved() {
	return 0;
}
Person::Person(int imageID, double startX, double startY, int points_saved, int points_killed) : Actor(imageID, startX, startY) {
	isInfected = false;
	infectionCount = 0;
	POINT_VALUE_SAVED = points_saved;
	POINT_VALUE_KILLED = points_killed;
}

Person::~Person() {
}

bool Person::isKillable() const {
	return true;
}

bool Person::canBeSaved() const {
	return true;
}

bool Person::zombieFood() const {
	return true;
}


bool Person::infected() const {
	return isInfected;
}

int Person::getInfectionCount() const {
	return infectionCount;
}
Zombie::Zombie(double startX, double startY, int points_killed) : Actor(IID_ZOMBIE, startX, startY) {
	POINT_VALUE_KILLED = points_killed;
	isAlive = true;
}

Zombie::~Zombie() {

}


void Zombie::vomit() {

}


Penelope::Penelope(double start_x, double start_y, StudentWorld * arena) : Person(IID_PLAYER, start_x, start_y, 0, 0) {
	m_arena = arena;
	flameCount = 0;
	mineCount = 0;
	vaccineCount = 0;

}

void Penelope::doSomething() {
	if (!alive()) {
		return;
	}
	if (infected()) {
		increaseInfection();
		if (getInfectionCount() >= 500) {
			die();
			//game must play player die sound & student world should detect dead
			return;
		}
	}
		int key_value;
		if (m_arena->getKey(key_value)) {
			switch (key_value) {
			case KEY_PRESS_RIGHT:
				this->setDirection(0);
				if (m_arena->canMoveTo(getX() + 1, getY(), this)) {
					this->moveTo(this->getX() + 1, this->getY());
				}
				break;
			case KEY_PRESS_UP:
				this->setDirection(90);
				if (m_arena->canMoveTo(getX(), getY() + 1, this)) {
					this->moveTo(this->getX(), this->getY() + 1);
				}
				break;
			case KEY_PRESS_LEFT:
				this->setDirection(180);
				if (m_arena->canMoveTo(getX() - 1, getY(), this)) {
					this->moveTo(this->getX() - 1, this->getY());
				}
				break;
			case KEY_PRESS_DOWN:
				this->setDirection(270);
				if (m_arena->canMoveTo(getX(), getY() - 1, this)) {
					this->moveTo(this->getX(), this->getY() - 1);
					break;

				}
			case KEY_PRESS_SPACE:
				if (flameCount <= 0) {
					break;
				}
				flameCount--;
				m_arena->playSound(SOUND_PLAYER_FIRE);
				switch (getDirection()) {
				case right:
					for (int i = 1; i < 4; i++) {
						if (getX() + i * SPRITE_WIDTH >= VIEW_WIDTH) {
							break;
						}
						m_arena->generateFlames(getX() + i * SPRITE_WIDTH, getY());
					}
					break;
				case up:
					for (int i = 1; i < 4; i++) {
						if (getY() + i * SPRITE_HEIGHT >= VIEW_HEIGHT) {
							break;
						}
						m_arena->generateFlames(getX(), getY() + i * SPRITE_HEIGHT);
					}
					break;
				case left:
					for (int i = 1; i < 4; i++) {
						if (getX() - i * SPRITE_WIDTH < 0) {
							break;
						}
						m_arena->generateFlames(getX() - i * SPRITE_WIDTH, getY());
					}
					break;
				case down:
					for (int i = 1; i < 4; i++) {
						if (getY() - i * SPRITE_HEIGHT < 0) {
							break;
						}
						m_arena->generateFlames(getX(), getY() - i * SPRITE_HEIGHT);
					}
					break;
				}
				

			}
		}
}







Penelope::~Penelope() {
}




Wall::Wall(double start_x, double start_y): Actor(IID_WALL, start_x, start_y) {

}

void Wall::doSomething() {
	return;
}


Wall::~Wall() {
}







 
Exit::Exit(double x, double y, StudentWorld * arena) : Actor(IID_EXIT, x, y, 0, 1) {
	m_arena = arena;
}

void Exit::doSomething() {
	m_arena->checkExit(this->getX(), this->getY());
}


Exit::~Exit() {}

bool Exit::isTangible() const {
	return false;
}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld * arena, Direction dir, int depth): Actor(imageID, startX, startY, dir, depth) {
	m_arena = arena;
}

VaccineGoodie::VaccineGoodie(double startX, double startY, StudentWorld * arena) : Goodie(IID_VACCINE_GOODIE, startX, startY, arena){

}

void VaccineGoodie::doSomething() {
	if (getArena()->goodieOverlap(this->getX(), this->getY())) {
		getArena()->getPenelope()->plusVaccine();
		die();
	}
}

GasCanGoodie::GasCanGoodie(double startX, double startY, StudentWorld * arena) : Goodie(IID_GAS_CAN_GOODIE, startX, startY, arena) {

}

void GasCanGoodie::doSomething() {
	if (getArena()->goodieOverlap(getX(), getY())) {
		getArena()->getPenelope()->plusFlame();
		die();
	}
}

LandmineGoodie::LandmineGoodie(double startX, double startY, StudentWorld * arena) : Goodie(IID_LANDMINE_GOODIE, startX, startY, arena) {
	
}

void LandmineGoodie::doSomething() {
	if (getArena()->goodieOverlap(getX(), getY())) {
		getArena()->getPenelope()->plusMines();
		die();
	}
}


Hazard::Hazard(int imageID, double startX, double startY, StudentWorld * arena) : Actor(imageID, startX, startY) {
	m_arena = arena;
	timer = 0;
}

Flame::Flame(double startX, double startY, StudentWorld * arena) : Hazard(IID_FLAME, startX, startY, arena) {

}

void Flame::doSomething() {
	if (timeUp()) {
		return;
	}
	else {
		getArena()->hazardOverlap(getX(), getY());
	}
}