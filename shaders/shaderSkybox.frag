#version 410

in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 pixelColor;

void main() {

	pixelColor = texture(skybox, TexCoords);

}