#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, TexCoord);
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	//
	//color = vec4(TexCoord, 1.0, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}