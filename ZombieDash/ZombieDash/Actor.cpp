#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>


Actor::Actor(int imageID, double startX, double startY, StudentWorld * arena, Direction dir, int depth): GraphObject(imageID, startX, startY, dir , depth){
	isAlive = true;
	m_infected = false;
	m_arena = arena;
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

void Actor::die() {
	isAlive = false;
	return;
}

 void Actor::beSaved() {
	return;
}

 
Agent::Agent(int imageID, double startX, double startY, StudentWorld * arena, int points_killed, int moveSize) : Actor(imageID, startX, startY, arena), time(0) {
	POINT_VALUE_KILLED = points_killed;
	moveSpace = moveSize;
}

bool Agent::move(Direction dir) {
//Inspects StudentWorld if Agent can move in a certain direction. If possible, move the Agent in that direction and return true. Else, return false
	setDirection(dir);
	switch (dir) {
	case right:
		if (getArena()->canMoveTo(getX() + moveSpace, getY(), this)) {
			moveTo(getX() + moveSpace, getY());
			return true;
		}
		else {
			return false;
		}
		break;
	case up: 
		if (getArena()->canMoveTo(getX(), getY() + moveSpace, this)) {
			moveTo(getX(), getY() + moveSpace);
			return true;
		}
		else {
			return false;
		}
		break;
	case left: 
		if (getArena()->canMoveTo(getX() - moveSpace, getY(), this)) {
			moveTo(getX() - moveSpace, getY());
			return true;
		}
		else {
			return false;
		}
		break;
	case down:
		if (getArena()->canMoveTo(getX(), getY() - moveSpace, this)) {
			moveTo(getX(), getY() - moveSpace);
			return true;
		}
		else {
			return false;
		}
		break;
	}
	return false;
	
}
Person::Person(int imageID, double startX, double startY, StudentWorld * arena, int points_saved, int points_killed , int moveSpace) : Agent(imageID, startX, startY, arena, points_killed, moveSpace) {
	infectionCount = 0;
	POINT_VALUE_SAVED = points_saved;
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


int Person::getInfectionCount() const {
	return infectionCount;
}
Zombie::Zombie(double startX, double startY, int points_killed, StudentWorld * arena) : Agent(IID_ZOMBIE, startX, startY, arena, points_killed, 1) {
	m_move = 0;
}

Zombie::~Zombie() {

}


bool Zombie::vomit() {
	//inspect cardinal directions of the zombie for nearby citizens, return true if zombie has vomitted, return false otherwise
	switch (getDirection()) {
	case right:
	{
		if (getArena()->findCitizens(getX() + SPRITE_WIDTH, getY(), this)) {
			return true;
		}
		break;
	}
	case up:
		if (getArena()->findCitizens(getX(), getY() + SPRITE_HEIGHT, this)) {
			return true;
		}
		break;
		{

		}
	case left:
	{
		if (getArena()->findCitizens(getX() - SPRITE_WIDTH, getY(), this)) {
			return true;
		}
		break;
	}
	case down:
	{
		if (getArena()->findCitizens(getX(), getY() + SPRITE_HEIGHT, this)) {
			return true;
		}
		break;
	}
	default:
		return false;
	}
	return false;
}



void DumbZombie::doSomething() {
	if (paralyzed()) {
		return;
	}
	if (vomit()) {
		return;
	}
	
	if (getMovementPlan() == 0) {
		setMovementPlan(randInt(3, 10));
		setDirection(90 * randInt(0, 3));
	}
	if (move(getDirection())) {
		return;
	}
	else {
		setMovementPlan(0);
		return;
	}
}

void DumbZombie::die() {
	Zombie::die();
	int chance = randInt(1, 10);
	if (chance == 1) {
		switch (90 * randInt(0, 3)) {
		case right:
			getArena()->generateVaccine(getX() + SPRITE_WIDTH, getY());
			break;
		case up:
			getArena()->generateVaccine(getX(), getY() + SPRITE_HEIGHT);
			break;
		case left:
			getArena()->generateVaccine(getX() - SPRITE_WIDTH, getY());
			break;
		case down:
			getArena()->generateVaccine(getX(), getY() - SPRITE_HEIGHT);
			break;
		}
	}
}

void SmartZombie::doSomething() {
	if (paralyzed()) {
		return;
	}
	if (vomit()) {
		return;
	}
	if (getMovementPlan() == 0) {
		setMovementPlan(randInt(3, 10));
		if (getArena()->goAfterHumanIfApplicable(this, getX(), getY())) {
			return;
		}
		else {
			setDirection(90 * randInt(0, 3));
		}
	}
	if (move(getDirection())) {
		return;
	}
	else {
		setMovementPlan(0);
	}

}
Citizen::Citizen(double startX, double startY, StudentWorld * arena): Person(IID_CITIZEN, startX, startY, arena, 500, -1000, 2) {
	
}

void Citizen::doSomething() {
	if (checkInfection()) {
		getArena()->generateZombie(getX(), getY());
		Actor::die();
		getArena()->decCitizens();
		return;
	}
	if (paralyzed()) {
		return;
		}
	getArena()->moveCitizen(this);

}

void Citizen::die() {
	getArena()->playSound(SOUND_CITIZEN_DIE);
	getArena()->decCitizens();
	Agent::die();
}

Penelope::Penelope(double start_x, double start_y, StudentWorld * arena) : Person(IID_PLAYER, start_x, start_y, arena, 0, 0, 4) {
	flameCount = 0;
	mineCount = 0;
	vaccineCount = 0;

}

void Penelope::doSomething() {
		if (checkInfection()) {
			die();
			//game must play player die sound & student world should detect dead
			return;
		}
		int key_value;
		if (getArena()->getKey(key_value)) {
			//Penelope tries to move
			switch (key_value) {
			case KEY_PRESS_RIGHT:
				Agent::move(right);
				break;
			case KEY_PRESS_UP:
				Agent::move(up);
				break;
			case KEY_PRESS_LEFT:
				Agent::move(left);
				break;
			case KEY_PRESS_DOWN:
				Agent::move(down);
				break;
				//penelope uses fire
			case KEY_PRESS_SPACE:
				if (flameCount <= 0) {
					break;
				}
				flameCount--;
				getArena()->playSound(SOUND_PLAYER_FIRE);
				switch (getDirection()) {
				case right:
					for (int i = 1; i < 4; i++) {
						if (getX() + i * SPRITE_WIDTH >= VIEW_WIDTH) {
							break;
						}
						getArena()->generateFlames(getX() + i * SPRITE_WIDTH, getY(), getDirection());
					}
					break;
				case up:
					for (int i = 1; i < 4; i++) {
						if (getY() + i * SPRITE_HEIGHT >= VIEW_HEIGHT) {
							break;
						}
						getArena()->generateFlames(getX(), getY() + i * SPRITE_HEIGHT, getDirection());
					}
					break;
				case left:
					for (int i = 1; i < 4; i++) {
						if (getX() - i * SPRITE_WIDTH < 0) {
							break;
						}
						getArena()->generateFlames(getX() - i * SPRITE_WIDTH, getY(), getDirection());
					}
					break;
				case down:
					for (int i = 1; i < 4; i++) {
						if (getY() - i * SPRITE_HEIGHT < 0) {
							break;
						}
						getArena()->generateFlames(getX(), getY() - i * SPRITE_HEIGHT, getDirection());
					}
					break;
				}
				break;
			case KEY_PRESS_TAB: {
				if (mineCount <= 0) {
					break;
				}
				getArena()->generateLandmine(getX(), getY());
				mineCount--;
			} break;
			case KEY_PRESS_ENTER: {
				if (vaccineCount <= 0) {
					break;
				}
				else {
					vaccineCount--;
					useVaccine();
				}
				break;
			}
			default: {
				break;
			}
			
				

			}
		}
}







Penelope::~Penelope() {
}




Wall::Wall(double start_x, double start_y, StudentWorld * arena): Actor(IID_WALL, start_x, start_y, arena) {

}

void Wall::doSomething() {
	return;
}


Wall::~Wall() {
}







 
Exit::Exit(double x, double y, StudentWorld * arena) : Actor(IID_EXIT, x, y, arena, 0, 1) {
}

void Exit::doSomething() {
	getArena()->checkExit(this->getX(), this->getY());
}


Exit::~Exit() {}

bool Exit::isTangible() const {
	return false;
}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld * arena, Direction dir, int depth): Actor(imageID, startX, startY, arena, dir, depth) {
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


Hazard::Hazard(int imageID, double startX, double startY, StudentWorld * arena, Direction dir) : Actor(imageID, startX, startY, arena, dir, 1) {
	timer = 0;
}

Flame::Flame(double startX, double startY, StudentWorld * arena, Direction dir) : Hazard(IID_FLAME, startX, startY, arena, dir) {

}

void Flame::doSomething() {
	if (timeUp()) {
		return;
	}
	else {
		getArena()->hazardOverlap(getX(), getY());
	}
}

Pit::Pit(double startX, double startY, StudentWorld * arena) : Hazard(IID_PIT, startX, startY, arena) {

}

void Pit::doSomething() {
	getArena()->hazardOverlap(getX(), getY());
}

Landmine::Landmine(double startX, double startY, StudentWorld * arena) : Hazard(IID_LANDMINE, startX, startY, arena) {
	isActive = false;
	safetyTicks = 30;
}

void Landmine::BOOM() {
	getArena()->playSound(SOUND_LANDMINE_EXPLODE);
	for (double i = getX() - SPRITE_WIDTH; i <= getX() + SPRITE_WIDTH; i+= SPRITE_WIDTH) {
		for (double j = getY() - SPRITE_HEIGHT; j <= getY() + SPRITE_HEIGHT; j += SPRITE_HEIGHT) {
			getArena()->generateFlames(i, j, up);
		}
	}
	getArena()->generatePit(getX(), getY());
	die();
}

void Landmine::activate() {
	if (safetyTicks > 0) {
		safetyTicks--;
	}
	if (safetyTicks <= 0) {
		isActive = true;
	}
}
void Landmine::doSomething() {
	activate();
	if (!isActive) {
		return;
	}
	if (getArena()->mineOverlap(getX(), getY(), this)) {
		BOOM();
	}
}

Vomit::Vomit(double startX, double startY, StudentWorld * arena, Direction dir): Hazard(IID_VOMIT, startX, startY, arena, dir) {

}


void Vomit::doSomething() {
	if (timeUp()) {
		return;
	}
	getArena()->vomitOverlap(getX(), getY());
}