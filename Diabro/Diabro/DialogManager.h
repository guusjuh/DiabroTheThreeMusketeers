#ifndef DIALOG_MANAGER_H
#define DIALOG_MANAGER_H
#include <string>
#include <vector>
#include "BaseNpc.h"
#include "BasicEnemy.h"

class Npc;

class DialogManager {
public:
	DialogManager();
	~DialogManager();

	std::string getDialogText(BaseNpc* thisBaseNpc);
	std::string getDialogText(Npc* thisNpc);
	std::string getDialogText(BasicEnemy* thisNpc);

private:
	static const std::string instructionStrings[6];
};

#endif