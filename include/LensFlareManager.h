#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "LensFlareRenderer.h"
#include "Camera.h"

class LensFlareManager {

public:
	LensFlareManager(float spacing, std::vector<LensFlare*> flareTextures);
	~LensFlareManager();
	void render(Camera* camera, glm::vec3 sunWorldPos);

private:
	glm::vec2 CENTER_SCREEN = glm::vec2(0.5f, 0.5f);
	std::vector<LensFlare*> flareTextures;
	float spacing;
	LensFlareRenderer* renderer;
	glm::vec2 convertToScreenSpace(glm::vec3 worldPos, glm::mat4 viewMat, glm::mat4 projectionMat);
	void calcFlarePositions(glm::vec2 sunToCenter, glm::vec2 sunCoords);
};