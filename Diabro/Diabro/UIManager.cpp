#include "UIManager.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="UIManager"/> class.
/// This class is created by the <see cref="GameManager" /> and contains all UI information, 
/// e.g. the in-game and menu UI.
/// </summary>
UIManager::UIManager() 
: _uiNode(0), _healthBarWidget(0), _maxWidthBar(0), _heightBar(0), 
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

	_maxWidthBar = _healthBarWidget->getOverlayElement()->getWidth();
	_heightBar = _healthBarWidget->getOverlayElement()->getHeight();

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

	// create health bar
	_healthBarWidget = _uiElementMgr->createHealthBar(DiabroUI::BOTTOMLEFT, "Health", 256, 256, 0, GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth(), 1);

	_hudTextWidget = nullptr;
}

void UIManager::startUpdate(const Ogre::FrameEvent& pFE)
{
	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if(!showStartText()) {
				GameManager::getSingletonPtr()->goNextState();
			}
		}
	}
	else {
		showStartText();
	}
}

void UIManager::inGameUpdate(const Ogre::FrameEvent& pFE)
{
	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
		}
	}
}

void UIManager::endUpdate(const Ogre::FrameEvent& pFE)
{
	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showEndText()) {
				GameManager::getSingletonPtr()->goNextState();
			}
		}
	}
	else {
		showEndText();
	}
}

void UIManager::diedUpdate(const Ogre::FrameEvent& pFE)
{
	if (_hudTextWithTimeOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if (_hudTimer <= 0) {
			hideHUDText();
			if (!showDiedText()) {
				GameManager::getSingletonPtr()->goToState(GameState::InGame);
			}
		}
	}
	else {
		showDiedText();
	}
}

bool UIManager::showStartText() {
	static int _textCount = 0;
	if (!_storyTextOn) {
		_storyTextOn = true;
		GameManager::getSingletonPtr()->getUIManager()->showHUDText((_first ? _startGameText[_textCount] : _startLevelText[_textCount]), 2.5f);
	}
	else {
		_textCount++;
		if (_textCount < (_first ? _startGameText.size() : _startLevelText.size())) {
			GameManager::getSingletonPtr()->getUIManager()->showHUDText(_first ? _startGameText[_textCount] : _startLevelText[_textCount], 2.5f);
		}
		else {
			GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
			_textCount = 0;
			_storyTextOn = false;
			_first = false;
		}
	}

	return _storyTextOn;
}

bool UIManager::showEndText() {
	static int _textCount = 0;
	if (!_storyTextOn) {
		_storyTextOn = true;
		GameManager::getSingletonPtr()->getUIManager()->showHUDText(_endLevelText[_textCount], 2.5f);
	}
	else {
		_textCount++;
		if (_textCount < _endLevelText.size()) {
			GameManager::getSingletonPtr()->getUIManager()->showHUDText(_endLevelText[_textCount], 2.5f);
		}
		else {
			GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
			_textCount = 0;
			_storyTextOn = false;
		}
	}

	return _storyTextOn;
}

bool UIManager::showDiedText() {
	static int _textCount = 0;
	if (!_storyTextOn) {
		_storyTextOn = true;
		GameManager::getSingletonPtr()->getUIManager()->showHUDText(_diedText[_textCount], 2.5f);
	}
	else {
		_textCount++;
		if (_textCount < _diedText.size()) {
			GameManager::getSingletonPtr()->getUIManager()->showHUDText(_diedText[_textCount], 2.5f);
		}
		else {
			GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
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

	if(_hudTextWidget != nullptr) 
		hideHUDText();
	if (_mDialogTextArea != nullptr)
		destroyDialog();

	_hudTextWidget = _uiElementMgr->createHUDText(DiabroUI::CENTER, "HUDText", pHUDText, width, 40);
	_hudTextWidget->getOverlayElement()->setTop(-128);
}

void UIManager::showHUDText(Ogre::String pHUDText, float time)
{
	// count the symbols in text
	int count = pHUDText.length();
	float width = count * 14.25f;

	if (_hudTextWidget != nullptr)
		hideHUDText();
	if (_mDialogTextArea != nullptr)
		destroyDialog();

	_hudTextWidget = _uiElementMgr->createHUDText(DiabroUI::CENTER, "HUDText", pHUDText, width, 40);
	_hudTextWidget->getOverlayElement()->setTop(-128);
	_hudTextWithTimeOn = true;

	_hudTimer = time;
}

void UIManager::hideHUDText()
{
	_hudTextWithTimeOn = false;

	if(_hudTextWidget != nullptr) {
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

/// <summary>
/// Destroys the talk.
/// </summary>
void UIManager::destroyDialog() {
	_uiElementMgr->destroyWidget("DialogTextArea");
	_mDialogTextArea = nullptr;
}

/// <summary>
/// Appends the talk text.
/// </summary>
/// <param name="pDialogText">The p talk text.</param>
void UIManager::appendDialogText(Ogre::String pDialogText) {
	_mDialogTextArea->appendText("\n" + pDialogText);
}

/// <summary>
/// Adjusts the health bar pValue.
/// </summary>
/// <param name="pValue">The pValue.</param>
/// <param name="pMaxValue">The maximum pValue.</param>
void UIManager::adjustHealthBar(Ogre::Real pValue, Ogre::Real pMaxValue)
{
	_healthBarWidget->setValue(pValue, calcBarSize(pValue, pMaxValue, _maxWidthBar));
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