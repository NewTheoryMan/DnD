#include <iostream>
#include "graph.h"
#include <cstdlib>
#include <ctime>
#include <string>


using namespace std;

const int MAX_WIDTH = 12;
const int MIN_WIDTH = 6;
const int MAX_HEIGHT = 10;
const int MIN_HEIGHT = 3;
const int ENEM_CHANCE = 3; //1/3 chance of there being enemies in a room

graph::graph()
{
	root = NULL;
	entrance = new room;
	exit = new room(true);
	number_rooms = 0;
}

graph::~graph()
{
	if(root)
	{
		destroy(root);
	}

	delete entrance;
	delete exit;
}

void graph::destroy(room * source)
{
	if(source->get_next())
	{
		destroy(source->get_next());
		delete source;
	}
	else
	{
		delete source;
	}
}

void graph::generate(int num_rooms)
{
	srand (time(NULL));

	//Initialize parameters
	int priz = 0;
	int enem = 0;
	int dist = 0;
	int hall = 0;
	int widt = 0;
	int heigh = 0;
	int peri = 0;
	bool _hall = false;
	room * temp = NULL;
	int chance_enem = 0;

	//create a number of rooms equal to num_rooms
	for(int i = 0; i < num_rooms; i++)
	{
		//Make width and height default to max hallway width. 
		widt = 2;
		heigh = 2;
		peri = 0;
		hall = rand()%10;// 1/10 chance of hallway

		//If the new room is not a hallway
		if(hall != 0)
		{

			widt = rand()%(MAX_WIDTH-MIN_WIDTH)+MIN_WIDTH;;//Random: 6-12 
			heigh = rand()%(MAX_HEIGHT-MIN_HEIGHT)+MIN_HEIGHT;//Random: 3-10
			peri = widt*2+heigh*2;
		}
		//If the new room is a hallway
		else
		{
			_hall = true;
			
			//Randomize if the hallway will go vertical/horizontal
			if(rand()%2)
			{
				heigh = rand()%(MAX_HEIGHT-MIN_HEIGHT)+MIN_HEIGHT;//Random: 3-10
				peri = widt*2+heigh*2;
			}
			else
			{
				widt = rand()%(MAX_WIDTH-MIN_WIDTH)+MIN_WIDTH;;//Random: 6-12 
				peri = widt*2+heigh*2;
			}
		}

		chance_enem = rand()%ENEM_CHANCE;//The problem here is that rand is based off of time, and time doesn't change much in the microseconds
										 // it takes for the loop to repeat. You end up with repeat # of enem for about 3 rooms. 
		//Randomize parameters
		if(chance_enem == 0)
		{
			enem = rand()%(peri/3);//Random: 0-(quarter of parameter)
			if(enem > 0) priz = rand()%(enem*2);//Random: 0-(enemies*2)
		}

		dist = rand();//Unsure why we have distance. Left it a random number

		//Create a new room using the above parameters and insert it into the graph
		temp = new room(priz, enem, _hall, widt, heigh, peri, dist, i+1);
		insert(temp);
	}

	//build the adjacentcy list of doors for each room once the graph is built
	root->set_doors(entrance, exit, this, num_rooms);

}

void graph::insert(room * source)
{
	if(root)
	{
		root->insert(source);
	}
	else
	{
		root = source;
	}

	number_rooms++;
}

room * graph::get_random_room()
{
	room * temp = root;

	for(int i = 0; i < rand()%number_rooms; i++)//rand()%number_rooms = Random: 0 - number_rooms-1
	{
		temp = temp->get_next();
	}

	return temp;
}

void graph::display()
{
	ofstream output;
	output.open("output/output"+to_string(time(NULL))+".txt");

	root->display(output);

	output.close();
}
