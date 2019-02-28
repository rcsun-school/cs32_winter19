#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), levelFinished(false), level(1), numCitizens(0), numZombies(0){
}

StudentWorld::~StudentWorld() {
	this->cleanUp();
}

int StudentWorld::init()
{
	/*1. Initialize the data structures used to keep track of your game’s world.
		2. Allocate and insert a Penelope object into the game world as specified in the
		current level’s data file.
		3. Allocate and insert various wall, pit, goodie, zombie, and exit objects into the
		game world as specified in the current level’s data file. */
	ostringstream filename;
	levelFinished = false;
	filename.fill('0');
	filename << "level" << setw(2) << getLevel() << ".txt";
	Level lev(assetPath());
	Level::LoadResult result = lev.loadLevel(filename.str());
	if (result == Level::load_fail_file_not_found || level > 99) {
		return GWSTATUS_PLAYER_WON;
	}
	if (result == Level::load_fail_bad_format) {
		return GWSTATUS_LEVEL_ERROR;
	}
		for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			switch (lev.getContentsOf(i, j)) {
			case Level::empty:
				break;
			case Level::player:
			{
				this->penelope = new Penelope(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				Actor * p = penelope;
				allChar.push_back(p);
			}
			break;
			case Level::wall:
			{
				Actor * w = new Wall(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(w);
			}
			break;
			case Level::exit: 
			{
				Actor * e = new Exit(i * SPRITE_WIDTH, j*SPRITE_HEIGHT, this);
				allChar.push_back(e);

			}
			break;
			case Level::vaccine_goodie:
			{
				Actor * v = new VaccineGoodie(i * SPRITE_WIDTH, j*SPRITE_HEIGHT, this);
				allChar.push_back(v);
			}
			break;
			case Level::gas_can_goodie:
			{
				Actor * g = new GasCanGoodie(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(g);
			}
			break;
			case Level::landmine_goodie: 
			{
				Actor * l = new LandmineGoodie(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(l);
			}
			break;
			case Level::pit: 
			{
				Actor * p = new Pit(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(p);
			}
			break;
			case Level::citizen: 
			{
				Actor * c = new Citizen(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(c);
				numCitizens++;
			}
			break;
			case Level::dumb_zombie:
			{
				Actor * dz = new DumbZombie(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(dz);
				numZombies++;
			}
			break;
			case Level::smart_zombie:
			{
				Actor * sz = new SmartZombie(i * SPRITE_WIDTH, j * SPRITE_HEIGHT, this);
				allChar.push_back(sz);
				numZombies++;
			}
			default:
				break;
			}
		}
	}
	
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	ostringstream oss;
	oss.fill('0');
	if (getScore() >= 0) {
		oss << "Score: " << setw(6) << getScore();
	}
	else {
		oss << "Score: " << "-" << setw(5) << (getScore() * -1);
	}
	oss.fill(' ');
	oss << setw(9) << "Level: " << getLevel() << setw(9) << "Lives: " << getLives() << setw(12) << "Vaccines: " << penelope->getVaccines() << setw(10) << "Flames: " << penelope->getFlame()
		<< setw(9) << "Mines: " << penelope->getMines() << setw(12) << "Infected: " << penelope->getInfectionCount();
	setGameStatText(oss.str());
	vector <Actor*>::iterator it = allChar.begin();
	for (; it != allChar.end(); it++) {
		if ((*it)->alive()) { //checks if actor is alive before calling doSomething()
			(*it)->doSomething();
		}
		if (!penelope->alive()) {
			return GWSTATUS_PLAYER_DIED;
		}
		if (levelFinished) {
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
		for (vector<Actor*>::iterator n = newChar.begin(); n != newChar.end();) {
			allChar.push_back(*n);
			n = newChar.erase(n);
		}
		for (it = allChar.begin(); it != allChar.end(); it++) {
		if (!((*it)->alive())) {
			Actor * killme = *it;
			it = allChar.erase(it);
			delete killme;
			it--;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	vector <Actor*>::iterator i = allChar.begin();
	while (!allChar.empty()) {
		Actor * killme = *i;
		i = allChar.erase(i);
		delete killme;
	}

	if (levelFinished) {
		level++;
	}
	else {
		decLives();
	}
}


bool StudentWorld::canMoveTo(double end_x, double end_y, Actor * character) {
	//the four corners of the moving actor's bounding box cannot enter the bounding box of any other tangible actors on the field. 
	for (vector <Actor *>::iterator i = allChar.begin(); i != allChar.end(); i++) {
		if ((*i)->isTangible() && *i != character) {
			if ((end_x >= (*i)->getX() && end_x <= (*i)->getX() + SPRITE_WIDTH - 1 && end_y >= (*i)->getY() && end_y <= (*i)->getY() + SPRITE_HEIGHT - 1)
				|| (end_x + SPRITE_WIDTH - 1 >= (*i)->getX() && end_x + SPRITE_WIDTH - 1 <= (*i)->getX() + SPRITE_WIDTH - 1 && end_y + SPRITE_HEIGHT - 1 >= (*i)->getY() && end_y + SPRITE_HEIGHT - 1 <= (*i)->getY() + SPRITE_HEIGHT - 1)
				|| (end_x + SPRITE_WIDTH - 1 >= (*i)->getX() && end_x + SPRITE_WIDTH - 1 <= (*i)->getX() + SPRITE_WIDTH - 1 && end_y >= (*i)->getY() && end_y <= (*i)->getY() + SPRITE_HEIGHT - 1)
				|| (end_x >= (*i)->getX() && end_x <= (*i)->getX() + SPRITE_WIDTH - 1 && end_y + SPRITE_HEIGHT - 1 >= (*i)->getY() && end_y + SPRITE_HEIGHT - 1 <= (*i)->getY() + SPRITE_HEIGHT - 1))
			{
				return false;
			}
		}
	}
	return true;

}

void StudentWorld::checkExit(double curx, double cury) {
	for (vector<Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it)) {
			if ((*it)->canBeSaved()) {
				if ((*it) == this->penelope && numCitizens == 0) {
					playSound(SOUND_CITIZEN_SAVED);
					levelFinished = true;
				}
				else if (*it != penelope) {
					increaseScore((*it)->beSaved());
				}
				else {
					continue;
				}
				
			}
		}
	}
}


bool StudentWorld::overlap(double curx, double cury, Actor * it) {
	return ((curx - it->getX()) * (curx - it->getX()) + (cury - it->getY()) * (cury - it->getY()) <= 100);
}


void StudentWorld::flameOverlap(double curx, double cury) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it)) {
			(*it)->activateByFlame();
			if ((*it)->isKillable()) {
					(*it)->die();
			}
		}
	}
}


void StudentWorld::vomitOverlap(double curx, double cury, Actor * caller) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it)) {
			if ((*it)->zombieFood()) {
				(*it)->getInfected();
			}
		}
	}
}

bool StudentWorld::findCitizens(double x, double y, Actor * caller) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(x, y, *it) && (*it)->zombieFood() && randInt(1, 3) == 1) {
			generateVomit(x, y, caller->getDirection());
			return true;
		}
	}
	return false;
}

bool StudentWorld::goodieOverlap(double curx, double cury) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it) && *it == penelope) {
			increaseScore(50);
			playSound(SOUND_GOT_GOODIE);
			return true;
		}
	}
	return false;
}


bool StudentWorld::mineOverlap(double curx, double cury, Actor * caller) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it) && *it != caller && (*it)->triggersLandmines()) {
			return true;
		}
	}
	return false;
}

void StudentWorld::generateFlames(double x, double y, int dir) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(x, y, *it) && (*it)->canBlockFlames()) {
			return;
		}
	}
	Actor * f = new Flame(x, y, this, dir);
	newChar.push_back(f);
}

void StudentWorld::generateLandmine(double x, double y) {
	Actor * l = new Landmine(x, y, this);
	newChar.push_back(l);
}

void StudentWorld::generatePit(double x, double y) {
	Actor * p = new Pit(x, y, this);
	newChar.push_back(p);
}


void StudentWorld::generateVomit(double x, double y, int dir) {
	Actor * v = new Vomit(x, y, this, dir);
	newChar.push_back(v);
	playSound(SOUND_ZOMBIE_VOMIT);
}

void StudentWorld::generateZombie(double x, double y) {
	int chance = randInt(1, 10);
	if (chance > 7) {
		Actor * z = new SmartZombie(x, y, this);
		newChar.push_back(z);
	}
	else {
		Actor * z = new DumbZombie(x, y, this);
		newChar.push_back(z);
	}
	playSound(SOUND_ZOMBIE_BORN);
}


void StudentWorld::moveCitizen(Agent * thisGuy) {
	double distP = sqrt(pow(thisGuy->getX() - penelope->getX(), 2) + pow(thisGuy->getY() - penelope->getY(), 2));
	double distZ = computeNearestDistZombie(thisGuy->getX(), thisGuy->getY());
	if (distP < distZ && distP <= 80) {
		if (penelope->getX() == thisGuy->getX()) {
			if (penelope->getY() > thisGuy->getY()) {
				if (thisGuy->move(90)) {
					return;
				}
			}
			else {
				if (thisGuy->move(270)) {
					return;
				}
			}
		}
		else if (penelope->getY() == thisGuy->getY()) {
			if (penelope->getX() > thisGuy->getX()) {
				if (thisGuy->move(0)) {
					return;
				}
			}
			else {
				if (thisGuy->move(180)) {
					return;
				}
			}
		}
		else {
			if (penelope->getY() > thisGuy->getY()) {
				if (thisGuy->move(90)) {
					return;
				}
			}
			if (penelope->getX() > thisGuy->getX()) {
				if (thisGuy->move(0)) {
					return;
				}
			}
			if (penelope->getY() < thisGuy->getY()) {
				if (thisGuy->move(270)) {
					return;
				}
			}
			if (penelope->getX() < thisGuy->getX()) {
				if (thisGuy->move(180)) {
					return;
				}
			}
		}
	}
	else if (distZ <= 80) {
		Direction bestDirection = -1;
		double bestSpot = distZ;
		double posNearbyZombie;
		if (canMoveTo(thisGuy->getX() + 2, thisGuy->getY(), thisGuy)) {
			posNearbyZombie = computeNearestDistZombie(thisGuy->getX() + 2, thisGuy->getY());
			if (posNearbyZombie > bestSpot) {
				bestDirection = 0;
				bestSpot = posNearbyZombie;
			}
		}
		if (canMoveTo(thisGuy->getX(), thisGuy->getY() + 2, thisGuy)) {
			posNearbyZombie = computeNearestDistZombie(thisGuy->getX(), thisGuy->getY() + 2);
			if (posNearbyZombie > bestSpot) {
				bestDirection = 90;
				bestSpot = posNearbyZombie;
			}
		}
		if (canMoveTo(thisGuy->getX() - 2, thisGuy->getY(), thisGuy)) {
			posNearbyZombie = computeNearestDistZombie(thisGuy->getX() - 2, thisGuy->getY());
			if (posNearbyZombie > bestSpot) {
				bestDirection = 180;
				bestSpot = posNearbyZombie;
			}
		}
		if (canMoveTo(thisGuy->getX(), thisGuy->getY() - 2, thisGuy)) {
			posNearbyZombie = computeNearestDistZombie(thisGuy->getX(), thisGuy->getY() - 2);
			if (posNearbyZombie > bestSpot) {
				bestDirection = 270;
				bestSpot = posNearbyZombie;
			}
		}
		if (bestDirection != -1) {
			thisGuy->move(bestDirection);
		}
		
	}
	else {
		return;
	}
}

bool StudentWorld::goAfterHumanIfApplicable(Agent * zombie, double x, double y) {
	Actor * human = penelope;
	double distH = computeNearestDistHuman(x, y, human);
	if (distH <= 80) {
		if (human->getX() == x) {
			if (human->getY() > y) {
				if (canMoveTo(x, y + 1, zombie)) {
					zombie->setDirection(90);
					return true;
				}
			}
			else {
				if (canMoveTo(x, y - 1, zombie)) {
					zombie->setDirection(270);
					return true;
				}
			}
		}
		else if (human->getY() == zombie->getY()) {
			if (human->getX() > zombie->getX()) {
				if (canMoveTo(x + 1, y, zombie)) {
					zombie->setDirection(0);
					return true;
				}
			}
			else {
				if (canMoveTo(x - 1, y, zombie)) {
					zombie->setDirection(180);
					return true;
				}
			}
		}
		else {
			if (human->getY() > zombie->getY()) {
				if (canMoveTo(x + 1, y, zombie)) {
					zombie->setDirection(0);
					return true;
				}
			}
			if (human->getX() > zombie->getX()) {
				if (canMoveTo(x, y + 1, zombie)) {
					zombie->setDirection(90);
					return true;
				}
			}
			if (human->getY() < zombie->getY()) {
				if (canMoveTo(x - 1, y, zombie)) {
					zombie->setDirection(180);
					return true;
				}
			}
			if (human->getX() < zombie->getX()) {
				if (canMoveTo(x, y - 1, zombie)) {
					zombie->setDirection(270);
					return true;
				}
			}
		}
	}
	return false;

}

double StudentWorld::computeNearestDistZombie(double x, double y) {
	if (numZombies == 0) {
		return VIEW_HEIGHT * VIEW_WIDTH; //need some absurdly large number so our nonexistent zombie value does not affect calculations
	}
	double distZ = VIEW_HEIGHT * VIEW_WIDTH;
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if ((*it)->scaresCitizens() && sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2)) < distZ) {
			distZ = sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2));
		}
	}
	return distZ;
}

double StudentWorld::computeNearestDistHuman(double x, double y, Actor * human) {
	if (numCitizens == 0) {
		return VIEW_HEIGHT * VIEW_WIDTH; //need some absurdly large number so our nonexistent zombie value does not affect calculations
	}
	double distH = VIEW_HEIGHT * VIEW_WIDTH;
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if ((*it)->zombieFood() && sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2)) < distH) {
			distH = sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2));
			human = *it;
		}
	}
	return distH;
}
