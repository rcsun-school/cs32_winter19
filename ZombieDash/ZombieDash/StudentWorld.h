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
	bool canMoveTo(double end_x, double end_y, Actor * character);
	void checkExit(double curx, double cury);
	~StudentWorld();

private:
	std::vector <Actor*> allChar;
	Penelope * penelope;
	bool levelFinished;
};

#endif // STUDENTWORLD_H_
