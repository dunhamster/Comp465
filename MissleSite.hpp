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

#ifndef MISSLESITE_HPP
#define MISSLESITE_HPP

#include "Shape3D.hpp"
#include "Utils.hpp"


class MissleSite : public Shape3D
{

private:

	int missleCount = 0;
	Planet* planet = NULL;

public:

	MissleSite(char* fileName, int vertexCount, float modelSize,
		bool active = true, int missleCount = 0, Planet* planet = NULL,
		glm::mat4 scaleMatrix = glm::mat4(), glm::mat4 translationMatrix = glm::mat4(),
		glm::mat4 rotationalMatrix = glm::mat4(), 
		glm::vec3 ambientMin = glm::vec3(.6, .6, .6),
		glm::vec3 diffuseMin = glm::vec3(.4, .4, .4),
		glm::vec3 specularMax = glm::vec3(.8, .8, .8))

		//Call the parent constructor
		:Shape3D(fileName, vertexCount, modelSize, active, scaleMatrix, 
		translationMatrix, rotationalMatrix, ambientMin, diffuseMin, specularMax){
			this->missleCount = missleCount;
			this->planet = planet;
		}

	void setMissleCount(int missleCount){
		this->missleCount = missleCount;
	}

	int getMissleCount(){
		return this->missleCount;
	}

	void setPlanet(Planet* planet){
		this->planet = planet;
	}

	Planet* getPlanet(){
		return this->planet;
	}

	virtual glm::mat4 getModelMatrix();
	virtual void update(bool update);
};

glm::mat4 MissleSite::getModelMatrix(){

	//If not placed on planet
	if (this->planet == NULL){
		this->setPrevModelMatrix(this->getRotationMatrix() * this->getTranslationMatrix() * this->getScaleMatrix());
		return this->prevModelMatrix;
	}

	glm::mat4 modelMatrix;
	modelMatrix *= glm::translate(glm::mat4(), Utilities::getVector(planet->getPrevModelMatrix(), 3));
	modelMatrix *= this->getTranslationMatrix() * this->getRotationMatrix() * this->getScaleMatrix();

	this->setPrevModelMatrix(modelMatrix);
	return this->prevModelMatrix;
}

void MissleSite::update(bool update){
	this->getModelMatrix();
}

#endif