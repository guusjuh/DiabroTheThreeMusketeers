#include "UIManager.h"
#include "GameManager.h"


/// <summary>
/// Creates a new instance of the <see cref="UIManager"/> class.
/// This class is created by the <see cref="GameManager" /> and contains all UI information, 
/// e.g. the in-game and menu UI.
/// </summary>
UIManager::UIManager()
	: _uiNode(0), _playerHealthBarWidget(0), _maxWidthBar(0), _enemyHealthBarWidget(0), _questOn(true),
	_mWindow(0), _hudTextWidget(0), _hudTotalTimer(3), _hudTextWithTimeOn(false), _uiElementMgr(0), _mDialogTextArea(0), _storyTextOn(false)
{
}

/// <summary>
/// Initializes this instance and sets up UI.
/// </summary>
void UIManager::init()
{
	_uiElementMgr = new DiabroUI::UIElementsManager("MyInterface", _mWindow);

	setupUI();

	_maxWidthBar = _playerHealthBarWidget->getOverlayElement()->getWidth();

	_startGameText.push_back("Sis, where are you? Where did you go?");
	_startGameText.push_back("This cave is huge.. How can I find her?");
	_startGameText.push_back("I'll have to use my wisdom and my sword");

	_startLevelText.push_back("Where did she go now?!");
	_startLevelText.push_back("I thought I just found her.");
	_startLevelText.push_back("Stupid kids, always getting in trouble...");

	_endLevelText.push_back("There you are!");
	_endLevelText.push_back("I searched all over the place for you!");
	_endLevelText.push_back("Now, let's go back home.");

	_diedText.push_back("You died!");
	_diedText.push_back("Better luck next time.");

	_first = true;
}

/// <summary>
/// Setups the UI elements.
/// </summary>
void UIManager::setupUI()
{
	_uiNode = GameManager::getSingleton().getLevelManager()->getCamNode()->createChildSceneNode("UINode");
	_uiNode->setPosition(0, 0, 0);

	// create health bars
	_playerHealthBarWidget = _uiElementMgr->createHealthBar(DiabroUI::BOTTOMLEFT, "Player", "PlayerHealth", 256, 256, 0, GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth(), 1);
	_maxWidthBar = _playerHealthBarWidget->getOverlayElement()->getWidth();

	_miniMap = _uiElementMgr->createMiniMap(DiabroUI::TOP, "MiniMap", 256, -180, 180);

	_hudTextWidget = nullptr;
}

void UIManager::startUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_first ? _startGameText : _startLevelText, 2.0f)){
				_first = false;
				GameManager::getSingletonPtr()->goNextState();
			}
		}
	}
	else {
		!showStoryText(_startGameText, 2.0f);
	}
}
void UIManager::inGameUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
		}
	}
}
void UIManager::endUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_endLevelText, 2.0f)) {
				GameManager::getSingletonPtr()->goNextState();
			}
		}
	}
	else {
		showStoryText(_endLevelText, 2.0f);
	}
}
void UIManager::diedUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_diedText, 2.0f)) {
				GameManager::getSingletonPtr()->goToState(GameState::InGame);
			}
		}
	}
	else {
		showStoryText(_diedText, 2.0f);
	}
}

bool UIManager::showStoryText(std::vector<std::string> pTextVector, float timer) {
	static int _textCount = 0;
	if (!_storyTextOn) {
		_storyTextOn = true;
		showHUDText((pTextVector[_textCount]), timer);
	}
	else {
		_textCount++;
		if (_textCount < (pTextVector.size())) {
			showHUDText(pTextVector[_textCount], timer);
		}
		else {
			hideHUDText();
			_textCount = 0;
			_storyTextOn = false;
		}
	}

	return _storyTextOn;
}

void UIManager::showHUDText(Ogre::String pHUDText)
{
	// count the symbols in text
	int count = pHUDText.length();
	float width = count * 14.25f;

	if (_hudTextWidget != nullptr)
		hideHUDText();

	_hudTextWidget = _uiElementMgr->createHUDText(DiabroUI::TOP, "HUDText", pHUDText, width, 40);
	_hudTextWidget->getOverlayElement()->setTop(128);
}
void UIManager::showHUDText(Ogre::String pHUDText, float time)
{
	showHUDText(pHUDText);

	_hudTextWithTimeOn = true;
	_hudTimer = time;
}
void UIManager::hideHUDText()
{
	_hudTextWithTimeOn = false;

	if (_hudTextWidget != nullptr) {
		_uiElementMgr->destroyWidget("HUDText");
		_hudTextWidget = nullptr;
	}

	_hudTimer = 0;
}

void UIManager::showDialog(Ogre::String pNPCName, Ogre::String pDialogText) {
	if (_hudTextWidget != nullptr) hideHUDText();

	_mDialogTextArea = _uiElementMgr->createDialogTextBox(DiabroUI::CENTER, "DialogTextArea", pNPCName, 400, 400);
	_mDialogTextArea->setText(pDialogText);
}
void UIManager::appendDialogText(Ogre::String pDialogText) {
	_mDialogTextArea->appendText("\n" + pDialogText);
}
void UIManager::hideDialog() {
	if (_mDialogTextArea != nullptr) {
		_uiElementMgr->destroyWidget("DialogTextArea");
		_mDialogTextArea = nullptr;
	}
}

/// <summary>
/// Adjusts the health bar pValue.
/// </summary>
/// <param name="pValue">The pValue.</param>
/// <param name="pMaxValue">The maximum pValue.</param>
void UIManager::adjustHealthBar(Ogre::Real pValue, Ogre::Real pMaxValue)
{
	_playerHealthBarWidget->setValue(pValue, calcBarSize(pValue, pMaxValue, _maxWidthBar));
}

/// <summary>
/// Adjusts the health bar pValue.
/// </summary>
/// <param name="pValue">The pValue.</param>
/// <param name="pMaxValue">The maximum pValue.</param>
void UIManager::adjustEnemyHealthBar(Ogre::Real pValue, Ogre::Real pMaxValue)
{
	_enemyHealthBarWidget->setValue(pValue, calcBarSize(pValue, pMaxValue, _maxWidthBar));
}

void UIManager::showEnemyHealthBar()
{
	if (_enemyHealthBarWidget != nullptr) { hideEnemyHealthBar(); }

	_enemyHealthBarWidget = _uiElementMgr->createHealthBar(DiabroUI::BOTTOMRIGHT, "Enemy", "EnemyHealth", 256, 256, 0, GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth(), 1);
}

void UIManager::hideEnemyHealthBar() {
	if (_enemyHealthBarWidget != nullptr) {
		_uiElementMgr->destroyWidget("EnemyHealth");
		_enemyHealthBarWidget = nullptr;
	}
}

/// <summary>
/// Calculates the new size of the bar.
/// </summary>
/// <param name="pValue">The current pValue.</param>
/// <param name="pMaxValue">The maximum pValue.</param>
/// <param name="maxSize">The maximum size.</param>
/// <returns></returns>
Ogre::Real UIManager::calcBarSize(Ogre::Real pValue, Ogre::Real pMaxValue, Ogre::Real pMaxSize)
{
	return((pValue / pMaxValue) * pMaxSize);
}

void UIManager::updateMiniMapLocators() {
	// update sis

	Ogre::Real angle = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getSisPos());
	_miniMap->setValueSister(angle, calcLocatorPos(angle, 180, -180, 256));

	// update quest 
	if (_questOn) {
		angle = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[0]->getPosition());
		_miniMap->setValueQuest(angle, calcLocatorPos(angle, 180, -180, 256));
	}
}

void UIManager::setQuestOn(bool val) 
{
	if(_questOn == val) {
		return;
	}

	_questOn = val;

	if(!_questOn) {
		_miniMap->getQuestLocator()->hide();
	}else {
		_miniMap->getQuestLocator()->show();
	}
}


Ogre::Real UIManager::calcLocatorPos(Ogre::Real angle, Ogre::Real pMaxValue, Ogre::Real pMinValue, Ogre::Real pMaxSize)
{
	Ogre::Real returnValue = ((angle - pMinValue) / (Ogre::Math::Abs(pMinValue - pMaxValue)) * pMaxSize);
	return returnValue;
}
