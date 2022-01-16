/**
 * \file Terrain.cpp
 *
 * \author Chris Cardimen
 *
 * A river, road, or any terrain that the hero can interact with.
 *
 */

#include "pch.h"
#include "Terrain.h"
#include "XmlNode.h"
#include"Item.h"
#include "Car.h"
#include "Game.h"
#include "River.h"
#include "Road.h"
#include<vector>
using namespace xmlnode;
using namespace Gdiplus;
using namespace std;

/**
 * Determines if hero is touching the terrain
 * \param hero Hero to be tested
 * \returns True if hero is on top of terrain
 */
bool CTerrain::IsTouching(CHero* hero)
{
	if (this->GetY() == (hero->GetY() - CItem::OffsetDown))
	{
		return true;
	}
	return false;
}

/**
 * Get width of terrain
 * \returns Width of terrain
 */
double CTerrain::GetWidth() 
{
	return mWidth;
}

/**
 * Get the speed of the terrain
 * \returns Speed of the terrain
 */
double CTerrain::GetSpeed() 
{
	return mSpeed;
}

/**
 * Set the width of the terrain
 * \param width Value of the width to be set
 */
void CTerrain::SetWidth(double width) 
{
	mWidth = width;
}

/**
 * Set the speed of the terrain
 * \param speed Value of the speed to be set
 */
void CTerrain::SetSpeed(double speed) 
{
	mSpeed = speed;
}

/**
 * Constructor in a game
 * \param game Game to be constructed in
 */
CTerrain::CTerrain(CGame* game)
	: CItem(game)
{
}
