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

#ifndef UTILS_H
#define UTILS_H

class Utilities{

public:

	static glm::vec3 positive(glm::vec3 vector){
		if (vector[0] < 0)
			vector[0] *= -1;
		if (vector[1] < 0)
			vector[1] *= -1;
		if (vector[2] < 0)
			vector[2] *= -1;
		return vector;
	}

	static glm::vec3 getVector(glm::mat4 matrix, int column){
		return glm::vec3(matrix[column][0], matrix[column][1], matrix[column][2]);
	}

	static bool isZero(glm::vec3 vector){
		if (vector[0] == 0 && vector[1] == 0 && vector[2] == 0)
			return true;
		return false;
	}

	//scales and returns a vec3
	static glm::vec3 setVectorMagnitude(glm::vec3 vector, float scalar){

		//Return if all zeros
		if (isZero(vector))
			return vector;

		//Set scaledVector equal to all positive vector
		glm::vec3 scaledVector;
		scaledVector = positive(vector);

		//Get smallest value in scaledVector
		float smallest = 1000000000;
		if (scaledVector[0] < smallest  && scaledVector[0] != 0)
			smallest = scaledVector[0];
		if (scaledVector[1] < smallest  && scaledVector[1] != 0)
			smallest = scaledVector[1];
		if (scaledVector[2] < smallest && scaledVector[2] != 0)
			smallest = scaledVector[2];

		//Reduce scaledVector
		scaledVector[0] /= smallest;
		scaledVector[1] /= smallest;
		scaledVector[2] /= smallest;

		//Final scaling
		float sum = (float)((pow(scaledVector[0], 2)) + (pow(scaledVector[1], 2)) + (pow(scaledVector[2], 2)));
		float scale = (float)(abs(scalar) / sqrt(sum));
		scaledVector[0] *= scale;
		scaledVector[1] *= scale;
		scaledVector[2] *= scale;

		//Undo positive() call
		if (vector[0] < 0)
			scaledVector[0] *= -1;
		if (vector[1] < 0)
			scaledVector[1] *= -1;
		if (vector[2] < 0)
			scaledVector[2] *= -1;

		return scaledVector;
	}
};

#endif