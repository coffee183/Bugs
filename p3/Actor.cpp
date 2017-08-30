#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include<cstdlib>
using namespace std;

Actor::Actor(int imageID, int startX, int startY, Direction dir, unsigned int depth, StudentWorld *sw)
	:GraphObject(imageID, startX, startY, dir, depth), m_alive(true),moved(false),my_sw(sw), m_hit(0)
{}
Actor::~Actor()
{}

bool Actor::checkAlive() const
{
	return (m_alive == true);
}
bool Actor::blockAnt() const
{
	return false;
}
bool Actor::isEdible() const
{
	return false;
}

void Actor::setDead()
{
	m_alive = false;
}
bool Actor::isEnemy() const
{
	return false;
}
void Actor::doSomething()
{
	setMoved();
}
Actor* Actor::getAnActorAtTheProposedLocation(int X, int Y) const
{
	return (my_sw->getFirstObjectPointer(X, Y));
}
void Actor::setMoved()
{
	moved = true;
}
void Actor::setUnmoved()
{
	moved = false;
}
bool Actor::checkMoved() const
{
	return (moved == true);
}
StudentWorld * Actor::myWorld() const
{
	return my_sw;
}
void Actor::setMyHit(int m)
{
	m_hit = m;
}

void Actor::decrease(int m)
{
	m_hit -= m;
}
void Actor::increase(int m)
{
	m_hit += m;
}
int Actor::getMyHit() const
{
	return m_hit;
}

Poison::Poison(int startX, int startY, StudentWorld *sw)
	: Actor(IID_POISON, startX, startY, right, 2, sw)
{}

void Poison::doSomething()
{
	setMoved();
	myWorld()->poisonTheList(getX(), getY());
}
Water::Water(int startX, int startY, StudentWorld *sw)
	: Actor(IID_WATER_POOL, startX, startY, right, 2, sw)
{}
void Water::doSomething()
{
	setMoved();
	myWorld()->waterTheList(getX(), getY());
}
Pebble::Pebble(int startX, int startY, StudentWorld* sw)
	:Actor(IID_ROCK, startX, startY, right, 1, sw)
{}
bool Pebble::blockAnt() const 
{
	return true;
}

Food::Food(int startX, int startY, StudentWorld *sw)
	: Actor(IID_FOOD, startX, startY, right, 2, sw)
{
	setMyHit(6000);
}
Food::Food(int startX, int startY, int volumn, StudentWorld *sw)
	: Actor(IID_FOOD, startX, startY, right, 2, sw)
{
	setMyHit(volumn);
}


void Food::doSomething()
{
	setMoved();
	if (getMyHit() <= 0)
	{
		setDead();
	}
}
Pheromone::Pheromone(int image, int startX, int startY, int colonyType, StudentWorld *sw)
	: Actor(image, startX, startY, right, 2, sw), m_colony(colonyType)
{
	setMyHit(256);
}
void Pheromone::doSomething()
{
	setMoved();
	decrease();
	if (getMyHit() <= 0)
		setDead();
}

Insect::Insect(int imageID, int startX, int startY, int volumn, Direction dir, int depth,StudentWorld *sw)
	:Actor(imageID, startX, startY, dir,depth, sw), m_eatOneTime(volumn), m_stunned(false),m_bitten(false),
	remianning_stunned_time(0)
{	
	setMemory(-1, -1);
}
Insect::~Insect()
{	
	myWorld()->addFood(getX(), getY(),100);
}
bool Insect::checkStunned() const
{
	return (m_stunned == true);
}
void Insect::setStunned()
{
	m_stunned = true;
	remianning_stunned_time += 2;
}

bool Insect::goodStatus()
{
	decrease();
	if (getMyHit() <= 0)
	{
		setDead();
		return false;
	}
	if (checkStunned())
	{
		decreaseStunnedTime();
		if (remianning_stunned_time == 0)
			setUnStunned();
		return false;
	}
	return true;

}
Actor* Insect::findFood(int X, int Y) const
{
	Actor* ap = getAnActorAtTheProposedLocation(X, Y);
	if (ap != nullptr)
	{
		if (ap->isEdible())
		{
			return ap;
		}
	}
	return nullptr;
}

void Insect::eat(Actor* a)  //TO DO: eat the food object on this point
{
	
	if (a->getMyHit() >= eatOneTime())
	{
		increase(eatOneTime());
		a->decrease(eatOneTime());
	}
		
	else
	{
		increase(a->getMyHit());
		a->setMyHit(0);
		a->setDead();
	}
}
void Insect::setPoisoned()
{
	if (getX() == getMemoryX() && getY() == getMemoryY())
		return;
	
	decrease(150);
	if (getMyHit() <= 0)
	{
		setDead();
		return;
	}
	setStunned();
	setMemory(getX(), getY());
	//cerr << "I got poisoned!------------now my hit point is " << getMyHit()<<endl;
	
}
void Insect::setWatered()
{
	if (getX() == getMemoryX() && getY() == getMemoryY())
		return;
	setStunned();
	cerr << "I am not gonna move for " << remianning_stunned_time << " ticks because of water" << endl;
	setMemory(getX(), getY());
}
int Insect::eatOneTime() const
{
	return m_eatOneTime;
}

bool Insect::isEnemy() const
{
	return true;
}

void Insect::decreaseStunnedTime()
{
	remianning_stunned_time--;
}
void Insect::setUnStunned()
{
	m_stunned = false;
}
void Insect::setMemory(int X, int Y)
{
	memoryX = X;
	memoryY = Y;
}

int Insect::getMemoryX() const
{
	return memoryX;
}
int Insect::getMemoryY() const
{
	return memoryY;
}
void Insect::pickRandomDirection()
{
	Direction d = Direction(rand() % 4 + 1); 
	setDirection(d);
}

bool Insect::move()
{

	int X = getX();
	int Y = getY();	
	if (getDirection() == left && !isBlocked(getX() - 1, getY()))
		
	{
		moveTo(getX() - 1, getY());
		myWorld()->addActor(X - 1, Y, this);
		myWorld()->removeActor(X, Y, this);		
		return true;
	}
	else if (getDirection() == right && !isBlocked(getX() + 1, getY()))		
	{
		
		moveTo(getX() + 1, getY());		
		myWorld()->addActor(X + 1, Y, this);
		myWorld()->removeActor(X, Y, this);
		return true;
	}
	else if (getDirection() == up && !isBlocked(getX(), getY() + 1))
	{
		moveTo(getX(), getY() + 1);
		myWorld()->addActor(X, Y + 1, this);
		myWorld()->removeActor(X, Y, this);		
		return true;
	}
	else if (getDirection() == down && !isBlocked(getX(), getY() - 1))
	{
		moveTo(getX(), getY() - 1);
		myWorld()->addActor(X, Y - 1, this);
		myWorld()->removeActor(X, Y, this);		
		return true;
	}
	return false;//blocked	
}
void Insect::setBitten(Actor* p)
{
	m_bitten = true;
	decrease(p->myHarm());
	if (getMyHit() <= 0)
	{
		setDead();
		return;
	}
}
bool Insect::isBlocked(int X, int Y) const
{
	Actor* ap = getAnActorAtTheProposedLocation(X, Y);
	if (ap != nullptr)
	{	
		if (ap ->blockAnt())
		{
			return true;
		}
	}
	return false;
}

Anthill::Anthill(int startX, int startY, int colonyType, int antImage, int pheromoneImage, Compiler* C, StudentWorld *sw)
	: Insect(IID_ANT_HILL, startX, startY, 10000, right,2, sw), m_colony(colonyType),m_C(C),
	m_antImage(antImage), m_pheromoneImage(pheromoneImage)
{
	setMyHit(8999);
}
void Anthill::doSomething()
{
	setMoved(); 
	decrease();
	if (getMyHit() <= 0)
	{
		setDead();
		return;
	}
	Actor* ap = findFood(getX(), getY());
	if (ap != nullptr)
	{

		eat(ap);
		return;
	}
	if (getMyHit() >= 2000)
	{
		produce();
	}
}

bool Anthill::move()
{
	return true;
}
void Anthill::produce()
{
	decrease(1500);
	myWorld()->produceAnAnt(m_antImage,m_pheromoneImage,getX(), getY(), m_colony, m_C);
}

Ant::Ant(int image, int pheromoneImage, int startX, int startY, int colonyType, Compiler* p, StudentWorld *sw)
	: Insect(image, startX, startY, 100, right,1, sw), m_colony(colonyType),m_ic(0),
	m_hold(0), last_random_value(0), myC(p), m_pheromoneImage(pheromoneImage),m_blocked(false)
{
	setMyHit(1500);
}
Ant::~Ant()
{
	Insect::~Insect();
	myWorld()->eliminateAnAnt(m_colony);
}

bool Ant::simpleInterpreter()
{
	//cerr << "I'm coming to the interpreter" << endl;
	Compiler::Command cmd;
	int totalCommands = 0;
	while (totalCommands < 10)
	{		
		totalCommands++;
		//cerr << "Currently, m_ic is "<<m_ic << endl;
		if (!myC->getCommand(m_ic, cmd))
		{
			return false;
		}
		switch (cmd.opcode)
		{			
		case Compiler::Opcode::moveForward:
		{
			//cerr << "I need to move forward" << endl;
			m_ic++;
			int X = getX();
			int Y = getY();
			if (move())
			{
				m_blocked = false;
				setUnbitten();
				X = getX();
				Y = getY();
				setMemory(-1, -1);
				return true;
			}
			else
			{
				m_blocked = true;
				X = getX();
				Y = getY();
			}
				
			break;
		}
		case Compiler::Opcode::generateRandomNumber:
		{
			m_ic++;
			if (cmd.operand1 == "0")
				last_random_value = 0;
			else
			{
				int n = stoi(cmd.operand1);
				//cerr << "The operand1 is " << cmd.operand1 <<"----------"<< n << endl;
				last_random_value = randInt(0, n - 1);
			}
			break;
		}
		case Compiler::Opcode::if_command:
		{
			//cerr << "------------------------iiiiiiiiiiiiiiiif --------------command-----------" << endl;
			switch (stoi(cmd.operand1))
			{
			case Compiler::Condition::last_random_number_was_zero:
			{
				if (last_random_value == 0)
				{
					setCounterValue(stoi(cmd.operand2));
					//cerr << "my Last value is 0000000000000000000000000000" << endl;
				}
					
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_am_carrying_food:
			{
				if(m_hold > 0)
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_am_hungry:
			{
				if (getMyHit() <= 25)
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_am_standing_with_an_enemy:
			{
				Actor* p = nullptr;
				if(myWorld()->antFindAnEnemy(getX(),getY(),m_colony,p))
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_am_standing_on_food:
			{
				Actor* p = findFood(getX(), getY());
				if (p!= nullptr)
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_am_standing_on_my_anthill:
			{
				if (myWorld()->withMyAliveHill(getX(),getY(),m_colony))
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_smell_pheromone_in_front_of_me:
			{
				Direction dir = getDirection();
				if (dir == up)
				{
					if (myWorld()->hasMyPheromone(getX(), getY()+1,m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == down)
				{
					if (myWorld()->hasMyPheromone(getX(), getY() - 1, m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == left)
				{
					if (myWorld()->hasMyPheromone(getX()-1, getY(), m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == right)
				{
					if (myWorld()->hasMyPheromone(getX()+1, getY(), m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}				
			}
			case Compiler::Condition::i_smell_danger_in_front_of_me:
			{
				Direction dir = getDirection();
				if (dir == up)
				{
					if (myWorld()->hasDanger(getX(), getY() + 1, m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == down)
				{
					if (myWorld()->hasDanger(getX(), getY() - 1, m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == left)
				{
					if (myWorld()->hasDanger(getX() - 1, getY(), m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
				if (dir == right)
				{
					if (myWorld()->hasDanger(getX() + 1, getY(), m_colony))
						setCounterValue(stoi(cmd.operand2));
					else
						m_ic++;
					break;
				}
			}
			case Compiler::Condition::i_was_bit:
			{
				if (isBitten() == true)
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			case Compiler::Condition::i_was_blocked_from_moving:
			{
				if (m_blocked == true)
					setCounterValue(stoi(cmd.operand2));
				else
					m_ic++;
				break;
			}
			}
			break;
		}
		case Compiler::Opcode::goto_command:
		{
			setCounterValue(stoi(cmd.operand1));
			break;
		}
		case Compiler::Opcode::emitPheromone:
		{
			cerr << "I want to emit" << endl;
			myWorld()->emitPheromoneOfMyType(getX(), getY(), m_colony,m_pheromoneImage);
			m_ic++;
			return true;
			break;
		}
		case Compiler::Opcode::faceRandomDirection:
		{
			pickRandomDirection();
			m_ic++;			
			return true;
			break;
		}
		case Compiler::Opcode::rotateClockwise:
		{
			Direction dir = getDirection();
			dir = Direction((dir + 1) % 4);
			if (dir == none)
				dir = left;
			setDirection(dir);
			m_ic++;
			return true;
			break;
		}
		case Compiler::Opcode::rotateCounterClockwise:
		{
			Direction dir = getDirection();
			dir = Direction((dir - 1) % 4);
			if (dir == none)
				dir = left;
			setDirection(dir);
			m_ic++;
			return true;
			break;
		}
		case Compiler::Opcode::bite:
		{
			Actor* p = nullptr;
			if (myWorld()->antBiteAnEnemy(getX(), getY(), m_colony, p, this))
			{
				m_ic++;
				return true;
			}
			break;
		}
		case Compiler::Opcode::pickupFood:
		{
			m_ic++;
			if (m_hold < 1800)
			{
				Actor* ap = findFood(getX(), getY());
				if (ap != nullptr)
				{
					if (m_hold <= 1400)
					{
						if (ap->getMyHit() >= 400)
						{
							m_hold += 400;
							ap->decrease(400);
						}
						else
						{
							m_hold += ap->getMyHit();
							ap->setMyHit(0);
							ap->setDead();
						}
						return true;
					}
					else
					{
						if (ap->getMyHit() < 1800 - m_hold)
						{
							m_hold += ap->getMyHit();
							ap->setMyHit(0);
							ap->setDead();
						}
						else
						{
							ap->decrease(1800 - m_hold);
							m_hold = 1800;
						}
						return true;
					}					
				}
			}
			break;
			
		}
		case Compiler::Opcode::dropFood:
		{			
			m_ic++;
			if (myWorld()->addFood(getX(), getY(), m_hold))
			{
				m_hold = 0;
				int X = getX();
				int Y = getY();
				return true;
			}
			break;
		}
		case Compiler::Opcode::eatFood:
		{
			m_ic++;
			if (m_hold > 0)
			{
				if (m_hold >= eatOneTime())
				{
					increase(eatOneTime());
					m_hold -= eatOneTime();
				}
				else
				{
					increase(m_hold);
					m_hold = 0;
				}
				return true;
			}
			break;
		}
		}
	}
	return true;
}

void Ant::doSomething()
{
	setMoved();
	if (goodStatus())
	{
		if (!simpleInterpreter())
		{
			setDead();
			return;
		}

	}

}
void Ant::setCounterValue(int m)
{
	m_ic = m;
}

Grasshopper::Grasshopper(int imageID, int startX, int startY, int volumn, StudentWorld *sw) // Volumn is how many units it can eat one time
	: Insect(imageID, startX, startY, volumn, right,1, sw)
{	
	setRandomDis();
	pickRandomDirection();
}
Grasshopper::~Grasshopper()
{}
void Grasshopper::setRandomDis()
{
	remaining_distance = randInt(2, 10);
}
int Grasshopper::getRemainingDistance() const
{
	return remaining_distance;
}
void Grasshopper::setRemainingDis(int m)
{
	remaining_distance = 0;
}
bool Grasshopper::haveARest()
{
	int n = randInt(0, 1);
	if (n == 0)
		return true;
	else
		return false;
}
bool Grasshopper::move()
{
	setStunned();
	if (!Insect::move())
	{		
		setRemainingDis(0);
		pickRandomDirection();
		setRandomDis();
		return false;
	}
	else
	{
		setMemory(-1, -1);
		decreaseRemainingDis();
		setUnbitten();
		return true;
	}
}

Baby_Grasshopper::Baby_Grasshopper(int startX, int startY, StudentWorld *sw)
	: Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, 200, sw)
{
	setMyHit(500);
}

void Baby_Grasshopper::doSomething()
{
	setMoved();
	if (goodStatus())
	{
		if (getMyHit() >= 1600)
		{
			setDead();
			myWorld()->addActor(getX(), getY(), new Adult_Grasshopper(getX(), getY(), myWorld()));
			return;
		}
		Actor* ap = findFood(getX(), getY());
		if (ap!=nullptr)
		{
			
			eat(ap);
			if (haveARest()) 
			{
				setStunned();
				return;
			}
		}
		if (getRemainingDistance() <= 0)
		{
			pickRandomDirection();
			setRandomDis();
		}
		move();
	}
}

Adult_Grasshopper::Adult_Grasshopper(int startX, int startY, StudentWorld *sw)
	: Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, 200, sw)
{
	setMyHit(1600);
}
bool Adult_Grasshopper::decideToBite() const
{
	int n = randInt(0, 2);
	if (n == 0)
		return true;
	else
		return false;
}
void Adult_Grasshopper::setBitten(Actor* p)
{
	Insect::setBitten(p);
	int n = randInt(0, 1);
	if (n == 0)
	{
		myWorld()->biteAnEnemy(getX(), getY(), this);
	}
}

bool Adult_Grasshopper::decideToJump() const
{
	int n = randInt(0, 9);
	if (n == 0)
		return true;
	else
		return false;
}

void Adult_Grasshopper::doSomething()
{
	setMoved();
	if (goodStatus())
	{
		
		if (decideToBite()) // a chance of 1 in 3, it will decide to bite
		{
			
			if (myWorld()->biteAnEnemy(getX(),getY(),this)) //if there is an enemy
			{
				setStunned();
				return;
			}
		}
		
		if (decideToJump())
		{
			if (myWorld()->jumpToASquare(getX(),getY(),this))
			{
				setStunned();
				return;
			}
		}
		Actor* ap = findFood(getX(), getY());
		if (ap != nullptr)
		{
			eat(ap);
			if (haveARest())
			{
				return;
			}
		}
		if (getRemainingDistance() <= 0)
		{
			pickRandomDirection();
			setRandomDis();
		}
		move();
			
		

	}
}