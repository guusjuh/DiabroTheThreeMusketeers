#include "DialogManager.h"
#include "GameManager.h"

const std::string DialogManager::instructionStrings[15] = {
	"Did you know you can take a break from all of your adventures? \\nJust press 'P'!",  // pausing
	"We spheres sure hate squares, don't we? \\nI hate squares! \\nYou can kill them by clicking with your left mouse button when you're close to them.", // killing squares
	"You still didn't figure out how to quest?! \\nIt's easy and you get lots of hints! \\nYellow indicators show which sphere has a quest for you. \\nAnd the green indicator shows the your current goal!", // finding quest goals
	"Family is the most important thing in the world, don't you think? \\nYour sister is lost?! Go find her, mate!", // family is important, save it
	"Some quests are so stupid, don't you think so? \\nLuckily you can abandon them by pressing 'BACKSPACE'.", // abandon quests
	"Escaping from this cave? \\n'ESC' is the fasted way to escape for sure, but that might not be the kind of escape you need.", // quiting the game
	"The map is pretty hard to understand, but you will get it! \\nIn the top of your screen you see a sort of compas. \\nThe pink indicator is your sister and the green one is your active quest. \\nNot that hard, is it?", // minimap
	"I pressed 'SHIFT' a while ago, but I sprinted so fast I hit a wall... \\nIt's sure faster, but be carefull!", // sprinting
	"You are going to need a bit more health to survive in here.. \\nQuesting is a really good way to get that bit extra you might need! \\nWhen killing an enemy, you also gain a bit of health, but you will probably lose more than you gain.", // healing
	"Quests are the only way to become stronger. \\nYou should quest a lot!", // quests have rewards
	"You noticed the heart and sword in the upper right corner of your screen? \\nThose are your upgrades and indicate your health and damage.", // upgrades
	"You are not the only one becoming stronger! \\nSquares gain upgrades each floor. You better watch your back!", // enemy upgrades en sterker per floor
	"I want to build a wall, but those squares have to pay for it! \\nWill you vote for me?", // little joke
	"There are different types of square bastards... \\nTerrible, right? There are rangers and knights. \\nBoth have to be fought differently!"
	"No no no no... \\nThey are squares, we are spheres. That's completely different!"
};

const std::string DialogManager::cantStartQuestString = "I have a quest, but you cannot start it until you finished your current quest! \\n";
const std::string DialogManager::rewardString = " \\nAs a reward I will give you ";
const std::string DialogManager::howToCancelQuestString = " \\nIf you aren't up for this quest, you can cancel it by pressing 'BACKSPACE' twice!";

DialogManager::DialogManager() { }

DialogManager::~DialogManager() { }

std::string DialogManager::getDialogText(BaseNpc* thisNpc) {
	return instructionStrings[GameManager::getSingletonPtr()->getRandomInRange(0, sizeof(instructionStrings)/ sizeof(instructionStrings[0]))];
}

std::string DialogManager::getDialogText(Npc* thisNpc) {
	// the string to return
	std::string returnString = "";

	// catch the current action number of the quest.
	// if no quest is active, the nr will be -1 and we can skip some checks. 
	int currActionNr = -1;
	if(GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()) {
		currActionNr = GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getCurrentAction();
	}

	// if the npc can be in this quest at all
	if(currActionNr > -1) {
		// if relevant in the current action, it knows, so directly obtain the dialog
		if (thisNpc->_relevantForAction) {
			returnString += GameManager::getSingletonPtr()->getQuestManager()->obtainDialog(thisNpc);

			// if this action is the last, add the reward text
			if(currActionNr == GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getStrategy()->getActionSequence().size() - 1) {
				returnString += rewardString + (GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->_upgradeReward.modifier == Damage ? 
					"a damage upgrade" : "a health upgrade") + " with a value of " + Ogre::StringConverter::toString(GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->_upgradeReward.value) + ".";
			}
		} 
		// else, does this npc have a quest and are we in the first action?
		// note: there is not a start action so we can't check for being content in such an action
		else if (thisNpc == (Npc*)GameManager::getSingletonPtr()->getSingletonPtr()->getQuestManager()->getCurrentQuest()->getSourceNpc()
			&& currActionNr == 0) {
			returnString += GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->getStrategy()->getDialog();
		}
		// else, was this npc relevant in the last action?
		else if(GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->isContentInAction(thisNpc, currActionNr - 1)) {
			// if this npc does have a quest, it should mention it cannot be started yet.
			if (thisNpc->_hasQuest) {
				returnString += cantStartQuestString;
			}

			returnString += GameManager::getSingletonPtr()->getQuestManager()->obtainDialog(thisNpc, currActionNr - 1);
		}
	}
	// else, does the npc has a quest + can it be started?
	else if (thisNpc->_hasQuest && GameManager::getSingletonPtr()->getQuestManager()->questCanStart()) {
		// get the dialog for starting the quest
		returnString += GameManager::getSingletonPtr()->getQuestManager()->startQuest(thisNpc);
		returnString += howToCancelQuestString;
	} 

	// it should mention it cannot be started yet.
	if(returnString == "" && thisNpc->_hasQuest && !GameManager::getSingletonPtr()->getQuestManager()->questCanStart()) {
		returnString += cantStartQuestString;
	}

	// if we still have an empty string at this point, none of the conditions was met or something went wrong
	if(returnString == "" || returnString == cantStartQuestString) {
		returnString += instructionStrings[GameManager::getSingletonPtr()->getRandomInRange(0, sizeof(instructionStrings) / sizeof(instructionStrings[0]))];
	}

	return returnString;
}

std::string DialogManager::getDialogText(BaseEnemy* thisNpc) {
	return instructionStrings[GameManager::getSingletonPtr()->getRandomInRange(0, sizeof(instructionStrings) / sizeof(instructionStrings[0]))];
}