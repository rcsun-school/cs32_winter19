#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
	//base class for all actors within game (Penelope, Persons, Zombies, etc.)
public:
	Actor(int imageID, double startX, double startY, StudentWorld * arena, Direction dir = 0, int depth = 0);
	virtual ~Actor();
	virtual bool alive() const {
		return isAlive;
	}
	virtual void doSomething() = 0;
	virtual bool isTangible() const;
	virtual bool canBeSaved() const;
	virtual bool isKillable() const;
	virtual bool zombieFood() const;
	virtual void activateByFlame() {
		return;
	}
	virtual bool scaresCitizens() {
		return false;
	}
	virtual void die();
	virtual void beSaved();
	virtual bool canBlockFlames() const {
		return false;
	}
	virtual bool triggersLandmines() const{
		return true;
	}
	
	void getInfected() {
		m_infected = true;
	}
	bool isInfected() const {
		return m_infected;
	}
	
protected:
	void useVaccine() {
		m_infected = false;
	}

	StudentWorld * getArena() const {
		return m_arena;
	}
private:
	bool isAlive;
	StudentWorld * m_arena;
	bool m_infected;
};


class Agent : public Actor {
public:
	Agent(int imageID, double startX, double startY, StudentWorld * arena, int points_killed, int moveSpace);
	virtual void die() {
		Actor::die();
		getArena()->increaseScore(POINT_VALUE_KILLED);
	}
	virtual bool move(Direction dir);
protected:
	int getTimer() {
		time++;
		return time;
	}
	virtual bool paralyzed() {
		time++;
		if (time%2 == 0) {
			return true;
		}
		else {
			return false;
		}
	}
private:
	int moveSpace;
	int POINT_VALUE_KILLED;
	int time;
};
class Person : public Agent{
public:
	Person(int imageID, double startX, double startY, StudentWorld * arena, int points_saved, int points_killed, int moveSpace);
	virtual ~Person();
	bool canBeSaved() const;
	bool isKillable() const;
	bool zombieFood() const;
	void beSaved() {
		Actor::die();
		getArena()->playSound(SOUND_CITIZEN_SAVED);
		getArena()->increaseScore(POINT_VALUE_SAVED);
		getArena()->decCitizens();
	}
	int getInfectionCount() const;
protected:
	bool checkInfection() {
		if (infectionCount >= 500) {
			return true;
		}
		else {
			if (Actor::isInfected()) {
				infectionCount++;
			}
			else {
				infectionCount = 0;
			}
			return false;
		}
	}

private: 
	int infectionCount;
	int POINT_VALUE_SAVED;
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
		flameCount += 5;
	}
	void plusMines() {
		mineCount += 2;
	}
	void plusVaccine() {
		vaccineCount++;
	}
	void die() {
		getArena()->playSound(SOUND_PLAYER_DIE);
		Actor::die();
		return;
	}
private:
	int flameCount;
	int mineCount;
	int vaccineCount;


};

class Citizen : public Person {
public: 
	Citizen(double startX, double startY, StudentWorld * arena);
	~Citizen() {}
	void doSomething();
	virtual void die();
};

class Zombie : public Agent {
public:
	Zombie(double startX, double startY, int points_killed, StudentWorld * arena);
	virtual ~Zombie();

	bool isKillable() const {
		return true;
	}
	virtual void die() {
		Agent::die();
		getArena()->playSound(SOUND_ZOMBIE_DIE);
		getArena()->decZombies();
	}
	bool vomit();
	bool scaresCitizens() {
		return true;
	}
protected:
	void setMovementPlan(int spaces) {
		m_move = spaces;
	}

	int getMovementPlan() const {
		return m_move;
	}


	bool move(Direction dir) {
		if (Agent::move(dir)) {
			m_move--;
			return true;
		}
		return false;
		
	}


private:
	int m_move;
};

class DumbZombie : public Zombie {
public:
	DumbZombie(double startX, double startY, StudentWorld * arena) : Zombie(startX, startY, 1000, arena) {

	}
	void doSomething();
	void die();
};

class SmartZombie : public Zombie {
public:
	SmartZombie(double startX, double startY, StudentWorld * arena) : Zombie(startX, startY, 2000, arena) {

	}
	void doSomething();
};

class Hazard : public Actor {
public:
	Hazard(int imageID, double startX, double startY, StudentWorld * arena, Direction dir = 0);
	virtual ~Hazard() {

	}
	bool isTangible() const {
		return false;
	}
	bool triggersLandmines() const {
		return false;
	}
protected:
	bool timeUp() {
		if (timer >= 2) {
			die();
			return true;
		}
		else {
			timer++;
			return false;
		}
	}
	int getTimer() const {
		return timer;
	}
	void incTimer() {
		timer++;
	}
	

private:
	int timer;
	int lifespan;

};

class Flame : public Hazard {
public:
	Flame(double startX, double startY, StudentWorld * arena, Direction dir);
	~Flame() {}
	void doSomething();
};

class Landmine : public Hazard {
public:
	Landmine(double startX, double startY, StudentWorld * arena);
	~Landmine() {};
	void doSomething();
	
	void activateByFlame() {
		BOOM();
	}
protected:
	void activate();
	void BOOM();
private:
	bool isActive;
	int safetyTicks;
};


class Vomit : public Hazard {
public:
	Vomit(double startX, double startY, StudentWorld * arena, Direction dir);
	~Vomit() {}
	void doSomething();
};

class Pit : public Hazard {
public:
	Pit(double startX, double startY, StudentWorld * arena);
	~Pit() {};
	void doSomething();
	bool canBlockFlames() {
		return false;
	}
	void die() {
		return;
	}
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
	bool triggersLandmines() const {
		return false;
	}
private:
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


class Wall : public Actor {
public: 
	Wall(double x, double y, StudentWorld * arena);
	void die() {
		return;
	}
	~Wall();
	void doSomething();
	bool canBlockFlames() const {
		return true;
	}
	bool triggersLandmines() const {
		return false;
	}
};

class Exit : public Actor {
public:
	Exit(double x, double y, StudentWorld * arena);
	~Exit();
	void doSomething();
	bool isTangible() const;
	void die() {
		return;
	}
	bool triggersLandmines() const {
		return false;
	}
	bool canBlockFlames() const {
		return true;
	}
private:
};



#endif // ACTOR_H_
