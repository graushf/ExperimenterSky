#version 410

in vec3 vViewRay;
in mat4 cameraToWorldMatrix;
in mat4 invProjectionMatrix;

out vec4 pixelColor;

uniform vec2 resolution;

float hash(vec2 p) 
{ 
	return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x))));
}

float noise(vec2 x)
{
	vec2 i = floor(x);
	float a = hash(i);
	float b = hash(i + vec2(1.0, 0.0));
	float c = hash(i + vec2(0.0, 1.0));
	float d = hash(i + vec2(1.0, 1.0));

	vec2 f = fract(x);
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 p) 
{
	const mat2 m2 = mat2(0.8, -0.6, 0.6, 0.8);
	float f = 0.5000 * noise(p); 
	p = m2 * p * 2.02;
	f += 0.2500 * noise(p);
	p = m2 * p * 2.03;
	f += 0.1250 * noise(p); 
	p = m2 * p * 2.01;
	f += 0.0625 * noise(p);

	return f / 0.9375;
}

vec4 render(in vec3 ro, in vec3 rd) 
{
	vec4 col;
	// Over ground level
	if (rd.y > 0.0) {
		col = vec4(0.5 * smoothstep(0.5, 0.8, fbm((ro.xz + rd.xz * (250000.0 - ro.y) / rd.y) * 0.000008)));
	}
	return col;
}

void main() {
	vec3 rd = normalize(mat3(cameraToWorldMatrix) * vec3((invProjectionMatrix * vec4(gl_FragCoord.xy / resolution.xy * 2.0 - 1.0, -1.0, 1.0)).xy, -1.0));
	vec4 _pixelColor = render(cameraToWorldMatrix[3].xyz, rd);
	_pixelColor.w *= 0.50;
	//float opacity = 1.0;
	//if (_pixelColor.x > 0.05) {
	//	opacity = 1.0;
	//} else {
	//	opacity = 0.0;
	//}
	pixelColor = _pixelColor;
}