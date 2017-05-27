#include "SoundManager.h"

void SoundManager::enterEndRoom() {
	_backgroundMusic->setIsPaused(true);
	_epicBackgroundMusic->setIsPaused(false);
}

void SoundManager::exitEndRoom() {
	_backgroundMusic->setIsPaused(false);
	_epicBackgroundMusic->setIsPaused(true);
}

void SoundManager::triggerEndRoom(bool val) {
	if (_inEndRoom == val) return;

	_inEndRoom = val;

	if (_inEndRoom) enterEndRoom();
	else exitEndRoom();
}

void SoundManager::hit() {
	_engine->play2D("../../media/sounds/hit.wav");
}

void SoundManager::playerDead() {
	_engine->play2D("../../media/sounds/dead.wav");
}

void SoundManager::completed() {
	_engine->play2D("../../media/sounds/completion.wav");
}

void SoundManager::dialog() {
	_engine->play2D("../../media/sounds/dialog.mp3");
}





