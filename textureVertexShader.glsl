/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# version 330 core

in vec4 vPosition;
in vec2 vTexCoord;
out vec2 vs_texCoord;  
uniform mat4 ViewProjection;

void main() {
    gl_Position = ViewProjection * vPosition;
    vs_texCoord = vTexCoord;
}