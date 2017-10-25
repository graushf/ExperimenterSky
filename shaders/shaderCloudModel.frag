#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture;

uniform int number_iterations;
uniform float C_0;
uniform float FX_0;
uniform float FY_0;

void main()
{
	//color = texture(texture, TexCoord);
	color = vec4(TexCoord, 0.0, 1);
	//color = vec4(1.0, 0.0, 1.0, 1.0);
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	//
	//color = vec4(TexCoord, 1.0, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}