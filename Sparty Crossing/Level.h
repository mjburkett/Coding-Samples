/**
 * \file Level.h
 *
 * \author Jason Israilov
 *
 * This manages the entire levels and what happens in them in the game.
 */


#pragma once

#include "Item.h"
#include "Game.h"
#include "XmlNode.h"
#include <vector>

class CGame;

/**
 * Manges the games level.
 */
class CLevel
{
public:

	CLevel(CGame* game, const std::wstring& filename);

	void XmlLoad(const std::wstring& filename);

	void Clear();

	void OnLevelLoad();

	void OnLevelReset();

	void XmlTile(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlRect(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlHero(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlCargo(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlRiver(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlBoat(const std::shared_ptr<xmlnode::CXmlNode>& boatNode, const std::shared_ptr<xmlnode::CXmlNode>& riverNode, 
		std::shared_ptr<CRiver> parent);
	void XmlSpace(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlSpaceShip(const std::shared_ptr<xmlnode::CXmlNode>& spaceShipNode, const std::shared_ptr<xmlnode::CXmlNode>& spaceNode, 
		std::shared_ptr<CSpace> parent);
	void XmlRoad(const std::shared_ptr<xmlnode::CXmlNode>& node);
	void XmlCar(const std::shared_ptr<xmlnode::CXmlNode>& carNode, const std::shared_ptr<xmlnode::CXmlNode>& roadNode, 
		std::shared_ptr<CRoad> parent);

	/**
	 * Getter for hero
	 * \param idx Index of hero to grab
	 * \returns Hero of the level
	 */
	std::shared_ptr<CHero> GetHero(int idx = 0) { return mHero.at(idx); }

	/**
	 * Getter for heroes
	 * \returns Heroes of the level
	 */
	std::vector<std::shared_ptr<CHero>> GetHeroes() { return mHero; }

	/**
	 * Get this level's number
	 * \return Number of level
	 */
	int GetLevelNum() { return mLevelNum; }

private:
	/// Game that level is in
	CGame* mGame; 
	///The level the user is on
	int mLevelNum; 
	///The name of the current level
	int mLevelName; 

	/// Collection of all items in the level
	std::vector<std::shared_ptr<CItem>> mLevelItems;

	///Collection of types that are decor in the level's Xml file
	std::vector<std::shared_ptr<xmlnode::CXmlNode>> mDecorTypes;
	///Collection of types that are cars in the level's Xml file
	std::vector<std::shared_ptr<xmlnode::CXmlNode>> mCarTypes;
	///Collection of types that are boats in the level's Xml file
	std::vector<std::shared_ptr<xmlnode::CXmlNode>> mBoatTypes;
	///Collection of types that are space ships in the level's Xml file
	std::vector<std::shared_ptr<xmlnode::CXmlNode>> mSpaceShipTypes;

	/// Heroes for the level
	std::vector<std::shared_ptr<CHero>> mHero; 

	
};
