//The following are used to check if there are something in the current point

Actor* hasFood(int X, int Y);
Actor* hasPebble(int X, int Y);

bool isPebble(list<Actor*>::iterator a);

Actor* hasPoison(int X, int Y);

bool isPoison(list<Actor*>::iterator a);

Actor* hasWater(int X, int Y);

bool isWater(list<Actor*>::iterator a);
//

list<Actor*>::iterator find_if(list<Actor*>::iterator b, list<Actor*>::iterator e, bool (StudentWorld::*otherfunction)(Actor*));
bool isFood(Actor* a);
list<Actor*>::iterator StudentWorld::find_if(list<Actor*>::iterator b, list<Actor*>::iterator e,
	bool (StudentWorld::*otherfunction)(Actor*))
{
	for (; b != e; b++)
	{
		if ((isFood)(*b))
			break;
	}
	return b;
}

bool StudentWorld::isFood(Actor* a)
{
	if (a->isEdible())
		return true;
	return false;
}

Actor* StudentWorld::hasFood(int X, int Y)
{
	list<Actor*>::iterator p = find_if(myActors[X][Y].begin(), myActors[X][Y].end(), isFood);
	if (p != myActors[X][Y].end())
		return (*p);
	else
		return nullptr;
}

bool StudentWorld::isPebble(list<Actor*>::iterator a)
{
	if ((*a)->blockAnt())
		return true;
	return false;
}

Actor* StudentWorld::hasPebble(int X, int Y)
{
	list<Actor*>::iterator p = find_if(myActors[X][Y].begin(), myActors[X][Y].end(), isPebble);
	if (p != myActors[X][Y].end())
		return (*p);
	else
		return nullptr;
}

Actor* StudentWorld::hasPoison(int X, int Y)
{
	list<Actor*>::iterator p = find_if(myActors[X][Y].begin(), myActors[X][Y].end(), isPoison);
	if (p != myActors[X][Y].end())
		return (*p);
	else
		return nullptr;
}

bool StudentWorld::isPoison(list<Actor*>::iterator a)
{
	if ((*a)->isPoisonous())
		return true;
	return false;
}

Actor* StudentWorld::hasWater(int X, int Y)
{
	list<Actor*>::iterator p = find_if(myActors[X][Y].begin(), myActors[X][Y].end(), isWater);
	if (p != myActors[X][Y].end())
		return (*p);
	else
		return nullptr;
}

bool StudentWorld::isWater(list<Actor*>::iterator a)
{
	if ((*a)->isWater)
		return true;
	return false;
}