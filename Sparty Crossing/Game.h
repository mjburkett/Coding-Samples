/**
 * \file Game.h
 *
 * \author Matthew Burkett
 *
 * Main Class for the game.
 */
#pragma once

#include <memory>
#include <vector>
#include <set>
#include "Item.h"
#include "Level.h"
#include "Hero.h"
#include "UIText.h"
#include "IsCargoVisitor.h"
#include "GetCargoVisitor.h"

class CLevel;

/**
 * Controller for game functionality
 */
class CGame
{
public:
    CGame();

    virtual ~CGame();

    /// Game area in virtual pixels
    const static int Width = 1224;

    /// Game area height in virtual pixels
    const static int Height = 1024;

    void OnUpdateCall(double elapsed);

    void Update(double elapsed);

    void OnDraw(Gdiplus::Graphics* graphics, int width, int height);

    std::shared_ptr<CDisplayImage> FindImage(const std::wstring& file);

    /**
     * Add image to images vector
     * \param image Image to be added
     */
    void AddImage(std::shared_ptr<CDisplayImage> image) { mImages.insert(image); }

    /**
     * Get a boundary of the hero move
     * \param direction Index of the direction to be moved
     * \return mBoundaries[direction] The boudries direction
     */
    int GetBoundary(int direction) { return mBoundaries[direction]; }

    void PickUpItem(std::shared_ptr<CItem> item, CIsCargoVisitor* visitor);

    void DropItem(std::shared_ptr<CItem> item, CIsCargoVisitor* visitor);

    void SwapItem(std::shared_ptr<CItem> item);

    /**
     * Get the game's hero
     * \returns The game's hero
     */
    std::vector<std::shared_ptr<CHero>> GetHeroes() { return mHero; }

      /**
     * Get the game's hero
     * \param idx The index of the hero to get.
     * \returns The game's hero
     */
    std::shared_ptr<CHero> GetHero(int idx = 0) { return mHero[idx]; }

    /**
     * Clears the games list of items
     */
    void ItemClear() { mItems.clear(); }

    void ItemsAdd(std::vector<std::shared_ptr<CItem>> itemVec);

    void OnStartup();

    void OnLevelLoad(int lvlNum);

    /**
     * Get flag of river cheat
     * \returns True if river cheat is active
     */
    bool IsRiverCheatActive() { return mRiverCheatActive; }

    /**
     * Get flag of road cheat
     * \returns True if road cheat is active
     */
    bool IsRoadCheatActive() { return mRoadCheatActive; }

    bool CheckWin();

    void Pause(double time);

    void Win();

    std::shared_ptr<CItem> HitTest(int x, int y);

    bool CheckEats();

    void Accept(CItemVisitor* visitor);

    /**
     * Set RoadCheat to boolean
     * \param truth Value to set mRoadCheatActive to
     */
    void SetRoadCheat(bool truth) { mRoadCheatActive = truth; }

    /** 
     * Set RiverCheat to boolean
     * \param truth Value to set mRiverCheatActive to
     */
    void SetRiverCheat(bool truth) { mRiverCheatActive = truth; }

    /**
     * Tell the game whether or not the victory condition has been completed.
     * \param levelIsWon Whether the level has been won
     **/
    void SetLevelCompletion(bool levelIsWon) { mLevelIsWon = levelIsWon; }


    /**
     * Tell the game whether or not the hero is dead.
     * \param isDead Whether the hero is dead
     **/
    void HeroIsDead(bool isDead) { mHeroDead = isDead; }

    /**
     * Check if the hero is dead.
     * \returns Whether the hero is dead
     **/
    bool IsHeroDead() { return mHeroDead; }

    /**
     * Get Offset in X direction
     * \returns Float of offset in X direction
     */
    float GetOffsetX() { return mXOffset; }

    /**
     * Get Offset in Y direction
     * \returns Float of offset in Y direction
     */
    float GetOffsetY() { return mYOffset; }

    /**
     * Get dilation scale of drawing
     * \returns Scale
     */
    float GetScale() { return mScale; }

    /**
     * Check if the game is paused
     * \returns Whether the game is paused
     **/
    bool IsPaused() { return mPaused; }

    bool CrashCheck();

    void MoveHero(int dir, int idx=0);

    void ResetLevels();

    void OnHeroDie(int method, int idx=0);

    /** Iterator that iterates over the game's items */
    class Iter
    {
    public:
        /** Constructor
         * \param game The game we are iterating over
         * \param pos The position of the iterator */
        Iter(CGame* game, int pos) : mGame(game), mPos(pos) {}

        /** Test for end of the iterator
         * \param  other The other iterator to look at
         * \returns mPos != other.mPos True if we this position equals not equal to the other position */
        bool operator!=(const Iter& other) const
        {
            return mPos != other.mPos;
        }

        /** Get value at current position
         * \returns Value at mPos in the collection */
        std::shared_ptr<CItem> operator *() const { return mGame->mItems[mPos]; }

        /** Increment the iterator
         * \returns Reference to this iterator */
        const Iter& operator++()
        {
            mPos++;
            return *this;
        }


    private:
        CGame* mGame;   ///< Game we are iterating over
        int mPos;       ///< Position in the collection
    };

    /** Get an iterator for the beginning of the collection
     * \returns Iter object at position 0 */
    Iter begin() { return Iter(this, 0); }

    /** Get an iterator for the end of the collection
     * \returns Iter object at position past the end */
    Iter end() { return Iter(this, mItems.size()); }

private:
    /// Scaling coefficient
    float mScale;
    
    /// Offset in the x direction
    float mXOffset;
    /// Offset in the y direction
    float mYOffset;

    ///Constant for Max Elapsed for Anti-Clipping
    double MaxElapsed = 0.3;

    /// Active hero items for game
    std::vector<std::shared_ptr<CHero>> mHero;

    /// Vector of all levels to be loaded into game
    std::vector<std::shared_ptr<CLevel>> mLevels;

    /// All of the tiles that make up our game
    std::vector<std::shared_ptr<CItem> > mItems;

    /// Holds all images to be copied for repeat items
    std::set<std::shared_ptr<CDisplayImage> > mImages;

    /// <summary>
    /// Boundaries for hero movement
    /// Format is {Left, Right, Top, Bottom}
    /// </summary>
    std::vector<int> mBoundaries = { 0, Width - 264, 64, Height - 64}; 

    int mCurrentLevel = 0; ///< The current level of the game

    /**UIText that displays the time*/
    std::shared_ptr<CUIText> mTime;

    bool mRoadCheatActive = false; ///< flag to check if the road cheat is active

    bool mRiverCheatActive = false; ///< flag to check if the river cheat is active
    
    double mTimePaused = 0; ///< the time the timer is paused

    double mPauseDuration = 0; ///< the duration that timer is paused for

    bool mPaused; ///< flag to check if the timer is paused

    bool mLevelIsWon = false; ///< flag to check if the level is won

    bool mHeroDead = false; ///< flag to check if the hero is dead
};

