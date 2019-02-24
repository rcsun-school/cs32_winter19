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
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), levelFinished(false){
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
	//ostringstream filename;
	levelFinished = false;
	//filename.fill('0');
	//filename << "level" << setw(2) << getLevel() << ".txt";
	string filename = "level02.txt";
	Level lev(assetPath());
	Level::LoadResult result = lev.loadLevel(filename);
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
				Actor * w = new Wall(i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
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
	oss << "Score: " << setw(6) << getScore(); 
	oss.fill(' ');
	oss << setw(9) << "Level: " << getLevel() << setw(9) << "Lives: " << getLives() << setw(12) << "Vaccines: " << penelope->getVaccines() << setw(10) << "Flames: " << penelope->getFlame()
		<< setw(9) << "Mines: " << penelope->getMines() << setw(12) << "Infected: " << penelope->getInfectionCount();
	setGameStatText(oss.str());
	vector <Actor*>::iterator it = allChar.begin();
	for (; it != allChar.end(); it++) {
		if ((*it)->alive()) {
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
				if ((*it) == this->penelope) {
					levelFinished = true;
				}
			}
		}
	}
}

bool StudentWorld::overlap(double curx, double cury, Actor * it) {
	return ((curx - it->getX()) * (curx - it->getX()) + (cury - it->getY()) * (cury - it->getY()) <= 100);
}


void StudentWorld::hazardOverlap(double curx, double cury) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(curx, cury, *it) && (*it)->isKillable()) {
			(*it)->die();
		}
	}
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

void StudentWorld::generateFlames(double x, double y) {
	for (vector <Actor *>::iterator it = allChar.begin(); it != allChar.end(); it++) {
		if (overlap(x, y, *it) && (*it)->canBlockFlames()) {
			return;
		}
	}
	Actor * f = new Flame(x, y, this);
	newChar.push_back(f);
}