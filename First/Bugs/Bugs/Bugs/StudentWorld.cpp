#include "StudentWorld.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include "Compiler.h"
//#include "GameController.h"
#include <list>
#include <vector>
#include<iostream>
#include<sstream>
#include<iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir), m_Anthill(0), tickCount(0),C0(nullptr),C1(nullptr),C2(nullptr),C3(nullptr)
{
	for (int i = 0; i < 4; i++)
	{
		myAnts[i] = 0;
		myTicks[i] = 0;
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
	if (C0 != nullptr)
		delete C0;
	if (C1 != nullptr)
		delete C1;
	if (C2 != nullptr)
		delete C2;
	if (C3 != nullptr)
		delete C3;
	
}
int StudentWorld::getNumberOfAntsForAnt(int m)
{
	if (m < m_Anthill)
		return myAnts[m];
	else
		return -1;
}
int StudentWorld::getCurrentTicks()
{
	return tickCount;
}
int StudentWorld::getWinningAntNumber()
{
	int max = 0;
	for (int i = 1; i < m_Anthill; i++)
	{
		if (myAnts[i] > myAnts[max])
			max = i;
		else if ((myAnts[i] == myAnts[max]) && (myTicks[i] < myTicks[max]))
			max = i;
	}
	return max;
}
void StudentWorld::eliminateAnAnt(int m_colony)
{
	myAnts[m_colony]--;
}
void StudentWorld::setDisplayText()
{
	int ticks = getCurrentTicks();
	int antsAnt[4];
	int winningAntNumber;
	for (int i =0; i< m_Anthill; i++)
		antsAnt[i] = getNumberOfAntsForAnt(i);
	winningAntNumber = getWinningAntNumber();
	ostringstream oss;
	string s = getTheText(ticks, antsAnt, winningAntNumber);
	setGameStatText(s);
}
string StudentWorld::getTheText(int ticks, int antsAnt[], int winningAntNumber)
{
	ostringstream oss;
	oss.fill(' ');
	oss << "Ticks:" << setw(5) << ticks <<" -  ";
	vector<string>fileNames = getFilenamesOfAntPrograms();
	switch (winningAntNumber)
	{
	case 0:
	{
		oss.fill('0');
		oss << fileNames[0] << "*: "<< setw(2) << antsAnt[0] <<"  ";
		for (int i = 1; i < m_Anthill; i++)
			oss << fileNames[i] << ": " << setw(2) << antsAnt[i] << "  ";
		break;
	}
	case 1:
	{
		oss.fill('0');
		oss << fileNames[0] << ": " << setw(2) << antsAnt[0] << "  ";
		oss << fileNames[1] << "*: " << setw(2) << antsAnt[1] << "  ";
		for (int i = 2; i< m_Anthill; i++)
			oss << fileNames[i] << ": " << setw(2) << antsAnt[i] << "  ";
		break;
	}
	case 2:
	{
		oss.fill('0');
		oss << fileNames[0] << ": " << setw(2) << antsAnt[0] << "  ";
		oss << fileNames[1] << ": " << setw(2) << antsAnt[1] << "  ";
		oss << fileNames[2] << "*: " << setw(2) << antsAnt[2] << "  ";
		if (m_Anthill == 4)
			oss << fileNames[3] << ": " << setw(2) << antsAnt[3] << endl;
		break;
	}
	case 3:
	{
		oss.fill('0');
		oss << fileNames[0] << ": " << setw(2) << antsAnt[0] << "  ";
		oss << fileNames[1] << ": " << setw(2) << antsAnt[1] << "  ";
		oss << fileNames[2] << ": " << setw(2) << antsAnt[2] << "  ";
		oss << fileNames[3] << "*: " << setw(2) << antsAnt[3] << endl;
	}
	}

	string s = oss.str();
	return s;
}
void StudentWorld::poisonTheList(int X, int Y)
{
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
		(*p)->setPoisoned();
}

Actor * StudentWorld::getFirstObjectPointer(int X, int Y)
{
	if (myActors[X][Y].empty())
		return nullptr;
	list <Actor *>::iterator p = myActors[X][Y].begin();
	return (*p);
}
void StudentWorld::removeActor(int X, int Y, Actor* m)
{
	list<Actor *>::iterator p = myActors[X][Y].begin();
	while ((myActors[X][Y].size() != 1) && (p != myActors[X][Y].end()))
	{
		//cerr << "Got in here now-------"<< endl;		
		if ((*p) == m)
			break;
		else
			++p;
	}
	//cerr << "************************Got in here now-------" << endl;
	myActors[X][Y].erase(p);
	//cerr << "notbad"<< endl;
}
void StudentWorld::addActor(int X, int Y, Actor* m)
{
	Actor* p = m;
	myActors[X][Y].push_back(p);
}
bool StudentWorld::addFood(int X, int Y, int m)
{
	if (m > 0)
	{
		Actor* p = getFirstObjectPointer(X, Y);
		if (p == nullptr || (!p->isEdible()))
			myActors[X][Y].push_front(new Food(X, Y, m, this));
		else
			p->increase(m);
		return true;
	}
	return false;	
}
void StudentWorld::produceAnAnt(int image, int pheromoneImage, int X, int Y, int TypeName, Compiler* m_C)
{
	Actor* p = new Ant(image, pheromoneImage, X, Y, TypeName, m_C, this);
	myActors[X][Y].push_back(p);
	myAnts[TypeName]++;
	myTicks[TypeName] = getCurrentTicks();
}
void StudentWorld::emitPheromoneOfMyType(int X, int Y, int m_colony, int m_pheromoneImage)
{
	list<Actor*>::iterator it = myActors[X][Y].begin();
	for (; it != myActors[X][Y].end(); it++)
	{
		if ((*it)->isPheromone() && (*it)->getMyColony() == m_colony)
		{
			if ((*it)->getMyHit() >= 512)
			{
				(*it)->setMyHit(768);
				return;
			}				
			else 
			{
				(*it)->increase(256);
				return;
			}
			
		}
	}
	if (it == myActors[X][Y].end())
	{
		myActors[X][Y].push_back(new Pheromone(m_pheromoneImage, X, Y, m_colony, this));
	}
}

bool StudentWorld::biteAnEnemy(int X, int Y, Actor* a)
{
	vector<Actor*> vi;
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
	{
		if ((*p)->isEnemy() && (*p) != a)
			vi.push_back(*p);
	}
	int m_size = vi.size();
	if (m_size == 0)
		return false;
	int n = randInt(0, m_size - 1);
	vi[n]->setBitten(a);
	return true;
}
bool StudentWorld::withMyAliveHill(int X, int Y, int m_colony)
{
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
	{
		if ((*p)->amAHill() && (*p)->getMyColony() == m_colony && (*p)->checkAlive())
			return true;
	}
	return false;
}
bool StudentWorld::antFindAnEnemy(int X, int Y, int m_colony)
{
	vector<Actor*> vi;
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
	{
		if ((*p)->isEnemy() && (*p)->getMyColony() != m_colony)

		{
			vi.push_back(*p);
			return true;
		}
	}
	return false;
		
}
bool StudentWorld::antBiteAnEnemy(int X, int Y, int m_colony,Actor* pp)
{
	vector<Actor*> vi;
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
	{
		if ((*p)->isEnemy() && (*p)->getMyColony() != m_colony)

		{
			vi.push_back(*p);
		}
	}
	if (vi.size() == 0)
		return false;
	else
	{
		int size = vi.size();
		int n = randInt(0, size - 1);
		vi[n]->setBitten(pp);
		return true;
	}
	
}
bool StudentWorld::hasMyPheromone(int X, int Y, int m_colony)
{
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
	{
		if ((*p)->isPheromone() && ((*p)->getMyColony() == m_colony) && ((*p)->getMyHit() >= 1))
			return true;
	}
	return false;
}
bool StudentWorld::hasDanger(int X, int Y, int m_colony)
{
	Actor* p = nullptr;
	if (antFindAnEnemy(X, Y, m_colony))
		return true;
	list<Actor*>::iterator it = myActors[X][Y].begin();
	{
		for (; it != myActors[X][Y].end(); p++)
		{
			if (((*it)->isDangerous()))
				return true;
		}
	}
	return false;

}
bool StudentWorld::jumpToASquare(int X, int Y, Actor* a)
{
	vector<Coord> vc;
	int left_b = X - 10;
	int right_b = X + 10;
	int up_b = Y + 10;
	int down_b = Y - 10;
	if (left_b < 0)
		left_b = 0;
	if (right_b > 63)
		right_b = 63;
	if (up_b > 63)
		up_b = 63;
	if (down_b < 0)
		down_b = 0;
	for (int i = left_b; i <= right_b; i++)
	{
		for (int j = down_b; j <= up_b; j++)
		{
			if (((X-i)*(X-i) + (Y-j)*(Y-j)) <= 100)
			{
				if (myActors[i][j].empty() || (!myActors[i][j].front()->blockAnt()))
				{
					vc.push_back(Coord(i, j));
				}
			}
		}
	}
	int m_size = vc.size();
	if (m_size == 0)
		return false;
	int n = randInt(0, m_size - 1);
	a->moveTo(vc[n].myX(), vc[n].myY());
	removeActor(X, Y, a);
	addActor(vc[n].myX(), vc[n].myY(), a);
	return true;
}
void StudentWorld::waterTheList(int X, int Y)
{
	list<Actor*>::iterator p = myActors[X][Y].begin();
	for (; p != myActors[X][Y].end(); p++)
		(*p)->setWatered();
}

int StudentWorld::loadMyField()  
{
	Field f;
	string fieldFile = getFieldFilename();
	string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) 
	{
		setError(fieldFile + " " + error);		
		return GWSTATUS_LEVEL_ERROR;
	}
	Compiler* compilerForEntrant[4];
	Anthill* ah;
	vector<string>fileNames = getFilenamesOfAntPrograms();
	int N = fileNames.size();
	m_Anthill = N;
	Field::FieldItem item;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			item = f.getContentsOf(i, j);
			if (item == Field::FieldItem::anthill0)
			{
				if (N >= 1)
				{
					compilerForEntrant[0] = new Compiler;
					if (!compilerForEntrant[0]->compile(fileNames[0], error))
					{
						setError(fileNames[0] + " " + error);
						return GWSTATUS_LEVEL_ERROR;
					}
					ah = new Anthill(i, j, IID_ANT_TYPE0, IID_PHEROMONE_TYPE0, 0, compilerForEntrant[0], this);
					myActors[i][j].push_back(ah);
					C0 = compilerForEntrant[0];
				}
				else
					return GWSTATUS_LEVEL_ERROR;
			}
			else if (item == Field::FieldItem::anthill1)
			{
				if (N >= 2)
				{
					compilerForEntrant[1] = new Compiler;
					if (!compilerForEntrant[1]->compile(fileNames[1], error))
					{
						setError(fileNames[1] + " " + error);
						return GWSTATUS_LEVEL_ERROR;
					}
					ah = new Anthill(i, j, IID_ANT_TYPE1, IID_PHEROMONE_TYPE1, 1, compilerForEntrant[1], this);
					myActors[i][j].push_back(ah);
					C1 = compilerForEntrant[1];
				}
			}
			else if (item == Field::FieldItem::anthill2)
			{
				if (N >= 3)
				{
					compilerForEntrant[2] = new Compiler;
					if (!compilerForEntrant[2]->compile(fileNames[2], error))
					{
						setError(fileNames[2] + " " + error);
						return GWSTATUS_LEVEL_ERROR;
					}
					ah = new Anthill(i, j, IID_ANT_TYPE2, IID_PHEROMONE_TYPE2, 2, compilerForEntrant[2], this);
					myActors[i][j].push_back(ah);
					C2 = compilerForEntrant[2];
				}
			}
			else if (item == Field::FieldItem::anthill3)
			{
				if (N == 4)
				{
					compilerForEntrant[3] = new Compiler;
					if (!compilerForEntrant[3]->compile(fileNames[3], error))
					{
						setError(fileNames[3] + " " + error);
						return GWSTATUS_LEVEL_ERROR;
					}
					ah = new Anthill(i, j, IID_ANT_TYPE3, IID_PHEROMONE_TYPE3, 3, compilerForEntrant[3], this);
					myActors[i][j].push_back(ah);
					C3 = compilerForEntrant[3];
				}
			}
			else if (item == Field::FieldItem::rock)
			{
				Actor* p = new Pebble(i, j, this);
				myActors[i][j].push_back(p);
			}

			else if (item == Field::FieldItem::water)
			{
				myActors[i][j].push_back(new Water(i, j, this));
			}

			else if (item == Field::FieldItem::food)
			{
				myActors[i][j].push_back(new Food(i, j, this));
			}

			else if (item == Field::FieldItem::poison)
			{
				myActors[i][j].push_back(new Poison(i, j, this));
			}
			else if (item == Field::FieldItem::grasshopper)
			{
				myActors[i][j].push_back(new Baby_Grasshopper(i, j, this));
			}
		}
	}	
	cerr << "I'm done loading!" << endl;
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::init()
{

	tickCount = 0;
	
	if (loadMyField() == GWSTATUS_CONTINUE_GAME)
	{			
		return GWSTATUS_CONTINUE_GAME;
	}		
	else
		return GWSTATUS_LEVEL_ERROR;

}

int StudentWorld::move()
{
	setDisplayText();
	tickCount++;	
	cerr << "I have run " << tickCount << " times" << endl;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (!myActors[i][j].empty())
			{

				list<Actor *>::iterator p = myActors[i][j].begin();
				while (p != myActors[i][j].end())
				{

					if ((!(*p)->checkMoved()) && ((*p)->checkAlive()))
					{

						list<Actor *>::iterator q = p;
						p++;
						(*q)->doSomething();

					}
					else
						++p;
				}
			}
		}
	}
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (!myActors[i][j].empty())
			{

				list<Actor *>::iterator p = myActors[i][j].begin();
				while (p != myActors[i][j].end())
				{
					if (!(*p)->checkAlive())
					{
						delete (*p);
						*p = nullptr;
						p = myActors[i][j].erase(p);
					}
					else
					{
						(*p)->setUnmoved();
						++p;
					}
				}
			}
		}
	}
	
	if (tickCount < 2000)
		return GWSTATUS_CONTINUE_GAME;
	else
	{
		if (myAnts[getWinningAntNumber()] > 5)
		{
			vector<string>fileNames = getFilenamesOfAntPrograms();
			setWinner(fileNames[getWinningAntNumber()]);
			return GWSTATUS_PLAYER_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
}
void StudentWorld::cleanUp()
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (!myActors[i][j].empty())
			{
				list<Actor*>::iterator p = myActors[i][j].begin();
				while (p != myActors[i][j].end())
				{
					delete (*p);
					*p = nullptr;
					p = myActors[i][j].erase(p);
				}
			}
			
		}

	}
}