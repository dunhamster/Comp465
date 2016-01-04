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

#ifndef Camera_HPP
#define Camera_HPP

#include <cmath>
#include "Shape3D.hpp"
#include "Utils.hpp"

class Camera
{
private:

	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	char viewID[20];
	Shape3D* parent = NULL;

	//Moving camera variables
	glm::vec3 eyeDynamic;
	glm::vec3 atDynamic;
	glm::vec3 upDynamic;

public:

	enum VIEW_TYPE{
		Follow = 0, //Follow from behind
		Top,        //Looking down on the object   
	} viewType;

	Camera(char* id, glm::vec3 eye, glm::vec3 at, glm::vec3 up, int viewFlag, Shape3D* parent = NULL){
		strcpy(this->viewID, id);
		this->eye = eye;
		this->at = at;
		this->up = up;
		this->parent = parent;
		setViewFlag(viewFlag);
	}

	char* getViewID(){
		return this->viewID;
	}
	void setViewID(char* id){
		strcpy(this->viewID, id);
	}
	glm::vec3 getEye(){
		return this->eye;
	}
	void setEye(glm::vec3 eye){
		this->eye = eye;
	}
	glm::vec3 getAt(){
		return this->at;
	}
	void setAt(glm::vec3 at){
		this->at = at;
	}
	glm::vec3 getUp(){
		return this->up;
	}
	void setUp(glm::vec3 up){
		this->up = up;
	}
	Shape3D* getParent(){
		return this->parent;
	}
	void setParent(Shape3D* parent){
		this->parent = parent;
	}
	int getViewFlag(){
		return this->viewType;
	}
	void setViewFlag(int f){
		viewType = (VIEW_TYPE)f;
	}

	glm::mat4 getLookAt();
};

glm::mat4 Camera::getLookAt(){

	//If Dynamic camera
	if (parent != NULL)
	{
		glm::mat4 modelMatrix = parent->getModelMatrix();
		glm::vec3 rightVec = getRight(modelMatrix);
		glm::vec3 upVec(modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2]);//getUp() method giving error????
		glm::vec3 atVec = getOut(modelMatrix);
		glm::vec3 posVec = getPosition(modelMatrix);

		//Looking at position of object
		this->atDynamic = posVec;
		//Offset at-vector
		this->atDynamic += Utilities::setVectorMagnitude(upVec, glm::distance(glm::vec3(0, 0, 0), at));

		if (viewType == Top){
			this->eyeDynamic = posVec;
			this->eyeDynamic += this->eye;
			this->upDynamic = atVec;
		}
		else if (viewType == Follow){
			upDynamic = upVec;
			//Calculate offsets
			glm::vec3 zOffset = Utilities::setVectorMagnitude(atVec, this->eye[2]);
			glm::vec3 yOffset = Utilities::setVectorMagnitude(upVec, this->eye[1]);
			glm::vec3 xOffset = Utilities::setVectorMagnitude(rightVec, this->eye[0]);
			eyeDynamic = -zOffset + yOffset + xOffset + posVec;
		}
		return glm::lookAt(eyeDynamic, atDynamic, upDynamic);
	}

	//Else static view
	glm::mat4 view = glm::lookAt(eye, at, up);
	return view;
}

#endif