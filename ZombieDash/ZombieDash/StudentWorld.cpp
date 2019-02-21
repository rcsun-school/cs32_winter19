#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
	/*1. Initialize the data structures used to keep track of your game’s world.
		2. Allocate and insert a Penelope object into the game world as specified in the
		current level’s data file.
		3. Allocate and insert various wall, pit, goodie, zombie, and exit objects into the
		game world as specified in the current level’s data file. */
	Level lev(assetPath());
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
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
	vector <Actor*>::iterator it = allChar.begin();
	for (; it != allChar.end(); it++) {
		if ((*it)->alive()) {
			(*it)->doSomething();
		}
		if (!penelope->alive()) {
			return GWSTATUS_PLAYER_DIED;
		}
	}
	for (it = allChar.begin(); it != allChar.end(); it++) {
		if (!((*it)->alive())) {
			delete &it;
			it = allChar.erase(it);
		}
	}
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (vector <Actor*>::iterator i = allChar.begin(); i != allChar.end(); i++) {
		delete &i;
		i = allChar.erase(i);
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