#include "Npc.h"
#include "GameManager.h"

const std::string Npc::_nameOptions[26] = { "Gleann", "Wolter", "Jesper", "Gus", "Fredo", "Kelvin", "Mike", "Resa",
										  "Charry", "Jule", "Darwin", "Alexia", "Anderson", "Dora", "Jordan", "Deric",
										  "Richael", "Sassia", "Greg", "Bonney", "Marlin", "Marlene", "Gustik",
										  "Mufo", "Jerico", "Marloes"};

std::vector<std::string> Npc::_usedNameOptions;

/// <summary>
/// Creates a new instance of the <see cref="Npc"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Npc::Npc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, Building* pBuilding) 
: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity), _hometown(pMyCity), _home(pBuilding), _initialized(false)
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
	indicatorNode->setScale(0.3f, 0.3f, 0.3f);
	indicatorNode->setPosition(0.0f, 150.0f, 0.0f);

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

	_name = getRandomName();
	_needs = NeedSet(tempNeeds);

	_hasQuest = false;
	_relevantForAction = false;

	_movespeed = 100;
	_rotationspeed = 180.0f;
	_noticeDistance = 400;
	_radius = 25.0f;

	goalPos = Coordinate(pMyNode->getPosition().x, pMyNode->getPosition().z);
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
std::string Npc::getRandomName() {
	std::vector<std::string> _currOptions;
	std::string _returnName;

	// find all names that aren't used yet 
	bool occured = false;
	int lengthNameOptions = sizeof(_nameOptions) / sizeof(_nameOptions[0]);

	for (int i = 0; i < lengthNameOptions; ++i) {
		occured = false;

		for (int j = 0; j < _usedNameOptions.size(); ++j) {
			if(_nameOptions[i] == _usedNameOptions[j]) {
				occured = true;
			}
		}

		if (!occured) _currOptions.push_back(_nameOptions[i]);
	}

	// if there are no available names anymore, start reusing
	if(_currOptions.size() == 0) {
		_usedNameOptions.clear();
		
		// call this method again 
		// now that the vector is empty, names will be reused
		_returnName = getRandomName();
	} 
	else {
		_returnName = _currOptions[GameManager::getSingletonPtr()->getRandomInRange(0, _currOptions.size())];
	}

	_usedNameOptions.push_back(_returnName);

	return _returnName;
}

/// <summary>
/// Updates the frame based on the specified deltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void Npc::update(Ogre::Real pDeltatime)
{

	// if we don't have a quest yet and we arn't initialized
	// this is contained in the update due to order problems when done in constructor
	if (!_hasQuest && !_initialized) {
		// generate a random number
		int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, 3);
		if(randomroll < 2) {
			needNewQuest();
			_hasQuest = true;
		}
		_initialized = true;
	}

	if(_playerDetected)
	{
		_dirVec = Ogre::Vector3::ZERO;
	} else {
		if(_dirVec == Ogre::Vector3::ZERO) _dirVec = Ogre::Vector3(1, 0, 0);

		if (getPosition().distance(Ogre::Vector3(getGoalPos().x, getPosition().y, getGoalPos().z)) < 50) {
			if (getNextPosSize() == 0) {
				_randomTownPos = _myCity->getNpcPosition();
				Ogre::Vector3 pos = Ogre::Vector3(_randomTownPos.rx, getPosition().y, _randomTownPos.rz);
				Ogre::Vector3 goalPosition = Ogre::Vector3(getGoalPos().x / getCity()->Scalar(), getPosition().y, getGoalPos().z / getCity()->Scalar());
				while (goalPosition.distance(pos) < 1) {
					_randomTownPos = _myCity->getNpcPosition();
					pos = Ogre::Vector3(_randomTownPos.rx, getPosition().y, _randomTownPos.rz);
				}
				pos.x = pos.x / Zone::scalar * City::gridScalar;
				pos.z = pos.z / Zone::scalar * City::gridScalar;

				calculateAStar(pos);
			}
			else {
				walkToNextPoint();
			}
		}
	}

	BaseNpc::update(pDeltatime);
}

/// <summary>
/// Dies this instance.
/// </summary>
void Npc::die() {
	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->detachFriendlyNPC(id);
}

void Npc::collide() {
	walkToNeighbour();
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

	questIndicatorEntity->setMaterialName(_questMaterial);
	indicatorNode->setVisible(true);

	lowestNeed.adjustValue(50);
}

/// <param name="pPlayerPos">The current player position.</param>
/// <returns>False if the player is too far away to start a talk</returns>
bool Npc::talk()
{
	if (!_inDialog) {
		// this npc is currently in dialog
		_inDialog = true;

		// if this quest can be started
		if (_hasQuest && GameManager::getSingletonPtr()->getQuestManager()->questCanStart()) {
			//TODO: this variable should be set when the quest is over and there is actually not a quest anymore
			indicatorNode->setVisible(false);
		}

		setDialog(GameManager::getSingletonPtr()->getDialogManager()->getDialogText(this));

		// show the first line of the dialog
		GameManager::getSingletonPtr()->getUIManager()->showDialog(_name, _dialog[_dialogCount]);
	}
	else {
		BaseNpc::talk();
	}

	return _inDialog;
}


