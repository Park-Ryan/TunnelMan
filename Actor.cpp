#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"

using namespace std;

//-------ACTOR-----
Actor::Actor(int imageID, int startX, int startY, Direction intialD, StudentWorld* ptr, double size, int depth, bool display)
	:GraphObject(imageID, startX, startY, intialD, size, depth), alive(true), worldPtr(ptr)
{
	setVisible(display); //by default make visible
}

Actor::~Actor()
{
}


shared_ptr<StudentWorld> Actor::getWorld() 
{
	return worldPtr;
}

bool Actor::getAlive() 
{
	return alive; //return true or false if still have lives
}
bool Actor::isBoulder()
{
	return false;
}
void Actor::setToDead()
{
	alive = false;
}
//------END OF ACTOR-------


//-----EARTH------
Earth::Earth(int startX, int startY) :Actor(TID_EARTH, startX, startY, right, nullptr, 0.25, 3, true)
{
	//does nothing
}

void Earth::doSomething()
{//does nothing
}

Earth::~Earth() {

}
//----------END OF EARTH-----


//---------PERSON---------
Person::Person(int imageID, int x, int y, Direction intialD, StudentWorld* worldPtr, int MAXHP) :
	Actor(imageID, x, y, intialD, worldPtr, 1.0, 0, true), hp(MAXHP) //tunnelman and protester have a size of 1.0 and depth of 0 b/c on top
{
}
int Person::getCurrentHealth()
{
	return hp;
}
//--------END OF PERSON----------



//----------TUNNELMAN---------
TunnelMan::TunnelMan(StudentWorld* worldptr) :
	Person(TID_PLAYER, tunnelMan_x, tunnelMan_y, right, worldptr, tunnelMan_HP), squirts(5), sonar(1), gold(0)
{
}

TunnelMan::~TunnelMan()
{

}

void TunnelMan::doSomething()
{
	/*void Tunnelman::doSomething()  {
	...  int ch;
	if (getWorld()->getKey(ch) == true)
	{
			// user hit a key this tick!
		switch (ch)
		{
			case KEY_PRESS_LEFT:
			check if the direction is left AND check bounds
				... move player to the left ...;
				else leave direction at current point

			break;
			case KEY_PRESS_RIGHT:
			same as left
				... move player to the right ...;
			break;
			case KEY_PRESS_SPACE:
				... add a Squirt in front of the player...; NOT USED IN PART 1
			break;
			 etc…
				}
		 }
			 ...
	 }
	*/
	/*
	-given direction
	-switch cases
	-need to make cases for up down left and right
	-dir is given as Direction
	switch(dir)
	case left:
		move left
	case right:
		move right
	case up:
		move up
	case down:
		move down



	*/
	//PROBLEM: ONLY TURNS RIGHT 
	//FIX: NEEDED TO use setDirection() to change orientation
	// if orientation is wrong then wont move



	int ch;
	if (getWorld()->getKey(ch) == true) {

		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getDirection() == left && getX() > 0 && getWorld()->noBoulder(getX(), getY())) //pos is greater than 0
			{
				moveTo(getX() - 1, getY());
				getWorld()->playSound(SOUND_DIG);
			}

			else moveTo(getX(), getY()); //if out of bounds then reset to the same position
			setDirection(left); //Also if facing wrong direction, set the right way
			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() == right && getX() < earth_row - 4 && getWorld()->noBoulder(getX(), getY())) //position is less than 60
			{
				moveTo(getX() + 1, getY());
				getWorld()->playSound(SOUND_DIG);
			}
			else moveTo(getX(), getY());
			setDirection(right);
			break;

		case KEY_PRESS_DOWN:
			if (getDirection() == down && getY() > 0 && getWorld()->noBoulder(getX(), getY())) //pos is greater than 0
			{
				moveTo(getX(), getY() - 1);
				getWorld()->playSound(SOUND_DIG);

			}
			else moveTo(getX(), getY());
			getWorld()->playSound(SOUND_DIG);

			setDirection(down);
			break;

		case KEY_PRESS_UP:
			if (getDirection() == up && getY() < earth_col - 4) //pos is less than 60
			{
				moveTo(getX(), getY() + 1);
				getWorld()->playSound(SOUND_DIG);
			}
			else  moveTo(getX(), getY());
			setDirection(up);
			break;

		/*case KEY_PRESS_SPACE:
			if (squirts > 0) {
				getWorld()->shoot();
			}
			break;*/


		}

	}


	getWorld()->dig(getX(), getY()); //after break then need to dig


}
int TunnelMan::getNumberOfSquirts()
{
	return squirts;
}
int TunnelMan::getNumberOfGold()
{
	return gold;
}
int TunnelMan::getNumberOfSonar()
{
	return sonar;
}
void TunnelMan::increaseGold()
{
	++gold;
}
//--------END OF TUNNELMAN----------


//--------GOODIE---------------
Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* ptr, int depth, int score, int sounds, bool display, int tick, int maxLife):
	Actor(imageID, startX, startY, right, ptr, default_SIZE, depth, display), ticks(tick), maxTicks(maxLife), sound(sounds)
{

}

bool Goodies::checkIfPickedUp(double distance)
{
	if (distance <= pickableDistance) {
		setToDead();
		getWorld()->playSound(sound);
		getWorld()->increaseScore(score);
		return true;
	}
	return false;
	
}

//-------END OF GOODIE------------------


//---------GOLD NUGGET---------
GoldNugget::GoldNugget(int startX, int startY, StudentWorld* worldPtr, bool display, int depth, bool pick) :
	Goodies(TID_GOLD, startX, startY, worldPtr, 2, GOLD_PICKUP_SCORE, SOUND_GOT_GOODIE, display, 0, 100),  canPickUp(pick)
{
}

void GoldNugget::doSomething()
{
	if (!getAlive())
		return;
	//Checking if this is gold that was hidden originally
	if (canPickUp)
	{
		double distanceFromTunnelMan = getWorld()->tunnelManDistance(getX(), getY());

		if (distanceFromTunnelMan <= 4 && !isVisible())
		{
			setVisible(true);
			return;
		}
		else if (checkIfPickedUp(distanceFromTunnelMan))
		{
			getWorld()->addItems(GOLD_TYPE);
			setVisible(false);
			getWorld()->increaseScore(GOLD_PICKUP_SCORE);
			
		}
	}
	

}


//---------END OF GOLD NUGGET--------


//--------------BOULDER-------------
Boulder::Boulder(int x, int y, StudentWorld *worldPtr):
	Actor(TID_BOULDER, x, y, right, worldPtr, default_SIZE, 1, true), ticks(0)
{

}

void Boulder::falling()
{
	if (getWorld()->noEarth(getX(), getY())) {

		if (getY() > 0) {
			moveTo(getX(), getY() - 1);
		}
		else
			setToDead();
	}


}

void Boulder::doSomething()
{
	if (!getAlive())
		return;

		if (ticks >= 30)
		{
			falling();

			//Playing the sound if this is the initial fall
			if (ticks == 30)
				getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		//++ticks;
}


bool Boulder::isBoulder()
{
	return true;
}



//--------------END OF BOULDER----------


//---------------OIL---------------

Oil::Oil(int x, int y, StudentWorld* worldPtr):
	Goodies(TID_BARREL, x, y, worldPtr,2, OIL_SCORE, SOUND_FOUND_OIL, false,0,0)
{
}

void Oil::doSomething()
{
	if (!getAlive())
		return;
	double distance = getWorld()->tunnelManDistance(getX(), getY());

	if (distance <= 4 && !isVisible()) {
		setVisible(true);
		return;
	}
	else if(checkIfPickedUp(distance)){
		getWorld()->decOil();
		setVisible(false); 
		getWorld()->increaseScore(OIL_SCORE);
	}

}

//-------------END OF OIL-----------------

//------------------WATER------------------------
Water::Water(int x, int y, StudentWorld* worldPtr, int maxTicks) :
	Goodies(TID_WATER_POOL, x, y, worldPtr,2, WATER_SCORE, SOUND_GOT_GOODIE, true,0, maxTicks)
{
}

void Water::doSomething()
{
	double distance = getWorld()->tunnelManDistance(getX(), getY());

}

//-------------END OF WATER----------------------


//---------REGULAR PROTESTER------------
RegularProtester::RegularProtester(StudentWorld* world, int level, int imageID, int maxHP, int squirtDamage) :
	Person(imageID, 60, 60, left, world, maxHP), lvlNum(level), tickCount(0), sqtDamage(squirtDamage), stunned(false)
{

}

void RegularProtester::doSomething()
{
	
	moveIn();

}

void RegularProtester::bribe()
{


}

void RegularProtester::annoy(int damagePts)
{
}

void RegularProtester::leave()
{
	if (getX() == 60 && getY() == 60) {
		setToDead();
	}
	else
	{
		Direction exit = getWorld()->exit(getX(), getY());
		setDirection(exit);
		
	}
}

void RegularProtester::moveIn()
{
	GraphObject::Direction d = up;
	int choice = rand() % 4;
	switch (choice)
	{
	case 0:
	{
		
		d = left;
		break;
	}
	case 1:
	{
		d = up;
		break;
	}
	case 2:
	{
		d = right;
		break;
	}
	case 3:
	{
		d = down;
		break;
	}
	}
	switch (d) {
	case left:
		if (getDirection() == left && getX() > 0 && getWorld()->noBoulder(getX(), getY())) //pos is greater than 0
		{
			moveTo(getX() - 1, getY());
			
		}

		else moveTo(getX(), getY()); //if out of bounds then reset to the same position
		setDirection(left); //Also if facing wrong direction, set the right way
		break;

	case right:
		if (getDirection() == right && getX() < earth_row - 4 && getWorld()->noBoulder(getX(), getY())) //position is less than 60
		{
			moveTo(getX() + 1, getY());
		
		}
		else moveTo(getX(), getY());
		setDirection(right);
		break;

	case down:
		if (getDirection() == down && getY() > 0 && getWorld()->noBoulder(getX(), getY())) //pos is greater than 0
		{
			moveTo(getX(), getY() - 1);
			

		}
		else moveTo(getX(), getY());
		

		setDirection(down);
		break;

	case up:
		if (getDirection() == up && getY() < earth_col - 4) //pos is less than 60
		{
			moveTo(getX(), getY() + 1);
			
		}
		else  moveTo(getX(), getY());
		setDirection(up);
		break;
	}



}








//----------END OF REGULAR PROTESTER---------


//---------------SONAR-------------------
Sonar::Sonar(int x, int y, StudentWorld* worldPtr, int maxTicks) :
	Goodies(TID_SONAR, x, y,worldPtr, 2, SOUND_GOT_GOODIE, true, 75,0,maxTicks)
{
}

void Sonar::doSomething()
{
	if (!getAlive())
		return;
	double distance = getWorld()->tunnelManDistance(getX(), getY());

	if (checkIfPickedUp(distance))
	{
		getWorld()->addItems(SONAR_TYPE);
	}
}


//------------------END OF SONAR----------------

//----------------SQUIRT-------------------
Squirt::Squirt(int x, int y, Direction shootingD, StudentWorld* worldPtr):
	Actor(TID_WATER_SPURT, x, y, shootingD, worldPtr, default_SIZE, 0, false)
{
}

void Squirt::doSomething()
{
	if (!getAlive())
		return;
	double distance = getWorld()->tunnelManDistance(getX(), getY());
	//if(distance<=4)
		
}

//-----------------END OF SQUIRT---------------
