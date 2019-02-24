#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
	//base class for all actors within game (Penelope, Persons, Zombies, etc.)
public:
	Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Actor();
	virtual bool alive() const {
		return isAlive;
	}
	virtual void doSomething() = 0;
	virtual bool isTangible() const;
	virtual bool canBeSaved() const;
	virtual bool isKillable() const;
	virtual bool zombieFood() const;
	virtual int die();
	virtual int beSaved();
	virtual bool canBlockFlames() {
		return true;
	}
private:
	bool isAlive;
};

class Person : public Actor {
public:
	Person(int imageID, double startX, double startY, int points_saved, int points_killed);
	virtual ~Person();
	int die() {
		Actor::die();
		return POINT_VALUE_KILLED;
	}
	bool canBeSaved() const;
	bool isKillable() const;
	bool zombieFood() const;
	bool infected() const;
	int getInfectionCount() const;
	void increaseInfection() {
		infectionCount++;
	}
	virtual void beInfected() {
		this->isInfected = true;
	}
	bool canBlockFlames() {
		return false;
	}
private: 
	bool isInfected;
	int infectionCount;
	int POINT_VALUE_SAVED;
	int POINT_VALUE_KILLED;
};

class Zombie : public Actor {
public:
	Zombie(double startX, double startY, int points_killed);
	virtual ~Zombie();
	virtual void vomit();
	bool isKillable() const {
		return true;
	}
	bool canBlockFlames() {
		return false;
	}
	int die() {
		isAlive = false;
		return POINT_VALUE_KILLED;
	};

	bool alive() {
		return isAlive;
	}
private:
	int POINT_VALUE_KILLED;
	bool isAlive;
};

class DumbZombie : public Zombie {
public:
	DumbZombie(double startX, double startY) : Zombie(startX, startY, 1000) {

	}
};

class Hazard : public Actor {
public:
	Hazard(int imageID, double startX, double startY, StudentWorld * arena);
	virtual ~Hazard() {

	}
	bool isTangible() const {
		return false;
	}
	bool isKillable() const {
		return true;
	}
	StudentWorld * getArena() {
		return m_arena;
	}
	bool timeUp() {
		if (timer == 2) {
			die();
			return true;
		}
		else {
			timer++;
			return false;
		}
	}

private:
	bool isAlive;
	StudentWorld * m_arena;
	int timer;

};

class Flame : public Hazard {
public:
	Flame(double startX, double startY, StudentWorld * arena);
	~Flame() {}
	void doSomething();
};

class Landmine : public Hazard {
public:
	Landmine(double startX, double startY, StudentWorld * arena);
	~Landmine() {};
	void doSomething();
	void BOOM();
};


class Vomit : public Hazard {
public:
	Vomit(double startX, double startY, StudentWorld * arena);
	~Vomit() {}
	void doSomething();
};

class Pit : public Hazard {
public:
	Pit(double startX, double startY, StudentWorld * arena);
	~Pit() {};
	void doSomething();
};

class Goodie : public Actor {
public:
	Goodie(int imageID, double startX, double startY, StudentWorld * arena, Direction dir = 0, int depth = 1);
	virtual ~Goodie() {}
	bool isTangible() const {
		return false;
	}
	bool isKillable() const {
		return true;
	}
	StudentWorld * getArena() const {
		return m_arena;
	}
private:
	StudentWorld * m_arena;
};

class VaccineGoodie : public Goodie {
public:
	VaccineGoodie(double startX, double startY, StudentWorld * arena);
	~VaccineGoodie() {}
	void doSomething();
private:
};

class GasCanGoodie : public Goodie {
public:
	GasCanGoodie(double startX, double startY, StudentWorld * arena);
	~GasCanGoodie() {};
	void doSomething();
};

class LandmineGoodie : public Goodie {
public:
	LandmineGoodie(double startX, double startY, StudentWorld * arena);
	~LandmineGoodie() {};
	void doSomething();
};
class Penelope : public Person {
public:
	Penelope(double start_X, double start_Y, StudentWorld * arena);
	~Penelope();
	void doSomething();
	int getFlame() const {
		return flameCount;
	}
	int getMines() const {
		return mineCount;
	}
	int getVaccines() const {
		return vaccineCount;
	}
	void plusFlame() {
		flameCount += 3;
	}
	void plusMines() {
		mineCount += 2;
	}
	void plusVaccine() {
		vaccineCount++;
	}
	int die() {
		m_arena->playSound(SOUND_PLAYER_DIE);
		Actor::die();
		return 0;
	}
	void shootFire();
private:
	int flameCount;
	int mineCount;
	int vaccineCount;
	StudentWorld * m_arena;


};

class Wall : public Actor {
public: 
	Wall(double x, double y);
	int die() {
		return 0;
	}
	~Wall();
	void doSomething();
};

class Exit : public Actor {
public:
	Exit(double x, double y, StudentWorld * arena);
	~Exit();
	void doSomething();
	bool isTangible() const;
	int die() {
		return 0;
	}
private:
	StudentWorld * m_arena;
};



#endif // ACTOR_H_
