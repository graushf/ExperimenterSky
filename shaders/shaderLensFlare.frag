#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D textureSampler;

uniform float brightness;

uniform int number_iterations;
uniform float C_0;
uniform float FX_0;
uniform float FY_0;

void main()
{
	color = texture(textureSampler, TexCoord);
	color = vec4(color.x * brightness, color.y * brightness, color.z * brightness, color.w);
	//color = vec4(TexCoord, 0.0, 1);
	//color = vec4(1.0, 0.0, 1.0, 1.0);
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	//
	//color = vec4(TexCoord, 1.0, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}