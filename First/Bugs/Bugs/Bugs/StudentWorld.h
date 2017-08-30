#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"

#include <list>
#include <string>
class Actor;
class Field;
class Compiler;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();
	virtual int init();

	virtual int move();

	virtual void cleanUp();

	int loadMyField();
	Actor* getFirstObjectPointer(int X, int Y);
	void removeActor(int X, int Y, Actor* m);
	void addActor(int X, int Y, Actor* m);
	void poisonTheList(int X, int Y);
	bool addFood(int X, int Y, int m);
	void waterTheList(int X, int Y);
	bool biteAnEnemy(int X, int Y, Actor* a);
	bool jumpToASquare(int X, int Y, Actor* a);
	int getNumberOfAntsForAnt(int m);
	int getCurrentTicks();
	int getWinningAntNumber();
	void produceAnAnt(int image, int pheromoneImage,int X, int Y, int TypeName, Compiler* m_C);
	bool antBiteAnEnemy(int X, int Y, int m_colony,Actor* pp);
	bool antFindAnEnemy(int X, int Y, int m_colony);
	void emitPheromoneOfMyType(int X, int Y, int m_colony, int m_pheromoneImage);
	bool withMyAliveHill(int X, int Y, int m_colony);
	bool hasMyPheromone(int X, int Y, int m_colony);
	bool hasDanger(int X, int Y, int m_colony);
	void setDisplayText();
	std::string getTheText(int ticks, int antsAnt[], int winningAntNumber);
	void eliminateAnAnt(int m_colony);

private:
	std::list<Actor*> myActors[64][64];
	int m_Anthill;
	int tickCount;
	int myAnts[4];
	int myTicks[4];
	Compiler* C1;
	Compiler* C2;
	Compiler* C3;
	Compiler* C0;

};
class Coord
{
public:
	Coord(int x, int y):m_x(x),m_y(y){}
	int myX() { return m_x; }
	int myY() { return m_y; }
private:
	int m_x;
	int m_y;	
};


#endif // STUDENTWORLD_H_
