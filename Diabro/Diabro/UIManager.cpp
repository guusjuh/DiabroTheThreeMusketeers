#include "UIManager.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="UIManager"/> class.
/// This class is created by the <see cref="GameManager" /> and contains all UI information, 
/// e.g. the in-game and menu UI.
/// </summary>
UIManager::UIManager() 
: _uiNode(0), _healthBarWidget(0), _maxWidthBar(0), _heightBar(0), 
_mWindow(0), _hudTextWidget(0), _hudTotalTimer(3), _hudTextOn(false), _uiElementMgr(0), _mDialogTextArea(0)
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

void UIManager::update(const Ogre::FrameEvent& pFE) 
{
	if(_hudTextOn) {
		_hudTimer -= pFE.timeSinceLastFrame;

		if(_hudTimer <= 0) {
			hideHUDText();
		}
	}
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
	_hudTextOn = true;

	_hudTimer = _hudTotalTimer;
}

void UIManager::hideHUDText()
{
	_hudTextOn = false;

	_uiElementMgr->destroyWidget("HUDText");
	_hudTextWidget = nullptr;

	_hudTimer = 0;
}

void UIManager::showDialog(Ogre::String pNPCName, Ogre::String pDialogText) {
	if (_hudTextWidget != nullptr) hideHUDText();

	_mDialogTextArea = _uiElementMgr->createDialogTextBox(DiabroUI::CENTER, "DialogTextArea", pNPCName, 400, 400);
	_mDialogTextArea->setText(pDialogText);
}

/// <summary>
/// Destroys the dialog.
/// </summary>
void UIManager::destroyDialog() {
	_uiElementMgr->destroyWidget("DialogTextArea");
	_mDialogTextArea = nullptr;
}

/// <summary>
/// Appends the dialog text.
/// </summary>
/// <param name="pDialogText">The p dialog text.</param>
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