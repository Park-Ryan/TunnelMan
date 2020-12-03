
#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}




void StudentWorld::decOil()
{
	//gets the current object oil count and will decrent
	--this->OilCount;
}

bool StudentWorld::noBoulder(int x, int y)
{
	double distance;
	for (std::vector<unique_ptr<Actor>>::const_iterator it = gameItems.begin(); it != gameItems.end(); ++it)
	{
		if ((*it)->isBoulder())
		{
			distance = calculateDistance(x, y, (*it)->getX(), (*it)->getY());
			if (distance <= 3.0)
				return false;   
		}
	}
	return true;   
}


StudentWorld::~StudentWorld() {
	cleanUp(); //deletes the earth created
	player.reset(); //deletes the tunnelMan created along with anything it has

}


int StudentWorld::init() {
	createEarth(); //calling earth to create
	addGameObjects();
	player = unique_ptr<TunnelMan>(new TunnelMan(this));//adds the player to the game
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	while (player->getAlive()) {
		player->doSomething(); //NEEEDED for tunnelMan to move and dig
		for (std::vector<unique_ptr<Actor>>::iterator it = gameItems.begin(); it != gameItems.end(); ++it)
			(*it)->doSomething();
		add_Sonar_Water();
		addProtester();
		setDisplayText();
	}
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//Similar to dig, but will remove when cleanup is called
	//Done this way instead of deleting right away
	for (int i = 0; i < earth_row; i++) {
		for (int j = 0; j < earth_row; j++) {
			earthContainer[i][j].reset();

		}
	}
}

//Create the earth
//using 2d unique array
/*
2 for loops
	1 for row
		1 for col
			check- if row and col conflict with the tunnel
			if not add earth row and col points to the unique earth
*/
void StudentWorld::createEarth() {
	for (int i = 0; i < earth_row; i++) {
		for (int j = 0; j < earth_col - 4; j++) {
			if (i <tunnelStart_col || i>tunnelEnd_col || j < tunnelStart_row) //col 30-33, row 4
			{
				earthContainer[i][j] = std::unique_ptr<Earth>(new Earth(i, j)); //at given point add as unique pointer
			}

		}
	}
}





bool StudentWorld::dig(int x, int y) {
	/*
	Remove earth object that the TunnelMan comes across
	Any piece of 4x4 tunnelman that touches earth need to remove
	NEED TO CLEAN 4x4 box
	x+4 to clear area that big
	ISSUE: problem NEED to set bounds
	FIXED: using do something function
			checks bounds
	Part 2: seperate function for bound checking?


	*/
	bool dug = false;
	for (int i = x; i < x + 4; i++) //NEED X+4 to clear the 4 in x dir
	{
		for (int j = y; j < y + 4; j++) //NEED y+4 to clear the 4 in the y dir
		{
			if (earthContainer[i][j] != nullptr) //while not part of tunnel or any other null area then 
			{
				earthContainer[i][j]->setVisible(false); //set visible to false at given coordinates 
														//delete later rather than right now to speed up process
				dug = true; //if reached then set dug to true
			}
		}
	}


	return dug;
}




void StudentWorld::addGameObjects()
{
	//gets the specified amount to be ditributed in the field
	int x = (5 - getLevel()) / 2;
	int goldNugCount = max(x, 2);
	int y = (getLevel() / 2) + 2;
	int boulderCount = min(y, 9);
	int L = (2 + getLevel());
	this->OilCount = min(L, 21);

	//Calls the function and will add them to the containers
	createGold_Boulders_Oil(goldNugCount, GOLD_TYPE);
	createGold_Boulders_Oil(boulderCount, BOULDER_TYPE);
	createGold_Boulders_Oil(OilCount, OIL_TYPE);

}

void StudentWorld::add_Sonar_Water()
{
	/*
	There is a 1 in G chance that a new Water Pool or Sonar Kit Goodie will be added
	use modulus and if == 0 then the chance and rand match so can add
			then generate rand coordinates
			if valid area
				push back into the containers
	
	*/
	int chance = (getLevel() * 25) + 300;;


	

	if (rand() % chance == 0) //1 in G chance new goodie is made
	{
		int b = getLevel() * 10 - 300;
		int T = max(100, b); //The maximum tick life of the goodie to be generated
		int sonarChance = rand() % 5; //1 in 5 chance sonar is made

		if (sonarChance == 0)	//Generate a sonar
		{
			gameItems.push_back(std::unique_ptr<Actor>(new Sonar(0, 60, this, T)));
		}
		else
		{
			int x = rand() % (max_X + 1);
			int y = rand() % (max_Y - 1);
			randomCoordinates(x, y, false);

			//Generate new random coordinates until the area is clear of earth and objects
			while (!noEarth(x, y) || !radiusIsclear(x, y))
			{
				randomCoordinates(x, y, false);
			}
			gameItems.push_back(std::unique_ptr<Actor>(new Water(x, y, this, T)));
		}
	}
}

void StudentWorld::createGold_Boulders_Oil(int numObjects, int type)
{
	/*
	going to add the gold boulders and oil
	need to generate rand coordinates
	Issue: will only generate one thing
	FIX: use for loop to iterate and make more

	for i is less than the number of objects being made 
		create random coordinate
			CANT MAKE items appear on pipe
			maybe use while loop??
			FIX: use while loop to keep generating values until valid


			switch cases
			boulder: add to vector 
					delete 4x4 sqaure so earth will sit flat using delete earth function
			gold: just add to vector 
				****ISSUE: CANT MAKE gold invisible
			OIL:

*/

	//check if it is the boulder 
	bool boulder;
	if (type == BOULDER_TYPE)
		boulder = true;
	else
		boulder = false;

	//for (int i = 0; i < numObjects; i++) {
	//	
	//	

	
	
	for (int i = 0; i < numObjects; i++) {
		
		int x = rand() % (max_X + 1);
		int y = rand() % (max_Y - 1);
		//while (!areaIsClearOfObjects(x, y) || ((x > tunnelStart_col - SPRITE_WIDTH) &&
		//	(x <= tunnelEnd_col) && (y > 0)))
		//{
		//	//Generate new coordinates because area was not clear or area overlapped vertical shaft
		//	randomCoordinates(x, y, boulder);
		//}
				switch (type) {
				case GOLD_TYPE:
					gameItems.push_back(std::unique_ptr <Actor>(new GoldNugget(x, y, this, false, 2, true)));
					break;
				case BOULDER_TYPE:
					gameItems.push_back(std::unique_ptr<Actor>(new Boulder(x, y, this)));
					deleteEarth(x, y);
					break;
				case OIL_TYPE:
					gameItems.push_back(std::unique_ptr<Actor>(new Oil(x, y, this)));
					break;
				}
			}
			
		
	
}
void StudentWorld::randomCoordinates(int x, int y, bool boulder) {


	x = rand() % (61);    //Random x coordinate from 0-60 inclusive.

	if (boulder)
		y = rand() % (37); //y coordinate 20-56 inclusive
	else
		y = rand() % (57); //y coordinate 0-56 inclusive
}

bool StudentWorld::noEarth(int x, int y)
{
	//using similar for loop to create earth
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {

			if (earthContainer[i][j] != nullptr) //if nullptr, then no earth below and set to true
				return true;
		}
	}
	return false;

}

void StudentWorld::deleteEarth(int x, int y)
{ 
	/*
	like dig and need to clear out a 4x4 area 
	for the objects like boulder to be placed on open earth
	make sure nothing is there
	NEED to use .reset to free

	*/
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			earthContainer[i][j]->setVisible(false); //resets the unique pointer
		}
	}


}



double StudentWorld::calculateDistance(double x1, double y1, double x2, double y2)
{
	/*
	USE POW to get distance
	using distance formula
	//(#-#, (what power))
	
	*/
	double x = x2 - x1;
	double y =y2 - y1;
	return sqrt(pow(x, 2) + pow(y, 2));

}
 




void StudentWorld::addProtester()
{
	gameItems.push_back(std::unique_ptr<Actor>(new RegularProtester(this, getLevel())));

}

void StudentWorld::addItems(int type)
{
	switch (type)
	{
	case SONAR_TYPE:{
		player->getNumberOfSonar()+1;
		break;
	}
	case GOLD_TYPE:
	{
		player->increaseGold();
		break;
	}
	case SQUIRT_TYPE:
	{
		player->getNumberOfSquirts() + 1;
		break;
	}
	}
}

void StudentWorld::shoot()
{
	gameItems.push_back(std::unique_ptr<Actor>(new Squirt(player->getX(), player->getY(), player->getDirection(), this)));
	playSound(SOUND_PLAYER_SQUIRT);
}

GraphObject::Direction StudentWorld::exit(int x, int y)
{
	return GraphObject::Direction();
}

double StudentWorld::tunnelManDistance(const int x, const int y)
{
	return calculateDistance(x, y, player->getX(), player->getY());
}


bool StudentWorld::radiusIsclear(const int x, const int y) 
{

	/*
		given the coordinate made by the randomcoordinate
		get the position using vector of gameobjects
		CANT have objects within 6 units of each other

		double distance
		go through for loop of iterator of gameobjects *it.gameobjects.begin() and end
			get the distance - calculateDistance(pass the four coordinates)

			if distance is less than 6
				false because item cant be there
			else
				true because item can be there

	*/
	double distance;
	for (std::vector<unique_ptr<Actor>>::const_iterator it = gameItems.begin(); it != gameItems.end(); ++it)
	{
		//Checking if the area within 6 Euclidian units is clear of other objects
		distance = calculateDistance(x, y, (*it)->getX(), (*it)->getY());

		if (distance < 6)
			return false;	//Object to be built is too close to another object and cannot be built there
	}
	return true;	//No objects within MIN_SPAWN_DIST units were found and object can be built
}




void StudentWorld::setDisplayText()
{

	stringstream text;
	text << "Lvl: " << setw(2) << getLevel() << " "
		<< "Lives: " << getLives() << " "
		<< "Hlth: " << setw(3) << player->getCurrentHealth() * 10 << "% "
		<< "Wtr: " << setw(2) << player->getNumberOfSquirts() << " "
		<< "Gld: " << setw(2) << player->getNumberOfGold() << " "
		<< "Oil Left: " << setw(2) << this->OilCount << " "
		<< "Sonar: " << setw(2) << player->getNumberOfSonar() << " "
		<< "Scr: " << setw(6) << setfill('0') << getScore();
	setGameStatText(text.str());


}





