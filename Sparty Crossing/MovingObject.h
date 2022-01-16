/**
*	\file MovingObject.h
*
*
*	\author Tyler Eiseman
*
*  Class for a boat or a car, an object that is moving in the program
**/
#pragma once
#include "Game.h"
#include "Item.h"
#include "ItemVisitor.h"
#include "XmlNode.h"

/**
 * Implement Moving Object class
 */
class CMovingObject:
	public CItem
{
public:
	///Constructor
	CMovingObject(CGame* game);

	///Default constructor (disabled)
	CMovingObject() = delete;

	/// Copy constructor (disabled)
	CMovingObject(const CMovingObject&) = delete;

	virtual void Update(double elapsed);

	/**Accept a visitor
	 *\param visitor The visitor accepted by this object*/
	virtual void Accept(CItemVisitor* visitor) override { visitor->VisitMovingObject(this); }

	/**
	 * Set the time to swap image
	 * \param sTime Time until swap images 
	 */
	void SetSwapTime(double sTime) { mSwapTime = sTime; }

	/**
	 * Set the speed of the moving object
	 * \param speed Speed of the moving object
	 */
	void SetSpeed(double speed) { mSpeed = speed; }

	void SetBoundingDistance(int width);

	/**
	 * Get the swap time for the moving object
	 * \returns Swap time of moving object
	 */
	double GetSwapTime() { return mSwapTime; }

	void LoopPosition(bool movingRight);
	
	/**
	 * Get the starting X position of moving object
	 * \returns Starting X Position
	 */
	double GetStartX() { return mStartX; }

	virtual void XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node);

private:
	///swap time for how it reflects
	double mSwapTime = 1;
	/// X position of start position for the moving object
	double mStartX = 0;
	///Speed at which the object is moving
	double mSpeed = 0; 
	///Distance at which the moving object resets
	double mBoundingDist = 0;
	///Constant of Width grabbed from game
	int mGameWidth;
};
