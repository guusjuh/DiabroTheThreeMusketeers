#include "SoundManager.h"
#include <irrklang.h>

bool SoundManager::initialize() {
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	engine->play2D("../../media/getout.ogg", true);

	return true;
}
