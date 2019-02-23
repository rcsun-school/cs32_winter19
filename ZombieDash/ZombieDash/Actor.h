#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
	//base class for all actors within game (Penelope, Persons, Zombies, etc.)
public:
	Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Actor();
	virtual bool alive() const = 0;
	virtual void doSomething() = 0;
	virtual bool isTangible() const;
	virtual bool canBeSaved() const;
	virtual bool isFlammable() const;
	virtual bool zombieFood() const;
private:
	bool isAlive;
};

class Person : public Actor {
public:
	Person(int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Person();
	virtual int die() = 0;
	virtual bool canBeSaved() const;
	virtual bool isFlammable() const;
	virtual bool zombieFood() const;
	virtual bool infected() const;
	virtual	int getInfectionCount() const;
private: 
	bool isInfected;
	int infectionCount;
};

class Zombie : public Actor {
public:
	Zombie(double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Zombie();
	virtual void vomit();
	bool isFlammable() const;

};

class Hazard : public Actor {
public:
	Hazard(int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Hazard();
	bool isTangible() const;
	bool isFlammable() const;

};

class Flame : public Hazard {
public:
	void doSomething();
};

class Goodie : public Actor {
public:
	Goodie(int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
	virtual ~Goodie();
	bool isTangible() const;
	bool isFlammable() const;
};
class Penelope : public Person{
public:
	Penelope(double start_X, double start_Y, StudentWorld * arena);
	~Penelope();
	void doSomething();
	bool alive() const;
	bool infected() const;
	int getInfectionCount() const;
	int die();
	int getFlame() const;
	int getMines() const;
	int getVaccines() const;
private:
	int flameCount;
	int mineCount;
	int vaccineCount;
	int isAlive;
	StudentWorld * m_arena;
	bool isInfected;
	int infectionCount;


};

class Wall : public Actor {
public: 
	Wall(double x, double y);
	~Wall();
	void doSomething();
	bool alive() const;
	bool isFlammable() const;
};

class Exit : public Actor {
public:
	Exit(double x, double y, StudentWorld * arena);
	~Exit();
	void doSomething();
	bool alive() const;
	bool isTangible() const;
	bool isFlammable() const;
private:
	StudentWorld * m_arena;
};



#endif // ACTOR_H_
