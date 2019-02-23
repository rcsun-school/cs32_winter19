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

bool Actor::alive() const {
	return isAlive;
}
bool Actor::isFlammable() const {
	return false;
}

bool Actor::zombieFood() const {
	return false;
}
Person::Person(int imageID, double startX, double startY, Direction dir, int depth) : Actor(imageID, startX, startY, dir, depth) {
}

Person::~Person() {
}

bool Person::isFlammable() const {
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
Zombie::Zombie(double startX, double startY, Direction dir, int depth) : Actor(IID_ZOMBIE, startX, startY, dir, depth) {

}

Zombie::~Zombie() {

}

bool Zombie::isFlammable() const {
	return true;
}

void Zombie::vomit() {

}


Penelope::Penelope(double start_x, double start_y, StudentWorld * arena) : Person(IID_PLAYER, start_x, start_y) {
	m_arena = arena;
	isInfected = false;
	infectionCount = 0;
	flameCount = 0;
	mineCount = 0;
	vaccineCount = 0;

}

void Penelope::doSomething() {
	if (!alive()) {
		return;
	}
	if (isInfected) {
		infectionCount++;
		if (infectionCount >= 500) {
			isAlive = false;
			m_arena->playSound(SOUND_PLAYER_DIE);
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

			}
		}
}


int Penelope::getFlame() const {
	return flameCount;
}

int Penelope::getMines() const {
	return mineCount;
}

int Penelope::getVaccines() const {
	return vaccineCount;
}
Penelope::~Penelope() {
}

bool Penelope::alive() const {
	return isAlive;
}

int Penelope::die() {
	isAlive = false;
	return 0;
}

int Penelope::getInfectionCount() const{
	return infectionCount;
}

bool Penelope::infected() const {
	return isInfected;
}

Wall::Wall(double start_x, double start_y): Actor(IID_WALL, start_x, start_y) {

}

void Wall::doSomething() {
	return;
}

bool Wall::alive() const {
	return true;
}

Wall::~Wall() {
}

bool Wall::isFlammable() const {
	return false;
}






 
Exit::Exit(double x, double y, StudentWorld * arena) : Actor(IID_EXIT, x, y, 0, 1) {
	m_arena = arena;
}

void Exit::doSomething() {
	m_arena->checkExit(this->getX(), this->getY());
}

bool Exit::alive() const {
	return true;
}

Exit::~Exit() {

}

bool Exit::isTangible() const {
	return false;
}

bool Exit::isFlammable() const {
	return false;
}