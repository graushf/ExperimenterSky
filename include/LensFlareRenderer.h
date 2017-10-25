#pragma once

#include "LensFlare.h"
#include <vector>

class LensFlareRenderer {
public:
	LensFlareRenderer();
	void render(std::vector<LensFlare*> lensf, float brightness, int toggleModeDraw);
private:


};