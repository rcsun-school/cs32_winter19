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
	void determineBounds(double x, double y, double& lx, double& ly, double& rx, double& ry);

private:
	std::vector <Actor*> allChar;
	Penelope * penelope;
};

#endif // STUDENTWORLD_H_
