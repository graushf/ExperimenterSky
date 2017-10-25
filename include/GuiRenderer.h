#pragma once

#include "GuiTexture.h"
#include <vector>

class GuiRenderer {
public:
	GuiRenderer();
	void render(std::vector<GuiTexture*> guis, int toggleModeDraw);
private:


};