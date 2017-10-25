#include "LensFlareRenderer.h"


LensFlareRenderer::LensFlareRenderer()
{

}

void LensFlareRenderer::render(std::vector<LensFlare*> lensf, float brightness, int toggleModeDraw)
{
	for (int i = 0; i < lensf.size(); ++i) {
		LensFlare* lensfptr = lensf.at(i);
		lensfptr->draw(brightness, toggleModeDraw);
	}
}