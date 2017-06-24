#include "UIManager.h"
#include "GameManager.h"
#include <math.h>

/// <summary>
/// Creates a new instance of the <see cref="UIManager"/> class.
/// This class is created by the <see cref="GameManager" /> and contains all UI information, 
/// e.g. the in-game and menu UI.
/// </summary>
UIManager::UIManager()
	: _uiNode(0), _playerHealthBarWidget(0), _maxWidthBar(0), _enemyHealthBarWidget(0), _questOn(true), _currentTransparancyScale(3.5f), _maxTransparancyScale(3.5f),
	_mWindow(0), _hudTextWidget(0), _hudTotalTimer(3), _hudTextWithTimeOn(false), _uiElementMgr(0), _mDialogTextArea(0), _storyTextOn(false), _showLoading(false)
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
	_playerHealthBarWidget = _uiElementMgr->createHealthBar(DiabroUI::BOTTOMLEFT, "Player", "PlayerHealth", 256, 256, 0, GameManager::getSingletonPtr()->getPlayer()->getMaxHealth(), 1);
	_maxWidthBar = _playerHealthBarWidget->getOverlayElement()->getWidth();

	_miniMap = _uiElementMgr->createMiniMap(DiabroUI::TOP, "MiniMap", 256, -90, 90);
	Ogre::Real angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getNorth());
	_miniMap->setValueNorth(angle, calcLocatorPos(angle, _maxWidthBar));
	angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getSouth());
	_miniMap->setValueSouth(angle, calcLocatorPos(angle, _maxWidthBar));

	_floorTextWidget = _uiElementMgr->createText(DiabroUI::TOPLEFT, "FloorIndicator", "FloorText", "Floor " + Ogre::StringConverter::toString(1), 40, 40);
	_questGoalTextWidget = _uiElementMgr->createText(DiabroUI::TOPLEFT, "InstructionIndicator", "QuestGoalText", "Test instruction!", 40, 10);
	_questGoalTextWidget->getOverlayElement()->setTop(128);
	_questGoalTextWidget->hide();

	_upgradeTextWidget = _uiElementMgr->createText(DiabroUI::TOPRIGHT, "UpgradeIndicator", "UpgradeIndicator", "Upgrades: ", 40, 20);
	_healthUpgradeIcon = _uiElementMgr->createUpgradeIcon(DiabroUI::TOPRIGHT, "Health", "HealthUpgrade", 64, 56);
	_dmgUpgradeIcon = _uiElementMgr->createUpgradeIcon(DiabroUI::TOPRIGHT, "Damage", "DamageUpgrade", 64, 56);

	setUpgradeText(Health);
	setUpgradeText(Damage);
	setQuestOn(false);

	_hudTextWidget = nullptr;

	_dialogOn = false;
	_enemyBarOn = false;
	_hudTextOn = false;

	_uiElementMgr->showBackdrop("UI/BloodBackdrop");
	_uiElementMgr->getBackdropLayer()->setScale(_currentTransparancyScale, _currentTransparancyScale);
}

/// <summary>
/// Update loop in start state (when player is starting the level).
/// </summary>
/// <param name="pFE">The frameevent to obtain the passed time.</param>
void UIManager::startUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_first ? _startGameText : _startLevelText, 2.5f)){
				_first = false;
				GameManager::getSingletonPtr()->goToState(InGame);
			}
		}
	}
	else {
		!showStoryText(_startGameText, 2.5f);
	}
}

/// <summary>
/// Update loop in ingame state (when player is playing the game).
/// </summary>
/// <param name="pFE">The frameevent to obtain the passed time.</param>
void UIManager::inGameUpdate(const Ogre::FrameEvent& pFE)
{
	updateMiniMapLocators();

	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;
		_hudTextWidget->getOverlayElement()->setTop(128);

		if (_hudTimer <= 0) {
			hideHUDText();
		}
	}
}

/// <summary>
/// Update loop in end state (when player finished the level).
/// </summary>
/// <param name="pFE">The frameevent to obtain the passed time.</param>
void UIManager::endUpdate(const Ogre::FrameEvent& pFE)
{
	static float loadingTimer = 3.0f;

	if (_showLoading) {
		loadingTimer -= pFE.timeSinceLastFrame;
		if(loadingTimer <= 0) {
			GameManager::getSingletonPtr()->goToState(Start);
			_showLoading = false;
			loadingTimer = 3.0f;
		}
		return;
	}
	else if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_endLevelText, 2.5f)) {
				_showLoading = true;
				hideHUDText();
				showLoadingScreen();
			}
		}
	}
	else if(!_showLoading) {
		showStoryText(_endLevelText, 2.5f);
	}
}

/// <summary>
/// Update loop in died state (when player is dead).
/// </summary>
/// <param name="pFE">The frameevent to obtain the passed time.</param>
void UIManager::diedUpdate(const Ogre::FrameEvent& pFE)
{
	static float loadingTimer = 3.0f;

	if (_showLoading) {
		loadingTimer -= pFE.timeSinceLastFrame;
		if (loadingTimer <= 0) {
			GameManager::getSingletonPtr()->goToState(InGame);
			_showLoading = false;
			loadingTimer = 3.0f;
		}
		return;
	}
	else if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showStoryText(_diedText, 2.5f)) {
				_showLoading = true;
				hideHUDText();
				showLoadingScreen();
			}
		}
	}
	else if (!_showLoading) {
		showStoryText(_diedText, 2.5f);
	}
}

/// <summary>
/// Shows the next part of the story text.
/// </summary>
/// <param name="pTextVector">Vector containing strings of story tekst.</param>
/// <param name="pTime">The time to show the text.</param>
/// <returns></returns>
bool UIManager::showStoryText(std::vector<std::string> pTextVector, float pTime) {
	static int _textCount = 0;
	if (!_storyTextOn) {
		_storyTextOn = true;
		showHUDText((pTextVector[_textCount]), pTime);
	}
	else {
		_textCount++;
		if (_textCount < (pTextVector.size())) {
			showHUDText(pTextVector[_textCount], pTime);
		}
		else {
			hideHUDText();
			_textCount = 0;
			_storyTextOn = false;
		}
	}

	return _storyTextOn;
}

/// shows a certain text within the HUD
/// \param pHUDText text that should be shown
void UIManager::showHUDText(Ogre::String pHUDText)
{
	// count the symbols in text
	int count = pHUDText.length();
	float width = count * 14.25f;

	if (_hudTextWidget != nullptr)
		hideHUDText();

	_hudTextOn = true;

	_hudTextWidget = _uiElementMgr->createHUDText(DiabroUI::TOP, "HUDText", pHUDText, width, 40);
	_hudTextWidget->getOverlayElement()->setTop(128);
}

///shows a text on the HUD
/// \param pHUDText text that should be shown
/// \param pTime
void UIManager::showHUDText(Ogre::String pHUDText, float pTime)
{
	showHUDText(pHUDText);

	_hudTextWithTimeOn = true;
	_hudTimer = pTime;
}

///hides the current message shown in the HUD
void UIManager::hideHUDText()
{
	_hudTextWithTimeOn = false;

	if (_hudTextWidget != nullptr) {
		_hudTextOn = false;
		_uiElementMgr->destroyWidget("HUDText");
		_hudTextWidget = nullptr;
	}

	_hudTimer = 0;
}

/// shows the dialog window
void UIManager::showDialog(Ogre::String pNPCName, Ogre::String pDialogText) {
	if (_hudTextWidget != nullptr) hideHUDText();

	_mDialogTextArea = _uiElementMgr->createDialogTextBox(DiabroUI::CENTER, "DialogTextArea", pNPCName, "Press 'SPACE' to continue.", 400, 400);
	_mDialogTextArea->setText(pDialogText);
	_dialogOn = true;
}

/// adds text to the dialog window
/// \param pDialogText text to append
void UIManager::appendDialogText(Ogre::String pDialogText) {
	_mDialogTextArea->appendText(pDialogText);
}

/// hides the dialog window
void UIManager::hideDialog() {
	if (_mDialogTextArea != nullptr) {
		_dialogOn = false;
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
	pValue = Ogre::Math::Clamp<Ogre::Real>(pValue, 0, pMaxValue);

	pValue = floor(pValue * 100) / 100;

	_playerHealthBarWidget->setValue(pValue, calcBarSize(pValue, pMaxValue, _maxWidthBar));

	_currentTransparancyScale = ((pValue / pMaxValue) * (_maxTransparancyScale - 1)) + 1.0f;
	Debug("", _currentTransparancyScale);
	_uiElementMgr->showBackdrop("UI/BloodBackdrop");
	_uiElementMgr->getBackdropLayer()->setScale(_currentTransparancyScale, _currentTransparancyScale);
}

/// <summary>
/// Adjusts the health bar pValue.
/// </summary>
/// <param name="pValue">The pValue.</param>
/// <param name="pMaxValue">The maximum pValue.</param>
void UIManager::adjustEnemyHealthBar(Ogre::Real pValue, Ogre::Real pMaxValue)
{
	pValue = floor(pValue * 100) / 100;

	_enemyHealthBarWidget->setValue(pValue, calcBarSize(pValue, pMaxValue, _maxWidthBar));
}

///shows the enemy healthbar
void UIManager::showEnemyHealthBar()
{
	if (_enemyHealthBarWidget != nullptr) {
		_enemyBarOn = false;
		hideEnemyHealthBar();
	}

	_enemyBarOn = true;

	_enemyHealthBarWidget = _uiElementMgr->createHealthBar(DiabroUI::BOTTOMRIGHT, "Enemy", "EnemyHealth", 256, 256, 0, GameManager::getSingletonPtr()->getPlayer()->getMaxHealth(), 1);
}

///hides the enemy healthbar
void UIManager::hideEnemyHealthBar() {
	if (_enemyHealthBarWidget != nullptr) {
		_enemyBarOn = false;
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
	return ((pValue / pMaxValue) * pMaxSize);
}

/// updates the position of the locators on the minimap.
void UIManager::updateMiniMapLocators() {
	// update sis
	Ogre::Real angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getSisPos());
	_miniMap->setValueSister(angle, calcLocatorPos(angle, _maxWidthBar));

	angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getNorth());
	_miniMap->setValueNorth(angle, calcLocatorPos(angle, _maxWidthBar));

	angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getSouth());
	_miniMap->setValueSouth(angle, calcLocatorPos(angle, _maxWidthBar));

	// update quest 
	if (_questOn) {
		angle = GameManager::getSingletonPtr()->getPlayer()->angleBetween(_questTargetPos);
		_miniMap->setValueQuest(angle, calcLocatorPos(angle, _maxWidthBar));
	}
}

/// defines whether the quest locator should be visible or not
/// \param val should the quest locator be shown
void UIManager::setQuestOn(bool val) 
{
	if(_questOn == val) {
		return;
	}

	_questOn = val;

	if(!_questOn) {
		_miniMap->getQuestLocator()->hide();
		_questGoalTextWidget->hide();
	}else {
		_miniMap->getQuestLocator()->show();
		_questGoalTextWidget->show();
		_questGoalTextWidget->setText(GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getCurrentInstruction());
	}
}

void UIManager::setQuestTarget(Ogre::Vector3 position) {
	_questTargetPos = position;
	_questGoalTextWidget->setText(GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getCurrentInstruction());
}

/// calculates the position of the locator as an angle
/// \param pAngle the angle between a player and the locator's object
/// \param pMaxValue the maximum value in
/// \param pMinValue the minimal value of
/// \param pMaxSize the maximum size of the bar
Ogre::Real UIManager::calcLocatorPos(Ogre::Real pAngle, Ogre::Real pMaxSize)
{
	if (pAngle < -90) pAngle = -90;
	if (pAngle > 90) pAngle = 90;
		
	Ogre::Real returnValue = ((pAngle + 90) / 180 * pMaxSize);
	return returnValue;
}

void UIManager::increaseFloorText() {
	_floorTextWidget->setText("Floor " + Ogre::StringConverter::toString(GameManager::getSingletonPtr()->getLevelManager()->getCurrentLevel()));
}

void UIManager::resetFloorText() {
	_floorTextWidget->setText("Floor " + Ogre::StringConverter::toString(1));
}

void UIManager::setUpgradeText(UpgradeModifierType type) {
	switch(type) {
	case Health:
		_healthUpgradeIcon->setValue(GameManager::getSingletonPtr()->getPlayer()->healthUpgradeAmount());
		break;
	case Damage:
		_dmgUpgradeIcon->setValue(GameManager::getSingletonPtr()->getPlayer()->damageUpgradeAmount());
		break;
	default:
		break;
	}
}

void UIManager::resetUpgradeText() {
	_healthUpgradeIcon->setValue(1);
	_dmgUpgradeIcon->setValue(1);
}

void UIManager::showPauseScreen() {
	hideAllIngameElements();

	_uiElementMgr->showBackdrop("UI/PauseBackdrop");

	// title: game is paused
	Ogre::String pausedString = "The game is paused.";
	int count = pausedString.length();  				// count the symbols in text
	float width = count * 14.25f;

	_pauseText = _uiElementMgr->createHUDText(DiabroUI::TOP, "PauseText", pausedString, width, 40);
	_pauseText->getOverlayElement()->setTop(128);

	// wide dialog
	_informationTextBox = _uiElementMgr->createDialogTextBox(DiabroUI::CENTER, "InformationTextArea", "Information", "Press 'P' to continue playing!", 500, 400);
	std::string informationString = "";
	informationString.append("Current floor: " + Ogre::StringConverter::toString(GameManager::getSingletonPtr()->getLevelManager()->getCurrentLevel()) + "\n\n" +
		"Values due to ugrades: \n"
		"- Health: " + Ogre::StringConverter::toString(GameManager::getSingletonPtr()->getPlayer()->getMaxHealth()) + "\n" +
		"- Damage: " + Ogre::StringConverter::toString(GameManager::getSingletonPtr()->getPlayer()->getDamage()) + "\n\n");
	
	if (_questOn) informationString.append("Current quest action: " + GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getCurrentInstruction() );

	_informationTextBox->setText(informationString);
}

void UIManager::hidePauseScreen() {
	// pauseText
	_uiElementMgr->destroyWidget("PauseText");
	_pauseText = nullptr;

	// information panel
	_uiElementMgr->destroyWidget("InformationTextArea");
	_informationTextBox = nullptr;

	_uiElementMgr->hideBackdrop();

	showAllIngameElements();
}

void UIManager::showMainMenu() {
	hideAllIngameElements();

	// background img
	_uiElementMgr->showBackdrop("UI/MenuBackdrop");

	// start game txt
	Ogre::String startTxt = "Press any key to start playing!";
	int count = startTxt.length();  				// count the symbols in text
	float width = count * 14.25f;
	_startGameTextElement = _uiElementMgr->createHUDText(DiabroUI::BOTTOM, "StartGameText", startTxt, width, 40);

	_startGameTextElement->getOverlayElement()->setTop(-128);

	// logo
	_logoImg = _uiElementMgr->createDecorWidget(DiabroUI::TOP, "Logo", "UI/Logo");
}

void UIManager::hideMainMenu() {
	_uiElementMgr->destroyWidget("StartGameText");
	_startGameTextElement = nullptr;

	_uiElementMgr->destroyWidget("Logo");
	_logoImg = nullptr;

	_uiElementMgr->hideBackdrop();

	showAllIngameElements();
}

void UIManager::showLoadingScreen() {
	hideAllIngameElements();

	// background img
	_uiElementMgr->showBackdrop("UI/MenuBackdrop");

	// start game txt
	Ogre::String startTxt = "Generating the dungeon and quests... Please be patient.";
	int count = startTxt.length();  				// count the symbols in text
	float width = count * 14.25f;
	_startGameTextElement = _uiElementMgr->createHUDText(DiabroUI::BOTTOM, "StartGameText", startTxt, width, 40);

	_startGameTextElement->getOverlayElement()->setTop(-128);
}

void UIManager::hideLoadingScreen() {
	_uiElementMgr->destroyWidget("StartGameText");
	_startGameTextElement = nullptr;

	_uiElementMgr->hideBackdrop();

	showAllIngameElements();
}

void UIManager::hideAllIngameElements() {
	_uiElementMgr->hideBackdrop();
	_uiElementMgr->getBackdropLayer()->setScale(1.0f, 1.0f);

	// _mDialogTextArea
	if(_dialogOn) _mDialogTextArea->hide();

	// _miniMap
	_miniMap->hide();

	if (_questOn) _questGoalTextWidget->hide();

	// _playerHealthBarWidget
	_playerHealthBarWidget->hide();

	// _enemyHealthBarWidget
	if(_enemyBarOn) _enemyHealthBarWidget->hide();

	// _floorTextWidget
	_floorTextWidget->hide();

	// upgrade text
	_upgradeTextWidget->hide();

	// _healthUpgradeIcon
	_healthUpgradeIcon->hide();
	// _dmgUpgradeIcon
	_dmgUpgradeIcon->hide();

	// _hudTextWidget
	if (_hudTextOn) _hudTextWidget->hide();
}

void UIManager::showAllIngameElements() {
	_uiElementMgr->showBackdrop("UI/BloodBackdrop");
	_uiElementMgr->getBackdropLayer()->setScale(_currentTransparancyScale, _currentTransparancyScale);

	// _mDialogTextArea
	if (_dialogOn) _mDialogTextArea->show();

	// _miniMap
	_miniMap->show();

	if (_questOn) _questGoalTextWidget->show();

	// _playerHealthBarWidget
	_playerHealthBarWidget->show();

	// _enemyHealthBarWidget
	if (_enemyBarOn) _enemyHealthBarWidget->show();

	// _floorTextWidget
	_floorTextWidget->show();

	// upgrade text
	_upgradeTextWidget->show();

	// _healthUpgradeIcon
	_healthUpgradeIcon->show();
	// _dmgUpgradeIcon
	_dmgUpgradeIcon->show();

	// _hudTextWidget
	if (_hudTextOn) _hudTextWidget->show();
}
