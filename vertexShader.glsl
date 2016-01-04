/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

out vec4 fColor;
out vec3 fPosition;
out vec3 fNormal;

uniform mat4 ModelViewProjection;
uniform mat4 ModelViewMatrix;


void main(){

    gl_Position = ModelViewProjection * vPosition;

    fColor = vColor;
    fNormal = (normalize(ModelViewMatrix * vec4(vNormal, 0))).xyz;
    fPosition = (ModelViewMatrix * vPosition).xyz;

}