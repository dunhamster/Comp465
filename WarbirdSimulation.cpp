/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# define __Windows__
# include "../includes465/include465.hpp"
# include "../includes465/texture.hpp"
# define __INCLUDES465__ 

#include "Shape3D.hpp"
#include "Planet.hpp"
#include "Camera.hpp"
#include "AnimationSpeed.hpp"
#include "SpaceShip.hpp"
#include "MissleSite.hpp"
#include "SmartMissle.hpp"
#include "Utils.hpp"

void reshape(int width, int height);
void updateTitle(void);
void display(void);
void init(void);
void initObjects(void);
void initCameras(void);
void initAnimationSpeeds(void);
void update(void);
void intervalTimer(int i);
void keyboard(BYTE key, int x, int y);
void specialKeyEvent(int key, int x, int y);
void findMissleTarget(void);
void gravityEffects(void);
void collissionCheck(void);


const int nModels = 11;     //Number of all models
const int nPlanets = 5;     //Number of planet models
const int nMissles = 3;     //Number of active missle models
const int nMissleSites = 2; //Number of missle sites
const int nCameras = 5;     //Number of cameras
const int nSpeeds = 5;      //Number of animation speeds

//Array of models
Shape3D* shapes[nModels];
Planet* planets[nPlanets];
SmartMissle* smartMissles[nMissles];
MissleSite* missleSites[nMissleSites];

//Ship
SpaceShip* ship;

//Animation speed variables
AnimationSpeed * animationSpeeds[nSpeeds];
int currentSpeed = 0;

//Camera variables
Camera* cameras[nCameras];
int currentCamera = 0;

//Ships movement variables
int warpLocation = 0;
const int nShipSpeeds = 3;		        //Number of ship speeds
int currentShipSpeed = 0;				//Ships current speed index
const float shipSpeed[nShipSpeeds] = {10, 50, 200};
const float shipTurnSpeed = 0.2f;		//Ships turning speed
bool updatedship = true;

//Missle variabels
const float missleSiteDetect = 5000.0f;		//Distance missles sites can detect target
const float missleDetect = 5000.0f;			//Distance missles can detect target
const float maxMissleDistance = 10000.0f;	//Missle travel distance till explodes
const float missleSpeed = 50.0f;			//Missle speed. Supposed to be 5???
const float missleDelay = 245.0f;			//Missles delay till its activated

//Gravity variables
bool gravity = true;
const double GRAVITY_CONSTANT = 90000000.0;

//Shader variables
const char* VERTEX_SHADER_FILE = "vertexShader.glsl";
const char* FRAGMENT_SHADER_FILE = "fragmentShader.glsl";
const char* TEX_VERTEX_SHADER_FILE = "textureVertexShader.glsl";
const char* TEX_FRAG_SHADER_FILE = "textureFragmentShader.glsl";

GLuint shaderProgram, textureShader;
GLuint VAO[nModels];     //Vertex Array Objects
GLuint buffer[nModels];  //Vertex Buffer Objects

GLuint MVP;
GLuint vPosition[nModels];
GLuint vColor[nModels];
GLuint vNormal[nModels];
GLuint MVM;  //Model View Matrix handle

GLuint PLL;  //Point Light Location
GLuint SLL;  //Spot Light Location
GLuint SLD;  //Spot Light Direction
GLuint SLA;  //Spot Light Angle

GLuint KA;   //Min ambient value
GLuint KD;   //Min diffuse value
GLuint KS;   //Max specular value

glm::vec4 toggledLights(1, 1, 1, 0);
GLuint TL;  //Toggle Lights

GLuint VP;	//View Projection in texture shaders

glm::mat4 projectionMatrix;          //set in reshape()
glm::mat4 viewMatrix;                //set in display()
glm::mat4 ModelViewProjectionMatrix; //set in display()

//Window title info variables
char titleStr[100];
int updateRate, updateCount;
int frameRate, frameCount;
int currentTime, lastTime, timeInterval;
int updateTime, lastUpdate, updateInterval;

//End game variables
bool gameOver = false;
bool win = false;

const BYTE nTextures = 6;   //Number of textures
GLuint ibo[nTextures];      //Index Buffer Object
GLuint VAO_T[nTextures];    //Vertex Array Object for Textures
GLuint VBO_T[nTextures];    //Vertex Buffer Ojbect for Textures
GLuint texture[nTextures];

//static const char* TEX_FILE_NAME = "Space_640_360.raw";
//static const UINT32 PixelSize[2] = {1920, 1080};

//Array of texture file names
//Maybe I'll add different texture!
static const char* TEX_FILE_NAME[] = {
	{ "Space_640_360.raw" },
	{ "Space_640_360.raw" },
	{ "Space_640_360.raw" },
	{ "Space_640_360.raw" },
	{ "Space_640_360.raw" },
	{ "Space_640_360.raw" }
};

//Dimension size of each texture file
static const UINT32 PixelSizes[nTextures][2] = {
	{ 640, 360 },
	{ 640, 360 },
	{ 640, 360 },
	{ 640, 360 },
	{ 640, 360 },
	{ 640, 360 }
};

//Four points for each texture plane
static const GLfloat point[6][16] = {

	{//Front Square
		40000.0f, 40000.0f, 40000.0f, 1.0f,
		-40000.0f, 40000.0f, 40000.0f, 1.0f,
		-40000.0f, -40000.0f, 40000.0f, 1.0f,
		40000.0f, -40000.0f, 40000.0f, 1.0f
	},
	{//Back Square
		-40000.0f, 40000.0f, -40000.0f, 1.0f,
		40000.0f, 40000.0f, -40000.0f, 1.0f,
		40000.0f, -40000.0f, -40000.0f, 1.0f,
		-40000.0f, -40000.0f, -40000.0f, 1.0f
	},
	{//Right Square
		40000.0f, 40000.0f, -40000.0f, 1.0f,
		40000.0f, 40000.0f, 40000.0f, 1.0f,
		40000.0f, -40000.0f, 40000.0f, 1.0f,
		40000.0f, -40000.0f, -40000.0f, 1.0f
	},
	{//Left Square
		-40000.0f, 40000.0f, 40000.0f, 1.0f,
		-40000.0f, 40000.0f, -40000.0f, 1.0f,
		-40000.0f, -40000.0f, -40000.0f, 1.0f,
		-40000.0f, -40000.0f, 40000.0f, 1.0f
	},
	{//Top square
		-40000.0f, 40000.0f, -40000.0f, 1.0f,
		40000.0f, 40000.0f, -40000.0f, 1.0f,
		40000.0f, 40000.0f, 40000.0f, 1.0f,
		-40000.0f, 40000.0f, 40000.0f, 1.0f
	},
	{//Bottom Square
		-40000.0f, -40000.0f, -40000.0f, 1.0f,
		40000.0f, -40000.0f, -40000.0f, 1.0f,
		40000.0f, -40000.0f, 40000.0f, 1.0f,
		-40000.0f, -40000.0f, 40000.0f, 1.0f
	}
};

//Indices for each texture
static const unsigned int indices[6][6] = {
	//Upper   -   Lower//
	{ 0, 3, 1,  1, 3, 2 }, //Front
	{ 0, 3, 1,  1, 3, 2 }, //Back
	{ 0, 3, 1,  1, 3, 2 }, //Right
	{ 0, 3, 1,  1, 3, 2 }, //Left
	{ 0, 3, 1,  1, 3, 2 }, //Top
	{ 0, 3, 1,  1, 3, 2 }  //Bottom
};

static const GLfloat texCoords[] = {
	0.0f, 0.0f,     // 0
	1.0f, 0.0f,     // 1
	1.0f, 1.0f,     // 2
	0.0f, 1.0f };   // 3

void reshape(int width, int height){
	float aspectRatio = (float)width / (float)height;
	float FOVY = glm::radians(60.0f);
	glViewport(0, 0, width, height);
	printf("Reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", FOVY, width, height, aspectRatio);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f);
}

void updateTitle(){

	if (gameOver){
		if (win)
			sprintf(titleStr, "Cadet passes flight training");
		else
			sprintf(titleStr, "Cadet resigns from War College");
	}
	else{
		sprintf(titleStr, "ship: %d  Unum: %d  Secundus:  %d  U/S: %d  F/S: %d  View: %s  Setting: %s",
			ship->getMissleCount(), missleSites[0]->getMissleCount(), missleSites[1]->getMissleCount(),
			updateRate, frameRate, cameras[currentCamera]->getViewID(), animationSpeeds[currentSpeed]->getID());
	}

	glutSetWindowTitle(titleStr);
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Get the current view matrix
	viewMatrix = cameras[currentCamera]->getLookAt();

	glUseProgram(shaderProgram);

	//Set the current point light location
	glUniform4fv(PLL, 1, glm::value_ptr(viewMatrix * planets[0]->getPrevModelMatrix() * glm::vec4(0, 0, 0, 1)));

	//Get the final to be location of the space ship
	glm::vec3 warbirdLoc = Utilities::getVector(viewMatrix * ship->getPrevModelMatrix(), 3);
	//We'll want the light to be in front of the object, not coming from its' insides
	glm::vec3 warbirdLightLoc = Utilities::setVectorMagnitude(Utilities::getVector(viewMatrix * ship->getPrevModelMatrix(), 2), 100) + warbirdLoc;
	
	glUniform3fv(SLL, 1, glm::value_ptr(warbirdLightLoc));
	glUniform3fv(SLD, 1, glm::value_ptr(glm::vec3(viewMatrix * glm::vec4(Utilities::getVector(ship->getPrevModelMatrix(), 2), 0))));
	
	for (int m = 0; m < nModels; m++) {

		//Only draw active objects
		if (!shapes[m]->isActive())
			continue;

		//Get the model view projection matrix
		ModelViewProjectionMatrix = projectionMatrix * cameras[currentCamera]->getLookAt() * shapes[m]->getPrevModelMatrix();

		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glUniformMatrix4fv(MVM, 1, GL_FALSE, glm::value_ptr(viewMatrix * shapes[m]->getPrevModelMatrix()));
		glUniform3fv(KA, 1, glm::value_ptr(shapes[m]->getAmbientMin()));
		glUniform3fv(KD, 1, glm::value_ptr(shapes[m]->getDiffuseMin()));
		glUniform3fv(KS, 1, glm::value_ptr(shapes[m]->getSpecularMax()));
		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, shapes[m]->getVertexCount());
	}

	glUseProgram(textureShader);

	//Set the View-Projection Matrix to be used with textures
	glUniformMatrix4fv(VP, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix));

	//ERROR HERE! Possibly only on some computers
	for (int i = 0; i < nTextures; i++){

		glBindVertexArray(VAO_T[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		
		//Comment out this line if program crashes. So you can grade just the lighting
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));//error right here

	}
	
	//Switch back to shader
	glUseProgram(shaderProgram);
	
	//Swap buffers
	glutSwapBuffers();

	//Update the frame count and estimate fps information
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeInterval = currentTime - lastTime;
	//Determine if 1 second has passed. If so, update frameRate
	if (timeInterval >= 1000) {
		frameRate = (int)(frameCount / (timeInterval / 1000.0f));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}

void init() {

	float modelBoundingRadius[nModels]; //Model's bounding radius
	glm::vec3 scale[nModels];

	// load the shader programs
	shaderProgram = loadShaders(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	textureShader = loadShaders(TEX_VERTEX_SHADER_FILE, TEX_FRAG_SHADER_FILE);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels, VAO);
	glGenBuffers(nModels, buffer);

	glGenVertexArrays(nTextures, VAO_T);
	glGenBuffers(nTextures, VBO_T);
	glGenBuffers(nTextures, ibo);
	glGenTextures(nTextures, texture);

	glUseProgram(shaderProgram);

	/*Load Buffer and get the models scale values*/
	for (int i = 0; i < nModels; i++) {
		//Load the buffers from the model files
		modelBoundingRadius[i] = loadModelBuffer(shapes[i]->getFileName(),
			shapes[i]->getVertexCount(),
			VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i],
			"vPosition", "vColor", "vNormal");

		scale[i] = glm::vec3(shapes[i]->getModelSize() * 1.0f / modelBoundingRadius[i]);

		//Now that we have the scale, update the scale matrix in each of the objects
		shapes[i]->setScaleMatrix(glm::scale(glm::mat4(), glm::vec3(scale[i])));
	}

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	MVM = glGetUniformLocation(shaderProgram, "ModelViewMatrix");
	PLL = glGetUniformLocation(shaderProgram, "PointLightLocation");
	SLL = glGetUniformLocation(shaderProgram, "SpotLightLocation");
	SLD = glGetUniformLocation(shaderProgram, "SpotLightDirection");
	SLA = glGetUniformLocation(shaderProgram, "SpotLightAngle");
	KA = glGetUniformLocation(shaderProgram, "KA");
	KD = glGetUniformLocation(shaderProgram, "KD");
	KS = glGetUniformLocation(shaderProgram, "KS");
	TL = glGetUniformLocation(shaderProgram, "ToggleLights");

	glUniform1f(SLA, 0.2f);
	glUniform4fv(TL, 1, glm::value_ptr(toggledLights));

	glUseProgram(textureShader);

	for (int i = 0; i < nTextures; i++){

		glBindVertexArray(VAO_T[i]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[i]), indices[i], GL_STATIC_DRAW);

		//Had to comment out this line and it works???
		//Thought I had to enable each VBO?
		//glEnableVertexAttribArray(VBO_T[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_T[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point[i]) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point[i]), point[i]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point[i]), sizeof(texCoords), texCoords);

		GLuint vPosition = glGetAttribLocation(textureShader, "vPosition");
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vPosition);

		GLuint vTexCoord = glGetAttribLocation(textureShader, "vTexCoord");
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point[i])));
		glEnableVertexAttribArray(vTexCoord);

		VP = glGetUniformLocation(textureShader, "ViewProjection");

		//Bind the texture that we want to use
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// set texture parameters
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glGetError();
		//Load texture
		texture[i] = loadRawTexture(texture[i], TEX_FILE_NAME[i], PixelSizes[i][0], PixelSizes[i][1]);
		if (texture[i] != 0) {
			printf("texture loaded \n");
		}
		else
			printf("Error loading texture \n");
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void update(){

	//Check if each missle site should fire missle
	for (int index = 0; index < nMissleSites; index++){
		if (missleSites[index]->isActive() && missleSites[index]->getMissleCount() > 0 && !smartMissles[index + 1]->isActive()){

			//Calculate distance between missle site and ship
			float distance = abs(glm::distance(Utilities::getVector(missleSites[index]->getPrevModelMatrix(), 3), Utilities::getVector(ship->getPrevModelMatrix(), 3)));

			//Check if ship is within missle sites range
			if (distance < missleDetect){
				//Decrement missle sites missleCount
				missleSites[index]->setMissleCount(missleSites[index]->getMissleCount() - 1);
				//Fire the sites missle
				smartMissles[index + 1]->setTranslationMatrix(glm::translate(glm::mat4(), Utilities::setVectorMagnitude(Utilities::getVector(missleSites[index]->getPrevModelMatrix(), 1), 60))
					* glm::translate(glm::mat4(), Utilities::getVector(missleSites[index]->getPrevModelMatrix(), 3)));
				smartMissles[index + 1]->setRotationMatrix(missleSites[index]->getRotationMatrix());
				smartMissles[index + 1]->setSmartMissleOn(false);
				smartMissles[index + 1]->setCurrentDistance(0);
				smartMissles[index + 1]->setActive(true);
				smartMissles[index + 1]->getModelMatrix();
				smartMissles[index + 1]->setTarget(ship);
			}
		}
	}

	//Check if ships missle can find a target in range of it
	if (smartMissles[0]->isActive() && smartMissles[0]->getTarget() == NULL)
		findMissleTarget();

	//Update all objects
	for (int i = 0; i < nModels; i++){
		shapes[i]->update(false);
	}

	//Apply gravity
	if (gravity && ship->isActive())
		gravityEffects();

	//Check for collisions
	collissionCheck();

	glutPostRedisplay();
	updatedship = true;
	updateCount++;
	updateTime = glutGet(GLUT_ELAPSED_TIME);
	updateInterval = updateTime - lastUpdate;
	//Determine if 1 second has passed. If so, update frameRate
	if (updateInterval >= 1000) {
		updateRate = (int)(updateCount / (updateInterval / 1000.0f));
		lastUpdate = updateTime;
		updateCount = 0;
		updateTitle();
	}
}

void initObjects(){

	//Ruber
	planets[0] = new Planet("ruber.tri", (1584 * 3), 2000, true, NULL, 0, false,
		glm::vec3(0, 1, 0), glm::mat4(), glm::translate(glm::mat4(), glm::vec3(0, 0, 0)), glm::mat4(),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

	//Unum
	planets[1] = new Planet("unum.tri", (1584 * 3), 200, true, planets[0], .004f, true,
		glm::vec3(0, 1, 0), glm::mat4(), glm::translate(glm::mat4(), glm::vec3(4000, 0, 0)), glm::mat4());

	//Duo
	planets[2] = new Planet("duo.tri", (1584 * 3), 400, true, planets[0], .002f, true,
		glm::vec3(0, 1, 0), glm::mat4(), glm::translate(glm::mat4(), glm::vec3(-9000, 0, 0)), glm::mat4());

	//Primus
	planets[3] = new Planet("primus.tri", (1584 * 3), 100, true, planets[2], .004f, true,
		glm::vec3(0, 1, 0), glm::mat4(), glm::translate(glm::mat4(), glm::vec3(900, 0, 0)), glm::mat4());

	//Secundus
	planets[4] = new Planet("secundus.tri", (1584 * 3), 150, true, planets[2], .002f, true,
		glm::vec3(0, 1, 0), glm::mat4(), glm::translate(glm::mat4(), glm::vec3(1650, 0, 0)), glm::mat4());

	//ship
	ship = new SpaceShip("ship.tri", (516 * 3), 100, true, 10,
		glm::mat4(), glm::translate(glm::mat4(), glm::vec3(5000, 1000, 5000)));

	//Missle Sites
	missleSites[0] = new MissleSite("missileSite.tri", 60 * 3, 30, true, 6, planets[1],
		glm::mat4(), glm::translate(glm::mat4(), glm::vec3(0, 225, 0)), glm::mat4());
	missleSites[1] = new MissleSite("missileSite.tri", 60 * 3, 30, true, 6, planets[4],
		glm::mat4(), glm::translate(glm::mat4(), glm::vec3(0, 175, 0)), glm::mat4());

	//Smart Missles
	smartMissles[0] = new SmartMissle("missile.tri", 336 * 3, 25, false, NULL, ship, missleSpeed, missleDelay, maxMissleDistance, glm::mat4(), glm::mat4(),
		glm::rotate(glm::mat4(), PI / 2, glm::vec3(-1, 0, 0)));
	smartMissles[1] = new SmartMissle("missile.tri", 336 * 3, 25, false, NULL, missleSites[0], missleSpeed, missleDelay, maxMissleDistance, glm::mat4(), glm::mat4(),
		glm::rotate(glm::mat4(), PI / 2, glm::vec3(-1, 0, 0)));
	smartMissles[2] = new SmartMissle("missile.tri", 336 * 3, 25, false, NULL, missleSites[1], missleSpeed, missleDelay, maxMissleDistance, glm::mat4(), glm::mat4(),
		glm::rotate(glm::mat4(), PI / 2, glm::vec3(-1, 0, 0)));

	shapes[0] = planets[0];
	shapes[1] = planets[1];
	shapes[2] = planets[2];
	shapes[3] = planets[3];
	shapes[4] = planets[4];
	shapes[5] = ship;
	shapes[6] = missleSites[0];
	shapes[7] = missleSites[1];
	shapes[8] = smartMissles[0];
	shapes[9] = smartMissles[1];
	shapes[10] = smartMissles[2];
}

void initCameras(){
	//Front View
	cameras[0] = new Camera("Front", glm::vec3(0, 10000, 20000), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), Camera::Follow, NULL);
	//Top View
	cameras[1] = new Camera("Top", glm::vec3(0, 20000, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), Camera::Top, NULL);
	//Ship View
	cameras[2] = new Camera("Ship", glm::vec3(0, 300, -1000), glm::vec3(0, 300, 0), glm::vec3(0, 1, 0), Camera::Follow, ship);
	//Unum View
	cameras[3] = new Camera("Unum", glm::vec3(0, 4000, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), Camera::Top, planets[1]);
	//Duo View
	cameras[4] = new Camera("Duo", glm::vec3(0, 4000, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), Camera::Top, planets[2]);
}

void initAnimationSpeeds(){
	animationSpeeds[0] = new AnimationSpeed("ace", 40);
	animationSpeeds[1] = new AnimationSpeed("pilot", 100);
	animationSpeeds[2] = new AnimationSpeed("trainee", 250);
	animationSpeeds[3] = new AnimationSpeed("debug", 500);
	animationSpeeds[4] = new AnimationSpeed("Light Speed!", 1);
}

void intervalTimer(int i) {
	//If game is over, stop updating
	if (gameOver)
		return;

	glutTimerFunc(animationSpeeds[currentSpeed]->getSpeed(), intervalTimer, 1);
	update();
}

void keyboard(BYTE key, int x, int y) {

	switch (key) {

	case 033: case 'q': case 'Q':  //Quit
		glutLeaveMainLoop();
		break;

	case 'v': case 'V':  //Switch to next camera
		currentCamera++;
		if (currentCamera >= nCameras)
			currentCamera = 0;
		break;

	case 't': case 'T':  //Change animation speed
		currentSpeed++;
		if (currentSpeed >= nSpeeds)
			currentSpeed = 0;
		break;

	case 'w': case 'W': //Warp ship to either Unum or Duo
		//Warp to Unum
		if (warpLocation == 0){
			//Get a copy of Unums model matrix
			glm::mat4 unumM = shapes[1]->getModelMatrix();

			//Translate ship to Unums position and offset
			unumM = glm::translate(glm::mat4(), Utilities::getVector(unumM, 3));
			unumM = unumM * glm::translate(glm::mat4(), glm::vec3(0, 4000, 0));
			ship->warp(unumM, glm::rotate(glm::mat4(), PI / 2, glm::vec3(1, 0, 0)));
			warpLocation = 1;
		}
		//Warp to Duo
		else{
			//Get a copy of Duos model matrix
			glm::mat4 duo = shapes[2]->getModelMatrix();

			//Translate ship to Duos position and offset
			duo = glm::translate(glm::mat4(), Utilities::getVector(duo, 3));
			duo = duo * glm::translate(glm::mat4(), glm::vec3(0, 4000, 0));
			ship->warp(duo, glm::rotate(glm::mat4(), PI / 2, glm::vec3(1, 0, 0)));
			warpLocation = 0;
		}
		break;

	case 's': case 'S':  //Change ship speed
		currentShipSpeed++;
		if (currentShipSpeed >= nShipSpeeds)
			currentShipSpeed = 0;
		break;

	case 'g': case 'G':  //Switch gravity on/off
		if (gravity == true)
			gravity = false;
		else
			gravity = true;
		break;

	case 'f': case 'F':  //Fire ships missle

		//Does ship have any missles or is a missle already active
		if (ship->getMissleCount() <= 0 || smartMissles[0]->isActive())
			return;

		//Decrement ships missle count
		ship->setMissleCount(ship->getMissleCount() - 1);

		//Set the missles location, orientation, and active it
		smartMissles[0]->setTranslationMatrix(ship->getTranslationMatrix());
		smartMissles[0]->setRotationMatrix(ship->getRotationMatrix());
		smartMissles[0]->setRotationMatrix(glm::rotate(glm::mat4(), PI / 2, Utilities::getVector(ship->getPrevModelMatrix(), 0)) * smartMissles[0]->getRotationMatrix());
		smartMissles[0]->setSmartMissleOn(false);
		smartMissles[0]->setCurrentDistance(0);
		smartMissles[0]->setActive(true);
		smartMissles[0]->getModelMatrix();

		//Determines target of ships missle
		findMissleTarget();
		break;

	case 'a': case 'A':  //Ambient lights
		toggledLights.x = (toggledLights.x == 0) ? 1.0f : 0.0f;
		glUniform4fv(TL, 1, glm::value_ptr(toggledLights));
		break;

	case 'p': case 'P':  //Point lights
		toggledLights.y = (toggledLights.y == 0) ? 1.0f : 0.0f;
		glUniform4fv(TL, 1, glm::value_ptr(toggledLights));
		break;

	case 'h': case 'H':  //Spot light
		toggledLights.z = (toggledLights.z == 0) ? 1.0f : 0.0f;
		glUniform4fv(TL, 1, glm::value_ptr(toggledLights));
		break;

	case 'd': case 'D':  //Debug light for Spot light
		toggledLights.w = (toggledLights.w == 0) ? 1.0f : 0.0f;
		glUniform4fv(TL, 1, glm::value_ptr(toggledLights));
		break;
	}
}

void specialKeyEvent(int key, int x, int y){

	if (!updatedship)
		return;
	updatedship = false;

	//Move forward
	if ((key == GLUT_KEY_UP) && (glutGetModifiers() != GLUT_ACTIVE_CTRL)){
		ship->moveAdjust(shipSpeed[currentShipSpeed]);
	}
	//Move backwards
	else if ((key == GLUT_KEY_DOWN) && (glutGetModifiers() != GLUT_ACTIVE_CTRL)){
		ship->moveAdjust(-shipSpeed[currentShipSpeed]);
	}

	//Yaw right
	else if ((key == GLUT_KEY_RIGHT) && (glutGetModifiers() != GLUT_ACTIVE_CTRL)){
		ship->yawAdjust(-shipTurnSpeed);
	}
	//Yaw left
	else if ((key == GLUT_KEY_LEFT) && (glutGetModifiers() != GLUT_ACTIVE_CTRL)){
		ship->yawAdjust(shipTurnSpeed);
	}

	//Pitch up
	else if ((key == GLUT_KEY_UP) && (glutGetModifiers() == GLUT_ACTIVE_CTRL)){
		ship->pitchAdjust(-shipTurnSpeed);
	}
	//Pitch down
	else if ((key == GLUT_KEY_DOWN) && (glutGetModifiers() == GLUT_ACTIVE_CTRL)){
		ship->pitchAdjust(shipTurnSpeed);
	}

	//Roll right
	else if ((key == GLUT_KEY_RIGHT) && (glutGetModifiers() == GLUT_ACTIVE_CTRL)){
		ship->rollAdjust(shipTurnSpeed);
	}
	//Roll left
	else if ((key == GLUT_KEY_LEFT) && (glutGetModifiers() == GLUT_ACTIVE_CTRL)){
		ship->rollAdjust(-shipTurnSpeed);
	}
	else{
		//If no update occurrs, set updated to false.
		updatedship = true;
	}
}

void findMissleTarget(){

	float distance1 = -1.0f;
	float distance2 = -1.0f;

	//Determines if either missle site is in range
	if (missleSites[0]->isActive()){
		distance1 = abs(glm::distance(Utilities::getVector(missleSites[0]->getPrevModelMatrix(), 3), Utilities::getVector(smartMissles[0]->getPrevModelMatrix(), 3)));
		if (distance1 > missleDetect)
			distance1 = -1.0f;
	}
	if (missleSites[1]->isActive()){
		distance2 = abs(glm::distance(Utilities::getVector(missleSites[1]->getPrevModelMatrix(), 3), Utilities::getVector(smartMissles[0]->getPrevModelMatrix(), 3)));
		if (distance2 > missleDetect)
			distance2 = -1.0f;
	}

	//Sets the ships missle target to be closest missle site
	if (distance1 == -1.0f && distance2 == -1.0f)
		smartMissles[0]->setTarget(NULL);
	else if (distance1 == -1.0f && distance2 > 0)
		smartMissles[0]->setTarget(missleSites[1]);
	else if (distance2 == -1.0f && distance1 > 0)
		smartMissles[0]->setTarget(missleSites[0]);
	else if (distance1 > distance2)
		smartMissles[0]->setTarget(missleSites[1]);
	else
		smartMissles[0]->setTarget(missleSites[0]);
}

void gravityEffects(){
	//Get ships model matrix translation
	glm::vec3 tm(Utilities::getVector(ship->getPrevModelMatrix(), 3));

	//Calculate gravitational force
	double force = GRAVITY_CONSTANT / ((pow(tm[0], 2)) + (pow(tm[1], 2)) + (pow(tm[2], 2)));

	//Calculate gravity vector
	glm::vec3 gravity(Utilities::getVector(ship->getPrevModelMatrix(), 3) - Utilities::getVector(planets[0]->getPrevModelMatrix(), 3));
	
	//Scale gravity vector based on ships distance from ruber
	gravity = Utilities::setVectorMagnitude(gravity, (float)force);

	//Apply gravity to ship
	ship->setTranslationMatrix(glm::translate(glm::mat4(), -gravity) * ship->getTranslationMatrix());
}

void collissionCheck(){

	float spaceBetween;

	//ship collission with planet
	for (int i = 0; i < nPlanets; i++){
		//Check for collisions
		spaceBetween = abs(glm::distance(Utilities::getVector(ship->getPrevModelMatrix(), 3), Utilities::getVector(planets[i]->getPrevModelMatrix(), 3)))
			- ship->getModelSize() - planets[i]->getModelSize();
		//If distance < 10 pixels
		if (spaceBetween < 10){
			ship->setActive(false);
			gameOver = true;
			win = false;
			break;
		}
	}

	//Missle collision with missle site
	for (int index = 0; index < nMissles; index++){
		for (int i = 0; i < nMissleSites; i++){
			//Check for collisions
			spaceBetween = abs(glm::distance(Utilities::getVector(smartMissles[index]->getPrevModelMatrix(), 3), Utilities::getVector(missleSites[i]->getPrevModelMatrix(), 3)))
				- smartMissles[index]->getModelSize() - missleSites[i]->getModelSize();
			//If distance < 10 pixels
			if (spaceBetween < 10){
				//Check missle site doesnt destroy itself
				if (smartMissles[index]->getShooter() == missleSites[i] && smartMissles[index]->getCurrentDistance() < smartMissles[index]->getActivationDistance())
					continue;
				//Destroy missle site
				smartMissles[index]->setActive(false);
				missleSites[i]->setActive(false);
				smartMissles[index]->setTarget(NULL);

				//If both missle sites are destroyed then game won
				if (!missleSites[0]->isActive() && !missleSites[1]->isActive()){
					gameOver = true;
					win = true;
				}
				break;
			}
		}

		//missle collision with planet
		for (int i = 0; i < nPlanets; i++){
			//Check for collisions
			spaceBetween = abs(glm::distance(Utilities::getVector(smartMissles[index]->getPrevModelMatrix(), 3), Utilities::getVector(planets[i]->getPrevModelMatrix(), 3)))
				- smartMissles[index]->getModelSize() - planets[i]->getModelSize();
			//If distance < 10 pixels
			if (spaceBetween < 10){
				smartMissles[index]->setActive(false);
				smartMissles[index]->setTarget(NULL);
			}
		}

		//Missle collision with ship
		//Check for collisions
		spaceBetween = abs(glm::distance(Utilities::getVector(smartMissles[index]->getPrevModelMatrix(), 3), Utilities::getVector(ship->getPrevModelMatrix(), 3)))
			- smartMissles[index]->getModelSize() - ship->getModelSize();
		//If distance < 10 pixels
		if (spaceBetween < 20){
			//Check ship doesnt destory itself when firing missle
			if (!(smartMissles[index]->getShooter() == ship && smartMissles[index]->getCurrentDistance() < smartMissles[index]->getActivationDistance())){
				smartMissles[index]->setActive(false);
				ship->setActive(false);
				smartMissles[index]->setTarget(NULL);
				gameOver = true;
				win = false;
			}
		}
	}
	return;
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Warbird Simulation");

	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	
	//initialization
	initObjects();
	initCameras();
	init();
	initAnimationSpeeds();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyEvent);
	glutIdleFunc(display);
	glutTimerFunc(animationSpeeds[currentSpeed]->getSpeed(), intervalTimer, 1);
	glutMainLoop();

	printf("done\n");
	return 0;
}