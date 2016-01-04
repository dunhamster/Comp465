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

#ifndef SHAPE3D_HPP
#define SHAPE3D_HPP

#include "Utils.hpp"
#include <iostream>

class Shape3D
{

protected:

	char fileName[25];
	int vertexCount = 0;
	float modelSize = 0;
	bool active = true;

	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 prevModelMatrix;

	/*The following vec3's relate to how the object reacts to light.*/
	glm::vec3 ambient;    //Minimum value for the objects ambient light
	glm::vec3 specular;   //Max specular light added to object
	glm::vec3 diffuse;    //Minimum value diffuse can be over an infinite distance

public:

	Shape3D(char* fileName, int vertexCount, float modelSize, bool active = true,
		glm::mat4 scaleMatrix = glm::mat4(), glm::mat4 translationMatrix = glm::mat4(),
		glm::mat4 rotationMatrix = glm::mat4(),
		glm::vec3 ambientMin = glm::vec3(.1, .1, .1),
		glm::vec3 diffuseMin = glm::vec3(.1, .1, .1),
		glm::vec3 specularMax = glm::vec3(.5, .5, .5)){

		strcpy(this->fileName, fileName);
		this->vertexCount = vertexCount;
		this->modelSize = modelSize;
		this->active = active;
		this->rotationMatrix = rotationMatrix;
		this->scaleMatrix = scaleMatrix;
		this->translationMatrix = translationMatrix;

		setAmbientMin(ambientMin);
		setDiffuseMin(diffuseMin);
		setSpecularMax(specularMax);
	}

	glm::vec3 getAmbientMin(){
		return this->ambient;
	}

	void setAmbientMin(glm::vec3 a){
		this->ambient = a;
	}

	glm::vec3 getDiffuseMin(){
		return this->diffuse;
	}

	void setDiffuseMin(glm::vec3 d){
		this->diffuse = d;
	}

	glm::vec3 getSpecularMax(){
		return this->specular;
	}

	void setSpecularMax(glm::vec3 s){
		this->specular = s;
	}

	char* getFileName(){
		return this->fileName;
	}

	void setFileName(char* fileName){
		strcpy(this->fileName, fileName);
	}

	int getVertexCount(){
		return this->vertexCount;
	}

	void setVertexCount(int vertexCount){
		this->vertexCount = vertexCount;
	}

	float getModelSize(){
		return this->modelSize;
	}

	void setModelSize(float modelSize){
		this->modelSize = modelSize;
	}

	bool isActive(){
		return this->active;
	}

	void setActive(bool b){
		this->active = b;
	}

	glm::mat4 getRotationMatrix(){
		return this->rotationMatrix;
	}

	void setRotationMatrix(glm::mat4 rotationMatrix){
		this->rotationMatrix = rotationMatrix;
	}

	glm::mat4 getScaleMatrix(){
		return this->scaleMatrix;
	}

	void setScaleMatrix(glm::mat4 rotationMatrix){
		this->scaleMatrix = rotationMatrix;
	}

	glm::mat4 getTranslationMatrix(){
		return this->translationMatrix;
	}

	void setTranslationMatrix(glm::mat4 rotationMatrix){
		this->translationMatrix = rotationMatrix;
	}

	glm::mat4 getPrevModelMatrix(){
		return this->prevModelMatrix;
	}

	void setPrevModelMatrix(glm::mat4 modelMatrix){
		this->prevModelMatrix = modelMatrix;
	}

	virtual glm::mat4 getModelMatrix(){
		return glm::mat4();
	}

	virtual void update(bool update){
		return;
	}
};

#endif