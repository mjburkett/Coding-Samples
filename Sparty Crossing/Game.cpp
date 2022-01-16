/**
 * \file Game.cpp
 *
 * \author Matthew Burkett
 */

#include "pch.h"
#include "framework.h"
#include "Game.h"
#include "Tile.h"
#include "Cargo.h"
#include "MovingObject.h"
#include "Rectangle.h"
#include "DrawingVisitor.h"
#include "Item.h"
#include "LivingCargo.h"
#include "CarGetter.h"
#include "Car.h"
#include "UITextVisitor.h"
#include <mmsystem.h>

using namespace std;
using namespace Gdiplus;

/// Names of files to be loaded
vector<wstring> FileNames = { L"levels/level0.xml", L"levels/level1.xml", L"levels/level2.xml", L"levels/level3.xml"};

/**
 * Constructor
*/
CGame::CGame()
{
    
}

/**
 * Destructor
*/
CGame::~CGame()
{
}

/**
 * For External Calls to Update, designed for adressing clipping.
 * \param elapsed Time since last update call
*/
void CGame::OnUpdateCall(double elapsed)
{
    //Anti-Clipping Measures (re-reuns updates if greater than MaxElapsed)
    while (elapsed > MaxElapsed)
    {
        this->Update(MaxElapsed);

        elapsed -= MaxElapsed;
    }

    if (elapsed > 0)
        this->Update(elapsed);

}

/**
 * Update All Game Objects, calling Update on all Objects,
 * then calls Visistor on all objects to gather data.
 * \param elapsed Time since last update
 */
void CGame::Update(double elapsed)
{
    // If the game is paused, it is checked if it should unpause.
    if (mPaused) {
        // The time that the game has been paused is increased by the time elasped in the update.
        mTimePaused += elapsed;
        // If the amount of time the game has been paused is greater than or equal to the duration, unpause.
        if (mTimePaused >= mPauseDuration) 
        {
            Pause(0); // This unpauses the game
        }
    }
    if (!mPaused && mLevelIsWon) {
        //Load the next level here.
        Win();
        //Set the level completion status back to false
        SetLevelCompletion(false);
    }
    if (!mPaused && mHeroDead) {
        //Reset the level
        ResetLevels();
        //Tell the game the hero is alive
        HeroIsDead(false);
    }
    //Update each item in the item list.
    for (shared_ptr<CItem> itm : mItems)
        itm->Update(elapsed);

    // Check if hero is hit by car
    if (!mRoadCheatActive && !mPaused)
    {
        CrashCheck();
    }
    //Update the hero (which is not in the item list).
    for (shared_ptr<CHero> hero : mHero)
        hero->Update(elapsed);
}

/**
 * Returns if Hero has crashed with a car.
 * \returns True if hero has collided with car.
 */
bool CGame::CrashCheck()
{
    CCarGetter visitor;
    Accept(&visitor);
    for (auto car : visitor.GetCars())
    {
        for (shared_ptr<CHero> hero : mHero)
        {
            if (hero->CheckCrash(car))
            {
                CUITextVisitor visitor;
                Accept(&visitor);

                for (auto text : visitor.GetUITexts())
                {
                    if (text->GetCarName().length() == 0)
                    {
                        text->SetCarName(car->GetName());
                    }
                }

                //Pause for 3 seconds after death
                Pause(3);
                //Tell the game that the hero is dead
                HeroIsDead(true);
                return true;
            }
        }

        
    }
    return false;
}


/**
 * Moves the hero in a specific direction, by calling the move in hero.
 * \param dir The direction 1=up, 2=right, 3=down, 4=left
 * \param idx The index of the hero to move. -1 means last hero.
 */
void CGame::MoveHero(int dir, int idx)
{
    std::shared_ptr<CHero> hero;
        
    if (idx == -1)
        hero = mHero.back();
    else
        hero = mHero[idx];

   

    if ((dir > 0) && (dir <= 4))
    {
        hero->Move(dir);
    }

    if (CheckEats() && !mPaused)
    {
        ResetLevels();
    }
}

/**
 * Find image of given filename
 * \param file Filename
 * \returns Pointer to a DisplayImage
 */
std::shared_ptr<CDisplayImage> CGame::FindImage(const std::wstring& file)
{
    for (std::shared_ptr<CDisplayImage> image : mImages)
    {
        if (image->GetFile() == file)
        {
            return image;
        }
    }
    return nullptr;
}

/**
 * Set the given item vector to the game's mItems
 * \param itemVec Vector of items to be set
 */
void CGame::ItemsAdd(std::vector<std::shared_ptr<CItem>> itemVec) {
    mItems = itemVec;
}

/** 
* Called once on booting the game, loads all relevant levels.
*/
void CGame::OnStartup()
{
    for (wstring file : FileNames)
    {
        shared_ptr<CLevel> level = make_shared<CLevel>(this, file);
        mLevels.push_back(level);
    }
}

/**
 * Called when a specified Level is being loaded into the Game
 * \param lvlNum index of level to be loaded
*/
void CGame::OnLevelLoad(int lvlNum) {
    ResetLevels();
    if (lvlNum >= 0 && (mLevels.size() > (size_t)lvlNum)) {
        mLevels.at(lvlNum)->OnLevelLoad();
        mHero = mLevels.at(lvlNum)->GetHeroes();
        std::shared_ptr<CUIText> timeItem = std::make_shared<CUIText>(this);
        timeItem->SetLevelString(lvlNum);
        std::shared_ptr<CItem> time = timeItem;
        
        mItems.push_back(time);
        mCurrentLevel = lvlNum;
    }

    // Calls to see if all cargo are at final position for win
    if (CheckWin())
        Win();

    if (mCurrentLevel == 3)
    {
        PlaySound(L"sounds/msu_fight_song.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else
    {
        PlaySound(0, 0, 0);
    }
    //Pause for 3 seconds after death
    Pause(3);
}

/** Test an x,y click location to see if it clicked
* on some item in the game.
* \param x X location
* \param y Y location
* \returns Pointer to item we clicked on or nullptr if none.
*/
std::shared_ptr<CItem> CGame::HitTest(int x, int y)
{
    // Convert to virual pixel coordiantes for click
    double oX = (x - mXOffset) / mScale;
    double oY = (y - mYOffset) / mScale;

    // Check if any of the items are clicked
    for (auto i = mItems.rbegin(); i != mItems.rend(); i++)
    {
        if ((*i)->HitTest(oX, oY))
        {
            return *i;
        }
    }

    return  nullptr;
}

/**
 * Draw the game area
 * \param graphics The GDI+ graphics context to draw on
 * \param width Width of the client window
 * \param height Height of the client window
 */
void CGame::OnDraw(Gdiplus::Graphics* graphics, int width, int height)
{
    // Fill the background with black
    SolidBrush brush(Color::Black);
    graphics->FillRectangle(&brush, 0, 0, width, height);

    //
    // Automatic Scaling
    //
    float scaleX = float(width) / float(Width);
    float scaleY = float(height) / float(Height);
    mScale = min(scaleX, scaleY);

    // Ensure it is centered horizontally
    mXOffset = (float)((width - Width * mScale) / 2);

    // Ensure it is centered vertically
    mYOffset = (float)((height - Height * mScale) / 2);

    graphics->TranslateTransform(mXOffset, mYOffset);
    graphics->ScaleTransform(mScale, mScale);

    // From here on you are drawing virtual pixels
 
    CDrawingVisitor visitor;
    Accept(&visitor);

    for (auto item : visitor.GetTiles())
    {
        item->Draw(graphics);
    }

    for (auto item : visitor.GetRects())
    {
        item->Draw(graphics);
    }

    for (auto item : visitor.GetMovObjs())
    {
        item->Draw(graphics);
    }

    for (shared_ptr<CHero> hero : mHero)
        hero->Draw(graphics);

    for (auto item : visitor.GetCargo())
    {
        item->Draw(graphics);
    }

    
    // drawing the right boundry
    SolidBrush brush2(Color::Black);
    graphics->FillRectangle(&brush2, 1024, 0, Width, Height); //drawing the right boundry
    graphics->FillRectangle(&brush2, 0 - width, 0, width, Height); //drawing the left boundry

    // draw text on screen
    for (auto item : visitor.GetUITexts()) 
    {
        item->DrawTimer(graphics);
        item->DrawLevel(graphics);
        item->DrawCargoes(graphics);
        item->DrawLevelBeginText(graphics);
        item->DrawLevelWinText(graphics);
        item->DrawLevelDeathText(graphics);
    }

}

/**
 * Accept visitors for all items in game
 * \param visitor Visitor to be accepted
 */
void CGame::Accept(CItemVisitor* visitor)
{
    for (auto item : mItems)
    {
        item->Accept(visitor);
    }
    for (shared_ptr<CHero> hero : mHero)
        hero->Accept(visitor);
}

/**
 * Pick up a cargo item and move it to Hero's location
 * \param item Cargo item to be picked up
 * \param visitor Visitor to the cargo item
 */
void CGame::PickUpItem(shared_ptr<CItem> item, CIsCargoVisitor* visitor)
{
    if (!IsPaused())
    {
        for (shared_ptr<CHero> hero : mHero)
        {
            // Pick Up cargo if hero is at beginning
            if (hero->GetY() == hero->GetStartY() && item->GetY() == hero->GetStartY() + CItem::GridSpacing)
            {
                if (hero->GetHeldItem() != nullptr)
                {
                    CIsCargoVisitor dropVisitor;
                    hero->GetHeldItem()->Accept(&dropVisitor);
                    dropVisitor.GetCargo()->SetParked(false);
                }
                hero->PickUpItem(item);
                visitor->GetCargo()->SetCarried(hero);
            }

            // Pick Up cargo if hero is at end
            else if (hero->GetY() == hero->GetFinalY() && item->GetY() == hero->GetFinalY() - CItem::GridSpacing)
            {
                if (hero->GetHeldItem() != nullptr)
                {
                    CIsCargoVisitor dropVisitor;
                    hero->GetHeldItem()->Accept(&dropVisitor);
                    dropVisitor.GetCargo()->SetParked(true);
                }
                hero->PickUpItem(item);
                visitor->GetCargo()->SetCarried(hero);
            }

        }

      
    }
}

/**
 * Set a cargo item that was picked up to its parked position
 * \param item Cargo item to be dropped
 * \param visitor Visitor to the cargo item
 */
void CGame::DropItem(shared_ptr<CItem> item, CIsCargoVisitor* visitor)
{
    if (!IsPaused())
    {
        // If in two player mode
        if (mHero.size() == 2)
        {
            GetHero(0);
            // If item is bottom hero's and bottom hero is at bottom
            if (item == mHero[0]->GetHeldItem() && mHero[0]->GetY() == mHero[0]->GetStartY())
            {
                mHero[0]->DropItem();
                visitor->GetCargo()->SetParked(false);
            }
            // If item is top hero's and top hero is at top
            else if (item == mHero[1]->GetHeldItem() && mHero[1]->GetY() == mHero[1]->GetFinalY())
            {
                mHero[1]->DropItem();
                visitor->GetCargo()->SetParked(true);
            }
        }
        // If in single player mode
        else
        {
            auto hero = GetHero();
            if (hero->GetY() == hero->GetStartY())
            {
                hero->DropItem();
                visitor->GetCargo()->SetParked(hero->GetY() == hero->GetFinalY() * CItem::GridSpacing);
            }
            else if (hero->GetY() == hero->GetFinalY())
            {
                hero->DropItem();
                visitor->GetCargo()->SetParked(hero->GetY() != hero->GetStartY() * CItem::GridSpacing);
            }
        }
    }
    
    // Calls to see if all cargo are at final position for win
    if (CheckWin())
    {
        //Pause the game for 3 seconds.
        Pause(3);
        //Tell the game that it has met its win condition.
        SetLevelCompletion(true);
    } 
}

/**
 * Swap the clicked item
 * \param item Item to be swapped
 */
void CGame::SwapItem(std::shared_ptr<CItem> item)
{
    if (!IsPaused())
    {
        if (mHero[0]->GetY() == (mHero[1]->GetY() + CItem::GridSpacing))
        {
            // If item held by first hero
            if (item == mHero[0]->GetHeldItem())
            {
                // Swap the items
                shared_ptr<CItem> swapItem = mHero[0]->SwapItem(item, mHero[1]);
                
                // Assign the player variable of the received cargo to
                // be the first hero
                if (swapItem != nullptr)
                {
                    CIsCargoVisitor visitor;
                    swapItem->Accept(&visitor);
                    visitor.GetCargo()->SetCarried(mHero[0]);
                }
            }
            // If item held by second hero
            else
            {
                // Swap the items
                shared_ptr<CItem> swapItem = mHero[1]->SwapItem(item, mHero[0]);

                // Assign the player variable of the received cargo to
                // be the first hero
                if (swapItem != nullptr)
                {
                    CIsCargoVisitor visitor;
                    swapItem->Accept(&visitor);
                    visitor.GetCargo()->SetCarried(mHero[1]);
                }
            }
        }
    }
}

/**
 * Checks to see if all cargo are at the final position.
 * \returns True if player has won
*/
bool CGame::CheckWin()
{
    CGetCargoVisitor visitor;
    Accept(&visitor);
    return visitor.GetAllAtFinal();
}

/**
 * Reset all levels in game
 */
void CGame::ResetLevels()
{
    CDrawingVisitor visitor;
    Accept(&visitor);
    for (auto item : visitor.GetUITexts())
    {
        item->OnReset();
    }
    for (auto lvl : mLevels)
    {
        lvl->OnLevelReset();
    }
    
    if (mCurrentLevel == 3)
    {
        PlaySound(L"sounds/msu_fight_song.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else
    {
        PlaySound(0, 0, 0);
    }
    Pause(3);
}

/**
 * Called upon death of hero
 * \param method Method of death (1=hitByCar, 2=drowned, 3=traveledOffScreen)
 * \param idx index of the hero to die
 */
void CGame::OnHeroDie(int method, int idx)
{
    CUITextVisitor visitor;
    Accept(&visitor);
    if (method == 1)
    {
        //setting the UIText
        for (auto text : visitor.GetUITexts())
        {
            text->SetHeroName(mHero[idx]->GetName());
        }
    }
    else if (method == 2)
    {
        // use visitor to mDrownTextActive = true
        
        //setting the UIText
        for (auto text : visitor.GetUITexts())
        {
            text->SetHeroName(mHero[idx]->GetName());
            text->SetDrownTextActive(true);
        }
    }
    else if (method == 3)
    {
        //setting the UIText
        for (auto text : visitor.GetUITexts())
        {
            text->SetHeroName(mHero[idx]->GetName());
            text->SetOffScreenTextActive(true);
        }
    }
    //Pause for 3 seconds after death
    Pause(3);
    //Tell the game that the hero is dead
    HeroIsDead(true);
}

/**
 * Check if any of the cargo eat each other
 * \returns True if any cargo eats another
 */
bool CGame::CheckEats()
{
    if (!IsPaused())
    {
        CGetCargoVisitor visitor;
        Accept(&visitor);
        for (auto livingCargo : visitor.GetLivingCargoes())
        {
            for (auto cargo : visitor.GetCargoes())
            {

                if (livingCargo->Eats(mHero[0], cargo) && livingCargo->Eats(mHero.back(), cargo))
                {
                    CUITextVisitor visitor;
                    Accept(&visitor);

                    for (auto text : visitor.GetUITexts())
                    {
                        if (text->GetLivingCargoName().length() == 0 && text->GetCargoToEat().length() == 0)
                        {
                            text->SetLivingCargoName(livingCargo->GetName());
                            text->SetCargoToEat(cargo->GetName());
                        }
                    }
                    Pause(3);
                    HeroIsDead(true);
                    return true;
                }

                
            }
        }
    }
    return false;
}

/**
 * Creates text and moves onto next level after pause
*/
void CGame::Win()
{
    //Display Win Stuff (UI Stuff)
    if (mCurrentLevel == FileNames.size() - 1)
        OnLevelLoad(mCurrentLevel);
    else
        OnLevelLoad(mCurrentLevel + 1);

}

/**
 * Pause inputs to the game and reset
 * \param time Time to pause the game
 */
void CGame::Pause(double time) {
    if (time > 0)
    {
        mPaused = true;
        mTimePaused = 0;
        mPauseDuration = time;
        for (shared_ptr<CHero> hero : mHero)
            hero->SetPause(true);
    }
    else
    {
        mPaused = false;
        mTimePaused = 0;
        mPauseDuration = 0;
        for (shared_ptr<CHero> hero : mHero)
            hero->SetPause(false);
    }
}