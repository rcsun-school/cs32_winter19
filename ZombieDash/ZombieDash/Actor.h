#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
	//base class for all actors within game (Penelope, Persons, Zombies, etc.)
public:
	Actor(int imageID, double startX, double startY);
	virtual ~Actor();
	virtual bool alive() const = 0;
	virtual void doSomething() = 0;
	virtual bool isTangible();
	
private:
};



class Penelope : public Actor{
public:
	Penelope(double start_X, double start_Y, StudentWorld * arena);
	void doSomething();
	bool alive() const;
	bool infected() const;
	int getInfectionCount() const;
private:
	bool isAlive;
	bool isInfected;
	int infectionCount;
	StudentWorld * m_arena;


};

class Wall : public Actor {
public: 
	Wall(double x, double y);
	void doSomething();
	bool alive() const;
};



#endif // ACTOR_H_
