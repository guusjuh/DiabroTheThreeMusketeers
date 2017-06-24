#ifndef SISTER_H_
#define SISTER_H_

#include "BaseNpc.h"
#include <fstream>
#include "NpcWanderState.h"
#include "NpcFollowPlayerState.h"
#include "NpcIdleState.h"
#include "NpcKidnappedState.h"


/// <summary>
/// The Sister NPC class. 
/// </summary>
/// <seealso cref="BaseNpc" />
class Sister : public BaseNpc
{
public:
	Sister(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*);
	~Sister();

	void update(Ogre::Real) override;
	void collide() override;

	City* getHomeTown() { return _hometown; }

private:
	bool _initialized;

	City* _hometown;
};

#endif