#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
class StudentWorld;


class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir = right, unsigned int depth = 0 , StudentWorld *sw = nullptr);
	virtual ~Actor();
	virtual int eatOneTime() const { return -1; }
	virtual void doSomething() = 0;
	bool checkAlive() const;
	void setDead();
	Actor* getAnActorAtTheProposedLocation(int X, int Y) const;
	StudentWorld * myWorld() const;
	void setMoved();
	void setUnmoved();
	bool checkMoved() const;
	virtual bool blockAnt() const;
	virtual bool isEdible() const;
	void setMyHit(int m);
	void decrease(int m = 1);
	void increase(int m);
	int getMyHit() const;
	virtual void setPoisoned(){}
	virtual void setWatered(){}
	virtual bool isEnemy() const;
	virtual int myHarm() const { return 0; }
	virtual int getMyColony() const { return -1; }
	virtual void setBitten(Actor* p){}
	virtual bool isPheromone() const { return false; }
	virtual bool amAHill() const { return false; }
	virtual bool isDangerous() const { return false; }
private:
	bool m_alive;
	StudentWorld *my_sw;
	bool moved;
	int m_hit;
};
class Poison : public Actor
{
public:
	Poison(int startX, int startY, StudentWorld *sw);
	virtual void doSomething();
	virtual bool isDangerous() const { return true; }
};


class Water : public Actor
{
public:
	Water(int startX, int startY, StudentWorld *sw);
	virtual void doSomething();
	virtual bool isDangerous() const { return true; }

};

class Pebble : public Actor
{
public:
	Pebble(int startX, int startY, StudentWorld *sw);
	virtual void doSomething() { setMoved(); }
	virtual bool blockAnt() const;
};


class Food : public Actor
{
public:
	Food(int startX, int startY , StudentWorld * sw);
	Food(int startX, int startY, int volumn, StudentWorld *sw);
	virtual void doSomething();
	virtual bool isEdible() const { return true; }	
};
class Pheromone: public Actor
{
public:
	Pheromone(int image, int startX, int startY, int colonyType, StudentWorld * sw);
	virtual void doSomething();
	virtual bool isPheromone() const { return true; }
	virtual int getMyColony() const { return m_colony; }
private:
	int m_colony;
};

class Insect : public Actor
{
public:
	Insect(int imageID, int startX, int startY, int volumn, Direction dir = Direction(rand() % 4 + 1), int depth = 1, StudentWorld *sw = nullptr);
	virtual ~Insect();
	virtual void doSomething() = 0;
	bool checkStunned() const;
	virtual void setStunned();		
	virtual int eatOneTime() const;
	virtual void setPoisoned();
	void setUnbitten() { m_bitten = false; }
	bool isBitten() const { return m_bitten == true; }
	void decreaseStunnedTime();
	void setUnStunned();
	bool goodStatus();
	virtual bool move();
	void setMemory(int X, int Y);
	int getMemoryX() const;
	int getMemoryY() const;
	void pickRandomDirection();
	bool isBlocked(int X, int Y) const;
	Actor* findFood(int X, int Y) const;
	void eat(Actor* a);
	virtual void setWatered();
	virtual bool isEnemy() const;
	virtual void setBitten(Actor* p);
	
	
private:
	bool m_stunned;
	int remianning_stunned_time;	
	int m_eatOneTime;
	bool m_bitten;
	int memoryX;
	int memoryY;
	
};
class Anthill : public Insect
{
public:
	Anthill(int startX, int startY, int colonyType, int antImage, int pheromoneImage, Compiler* C, StudentWorld *sw);
	virtual void doSomething();
	virtual bool move();
	virtual void setStunned(){}
	virtual void setPoisoned(){}
	virtual void setWatered(){}
	virtual bool isEnemy() const { return false; }
	void produce();
	virtual void setBitten(Actor* p){}
	virtual bool amAHill() const { return true; }
	virtual int getMyColony() const { return m_colony; }
private:
	int m_colony;
	int m_antImage;
	int m_pheromoneImage;
	Compiler* m_C;
	// compile object
};

class Ant : public Insect
{
public:
	Ant(int image, int pheromoneImage,int startX, int startY, int colonyType,Compiler* p , StudentWorld *sw);
	virtual ~Ant();
	virtual void doSomething();
	virtual int myHarm() const { return 15; }
	bool simpleInterpreter();
	void setCounterValue(int m);
	virtual int getMyColony() const { return m_colony; }
private:
	int m_colony;
	int last_random_value;
	int m_hold;
	int m_pheromoneImage;
	Compiler *myC;
	int m_ic;
	bool m_blocked;
	
};
class Grasshopper : public Insect
{
public:
	Grasshopper(int imageID, int startX, int startY, int volumn, StudentWorld *sw); // Volumn is how many units it can eat one time
	virtual ~Grasshopper();
	virtual void doSomething() = 0;
	virtual bool move();
	bool haveARest();
	void decreaseRemainingDis() { remaining_distance--; }
	void setRandomDis();
	int getRemainingDistance() const;
	void setRemainingDis(int m);
private:
	int remaining_distance;
};
class Baby_Grasshopper : public Grasshopper
{
public:
	Baby_Grasshopper(int startX, int startY, StudentWorld *sw);
	virtual void doSomething();
	
};
class Adult_Grasshopper : public Grasshopper
{
public:
	Adult_Grasshopper(int startX, int startY, StudentWorld *sw);
	virtual void doSomething();
	virtual void setPoisoned(){}
	virtual void setWatered(){}
	bool decideToBite() const;
	bool decideToJump() const;
	virtual int myHarm() const { return 50; }
	virtual void setBitten(Actor* p);	
};
#endif // ACTOR_H_
