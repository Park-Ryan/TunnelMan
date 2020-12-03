//Authors: Ryan Park and Kyle Ngyuen
// CS30: Project 3
// Part 1: Finished 12/3 

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>


//DISTRIBUTING GOLD, BOULDER, OIL
const int max_X = 60;
const int max_Y = 56;

//Game dimension defaults (const)
const int objectSize = 4; //each object has a size of 4: the Tunnelman, Protesters, Boulders, gold, oil and sonar
//TUNNELMAN
const int tunnelStart_col = 30; //col must occupy from 30-33
const int tunnelEnd_col = 33;
const int tunnelStart_row = 4; //row must occupy from 0-59
const int tunnelEnd_row = 59;
const int tunnelMan_x = 30; //tunnel man must 
const int tunnelMan_y = 60;// start at x = 30 and y = 60
const int tunnelMan_HP = 10; //tunnelMan starts at 100% health

//SCORES
const int OIL_SCORE = 1000;
const int GOLD_PICKUP_SCORE = 10;
const int WATER_SCORE = 100;

//EARTH FIELD
const int earth_col = 64;
const int earth_row = 64; //earth field 64*64

//Objects in game
const double default_SIZE = 1.0; //everything except earth
const int BOULDER_TYPE = 1;
const int GOLD_TYPE = 2;
const int OIL_TYPE = 3;
const int SONAR_TYPE = 4;
const int SQUIRT_TYPE = 5;
const int GOLD_LIFE = 100; //gold nugget temp life time, 100 ticks

//PICKUP-able distance
const double pickableDistance = 3.0;

const double radius = 3.0;

//forward declaration
class Actor;
class Earth;
class TunnelMan;
class Person;
class Goodies;
class GoldNugget;
class Water;
class Boulder;
class Oil;
class RegularProtester;


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	virtual int init(); //Create new game


	virtual int move(); //Needed for game to function

	virtual void cleanUp(); //DESTROYS 

	//Create the Earth
	void createEarth();

	void addGameObjects(); //will call createGold_Boulders_Oil
							//Needed to distribute OIL, BOULDERS, GOLD

	void add_Sonar_Water(); // needed to distribute sonar and water on the field

	//Make objects-gold and boulders needed to distribute
	//passing type and number of objects
	void createGold_Boulders_Oil(int numObjects, int type);

	//NEED TO MAKE RANDOM COORDINDATES and pass t/f for if boulder or not
	void randomCoordinates(int X, int Y, bool isBoulder);

	//check if any earth is blocking in any direction
	//going to be used for the protester to check if they can go down the certain path
	bool noEarth(int x, int y);


	bool dig(int x, int y); //TunnelMan will call to dig earth
							//out of 4x4 square

	//function to delete the earth around the objects
	void deleteEarth(int x, int y);

	

	//calculate the distance between two points
	//this is going to be used for the check if you can add an actor within the 6.0 distance radius
	double calculateDistance(double x1, double y1, double x2, double y2);
	
	//get the distance between the tunnelman and another object at a given point
	//NEeded for oil 
	double distanceFromPlayer(double x, double y);

	
	//Reduce the barrel count
	void decOil();

	

	bool noBoulder(int x, int y);

	//keep track of all the oil
	int OilCount; 

	//using calculate distance to get distance from tunnelman to object at x and y
	double tunnelManDistance(const int x, const int y);

	bool radiusIsclear(const int x, const int y);

	//Display Stat
	void setDisplayText();

	void addProtester();

	void addItems(int type);

	void shoot();

	GraphObject::Direction exit(int x, int y);

	~StudentWorld();


private:


	//Earth Container that will hold earth field-made of unique pointers
	std::unique_ptr<Earth> earthContainer[earth_row][earth_col];

	//creates the player
	//Needed to display the tunnelman
	std::unique_ptr<TunnelMan> player;

	std::vector<std::unique_ptr<Actor>> gameItems; //going to hold-gold, sonar, water, boulder, protesters


};

#endif // STUDENTWORLD_H_
