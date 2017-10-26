#include "LensFlareManager.h"

LensFlareManager::LensFlareManager(float spacing, std::vector<LensFlare*> flareTextures)
{
	this->spacing = spacing;
	this->flareTextures = flareTextures;
	this->renderer = new LensFlareRenderer();
}

void LensFlareManager::render(Camera* camera, glm::vec3 sunWorldPos) 
{
	std::cout << "SUN WORLD POSITION:      " << sunWorldPos.x << "   " << sunWorldPos.y << "   " << sunWorldPos.z << std::endl;

	// convert sun pos to screen coords
	glm::vec2 sunCoords = convertToScreenSpace(sunWorldPos, camera->GetViewMatrix(), camera->GetPerspectiveMatrix());
	if ((sunCoords.x == -1.0f) && (sunCoords.y == -1.0f)) {
		return;
	}
	glm::vec2 sunToCenter = CENTER_SCREEN - sunCoords;
	//std::cout << "sunToCenterLength:    " << glm::length(sunToCenter) << std::endl;
	float brightness = 1 - (glm::length(sunToCenter) / 0.6f);
	std::cout << "sunCoords:	" << sunCoords.x << "	" << sunCoords.y << std::endl;
	//std::cout << " brightness:	" << brightness << std::endl;
	//std::cout << " sunToCenter:		" << sunToCenter.x << "		" << sunToCenter.y << std::endl;
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
		direction = direction * ((i+1) * spacing);
		//direction = glm::vec2(+1, -1);

		glm::vec2 flarePos = sunCoords + direction;
		flareTextures[i]->setPosition(sunCoords);
	}
}

glm::vec2 LensFlareManager::convertToScreenSpace(glm::vec3 worldPos, glm::mat4 viewMat, glm::mat4 projectionMat)
{
	worldPos = glm::normalize(worldPos);
	glm::vec4 coords = glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	coords =  viewMat * coords; // CHECK IF ITS INVERSE ORDER
	coords = projectionMat * coords;
	coords = coords / coords.w;

	glm::vec2 screenCoords = glm::vec2(coords.x, coords.y)*0.5f + 0.5f; // Normalizing from -1 - 1 to 0 - 1 
	// 2*vec2()-1

	//float x = (coords.x / coords.w + 1) / 2.0f;
	////float y = (coords.y / coords.w + 1) / 2.0f;

	//float y = 1.0f - ((coords.y / coords.w + 1) / 2.0f);
	//
	//return glm::vec2(x, y);
	return screenCoords;
}