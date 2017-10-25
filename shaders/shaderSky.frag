#version 330 core

in vec3 vViewRay;
in vec2 TexCoord;

uniform vec3 uSunDir;
uniform sampler2D textureNight;
uniform float gammaMix;

uniform int isNightTransition;
uniform float gamma1;


uniform int isNightSky;
uniform float gamma2;

out vec4 color;

#define iSteps 16
#define jSteps 8
#define PI 3.141592

vec2 rsi(vec3 r0, vec3 rd, float sr) {
	// ray-sphere intersection that assumes
	// the sphere is centered at the origin.
	// No intersection when result.x > result.y
	float a = dot(rd, rd);
	float b = 2.0 * dot(rd, r0);
	float c = dot(r0, r0) - (sr * sr);
	float d = (b*b) - 4.0*a*c;
	if (d < 0.0) return vec2(1e5, -1e5);
	return vec2(
		(-b - sqrt(d))/(2.0*a),
		(-b + sqrt(d))/(2.0*a)
	);
}

vec3 atmosphere(vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
	// Normalize the sun and view directions.
	pSun = normalize(pSun);
	r = normalize(r);

	// Calculate the step size of the primary ray.
	vec2 p = rsi(r0, r, rAtmos);
	if (p.x > p.y) return vec3(0,0,0);
	p.y = min(p.y, rsi(r0, r, rPlanet).x);
	float iStepSize = (p.y - p.x) / float(iSteps);

	// Initialize the primary ray time.
	float iTime = 0.0;

	// Initialize accumulators for Rayleigh and Mie scaterring.
	vec3 totalRlh = vec3(0,0,0);
	vec3 totalMie = vec3(0,0,0);

	// Initialize optical depth accumulators for the primary ray.
	float iOdRlh = 0.0;
	float iOdMie = 0.0;

	// Calculate the Rayleigh and Mie phases.
	float mu = dot(r, pSun);
	float mumu = mu * mu;
	float gg = g * g;
	float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
	float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

	// Sample the primary ray.
	for (int i = 0; i < iSteps; i++) {
		
		// Calculate the primary ray sample position.
		vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

		// Calculate the height of the sample.
		float iHeight = length(iPos) - rPlanet;

		// Calculate the optical depth of the Rayleigh and Mie scattering for this step.
		float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
		float odStepMie = exp(-iHeight / shMie) * iStepSize;

		// Accumulate optical depth.
		iOdRlh += odStepRlh;
		iOdMie += odStepMie;

		// Calculate the step size of the secondary ray.
		float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

		// Initialize the secondary ray time.
		float jTime = 0.0;

		// Initialize optical depth accumulators for the secondary ray.
		float jOdRlh = 0.0;
		float jOdMie = 0.0;

		// Sample the secondary ray.
		for (int j = 0; j < jSteps; j++) {
			
			// Calculate the secondary ray sample position
			vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

			// Calculate the height of the sample.
			float jHeight = length(jPos) - rPlanet;

			// Accumulate the optical depth.
			jOdRlh += exp(-jHeight / shRlh) * jStepSize;
			jOdMie += exp(-jHeight / shMie) * jStepSize;

			// Increment the secondary ray time.
			jTime += jStepSize;
		}

		// Calculate attenuation.
		vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

		// Accumulate scattering.
		totalRlh += odStepRlh * attn;
		totalMie += odStepMie * attn;

		// Increment the primary ray time.
		iTime += iStepSize;
	}

	// Calculate and return the final color.
	return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

void main()
{
	//color = vec4(1.0f, 0.0f, 0.7f, 1.0f);

	vec3 vView = vec3(vViewRay.x, vViewRay.z, vViewRay.y);
	vec3 vSkyColor = atmosphere(normalize(vViewRay), vec3(0, 6372e3, 0), uSunDir, 22.0, 6371e3, 6471e3, vec3(5.5e-6, 13.0e-6, 22.4e-6), 21e-6, 8e3, 1.2e3, 0.758);
	vSkyColor = 1.0 - exp(-1.0 * vSkyColor);
	 
	//color = vec4(normalize(vViewRay), 1.0);
	//if (gammaMix == 1.0) {
	//	color = texture(textureNight, vec2(TexCoord.x, 1 - TexCoord.y));
	//} else {
		//color = vec4(vSkyColor, 1.0);
	//}

	color = mix(vec4(vSkyColor, 1.0), texture(textureNight, vec2(TexCoord.x, 1 - TexCoord.y)), gammaMix);

	if (isNightTransition == 1) {
		color = mix(vec4(vSkyColor, 1.0), vec4(0.0, 0.0, 0.0, 1.0), gamma1);
	}

	if (isNightSky == 1) {
		color = mix(vec4(0.0, 0.0, 0.0, 1.0), texture(textureNight, vec2(TexCoord.x, 1 - TexCoord.y)), gamma2);
	}

	color = vec4(vViewRay, 1.0);
}