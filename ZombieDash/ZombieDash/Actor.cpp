#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY): GraphObject(imageID, startX, startY){

}

Actor::~Actor() {

}

bool Actor::isTangible() {
	return true;
}


Penelope::Penelope(double start_x, double start_y, StudentWorld * arena) : Actor(IID_PLAYER, start_x, start_y) {
	m_arena = arena;
	isAlive = true;
	isInfected = false;
	infectionCount = 0;

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

bool Penelope::alive() const {
	return isAlive;
}

bool Penelope::infected() const {
	return isInfected;
}

int Penelope::getInfectionCount() const {
	return infectionCount;
}

Wall::Wall(double start_x, double start_y): Actor(IID_WALL, start_x, start_y) {

}

void Wall::doSomething() {
	return;
}

bool Wall::alive() const {
	return true;
}