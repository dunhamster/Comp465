/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# version 330 core

in vec2 vs_texCoord;
out vec4 fragColor;
uniform sampler2D Texture;

void main() {
    fragColor = texture(Texture, vs_texCoord);
  }