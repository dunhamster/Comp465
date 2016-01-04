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

#ifndef Planet_HPP
#define Planet_HPP

#include "Shape3D.hpp"


class Planet : public Shape3D
{

private:

	float rotationRadians = 0;
	float currentRotation = 0;
	bool  orbitting = false;
	glm::vec3 rotationAxis;
	Planet* parent = NULL;

public:

	Planet(char* fileName, UINT32 vertexCount, float modelSize, bool active = true,
		Planet* parent = NULL, float rotationRadians = 0, bool orbitting = false,
		glm::vec3 rotationAxis = glm::vec3(0, 1, 0),
		glm::mat4 scaleMatrix = glm::mat4(),
		glm::mat4 translationMatrix = glm::mat4(),
		glm::mat4 rotationalMatrix = glm::mat4(), 
		glm::vec3 ambientMin = glm::vec3(.3, .3, .3),
		glm::vec3 diffuseMin = glm::vec3(.4, .4, .4),
		glm::vec3 specularMax = glm::vec3(.1, .1, .1))

		//Call the parent constructor
		: Shape3D(fileName, vertexCount, modelSize, active, scaleMatrix,
		translationMatrix, rotationalMatrix, ambientMin, diffuseMin, specularMax)
		{
			this->rotationRadians = rotationRadians;
			this->orbitting = orbitting;
			this->rotationAxis = rotationAxis;
			this->parent = parent;
			setPrevModelMatrix(glm::mat4());
		}

	float getrotationRadians(){
		return this->rotationRadians;
	}
	void setrotationRadians(float r){
		this->rotationRadians = r;
	}
	bool isorbitting(){
		return this->orbitting;
	}
	void setorbitting(bool b){
		this->orbitting = b;
	}
	glm::vec3 getRotationAxis(){
		return this->rotationAxis;
	}
	void setRotationAxis(glm::vec3 ra){
		this->rotationAxis = ra;
	}
	void setParent(Planet* p){
		this->parent = p;
	}
	Planet* getParent(){
		return this->parent;
	}
	glm::mat4 getCurrentRotation(){
		return glm::rotate(glm::mat4(), this->currentRotation, this->rotationAxis);
	}

	glm::mat4 getModelMatrix();

	void update(bool update);
};

glm::mat4 Planet::getModelMatrix(){

	glm::mat4 modelMatrix(1.0f);

	//Translate to parent
	if (this->getParent() != NULL){
		glm::mat4 parentModel = this->getParent()->getPrevModelMatrix();
		modelMatrix *= glm::translate(glm::mat4(), getPosition(parentModel));
	}

	//Rotate if orbitting
	if (this->orbitting)
		modelMatrix *= this->getCurrentRotation();

	//Scale and translate
	modelMatrix *= this->getTranslationMatrix() * this->scaleMatrix;
	this->setPrevModelMatrix(modelMatrix);
	return modelMatrix;
}

void Planet::update(bool update) {
	if (!(update || this->active))
		return;
	if (this->orbitting){
		this->currentRotation += this->rotationRadians;
		if ((currentRotation <= -(2 * PI)) || (currentRotation >= 2 * PI)){
			int div = (int)(currentRotation / (2 * PI));
			currentRotation -= (div * 2 * PI);
		}
	}
	this->getModelMatrix();
}

#endif