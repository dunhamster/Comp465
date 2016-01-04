/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

#ifndef ANIMATIONSPEED_HPP
#define ANIMATIONSPEED_HPP

class AnimationSpeed
{

private:

	int speed = 0;
	char id[15];

public:

	AnimationSpeed(char* id, int speed){
		this->speed = speed;
		strcpy(this->id, id);
	}

	int getSpeed(){
		return this->speed;
	}

	void setSpeed(int s){
		this->speed = s;
	}

	char* getID(void){
		return this->id;
	}

	void setID(char* c){
		strcpy(this->id, c);
	}
};

#endif