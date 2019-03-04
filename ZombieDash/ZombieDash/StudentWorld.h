#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
class Actor;
class Penelope;
class Agent;

//Class StudentWorld
//Creates the game world and facilitates the creation, deletion, and interactions of game objects with the game and other objects

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
	bool mineOverlap(double curx, double cury, Actor * caller);
	void vomitOverlap(double curx, double cury);
	void generateVaccine(double x, double y);
	void generateFlames(double x, double y, int dir);
	void generateLandmine(double x, double y);
	void generatePit(double x, double y);
	void generateVomit(double x, double y, int dir);
	void generateZombie(double x, double y);
	void moveCitizen(Agent * thisGuy);
	bool goAfterHumanIfApplicable(Agent * zombie, double x, double y);
	bool findCitizens(double x, double y, Actor * caller);
	void decCitizens() {
		numCitizens--;
	}
	void decZombies() {
		numZombies--;
	}
	~StudentWorld();

private:
	std::vector <Actor*> allChar;
	std::vector <Actor *> newChar;
	Penelope * penelope;
	bool levelFinished;
	int level;
	int numCitizens;
	int numZombies;
	double computeNearestDistZombie(double x, double y);
	bool overlap(double curx, double cury, Actor * it);
	double computeNearestDistHuman(double x, double y, Actor * human);
};

#endif // STUDENTWORLD_H_
