#include "Npc.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="Npc"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Npc::Npc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, Building* pBuilding) 
: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity), _inDialog(false), _hometown(pMyCity), _home(pBuilding)
{
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeFriendlyNPC(this);
	rotatePivot(Ogre::Vector3(0, 90, 0));
	_dialogFile.open("DialogText.txt");
	if (_dialogFile.fail()) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("Error opening text file, file maybe corrupt or unreachable");
		fclose(fp);
#endif
	}
	else {
		std::string line;
		for (int i = 1; !_dialogFile.eof(); i++)
		{
			getline(_dialogFile, line);

			if (i == 1) {
				_startDialogText = line;
			}
			else if (i == 2) {
				_endDialogText = line;
			}

		}
		_dialogFile.close();
	}
	
	_dialog.push_back("Hello! Nice to meet you, sir! I'm telling you a long story. This way I can test whether or not the text goes to the next line.. that would be nice, wouldn't it?");
	_dialog.push_back("This is the first sentence of the talk.");
	_dialog.push_back("And this is the second sentence.");
	_dialogCount = 0;

	// randomly assign a profession based on the bui;ding type of its home
	_profession = (Profession)_home->type;

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
	_needs = new NeedSet(tempNeeds);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	std::cout << "Hi, my name is " << _name << " and I am a " << _profession << std::endl;
	fclose(fp);
#endif
}

Npc::~Npc() {
	delete _needs;
}

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

	if(_playerDetected)
	{
		_dirVec = Ogre::Vector3::ZERO;
	} 
}


/// <summary>
/// Starts dialogs based on the distance between this instance and the specified player position.
/// </summary>
/// <param name="pPlayerPos">The current player position.</param>
/// <returns>False if the player is too far away to start a dialog</returns>
bool Npc::dialog(Ogre::Vector3 pPlayerPos)
{
	Ogre::Real distance = _myNode->getPosition().distance(pPlayerPos);
	
	if (distance < 1000000000000) // needs to be tweaked
	{
		_inDialog = true;

		GameManager::getSingletonPtr()->getUIManager()->showDialog(_name, "Hello! Nice to meet you, sir! I'm telling you a long story. This way I can test whether or not the text goes to the next line.. that would be nice, wouldn't it?");

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("dialog on\n");
		fclose(fp);
#endif
		
		return true;
	} else
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("out of range for dialog\n");
		fclose(fp);
#endif
		
		return false;
	}
}

/// <summary>
/// Dies this instance.
/// </summary>
void Npc::die() {
	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->detachFriendlyNPC(id);
}

/// <summary>
/// Starts dialogs based on the distance between this instance and the specified player position.
/// </summary>
void Npc::toggleDialog() {
	_inDialog = false;
	try {
		GameManager::getSingletonPtr()->getUIManager()->destroyDialog();
	}

	catch (...) {
		return;
	};
}

/// <param name="pPlayerPos">The current player position.</param>
/// <returns>False if the player is too far away to start a talk</returns>
bool Npc::talk(Ogre::Vector3 pPlayerPos)
{
	if (!_inDialog) {
		_inDialog = true;
		GameManager::getSingletonPtr()->getUIManager()->showDialog(_name, _dialog[_dialogCount]);
	}
	else {
		_dialogCount++;
		if (_dialogCount < _dialog.size()) {
			GameManager::getSingletonPtr()->getUIManager()->appendDialogText(_dialog[_dialogCount]);
		}
		else {
			GameManager::getSingletonPtr()->getUIManager()->destroyDialog();
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
	_needs->adjustValueOf(pAdjust, pNeedType);
	return;
}
