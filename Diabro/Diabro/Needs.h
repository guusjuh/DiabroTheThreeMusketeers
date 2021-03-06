#ifndef NEEDS_H
#define NEEDS_H

#pragma once
#include <OgrePrerequisites.h>

/// <summary>
/// Enum for the types of needs. 
/// </summary>
enum NeedType {
	KnowledgeNeed = 0, 
	//ComfortNeed,
	ReputationNeed,
	//SerenityNeed,
	//ProtectionNeed,
	//ConquestNeed,
	//WealthNeed,
	//EquipmentNeed,

	AMOUNT_OF_NEEDTYPES
};

/// <summary>
/// A strucuture containing all data relevant to a need. 
/// </summary>
struct Need {
	Need() : type(NeedType::KnowledgeNeed), value(0) {};
	Need(NeedType pType, Ogre::Real pValue) : type(pType), value(pValue) {};

	NeedType type;
	Ogre::Real value;

	void adjustValue(int pAdjustment) { value += pAdjustment; };
};

/// <summary>
/// A data class containing a set of needs. 
/// </summary>
class NeedSet {
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Needs"/> class.
	/// </summary>
	NeedSet() {
		// declare a need instance
		Need tempNeed;

		// for each need type, set the values of the need instance and push it to my list with needs
		for (int i = 0; i < NeedType::AMOUNT_OF_NEEDTYPES; ++i) {
			tempNeed.type = (NeedType)i;
			tempNeed.value = 100;
			_needs.push_back(tempNeed);
		};
	};

	NeedSet(std::vector<Need> pNeeds) : _needs(pNeeds) {};

	std::vector<Need> getNeeds() { return _needs; }

	int getKnowledgeValue() { return _needs[(int)NeedType::KnowledgeNeed].value; };
	//int getComfortValue() { return _needs[(int)NeedType::ComfortNeed].value; };
	//int getReputationValue() { return _needs[(int)NeedType::ReputationNeed].value; };
	//int getSerenityValue() { return _needs[(int)NeedType::SerenityNeed].value; };
	//int getProtectionValue() { return _needs[(int)NeedType::ProtectionNeed].value; };
	//int getConquestValue() { return _needs[(int)NeedType::ConquestNeed].value; };
	//int getWealthValue() { return _needs[(int)NeedType::WealthNeed].value; };
	//int getEquipmentValue() { return _needs[(int)NeedType::EquipmentNeed].value; };

	/// <summary>
	/// Adjusts the value of the given need.
	/// </summary>
	/// <param name="pAdjustment">The adjustment value.</param>
	/// <param name="pNeed">The need type.</param>
	void adjustValueOf(int pAdjustment, NeedType pNeed) { _needs[(int)pNeed].adjustValue(pAdjustment); };

private:
	std::vector<Need> _needs;
};

#endif
