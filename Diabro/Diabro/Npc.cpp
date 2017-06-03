#include "Npc.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="Npc"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Npc::Npc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, Building* pBuilding) 
: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity), _inDialog(false), _hometown(pMyCity), _home(pBuilding), _initialized(false)
{
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeFriendlyNPC(this);
	rotatePivot(Ogre::Vector3(0, 90, 0));
	
	_dialog.push_back("Hello! Nice to meet you, sir! I'm telling you a long story. This way I can test whether or not the text goes to the next line.. that would be nice, wouldn't it?");
	_dialog.push_back("This is the first sentence of the talk.");
	_dialog.push_back("And this is the second sentence.");
	_dialogCount = 0;

	// randomly assign a profession based on the bui;ding type of its home
	_profession = (Profession)_home->type;

	pMyNode->setScale(0.25f, 0.25f, 0.25f);
	pMyNode->setPosition(pMyNode->getPosition().x, 18.0f, pMyNode->getPosition().z);

	switch (_home->type) // assign building random professions by giving them a rolenode
	{
	case Smithery:
		_originalMaterialName = "InGame/YellowNPC";
		break;
	case GuardHouse:
		_originalMaterialName = "InGame/PinkNPC";
		break;
	case Church:
		_originalMaterialName = "InGame/GreenNPC";
		break;
	case PotionStore:
		_originalMaterialName = "InGame/PurpleNPC";
		break;
	case Shop:
		_originalMaterialName = "InGame/BlueNPC";
		break;
	case FancyHouse:
		_originalMaterialName = "InGame/RedNPC";
		break;
	default:
		break;
	}
	pMyEntity->setMaterialName(_originalMaterialName);

	// randomly assign needs
	Need tempNeed;
	std::vector<Need> tempNeeds;
	for (int i = 0; i < NeedType::AMOUNT_OF_NEEDTYPES; ++i) {
		tempNeed.type = (NeedType)i;

		int randomRoll = GameManager::getSingletonPtr()->getRandomInRange(10, 100);
		tempNeed.value = randomRoll;
		tempNeeds.push_back(tempNeed);
	};

	_name = getNameOptions()[GameManager::getSingletonPtr()->getRandomInRange(0, getNameOptions().size())];
	_needs = NeedSet(tempNeeds);

	_hasQuest = false;
	_relevantForAction = false;

	_movespeed = 100;
	_rotationspeed = 0.13f;
	_noticeDistance = 400;
}

/// <summary>
/// Finalizes an instance of the <see cref=""/> class.
/// </summary>
Npc::~Npc() {
}

/// <summary>
/// Gets the name options.
/// </summary>
/// <returns></returns>
std::vector<std::string> Npc::getNameOptions() {
	std::vector<std::string> _nameOptions;
	_nameOptions.push_back("Gleann");
	_nameOptions.push_back("Wolter");
	_nameOptions.push_back("Jesper");
	_nameOptions.push_back("Gus");
	_nameOptions.push_back("Fredo");
	_nameOptions.push_back("Kelvin");
	_nameOptions.push_back("Mike");
	_nameOptions.push_back("Resa");

	return _nameOptions;
}

/// <summary>
/// Updates the frame based on the specified deltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void Npc::update(Ogre::Real pDeltatime)
{
	BaseNpc::update(pDeltatime);

	// if we don't have a quest yet and we arn't initialized
	// this is contained in the update due to order problems when done in constructor
	if (!_hasQuest && !_initialized) {
		// generate a random number
		int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, 3);
		Debug("Random roll was ", randomroll);
		if(randomroll < 1) {
			Debug("I got a quest! ", randomroll);
			needNewQuest();
			_hasQuest = true;
		}
		_initialized = true;
	}

	if(_playerDetected)
	{
		_dirVec = Ogre::Vector3::ZERO;
	} 
}

/// <summary>
/// Dies this instance.
/// </summary>
void Npc::die() {
	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->detachFriendlyNPC(id);
}

/// <param name="pPlayerPos">The current player position.</param>
/// <returns>False if the player is too far away to start a talk</returns>
bool Npc::talk(Ogre::Vector3 pPlayerPos)
{
	// start the dialog if it wasn't started already
	if (!_inDialog) {
		// this npc is currently in dialog
		_inDialog = true;

		// if this NPC has a quest 
		if (_relevantForAction) {
			setDialog(GameManager::getSingletonPtr()->getQuestManager()->obtainDialog(this));
		}
		else if (_hasQuest && GameManager::getSingletonPtr()->getQuestManager()->questCanStart()) {
			// if this quest can be started
				// get the dialog for starting the quest
			setDialog(GameManager::getSingletonPtr()->getQuestManager()->startQuest(this));
			_hasQuest = false;
		}
		// else if // do i have text in the current quest?
		else {
			_dialog = getStndrtDialog();
		}

		// show the first line of the dialog
		GameManager::getSingletonPtr()->getUIManager()->showDialog(_name, _dialog[_dialogCount]);
	}
	else {
		_dialogCount++;
		if (_dialogCount < _dialog.size()) {
			GameManager::getSingletonPtr()->getUIManager()->appendDialogText(_dialog[_dialogCount]);
		}
		else {
			if (_hasItem && _needToGiveItem) {
				giveItem(GameManager::getSingletonPtr()->getLevelManager()->getPlayer());
			}
			else if (_relevantForAction) {
				GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(Action::msgPlayerInfo);
			}

			GameManager::getSingletonPtr()->getUIManager()->hideDialog();
			_dialogCount = 0;
			_inDialog = false;
		}
	}

	return _inDialog;
}

/// <summary>
/// Adjusts the given need.
/// </summary>
/// <param name="pNeedType">Type of the need.</param>
/// <param name="pAdjust">The adjustment value.</param>
void Npc::adjustNeed(NeedType pNeedType, int pAdjust) {
	_needs.adjustValueOf(pAdjust, pNeedType);
	return;
}

/// <summary>
/// Assigns new quest based on current needs.
/// </summary>
void Npc::needNewQuest() {
	// set the lowest need by default to the first
	Need lowestNeed = _needs.getNeeds()[0];

	// find the lowest need
	for(int i = 1; i < _needs.getNeeds().size(); ++i) {
		if(_needs.getNeeds()[i].value < lowestNeed.value) {
			lowestNeed = _needs.getNeeds()[i];
		}
	}

	// set the current quest to a new random quest 
	_currentQuest = GameManager::getSingletonPtr()->getQuestManager()->generateQuest(this, lowestNeed.type);

	lowestNeed.adjustValue(50);
}


void Npc::recieveItem() {
	Character::recieveItem();

	GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(Action::msgNpcItem);
}

