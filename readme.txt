-PHASE 2-
BRIAN DUNHAM
brian.dunham.747@my.csun.edu
WARBIRD SIMULATION PROJECT
BARNES COMP 465
CSUN FALL 2014
	                                                         ___ 
	                                                      ,o88888 
	                                                   ,o8888888' 
	                             ,:o:o:oooo.        ,8O88Pd8888" 
	                         ,.::.::o:ooooOoOoO. ,oO8O8Pd888'" 
	                       ,.:.::o:ooOoOoOO8O8OOo.8OOPd8O8O" 
	                      , ..:.::o:ooOoOOOO8OOOOo.FdO8O8" 
	 ________      ________ . ..:_:ooooOoO8_OO8_oOodO8O"________      
	|\   __  \    |\   __  \ ..:|\  \oOoO|\   __  \O8" |\   ___  \    
	\ \  \|\ /_   \ \  \|\  \ ::\ \  \oOo\ \  \|\  \   \ \  \\ \  \   
	 \ \   __  \   \ \   _  _\. :\ \  \oO8\ \   __  \   \ \  \\ \  \  
	  \ \  \|\  \   \ \  \\  \| .:\ \  \coo\ \  \ \  \   \ \  \\ \  \ 
	   \ \_______\   \ \__\\ _\.:.:\ \__\o:.\ \__\ \__\   \ \__\\ \__\
	    \|_______|    \|__|\|__|c:cc\|__|'..:\|__|\|__|    \|__| \|__|
	                :.:.    ,c:cccc"':.:.:.:.:.'
	              ..:.:"'`::::c:"'..:.:.:.:.:.'
	            ...:.'.:.::::." ' . . . . .'
	           .. . ....:."'
	         . . . ...."' 
	         .. . ."'
	        . 


--------------------------------------------------------------------------------------

DESCRIPTION PHASE 1:

This project was written using visual studio 2013 in C++ and uses OpenGL. This project 
demonstrates use of matrices and vectors in order to manipulate objects in 3D space.
The setting consists of a small solar system with orbiting planets and one spaceship.
Every Planet object inherits from the Shape3D class and is able to have a parent object.
This makes the program highly modifiable when adding new Planets to the solar system.
Also added different animation speeds which made debugging more effecient. From what
I have tested, everything works perfectly and it meets all the project specifications.
Used AC3D to make and export *.tri models.

DESCRIPTION PHASE 2:

Added complete ship movement to the project (See ship controls) as well as smart missiles,
enemy missile sites, collisions, and of course gravity!! Made a new model for missiles 
and a missile site model. I changed the missile travel speed and the ships rotation speed
to be much faster than what the specifications asked. Seemed weird to me that a planet's
orbiting speed is faster than the missile speed. It made the game too hard! As far as I
know, my phase 2 project works great. Due to chronic headaches and personal issues, I was
not able to clean up the project as much as I would have liked however, it does meet all
of the requirements for phase 2.

DESCRIPTION PHASE 3:

Added more complex lighting and simple textures to the project!! This involved writing
my own lighting and texture *.glsl files to handle the lighting. The simple textures
were created/displayed in the WarbirdSimulation.cpp file and they use their own *.glsl
files for lighting. One majorly huge issue I experienced was with the textures. I
followed the example program and lecture slides but I experienced some very strange errors.
The glDrawElements() call in my display() method would cause my program to instantly crash
when it would try to execute that line. After further research I found out that certain
graphic drivers have a hard time performing that operation. In addition, after talking to
other classmates, I found out that several people's programs would work on one computer
yet crash on a different computer that had different graphic card. After many hours of
bashing my head into the computer I finally came to a solution where I commented out
the line where I enable my VBO for textures: glEnableVertexAttribArray(VBO_T[i]);
I figured it was because that takes an attribute location as its argument, not a buffer id.
But this still confuses me when it comes to looking at the rose example program. However,
in the end my program works perfectly. One thing I added was a debug mode for the spot light.
By pressing the "d" key it makes seeing the spot light a whole lot easier. Other than that,
all lighting and textures are working and meet all specifications!
Or atleast they work on this computer... 

--------------------------------------------------------------------------------------

REQUIREMENTS:

Requires the includes465 library provided by Professor Barnes to run.

	PROCESSOR DEFINITION:
		_CRT_SECURE_NO_DEPRECATE

	LINKER INPUT:
		freeglut.lib
		glew32.lib

--------------------------------------------------------------------------------------

CONTROLS:

q = quit
v = switch to next camera
t = switch to next animation speed

LIGHT CONTROLS:

a = toggle ambient light
p = toggle point light
h = toggle spot light
d = toggle debug light

SHIP CONTROLS:

up = move forward
down = move backwards
left = turn left
right = turn right
ctrl up = pitch up
ctrl down = pitch down
ctrl left = roll left
ctrl right = roll right
s = switch to next ship speed
f = fire ship missile
w = warp ship to a planet






