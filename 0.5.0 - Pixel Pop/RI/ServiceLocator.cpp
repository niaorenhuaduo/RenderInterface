#include "..\Include\ServiceLocator.h"
#include "..\Include\IRenderSystem.h"


namespace ri
{

IRenderSystem*   ServiceLocator::mpRenderSystem = 0;
ITextureManager* ServiceLocator::mpTextureManager = 0;


} // namespace ri
