#include "LensFlareManager.h"

LensFlareManager::LensFlareManager(float spacing, std::vector<LensFlare*> flareTextures)
{
	this->spacing = spacing;
	this->flareTextures = flareTextures;
	this->renderer = new LensFlareRenderer();
}

void LensFlareManager::render(Camera* camera, glm::vec3 sunWorldPos) 
{
	// convert sun pos to screen coords
	glm::vec2 sunCoords = convertToScreenSpace(sunWorldPos, camera->GetViewMatrix(), camera->GetPerspectiveMatrix());
	if ((sunCoords.x == -1.0f) && (sunCoords.y == -1.0f)) {
		return;
	}
	glm::vec2 sunToCenter = CENTER_SCREEN - sunCoords;
	float brightness = 1 - (sunToCenter.length() / 0.6f);
	if (brightness > 0) {
		calcFlarePositions(sunToCenter, sunCoords);
		renderer->render(flareTextures, brightness, 1.0);
	}
}

void LensFlareManager::calcFlarePositions(glm::vec2 sunToCenter, glm::vec2 sunCoords)
{
	for (int i = 0; i < flareTextures.size(); ++i) 
	{
		glm::vec2 direction = sunToCenter;
		direction = direction * (i * spacing);
		glm::vec2 flarePos = sunCoords + direction;
		flareTextures[i]->setPosition(flarePos);
	}
}

glm::vec2 LensFlareManager::convertToScreenSpace(glm::vec3 worldPos, glm::mat4 viewMat, glm::mat4 projectionMat)
{
	glm::vec4 coords = glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	coords = coords * viewMat; // CHECK IF ITS INVERSE ORDER
	coords = coords * projectionMat;
	if (coords.w <= 0)
	{
		return glm::vec2(-1.0f, -1.0f);
	}
	float x = (coords.x / coords.w + 1) / 2.0f;
	float y = 1.0f - ((coords.y / coords.w + 1) / 2.0f);
	
	return glm::vec2(x, y);
}