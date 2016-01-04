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

#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Shape3D.hpp"
#include "Utils.hpp"

class SpaceShip : public Shape3D
{

private:

	int missleCount = 0;

public:

	SpaceShip(char* fileName, int vertexCount, float modelSize,
		bool active = true, int missleCount = 0,
		glm::mat4 scaleMatrix = glm::mat4(), glm::mat4 translationMatrix = glm::mat4(),
		glm::mat4 rotationalMatrix = glm::mat4(),
		glm::vec3 ambientMin = glm::vec3(.6, .6, .6),
		glm::vec3 diffuseMin = glm::vec3(.2, .2, .2),
		glm::vec3 specularMax = glm::vec3(.8, .8, .8))

		//Call the parent constructor
		: Shape3D(fileName, vertexCount, modelSize, active, scaleMatrix,
		translationMatrix, rotationalMatrix, ambientMin, diffuseMin, specularMax)
		{
			this->missleCount = missleCount;
			//Initialize ship to be facing the right direction
			this->setRotationMatrix(glm::rotate(glm::mat4(), PI, Utilities::getVector(this->rotationMatrix, 1)) * this->rotationMatrix);
		}

	void setMissleCount(int c){
		this->missleCount = c;
	}

	int getMissleCount(){
		return this->missleCount;
	}

	void warp(glm::mat4 translation, glm::mat4 rotation){
		this->translationMatrix = translation;
		this->rotationMatrix = rotation;
	}

	virtual glm::mat4 getModelMatrix();
	virtual void update(bool update);
	void moveAdjust(float distance);
	void yawAdjust(float radians);
	void pitchAdjust(float radians);
	void rollAdjust(float radians);
};

glm::mat4 SpaceShip::getModelMatrix(){
	this->setPrevModelMatrix(this->translationMatrix * this->rotationMatrix * this->scaleMatrix);
	return this->prevModelMatrix;
}

void SpaceShip::update(bool update){
	this->getModelMatrix();
}

void SpaceShip::moveAdjust(float distance){

	glm::mat4 modelMatrix = this->prevModelMatrix;

	//Get at-vector from model matrix
	glm::vec3 at(modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]);

	//Scale at-vector
	at = Utilities::setVectorMagnitude(at, distance);

	//Add at-vector to the ships translation
	if (distance < 0){
		this->setTranslationMatrix(glm::translate(glm::mat4(), glm::vec3(modelMatrix[3][0] - at[0], modelMatrix[3][1] - at[1], modelMatrix[3][2] - at[2])));
	}
	else{
		this->setTranslationMatrix(glm::translate(glm::mat4(), glm::vec3(modelMatrix[3][0] + at[0], modelMatrix[3][1] + at[1], modelMatrix[3][2] + at[2])));
	}
}

void SpaceShip::yawAdjust(float radians){
	this->setRotationMatrix(glm::rotate(glm::mat4(), radians, Utilities::getVector(this->rotationMatrix, 1)) * this->rotationMatrix);
}

void SpaceShip::pitchAdjust(float radians){
	this->setRotationMatrix(glm::rotate(glm::mat4(), radians, Utilities::getVector(this->rotationMatrix, 0)) * this->rotationMatrix);
}

void SpaceShip::rollAdjust(float radians){
	this->setRotationMatrix(glm::rotate(glm::mat4(), radians, Utilities::getVector(this->rotationMatrix, 2)) * this->rotationMatrix);
}

#endif