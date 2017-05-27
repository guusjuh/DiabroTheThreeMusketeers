#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "irrklang.h"

class SoundManager {
public:
	SoundManager(bool soundOn = true) {
		_engine = irrklang::createIrrKlangDevice();
		if (soundOn) { _engine->setSoundVolume(0.5f); }
		else { _engine->setSoundVolume(0.0f); }

		_backgroundMusic = _engine->play2D("../../media/sounds/backgroundMusic.mp3", true, false, true);
		_epicBackgroundMusic = _engine->play2D("../../media/sounds/endRoomBackgroundMusic.mp3", true, true, true);

		_backgroundMusic->setVolume(0.5f);
		_epicBackgroundMusic->setVolume(0.5f);
	}

	~SoundManager() {
		_backgroundMusic->drop();
		_epicBackgroundMusic->drop();

		_engine->drop();
		delete _engine;
	}

	void enterEndRoom();
	void exitEndRoom();
	void triggerEndRoom(bool val = true);

	void hit();
	void playerDead();
	void completed();
	void dialog();

private:
	irrklang::ISoundEngine* _engine;

	irrklang::ISound* _backgroundMusic;
	irrklang::ISound* _epicBackgroundMusic;

	bool _inEndRoom;
};

#endif
