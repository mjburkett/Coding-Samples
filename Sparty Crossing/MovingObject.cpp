/**
*	\file MovingObject.cpp
*
*	\author Tyler Eiseman
**/

#include "pch.h"
#include "MovingObject.h"

using namespace std;
using namespace Gdiplus;


/**  Constructor
 * \param game The game this item is a member of
 */
CMovingObject::CMovingObject(CGame* game) : CItem(game)
{
	mGameWidth = game->Width;

}

/**
* Updates the location of the moving object
* \param elapsed Time since last update
**/
void CMovingObject::Update(double elapsed)
{
	if (GetX() > (mGameWidth + mBoundingDist))
		LoopPosition(true);
	else if (GetX() < (-mBoundingDist))
		LoopPosition(false);
	
	SetLocation(GetX() + mSpeed * CItem::GridSpacing * elapsed, GetY());
}



/**
 * Sets how far cargo should move before looping
 * \param width Distance to move given by xml file
 */
void CMovingObject::SetBoundingDistance(int width)
{
	mBoundingDist = (width * CItem::GridSpacing - mGameWidth /2 ) / 2;
}

/**
 * Sets moving object to its loop position
 * \param movingRight Flag for whether object is moving to the right
 */
void CMovingObject::LoopPosition(bool movingRight)
{
	if (movingRight)
		SetLocation(0 - mBoundingDist, GetY());
	else
		SetLocation(mGameWidth + mBoundingDist, GetY());
}

/**
* Load the attributes for a moving object node.
* \param node The Xml node we are loading the item from
*/
void CMovingObject::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node)
{
	//One (y) is an arbitrary number which is changed immediately after creation.
	SetCoords(node->GetAttributeDoubleValue(L"x", 0), 1);
}
