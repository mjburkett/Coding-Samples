/**
 * \file Terrain.h
 *
 * \author Chris Cardimen
 *
 * A river, road, or any terrain that the hero can interact with.
 *
 */
#pragma once
#include<vector>
#include<string>
#include"Hero.h"
#include "pch.h"
#include "Terrain.h"
#include "XmlNode.h"
#include"Item.h"
#include "Game.h"
#include"ItemVisitor.h"

using namespace xmlnode;
using namespace Gdiplus;
using namespace std;

/// Manages rivers, roads, or any terrain that the hero can interact with.
class CTerrain :
	public CItem
{
private:
	bool mIsDeadly = false; ///< boolean to whether the terrain is deadly or not
	double mVehicleSpeed; ///< determines the vehicle speed
	double mWidth; ///< width
	double mSpeed; ///< speed
public:

	CTerrain(CGame* game);

	bool IsTouching(CHero* hero);

	double GetWidth();

	double GetSpeed();

	void SetWidth(double width);

	void SetSpeed(double speed);

	/**
	 * Accept a visitor
	 * \param visitor The visitor being accepted
	 **/
	virtual void Accept(CItemVisitor* visitor) override { visitor->VisitTerrain(this); }

	/**
	 * Return if the terrain is deadly or not
	 * \return Whether terrain is deadly
	 **/
	bool GetIsDeadly() { return mIsDeadly; }

	/**
	 * Sets deadly flag to given value
	 * \param val Value to set mIsDeadly to 
	 */
	void SetIsDeadly(bool val) { mIsDeadly = val; }

};

