#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
class Actor;
class Penelope;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	Penelope * getPenelope() {
		return penelope;
	}
	bool canMoveTo(double end_x, double end_y, Actor * character);
	void checkExit(double curx, double cury);
	void hazardOverlap(double curx, double cury);
	bool goodieOverlap(double curx, double cury);
	bool overlap(double curx, double cury, Actor * it);
	void generateFlames(double x, double y);
	~StudentWorld();

private:
	std::vector <Actor*> allChar;
	std::vector <Actor *> newChar;
	Penelope * penelope;
	bool levelFinished;
};

#endif // STUDENTWORLD_H_
