#include "GuiRenderer.h"


GuiRenderer::GuiRenderer()
{

}

void GuiRenderer::render(std::vector<GuiTexture*> guis, int toggleModeDraw)
{
	for (int i = 0; i < guis.size(); ++i) {		
		GuiTexture* guitextureptr = guis.at(i);
		guitextureptr->draw(toggleModeDraw);
	}
}