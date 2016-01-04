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

#ifndef SMARTMISSLE_HPP
#define SMARTMISSLE_HPP

#include "Shape3D.hpp"
#include "Utils.hpp"


class SmartMissle : public Shape3D
{

private:

	Shape3D* target = NULL;		  //Missles target
	Shape3D* shooter = NULL;      //Who fired the missle
	float speed;				  //Missle travel speed
	float activationDistance = 0; //Distance till activation
	bool smartMissleOn = false;   //Smart on/off
	float currentDistance = 0;    //Current distance traveled
	float maxDistance = 0;        //Max distance missle can travel

public:

	SmartMissle(char* fileName, int vertexCount, float modelSize,
		bool active = true, Shape3D* target = NULL, Shape3D* shooter = NULL,
		float speed = 5, float activationDistance = 245,
		float maxDistance = 15000, glm::mat4 scaleMatrix = glm::mat4(),
		glm::mat4 translationMatrix = glm::mat4(), glm::mat4 rotationalMatrix = glm::mat4(),
		glm::vec3 ambientMin = glm::vec3(.6, .6, .6),
		glm::vec3 diffuseMin = glm::vec3(.2, .2, .2),
		glm::vec3 specularMax = glm::vec3(.8, .8, .8))

		//Call the parent constructor
		:Shape3D(fileName, vertexCount, modelSize, active, scaleMatrix, translationMatrix,
		rotationalMatrix, ambientMin, diffuseMin, specularMax)
		{
			this->target = target;
			this->shooter = shooter;
			this->speed = speed;
			this->activationDistance = activationDistance;
			this->maxDistance = maxDistance;
		}

	bool setTarget(Shape3D* t){
		this->target = t;
		return true;
	}

	Shape3D* getTarget(){
		return this->target;
	}

	void setShooter(Shape3D* s){
		this->shooter = s; 
	}

	Shape3D* getShooter(){
		return this->shooter;
	}

	void setSpeed(float s){
		this->speed = s;
	}

	float getSpeed(){
		return this->speed;
	}

	void setActivationDistance(float d){
		this->activationDistance = d;
	}

	float getActivationDistance(){
		return this->activationDistance;
	}

	void setSmartMissleOn(bool s){
		this->smartMissleOn = s;
	}

	bool getSmartMissleOn(){
		return this->smartMissleOn;
	}

	void setCurrentDistance(float d){
		this->currentDistance = d;
	}

	float getCurrentDistance(){
		return this->currentDistance;
	}

	void setMaxDistance(float d){
		this->maxDistance = d;
	}

	float getMaxDistance(){
		return this->maxDistance;
	}

	virtual glm::mat4 getModelMatrix(void);

	virtual void update(bool updateIfInactive);
};

glm::mat4 SmartMissle::getModelMatrix(void){
	this->prevModelMatrix = this->getTranslationMatrix() * this->getRotationMatrix() * this->getScaleMatrix();
	return this->prevModelMatrix;
}

void SmartMissle::update(bool update){

	if (update || this->isActive()){

		glm::vec3 dirVec = glm::vec3(Utilities::getVector(this->prevModelMatrix, 1));

		//Missle travels forward until smart is on and has a target
		if (!smartMissleOn || this->target == NULL){

			//Scale the dirVec vector
			dirVec = Utilities::setVectorMagnitude(dirVec, (float)this->speed);

			//Add dirVec vector to the missles translation
			this->setTranslationMatrix(glm::translate(glm::mat4(), Utilities::getVector(this->prevModelMatrix, 3) + dirVec));

			//Activate smartness if within distance
			if (currentDistance >= this->activationDistance)
				smartMissleOn = true;
		}
		//Else travel towards target
		else{

			//Calculate at-vector from missles location to targets location
			glm::vec3 at(Utilities::getVector(this->getTarget()->getPrevModelMatrix(), 3) - Utilities::getVector(this->getPrevModelMatrix(), 3));

			//Rotate missle towards target
			if (!(colinear(dirVec, at, 0.1f) || Utilities::isZero(at))){

				//Calculate angle to rotate
				float angle = acos((glm::dot(dirVec, at) / (glm::distance(dirVec, glm::vec3(0, 0, 0)) * glm::distance(at, glm::vec3(0, 0, 0)))));

				//Calculate axis of rotation
				glm::vec3 axis = glm::cross(dirVec, at);

				//Rotated the missle
				this->rotationMatrix = glm::rotate(glm::mat4(), angle, axis) * this->rotationMatrix;
			}

			//Scale at-vector
			at = Utilities::setVectorMagnitude(at, (float)this->speed);

			//Add at-vector to the ships translation
			this->setTranslationMatrix(glm::translate(glm::mat4(), glm::vec3(this->prevModelMatrix[3][0] + at[0], this->prevModelMatrix[3][1] + at[1], this->prevModelMatrix[3][2] + at[2])));
		}

		//Update the current distance traveled
		currentDistance += speed;

		//Check if missle has travelled its max distance
		if (currentDistance >= maxDistance){
			this->setActive(false);
			this->setTarget(NULL);
		}
		this->getModelMatrix();
	}
}

#endif