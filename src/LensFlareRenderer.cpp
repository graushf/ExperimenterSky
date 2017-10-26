#include "LensFlareRenderer.h"


LensFlareRenderer::LensFlareRenderer()
{

}

void LensFlareRenderer::render(std::vector<LensFlare*> lensf, float brightness, int toggleModeDraw)
{
	glDisable(GL_DEPTH_TEST);

	for (int i = 0; i < lensf.size(); ++i) {
		LensFlare* lensfptr = lensf.at(i);
		lensfptr->draw(brightness, toggleModeDraw);
	}

	glEnable(GL_DEPTH_TEST);

}