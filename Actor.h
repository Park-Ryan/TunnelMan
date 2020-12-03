#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

#include "GameConstants.h"
#include "StudentWorld.h"
#include <memory>
#include <string>

//Forward Declaration
class StudentWorld;



//will serve as the base class for the objects in the game
//included is the Tunnelman, Regular Protesters, Hardcore Protesters,
//Goodies (e.g., Sonar Kits, Gold Nuggets, Water, Barrels of oil) 
class Actor :public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction intialD, StudentWorld *ptr, double size, int depth, bool display);

	//destructor
	virtual ~Actor();

	//will be used by other classes
	virtual void doSomething() = 0;

	//needed to run the game
	bool getAlive();

	virtual bool isBoulder();
	//this will set the objects to dead
	void setToDead();


	

	//changed to shared pointer
	std::shared_ptr<StudentWorld> getWorld();


	

private:
	//StudentWorld* worldPtr; //pointer for StudentWorld
							//make a smart pointer
	//Make shared pointer for student world
	std::shared_ptr<StudentWorld> worldPtr = std::make_shared<StudentWorld>();


	bool alive;

};
//EARTH CLASS
class Earth : public Actor {
public:
	Earth(int startX, int startY); //only need to use the start x and y
	~Earth();
	void doSomething(); //virtual from actor
};

//Person class for people in the game derived from actor
class Person : public Actor {
public:
	
	Person(int imageID, int x, int y, Direction intialD, StudentWorld* worldPtr, int maxHealth);
	int getCurrentHealth();
	

private:
	int hp; //keep hp seperated from each other
};

class TunnelMan :public Person {
public:
	TunnelMan(StudentWorld *worldPtr); 
	~TunnelMan();

	void doSomething(); //virtual from actor
	
	int getNumberOfSquirts();
	int getNumberOfGold();
	int getNumberOfSonar();

	void increaseGold();
	

private:
	
	//NOT used in part 1
	int squirts;
	int sonar;
	int gold;

	//initialize in part 2
};


// Goodie objects :(like Water, Gold Nuggets, Barrels of oil), Squirt objects, etc.)
class Goodies :public Actor {
public:
	Goodies(int imageID, int startX, int startY, StudentWorld* ptr, int depth, int score, int sound, bool display, int ticks, int maxTicks);

	//check if the tunnelman picked up the goodie
	//will play sound and update invetory
	//returns true
	// else return false if did not pick up
	bool checkIfPickedUp(double distance);

private:
	int sound; //keeps track of which sound to play when a certain action is done
	int score; // keeps track of the score to be added to the tunnelman 

	int ticks; //keeps track of the current ticks(unit of time)
	int maxTicks; //keeps track of how many ticks can pass before the goodie will die
					


};


class GoldNugget:public Goodies{
public:
	GoldNugget(int startX, int startY, StudentWorld* worldPtr, bool display, int depth, bool pick);
	void doSomething() override;


private:
	bool canPickUp;
};


class Boulder :public Actor {
public:
	Boulder(int x, int y, StudentWorld* worldPtr);

	void falling(); //falling functio will continue to decrease the y until hits next earth or y< 0
	void doSomething()override;
	bool isBoulder(); //need to return true for boulder

private:
	int ticks;
	bool earthBelow; //is true until tunnelman digs out the earth

};

class Oil :public Goodies{
public: 
	Oil(int x, int y, StudentWorld* worldPtr);
	void doSomething()override;

};


class Water :public Goodies {
public:
	Water(int x, int y, StudentWorld* worldPtr, int maxTicks);
	
	void doSomething()override;

};
class Sonar :public Goodies {
public:
	Sonar(int x, int y, StudentWorld* worldPtr, int maxTicks);
	void doSomething()override;

};

class RegularProtester :public Person {
public:
	RegularProtester(StudentWorld* worldPtr, int level, int imageID = TID_PROTESTER, int maxHP = 5, int squirtDamage = 100);

	virtual void doSomething() override;
	virtual void bribe();
	void annoy(int damagePts);

	void leave();

private:
	int sqtDamage;
	void moveIn();
	bool stunned;
	int lvlNum;
	int tickCount;
	//void randomDirection();
	//virtual bool detectTunnelMan();


};


class Squirt : public Actor {
public:
	Squirt(int x, int y, Direction shootingD, StudentWorld* worldPtr);
	void doSomething();


};
#endif // ACTOR_H_